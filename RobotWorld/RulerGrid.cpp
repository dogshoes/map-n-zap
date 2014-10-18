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
  // RulerGrid.cpp: implementation of the CRulerGrid class.
  //
  //////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "RulerGrid.h"
  #include "RobotWorldView.h"
  #include "Probability Grid.h"
  #include "WorldGridConsts.h"

  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////
  
  CRulerGrid::CRulerGrid() : CRWGraphicObject()
  {
  	m_ShowCaption = false;
  	m_GridSpacing = 24 * cTPI;
  }
  
  CRulerGrid::~CRulerGrid()
  {
  
  }
  
  bool CRulerGrid::InsideObject(robPOINT point, float WorldPointsPerPixel)
  {
  	return false;
  }
  
  void CRulerGrid::Move(robPOINT TopCorner)
  {
  	return;
  }
  
  void CRulerGrid::Redraw(bool UpdateNow)
  {
  	if (UpdateNow) {
  		m_ParentView->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);
  	} else {
  		m_ParentView->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	}
  }
  
  void CRulerGrid::OnEditClick(robPOINT point, CRobotWorldView *View)
  {
  	return;
  }
  
  void CRulerGrid::Draw(CRect ProbabilityGridUpdateRect, CDC *pDC, CProbabilityGrid *ProbGrid)
  {
  	if (!m_IsVisible) return;
  
  	const double MaxX = cWorldGridXSize * cWorldGridPointSize / 2;
  	const double MaxY = cWorldGridYSize * cWorldGridPointSize / 2;
  
  	CPen RulerPen(PS_SOLID, 1, COLORREF(0xE1F0AE));
  	CPen* OldPen = pDC->SelectObject(&RulerPen);
  	
  	POINT SurfacePoint;
  	robPOINT RobPoint;
  	CPoint SurfaceOffset = ProbabilityGridUpdateRect.TopLeft();
  	robRECT RobotCoordUpdateRect = ProbGrid->ProbGridToRobotCoord(ProbabilityGridUpdateRect);
  	
  	double XStart, XEnd, YStart, YEnd;
  	
  	XStart = ((long)(RobotCoordUpdateRect.left / m_GridSpacing) - 1) * m_GridSpacing;
  	
  	YEnd = ((long)(RobotCoordUpdateRect.top / m_GridSpacing) + 1) * m_GridSpacing;
  
  	XEnd = ((long)(RobotCoordUpdateRect.right / m_GridSpacing) + 1) * m_GridSpacing;
  
  	YStart = ((long)(RobotCoordUpdateRect.bottom / m_GridSpacing) - 1) * m_GridSpacing;
  
  	for (double X = XStart; X < XEnd; X += m_GridSpacing) {
  		RobPoint.x = X;
  		RobPoint.y = RobotCoordUpdateRect.top;
  		SurfacePoint = ProbGrid->RobotToDDSurface(RobPoint, SurfaceOffset);
  		pDC->MoveTo(SurfacePoint);
  
  		RobPoint.y = RobotCoordUpdateRect.bottom;
  		SurfacePoint = ProbGrid->RobotToDDSurface(RobPoint, SurfaceOffset);
  		pDC->LineTo(SurfacePoint);
  	}
  
  	for (double Y = YStart; Y < YEnd; Y += m_GridSpacing) {
  		RobPoint.x = RobotCoordUpdateRect.left;
  		RobPoint.y = Y;
  		SurfacePoint = ProbGrid->RobotToDDSurface(RobPoint, SurfaceOffset);
  		pDC->MoveTo(SurfacePoint);
  
  		RobPoint.x = RobotCoordUpdateRect.right;
  		SurfacePoint = ProbGrid->RobotToDDSurface(RobPoint, SurfaceOffset);
  		pDC->LineTo(SurfacePoint);
  	}
  
  
  /*	for (double X = 0; X < MaxX; X += m_GridSpacing) {
  		RobPoint.x = X;
  		RobPoint.y = MaxY;
  		SurfacePoint = ProbGrid->RobotToDDSurface(RobPoint, SurfaceOffset);
  		pDC->MoveTo(SurfacePoint);
  
  		RobPoint.y = -MaxY;
  		SurfacePoint = ProbGrid->RobotToDDSurface(RobPoint, SurfaceOffset);
  		pDC->LineTo(SurfacePoint);
  
  		RobPoint.x = -X;
  		RobPoint.y = MaxY;
  		SurfacePoint = ProbGrid->RobotToDDSurface(RobPoint, SurfaceOffset);
  		pDC->MoveTo(SurfacePoint);
  
  		RobPoint.y = -MaxY;
  		SurfacePoint = ProbGrid->RobotToDDSurface(RobPoint, SurfaceOffset);
  		pDC->LineTo(SurfacePoint);
  	}
  
  	for (double Y = 0; Y < MaxY; Y += m_GridSpacing) {
  		RobPoint.x = -MaxX;
  		RobPoint.y = Y;
  		SurfacePoint = ProbGrid->RobotToDDSurface(RobPoint, SurfaceOffset);
  		pDC->MoveTo(SurfacePoint);
  
  		RobPoint.x = MaxX;
  		SurfacePoint = ProbGrid->RobotToDDSurface(RobPoint, SurfaceOffset);
  		pDC->LineTo(SurfacePoint);
  
  		RobPoint.x = -MaxX;
  		RobPoint.y = -Y;
  		SurfacePoint = ProbGrid->RobotToDDSurface(RobPoint, SurfaceOffset);
  		pDC->MoveTo(SurfacePoint);
  
  		RobPoint.x = MaxX;
  		SurfacePoint = ProbGrid->RobotToDDSurface(RobPoint, SurfaceOffset);
  		pDC->LineTo(SurfacePoint);
  	}
  */
  	pDC->SelectObject(OldPen);
  	return;
  }
  
  void CRulerGrid::SetGridSpacing(float Spacing)
  {
  	m_GridSpacing = Spacing;
  }
  
  float CRulerGrid::GetGridSpacing()
  {
  	return m_GridSpacing;
  }
