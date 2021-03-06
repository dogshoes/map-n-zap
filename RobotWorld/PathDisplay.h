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
// PathDisplay.h: interface for the CPathDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHDISPLAY_H__3D2C1941_68E7_11D2_B231_0040F6B87672__INCLUDED_)
#define AFX_PATHDISPLAY_H__3D2C1941_68E7_11D2_B231_0040F6B87672__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RWGraphicObject.h"

class CCTDPath;
class CProbabilityGrid;

typedef CList<robPOINT , robPOINT> TrobPOINTList;
void DrawOuterLines(CPoint SurfaceOffset, CDC* pDC, CProbabilityGrid* ProbGrid, TrobPOINTList* PointList);

class CPathDisplay : public CRWGraphicObject
{
    public:
        void SetShowCaption(bool)
        {
            return;
        };
        void Redraw(bool UpdateNow = true);
        void SetPath(CCTDPath* path);
        TRWGraphicObjectType GetObjectType(void);
        void Draw(CRect ProbabilityGridUpdateRect, CDC* pDC, CProbabilityGrid* ProbGrid);
        void SetBounds(robRECT bounds);
        void OnDefaultClick(robPOINT point, float WorldPointsPerPixel, CRobot* Robot);
        unsigned int GetZLayer(void);
        bool GetShowCaption(void);
        void OnEditClick(robPOINT point, CRobotWorldView* View);
        CPathDisplay();
        virtual ~CPathDisplay();
    private:
    protected:
        TrobPOINTList m_DisplayPath;
        void DrawCTDPath(CDC* pdc, TrobPOINTList * path, bool ShowRobotBounds, CProbabilityGrid *ProbGrid, CRect ProbabilityGridUpdateRect);
};

#endif // !defined(AFX_PATHDISPLAY_H__3D2C1941_68E7_11D2_B231_0040F6B87672__INCLUDED_)
