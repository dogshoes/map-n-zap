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
  // LineRectTracker.cpp: implementation of the CLineRectTracker class.
  //
  //////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "LineRectTracker.h"
  #include <assert.h>
  
  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////
  
  CPolyLineTracker::CPolyLineTracker()
  {
  
  }
  
  CPolyLineTracker::~CPolyLineTracker()
  {
  
  }
  
  void CPolyLineTracker::DrawTracker(CDC* pDC, CWnd* pWnd )
  {
  	CPen Pen(PS_SOLID, 1, COLORREF(0xFF));
  	CPen* OldPen = pDC->SelectObject(&Pen);
  	int OldROP2 = pDC->SetROP2(R2_XORPEN);
  	POSITION Position = m_PointList.GetHeadPosition();
  	POINT ClientPoint;
  	RECT rect;
  	if (Position != NULL) {
  		ClientPoint = m_PointList.GetNext(Position);
  		pDC->MoveTo(ClientPoint);
  		rect.left = ClientPoint.x - m_nHandleSize/2;
  		rect.right = rect.left + m_nHandleSize;
  		rect.bottom = ClientPoint.y + m_nHandleSize/2;
  		rect.top = rect.bottom - m_nHandleSize;
  //		pDC->FillSolidRect(&rect, RGB(0xFF, 0, 0));
  	}
  	while (Position != NULL) {
  		ClientPoint = m_PointList.GetNext(Position);
  		pDC->LineTo(ClientPoint);
  		rect.left = ClientPoint.x - m_nHandleSize/2;
  		rect.right = rect.left + m_nHandleSize;
  		rect.bottom = ClientPoint.y + m_nHandleSize/2;
  		rect.top = rect.bottom - m_nHandleSize;
  //		pDC->FillSolidRect(&rect, RGB(0xFF, 0, 0));
  	}
  	pDC->SelectObject(OldPen);
  	pDC->SetROP2(OldROP2);
  }
  
  void CPolyLineTracker::Draw(CDC * pDC)
  {
  	CPen Pen(PS_SOLID, 1, COLORREF(0xFF));
  	CPen* OldPen = pDC->SelectObject(&Pen);
  	int OldROP2 = pDC->SetROP2(R2_COPYPEN);
  	POSITION Position = m_PointList.GetHeadPosition();
  	POINT ClientPoint;
  	RECT rect;
  	if (Position != NULL) {
  		ClientPoint = m_PointList.GetNext(Position);
  		pDC->MoveTo(ClientPoint);
  		rect.left = ClientPoint.x - m_nHandleSize/2;
  		rect.right = rect.left + m_nHandleSize;
  		rect.bottom = ClientPoint.y + m_nHandleSize/2;
  		rect.top = rect.bottom - m_nHandleSize;
  		pDC->FillSolidRect(&rect, RGB(0xFF, 0, 0));
  	}
  	while (Position != NULL) {
  		ClientPoint = m_PointList.GetNext(Position);
  		pDC->LineTo(ClientPoint);
  		rect.left = ClientPoint.x - m_nHandleSize/2;
  		rect.right = rect.left + m_nHandleSize;
  		rect.bottom = ClientPoint.y + m_nHandleSize/2;
  		rect.top = rect.bottom - m_nHandleSize;
  		pDC->FillSolidRect(&rect, RGB(0xFF, 0, 0));
  	}
  	pDC->SelectObject(OldPen);
  	pDC->SetROP2(OldROP2);
  }
  
  BOOL CPolyLineTracker::Track(CWnd* pWnd, CPoint point)
  {
  	// perform hit testing on the handles
  	int nHandle = HitTestHandles(point);
  	if (nHandle == htNoHandle)
  	{
  		// didn't hit a handle, so just return FALSE
  		return FALSE;
  	}
  
  	// otherwise, call helper function to do the tracking
  	return TrackHandle(nHandle, pWnd, point);
  }
  
  BOOL CPolyLineTracker::TrackHandle(int nHandle, CWnd* pWnd, CPoint point)
  {
  	POSITION MovePointPosition = NULL;
  	// don't handle if capture already set
  	if (::GetCapture() != NULL)
  		return FALSE;
  
  	// set capture to the window which received this message
  	pWnd->SetCapture();
  	assert(pWnd == CWnd::GetCapture());
  	pWnd->UpdateWindow();
  	CPoint pointSave, OldPoint;
  	if (nHandle >= 0) {
  		MovePointPosition = m_PointList.FindIndex(nHandle);
  		pointSave = m_PointList.GetAt(MovePointPosition);
  		OldPoint = pointSave;
  	} else if (nHandle == htLineSegment) {
  		OldPoint = point;
  	}
  
  
  	// get DC for drawing
  	CDC* pDrawDC;
  	pDrawDC = pWnd->GetDC();
  	ASSERT_VALID(pDrawDC);
  
  	BOOL bMoved = FALSE;
  
  	AfxLockTempMaps();  // protect maps while looping
  
  	int px, py;
  	// get messages until capture lost or cancelled/accepted
  	for (;;)
  	{
  		MSG msg;
  		VERIFY(::GetMessage(&msg, NULL, 0, 0));
  
  		if (CWnd::GetCapture() != pWnd)
  			break;
  
  		switch (msg.message)
  		{
  		// handle movement/accept messages
  		case WM_LBUTTONUP:
  		case WM_MOUSEMOVE:
  			OldPoint.x = point.x;
  			OldPoint.y = point.y;
  			px = (int)(short)LOWORD(msg.lParam);
  			py = (int)(short)HIWORD(msg.lParam);
  			point.x = px;
  			point.y = py;
  		/*Erase old stuff*/
  			DrawTracker(pDrawDC, pWnd);
  			if (nHandle >= 0) {
  				m_PointList.SetAt(MovePointPosition, point);
  			} else if (nHandle == htLineSegment) {
  				MovePoints(point.x - OldPoint.x, point.y - OldPoint.y);
  			}
  		/*Draw new stuff*/
  			// only redraw and callback if the rect actually changed!
  			DrawTracker(pDrawDC, pWnd);
  			if (msg.message != WM_LBUTTONUP) bMoved = TRUE;
  			if (msg.message == WM_LBUTTONUP) goto ExitLoop;
  			break;
  
  		// handle cancel messages
  		case WM_KEYDOWN:
  			if (msg.wParam != VK_ESCAPE) break;
  		case WM_RBUTTONDOWN:
  			if (bMoved)
  			{
  				DrawTracker(pDrawDC, pWnd);
  			}
  			if (nHandle >= 0) {
  				m_PointList.SetAt(MovePointPosition, pointSave);
  			} else if (nHandle == htLineSegment) {
  				point.x = pointSave.x;
  				point.y = pointSave.y;
  			}
  			goto ExitLoop;
  
  		// just dispatch rest of the messages
  		default:
  			DispatchMessage(&msg);
  			break;
  		}
  	}
  
  ExitLoop:
  	pWnd->ReleaseDC(pDrawDC);
  	ReleaseCapture();
  
  	AfxUnlockTempMaps(FALSE);
  
  	// return false if nothing is moved.
  	if (!bMoved) return FALSE;
  
  	// return TRUE only if rect has changed
  	if (nHandle >= 0) {
  		POINT temp = m_PointList.GetAt(MovePointPosition);
  		if ( (temp.x != pointSave.x) || (temp.y != pointSave.y) ) return TRUE;
  	} else if (nHandle == htLineSegment) {
  		if ( (point.x != pointSave.x) || (point.y != pointSave.y) ) return TRUE;
  	}
  	return FALSE;
  }
  
  
  int CPolyLineTracker::HitTestHandles(CPoint point)
  {
  	POSITION Position = m_PointList.GetHeadPosition();
  	if (Position == NULL) return htNoHandle;
  	int SelectedPoint = htNoHandle;
  	int i = 0;
  	POINT ClientPoint;
  	robPOINT p1;
  	ClientPoint = m_PointList.GetNext(Position);
  	p1.x = ClientPoint.x;
  	p1.y = ClientPoint.y;
  	if ( (abs(point.x - ClientPoint.x) <= m_nHandleSize/2.0) && (abs(point.y - ClientPoint.y) <= m_nHandleSize/2.0) ) {
  		return 0;
  	}
  	i = 1;
  	while (Position != NULL) {
  		ClientPoint = m_PointList.GetNext(Position);
  		robPOINT p2;
  		p2.x = ClientPoint.x;
  		p2.y = ClientPoint.y;
  		if ( (abs(point.x - ClientPoint.x) <= m_nHandleSize/2.0) && (abs(point.y - ClientPoint.y) <= m_nHandleSize/2.0) ) {
  			return i;
  		} else {
  			TLine LineSegment;
  			FindLineFrom2Points(p1, p2, &LineSegment);
  			robPOINT p3;
  			p3.x = point.x;
  			p3.y = point.y;
  			robPOINT Intersection = FindPerpendicularIntersection(LineSegment, p3);
  			if ( PointIsBetween(Intersection, p1, p2) ) {
  				if (PerpendicularDistanceFromLine(LineSegment, point) <= m_nHandleSize/2.0) return htLineSegment;
  			}
  		}
  		p1.x = p2.x;
  		p1.y = p2.y;
  		i++;
  	}
  	return htNoHandle;
  }
  
  int CPolyLineTracker::FindSelectedPoint(POINT point)
  {
  	return HitTestHandles(point);
  }
  
  void CPolyLineTracker::MovePoints(int dx, int dy)
  {
  	POSITION Position = m_PointList.GetHeadPosition();
  	while (Position != NULL) {
  		POINT point;
  		point = m_PointList.GetAt(Position);
  		point.x += dx;
  		point.y += dy;
  		m_PointList.SetAt(Position, point);
  		m_PointList.GetNext(Position);
  	}
  }
  
  BOOL CPolyLineTracker::SetCursor(CWnd* pWnd, UINT nHitTest)
  {
  	CPoint point;
  	GetCursorPos(&point);
  	pWnd->ScreenToClient(&point);
  	if (FindSelectedPoint(point) != htNoHandle) {
  		::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
  		return TRUE;
  	}
  	return FALSE;
  }
  
  CPolyLineTracker::CPolyLineTracker(CPolyLineTracker& Tracker)
  {
  	m_nHandleSize = Tracker.m_nHandleSize;
  	POSITION Position = Tracker.m_PointList.GetHeadPosition();
  	while (Position != NULL) {
  		m_PointList.AddTail(Tracker.m_PointList.GetNext(Position));
  	}
  }
