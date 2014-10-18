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
  // Red Zone.cpp : implementation file
  //
  
  #include "stdafx.h"
  //#include "RobotWorld.h"
  #include "RobotWorld Resource.h"
  #include "Red Zone.h"
  #include "Probability Grid.h"
  #include "Global.h"
  #include "RobotWorldView.h"

  /////////////////////////////////////////////////////////////////////////////
  // CRedZone
  
  IMPLEMENT_SERIAL(CRedZone, CRWGraphicObject, VERSIONABLE_SCHEMA | VERSION_NUMBER)
  LPDIRECTDRAWSURFACE3 CRedZone::m_ZoneBitmap = NULL;
  
  CRedZone::CRedZone() : CRWGraphicObject()
  {
  	m_ShowCaption = false;
  	m_FirstSetBoundsCall = true;
  	m_LeftOldGridValues = NULL;
  	m_RightOldGridValues = NULL;
  	m_TopOldGridValues = NULL;
  	m_BottomOldGridValues = NULL;
  	m_Bounds.left = m_Bounds.top = 0;
  	m_Bounds.right = MinWidth();
  	m_Bounds.bottom = -MinHeight();
  	m_ProbabilityGrid = NULL;
  }
  
  CRedZone::CRedZone(robRECT Bounds, CProbabilityGrid* ProbGrid) : CRWGraphicObject()
  {
  	m_ShowCaption = false;
  	m_FirstSetBoundsCall = true;
  	m_LeftOldGridValues = NULL;
  	m_RightOldGridValues = NULL;
  	m_TopOldGridValues = NULL;
  	m_BottomOldGridValues = NULL;
  	m_ProbabilityGrid = ProbGrid;
  	SetBounds(Bounds);
  }
  
  void CRedZone::Draw(CRect ProbabilityGridUpdateRect, CDC* pDC, CProbabilityGrid* ProbGrid)
  {
  	if (!m_IsVisible) return;
  
  	int OldPolyFillMode = pDC->SetPolyFillMode(ALTERNATE);
  	CGdiObject* OldPen = pDC->SelectStockObject(BLACK_PEN);
  	CGdiObject* OldBrush = pDC->SelectStockObject(BLACK_BRUSH);
  	
  	CPen YellowPen;
  	YellowPen.CreatePen(PS_SOLID, 1, COLORREF(0X00FFFF));
  	CBrush YellowBrush;
  	YellowBrush.CreateSolidBrush(COLORREF(0X00FFFF));
  
  /*Find RedZone bounds on surface*/
  	robPOINT point;
  	point = m_Bounds.TopLeft();
  	point.x -= cWorldGridPointSize / 2;
  	point.y += cWorldGridPointSize / 2;
  	POINT TopLeft = ProbGrid->RobotToDDSurface(point, ProbabilityGridUpdateRect.TopLeft());
  	point = m_Bounds.BottomRight();
  	point.x += cWorldGridPointSize / 2;
  	point.y -= cWorldGridPointSize / 2;
  	POINT BottomRight = ProbGrid->RobotToDDSurface(point, ProbabilityGridUpdateRect.TopLeft());
  	CRect Bounds;
  	Bounds.top = TopLeft.y;
  	Bounds.left = TopLeft.x;
  	Bounds.bottom = BottomRight.y;
  	Bounds.right = BottomRight.x;
  	Bounds.NormalizeRect();
  
  /*	Bounds.top -= 2;
  	Bounds.bottom += 2;
  	Bounds.left -= 2;
  	Bounds.right += 2;
  */
  	POINT P[4];
  	int BlackIndent = 2;
  //	if ((BlackIndent >= 2*Bounds.Width()) || (BlackIndent >= 2*Bounds.Height())) BlackIndent = 0;
  
  	int PointSpacing;
  	if (ProbGrid->GetZoomLevel() == cZoomLevel3) PointSpacing = 8;
  	else if (ProbGrid->GetZoomLevel() == cZoomLevel2) PointSpacing = 4;
  	else PointSpacing = 2;
  
  	P[0].y = Bounds.bottom;
  	P[0].x = Bounds.left;
  	P[3] = P[0];
  	bool Done = false;
  	bool LeftSideDone = false;
  	bool BottomSideDone = false;
  	bool YellowStripe = true;
  	bool DrawTopLeftCorner = false;
  	bool DrawBottomRightCorner = false;
  	while (!Done) {
  		if (!LeftSideDone) {
  			P[1].x = Bounds.left;
  			P[1].y = P[0].y - PointSpacing;
  			if (P[1].y <= Bounds.top) {
  				LeftSideDone = true;
  				DrawTopLeftCorner = true;
  				P[1].x += Bounds.top - P[1].y;
  				if (P[1].x >= Bounds.right) {
  					P[1].x = Bounds.right;
  					Done = true;
  				}
  				P[1].y = Bounds.top;
  			}
  		} else {
  			P[1].y = Bounds.top;
  			P[1].x = P[0].x + PointSpacing;
  			if (P[1].x >= Bounds.right) {
  				P[1].x = Bounds.right;
  				Done = true;
  			}
  		}
  
  		if (!BottomSideDone) {
  			P[2].y = Bounds.bottom;
  			P[2].x = P[3].x + PointSpacing;
  			if (P[2].x >= Bounds.right) {
  				BottomSideDone = true;
  				DrawBottomRightCorner = true;
  				P[2].y -= P[2].x - Bounds.right;
  				if (P[2].y <= Bounds.top) {
  					P[2].y = Bounds.top;
  					Done = true;
  				}
  				P[2].x = Bounds.right;
  			}
  		} else {
  			P[2].x = Bounds.right;
  			P[2].y = P[3].y - PointSpacing;
  			if (P[2].y <= Bounds.top) {
  				P[2].y = Bounds.top;
  				Done = true;
  			}
  		}
  
  		POINT PolygonPoints[6];
  		int nPolygonPoints = 4;
  		if (YellowStripe) {
  			YellowStripe = false;
  			pDC->SelectObject(YellowBrush);
  			pDC->SelectObject(YellowPen);
  
  			int Indent = -BlackIndent;
  /*			if ( (P[0].x + 2) >= (P[3].x - 2) ) {
  				Indent = 0;
  			}
  			if ( (P[1].x + 2) >= (P[2].x - 2) ) {
  				Indent = 0;
  			}
  */
  			PolygonPoints[0].x = P[0].x + Indent;
  			PolygonPoints[0].y = P[0].y + Indent;
  			PolygonPoints[1].x = P[1].x + Indent;
  			PolygonPoints[1].y = P[1].y + Indent;
  			PolygonPoints[2].x = P[2].x - Indent;
  			PolygonPoints[2].y = P[2].y - Indent;
  			PolygonPoints[3].x = P[3].x - Indent;
  			PolygonPoints[3].y = P[3].y - Indent;
  
  			if (DrawTopLeftCorner) {
  				DrawTopLeftCorner = false;
  				PolygonPoints[4] = PolygonPoints[3];
  				PolygonPoints[3] = PolygonPoints[2];
  				PolygonPoints[2] = PolygonPoints[1];
  				PolygonPoints[1].x = Bounds.left + Indent;
  				PolygonPoints[1].y = Bounds.top + Indent;
  				nPolygonPoints++;
  			}
  			if (DrawBottomRightCorner) {
  				DrawBottomRightCorner = false;
  				PolygonPoints[nPolygonPoints] = PolygonPoints[nPolygonPoints-1];
  				PolygonPoints[nPolygonPoints-1].x = Bounds.right - Indent;
  				PolygonPoints[nPolygonPoints-1].y = Bounds.bottom - Indent;
  				nPolygonPoints++;
  			}
  
  /*			PolygonPoints[0].x = P[0].x + BlackIndent;
  			PolygonPoints[0].y = P[0].y + BlackIndent;
  			PolygonPoints[1].x = P[1].x + BlackIndent;
  			PolygonPoints[1].y = P[1].y + BlackIndent;
  			PolygonPoints[2].x = P[2].x - BlackIndent;
  			PolygonPoints[2].y = P[2].y - BlackIndent;
  			PolygonPoints[3].x = P[3].x - BlackIndent;
  			PolygonPoints[3].y = P[3].y - BlackIndent;
  */
  		} else {
  			YellowStripe = true;
  			pDC->SelectStockObject(BLACK_BRUSH);
  			pDC->SelectStockObject(BLACK_PEN);
  
  			for (int i = 0; i < 4; i++) {
  				PolygonPoints[i] = P[i];
  			}
  			if (DrawTopLeftCorner) {
  				DrawTopLeftCorner = false;
  				PolygonPoints[4] = PolygonPoints[3];
  				PolygonPoints[3] = PolygonPoints[2];
  				PolygonPoints[2] = PolygonPoints[1];
  				PolygonPoints[1] = Bounds.TopLeft();
  				nPolygonPoints++;
  			}
  			if (DrawBottomRightCorner) {
  				DrawBottomRightCorner = false;
  				PolygonPoints[nPolygonPoints] = PolygonPoints[nPolygonPoints-1];
  				PolygonPoints[nPolygonPoints-1] = Bounds.BottomRight();
  				nPolygonPoints++;
  			}
  		
  		}
  //		if (!YellowStripe) 
  		pDC->Polygon(PolygonPoints, nPolygonPoints);
  
  		P[0] = P[1];
  		P[3] = P[2];
  	}
  
  	pDC->SelectObject(OldBrush);
  	pDC->SelectObject(OldPen);
  	pDC->SetPolyFillMode(OldPolyFillMode);
  
  	if (m_EditMode != emNone) {
  		CRectTracker Tracker = SetupTracker(ProbGrid, ProbabilityGridUpdateRect.TopLeft());
  		Tracker.Draw(pDC);
  	}
  }
  
  void CRedZone::Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid)
  {
  #ifdef JUNK
  	if (!m_IsVisible) return;
  const int cRedZoneBmpHeight = 99;
  const int cRedZoneBmpWidth = 99;
  	if (m_ZoneBitmap == NULL) {
  		m_ZoneBitmap = ProbGrid->DDrawLoadBitmap(IDB_REDZONE);
  		if( m_ZoneBitmap == NULL ) {
  			fprintf(stderr, "Load RedZone Bitmap Failed\n");
  			fflush(stderr);
  		}
  	}
  	if (m_ZoneBitmap->IsLost() == DDERR_SURFACELOST) {
  		m_ZoneBitmap->Restore();
  	}
  
  
  	if (m_ZoneBitmap != NULL) {
  		CRect rcRect, destRect;
  		HRESULT ddrval;
  
  		rcRect.left = 0;
  		rcRect.top = 0;
  		rcRect.right = rcRect.left + cRedZoneBmpWidth;
  		rcRect.bottom = rcRect.top + cRedZoneBmpHeight;
  		float temp;
  		temp = ProbGrid->RobotToDDSurfaceX(m_Bounds.left, ProbabilityGridUpdateRect.left);
  		destRect.left = Round(temp);
  		destRect.right = Round(ProbGrid->RobotToDDSurfaceX(m_Bounds.right, ProbabilityGridUpdateRect.left));
  		temp = ProbGrid->RobotToDDSurfaceY(m_Bounds.top, ProbabilityGridUpdateRect.top);
  		destRect.top = Round(temp);
  		destRect.bottom = Round(ProbGrid->RobotToDDSurfaceY(m_Bounds.bottom, ProbabilityGridUpdateRect.top));
  		ProbGrid->RectifyStretchedRectangles(&rcRect, &destRect);
  		if ( (destRect.Height() != 0) && (destRect.Width() != 0) && (rcRect.Height() != 0) && (rcRect.Width() != 0) ) {
  			int i = 0;
  			while (BackBuffer->GetBltStatus(DDGBS_CANBLT) != DD_OK);
  			ddrval = BackBuffer->Blt(&destRect, m_ZoneBitmap, &rcRect, 0, NULL);
  			if (ddrval != DD_OK) {
  				fprintf(stderr, "Draw Calibration Zones Failed\n");
  				fprintf(stderr, "***DrawRobot Error***\n");
  				fprintf(stderr, "destRect = %d %d %d %d\n",destRect.left, destRect.top, destRect.right, destRect.bottom);
  				fprintf(stderr, "rcRect = %d %d %d %d\n", rcRect.left,rcRect.top, rcRect.right, rcRect.bottom);
  				fflush(stderr);
  			}
  		}
  	}
  #endif
  }
  
  CRedZone::~CRedZone()
  {
  	delete m_LeftOldGridValues;
  	delete m_RightOldGridValues;
  	delete m_TopOldGridValues;
  	delete m_BottomOldGridValues;
  	if (m_ZoneBitmap != NULL) {
  		m_ZoneBitmap->Release();
  		m_ZoneBitmap = NULL;
  	}
  }
  
  CRedZoneListNode::~CRedZoneListNode()
  /*******************************************************************************
  //Created: 07/09/97 S.R.
  //Last Revision: 07/09/97 S.R.
  //Parameters:
  //	none
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  }
  
  void CRedZoneList::Remove(CRedZone * RedZone)
  /*******************************************************************************
  //Created: 07/09/97 S.R.
  //Last Revision: 07/09/97 S.R.
  //Parameters:
  //	RedZone:	pointer to red zone to remove from the list.
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //		This function removes a red zone from the list of all red zones, and
  //	destroys the window.
  //Method:
  //
  *******************************************************************************/
  {
  	CRedZoneListNode* CurrentNode = m_RedZoneListHead;
  	while (CurrentNode != NULL) {
  		if (CurrentNode->m_RedZone == RedZone) {
  			if (CurrentNode->m_Next != NULL) {
  				CurrentNode->m_Next->m_Previous = CurrentNode->m_Previous;
  			}
  			if (CurrentNode->m_Previous != NULL) {
  				CurrentNode->m_Previous->m_Next = CurrentNode->m_Next;
  			} else {
  				m_RedZoneListHead = CurrentNode->m_Next;
  			}
  			delete CurrentNode;
  			return;
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  }
  
  CRedZoneList::~CRedZoneList()
  /*******************************************************************************
  //Created: 07/09/97 S.R.
  //Last Revision: 07/09/97 S.R.
  //Parameters:
  //	none
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	Purge();
  }
  
  CRedZoneList::CRedZoneList()
  /*******************************************************************************
  //Created: 07/09/97 S.R.
  //Last Revision: 07/09/97 S.R.
  //Parameters:
  //	none
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	m_RedZoneListHead = NULL;
  	m_ProbabilityGrid = NULL;
  }
  
  CRedZoneList::CRedZoneList(CProbabilityGrid* ProbGrid)
  /*******************************************************************************
  //Created: 07/09/97 S.R.
  //Last Revision: 07/09/97 S.R.
  //Parameters:
  //	none
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	m_RedZoneListHead = NULL;
  	m_ProbabilityGrid = ProbGrid;
  }
  
  void CRedZoneList::Purge()
  /*******************************************************************************
  //Created: 07/09/97 S.R.
  //Last Revision: 07/09/97 S.R.
  //Parameters:
  //	none
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //	Removes all red zones from the red zone list, and destroys all the windows.
  //Method:
  //
  *******************************************************************************/
  {
  	while (m_RedZoneListHead != NULL) {
  		CRedZoneListNode* tempNode;
  		tempNode = m_RedZoneListHead->m_Next;
  		delete m_RedZoneListHead;
  		m_RedZoneListHead = tempNode;
  	}
  }
  
  CRedZone* CRedZoneList::Add(robRECT WorldRect)
  /*******************************************************************************
  //Created: 07/09/97 S.R.
  //Last Revision: 07/09/97 S.R.
  //Parameters:
  //	View	The view where the red zone will be displayed
  //	WorldRect	The rectangle (in world coordinates) where the red zone is located.
  //Return Value:
  //	a pointer to the red zone created.
  ********************************************************************************
  //Description:
  //	This function creates and adds a red zone to the red zone list.
  //Method:
  //
  *******************************************************************************/
  {
  	CRedZone* RedZone = new CRedZone(WorldRect, m_ProbabilityGrid);
  	Add(RedZone);
  	return RedZone;
  }
  
  void CRedZoneList::Add(CRedZone* RedZone)
  {
  	CRedZoneListNode* tempNode = m_RedZoneListHead;
  	m_RedZoneListHead = new CRedZoneListNode;
  	m_RedZoneListHead->m_RedZone = RedZone;
  	m_RedZoneListHead->m_Next = tempNode;
  	m_RedZoneListHead->m_Previous = NULL;
  	if (tempNode != NULL) tempNode->m_Previous = m_RedZoneListHead;
  	RedZone->SetRedZoneList(this);
  }
  
  BOOL CRedZoneList::InRedZone(robPOINT WorldPoint)
  /*******************************************************************************
  //Created: 07/09/97 S.R.
  //Last Revision: 07/09/97 S.R.
  //Parameters:
  //	WorldPoint	point that might be inside of a red zone. (in world coordinates)
  //Return Value:
  //	TRUE	WorldPoint is inside of a red zone
  //	FALSE	WorldPoint is outside of all the red zones.
  ********************************************************************************
  //Description:
  //		This function searches through the red zone list, and sees if WorldPoint
  //	is inside any of the red zones.
  //Method:
  //
  *******************************************************************************/
  {
  	CRedZoneListNode* CurrentNode = m_RedZoneListHead;
  	while (CurrentNode != NULL) {
  		if (CurrentNode->m_RedZone->InsideObject(WorldPoint, 0)) return TRUE;
  		CurrentNode = CurrentNode->m_Next;
  	}
  	return FALSE;
  }
  
  void CRedZoneList::Serialize(CArchive & archive)
  {
  	UINT nVersion;
  	if (archive.IsStoring()) {
  		nVersion = VERSION_NUMBER;
  		archive << nVersion;
  		archive.WriteCount(NumberOfNodes());
  		CRedZoneListNode* CurrentNode = m_RedZoneListHead;
  		while (CurrentNode != NULL) {
  			archive << CurrentNode->m_RedZone;
  			CurrentNode = CurrentNode->m_Next;
  		}
  	} else {
  		archive >> nVersion;
  		unsigned int nCount = archive.ReadCount();
  		CRedZone* NewZone;
  		while (nCount) {
  			archive >> NewZone;
  			NewZone->SetProbabilityGrid(m_ProbabilityGrid);
  			Add(NewZone);
  			nCount--;
  		}
  	}
  }
  
  void CRedZoneList::SerializeV2(CArchive & archive)
  {
  	if (archive.IsStoring()) {
  		archive.WriteCount(NumberOfNodes());
  		CRedZoneListNode* CurrentNode = m_RedZoneListHead;
  		while (CurrentNode != NULL) {
  			archive << CurrentNode->m_RedZone;
  			CurrentNode = CurrentNode->m_Next;
  		}
  	} else {
  		unsigned int nCount = archive.ReadCount();
  		CRedZone* NewZone;
  		while (nCount) {
  			archive >> NewZone;
  			NewZone->SetProbabilityGrid(m_ProbabilityGrid);
  			Add(NewZone);
  			nCount--;
  		}
  	}
  }
  
  unsigned int CRedZoneList::NumberOfNodes()
  /*******************************************************************************
  //Created: 07/17/97 S.R.
  //Last Revision: 07/17/97 S.R.
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
  	unsigned int Count = 0;
  	CRedZoneListNode* CurrentNode = m_RedZoneListHead;
  	while (CurrentNode != NULL) {
  		Count++;
  		CurrentNode = CurrentNode->m_Next;
  	}
  	return Count;
  
  }
  
  void CRedZone::SetBounds(robRECT bounds)
  {
  	robPOINT CurrentPoint;
  	unsigned int i;
  /*Make sure rectangle is normalized*/
  	bounds.Normalize();
  
  /*	bounds.left = (long)bounds.left - (long)bounds.left % (long)cWorldGridPointSize;
  	bounds.right = ((long)bounds.right - ((long)bounds.right % (long)cWorldGridPointSize));
  	bounds.top = (long)bounds.top - (long)bounds.top % (long)cWorldGridPointSize;
  	bounds.bottom = (long)bounds.bottom - ((long)bounds.bottom % (long)cWorldGridPointSize);
  */
  	if (bounds.left >= 0) {
  		bounds.left = (long)bounds.left - (long)bounds.left % (long)cWorldGridPointSize + cWorldGridPointSize/2;
  	} else {
  		if ( ((long)bounds.left % (long)cWorldGridPointSize) != 0 ) {
  			bounds.left = (long)bounds.left - (long)bounds.left % (long)cWorldGridPointSize - (cWorldGridPointSize/2);
  		} else {
  			bounds.left = (long)bounds.left + (cWorldGridPointSize/2);
  		}
  	}
  	if (bounds.right >= 0) {
  		bounds.right = ((long)bounds.right - ((long)bounds.right % (long)cWorldGridPointSize)) + cWorldGridPointSize / 2;
  	} else {
  		if ( ((long)bounds.right % (long)cWorldGridPointSize) != 0 ) {
  			bounds.right = ((long)bounds.right - ((long)bounds.right % (long)cWorldGridPointSize)) - (cWorldGridPointSize / 2);
  		} else {
  			bounds.right = (long)bounds.right + (cWorldGridPointSize / 2);
  		}
  	}
  	if (bounds.top >= 0) {
  		bounds.top = (long)bounds.top - (long)bounds.top % (long)cWorldGridPointSize + cWorldGridPointSize/2;
  	} else {
  		if ( ((long)bounds.top % (long)cWorldGridPointSize) != 0 ) {
  			bounds.top = (long)bounds.top - (long)bounds.top % (long)cWorldGridPointSize - cWorldGridPointSize/2;
  		} else {
  			bounds.top = (long)bounds.top + cWorldGridPointSize/2;
  		}
  	}
  	if (bounds.bottom >= 0) {
  		bounds.bottom = (long)bounds.bottom - ((long)bounds.bottom % (long)cWorldGridPointSize) + cWorldGridPointSize/2;
  	} else {
  		if ( ((long)bounds.bottom % (long)cWorldGridPointSize) != 0 ) {
  			bounds.bottom = (long)bounds.bottom - ((long)bounds.bottom % (long)cWorldGridPointSize) - cWorldGridPointSize/2;
  		} else {
  			bounds.bottom = (long)bounds.bottom + cWorldGridPointSize/2;
  		}
  	}
  
  /*Make sure rectangle is normalized*/
  	bounds.Normalize();
  /*Make sure bounds obeys MinWidth and MinHeight*/
  	if (bounds.top - bounds.bottom < MinHeight()) {
  		bounds.bottom = bounds.top - MinHeight();
  	}
  	if (bounds.right - bounds.left < MinWidth()) {
  		bounds.right = bounds.left + MinWidth();
  	}
  
  	if (!m_FirstSetBoundsCall) {
  		ReplaceOldGridValues();
  	/*Resize the m_TopOldGridValues and m_BotttomOldGridValues if needed*/
  		i = unsigned int((bounds.right - bounds.left) / cWorldGridPointSize + 1);
  		if ( i > sizeof(m_TopOldGridValues) / sizeof(m_TopOldGridValues[0]) ) {
  			delete m_TopOldGridValues;
  			delete m_BottomOldGridValues;
  			m_TopOldGridValues = new int[i];
  			m_BottomOldGridValues = new int[i];
  		}
  	/*Resize the m_LeftOldGridValues and m_RightOldGridValues if needed*/
  		i = unsigned int((bounds.top - bounds.bottom) / cWorldGridPointSize + 1);
  		if ( i > sizeof(m_LeftOldGridValues) / sizeof(m_LeftOldGridValues[0]) ) {
  			delete m_LeftOldGridValues;
  			delete m_RightOldGridValues;
  			m_LeftOldGridValues = new int[i];
  			m_RightOldGridValues = new int[i];
  		}
  	} else {
  		m_FirstSetBoundsCall = false;
  		i = unsigned int((bounds.right - bounds.left) / cWorldGridPointSize + 1);
  		m_TopOldGridValues = new int[i];
  		m_BottomOldGridValues = new int[i];
  		i = unsigned int((bounds.top - bounds.bottom) / cWorldGridPointSize + 1);
  		m_LeftOldGridValues = new int[i];
  		m_RightOldGridValues = new int[i];
  	}
  	int OldGridValue;
  /*Update probability grid*/
  /*Do the top and bottom lines*/
  	for (i = 0, CurrentPoint.x = bounds.left; CurrentPoint.x <= bounds.right; i++, CurrentPoint.x += cWorldGridPointSize) {
  	/*Top*/
  		CurrentPoint.y = bounds.top;
  		OldGridValue = m_ProbabilityGrid->GetGridValue(CurrentPoint);
  		if ( (OldGridValue < cMinProb) || (OldGridValue > cMaxProb) ) {
  			m_TopOldGridValues[i] = cWorldGridInitialValue;
  		} else {
  			m_TopOldGridValues[i] = OldGridValue;
  		}
  		m_ProbabilityGrid->SetGridValue(CurrentPoint, cRedZoneProb);
  	/*Bottom*/
  		CurrentPoint.y = bounds.bottom;
  		OldGridValue = m_ProbabilityGrid->GetGridValue(CurrentPoint);
  		if ( (OldGridValue < cMinProb) || (OldGridValue > cMaxProb) ) {
  			m_BottomOldGridValues[i] = cWorldGridInitialValue;
  		} else {
  			m_BottomOldGridValues[i] = OldGridValue;
  		}
  //		m_BottomOldGridValues[i] = m_ProbabilityGrid->GetGridValue(CurrentPoint);
  		m_ProbabilityGrid->SetGridValue(CurrentPoint, cRedZoneProb);
  	}
  /*Do the left and right lines*/
  	for (i = 0, CurrentPoint.y = bounds.bottom; CurrentPoint.y <= bounds.top; i++, CurrentPoint.y += cWorldGridPointSize) {
  	/*Left*/
  		CurrentPoint.x = bounds.left;
  		OldGridValue = m_ProbabilityGrid->GetGridValue(CurrentPoint);
  		if ( (OldGridValue < cMinProb) || (OldGridValue > cMaxProb) ) {
  			m_LeftOldGridValues[i] = cWorldGridInitialValue;
  		} else {
  			m_LeftOldGridValues[i] = OldGridValue;
  		}
  //		m_LeftOldGridValues[i] = m_ProbabilityGrid->GetGridValue(CurrentPoint);
  		m_ProbabilityGrid->SetGridValue(CurrentPoint, cRedZoneProb);
  	/*Right*/
  		CurrentPoint.x = bounds.right;
  		OldGridValue = m_ProbabilityGrid->GetGridValue(CurrentPoint);
  		if ( (OldGridValue < cMinProb) || (OldGridValue > cMaxProb) ) {
  			m_RightOldGridValues[i] = cWorldGridInitialValue;
  		} else {
  			m_RightOldGridValues[i] = OldGridValue;
  		}
  //		m_RightOldGridValues[i] = m_ProbabilityGrid->GetGridValue(CurrentPoint);
  		m_ProbabilityGrid->SetGridValue(CurrentPoint, cRedZoneProb);
  	}
  
  	m_Bounds = bounds;
  }
  
  void CRedZone::ReplaceOldGridValues(void)
  {
  /*When replacing old values, it must be done in the reverse order of what they were stored.  That's because
  some of the values (the corner values) stored in m_LeftOldGridValues and m_RightOldGridValues were updated
  before stored.*/
  /*Do the left and right lines*/
  	unsigned int i;
  	robPOINT CurrentPoint;
  	for (i = 0, CurrentPoint.y = m_Bounds.bottom; CurrentPoint.y <= m_Bounds.top; i++, CurrentPoint.y += cWorldGridPointSize) {
  		/*Left*/
  		CurrentPoint.x = m_Bounds.left;
  		m_ProbabilityGrid->SetGridValue(CurrentPoint, m_LeftOldGridValues[i], 1, true);
  		/*Right*/
  		CurrentPoint.x = m_Bounds.right;
  		m_ProbabilityGrid->SetGridValue(CurrentPoint, m_RightOldGridValues[i], 1, true);
  	}
  /*Do the top and bottom lines*/
  	for (i = 0, CurrentPoint.x = m_Bounds.left; CurrentPoint.x <= m_Bounds.right; i++, CurrentPoint.x += cWorldGridPointSize) {
  		/*Top*/
  		CurrentPoint.y = m_Bounds.top;
  		m_ProbabilityGrid->SetGridValue(CurrentPoint, m_TopOldGridValues[i], 1, true);
  		/*Bottom*/
  		CurrentPoint.y = m_Bounds.bottom;
  		m_ProbabilityGrid->SetGridValue(CurrentPoint, m_BottomOldGridValues[i], 1, true);
  	}
  }
  
  void CRedZone::SetProbabilityGrid(CProbabilityGrid * ProbGrid)
  {
  	m_ProbabilityGrid = ProbGrid;
  }
  
  void CRedZone::Serialize(CArchive & archive)
  {
  	unsigned int size;
  	unsigned int i;
  	CRWGraphicObject::Serialize(archive);
  	if (archive.IsStoring()) {
  		archive << ((BOOL)m_FirstSetBoundsCall);
  		archive << (size = HorizontalGridSize());
  		for (i = 0; i < size; i++) {
  			archive << m_TopOldGridValues[i];
  			archive << m_BottomOldGridValues[i];
  		}
  		archive << (size = VerticalGridSize());
  		for (i = 0; i < size; i++) {
  			archive << m_LeftOldGridValues[i];
  			archive << m_RightOldGridValues[i];
  		}
  	} else {
  		BOOL temp;
  		archive >> temp;
  		m_FirstSetBoundsCall = temp != 0;
  		archive >> size;
  		m_TopOldGridValues = new int[size];
  		m_BottomOldGridValues = new int[size];
  		for (i = 0; i < size; i++) {
  			archive >> m_TopOldGridValues[i];
  			archive >> m_BottomOldGridValues[i];
  		}
  		archive >> size;
  		m_LeftOldGridValues = new int[size];
  		m_RightOldGridValues = new int[size];
  		for (i = 0; i < size; i++) {
  			archive >> m_LeftOldGridValues[i];
  			archive >> m_RightOldGridValues[i];
  		}
  	}
  }
  
  void CRedZone::OnErase()
  {
  	ReplaceOldGridValues();
  	m_RedZoneList->Remove(this);
  }
  
  void CRedZone::SetRedZoneList(CRedZoneList * RedZoneList)
  {
  	m_RedZoneList = RedZoneList;
  }
  
  void CRedZone::OnEditClick(robPOINT point, CRobotWorldView *View)
  {
  	m_EditMode = emSelected;
  
  	CRectTracker Tracker = SetupTracker(View);
  	View->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	if (Tracker.Track(View, View->RobotToClientCoords(point))) {
  		SetBounds(View->ClientToRobotCoords(Tracker.m_rect));
  	/*Reset all the other red zones that may have been drawn over*/
  		if (m_RedZoneList != NULL) {
  			CRedZoneListNode* CurrentNode = m_RedZoneList->m_RedZoneListHead;
  			while (CurrentNode != NULL) {
  				if (CurrentNode->m_RedZone != this) {
  					CurrentNode->m_RedZone->SetBounds(CurrentNode->m_RedZone->GetBounds());
  				}
  				CurrentNode = CurrentNode->m_Next;
  			}
  		}
  		View->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	}
  }
