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
// IPathPlanner.h: interface for the IPathPlanner class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPATHPLANNER_H__656AA560_2F30_11D3_883A_006008E03A41__INCLUDED_)
#define AFX_IPATHPLANNER_H__656AA560_2F30_11D3_883A_006008E03A41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Path;
class CProbabilityGrid;
class CCalibrationZone;

typedef struct _pathcachestruct
{
    double xStart, yStart;
    double xEnd, yEnd;
    Path *thePath;
    int age;
} PathCacheStruct;

typedef struct _goalSet
{
    Path *thePath;
    CCalibrationZone *calZone;
    bool Cached;
} goalSet;

typedef struct _goalStruct
{
    double goalX;
    double goalY;
    int isPreferred;
    int goalVisited;
    int goalType;
    CCalibrationZone *theGoal;
} goalStruct;


int cmp_int(const void *a1, const void *a2);

class IPathPlanner
{
    public:
        void ErasePathFromCache(Path* Remove);
        // we can assume that IPathPlanner is instantiated once, when the robot
        // is created and findpaththread is instantiated. We also assume that
        // the probability grid doesn't get "swapped out" either!
        void SetProbabilityGrid(CProbabilityGrid* ProbGrid)
        {
            m_ProbGrid = ProbGrid;
        };
        void SetParentWindowHandle(HWND ParentWindowHandle)
        {
            m_ParentWindowHandle = ParentWindowHandle;
        };
        IPathPlanner();
        virtual ~IPathPlanner();

        // Cache functions
        Path *SearchPathCache(Path *p);
        void AddPathToCache(Path *p);
        void ErasePathFromCache(double x1, double y1);
        void IncrementCacheAge(int i);
        void RemoveOldestFromCache(void);

        // Computes distance of a point to the path
        void ComputeDistanceToPath(Path *p);
        void AddPathToGrid(Path *p);
        int GetDistanceFromPath(Path *p, double x, double y);

        // re-populates the IGrid obstacle grid from ProbGrid //
        // this only refreshes the obstacle grid in the vicinity of the robot and goal
        // and bounds the area so nothing dangerous can happen
        void QuickRefreshIGrid(Path *InitialPath, bool GoThroughGrey = false, bool Shortcut = true);

        // cleans up IGrid so you can re-plan with it (without calling RefreshGrid
        // of course, if you think probgrid changed a lot, then just cal RefreshGrid
        // Basically changes all positives to zeroes only
        void CleanupGrid();

        // this takes IGrid as a field and creates a whole new creature in which
        // full field of obstacles exists, with positive sign indicating the path!
        void CrystallizePath(int IllahStartX, int IllahStartY, int IllahEndX, int IllahEndY);
        void QuickCrystallizePath(int IllahStartX, int IllahStartY,
                                  int IllahEndX, int IllahEndY);


        void ReplacePath(int endX, int endY, int newVal);

        int FindCheckinPoint(Path *FoundPath, goalStruct *allGoals, int numGoals);
        int FoundGoal(goalStruct *ag, int x, int y, int numGoals);
        void FindMidPoint(Path *FoundPath, int maxLength, int &mx, int &my, double &l);

        // This takes a field and a starting point (a large field number) and
        // generates the provably shortest path to the field ZERO point.
        // end1 is the robotworld coordinate to correlate with beginAtx,beginAty in
        // the illah IGrid coordinates.  end2 is the other robotworld end of the path desired
        Path* GeneratePathFromCrystal(int beginAtx, int beginAty, int endAtx, int endAty,
                                      robPOINT end1, robPOINT end2);

        // this takes the path and merges segments as much as possible without
        // violating the quality constraints. It tends to cut corners maximally!
        void OptimizePathLength(Path* FoundPath);

        // grows a potential field in IGrid starting at StartX,StartY with value
        // 1 and growing up and out to fill entire grid or until End is reached, whichever
        // happens first
        int QuickCreateField(int IllahStartX, int IllahStartY,
                             int IllahEndX, int IllahEndY);


        // really just uses InitialPath for the start and end robPoints //
        // returns 1 if it succeeded (atgoal)
        // (to be used with QuickRefreshIGrid), only looks for goal in same vicinity //
        Path* QuickFindIPath(Path* InitialPath);

        CProbabilityGrid* m_ProbGrid;

    private:
        HWND m_ParentWindowHandle;

};



#endif // !defined(AFX_IPATHPLANNER_H__656AA560_2F30_11D3_883A_006008E03A41__INCLUDED_)
