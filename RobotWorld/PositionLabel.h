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
 // PositionLabel.h: interface for the CPositionLabel class.
 //
 //////////////////////////////////////////////////////////////////////
 
 #if !defined(AFX_POSITIONLABEL_H__0D769B63_8E5B_11D1_B231_0040F6B87672__INCLUDED_)
 #define AFX_POSITIONLABEL_H__0D769B63_8E5B_11D1_B231_0040F6B87672__INCLUDED_
 
 #if _MSC_VER >= 1000
 #pragma once
 #endif // _MSC_VER >= 1000
 
 #include "ExecutableRWGraphicObject.h"
 #include "RothMath.h"	// Added by ClassView
 #include "ddraw.h"
 #include "Robot.h"	// Added by ClassView
 
 class Path;
 
 class CPositionLabel : public CExecutableRWGraphicObject 
 {
 public:
 	TTaskResult Execute(CRobot* Robot, short Speed);
 	int GetNumberAnimationFrames(void) {return 12;};
 	void SetCaption(CString Caption);
 	unsigned int GetZLayer(void) {return 200;};
 	TRWGraphicObjectType GetObjectType(void) {return goPosition;};
 	void SetEntryPoint(robPOINT point);
 	void Resize(TFrameSide DragSide, robPOINT point);
 	void Move(robPOINT TopCorner);
 	virtual void Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid);
 	void EditProperties(void);
 	CPositionLabel();
 	void SetBounds(robRECT bounds);
 	virtual ~CPositionLabel();
 protected:
 	unsigned int GetAnimatedBitmapID(float ZoomLevel, int AnimationFrame);
 	void SetExitPoint(robPOINT ExitPoint) {CExecutableRWGraphicObject::SetExitPoint(ExitPoint);};
 	static LPDIRECTDRAWSURFACE3 m_lpDDSAnimatedPositionLabelBmp;
 	static LPDIRECTDRAWSURFACE3 m_lpDDSPositionLabelBmp;
 	static float m_OldZoomLevel;
 	static float m_OldAnimationZoomLevel;
 	static int m_OldAnimationFrame;
 private:
 	DECLARE_SERIAL(CPositionLabel)
 };
 
 #endif // !defined(AFX_POSITIONLABEL_H__0D769B63_8E5B_11D1_B231_0040F6B87672__INCLUDED_)
