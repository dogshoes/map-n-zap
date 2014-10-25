/**********************************************************************
   Map-N-Zap v2.x. Interface to the CYE Personal Robot
   Copyright (C) 2000, Probotics, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.Furthermore, this source code
   is distributed without any promise, implied or explicit, of
   support from Probotics, Inc.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.

   Probotics, Inc
   http://www.personalrobots.com
   700 River Ave #223
   Pittsburgh, PA 15212
   USA
 **********************************************************************/

// IPathPlanner.cpp: implementation of the IPathPlanner class.
//
//////////////////////////////////////////////////////////////////////
// uses all the same includes at findpaththread.cpp //

// 8/22/99 -- modified search to grow potential field from start to finish
// instead of the reverse so that a set of goal points can be used at once - Illah

// TO DO
// bug: optimize path length is definitely going forward still.
// looks like the other crystal stuff is buggy. create executable that runs
// the other way around and save it and test side-by-side. check starts and ends
// after all is fixed, then:
// 1) add set of goals to create field and higher functions
// 2) change way it looks for goals by noting them in grid
// 3) change find path entirely so it keeps track of uncertainty and checks robot in

#include "stdafx.h"
#include "Robot.h"
#include "Path.h"
#include "WorldGridConsts.h"
#include "Probability Grid.h"

#ifdef _DEBUG
// #define SHOW_DEBUG_MESSAGES
#endif

#include "IPathPlanner.h"

// extern "C" {
// #define PROTOTYPE
// #include "ateam_opt.h"
// }

enum {cGoalInObstacleErrorNumber = 1, cStartInObstacleErrorNumber, cNoPathToGoalErrorNumber};

// SPECIAL VALUES AND THE BIG GRID ITSELF //
// this number below, times cWorldGridPointSize, is the cellsize of IllahGrid //
const int IGrain = 2; // Illah (so each cell is 4 inches right now)
/* The next number governs search depth. YOu can easily increase this to 10,000.
	Do not exceed 30,000 */
const int searchLimit = 10000; // Illah that's 1000 x 4 inches or 4000 inches linear travel
const int inflation = 80; // in .1 inches, the amount to grow each obstacle
// inflation should be set to 8 inches, the code below provides an additional 2 inches clearance


const int optimizeLimit = 7; // The maximum distance to which the paths should be pushed
// away from walls. 7 would mean 7 x 4 inches = 28 inches (2.? feet)


const int optimizeWindow = 5; // WAS 7 The low-pass filter window size for final smoothing.
// the units of this are the # of grids in the IllahGrid The larger this is, the more
// final smoothing you get, which can cut corner turns somewhat short


const double quickBuffer = 2000.0; // the amount of robotworld buffer area to put
// around the search area (the bigger the more the chance that the shortcut-planner
// will succeed, but the slower it is in the best case...


// globals used for Quick computations. illahoffsetx is the beginning of the
// "expanded" box surrounding the start and goal. illahmaxx is the end of the
// same box.
int illahoffsetx, illahoffsety, illahmaxx, illahmaxy;

// This is the famed, massive IGrid that is the start/goal potential field
int IllahGrid[cWorldGridXSize / IGrain][cWorldGridYSize / IGrain];
// create a new one, to contain obstacle occupancy and distance only.
int ObstacleGrid[cWorldGridXSize / IGrain][cWorldGridYSize / IGrain];
// yet another one, to handle multiple goals for CheckinPoint search.
int GoalGrid[cWorldGridXSize / IGrain][cWorldGridYSize / IGrain];
// one more, to keep track of how far a point is from a
// PATH.
int PathDistanceGrid[cWorldGridXSize / IGrain][cWorldGridYSize / IGrain];

// This is for the plan cache
// Whether or not to use the cache. Set this to zero to disable caching
const int cUseCache = 0;
// Number of paths to store in cache
const int cCacheSize = 1000;
// The cache itself
PathCacheStruct PathCache[cCacheSize];
// The number of entries in the cache
int NumCacheEntries = 0;
// This is used when searching the cache to determine whether the robot's
// current location is near the start or end point of a previously
// cached path. It's in robot units (60 = 6.0 inches).
double Epsilon = 60;

//	This is the minimum probability grid value that the RefreshGrid functions
//  consider to be clear.
const int cMinClearValue = 189;

// simple helper functions for going from crystal directions to values
int getYVal(int x, int y, int dire)
{
    if (dire == 0)
    {
        return y;
    }
    else if (dire == 1)
    {
        return y + 1;
    }
    else if (dire == 2)
    {
        return y;
    }
    else
    {
        return y - 1;
    }
} // getYVal() //


int getXVal(int x, int y, int dire)
{
    if (dire == 0)
    {
        return x + 1;
    }
    else if (dire == 1)
    {
        return x;
    }
    else if (dire == 2)
    {
        return x - 1;
    }
    else
    {
        return x;
    }
} // getXVal() //

// Takes prob grid coordinates, and converts to IllahGrid.
void toIGrid(double x, double y, int &ix, int &iy)
{
    ix = (int)(((x) / (cWorldGridPointSize * IGrain)) +
               cWorldGridXSize / (IGrain * 2));
    iy = (int)(((y) / (cWorldGridPointSize * IGrain)) +
               cWorldGridYSize / (IGrain * 2));
}

#define SGN(x) ((x < 0) ? -1 : 1);


// Here we are going to just do a window around the start and goal position. heh heh. //
// Instead of growing obstacles by a finite amount, we will now grow them until no
// progress is possible.
void IPathPlanner::QuickRefreshIGrid(Path* InitialPath,
                                     bool GoThroughGrey, bool Shortcut)
{
    int j, k, illahj, illahk;
    robPOINT tempPoint1, tempPoint2, tempPoint3, tempPoint4; // Illah
    robPOINT tempPoint5, tempPoint6, tempPoint7, tempPoint8;
    robPOINT tempPoint9, tempPoint10, tempPoint11, tempPoint12;
    int inflateLimit, currentInflate; // Illah
    int gridVal9, gridVal10, gridVal11, gridVal12;

    int IllahStartX, IllahStartY, IllahEndX, IllahEndY;
    robPOINT actualStart;
    robPOINT actualEnd;

    // amount to stay away from grey. Grey is initially seeded with this value.
    // If nogo > greySeed, then grey areas will never be entered. This value can
    // be considered a weight on how likely the robot is to go through a grey
    // area.
    //	int greySeed = 7;
    //	int greySeed = 6;
    int greySeed = 3;	//S.R. 11/12/99 Keep the robot out of the grey.


    InitialPath->GetPoint(0, actualStart.x, actualStart.y);
    InitialPath->GetPoint(InitialPath->GetNumberOfPoints() - 1, actualEnd.x, actualEnd.y);


    // start and goal position in illah coordinates.
    IllahStartX = (int)(((actualStart.x) / (cWorldGridPointSize * IGrain)) + cWorldGridXSize / (IGrain * 2));
    IllahStartY = (int)((actualStart.y) / (cWorldGridPointSize * IGrain) + cWorldGridYSize / (IGrain * 2));
    IllahEndX = (int)((actualEnd.x) / (cWorldGridPointSize * IGrain) + cWorldGridXSize / (IGrain * 2));
    IllahEndY = (int)((actualEnd.y) / (cWorldGridPointSize * IGrain) + cWorldGridYSize / (IGrain * 2));

    double minx, miny, maxx, maxy, offsetx, offsety;

    if (actualEnd.x < actualStart.x)
    {
        minx = actualEnd.x;
        maxx = actualStart.x;
    }
    else
    {
        minx = actualStart.x;
        maxx = actualEnd.x;
    }

    if (actualEnd.y < actualStart.y)
    {
        miny = actualEnd.y;
        maxy = actualStart.y;
    }
    else
    {
        miny = actualStart.y;
        maxy = actualEnd.y;
    }

    // now we add some room for maneuvering!
    if (Shortcut)
    {
        minx = minx - quickBuffer;
        maxx = maxx + quickBuffer;
        miny = miny - quickBuffer;
        maxy = maxy + quickBuffer;
    }
    else
    {
        minx = minx - quickBuffer;
        maxx = maxx + quickBuffer;
        miny = miny - quickBuffer;
        maxy = maxy + quickBuffer;
        /*Search double the area of the shortcut*/
        double expand;
        expand = maxx - minx;
        minx -= expand;
        maxx += expand;
        expand = maxy - miny;
        miny -= expand;
        maxy += expand;
    }

    // the range (minx, maxx) and (miny, maxy) have to be
    // divisible by 40 (the illahgrid size, in robot units),
    // to prevent strange offset problems. - Parag 11/6/99
    // This is essentially like aligning the search area on a
    // dword boundary.
    if (fabs(((maxx) / 40.0) - ceil(maxx / 40)) > 0.0001)
    {
        // need to fix. Find desired difference
        double diff = (ceil((maxx) / 40));
        double diff2 = diff - ((maxx) / 40);
        maxx = maxx + (40 * diff2);
    }

    if (fabs(((minx) / 40.0) - ceil(minx / 40)) > 0.0001)
    {
        // need to fix. Find desired difference
        double diff = (ceil((minx) / 40));
        double diff2 = diff - ((minx) / 40);
        minx = minx + (40 * diff2);
    }

    if (fabs(((maxy) / 40.0) - ceil(maxy / 40)) > 0.0001)
    {
        // need to fix. Find desired difference
        double diff = (ceil((maxy) / 40));
        double diff2 = diff - ((maxy) / 40);
        maxy = maxy + (40 * diff2);
    }

    if (fabs(((miny) / 40.0) - ceil(miny / 40)) > 0.0001)
    {
        // need to fix. Find desired difference
        double diff = (ceil((miny) / 40));
        double diff2 = diff - ((miny) / 40);
        miny = miny + (40 * diff2);
    }


    offsetx = minx - (-cWorldGridXSize * cWorldGridPointSize / 2);
    offsety = miny - (-cWorldGridYSize * cWorldGridPointSize / 2);

    if (offsetx < 0)
    {
        offsetx = 0;
    }

    if (offsety < 0)
    {
        offsety = 0;
    }


    // globals now!  int illahoffsetx, illahoffsety, illahmaxx, illahmaxy;
    illahoffsetx = (int)(offsetx / (cWorldGridPointSize * IGrain));
    illahoffsety = (int)(offsety / (cWorldGridPointSize * IGrain));
    illahmaxx = (int)(((maxx) / (cWorldGridPointSize * IGrain)) + cWorldGridXSize / (IGrain * 2));
    illahmaxy = (int)(((maxy) / (cWorldGridPointSize * IGrain)) + cWorldGridYSize / (IGrain * 2));
    // now we just limit everything by these values! //


    // for (j=0+illahoffsetx; ((j< cWorldGridXSize/IGrain) && (j<illahmaxx)); j++)
    //	for (k=0+illahoffsety; ((k< cWorldGridYSize/IGrain) && (k<illahmaxy)); k++)
    //		ObstacleGrid[j][k]=0;
    memset(ObstacleGrid, 0, (int)(sizeof(int) * (cWorldGridXSize / IGrain) * (cWorldGridYSize / IGrain)));

    // Now we populate IGrid using the probability grid
    //int xll= (-cWorldGridXSize*cWorldGridPointSize/2)+
    //					cWorldGridPointSize*(IGrain - 1) + offsetx;
    int xl = (int)((-cWorldGridXSize * cWorldGridPointSize / 2) +
                   cWorldGridPointSize * IGrain + offsetx);
    int xr = (int)((-cWorldGridXSize * cWorldGridPointSize / 2) +
                   (cWorldGridPointSize * (IGrain - 1)) +
                   cWorldGridPointSize * IGrain + offsetx);
    int xrr = (int)((-cWorldGridXSize * cWorldGridPointSize / 2) +
                    (cWorldGridPointSize * (IGrain - 1)) +
                    cWorldGridPointSize * IGrain +
                    cWorldGridPointSize + offsetx);
    //int ydd=(-cWorldGridYSize*cWorldGridPointSize/2)+
    //						cWorldGridPointSize*(IGrain-1) + offsety;
    int yd = (int)((-cWorldGridYSize * cWorldGridPointSize / 2) +
                   cWorldGridPointSize * IGrain + offsety);
    int yu = (int)((-cWorldGridYSize * cWorldGridPointSize / 2) +
                   (cWorldGridPointSize * (IGrain - 1)) +
                   cWorldGridPointSize * IGrain + offsety);
    int yuu = (int)((-cWorldGridYSize * cWorldGridPointSize / 2) +
                    (cWorldGridPointSize * (IGrain - 1)) +
                    cWorldGridPointSize * IGrain +
                    cWorldGridPointSize + offsety);
    int increm = (int)(cWorldGridPointSize * IGrain);
    // Okay. What this does it, it runs though the entire world grid in increments,
    // deciding how to fill in obstacles in the more granular ObstacleGrid
    // Designed for efficiency not readability!
    // by the way, for the subsampling, it samples the four 'corners' of meta-cells
    int ProgressIndicator = 0;
    int ProgressPercent = 0;

    for (//tempPoint1.x=xll, tempPoint2.x=xr, tempPoint3.x=xrr, tempPoint4.x=xl,
        //tempPoint5.x=xl, tempPoint6.x=xrr, tempPoint7.x=xr, tempPoint8.x=xll,
        tempPoint3.x = xrr,
        tempPoint9.x = xl, tempPoint10.x = xl, tempPoint11.x = xr, tempPoint12.x = xr,
        illahj = 1 + illahoffsetx;
        ((tempPoint3.x < cWorldGridXSize * cWorldGridPointSize / 2) &&
         (tempPoint3.x < maxx)); // IIIIIllAHHHH
        //tempPoint1.x += increm,
        //tempPoint2.x += increm,
        tempPoint3.x += increm,
        //tempPoint4.x += increm,
        //tempPoint5.x += increm,
        //tempPoint6.x += increm,
        //tempPoint7.x += increm,
        //tempPoint8.x += increm,
        tempPoint9.x += increm,
        tempPoint10.x += increm,
        tempPoint11.x += increm,
        tempPoint12.x += increm,
        illahj++)
    {
        for (//tempPoint1.y=yd,tempPoint2.y=ydd,tempPoint3.y=yu,tempPoint4.y=yuu,
            //tempPoint5.y=ydd,tempPoint6.y=yd,tempPoint7.y=yuu,tempPoint8.y=yu,
            tempPoint4.y = yuu,
            tempPoint9.y = yu, tempPoint10.y = yd, tempPoint11.y = yd, tempPoint12.y = yu,
            illahk = 1 + illahoffsety;
            ((tempPoint4.y < cWorldGridYSize * cWorldGridPointSize / 2) &&
             (tempPoint4.y < maxy)); // IIIILLLLAAAHHH
            //tempPoint1.y += increm,
            //tempPoint2.y += increm,
            //tempPoint3.y += increm,
            tempPoint4.y += increm,
            //tempPoint5.y += increm,			 //tempPoint6.y += increm,
            //tempPoint7.y += increm,
            //tempPoint8.y += increm,
            tempPoint9.y += increm,
            tempPoint10.y += increm,
            tempPoint11.y += increm,
            tempPoint12.y += increm,
            illahk++)
        {

            //gridVal1 = m_ProbGrid->GetGridValue(tempPoint1);
            //gridVal2 = m_ProbGrid->GetGridValue(tempPoint2);
            //gridVal3 = m_ProbGrid->GetGridValue(tempPoint3);
            //gridVal4 = m_ProbGrid->GetGridValue(tempPoint4);
            //gridVal5 = m_ProbGrid->GetGridValue(tempPoint5);
            //gridVal6 = m_ProbGrid->GetGridValue(tempPoint6);
            //gridVal7 = m_ProbGrid->GetGridValue(tempPoint7);
            //gridVal8 = m_ProbGrid->GetGridValue(tempPoint8);
            gridVal9 = m_ProbGrid->GetGridValue(tempPoint9);
            gridVal10 = m_ProbGrid->GetGridValue(tempPoint10);
            gridVal11 = m_ProbGrid->GetGridValue(tempPoint11);
            gridVal12 = m_ProbGrid->GetGridValue(tempPoint12);

            // in the probgrid, 0 is obstacle; 255 is clear //
            if (//(gridVal1 < 127) || (gridVal2 < 127) ||
                //(gridVal3 < 127) || (gridVal4 < 127) ||
                //(gridVal5<127) || (gridVal6<127) || (gridVal7<127) || (gridVal8<127) ||
                (gridVal9 < 127) || (gridVal10 < 127) ||
                (gridVal11 < 127) || (gridVal12 < 127))
            {
                // there's almost a definite obstacle here
                ObstacleGrid[illahj][illahk] = 1; // make it positive - Parag
            }
            else if (  //(gridVal1 < 250) || (gridVal2 < 250) ||
                //(gridVal3 < 250) || (gridVal4 < 250) ||
                //(gridVal5<250) || (gridVal6<250) || (gridVal7<250) || (gridVal8<250) ||
                (gridVal9 < cMinClearValue) || (gridVal10 < cMinClearValue) ||
                (gridVal11 < cMinClearValue) || (gridVal12 < cMinClearValue)) // unknown
            {
                if (!GoThroughGrey)
                {
                    ObstacleGrid[illahj][illahk] = greySeed; // Parag
                }
            } // else if
        } // for (int k

        ProgressIndicator++;

        if (ProgressIndicator >= int((maxx - xrr) / (3 * increm)))
        {
            ProgressIndicator = 0;
            ProgressPercent ++;
            PostMessage(m_ParentWindowHandle, ID_ROBOT_FIND_PATH_STAGE, 4, ProgressPercent);
        }
    } // for (int j


    // Check to see if the start or end point is contained in an obstacle field //
    if (ObstacleGrid[IllahEndX][IllahEndY] == 1)   // Parag
    {
        PostMessage(m_ParentWindowHandle, ID_ROBOT_FIND_PATH_ERROR, cGoalInObstacleErrorNumber, 0);
        Sleep(1000);
#ifdef SHOW_DEBUG_MESSAGES
        //		AfxMessageBox("PROBLEM: Goal is contained in an obstacle or an obstacle inflation area. No can do.");
#endif
        return;
    }

    if (ObstacleGrid[IllahStartX][IllahStartY] == 1)   // Parag
    {
        PostMessage(m_ParentWindowHandle, ID_ROBOT_FIND_PATH_ERROR, cStartInObstacleErrorNumber, 0);
        Sleep(1000);
#ifdef SHOW_DEBUG_MESSAGES
        //		AfxMessageBox("PROBLEM: Robot start point is contained in obstacle or obs inflation. No can do.");
#endif
        return;
    }

    // Now we put down a dummy number at start and goal so we don't clobber them with
    // the inflation process
    ObstacleGrid[IllahStartX][IllahStartY] = 300000;
    ObstacleGrid[IllahEndX][IllahEndY] = 300000;


    // now we do obstacle inflation by doing potential cell growth from -1 out
    inflateLimit = (int)(1 * ((inflation / (cWorldGridPointSize * IGrain)) + 1));
    // inflateLimit is how many cells we need to inflate out ObstacleGrid for obstacles
    int progress = 1;

    currentInflate = 2;

    // while (currentInflate >= inflateLimit) {
    while (progress)
    {
        int mycntr = 0;
        progress = 0;

        for (j = 0 + illahoffsetx; ((j < cWorldGridXSize / IGrain) &&
                                    (j < illahmaxx)); j++)
        {
            for (k = 0 + illahoffsety; ((k < cWorldGridYSize / IGrain) &&
                                        (k < illahmaxy)); k++)
            {
                // check to see if there is an obstacle pixel here. An obstacle pixel
                // is either a -1 or less than -1, as set by this routine.
                if (ObstacleGrid[j][k] == currentInflate - 1)
                {
                    // if left of current pos is either empty, or contains an obstacle
                    // value which is *more* positive than the current inflate value,
                    // then replace is with current inflate. Therefore, each cell is
                    // guaranteed to contain the distance to the closest obstacle.
                    // If a neighboring cell contains a -30000, then we make it negative,
                    // so that QuickCreateModifiedField() can use that information.
                    int temp1;

                    temp1 = ObstacleGrid[j - 1][k];

                    if ((j != 0) && ((temp1 == 0) ||
                                     (temp1 > currentInflate)))
                    {
                        ObstacleGrid[j - 1][k] = currentInflate;
                        progress = 1;
                    }

                    // Same for right
                    temp1 = ObstacleGrid[j + 1][k];

                    if ((j < cWorldGridXSize / IGrain - 1) &&
                            ((temp1 == 0) ||
                             (temp1 > currentInflate)))
                    {
                        ObstacleGrid[j + 1][k] = currentInflate;
                        progress = 1;
                    }

                    // Now below
                    temp1 = ObstacleGrid[j][k - 1];

                    if ((k != 0) && ((temp1 == 0) ||
                                     (temp1 > currentInflate)))
                    {
                        ObstacleGrid[j][k - 1] = currentInflate;
                        progress = 1;
                    }

                    // Now above
                    temp1 = ObstacleGrid[j][k + 1];

                    if ((k < cWorldGridYSize / IGrain - 1) &&
                            ((temp1 == 0) ||
                             (temp1 > currentInflate)))
                    {
                        ObstacleGrid[j][k + 1] = currentInflate;
                        progress = 1;
                    }

                    // bottom left
                    temp1 = ObstacleGrid[j - 1][k - 1];

                    if ((j != 0) && (k != 0) &&
                            ((temp1 == 0) ||
                             (temp1 > currentInflate)))
                    {
                        ObstacleGrid[j - 1][k - 1] = currentInflate;
                        progress = 1;
                    }

                    // top left
                    temp1 = ObstacleGrid[j - 1][k + 1];

                    if ((j != 0) && (k < cWorldGridYSize / IGrain - 1) &&
                            ((temp1 == 0) ||
                             (temp1 > currentInflate)))
                    {
                        ObstacleGrid[j - 1][k + 1] = currentInflate;
                        progress = 1;
                    }

                    // bottom right
                    temp1 = ObstacleGrid[j + 1][k - 1];

                    if ((j < cWorldGridXSize / IGrain - 1) && (k != 0) &&
                            ((temp1 == 0) ||
                             (temp1 > currentInflate)))
                    {
                        ObstacleGrid[j + 1][k - 1] = currentInflate;
                        progress = 1;
                    }

                    // top right
                    temp1 = ObstacleGrid[j + 1][k + 1];

                    if ((j < cWorldGridXSize / IGrain - 1) &&
                            (k < cWorldGridYSize / IGrain - 1) &&
                            ((temp1 == 0) ||
                             (temp1 > currentInflate)))
                    {
                        ObstacleGrid[j + 1][k + 1] = currentInflate;
                        progress = 1;
                    }

                    if ((j == IllahStartX) && (k == IllahStartY))
                    {
                        char buf[100];
                        sprintf(buf, "Obstacle Value at end point: %d\n\0",
                                ObstacleGrid[j][k]);
                        //	AfxMessageBox(buf);
                    }
                } // else if (ObstacleGrid...
            } // for (k
        } // for (j

        currentInflate++;
    } // while (currentInflate

    // return the values of the start and goal positions to 0 (temporarily were 1)
    // ObstacleGrid[IllahStartX][IllahStartY] = 0;
    // ObstacleGrid[IllahEndX][IllahEndY] = 0;

    // write temporary file
    /*	FILE *output;
    	output = fopen("obs.dat", "wa");
    	for (j=0 + illahoffsetx; ((j < cWorldGridXSize/IGrain) &&
    							(j < illahmaxx)); j++) {
    			for (k=0 + illahoffsety; ((k<cWorldGridYSize/IGrain) &&
    										(k<illahmaxy)); k++) {
    				fprintf(output, "%2d ", ObstacleGrid[j][k]);
    			}
    			fprintf(output, "\n");
    	}
    	fclose(output); */


    // IGrid contains at this point: 0's, -1, -2, -3 and -30000
    // 0 is no obstacle, -1 is obstacle, and -2 and -3 are "grown"
    // portions of obstacles. -30000 is gray area, and is not grown.

} // QuickRefreshIGrid() //





// After using the IGrid, this function MUST be called to return the
// grid to its proper condition to be used again. If you call this function,
// and if you think obstacle in the probgrid haven't moved, then you don't
// need to call RefreshGrid and you'll save a lot of time. If you do want to
// call RefreshGrid, then don't bother with this function. This destroys
// the distinction of gray areas and forever marks them as obstacles? - Parag


void IPathPlanner::CleanupGrid()
{
    int j, k;

    memset(IllahGrid, 0,
           sizeof(int) * (cWorldGridXSize / IGrain) * (cWorldGridYSize / IGrain));
    return;


    for (j = 0; j < (cWorldGridXSize / IGrain); j++)
    {
        for (k = 0; k < (cWorldGridYSize / IGrain); k++)
        {
            IllahGrid[j][k] = 0;
        }
    }

    return;
} // CleanupGrid


// modification of crystallizepath for limited scope operation.
// Before entering this function, IGrid contains negative numbers indicating
// grown obstacles, and positivie numbers indicating the path.
void IPathPlanner::QuickCrystallizePath(int IllahStartX, int IllahStartY,
                                        int IllahEndX, int IllahEndY)
{
    Path *FoundPath = NULL;


    // First we replace the actual path with a token
    ReplacePath(IllahEndX, IllahEndY, 30000);
    return; // - Parag - Don't need to grow obstacles anymore
    // 'nogo' variable takes care of min distance to obstacle.


    // get rid of all positive numbers! //
    // CleanupGrid();
    // Now, IGrid contains 30000 along the path, -1 at obstacles and gray areas,
    // and 0 everywhere else.


    // Now we do obstacle growth but turn only the 30000's into positive sign, same
    // magnitude (to complete the crystallization process)


    // now we do obstacle inflation by doing potential cell growth from -1 out
    // until no more inflation is possible

    int progress = 1;
    int currentInflate;
    /*int inflateLimit;
    inflateLimit = -1 * ((inflation / (cWorldGridPointSize*IGrain)) + 1);
    */

    int j, k;
    //currentInflate = inflateLimit - 1;  // -4 in other words
    currentInflate = -2; // just ask.

    while (progress == 1)
    {
        //if (currentInflate > -4) { progress = 1; }

        /*	FoundPath = GeneratePathFromCrystal(IllahStartX,IllahStartY, IllahEndX, IllahEndY,
        							actualStart, actualEnd);


        	CFindPathThread::ShowPath(FoundPath, 1); */


        // Sleep(500);

        // This code grows the obstacles. At the end, each point contains the min
        // distance from an obstacle (represented as a negative number) IFF it is
        // not a path point. If it is a path point, then it contains a positive number
        // which represents the distance from the nearest obstacle. This will be useful
        // later for the "min distance from wall" optimization. Are the obstacle and
        // gray areas grown? - Parag
        progress = 0;
        int ProgressIndicator = 0;
        int ProgressPercent = 0;

        for (j = 0 + illahoffsetx; ((j < cWorldGridXSize / IGrain) &&
                                    (j < illahmaxx)); j++)
        {
            for (k = 0 + illahoffsety; ((k < cWorldGridYSize / IGrain) &&
                                        (k < illahmaxy)); k++)
            {
                if (ABS(IllahGrid[j][k]) == ABS(currentInflate + 1))
                {
                    if ((j != 0) && ((IllahGrid[j - 1][k] == 0) ||
                                     (ABS(IllahGrid[j - 1][k]) > ABS(currentInflate))))
                    {
                        progress = 1;

                        if (IllahGrid[j - 1][k] == 30000)
                        {
                            IllahGrid[j - 1][k] = ABS(currentInflate);
                        }
                        else
                        {
                            IllahGrid[j - 1][k] = currentInflate;
                        }
                    }  // end if

                    if ((j < cWorldGridXSize / IGrain - 1) &&
                            ((IllahGrid[j + 1][k] == 0) ||
                             (ABS(IllahGrid[j + 1][k]) > ABS(currentInflate))))
                    {
                        progress = 1;

                        if (IllahGrid[j + 1][k] == 30000)
                        {
                            IllahGrid[j + 1][k] = ABS(currentInflate);
                        }
                        else
                        {
                            IllahGrid[j + 1][k] = currentInflate;
                        }
                    } // endif

                    if ((k != 0) && ((IllahGrid[j][k - 1] == 0) ||
                                     (ABS(IllahGrid[j][k - 1]) > ABS(currentInflate))))
                    {
                        progress = 1;

                        if (IllahGrid[j][k - 1] == 30000)
                        {
                            IllahGrid[j][k - 1] = ABS(currentInflate);
                        }
                        else
                        {
                            IllahGrid[j][k - 1] = currentInflate;
                        }
                    } // endif

                    if ((k < cWorldGridYSize / IGrain - 1) &&
                            ((IllahGrid[j][k + 1] == 0) ||
                             (ABS(IllahGrid[j][k + 1]) > ABS(currentInflate))))
                    {
                        progress = 1;

                        if (IllahGrid[j][k + 1] == 30000)
                        {
                            IllahGrid[j][k + 1] = ABS(currentInflate);
                        }
                        else
                        {
                            IllahGrid[j][k + 1] = currentInflate;
                        }
                    } // endif

                    if ((j != 0) && (k != 0) &&
                            ((IllahGrid[j - 1][k - 1] == 0) ||
                             (ABS(IllahGrid[j - 1][k - 1]) > ABS(currentInflate))))
                    {
                        progress = 1;

                        if (IllahGrid[j - 1][k - 1] == 30000)
                        {
                            IllahGrid[j - 1][k - 1] = ABS(currentInflate);
                        }
                        else
                        {
                            IllahGrid[j - 1][k - 1] = currentInflate;
                        }
                    }

                    if ((j != 0) && (k < cWorldGridYSize / IGrain - 1) &&
                            ((IllahGrid[j - 1][k + 1] == 0) ||
                             (ABS(IllahGrid[j - 1][k + 1]) > ABS(currentInflate))))
                    {
                        progress = 1;

                        if (IllahGrid[j - 1][k + 1] == 30000)
                        {
                            IllahGrid[j - 1][k + 1] = ABS(currentInflate);
                        }
                        else
                        {
                            IllahGrid[j - 1][k + 1] = currentInflate;
                        }
                    }

                    if ((j < cWorldGridXSize / IGrain - 1) && (k != 0) &&
                            ((IllahGrid[j + 1][k - 1] == 0) ||
                             (ABS(IllahGrid[j + 1][k - 1]) > ABS(currentInflate))))
                    {
                        progress = 1;

                        if (IllahGrid[j + 1][k - 1] == 30000)
                        {
                            IllahGrid[j + 1][k - 1] = ABS(currentInflate);
                        }
                        else
                        {
                            IllahGrid[j + 1][k - 1] = currentInflate;
                        }
                    }

                    if ((j < cWorldGridXSize / IGrain - 1) &&
                            (k < cWorldGridYSize / IGrain - 1) &&
                            ((IllahGrid[j + 1][k + 1] == 0) ||
                             (ABS(IllahGrid[j + 1][k + 1]) > ABS(currentInflate))))
                    {
                        progress = 1;

                        if (IllahGrid[j + 1][k + 1] == 30000)
                        {
                            IllahGrid[j + 1][k + 1] = ABS(currentInflate);
                        }
                        else
                        {
                            IllahGrid[j + 1][k + 1] = currentInflate;
                        }
                    }
                } // else if (IllahGrid...
            } // for (k

            ProgressIndicator++;

            if (ProgressIndicator >= int((illahmaxx - illahoffsetx) / 3))
            {
                ProgressPercent++;
                ProgressIndicator = 0;
                PostMessage(m_ParentWindowHandle, ID_ROBOT_FIND_PATH_STAGE, 4, ProgressPercent);
            }
        } // for (j

        currentInflate--;
    } // while (progress


    //sprintf(myString,"Crystallize done with maximum inflate of %d",currentInflate);
    //AfxMessageBox(myString);


} // QuickCrystallizePath()






/* assuming you have a potential field, this replaces the actual path with newVal */
// 7/9: changed to prefer shortest diagonals! //
// 7/22: modified for progression (start from end not start) rather than regression srch

// walk backwards, starting at the goal, following decreasing numbers based on the
// potential field. As this is going on, replace the path with newVal.
// At the end, Igrid will have positive numbers indicating potential (distance from start),
// and 30000 indicating the path.
void IPathPlanner::ReplacePath(int endX, int endY, int newVal)
{
    int lastdir = 0; // last direction (avoid same direction again!)
    int illahj, illahk, curField;
    // set goal point
    illahj = endX;
    illahk = endY;
    curField = IllahGrid[illahj][illahk];
    IllahGrid[illahj][illahk] = newVal;
    int val1, val2, val3, val4;
    int bestval;
    val1 = val2 = val3 = val4 = 100000;
    bestval = 100000;

    while (curField != 1)
    {
        val1 = val2 = val3 = val4 = 100000;
        bestval = 100000;

        if ((lastdir == 0) || (lastdir == 2))
        {
            // now, roundup the suspects, execpt lastdir.
            if (illahk != 0)
            {
                bestval = IllahGrid[illahj][illahk - 1];

                if (bestval != 100000)
                {
                    lastdir = 3;
                }
            }

            if (illahk < cWorldGridYSize / IGrain - 1)
            {
                val2 = IllahGrid[illahj][illahk + 1];

                if (val2 < bestval)
                {
                    lastdir = 1;
                    bestval = val2;
                }
            }

            if (illahj != 0)
            {
                val3 = IllahGrid[illahj - 1][illahk];

                if (val3 < bestval)
                {
                    lastdir = 2;
                    bestval = val3;
                }
            }

            if (illahj < cWorldGridXSize / IGrain - 1)
            {
                val4 = IllahGrid[illahj + 1][illahk];
            }

            {
                if (val4 < bestval)
                {
                    lastdir = 0;
                    bestval = val4;
                }
            }

            if (bestval == 100000)
            {
                AfxMessageBox("BIG WACKINESS DURING PATH RECONSTRUCTION");
                exit(1);
            }

            // now find the lowest, to see what 'lastdir' should be, and
            // what direction we move in.

            switch (lastdir)
            {
            case 0:
                illahj++;
                break;

            case 1:
                illahk++;
                break;

            case 2:
                illahj--;
                break;

            case 3:
                illahk--;
                break;

            default:
            {
                AfxMessageBox("STRANGENESS IN CASE STATEMENT - NO LASTDIR");
                exit(1);
            }
            }
        }
        else
        {
            if (illahj != 0)
            {
                bestval = IllahGrid[illahj - 1][illahk];

                if (bestval != 100000)
                {
                    lastdir = 2;
                }
            }

            if (illahj < cWorldGridXSize / IGrain - 1)
            {
                val2 = IllahGrid[illahj + 1][illahk];

                if (val2 < bestval)
                {
                    lastdir = 0;
                    bestval = val2;
                }
            }

            if (illahk != 0)
            {
                val3 = IllahGrid[illahj][illahk - 1];

                if (val3 < bestval)
                {
                    lastdir = 3;
                    bestval = val3;
                }
            }

            if (illahk < cWorldGridYSize / IGrain - 1)
            {
                val4 = IllahGrid[illahj][illahk + 1];

                if (val4 < bestval)
                {
                    lastdir = 1;
                    bestval = val4;
                }
            }

            if (bestval == 100000)
            {
                AfxMessageBox("BIG WACKINESS 2 DURING PATH RECONSTRUCTION");
                exit(1);
            }

            switch (lastdir)
            {
            case 0:
                illahj++;
                break;

            case 1:
                illahk++;
                break;

            case 2:
                illahj--;
                break;

            case 3:
                illahk--;
                break;

            default:
            {
                AfxMessageBox("STRANGENESS IN CASE STATEMENT 2 - NO LASTDIR");
                exit(1);
            }
            }
        } // endelse

        IllahGrid[illahj][illahk] = newVal;
        curField = bestval;
    } // end while (curField != 1)

    // at this point, actual path has been replaced with newVal everywhere! //

    /*		int j,k;
    	FILE *output;
    	output = fopen("path.txt", "wa");
    	for (j=0 + illahoffsetx; ((j < cWorldGridXSize/IGrain) &&
    							(j < illahmaxx)); j++) {
    			for (k=0 + illahoffsety; ((k<cWorldGridYSize/IGrain) &&
    										(k<illahmaxy)); k++) {
    				if (IllahGrid[j][k] == 10000)
    				{
    					fprintf(output, "0    ");
    				}
    				else if (IllahGrid[j][k] == newVal)
    				{
    					fprintf(output, "*    ");
    				}
    				else
    				{
    					fprintf(output, "%4d ", IllahGrid[j][k]);
    				}
    			}
    			fprintf(output, "\n");
    	}
    	fclose(output); */


} // ReplacePath()



Path* IPathPlanner::GeneratePathFromCrystal(int beginAtx, int beginAty,
        int endAtx, int endAty,
        robPOINT end1, robPOINT end2)
{
    Path* FoundPath = NULL;
    int illahj, illahk, curField, lastx, lasty;
    illahj = endAtx;
    illahk = endAty;
    curField = IllahGrid[illahj][illahk];

    if (curField < 0)
    {
#ifdef SHOW_DEBUG_MESSAGES
        AfxMessageBox("BIG Error in GeneratePathFromCrystal");
#endif
    }

    // look for adjacent positive number, to see which direction to go
    lastx = illahj;
    lasty = illahk;

    if ((illahj != 0) && (IllahGrid[illahj - 1][illahk] == 30000))
    {
        illahj--;
        //AfxMessageBox("initialleft");
    }
    else if ((illahj < cWorldGridXSize / IGrain - 1) && (IllahGrid[illahj + 1][illahk] == 30000))
    {
        illahj++;
        //AfxMessageBox("initialright");
    }
    else if ((illahk != 0) && (IllahGrid[illahj][illahk - 1] == 30000))
    {
        illahk--;
        //AfxMessageBox("Initialdown");
    }
    else if ((illahk < cWorldGridYSize / IGrain - 1) && (IllahGrid[illahj][illahk + 1] = 30000))
    {
        illahk++;
        //AfxMessageBox("Initialup");
    }
    else
    {
#ifdef SHOW_DEBUG_MESSAGES
        AfxMessageBox("BIG ERROR 1 DURING PATH RECONSTRUCTION");
#endif
    } // endelse

    FoundPath = new Path(m_ProbGrid->m_RobotWorld,
                         end2.x, end2.y,
                         illahj * cWorldGridPointSize * IGrain - cWorldGridPointSize * cWorldGridXSize / 2,
                         illahk * cWorldGridPointSize * IGrain - cWorldGridPointSize * cWorldGridYSize / 2);
    int progress = 1;

    while (progress == 1)
    {
        //sprintf(myString,"Field at current point is %d", IllahGrid[illahj][illahk]);
        //AfxMessageBox(myString);

        // the sign of the number represents the path (positive if path), and the
        // magnitude represents the distance from nearest obstacle. Therefore,
        // we have to make sure that we do not double back on ourselves when
        // tracking the path, hence storing lastx and lasty. - Parag
        progress = 0;

        if ((illahj != 0) && (IllahGrid[illahj - 1][illahk] == 30000) &&
                ((illahj - 1 != lastx) || (illahk != lasty)))
        {
            progress = 1;
            lastx = illahj;
            lasty = illahk;
            illahj--;
            //		AfxMessageBox("left");
        }
        else if ((illahj < cWorldGridXSize / IGrain - 1) &&
                 (IllahGrid[illahj + 1][illahk] == 30000) &&
                 ((illahj + 1 != lastx) || (illahk != lasty)))
        {
            progress = 1;
            lastx = illahj;
            lasty = illahk;
            illahj++;
            //		AfxMessageBox("right");
        }
        else if ((illahk != 0) && (IllahGrid[illahj][illahk - 1] == 30000) &&
                 ((illahj != lastx) || (illahk - 1 != lasty)))
        {
            progress = 1;
            lastx = illahj;
            lasty = illahk;
            illahk--;
            //		AfxMessageBox("down");
        }
        else if ((illahk < cWorldGridYSize / IGrain - 1) &&
                 (IllahGrid[illahj][illahk + 1] == 30000) &&
                 ((illahj != lastx) || (illahk + 1 != lasty)))
        {
            progress = 1;
            lastx = illahj;
            lasty = illahk;
            illahk++;
            //		AfxMessageBox("up");
        }
        else
        {
            ; //AfxMessageBox("zero progress during crystal reconstruction path");
        } // endelse

        FoundPath->AppendPoint(illahj * cWorldGridPointSize * IGrain -
                               cWorldGridPointSize * cWorldGridXSize / 2,
                               illahk * cWorldGridPointSize * IGrain -
                               cWorldGridPointSize * cWorldGridYSize / 2);
    } // end while (progress == 1)


    // Now we change the final goal point to be the original PathInfo point
    FoundPath->SetPoint(FoundPath->GetNumberOfPoints() - 1,
                        end1.x, end1.y);

    FoundPath->Reverse(); // 8/22/99 added because of progression not regression Illah

    return FoundPath;
} // ModifiedGeneratePathFromCrystalField() //




/* now we optimize the path by doing as many segment merges as possible
	without going under the minimum quality measurement.  For the viewer's
	pleasure, I draw during this part; but that's not critical */
// This function now limits its fusion to optimizeWindow cells //
// we are now outside igrid land and are in the Path land, so we have
// to use GetQualities and cannot use our own potential field. waan.

void IPathPlanner::OptimizePathLength(Path* FoundPath)
{
    int currentCandidate = 0;
    int numinarow = 0;
    int beforeQuality, afterQuality;
    int numSegs = FoundPath->GetNumberOfSegments();
    double removedX, removedY, candX, candY, termX, termY;
    int showCounter = 0;
    int i;

    // candX/Y are the start points of the segment
    // removedX/Y are the original end points of the segment
    // termX/Y are the new endpoints of the segment
    CFindPathThread::ShowPath(FoundPath, 1);
    Sleep(500);


    //			World *world = FoundPath->GetWorld();
    //			double theNumber;
    //			world->roverpath_step_size = 400.0;
    //			theNumber = world->rover.PathStepSize();
    //			sprintf(myString,"the number is: %d", (int)theNumber);
    //			AfxMessageBox(myString);


    while (currentCandidate < numSegs - 1)
    {
        if (CFindPathThread::bStopFindPath)
        {
            return;
        }

        // store the point we're going to remove //

        FoundPath->GetPoint(currentCandidate + 1, removedX, removedY);
        FoundPath->GetPoint(currentCandidate, candX, candY);
        beforeQuality = (int)(FoundPath->GetSegment(currentCandidate)->GetQuality(MinFound, true));
        FoundPath->JoinSegment(currentCandidate);
        FoundPath->GetPoint(currentCandidate + 1, termX, termY);
        showCounter++;

        if ((showCounter % 5) == 0)
        {
            CFindPathThread::ShowPath(FoundPath, 1);
        }

        numSegs--;
        afterQuality = (int)(FoundPath->GetSegment(currentCandidate)->GetQuality(MinFound, true));

        //sprintf(myString,"Proposed change has quality %d compare before %d",
        //		afterQuality, beforeQuality);
        //AfxMessageBox(myString);
        if (numinarow > optimizeWindow)
        {
            afterQuality = -1; // force window size to be maximum fusion size
        }

        if ((afterQuality < cVeryClearPathMinValue) &&
                ((afterQuality < beforeQuality) || ((beforeQuality < 5) &&
                        (afterQuality <= beforeQuality))) &&
                (!(((candX == removedX) && (removedX == termX)) ||
                   ((candY == removedY) && (removedY == termY)))))
        {
            // if quality is bad (worse than before) and this isn't a straight-line
            // merge updown or leftright, then undo the merge
            FoundPath->InsertPoint(currentCandidate + 1, removedX, removedY);
            currentCandidate++;
            numSegs++;
            numinarow = 0;
        }
        else
        {
            numinarow++;
        } // end else
    } // end while (currentCand..

    // AfxMessageBox("Optimization of path is complete");


} // OptimizePathLength() //




int IPathPlanner::QuickCreateField(int IllahStartX, int IllahStartY,
                                   int IllahEndX, int IllahEndY)
{
    // This routine creats a potential field based on the start and goal points,
    // using the obstacle grid to properly weight the potential field, to try
    // to remain 'n' spaces away from a gray area or wall.
    // Parag Batavia

    int i, j, k;
    bool t1, t2, t3;
    int NewWorldXSize, NewWorldYSize, NewMaxX, NewMaxY;
    int NewOffsetX, NewOffsetY;
    int curField, atGoal, progress;
    int ObstacleVal;
    int MinProgress;
    bool *fieldArray;
    int newVal;
    int powerarray[20];
    int powerbase = 3;

    // nogo is a min distance to keep from obstacles. I.e., the robot will never
    // go to an area which is only 'nogo' grid points away from an obstacle, based
    // on ObstacleGrid.
    int nogo = 2;

    // If a point is within 'minObstacleVal' of an obstacle, then add a penalty,
    // proportional to the closeness of the point to the obstacle to the IllahGrid.
    // This makes it harder for a robot to get near an obstacle, and will cause it
    // to prefer paths which are further away.
    int minObstacleVal = 6;

    // set up the powerarray
    for (i = 0; i < 20; i++)
    {
        powerarray[i] = (int)pow((long double)powerbase, i);
    }

    int stuffValue = 10000;

    fieldArray = new bool[searchLimit];
    memset(fieldArray, 0, sizeof(bool)*searchLimit);

    // clear out the IllahGrid.
    //	for (j=0+illahoffsetx; ((j< cWorldGridXSize/IGrain) && (j<illahmaxx)); j++)
    //		for (k=0+illahoffsety; ((k< cWorldGridYSize/IGrain) && (k<illahmaxy)); k++)
    for (j = 0; (j < cWorldGridXSize / IGrain); j++)
        for (k = 0; (k < cWorldGridYSize / IGrain); k++)
        {
            IllahGrid[j][k] = stuffValue;
        }

    // make the starting point in IllahGrid 1 (the only 1 in the whole thing)
    // the search starting point is, by the way, the *start* (progression search)
    IllahGrid[IllahStartX][IllahStartY] = 1;

    // Now, we populate the potential field as follows:
    // First, for each point, check to see if it is the goal. If so, then
    // set atGoal=1 and return. If not, figure out it's value. This is done
    // by incrementing curField, and *adding* a factor based on the corresponding
    // obstacle grid. So if the point is near an obstacle, then increase the
    // the grid value, if the distance is greater than minObstacleDistance,
    // then just increment by one.

    curField = 1;
    fieldArray[curField] = true;
    atGoal = 0;

    // Set these here because they are unchanging. We don't want to
    // recalculate these each time through the loop.
    NewOffsetX = illahoffsetx + 1;
    NewOffsetY = illahoffsety + 1;
    NewWorldXSize = cWorldGridXSize / IGrain;
    NewWorldYSize = cWorldGridYSize / IGrain;
    NewMaxX = MIN(illahmaxx - 1, NewWorldXSize);
    NewMaxY = MIN(illahmaxy - 1, NewWorldYSize);

    MinProgress = (int)(-(1 + (int)pow((long double)3, (minObstacleVal) + 20)));

    progress = 0; // if this stays at zero, there is no path

    while (atGoal == 0)
    {
        if (CFindPathThread::bStopFindPath)
        {
            return 0;
        }

        if (progress > 0)
        {
            progress = 0;
        }

        if (curField == (searchLimit - 1))
        {
            atGoal = -1;
            continue;
        }

        // If none of the potential field cells have a value
        // of curField, no need to look for it. Just move on.
        if (!fieldArray[curField])
        {
            curField++;

            // because adjacent values in the potential field may not contain
            // consecutive values (due to grey and wall distance penalties),
            // we have to account for that by allowing more runs before
            // determining no progress.
            if (progress <= 0)
            {
                progress--;

                // if (progress == -(1 + minObstacleVal))
                // if (progress == -(1 + (minObstacleVal*minObstacleVal+minObstacleVal)))
                if (progress == MinProgress)
                {
                    atGoal = -1; // stop searching if no progress
                }
            }

            continue;
        }

        // for (j=NewOffsetX; ((j<NewWorldXSize) && (j<NewMaxX)); j++)
        for (j = NewOffsetX; j < NewMaxX; j++)
        {
            // for (k=NewOffsetY; ((k<NewWorldYSize) && (k<NewMaxY)); k++)
            for (k = NewOffsetY; k < NewMaxY; k++)
            {
                if (curField > searchLimit)
                {
                    // we're past the search limit
                    atGoal = -2;
                }
                else if (IllahGrid[j][k] == curField)
                {
                    progress = 1;
                    // using boolean temp vars here is faster than rolling all
                    // the conditions into one large && clause. This is because there
                    // is only one conditional jump now, which means less chance of
                    // blowing the pipeline.
                    t1 = (j == IllahEndX);
                    t2 = (k == IllahEndY);
                    t3 = (IllahGrid[j][j] != 0);

                    if (t1 & t2 & t3)
                    {
                        // Success! Found goal.
                        atGoal = 1;
                    }

                    // grow four neighbors.

                    t1 = (j != 0);
                    t2 = (IllahGrid[j - 1][k] == stuffValue);

                    if (t1 & t2)
                    {
                        // if we're already real far from an obstacle, don't bother adding
                        // a penalty
                        ObstacleVal = ObstacleGrid[j - 1][k];

                        if (ObstacleVal > minObstacleVal)
                        {
                            newVal = curField + 1;

                            if (newVal < IllahGrid[j - 1][k])
                            {
                                IllahGrid[j - 1][k] = newVal;
                                fieldArray[MIN(newVal, searchLimit)] = true;
                            }
                        }
                        else if (ObstacleVal > nogo)
                        {
                            newVal = curField + 1 + powerarray[minObstacleVal - ObstacleVal];

                            if (newVal < IllahGrid[j - 1][k])
                            {
                                IllahGrid[j - 1][k] = newVal;
                                fieldArray[MIN(newVal, searchLimit)] = true;
                            }
                        }
                    }  // end if

                    t1 = (j < cWorldGridXSize / IGrain - 1);
                    t2 = (IllahGrid[j + 1][k] == stuffValue);

                    if (t1 & t2)
                    {
                        ObstacleVal = ObstacleGrid[j + 1][k];

                        if (ObstacleVal > minObstacleVal)
                        {
                            newVal = curField + 1;

                            if (newVal < IllahGrid[j + 1][k])
                            {
                                IllahGrid[j + 1][k] = newVal;
                                fieldArray[MIN(newVal, searchLimit)] = true;
                            }
                        }
                        else if (ObstacleVal > nogo)
                        {
                            newVal = curField + 1 + powerarray[minObstacleVal - ObstacleVal];

                            if (newVal < IllahGrid[j + 1][k])
                            {
                                IllahGrid[j + 1][k] = newVal;
                                fieldArray[MIN(newVal, searchLimit)] = true;
                            }
                        }
                    } // endif

                    t1 = (k != 0);
                    t2 = (IllahGrid[j][k - 1] == stuffValue);

                    if (t1 & t2)
                    {
                        ObstacleVal = ObstacleGrid[j][k - 1];

                        if (ObstacleVal > minObstacleVal)
                        {
                            newVal = curField + 1;

                            if (newVal < IllahGrid[j][k - 1])
                            {
                                IllahGrid[j][k - 1] = newVal;
                                fieldArray[MIN(newVal, searchLimit)] = true;
                            }
                        }
                        else if (ObstacleVal > nogo)
                        {
                            newVal = curField + 1 + powerarray[minObstacleVal - ObstacleVal];

                            if (newVal < IllahGrid[j][k - 1])
                            {
                                IllahGrid[j][k - 1] = newVal;
                                fieldArray[MIN(newVal, searchLimit)] = true;
                            }
                        }
                    } // endif

                    t1 = (k < cWorldGridYSize / IGrain - 1);
                    t2 = (IllahGrid[j][k + 1] == stuffValue);

                    if (t1 & t2)
                    {
                        ObstacleVal = ObstacleGrid[j][k + 1];

                        if (ObstacleVal > minObstacleVal)
                        {
                            newVal = curField + 1;

                            if (newVal < IllahGrid[j][k + 1])
                            {
                                IllahGrid[j][k + 1] = newVal;
                                fieldArray[MIN(newVal, searchLimit)] = true;
                            }
                        }
                        else if (ObstacleVal > nogo)
                        {
                            newVal = curField + 1 + powerarray[minObstacleVal - ObstacleVal];

                            if (newVal < IllahGrid[j][k + 1])
                            {
                                IllahGrid[j][k + 1] = newVal;
                                fieldArray[MIN(newVal, searchLimit)] = true;
                            }
                        }
                    }
                } // else if (IllahGrid...

#ifdef SHOW_DEBUG_MESSAGES

                if ((j == IllahEndX) && (k == IllahEndY))
                {
                    char buf[100];
                    sprintf(buf, "obs = %d, pot = %d\0",
                            ObstacleGrid[j][k], IllahGrid[j][k]);
                    //					AfxMessageBox(buf);
                }

#endif
            } // for (k
        } // for (j

        curField++;
    } // while (!atGoal)

    delete fieldArray;

    /*	FILE *output;
    	output = fopen("pot.txt", "wa");
    	for (j=0 + illahoffsetx; ((j < cWorldGridXSize/IGrain) &&
    							(j < illahmaxx)); j++) {
    			for (k=0 + illahoffsety; ((k<cWorldGridYSize/IGrain) &&
    										(k<illahmaxy)); k++) {
    				if (IllahGrid[j][k] == stuffValue)
    				{
    					fprintf(output, "0    ");
    				}
    				else
    				{
    					fprintf(output, "%4d ", IllahGrid[j][k]);
    				}
    			}
    			fprintf(output, "\n");
    	}
    	fclose(output); */

    // I'm writing this error message with AfxMess.Box, which I'm sure is wrong! //
    if (atGoal == -1)
    {
        PostMessage(m_ParentWindowHandle, ID_ROBOT_FIND_PATH_ERROR, cNoPathToGoalErrorNumber, 0);
        //	Sleep(1000);
#ifdef SHOW_DEBUG_MESSAGES
        //		sprintf(myString,"No path to the goal found.  Consider making more room. Field stopped at %d",
        //				curField);
        //		AfxMessageBox(myString);
#endif
    }
    else if (atGoal == -2)
    {
#ifdef SHOW_DEBUG_MESSAGES
        AfxMessageBox("Search exhausted to maximum search depth. Increase search depth.");
#endif
    } // else if

    return (atGoal);
}

// a variant of FindIPath that looks in vicinity only.
// use with QuickRefreshIGrid.
Path* IPathPlanner::QuickFindIPath(Path* InitialPath)
{
    int IllahStartX, IllahStartY, IllahEndX, IllahEndY; // Illah
    int atGoal;
    int i;

    Path* FoundPath = NULL;
    double qualities[5];

    // first, let's check the cache. If there is a plan already in the
    // cache, don't bother planning anything.
    if (cUseCache)
    {
        FoundPath = SearchPathCache(InitialPath);

        // proceed iff a cached path exists, and it is a clear path.
        // This takes care of the case where a person draws an imaginary
        // obstacle on the screen, and a cached path is recalled which goes
        // through it.
        if (FoundPath)
        {
            FoundPath->GetQualities(qualities, true);

            if (qualities[MinFound] > cVeryClearPathMinValue)
            {
                CFindPathThread::ShowPath(FoundPath, 1);
                return FoundPath;
            }
            // no good, so delete the path from the cache
            else
            {
                ErasePathFromCache(FoundPath);
                FoundPath = NULL;
            }
        }
    }

    // use InitialPath to set IllahStarts and Ends //
    robPOINT actualStart;
    robPOINT actualEnd;

    // InitialPath->Reverse(); // because of directionality of search 8/23/99 Illah //
    InitialPath->GetPoint(0, actualStart.x, actualStart.y);
    InitialPath->GetPoint(InitialPath->GetNumberOfPoints() - 1, actualEnd.x, actualEnd.y);

    // InitialPath->Reverse(); // to undo our earlier reversing! //


    // map actual start into Illah's coarser map.
    IllahStartX = (int)(((actualStart.x) / (cWorldGridPointSize * IGrain)) + cWorldGridXSize / (IGrain * 2));
    IllahStartY = (int)((actualStart.y) / (cWorldGridPointSize * IGrain) + cWorldGridYSize / (IGrain * 2));
    IllahEndX = (int)((actualEnd.x) / (cWorldGridPointSize * IGrain) + cWorldGridXSize / (IGrain * 2));
    IllahEndY = (int)((actualEnd.y) / (cWorldGridPointSize * IGrain) + cWorldGridYSize / (IGrain * 2));


    // Check to see if the start or end point is contained in an obstacle field //
    if (ObstacleGrid[IllahEndX][IllahEndY] == 1)    // Parag
    {
        PostMessage(m_ParentWindowHandle, ID_ROBOT_FIND_PATH_ERROR, cGoalInObstacleErrorNumber, 0);
        Sleep(1000);
#ifdef SHOW_DEBUG_MESSAGES
        //		AfxMessageBox("PROBLEM: Goal is contained in an obstacle or an obstacle inflation area. No can do.");
#endif
        return NULL;
    }

    if (ObstacleGrid[IllahStartX][IllahStartY] == 1)   // Parag
    {
        PostMessage(m_ParentWindowHandle, ID_ROBOT_FIND_PATH_ERROR, cStartInObstacleErrorNumber, 0);
        Sleep(1000);
#ifdef SHOW_DEBUG_MESSAGES
        //		AfxMessageBox("PROBLEM: Robot start point is contained in obstacle or obs inflation. No can do.");
#endif
        return NULL;
    }


    // create the potential field //
    atGoal = QuickCreateField(IllahStartX, IllahStartY, IllahEndX, IllahEndY);


    /* Now we generate a pure 1-step-per-segment Path  We do this by literally
    	walking from the start point "downhill" through the potential field until
    	we reach the goal */
    if (atGoal == 1)
    {
        // crystallizes path only out and fills rest of grid with
        // occupancy values. Positive values, not magnitude but sign only,
        // indicate the path!
        QuickCrystallizePath(IllahStartX, IllahStartY, IllahEndX, IllahEndY);

        if (CFindPathThread::bStopFindPath)
        {
            return NULL;
        }

        // generate a path by following the sign of the crystal field //
        // the positive numbers are the path - only doing this here
        // to show the user the current best paqth on  the screen
        FoundPath = GeneratePathFromCrystal(IllahStartX, IllahStartY,
                                            IllahEndX, IllahEndY,
                                            actualStart, actualEnd);


        CFindPathThread::ShowPath(FoundPath, 1);


        // the ridiculously simple low pass smoothing filter //

        OptimizePathLength(FoundPath);

        if (CFindPathThread::bStopFindPath)
        {
            delete FoundPath;
            return NULL;
        }


    } // endif (atGoal == 1)

    // blank out the IllahGrid, so we can build a new potential field
    // from start to goal
    CleanupGrid();

    if (atGoal == 1)
    {
        //	FoundPath->Reverse();
        // Add the path to the cache;
        if (cUseCache)
        {
            AddPathToCache(FoundPath);
        }

        return FoundPath;
    }
    else
    {
        if (FoundPath != NULL)
        {
            delete FoundPath;
        }

        return NULL;
    }


} // QuickFindIPath() //


// FindCheckinPoint() looks at path from start to goal, which was precomputed,
// and finds a nearby checkpoint, if necessary.
int IPathPlanner::FindCheckinPoint(Path *FoundPath, goalStruct *allGoals,
                                   int numGoals)
{
    // FoundPath contains a path from (current) start to (ultimate) goal.

    double PathLength;

    int j, k;
    int MidPointX;
    int MidPointY;
    int minx, miny, maxx, maxy;
    int stuffValue = 10000;
    double fStartX, fStartY;
    int StartX, StartY;

    int backupGoal = -1;
    int localSearchLimit = searchLimit;

    FoundPath->GetPoint(0, fStartX, fStartY);
    toIGrid(fStartX, fStartY, StartX, StartY);


    // this is a lower bound on path length. If the total length of the
    // current path is less than this value, then don't bother trying to find
    // a checkpoint. Just return the goal. A value of 3 should be 1 foot, as these
    // are IllahGrid coordinates.
    int pathLengthLowerBound = 0;

    // This is the upper bound on checkpoint search distance. The center point
    // of the checkpoint search area is either the midpoint of the path, or this
    // distance away from the path, whichever is closer. A value of 3 should be
    // one foot.
    int pathLengthUpperBound = 100000;

    // When looking for checkpoints, it is better to find either the final goal
    // or a corner checkpoint, as opposed to a flat checkpoint. So, if we find
    // a flat checkpoint, keep searching for a short distance further to find
    // a preferred goal, which is either the final goal or a corner checkpoint.
    // This variable tells how much further to search. A value of 15 should be
    // 60"
    int additionalSearchDistance = 15;

    // find the actual (not straight line) midpoint of the path.

    if (FoundPath == NULL)
    {
        return -1;
    }

    FindMidPoint(FoundPath, pathLengthUpperBound, MidPointX, MidPointY, PathLength);
    PathLength = MIN(PathLength / 2, pathLengthUpperBound);
    int Buffer = 4;

    // check to see if we should bother finding a checkpoint.
    if (PathLength < pathLengthLowerBound)
    {
        // find and return the goal as the checkpoint. No need to actually
        // search. We can do this because we know that the path which was
        // passed in has the goal as the endpoint.
        for (int i = 0; i < numGoals; i++)
        {
            if (allGoals[i].goalType == 0)
            {
                return i;
            }
        }

        return -1;
    }

    minx = int(MidPointX - PathLength - Buffer);
    maxx = int(MidPointX + PathLength + Buffer);
    miny = int(MidPointY - PathLength - Buffer);
    maxy = int(MidPointY + PathLength + Buffer);


    /*
    double sx, sy, ex, ey;
    FoundPath->GetPoint(0, sx, sy);
    FoundPath->GetPoint(FoundPath->GetNumberOfPoints()-1, ex, ey);

    int isx, isy, iex, iey;
    isx =(int)(((sx)/(cWorldGridPointSize*IGrain))+cWorldGridXSize/(IGrain*2));
    isy =(int)(((sy)/(cWorldGridPointSize*IGrain))+cWorldGridYSize/(IGrain*2));
    iex =(int)(((ex)/(cWorldGridPointSize*IGrain))+cWorldGridXSize/(IGrain*2));
    iey =(int)(((ey)/(cWorldGridPointSize*IGrain))+cWorldGridYSize/(IGrain*2));

    char buf[100];
    sprintf(buf, "pstart=(%d, %d), pend=(%d, %d), mp=(%d, %d)\0",
    	isx, isy, iex, iey, MidPointX, MidPointY);
    // AfxMessageBox(buf);
    memset(buf, 0, sizeof(char)*100);
    sprintf(buf, "minx = %d, miny = %d, maxx = %d, maxy = %d\0",
    	minx, miny, maxx, maxy);
    // AfxMessageBox(buf);

    */

    // now, clear out the GoalGrid and populate it with possible goals,
    // which are checkpoints and the actual goal. For now, there is no
    // marking of checkpoints which have already been visited, although
    // this will change.

    // clear out the Grid.
    //	for (j=0+illahoffsetx; ((j< cWorldGridXSize/IGrain) && (j<illahmaxx)); j++)
    //		for (k=0+illahoffsety; ((k< cWorldGridYSize/IGrain) && (k<illahmaxy)); k++)
    for (j = 0; (j < cWorldGridXSize / IGrain); j++)
        for (k = 0; (k < cWorldGridYSize / IGrain); k++)
        {
            GoalGrid[j][k] = stuffValue;
        }

    // Now, we're going to start the search from the start of the path,
    // to look for the nearest checkin point.
    // GoalGrid[MidPointX][MidPointY] = 1;
    GoalGrid[StartX][StartY] = 1;


    // now, grow the multiple goal potential field, using basicallly
    // the same method as in QuickCreateModifiedField(), but without penalties
    // for being near an obstacle or in a grey area.
    int curField = 1;
    int atGoal = 0;
    int progress = 0; // if this stays at zero, there is no path
    int ObstacleVal;

    while (atGoal == 0)
    {
        if (CFindPathThread::bStopFindPath)
        {
            return -1;
        }

        if (progress > 0)
        {
            progress = 0;
        }

        int ProgressIndicator = 0;
        int ProgressPercent = 0;

        for (j = minx + 1; ((j < cWorldGridXSize / IGrain) &&
                            (j < maxx - 1)); j++)
        {
            for (k = miny + 1; ((k < cWorldGridYSize / IGrain) &&
                                (k < maxy - 1)); k++)
            {
                if (curField > localSearchLimit)
                {
                    // we're past the search limit. If we have a backup (non-preferred)
                    // goal, return that. Else, return -1;
#ifdef SHOW_DEBUG_MESSAGES
                    char buf[100];
                    sprintf(buf, "Returning non-preferred goal\0");
                    AfxMessageBox(buf);
#endif
                    return backupGoal;
                }
                else if (GoalGrid[j][k] == curField)
                {
                    progress = 1;
                    int goalNum = FoundGoal(allGoals, j, k, numGoals);

                    if (goalNum != -1)
                    {
                        // We've found something. See if it is a preferred goal
                        // (i.e., the actual goal, or a corner checkpoint
                        if ((allGoals[goalNum].goalType == 0) ||
                                (allGoals[goalNum].goalType == 2))
                        {
                            // Success! Found goal.
                            char buf[100];
                            sprintf(buf, "Found preferred checkpoint\0");
                            // AfxMessageBox(buf);
                            return goalNum;
                        }
                        // We've found a non-preferred goal (flat checkpoint),
                        // and we've already found one before, so just return
                        // the closer one.
                        else if (backupGoal > -1)
                        {
#ifdef SHOW_DEBUG_MESSAGES

                            char buf[100];
                            sprintf(buf, "Found backup checkpoint\0");
                            AfxMessageBox(buf);
#endif
                            int d1, d2;
                            d1 = GetDistanceFromPath(NULL, allGoals[backupGoal].goalX,
                                                     allGoals[backupGoal].goalY);
                            d2 = GetDistanceFromPath(NULL, allGoals[goalNum].goalX,
                                                     allGoals[goalNum].goalY);

                            if (d1 < d2)
                            {
                                return backupGoal;
                            }
                            else
                            {
                                return goalNum;
                            }

                            // return backupGoal;
                            // return goalNum;
                        }
                        // We've found a non-preferred goal (flat checkpoint) for
                        // the first time, so save it, and search a little bit further
                        // to see if we can find a preferred goal.
                        else
                        {
#ifdef SHOW_DEBUG_MESSAGES
                            char buf[100];
                            sprintf(buf, "Found non-preferred checkpoint\0");
                            AfxMessageBox(buf);
#endif
                            backupGoal = goalNum;
                            localSearchLimit = curField + additionalSearchDistance;
                        }
                    }

                    // grow four neighbors. So each cell will contain distance from the
                    // start point.

                    if ((j != 0) && (GoalGrid[j - 1][k] == stuffValue))
                    {
                        // if we're already real far from an obstacle, don't bother adding
                        // a penalty
                        ObstacleVal = ObstacleGrid[j - 1][k];

                        if ((ObstacleVal > 1) || (ObstacleVal == 0))
                        {
                            GoalGrid[j - 1][k] = curField + 1;
                        }
                    }  // end if

                    if ((j < cWorldGridXSize / IGrain - 1) && (GoalGrid[j + 1][k] == stuffValue))
                    {
                        ObstacleVal = ObstacleGrid[j + 1][k];

                        if ((ObstacleVal > 1) || (ObstacleVal == 0))
                        {
                            GoalGrid[j + 1][k] = curField + 1;
                        }
                    } // endif

                    if ((k != 0) && (GoalGrid[j][k - 1] == stuffValue))
                    {
                        ObstacleVal = ObstacleGrid[j][k - 1];

                        if ((ObstacleVal > 1) || (ObstacleVal == 0))
                        {
                            GoalGrid[j][k - 1] = curField + 1;
                        }
                    } // endif

                    if ((k < cWorldGridYSize / IGrain - 1) && (GoalGrid[j][k + 1] == stuffValue))
                    {
                        ObstacleVal = ObstacleGrid[j][k + 1];

                        if ((ObstacleVal > 1) || (ObstacleVal == 0))
                        {
                            GoalGrid[j][k + 1] = curField + 1;
                        }
                    }
                } // else if (GoalGrid...

            }

            ProgressIndicator++;

            if (ProgressIndicator >= int((maxx - minx) / 3))
            {
                ProgressIndicator = 0;
                ProgressPercent ++;
                PostMessage(m_ParentWindowHandle, ID_ROBOT_FIND_PATH_STAGE, 4, ProgressPercent);
            }
        }

        if (progress <= 0)
        {
            progress--;

            if (progress == -1)
            {
                return -1; // stop searching if no progress
            }
        }

        curField++;
    } // while (!atGoal)

    return -1;
}

void IPathPlanner::FindMidPoint(Path *p, int pathLengthUpperBound,
                                int &mpx, int &mpy, double &totalLength)
{
    // This takes the passed in path, and finds the midpoint in
    // Illahgrid coordinates.

    int numPts;
    int i;
    double pathLength;
    double oldPathLength;
    double desLength;
    double x1, y1, x2, y2;
    int ix1, iy1, ix2, iy2;

    numPts = p->GetNumberOfPoints();
    totalLength = 0;

    // find the actual total path length.
    i = 1;

    do
    {
        p->GetPoint(i - 1, x1, y1);
        p->GetPoint(i, x2, y2);
        toIGrid(x1, y1, ix1, iy1);
        toIGrid(x2, y2, ix2, iy2);
        totalLength += sqrt((double)((ix1 - ix2) * (ix1 - ix2)) +
                            (double)((iy1 - iy2) * (iy1 - iy2)));
        i++;
    }
    while (i < numPts);

    // This is how far along the path we want to center the search.
    desLength = MIN(totalLength / 2.0, pathLengthUpperBound);

    pathLength = oldPathLength = 0;
    i = 1;

    do
    {
        p->GetPoint(i - 1, x1, y1);
        p->GetPoint(i, x2, y2);
        toIGrid(x1, y1, ix1, iy1);
        toIGrid(x2, y2, ix2, iy2);
        pathLength += sqrt((double)((ix1 - ix2) * (ix1 - ix2)) +
                           (double)((iy1 - iy2) * (iy1 - iy2)));

        // we've passed the desired length, so find the true coordinates
        //  of the midpoint or max length using linear interpolation.
        if (pathLength > desLength)
        {
            double remainingLength;
            double percentage;
            remainingLength = (desLength - oldPathLength);
            percentage = remainingLength / (pathLength - oldPathLength);

            if (abs(ix2 - ix1) > 0)
            {
                mpx = (int)(ix1 + ((double)(ix2 - ix1) * percentage));
            }
            else
            {
                mpx = ix1;
            }

            if (abs(iy2 - iy1) > 0)
            {
                mpy = (int)(iy1 + ((double)(iy2 - iy1) * percentage));
            }
            else
            {
                mpy = iy1;
            }

            double testLength;
            testLength = sqrt((double)((ix1 - mpx) * (ix1 - mpx)) +
                              (double)((iy1 - mpy) * (iy1 - mpy)));
            char buf[100];
            sprintf(buf, "deslen = %f, testlen = %f\0", remainingLength,
                    testLength);
            // AfxMessageBox(buf);
            break;
        }

        i++;
        oldPathLength = pathLength;
    }
    while (i < numPts);

}

int IPathPlanner::FoundGoal(goalStruct *ag, int x, int y, int numGoals)
{
    // check to see if the current position is on the goal list. If so,
    // we're done.

    int i;
    int ix, iy;

    for (i = 0; i < numGoals; i++)
    {
        toIGrid(ag[i].goalX, ag[i].goalY, ix, iy);

        if ((x == ix) && (y == iy) && (!ag[i].goalVisited))
        {
            return i;
        }
    }

    return -1;
}

// This function takes a path, and computes the distance of
// each point to the path, taking obstacles into account.
void IPathPlanner::ComputeDistanceToPath(Path *p)
{
    int j, k;
    int stuffValue = 10000;
    int curField;
    int progress;
    int ObstacleVal;


    //	for (j=0; ((j< cWorldGridXSize/IGrain) && (j<illahmaxx)); j++)
    //		for (k=0; ((k< cWorldGridYSize/IGrain) && (k<illahmaxy)); k++)

    for (j = 0; (j < cWorldGridXSize / IGrain); j++)
        for (k = 0; (k < cWorldGridYSize / IGrain); k++)
        {
            PathDistanceGrid[j][k] = stuffValue;
        }

    // First, seed the grid with the path. This creates a set of 1's in the
    // grid, which correspond to the path.
    AddPathToGrid(p);

    // now, grow out the path taking obstacles into account,
    // but not caring about being really close to an
    // obstacle, or being in a grey area, just like in
    // FindCheckinPoint().

    curField = 1;
    progress = 1;

    while (progress)
    {
        progress = 0;

        for (j = 0 + illahoffsetx; ((j < cWorldGridXSize / IGrain) &&
                                    (j < illahmaxx)); j++)
        {
            for (k = 0 + illahoffsety; ((k < cWorldGridYSize / IGrain) &&
                                        (k < illahmaxy)); k++)
            {
                // grow four neighbors. So each cell will contain distance
                // from the path
                if (PathDistanceGrid[j][k] == curField)
                {
                    progress = 1;

                    if (j != 0)
                    {
                        // if we're already real far from an obstacle, don't bother adding
                        // a penalty
                        ObstacleVal = ObstacleGrid[j - 1][k];

                        // ObstacleVal = 1000;
                        if ((ObstacleVal > 1) || (ObstacleVal == 0))
                        {
                            if (PathDistanceGrid[j - 1][k] > curField + 1)
                            {
                                PathDistanceGrid[j - 1][k] = curField + 1;
                            }
                        }
                    }  // end if

                    if (j < cWorldGridXSize / IGrain - 1)
                    {
                        ObstacleVal = ObstacleGrid[j + 1][k];

                        // ObstacleVal = 1000;
                        if ((ObstacleVal > 1) || (ObstacleVal == 0))
                        {
                            if (PathDistanceGrid[j + 1][k] > curField + 1)
                            {
                                PathDistanceGrid[j + 1][k] = curField + 1;
                            }
                        }
                    } // endif

                    if (k != 0)
                    {
                        ObstacleVal = ObstacleGrid[j][k - 1];

                        // ObstacleVal = 1000;
                        if ((ObstacleVal > 1) || (ObstacleVal == 0))
                        {
                            if (PathDistanceGrid[j][k - 1] > curField + 1)
                            {
                                PathDistanceGrid[j][k - 1] = curField + 1;
                            }
                        }
                    } // endif

                    if (k < cWorldGridYSize / IGrain - 1)
                    {
                        ObstacleVal = ObstacleGrid[j][k + 1];

                        // ObstacleVal = 1000;
                        if ((ObstacleVal > 1) || (ObstacleVal == 0))
                        {
                            if (PathDistanceGrid[j][k + 1] > curField + 1)
                            {
                                PathDistanceGrid[j][k + 1] = curField + 1;
                            }
                        }
                    }
                }  // if ... curfield...
            } // for k..
        } // for j...

        curField++;
    } // while(progress)

    /*	FILE *output;
    	output = fopen("ck.txt", "wa");
    	for (j=0 + illahoffsetx; ((j < cWorldGridXSize/IGrain) &&
    							(j < illahmaxx)); j++)
    	{
    		for (k=0 + illahoffsety; ((k<cWorldGridYSize/IGrain) &&
    									(k<illahmaxy)); k++)
    		{
    			fprintf(output, "%5d ", PathDistanceGrid[j][k]);
    		}
    		fprintf(output, "\n");
    	}
    	fclose(output); */
}


// This function fills in the PathDistanceGrid with '1's
// along the length of the path.
void IPathPlanner::AddPathToGrid(Path *p)
{
    int i;
    int numPts;
    double dStartX, dStartY, dEndX, dEndY;
    double dCurrX, dCurrY;
    int CurrX, CurrY;
    double Dist;
    double SegLength;
    double Percentage;

    // now, we go through each segment in the path, and
    // fill in 1's along the segment in the grid.
    numPts = p->GetNumberOfPoints();

    // find the actual total path length.
    i = 1;

    do
    {
        // Get the start and end points of the current
        // segment
        p->GetPoint(i - 1, dStartX, dStartY);
        p->GetPoint(i, dEndX, dEndY);
        SegLength = sqrt(pow((dStartX - dEndX), 2) +
                         pow((dStartY - dEndY), 2));

        // now, move 2" at a time, along the segment,
        // filling in 1's in the PathDistanceGrid
        for (Dist = 0; Dist <= SegLength; Dist += 2.0)
        {
            // Figure out (x,y) coords of 'Dist' inches
            // away from the start point.
            Percentage = Dist / SegLength;

            if (fabs(dEndX - dStartX) > 0)
            {
                dCurrX = (dStartX +
                          ((dEndX - dStartX) * Percentage));
            }
            else
            {
                dCurrX = dStartX;
            }

            if (fabs(dEndY - dStartY) > 0)
            {
                dCurrY = (dStartY +
                          ((dEndY - dStartY) * Percentage));
            }
            else
            {
                dCurrY = dStartY;
            }

            toIGrid(dCurrX, dCurrY, CurrX, CurrY);
            PathDistanceGrid[CurrX][CurrY] = 1;
        }

        i++;
    }
    while (i < numPts);
}

int IPathPlanner::GetDistanceFromPath(Path *p, double x, double y)
{

    int ix, iy;

    toIGrid(x, y, ix, iy);

    // each grid cell size is 4", so convert from grid distance to inches.
    return ((PathDistanceGrid[ix][iy]) * 4);
}

/*
 Parag Batavia
 10/27/99
 This code searches the path cache, and returns a COPY of the path if
 found. We return a copy, because the path will be deleted in
 FindPathThread.
*/
Path *IPathPlanner::SearchPathCache(Path *p)
{
    int i;

    i = 0;

    while (i < NumCacheEntries)
    {
        // see if the start and end points of the cached path are within
        // 6" of the desired start and end points

        if (PathCache[i].thePath)
        {
            robPOINT Start, End;
            p->GetPoint(0, Start.x, Start.y);
            p->GetPoint(p->GetNumberOfPoints() - 1, End.x, End.y);

            if ((fabs(PathCache[i].xStart - Start.x) < Epsilon) &&
                    (fabs(PathCache[i].yStart - Start.y) < Epsilon) &&
                    (fabs(PathCache[i].xEnd - End.x) < Epsilon) &&
                    (fabs(PathCache[i].yEnd - End.y) < Epsilon))
            {
                // Found a path. Copy and Return it.
                //				Path *newPath = new Path(*(PathCache[i].thePath));
                Path *newPath = PathCache[i].thePath;	//S.R. 10/30/99
#ifdef SHOW_DEBUG_MESSAGES
                double s1, s2, e1, e2;
                char buf[100];
                newPath->GetPoint(0, s1, e1);
                newPath->GetPoint(newPath->GetNumberOfPoints() - 1, s2, e2);
                sprintf(buf, "Cached Path: (%f,%f) to (%f, %f)\0",
                        s1, e1, s2, e2);
                AfxMessageBox(buf);
#endif
                IncrementCacheAge(i);
                return newPath;
            }
            else if ((fabs(PathCache[i].xStart - End.x) < Epsilon) &&
                     (fabs(PathCache[i].yStart - End.y) < Epsilon) &&
                     (fabs(PathCache[i].xEnd - Start.x) < Epsilon) &&
                     (fabs(PathCache[i].yEnd - Start.y) < Epsilon))
            {
                // Found a reverse path. Copy, reverse, and return it.
                Path *newPath = new Path(*(PathCache[i].thePath));
                newPath->Reverse();
#ifdef SHOW_DEBUG_MESSAGES
                char buf[100];
                double s1, s2, e1, e2;
                newPath->GetPoint(0, s1, e1);
                newPath->GetPoint(newPath->GetNumberOfPoints() - 1, s2, e2);
                sprintf(buf, "Reverse Cached Path: (%f,%f) to (%f, %f)\0",
                        s1, e1, s2, e2);
                AfxMessageBox(buf);
#endif
                IncrementCacheAge(i);
                return newPath;
            }
        }

        i++;
    }

    return NULL;
}

void IPathPlanner::IncrementCacheAge(int i)
{
    int j;

    for (j = 0; j < cCacheSize; j++)
    {
        if ((PathCache[j].thePath) && (i != j))
        {
            PathCache[j].age++;
        }
    }
}

int cmp_int(const void *a1, const void *a2)
{
    if (*((int*)a1) < * ((int*)a2))
    {
        return 1;
    }
    else if (*((int*)a1) > *((int*)a2))
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void IPathPlanner::RemoveOldestFromCache(void)
{
    int i;
    int AllAge[cCacheSize];
    int NumToDelete;
    float PercentToDelete = 10.0;
    int AgeThreshold;

    NumToDelete = (int)(cCacheSize * (PercentToDelete / 100.0));

    for (i = 0; i < cCacheSize; i++)
    {
        AllAge[i] = PathCache[i].age;
    }

    qsort((void*)AllAge, cCacheSize, sizeof(int), cmp_int);

    AgeThreshold = AllAge[NumToDelete];

    for (i = 0; i < cCacheSize; i++)
    {
        if (PathCache[i].age > AgeThreshold)
        {
            delete PathCache[i].thePath;
            PathCache[i].thePath = NULL;
            PathCache[i].age = 0;
            PathCache[i].xEnd = 0;
            PathCache[i].xStart = 0;
            PathCache[i].yEnd = 0;
            PathCache[i].yStart = 0;
            NumCacheEntries--;
        }
    }

    ASSERT(NumCacheEntries < cCacheSize);
}
/*
 Parag Batavia
 10/27/99
 This code adds a copy of the passed in path to the cache.
*/
void IPathPlanner::AddPathToCache(Path *PathToAdd)
{
    // The paths are all stored in an array (a list would be a better idea)
    // Empty entries in the array have NULLs for all the fields.

    int i;

    // have to do some cache flushing
    if (cCacheSize - NumCacheEntries < 10)
    {
        RemoveOldestFromCache();
    }

    // search until we find an empty spot in the cache. This isn't the
    // most efficient way to do things - have to ask stephan if he has any
    // list code buried anywhere that I can use. If not, I'll add my own
    // later.
    for (i = 0; i < cCacheSize; i++)
    {
        if (!PathCache[i].thePath)
        {
            Path *dummyPath;
            //			dummyPath = new Path(*PathToAdd);
            dummyPath = PathToAdd;	//S.R. 10/29/99
            PathCache[i].thePath = dummyPath;
            dummyPath->GetPoint(0, PathCache[i].xStart, PathCache[i].yStart);
            dummyPath->GetPoint(dummyPath->GetNumberOfPoints() - 1,
                                PathCache[i].xEnd, PathCache[i].yEnd);
            NumCacheEntries++;
#ifdef SHOW_DEBUG_MESSAGES
            AfxMessageBox("Added Path to Cache\0");
#endif
            break;
        }
    }
}

void IPathPlanner::ErasePathFromCache(double x1, double y1)
{
    int i;

    i = 0;

    while (i < NumCacheEntries)
    {
        if (PathCache[i].thePath)
        {
            double s1, s2, e1, e2;
            PathCache[i].thePath->GetPoint(0, s1, e1);
            PathCache[i].thePath->GetPoint(PathCache[i].thePath->GetNumberOfPoints(),
                                           s2, e2);

            if (((fabs(x1 - s1) < Epsilon) &&
                    (fabs(y1 - e1) < Epsilon)) ||
                    ((fabs(x1 - s2) < Epsilon) &&
                     (fabs(y1 - e2) < Epsilon)))
            {
                // delete this entry
                delete PathCache[i].thePath;
                PathCache[i].thePath = NULL;
                PathCache[i].xStart = 0;
                PathCache[i].xEnd = 0;
                PathCache[i].yStart = 0;
                PathCache[i].yEnd = 0;
                NumCacheEntries--;
            }
        }

        i++;
    }
}

void IPathPlanner::ErasePathFromCache(Path *Remove)
{
    for (int i = 0; (i < NumCacheEntries) && (Remove != NULL); i++)
    {
        if (PathCache[i].thePath == Remove)
        {
            // delete this entry
            delete Remove;
            Remove = NULL;
            PathCache[i].thePath = NULL;
            PathCache[i].xStart = 0;
            PathCache[i].xEnd = 0;
            PathCache[i].yStart = 0;
            PathCache[i].yEnd = 0;
            NumCacheEntries--;
        }
    }
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



IPathPlanner::IPathPlanner()
{
    m_ProbGrid = NULL;
    m_ParentWindowHandle = NULL;
}

IPathPlanner::~IPathPlanner()
{
    for (int i = 0; i < NumCacheEntries; i++)
    {
        // delete this entry
        if (PathCache[i].thePath != NULL)
        {
            delete(PathCache[i].thePath);
        }
    }
}
