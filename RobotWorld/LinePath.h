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
 // LinePath.h: interface for the CLinePath class.
 //
 //////////////////////////////////////////////////////////////////////
 
 #if !defined(AFX_LINEPATH_H__BC8ACB61_213D_11D2_B231_0040F6B87672__INCLUDED_)
 #define AFX_LINEPATH_H__BC8ACB61_213D_11D2_B231_0040F6B87672__INCLUDED_
 
 #if _MSC_VER >= 1000
 #pragma once
 #endif // _MSC_VER >= 1000
 
 #include "ExecutableRWGraphicObject.h"
 #include "Rothmath.h"	// Added by ClassView
 #include "LineRectTracker.h"
 #include "Robot.h"	// Added by ClassView
 
 class CGoalSet;
 
 class CLinePath : public CExecutableRWGraphicObject  
 {
 public:
 	robPOINT GetEntryPoint(void) {return CExecutableRWGraphicObject::GetEntryPoint();};
 	robPOINT GetEntryPoint(robPOINT From);
 	double DistanceToEntryPoint(robPOINT From);
 	Path* CreatePathToEntryPoint(robPOINT From, CRobot* Robot);
 	CGoalSet* FindPathToEntryPoint(robPOINT From, CRobot* Robot, bool& GoodPath);
 	bool IsPathBetween(robPOINT Start, CExecutableRWGraphicObject* Destination);
 	void MovePoint(POSITION PointListPosition, robPOINT Point);
 	void SetEditMode(TEditMode EditMode);
 //	void DrawTracker(CDC* pDC);
 	void MovePoint(int Index, robPOINT Position);
 	void OnEditClick(robPOINT point, CRobotWorldView * View);
 	void Serialize(CArchive & archive);
 	bool InsideExecuteZone(robPOINT point, float WorldPointsPerPixel);
 	bool InsideObject(robPOINT point, float WorldPointsPerPixel);
 	TTaskResult SpecialFunction(CRobot * Robot, short Speed);
 	void EditProperties(void);
 	unsigned int GetZLayer(void) {return 140;};
 	unsigned int NumPoints(void);
 	void AddPoint(robPOINT Point);
 	CLinePath();
 	virtual ~CLinePath();
 	void Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbabilityGrid);
 	virtual TRWGraphicObjectType GetObjectType(void) {return goLinePath;};
 	BOOL SetTrackerCursor(CRobotWorldView* pWnd, UINT nHitTest);
 private:
 	void AddPointToPath(robPOINT P, CCTDPath **RobotPath, CRobot* Robot);
 //	int m_SelectedPoint;
 protected:
 	CExecutableRWGraphicObject *m_ConnectObject0;
 	CExecutableRWGraphicObject *m_ConnectObject1;
 	void DrawPath(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbabilityGrid);
 	CPolyLineTracker SetupPolyLineTracker(CRobotWorldView* View);
 	DECLARE_SERIAL(CLinePath);
 	CPolyLineTracker SetupPolyLineTracker(CProbabilityGrid* ProbabilityGrid, CPoint SurfaceOffset);
 	void CalculateBounds(void);
 	CList<robPOINT, robPOINT> m_PointList;
 	virtual void SetEntryPoint(robPOINT Point) {CExecutableRWGraphicObject::SetEntryPoint(Point);};
 	virtual void SetExitPoint(robPOINT Point) {CExecutableRWGraphicObject::SetExitPoint(Point);};
 	virtual void SetBounds(robRECT bounds);
 //	CPolyLineTracker m_LineTracker;
 };
 
 #endif // !defined(AFX_LINEPATH_H__BC8ACB61_213D_11D2_B231_0040F6B87672__INCLUDED_)
