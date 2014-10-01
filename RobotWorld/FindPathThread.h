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
 ///////////Find Path Thread Stuff//////////////////////////
 #include <afxmt.h>
 #include "IPathPlanner.h"
 
 //UINT FindPathThread(LPVOID pParam);
 //CEvent FindPathFinished(TRUE);
 
 class Path;
 class CRobot;
 class CExecutableRWGraphicObjectListNode;
 class CProbabilityGrid;
 class CCalibrationZone;
 class CGoalSet;
 
 struct TPathThreadInput;
 
 struct TFindPathQueue {
 	TFindPathQueue* Next;
 	TPathThreadInput* ThreadInput;
 };
 
 typedef void(*TFoundPathFunction)(CGoalSet*, UINT, void*);
 
 struct TFindPathThreadOutput {
 	CGoalSet* GoalSet;
 	Path* FoundPath;
 	TFoundPathFunction CallBack;
 	UINT CallBackParam1;
 	void* CallBackParam2;
 	bool GoodPath;
 };
 
 class CFindPathThread {
 public:
 	void RemovePathFromCache(Path* Remove);
 	void RemovePathFromCache(double x1, double y1);
 	static bool GoodPath(Path* FoundPath, bool recalc = false);
 	void SetStopFindPath();
 	CFindPathThread(CRobot* Robot, CWnd* Parent);
 	~CFindPathThread();
 	static void ShowPath(Path* path, int type);
 	void AddToQueue(robPOINT Start, robPOINT End, int Iterations, int Repeat, bool ShowPaths, CProbabilityGrid* ProbGrid, HWND Parent, TFoundPathFunction FoundPathFunction, UINT FoundPathParam1, void* FoundPathParam2);
 	static UINT ThreadFunction(LPVOID pParam);
 	static void DefaultFoundPathFunction(Path* FoundPath, UINT FoundPathParam, void*);
 	static bool bStopFindPath;
 private:
 	static IPathPlanner m_IPathPlanner;
 	static void GetListOfGoals(goalStruct *allGoals, int& cntr, TPathThreadInput* PathInfo);
 	void AddToQueue(TPathThreadInput* ThreadParam);
 	static bool ShowPaths;
 	static CEvent QueueEvent;
 	static HWND FindPathThreadWindowHandle;
 	static bool ExecuteFindPathQueueThread;
 	CWinThread* m_FindPathQueueThread;
 	static CWnd* m_Parent;
 	static CRobot* m_Robot;
 	static TFindPathQueue* QueueTail;
 	static TFindPathQueue* QueueHead;
 	static CCriticalSection QueueLock;
 	static TPathThreadInput* RemoveHeadFromQueue(void);
 };
