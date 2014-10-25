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
// LineRectTracker.h: interface for the CLineRectTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINERECTTRACKER_H__29EF2AC3_2535_11D2_B231_0040F6B87672__INCLUDED_)
#define AFX_LINERECTTRACKER_H__29EF2AC3_2535_11D2_B231_0040F6B87672__INCLUDED_

#include "RothMath.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

const int htNoHandle = -2;
const int htLineSegment = -1;
class CPolyLineTracker
{
    public:
        CPolyLineTracker(CPolyLineTracker& Tracker);
        BOOL SetCursor(CWnd* pWnd, UINT nHitTest);
        int m_nHandleSize;
        int FindSelectedPoint(POINT point);
        void Draw(CDC* pDC);
        CPolyLineTracker();
        virtual ~CPolyLineTracker();
        CList<POINT, POINT> m_PointList;
        BOOL Track(CWnd* pWnd, CPoint point);
    protected:
        void DrawTracker(CDC* pDC, CWnd* pWnd);
        void MovePoints(int dx, int dy);
        int HitTestHandles(CPoint point);

        BOOL TrackHandle(int nHandle, CWnd* pWnd, CPoint point);

};

#endif // !defined(AFX_LINERECTTRACKER_H__29EF2AC3_2535_11D2_B231_0040F6B87672__INCLUDED_)
