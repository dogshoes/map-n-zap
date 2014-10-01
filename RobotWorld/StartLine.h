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
 // StartLine.h: interface for the CStartLine class.
 //
 //////////////////////////////////////////////////////////////////////
 
 #if !defined(AFX_STARTLINE_H__EDD8FF81_BA7F_11D2_A5DD_00105A98F145__INCLUDED_)
 #define AFX_STARTLINE_H__EDD8FF81_BA7F_11D2_A5DD_00105A98F145__INCLUDED_
 
 #if _MSC_VER > 1000
 #pragma once
 #endif // _MSC_VER > 1000
 
 #include "ExecutableRWGraphicObject.h"
 
 class CStartLine : public CRWGraphicObject  
 {
 public:
 	CStartLine();
 	virtual ~CStartLine();
 	unsigned short MinHeight(void) {return 165;};
 	unsigned short MinWidth(void) {return 165;};
 	TRWGraphicObjectType GetObjectType(void) {return goStartLine;};
 	void Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbabilityGrid);
 	void Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid *ProbGrid);
 	bool GetShowCaption(void) {return false;};
 	void OnDefaultClick(robPOINT point, float WorldPointsPerPixel, CRobot* Robot);
 	void SetBounds(robRECT Bounds);
 private:
 	void LoadBitmaps(CProbabilityGrid *ProbGrid);
 	LPDIRECTDRAWSURFACE3 GetStartBitmap(float ZoomLevel);
 	void DrawStart(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid *ProbGrid);
 	static LPDIRECTDRAWSURFACE3 m_lpddsAnimationBmps1[1];
 	static LPDIRECTDRAWSURFACE3 m_lpddsAnimationBmps2[1];
 	static LPDIRECTDRAWSURFACE3 m_lpddsAnimationBmps3[1];
 	DECLARE_SERIAL(CStartLine)
 };
 
 #endif // !defined(AFX_STARTLINE_H__EDD8FF81_BA7F_11D2_A5DD_00105A98F145__INCLUDED_)
