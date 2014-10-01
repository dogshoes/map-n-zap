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
  // PathDisplay.cpp: implementation of the CPathDisplay class.
  //
  //////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "PathDisplay.h"
  #include "CTDPath.h"
  #include "Probability Grid.h"
  #include "RobotWorldView.h"
  
  #ifdef _DEBUG
  #undef THIS_FILE
  static char THIS_FILE[]=__FILE__;
  #define new DEBUG_NEW
  #endif
  
  void CleanPointList(robPOINT* OriginalPointList, int nPoints, TrobPOINTList* CleanedPointList);
  void CleanPointList(TrobPOINTList* OriginalPointList, TrobPOINTList* CleanedPointList);
  void DrawOuterLines(CPoint SurfaceOffset, CDC* pDC, CProbabilityGrid* ProbGrid, TrobPOINTList* PointList);
  void DrawCleanedLines(CPoint SurfaceOffset, CDC* pDC, CProbabilityGrid* ProbGrid, TrobPOINTList* CleanedPointList);
  void DrawOuterLines(CPoint SurfaceOffset, CDC* pDC, CProbabilityGrid* ProbGrid, robPOINT* PointList, int nPoints);
  
  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////
  
  CPathDisplay::CPathDisplay() : CRWGraphicObject()
  {
  	m_ShowCaption = false;
  	m_IsVisible = false;
  	m_Bounds.top = 0;
  	m_Bounds.bottom = 0;
  	m_Bounds.left = 0;
  	m_Bounds.right = 0;
  }
  
  CPathDisplay::~CPathDisplay()
  {
  	m_DisplayPath.RemoveAll();
  }
  
  
  void CPathDisplay::OnEditClick(robPOINT point, CRobotWorldView *View)
  {
  	return;
  }
  
  bool CPathDisplay::GetShowCaption()
  {
  	return false;
  }
  
  unsigned int CPathDisplay::GetZLayer()
  {
  	return 141;
  }
  
  void CPathDisplay::OnDefaultClick(robPOINT point, float WorldPointsPerPixel, CRobot *Robot)
  {
  	return;
  }
  
  void CPathDisplay::SetBounds(robRECT bounds)
  {
  
  }
  
  void CPathDisplay::Draw(CRect ProbabilityGridUpdateRect, CDC *pDC, CProbabilityGrid *ProbGrid)
  {
  	const COLORREF cFindPathPenColor = COLORREF(0x0);
  	if (m_IsVisible) {
  		CPen PathPen(PS_SOLID, 1, cFindPathPenColor);
  		int OldROP2 = pDC->SetROP2(R2_COPYPEN);
  		CPen* OldDCPen = pDC->SelectObject(&PathPen);
  		DrawCTDPath(pDC, &m_DisplayPath, true, ProbGrid, ProbabilityGridUpdateRect);
  		pDC->SelectObject(OldDCPen);
  		pDC->SetROP2(OldROP2);
  		PathPen.DeleteObject();
  	}
  }
  
  TRWGraphicObjectType CPathDisplay::GetObjectType()
  {
  	return goDisplayPath;
  }
  
  void CPathDisplay::SetPath(CCTDPath *path)
  {
  	if (path == NULL) {
  		m_IsVisible = false;
  		m_DisplayPath.RemoveAll();
  		m_Bounds.top = 0;
  		m_Bounds.left = 0;
  		m_Bounds.bottom = 0;
  		m_Bounds.right = 0;
  	} else {
  		m_IsVisible = true;
  		m_DisplayPath.RemoveAll();
  		CleanPointList(path->m_Path, path->m_Size, &m_DisplayPath);
  		robPOINT TopLeft, BottomRight, CurrentPoint;
  		POSITION Position = m_DisplayPath.GetHeadPosition();
  		if (Position != NULL) {
  			CurrentPoint = m_DisplayPath.GetNext(Position);
  			TopLeft.x = BottomRight.x = CurrentPoint.x;
  			TopLeft.y = BottomRight.y = CurrentPoint.y;
  		}
  		while (Position != NULL) {
  			robPOINT CurrentPoint = m_DisplayPath.GetNext(Position);
  			if (CurrentPoint.x < TopLeft.x) TopLeft.x = CurrentPoint.x;
  			if (CurrentPoint.y > TopLeft.y) TopLeft.y = CurrentPoint.y;
  			if (CurrentPoint.x > BottomRight.x) BottomRight.x = CurrentPoint.x;
  			if (CurrentPoint.y < BottomRight.y) BottomRight.y = CurrentPoint.y;
  		}
  		
  
  		m_Bounds.top = TopLeft.y + cRRWidth/2;
  		m_Bounds.left = TopLeft.x - cRRWidth/2;
  		m_Bounds.bottom = BottomRight.y - cRRWidth/2;
  		m_Bounds.right = BottomRight.x + cRRWidth/2;
  	}
  }
  
  void CleanPointList(robPOINT* OriginalPointList, int nPoints, TrobPOINTList* CleanedPointList)
  /*Removes consecutive colinear points from the original list*/
  {
  	if ( (OriginalPointList == NULL) || (CleanedPointList == NULL) ) return;
  	robPOINT P1, P2, P3;
  	CleanedPointList->AddTail(OriginalPointList[0]);
  	for (unsigned short i = 1; i < nPoints; i++) {
  		P1 = OriginalPointList[i-1];
  		P2 = OriginalPointList[i];
  		double slope = 0.0;
  		bool Vertical = false;
  		if (P2.x == P1.x) {
  			Vertical = true;
  		} else {
  			Vertical = false;
  			slope = (P2.y - P1.y) / (P2.x - P1.x);
  		}
  		P3.x = P2.x;
  		P3.y = P2.y;
  		while ( ( Distance(P3, P2) < 1 ) && (i < nPoints - 1) ) {
  			i++;
  			if (i < nPoints) {
  				P2 = OriginalPointList[i];
  			}
  			if (Vertical) {
  				P3.y = P2.y;
  			} else {
  				P3.x = P2.x;
  				P3.y = slope * (P3.x - P1.x) + P1.y;
  			}
  		}
  		if (i < (nPoints - 1)) {
  			i--;
  			P2 = OriginalPointList[i];
  			CleanedPointList->AddTail(P2);
  		} else {
  			if (Distance(P3, P2) >= 1) {
  				P2 = OriginalPointList[i-1];
  				CleanedPointList->AddTail(P2);
  				CleanedPointList->AddTail(OriginalPointList[nPoints-1]);
  			} else {
  				CleanedPointList->AddTail(OriginalPointList[nPoints-1]);
  			}
  		}
  	}
  }
  
  void CleanPointList(TrobPOINTList* OriginalPointList, TrobPOINTList* CleanedPointList)
  /*Removes consecutive colinear points from the original list*/
  {
  	if ( (OriginalPointList == NULL) || (CleanedPointList == NULL) ) return;
  	int nPoints = OriginalPointList->GetCount();
  	robPOINT P1, P2, P3;
  	CleanedPointList->AddTail(OriginalPointList->GetHead());
  	POSITION Position = OriginalPointList->GetHeadPosition();
  	P1 = OriginalPointList->GetNext(Position);
  	while (Position != NULL) {
  		P2 = OriginalPointList->GetNext(Position);
  		double slope = 0.0;
  		bool Vertical = false;
  		if (P2.x == P1.x) {
  			Vertical = true;
  		} else {
  			Vertical = false;
  			slope = (P2.y - P1.y) / (P2.x - P1.x);
  		}
  		P3.x = P2.x;
  		P3.y = P2.y;
  		POSITION PreviousPosition = NULL;
  		while ( ( Distance(P3, P2) < 1 ) && (Position != NULL) ) {
  			PreviousPosition = Position;
  			P2 = OriginalPointList->GetNext(Position);
  			if (Vertical) {
  				P3.y = P2.y;
  			} else {
  				P3.x = P2.x;
  				P3.y = slope * (P3.x - P1.x) + P1.y;
  			}
  		}
  		if (Position != NULL) {
  			Position = PreviousPosition;
  			OriginalPointList->GetPrev(Position);
  			P2 = OriginalPointList->GetNext(Position);
  			CleanedPointList->AddTail(P2);
  		} else {
  			if (Distance(P3, P2) >= 1) {
  				Position = PreviousPosition;
  				OriginalPointList->GetPrev(Position);
  				P2 = OriginalPointList->GetNext(Position);
  				CleanedPointList->AddTail(P2);
  				CleanedPointList->AddTail(OriginalPointList->GetNext(Position));	//Add Tail
  			} else {
  				CleanedPointList->AddTail(OriginalPointList->GetTail());
  			}
  		}
  		P1 = P2;
  	}
  }
  
  void DrawCleanedLines(CPoint SurfaceOffset, CDC* pDC, CProbabilityGrid* ProbGrid, TrobPOINTList* CleanedPointList)
  {
  	if (CleanedPointList == NULL) return;
  	robPOINT P1, P2, P3;
  	CPoint P;
  	POSITION Position;
  	int nPoints = CleanedPointList->GetCount();
  	if (nPoints > 1) {
  		robPOINT P1a, P2a, P3a, P1b, P2b, P3b;
  		Position = CleanedPointList->GetHeadPosition();
  		P1 = CleanedPointList->GetNext(Position);
  		P2 = CleanedPointList->GetNext(Position);
  		double theta1 = atan2(P2.y - P1.y, P2.x - P1.x);
  		P1a.x = P1.x + cRRWidth/2*cos(theta1+pi/2);
  		P1a.y = P1.y + cRRWidth/2*sin(theta1+pi/2);
  		P1b.x = P1.x + cRRWidth/2*cos(theta1-pi/2);
  		P1b.y = P1.y + cRRWidth/2*sin(theta1-pi/2);
  		bool AOverlap = false;
  		bool BOverlap = false;
  		
  		for (unsigned short i = 1; i < nPoints; i++) {
  			P1 = CleanedPointList->GetAt(CleanedPointList->FindIndex(i-1));
  			P2 = CleanedPointList->GetAt(CleanedPointList->FindIndex(i));
  			double slope = (P2.y - P1.y) / (P2.x - P1.x);
  			if (i+1 < nPoints) {
  				P3 = CleanedPointList->GetAt(CleanedPointList->FindIndex(i+1));
  				double theta2;
  				theta1 = atan2(P2.y - P1.y, P2.x - P1.x);
  				theta2 = atan2(P3.y - P2.y, P3.x - P2.x);
  
  				robPOINT C;				
  				double phi = BringAngleInRange(theta2 - theta1);
  				
  				if (phi < pi) {
  					double theta = (pi + phi) / 2 + theta1;
  					
  					double d = (cRRWidth/2) / cos(phi/2);
  					P2a.x = P2.x + d * cos(theta);
  					P2a.y = P2.y + d * sin(theta);
  				/*Handle Overlapping stuff if necessary*/
  					P3a.x = P3.x + cRRWidth/2 * cos(theta2 + pi/2);
  					P3a.y = P3.y + cRRWidth/2 * sin(theta2 + pi/2);
  					TLine L1;
  					FindLineFrom2Points(P1, P2, &L1);
  					bool newAOverlap;
  					bool ExtremeOverlap = false;
  					if (PerpendicularDistanceFromLine(L1, P3a) < cRRWidth/2) {
  						newAOverlap = true;
  						robPOINT Intersection;
  						Intersection = FindPerpendicularIntersection(L1, P3);
  						P2a.x = Intersection.x + cRRWidth/2 * cos(theta1 + pi/2);
  						P2a.y = Intersection.y + cRRWidth/2 * sin(theta1 + pi/2);
  					} else {
  						newAOverlap = false;
  					}
  					if (!AOverlap) {
  						P = ProbGrid->RobotToDDSurface(P1a, SurfaceOffset);
  						pDC->MoveTo(P);
  						P = ProbGrid->RobotToDDSurface(P2a, SurfaceOffset);
  						pDC->LineTo(P);
  					}
  					AOverlap = newAOverlap;
  
  					P1a = P2a;
  
  					P2b.x = P2.x + cRRWidth/2*cos(theta1 - pi/2);
  					P2b.y = P2.y + cRRWidth/2*sin(theta1 - pi/2);
  					P3b.x = P2.x + cRRWidth/2*cos(theta2 - pi/2);
  					P3b.y = P2.y + cRRWidth/2*sin(theta2 - pi/2);
  					C = P2;
  
  					P = ProbGrid->RobotToDDSurface(P1b, SurfaceOffset);
  					pDC->MoveTo(P);
  					CPoint cP1, cP2;
  					cP1 = ProbGrid->RobotToDDSurface(P2b, SurfaceOffset);
  					cP2 = ProbGrid->RobotToDDSurface(P3b, SurfaceOffset);
  					
  					if (!BOverlap) {
  						pDC->LineTo(cP1);
  					} else {
  						BOverlap = false;
  					}
  				/*Calculate rectangle enclosing the turning circle*/
  					robPOINT CircleRectTopLeft, CircleRectBottomRight;
  					CircleRectTopLeft.x = C.x - cRRWidth/2;
  					CircleRectTopLeft.y = C.y + cRRWidth/2;
  					CircleRectBottomRight.x = C.x + cRRWidth/2;
  					CircleRectBottomRight.y = C.y - cRRWidth/2;
  					CRect CircleRectangle;
  					CircleRectangle.TopLeft() = ProbGrid->RobotToDDSurface(CircleRectTopLeft, SurfaceOffset);
  					CircleRectangle.BottomRight() = ProbGrid->RobotToDDSurface(CircleRectBottomRight, SurfaceOffset);
  					CPoint ArcCenter = ProbGrid->RobotToDDSurface(C, SurfaceOffset);
  
  					if ( abs(int(atan2(cP2.y - ArcCenter.y, cP2.x - ArcCenter.x) - atan2(cP1.y - ArcCenter.y, cP1.x - ArcCenter.x))) >= (1.0 / 16) ) {
  						pDC->Arc(CircleRectangle, cP1, cP2);
  					} else {
  						pDC->LineTo(cP2);
  					}
  					P1b = P3b;
  
  					P1 = P2;
  				} else {
  					P2a.x = P2.x + cRRWidth/2*cos(theta1 + pi/2);
  					P2a.y = P2.y + cRRWidth/2*sin(theta1 + pi/2);
  					P3a.x = P2.x + cRRWidth/2*cos(theta2 + pi/2);
  					P3a.y = P2.y + cRRWidth/2*sin(theta2 + pi/2);
  					C = P2;
  
  					P = ProbGrid->RobotToDDSurface(P1a, SurfaceOffset);
  					pDC->MoveTo(P);
  					CPoint cP1, cP2;
  					cP1 = ProbGrid->RobotToDDSurface(P2a, SurfaceOffset);
  					cP2 = ProbGrid->RobotToDDSurface(P3a, SurfaceOffset);
  					if (!AOverlap) {
  						pDC->LineTo(cP1);
  					} else {
  						AOverlap = false;
  					}
  				/*Calculate rectangle enclosing the turning circle*/
  					robPOINT CircleRectTopLeft, CircleRectBottomRight;
  					CircleRectTopLeft.x = C.x - cRRWidth/2;
  					CircleRectTopLeft.y = C.y + cRRWidth/2;
  					CircleRectBottomRight.x = C.x + cRRWidth/2;
  					CircleRectBottomRight.y = C.y - cRRWidth/2;
  					CRect CircleRectangle;
  					CircleRectangle.TopLeft() = ProbGrid->RobotToDDSurface(CircleRectTopLeft, SurfaceOffset);
  					CircleRectangle.BottomRight() = ProbGrid->RobotToDDSurface(CircleRectBottomRight, SurfaceOffset);
  					CPoint ArcCenter = ProbGrid->RobotToDDSurface(C, SurfaceOffset);
  
  					if ( abs(int(atan2(cP2.y - ArcCenter.y, cP2.x - ArcCenter.x) - atan2(cP1.y - ArcCenter.y, cP1.x - ArcCenter.x))) >= (1.0 / 16) ) {
  						pDC->Arc(CircleRectangle, cP2, cP1);
  					} else {
  						pDC->LineTo(cP2);
  					}
  					P1a = P3a;
  
  
  					phi = -phi;
  					double theta = theta1 - (pi + phi)/2;
  					double d = (cRRWidth/2) / cos(phi/2);
  					P2b.x = P2.x + d*cos(theta);
  					P2b.y = P2.y + d*sin(theta);
  				/*Handle Overlapping stuff if necessary*/
  					P3b.x = P3.x + cRRWidth/2 * cos(theta2 - pi/2);
  					P3b.y = P3.y + cRRWidth/2 * sin(theta2 - pi/2);
  					TLine L1;
  					FindLineFrom2Points(P1, P2, &L1);
  					bool newBOverlap;
  					bool ExtremeOverlap = false;
  					if (PerpendicularDistanceFromLine(L1, P3b) < cRRWidth/2) {
  						newBOverlap = true;
  						robPOINT Intersection;
  						Intersection = FindPerpendicularIntersection(L1, P3);
  						P2b.x = Intersection.x + cRRWidth/2 * cos(theta1 - pi/2);
  						P2b.y = Intersection.y + cRRWidth/2 * sin(theta1 - pi/2);
  					} else {
  						newBOverlap = false;
  					}
  					if (!BOverlap) {
  						P = ProbGrid->RobotToDDSurface(P1b, SurfaceOffset);
  						pDC->MoveTo(P);
  						P = ProbGrid->RobotToDDSurface(P2b, SurfaceOffset);
  						pDC->LineTo(P);
  					}
  					BOverlap = newBOverlap;
  
  					P1b = P2b;
  				/*Overlapping Stuff End*/
  					P1 = P2;
  				}
  				
  			} else {
  				theta1 = atan2(P2.y - P1.y, P2.x - P1.x);
  				P2a.x = P2.x + cRRWidth/2*cos(theta1+pi/2);
  				P2a.y = P2.y + cRRWidth/2*sin(theta1+pi/2);
  				P = ProbGrid->RobotToDDSurface(P1a, SurfaceOffset);
  				pDC->MoveTo(P);
  				P = ProbGrid->RobotToDDSurface(P2a, SurfaceOffset);
  				if (!AOverlap) {
  					pDC->LineTo(P);
  				}
  				P2b.x = P2.x + cRRWidth/2*cos(theta1-pi/2);
  				P2b.y = P2.y + cRRWidth/2*sin(theta1-pi/2);
  				P = ProbGrid->RobotToDDSurface(P1b, SurfaceOffset);
  				pDC->MoveTo(P);
  				P = ProbGrid->RobotToDDSurface(P2b, SurfaceOffset);
  				if (!BOverlap) {
  					pDC->LineTo(P);
  				}
  			}
  
  		}
  	}
  }
  
  void DrawOuterLines(CPoint SurfaceOffset, CDC* pDC, CProbabilityGrid* ProbGrid, TrobPOINTList* PointList)
  /*Draws the path's outside lines*/
  {
  	if (PointList == NULL) return;
  	TrobPOINTList CleanedPointList;
  	CleanPointList(PointList, &CleanedPointList);
  	DrawCleanedLines(SurfaceOffset, pDC, ProbGrid, &CleanedPointList);
  }
  
  void DrawOuterLines(CPoint SurfaceOffset, CDC* pDC, CProbabilityGrid* ProbGrid, robPOINT* PointList, int nPoints)
  /*Draws the path's outside lines*/
  {
  	if (PointList == NULL) return;
  	TrobPOINTList CleanedPointList;
  	CleanPointList(PointList, nPoints, &CleanedPointList);
  	DrawCleanedLines(SurfaceOffset, pDC, ProbGrid, &CleanedPointList);
  }
  
  void CPathDisplay::DrawCTDPath(CDC* pdc, TrobPOINTList* path, bool ShowRobotBounds, CProbabilityGrid *ProbGrid, CRect ProbabilityGridUpdateRect)
  {
  	robPOINT P1, P2, P3;
  	if (path == NULL) return;
  	POSITION Position = path->GetHeadPosition();
  	if (Position == NULL) return;
  
  	P1 = path->GetNext(Position);
  	CPoint P;
  	CPoint SurfaceOffset = ProbabilityGridUpdateRect.TopLeft();
  	P = ProbGrid->RobotToDDSurface(P1, SurfaceOffset);
  	pdc->MoveTo(P);
  	while (Position != NULL) {
  		P1 = path->GetNext(Position);
  
  		P = ProbGrid->RobotToDDSurface(P1, SurfaceOffset);
  		pdc->LineTo(P);
  	}
  	
  	if (ShowRobotBounds) {
  		
  		DrawCleanedLines(SurfaceOffset, pdc, ProbGrid, path);
  
  	}
  }
  
  void CPathDisplay::Redraw(bool UpdateNow)
  {
  	ASSERT(m_ParentView != NULL);
  	if (m_ParentView == NULL) return;
  	
  	if (!m_IsVisible) return;
  	robRECT RobotUpdate = m_Bounds;
  	CRect UpdateRect = m_ParentView->RobotToClientCoords(RobotUpdate);
  	if (UpdateNow) {
  		m_ParentView->RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);
  	} else {
  		m_ParentView->RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	}
  }
