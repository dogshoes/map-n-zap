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
/*******************************************************************************
//FindPathThread.cpp
//Created: 01/05/97 S.R.
********************************************************************************
//Description:
//This file contains the functions for the FindPath thread.
*******************************************************************************/
#include "stdafx.h"
#include "Robot.h"
#include "Path.h"
#include "WorldGridConsts.h"
#include "Probability Grid.h"
//#include "IPathPlanner.h" // added as Illah's new path planner
#include "PositionLabel.h"
#include "GoalSet.h"

#ifdef _DEBUG
// #define SHOW_DEBUG_MESSAGES
#endif

// extern "C" {
// #define PROTOTYPE
// #include "ateam_opt.h"
// }

/*******************************************************************************
//Created: 01/05/98 S.R.
//Last Revision: 01/07/98 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/

struct TPathThreadInput
{
    long StartX;
    long StartY;
    long EndX;
    long EndY;
    //	Path* OldPath;
    //	int Iterations;
    //	int Repeat;
    CProbabilityGrid* ProbGrid;
    HWND Parent;
    TFoundPathFunction CallBack;
    UINT CallBackParam1;
    void* CallBackParam2;
    bool ShowPaths;
};




bool CFindPathThread::ExecuteFindPathQueueThread = true;
HWND CFindPathThread::FindPathThreadWindowHandle = NULL;
bool CFindPathThread::bStopFindPath = false;
bool CFindPathThread::ShowPaths = true;
CEvent CFindPathThread::QueueEvent;
CWnd* CFindPathThread::m_Parent = NULL;
CRobot* CFindPathThread::m_Robot = NULL;
TFindPathQueue* CFindPathThread::QueueTail = NULL;
TFindPathQueue* CFindPathThread::QueueHead = NULL;
CCriticalSection CFindPathThread::QueueLock;
IPathPlanner CFindPathThread::m_IPathPlanner;

CFindPathThread::CFindPathThread(CRobot* Robot, CWnd* Parent)
/*******************************************************************************
//Created: 01/05/98 S.R.
//Last Revision: 01/07/98 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    ExecuteFindPathQueueThread = true;
    FindPathThreadWindowHandle = NULL;
    bStopFindPath = FALSE;
    ShowPaths = TRUE;
    m_Parent = Parent;
    m_Robot = Robot;
    QueueTail = NULL;
    QueueHead = NULL;
    m_FindPathQueueThread = AfxBeginThread(CFindPathThread::ThreadFunction, 0, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    assert(m_FindPathQueueThread);
    m_FindPathQueueThread->m_bAutoDelete = FALSE;
    m_FindPathQueueThread->ResumeThread();

}

CFindPathThread::~CFindPathThread()
{
    ExecuteFindPathQueueThread = FALSE;
    QueueEvent.SetEvent();
    WaitForSingleObject(m_FindPathQueueThread->m_hThread, INFINITE);
    delete m_FindPathQueueThread;
}

void CFindPathThread::AddToQueue(robPOINT Start, robPOINT End, int Iterations, int Repeat,
                                 bool ShowPaths, CProbabilityGrid* ProbGrid, HWND Parent,
                                 TFoundPathFunction FoundPathFunction, UINT FoundPathParam1,
                                 void* FoundPathParam2)
/*******************************************************************************
//Created: 01/05/98 S.R.
//Last Revision: 01/09/98 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    /*Create new thread param structure to put into queue*/
    TPathThreadInput* ThreadParam = new TPathThreadInput;
    ThreadParam->StartX = long(Start.x);
    ThreadParam->StartY = long(Start.y);
    ThreadParam->EndX = long(End.x);
    ThreadParam->EndY = long(End.y);
    //	ThreadParam->OldPath = NULL;
    //	ThreadParam->Iterations = Iterations;
    //	ThreadParam->Repeat = Repeat;
    ThreadParam->ProbGrid = ProbGrid;
    ThreadParam->Parent = Parent;

    //	if (FoundPathFunction == NULL) FoundPathFunction = m_FindPathThread->DefaultFoundPathFunction;
    ThreadParam->CallBack = FoundPathFunction;
    ThreadParam->CallBackParam1 = FoundPathParam1;
    ThreadParam->CallBackParam2 = FoundPathParam2;
    ThreadParam->ShowPaths = ShowPaths;
    /*Add to queue*/
    AddToQueue(ThreadParam);
}

void CFindPathThread::AddToQueue(TPathThreadInput* ThreadParam)
/*******************************************************************************
//Created: 01/22/98 S.R.
//Last Revision: 01/22/98 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    CSingleLock QueueLock(&QueueLock);
    QueueLock.Lock();

    if (QueueHead == NULL)
    {
        QueueTail = new TFindPathQueue;
        QueueHead = QueueTail;
    }
    else
    {
        QueueTail->Next = new TFindPathQueue;
        QueueTail = QueueTail->Next;
    }

    QueueTail->ThreadInput = ThreadParam;
    QueueTail->Next = NULL;
    /*Signal QueueEvent to tell the thread something new is in the queue*/
    QueueEvent.SetEvent();
}

TPathThreadInput* CFindPathThread::RemoveHeadFromQueue(void)
/*******************************************************************************
//Created: 01/05/98 S.R.
//Last Revision: 01/07/98 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    CSingleLock QueueLock(&QueueLock);
    QueueLock.Lock();
    TPathThreadInput* TempThreadInput = NULL;

    if (QueueHead != NULL)
    {
        TFindPathQueue* TempHead = QueueHead;
        TempThreadInput = QueueHead->ThreadInput;
        QueueHead = QueueHead->Next;

        if (QueueHead == NULL)
        {
            QueueTail = NULL;
        }

        delete TempHead;
    }

    return TempThreadInput;
}

void CFindPathThread::DefaultFoundPathFunction(Path* FoundPath, UINT FoundPathParam, void*)
/*******************************************************************************
//Created: 01/05/98 S.R.
//Last Revision: 01/07/98 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    m_Robot->SetState(stWaiting);
    m_Parent->PostMessage(ID_ROBOT_FIND_PATH_FINISHED, 0, (LPARAM)(FoundPath));
}


void CFindPathThread::ShowPath(Path* path, int type)
/*******************************************************************************
//Created: 01/05/98 S.R.
//Last Revision: 01/07/98 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    if (ShowPaths)
    {
        Path* PathToShow = NULL;

        if (type != 2)
        {
            if (path != NULL)
            {
                PathToShow = new Path(*path);
            }
        }

        PostMessage(FindPathThreadWindowHandle, ID_ROBOT_SHOW_FIND_PATH, type, (LPARAM)(PathToShow));
    }
}

const int goalEpsilon = 60;
const int cMaxGoals = 100;
// This controls how far we are willing to deviate from the original
// start->goal path, in order to find checkpoints.
const int MaxDistanceFromPath = 60;


UINT CFindPathThread::ThreadFunction(LPVOID pParam)
/*******************************************************************************
//Created: 01/05/98 S.R.
//Last Revision: 01/09/98 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
/*Given the parameters Start(-1000,0) End (-274,-46) on LargeWorld.map Iterations = 100, Repeat = 3
	This thread will give memory leaks.  However, if Repeat = 2, memory leaks go away.  Why?  I don't know.*/
{
    CSingleLock QueueSignal(&QueueEvent);	//Event to wake up the thread
    double startX, startY;
    goalStruct allGoals[cMaxGoals];
    goalSet goalList[cMaxGoals];
    int numGoals, resultGoal;
    bool usingShortCut;
    // this is in inches

    startX = startY = 0.0;
    usingShortCut = true;

    while (ExecuteFindPathQueueThread)
    {
        /*Wait until the thread receives an event signaling new paths to find in the queue*/
        QueueSignal.Lock();
        bStopFindPath = FALSE;
        TPathThreadInput* PathInfo;

        while ((PathInfo = RemoveHeadFromQueue()) && (!bStopFindPath) && (ExecuteFindPathQueueThread))
        {
            int i = 0;

            Path* PathToDestination = NULL;
            Path* PathToDestinationBackup = NULL;
            Path* InitialPath = NULL;
            Path* CheckpointPath = NULL;
            memset(allGoals, 0, sizeof(goalStruct) * 100);
            memset(goalList, 0, sizeof(goalSet) * 100);

            ShowPaths = PathInfo->ShowPaths;
            FindPathThreadWindowHandle = PathInfo->Parent;

            long x1 = PathInfo->StartX;
            long y1 = PathInfo->StartY;
            long x2 = PathInfo->EndX;
            long y2 = PathInfo->EndY;
            InitialPath = new Path(PathInfo->ProbGrid->m_RobotWorld, x1, y1, x2, y2);

            // if cached path existed, we have it by now in InitialPath
            // else Initialpath is a straight line from start to finish

            double qualities[5];

            InitialPath->GetQualities(qualities, true);
            // getqualities checks to see if the given path goes through obstacles.

            bool GoodPath = false;

            if (qualities[MinFound] < cVeryClearPathMinValue)
            {
                // darn. initial path sucks. so let's do some planning!

                // instantiate IPathPlanner class.
                // This should be done in constructor of findpaththread
                // The bottom needs to be a global -- it needs to be for
                // memory considerations
                // and so that the map resides from call to call.
                PostMessage(FindPathThreadWindowHandle, ID_ROBOT_FIND_PATH_STAGE, 0, 0);
                m_IPathPlanner.SetProbabilityGrid(PathInfo->ProbGrid);
                m_IPathPlanner.SetParentWindowHandle(FindPathThreadWindowHandle);
                IPathPlanner* IllahPathMaker = &m_IPathPlanner;

                // If not, try to find a path in shortcut world
                IllahPathMaker->QuickRefreshIGrid(InitialPath);
                PathToDestination = IllahPathMaker->QuickFindIPath(InitialPath);

                // If we can't, then try to find a path in the larger world.
                if (!PathToDestination)
                {
                    IllahPathMaker->QuickRefreshIGrid(InitialPath, false, false);
                    PathToDestination = IllahPathMaker->QuickFindIPath(InitialPath);
                    usingShortCut = false;
                }


                // Enable this if we don't want to look for checkpoints.
                /* if (PathToDestination)
                {
                	goalList[0].thePath = PathToDestination;
                	goalList[0].calZone = NULL;
                	goalList[0].Cached = true;
                } */

                // This code finds any intermediate checkpoints we may need to use.
                if (PathToDestination)
                    // if (0)
                {
                    int cntr = 0;

                    // Make a copy of PathToDestination - we may need it later,
                    // as a backup, if we are unable to find a path to a
                    // desired checkpoint
                    //					PathToDestinationBackup = new Path(*PathToDestination);
                    PathToDestinationBackup = PathToDestination;

                    // First, set ourselves back to shortcut world to find
                    // the checkpoints.
                    if (usingShortCut == false)
                    {
                        IllahPathMaker->QuickRefreshIGrid(InitialPath);
                        usingShortCut = true;
                    }

                    // Used to find distance of a given checkpoint to
                    // the path.
                    IllahPathMaker->ComputeDistanceToPath(PathToDestination);

                    // Now, set up the goal list. This is an array including the
                    // final goal point and checkpoints. If a checkpoint is really close
                    // to the start or final goal, it is not included.
                    GetListOfGoals(allGoals, cntr, PathInfo);
                    numGoals = cntr;

                    // now, if we've found a path, find the midpoint of the path,
                    // and look for the nearest checkin point.
                    resultGoal = -2;

                    // This call finds a nearby checkpoint. It will search the
                    // neighborhood of the start->goal path up to a certain distance,
                    // which is either half the path length, or a threshold set in
                    // this function. The center of the search is either the midpoint
                    // of the path, or 'threshold' along the path, whichever is closer.
                    resultGoal = IllahPathMaker->FindCheckinPoint(PathToDestination,
                                 allGoals, numGoals);
                    PathToDestination->GetPoint(0, startX, startY);
                    cntr = 0;

                    // This loop evaluates the checkin points which are found. One of
                    // three things can happen: The goal is found, in which case we
                    // are done. A checkin point is found, in which case it is added
                    // to the path list, or nothing is found. This last case shouldn't
                    // happen, but if it does, the default behavior is to add a path
                    // from the current location (either start point or checkpoint)
                    // to the goal, so that things don't break. NOTE: This is a checkpoint
                    // search, NOT the path search.
                    while (!bStopFindPath)
                    {
#ifdef SHOW_DEBUG_MESSAGES
                        char buf[100];
#endif

                        // Find a good checkpoint.
                        if (resultGoal == -1) // Couldn't find anything.
                        {
#ifdef SHOW_DEBUG_MESSAGES
                            // Couldn't find a checkpoint or goal. Break.
                            memset(buf, 0, sizeof(char) * 100);
                            sprintf(buf, "Couldn't find checkpoint or goal.");
                            AfxMessageBox(buf);
#endif
                            // This shouldn't happen, but if it does, then just add
                            // dummyPath, which is a path from the current checkpoint
                            // to the goal to the goalList, and return.
                            /*10/24/99 S.R. If, on first pass, FindCheckinPoint can't find anything then use PathToDestination*/
                            goalList[cntr].thePath = PathToDestination;
                            goalList[cntr].calZone = NULL;
                            goalList[cntr].Cached = true;
                            ShowPath(goalList[cntr].thePath, 2);
                            cntr++;
                            break;
                        }
                        else if (allGoals[resultGoal].goalType == 0) // Found the goal.
                        {
                            // If the checkpoint search has found the goal, then we either
                            // add in PathToDestination (which was the original path from start
                            // to goal), or dummyPath. If this is the first time we're
                            // coming through here, then no checkpoints are necessary,
                            // and the original PathToDestination can be used. Else, dummyPath
                            // contains a path from the current checkpoint to the goal,
                            // so save that.
                            goalList[cntr].thePath = PathToDestination;
                            goalList[cntr].calZone = NULL;
                            goalList[cntr].Cached = true;
                            cntr++;

                            ShowPath(goalList[cntr].thePath, 2);

#ifdef SHOW_DEBUG_MESSAGES
                            // Found end goal. We're done.
                            memset(buf, 0, sizeof(char) * 100);
                            sprintf(buf, "Found goal!");
                            AfxMessageBox(buf);
#endif
                            break;
                        }
                        else // Found a checkpoint.
                        {
                            //							delete PathToDestination;
                            //							PathToDestination = NULL;

                            // mark the checkin point as visited.
                            allGoals[resultGoal].goalVisited = 1;

                            // plan a path from the current location to the
                            // new checkin point. If we can't find a path in
                            // the shortcut world, grow the world to a larger
                            // size.
                            Path FromLastGoalToCheckpoint(PathInfo->ProbGrid->m_RobotWorld,
                                                          startX, startY,
                                                          allGoals[resultGoal].goalX, allGoals[resultGoal].goalY);
                            Path *dummy = IllahPathMaker->QuickFindIPath(&FromLastGoalToCheckpoint);

                            if ((!dummy) && (usingShortCut))
                            {
                                // Couldn't find a path, so search in larger
                                // world. - Iterative Deepening.
                                IllahPathMaker->QuickRefreshIGrid(InitialPath,
                                                                  false, false);
                                dummy = IllahPathMaker->QuickFindIPath(&FromLastGoalToCheckpoint);
                                IllahPathMaker->QuickRefreshIGrid(InitialPath);
                                usingShortCut = true;
                            }

                            // This shouldn't happen, but if it does, then
                            // just return PathToDestinationBackup, and
                            // give up on finding checkpoints.
                            if (!dummy)
                            {
                                // First, we have to go through and delete
                                // goalList of any previous paths that may
                                // have been found.
                                for (int k = 0; k < cntr; k++)
                                {
                                    //									if (goalList[k].thePath)
                                    //									{
                                    //										delete goalList[k].thePath;
                                    //									}
                                    goalList[k].thePath = NULL;
                                    goalList[k].calZone = NULL;
                                }

                                //								goalList[0].thePath = new Path(*PathToDestinationBackup);
                                goalList[0].thePath = PathToDestinationBackup;
                                goalList[0].Cached = true;
                                break;
                            }

                            goalList[cntr].thePath = dummy;
                            goalList[cntr].calZone = allGoals[resultGoal].theGoal;
                            goalList[cntr].Cached = true;
                            cntr++;

                            // Find the start of the new path, which is the entry point
                            // of the checkpoint we just found. This path will be from
                            // the new checkpoint to the goal.
                            startX = allGoals[resultGoal].goalX;
                            startY = allGoals[resultGoal].goalY;
                            ShowPath(goalList[cntr].thePath, 2);

                            Path FromCheckpointToDestination(PathInfo->ProbGrid->m_RobotWorld,
                                                             startX, startY,
                                                             PathInfo->EndX, PathInfo->EndY);
                            PathToDestination = IllahPathMaker->QuickFindIPath(&FromCheckpointToDestination);

                            if ((!PathToDestination) && (usingShortCut))
                            {
                                IllahPathMaker->QuickRefreshIGrid(InitialPath, false, false);
                                usingShortCut = false;
                                PathToDestination = IllahPathMaker->QuickFindIPath(&FromCheckpointToDestination);
                            }

                            // This shouldn't happen, but if it does, then
                            // just return PathToDestinationBackup, and
                            // give up on finding checkpoints.
                            if (!PathToDestination)
                            {
                                // First, we have to go through and delete
                                // goalList of any previous paths that may
                                // have been found.
                                for (int k = 0; k < cntr; k++)
                                {
                                    //									if (goalList[k].thePath)
                                    //									{
                                    //										delete goalList[k].thePath;
                                    //									}
                                    goalList[k].thePath = NULL;
                                    goalList[k].calZone = NULL;
                                }

                                //								goalList[0].thePath = new Path(*PathToDestinationBackup);
                                goalList[0].thePath = PathToDestinationBackup;
                                goalList[0].Cached = true;
                                break;
                            }

#ifdef SHOW_DEBUG_MESSAGES
                            char buf[100];
                            memset(buf, 0, sizeof(char) * 100);
                            sprintf(buf, "Using Checkpoint %s",
                                    (allGoals[resultGoal].theGoal)->GetCaption());
                            AfxMessageBox(buf);
#endif
                            resultGoal = IllahPathMaker->FindCheckinPoint(PathToDestination,
                                         allGoals, numGoals);

                        } // End of else condition "if checkpoint is found."

                        ASSERT(cntr < cMaxGoals);
                    }	//End of while loop finding all the paths to the goals

                    if (!bStopFindPath)
                    {
                        GoodPath = true;
                        PostMessage(FindPathThreadWindowHandle,
                                    ID_ROBOT_FIND_PATH_STAGE, 2, 1);
                    }
                }
                else
                {
                    // If (PathToDestination)
                    PostMessage(FindPathThreadWindowHandle,
                                ID_ROBOT_FIND_PATH_STAGE, 2, 0);
                    GoodPath = false;
                }

                //				m_IPathPlanner.ErasePathFromCache(InitialPath);
                delete InitialPath;
                InitialPath = NULL;
            }
            else // Didn't need the planner.
            {
                // If the initial guess was a good enough path
                PostMessage(FindPathThreadWindowHandle, ID_ROBOT_FIND_PATH_STAGE, 2, 1);
                GoodPath = true;
                goalList[0].thePath = InitialPath;
                goalList[0].calZone = NULL;
                goalList[0].Cached = false;
                //				cntr = 1;
            }

            // See if stopped find path
            CGoalSet* GoalSet = NULL;

            if (bStopFindPath)
            {
                //				delete PathToDestinationBackup;
                //				PathToDestinationBackup = NULL;
                for (int i = 0; (goalList[i].thePath != NULL) && (i < cMaxGoals); i++)
                {
                    //					delete (goalList[i].thePath);
                    goalList[i].thePath = NULL;
                }

                GoodPath = false;
            }
            else
            {
                GoalSet = new CGoalSet(goalList);
            }

            ShowPath(NULL, 2);
            TFindPathThreadOutput* ThreadOutput = new TFindPathThreadOutput;
            ThreadOutput->GoalSet = GoalSet;
            ThreadOutput->FoundPath = NULL;
            ThreadOutput->CallBack = PathInfo->CallBack;
            ThreadOutput->CallBackParam1 = PathInfo->CallBackParam1;
            ThreadOutput->CallBackParam2 = PathInfo->CallBackParam2;
            ThreadOutput->GoodPath = GoodPath;

            if (!PostMessage(FindPathThreadWindowHandle, ID_ROBOT_FIND_PATH_FINISHED, 0, (LPARAM)(ThreadOutput)))
            {
                ASSERT(FALSE);
                GetLastError();
            }

            /*Clean up*/
            delete PathInfo;
            PathInfo = NULL;
        }

        /*If stop finding paths, purge the find path queue*/
        if (bStopFindPath || (!ExecuteFindPathQueueThread))
        {
            while (PathInfo = RemoveHeadFromQueue())
            {
                if (PathInfo->CallBack != NULL)
                {
                    PathInfo->CallBack(NULL, PathInfo->CallBackParam1, PathInfo->CallBackParam2);
                }

                delete PathInfo;
                PathInfo = NULL;
            }
        }

        QueueSignal.Unlock();
    }

    return 0;
}

void CFindPathThread::GetListOfGoals(goalStruct *allGoals, int& cntr, TPathThreadInput *PathInfo)
{
    CExecutableRWGraphicObjectListNode* CurrentNode = PathInfo->ProbGrid->m_ExecutableObjectList->Head();

    while (CurrentNode != NULL)
    {
        if (CurrentNode->m_Object->IsKindOf(RUNTIME_CLASS(CPositionLabel)))
        {
            ;
        }
        else if (CurrentNode->m_Object->
                 IsKindOf(RUNTIME_CLASS(CCalibrationZone)))
        {
            // You've found a calibration zone
            CCalibrationZone* FoundCalibrationZone =
                (CCalibrationZone*)(CurrentNode->m_Object);
            // Get the Entry point to the calibration zone
            robPOINT Entrance = FoundCalibrationZone->GetEntryPoint();
            double x, y;
            x = Entrance.x;
            y = Entrance.y;

            // Figure out whether the checkin point is a corner or
            // a flat surface.
            int CkptType;

            if (CurrentNode->m_Object->
                    IsKindOf(RUNTIME_CLASS(CCornerWall)))
            {
                CkptType = 2;
            }
            else
            {
                CkptType = 1;
            }

            // Don't add the checkpoint if it is the goal or the start, or if
            // it is too far from the full path, to avoid long detours
            int Dist = m_IPathPlanner.GetDistanceFromPath(NULL, x, y);

            if ((((abs((int)(x) - PathInfo->EndX) < goalEpsilon) &&
                    (abs((int)(y) - PathInfo->EndY) < goalEpsilon)) ||
                    ((abs((int)(x) - PathInfo->StartX) < goalEpsilon) &&
                     (abs((int)(y) - PathInfo->StartY) < goalEpsilon)))

                    ||

                    ((Dist > MaxDistanceFromPath) || (Dist == 0)))

            {
#ifdef SHOW_DEBUG_MESSAGES
                char buf[100];
                sprintf(buf, "Skipping %s\0",
                        FoundCalibrationZone->GetCaption());
                AfxMessageBox(buf);
#endif
            }
            else // This checkpoint is not near the start or goal, so add it
            {
                robPOINT ep;
                ep = FoundCalibrationZone->GetExitPoint();

                // check the exit point. If it is near the start, don't
                // add it. This happens when we are at the exit point of
                // a checkpoint, which is where the robot goes after it is
                // done checking in.
                if ((abs((int)(ep.x) - (PathInfo->StartX)) < goalEpsilon) &&
                        (abs((int)(ep.y) - (PathInfo->StartY)) < goalEpsilon))
                {
#ifdef SHOW_DEBUG_MESSAGES
                    char buf[100];
                    sprintf(buf, "Skipping Start %s\0",
                            FoundCalibrationZone->GetCaption());
                    AfxMessageBox(buf);
#endif
                    ;
                }
                else // exit point not near start. Add to list.
                {
                    allGoals[cntr].goalX = x;
                    allGoals[cntr].goalY = y;
                    allGoals[cntr].goalType = CkptType;
                    allGoals[cntr].goalVisited = 0;
                    //	allGoals[cntr].goalName = Caption;
                    allGoals[cntr].theGoal = FoundCalibrationZone;
                    cntr++;
#ifdef SHOW_DEBUG_MESSAGES
                    char buf[100];
                    // sprintf(buf, "Adding (%f, %f), s(%d, %d) e(%d, %d)\0",
                    //	x, y, PathInfo->StartX, PathInfo->StartY,
                    //	PathInfo->EndX, PathInfo->EndY);
                    int dist;
                    dist = m_IPathPlanner.GetDistanceFromPath(NULL, x, y);
                    char buf2[100];
                    sprintf(buf2, "%s\0", FoundCalibrationZone->GetCaption());
                    sprintf(buf, "Adding %s, d = %d\0",
                            buf2, dist);
                    AfxMessageBox(buf);
#endif
                }
            }
        }

        CurrentNode = CurrentNode->m_Next;
    }

    // Add the current end point to the goal list as well.
    allGoals[cntr].goalX = PathInfo->EndX;
    allGoals[cntr].goalY = PathInfo->EndY;
    allGoals[cntr].goalType = 0;
    allGoals[cntr].goalVisited = 0;
    allGoals[cntr].theGoal = NULL;
    cntr++;
    ASSERT(cntr < cMaxGoals);
    return;
}

void CFindPathThread::SetStopFindPath()
/*******************************************************************************
//Created: 01/05/98 S.R.
//Last Revision: 01/07/98 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    bStopFindPath = true;
    // ::SetStopATeamRun(1);
}

bool CFindPathThread::GoodPath(Path* FoundPath, bool recalc)
{
    double qualities[5];
    FoundPath->GetQualities(qualities, recalc);
    return (qualities[MinFound] >= cGoodPathMinFoundValue);
}

void CFindPathThread::RemovePathFromCache(double x1, double y1)
{
    m_IPathPlanner.ErasePathFromCache(x1, y1);
}

void CFindPathThread::RemovePathFromCache(Path *Remove)
{
    m_IPathPlanner.ErasePathFromCache(Remove);
}
