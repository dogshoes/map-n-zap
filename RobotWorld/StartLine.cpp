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
// StartLine.cpp: implementation of the CStartLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "global.h"
#include "robotworld.h"
#include "StartLine.h"
#include "Probability Grid.h"

IMPLEMENT_SERIAL(CStartLine, CRWGraphicObject, VERSIONABLE_SCHEMA | VERSION_NUMBER)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
LPDIRECTDRAWSURFACE3 CStartLine::m_lpddsAnimationBmps1[1] = {NULL};
LPDIRECTDRAWSURFACE3 CStartLine::m_lpddsAnimationBmps2[1] = {NULL};
LPDIRECTDRAWSURFACE3 CStartLine::m_lpddsAnimationBmps3[1] = {NULL};

const unsigned int cZoom3BmpHeight = 72;
const unsigned int cZoom2BmpHeight = 36;
const unsigned int cZoom1BmpHeight = 18;
const unsigned int cZoom3BmpWidth = 72;
const unsigned int cZoom2BmpWidth = 36;
const unsigned int cZoom1BmpWidth = 18;

CStartLine::CStartLine()
{
    m_Bounds.left = -MinWidth() / 2;
    m_Bounds.top = MinHeight() / 2;
    m_Bounds.right = MinWidth() / 2;
    m_Bounds.bottom = -MinHeight() / 2;
    SetShowCaption(false);
}

CStartLine::~CStartLine()
{

}

void CStartLine::Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbabilityGrid)
{
#ifdef IMPLEMENTED

    if (m_IsVisible)
    {
        CBrush BorderBrush(COLORREF(0x00FF00));
        /*Draw border rectangle*/
        CRect OuterRect;
        robPOINT RobotPoint;
        POINT point;
        CPoint SurfaceOffset = ProbGridBmpUpdateRect.TopLeft();
        RobotPoint.x = m_Bounds.left, RobotPoint.y = m_Bounds.top;
        point = ProbabilityGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
        OuterRect.left = point.x;
        OuterRect.top = point.y;
        RobotPoint.x = m_Bounds.right, RobotPoint.y = m_Bounds.bottom;
        point = ProbabilityGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
        OuterRect.right = point.x;
        OuterRect.bottom = point.y;

        pDC->FillRect(&OuterRect, &BorderBrush);
        BorderBrush.DeleteObject();

        if (OuterRect.Width() >= OuterRect.Height())
        {
            CFont CaptionFont;
            int MinMargin;

            if (ProbabilityGrid->m_ZoomLevel == cZoomLevel3)
            {
                CaptionFont.CreatePointFont(110, "Arial", pDC);
                MinMargin = 4;
            }
            else if (ProbabilityGrid->m_ZoomLevel == cZoomLevel2)
            {
                CaptionFont.CreatePointFont(70, "Arial", pDC);
                MinMargin = 2;
            }
            else if (ProbabilityGrid->m_ZoomLevel == cZoomLevel1)
            {
                CaptionFont.CreatePointFont(40, "Arial", pDC);
                MinMargin = 1;
            }

            CFont* OldFont = pDC->SelectObject(&CaptionFont);
            int OldBkMode = pDC->SetBkMode(TRANSPARENT);
            COLORREF OldTextColor = pDC->SetTextColor(COLORREF(0x0));
            CString Caption("Start");
            int MinWidth = OuterRect.Width() - 2 * MinMargin;
            CSize CaptionSize = pDC->GetTextExtent(Caption);

            if (CaptionSize.cx > MinWidth)
            {
                Caption = "Star";
                CaptionSize = pDC->GetTextExtent(Caption);

                if (CaptionSize.cx > MinWidth)
                {
                    Caption = "Sta";
                    CaptionSize = pDC->GetTextExtent(Caption);

                    if (CaptionSize.cx > MinWidth)
                    {
                        Caption = "St";
                        CaptionSize = pDC->GetTextExtent(Caption);

                        if (CaptionSize.cx > MinWidth)
                        {
                            Caption = "S";
                            CaptionSize = pDC->GetTextExtent(Caption);
                        }
                    }
                }
            }

            int x = OuterRect.left + (OuterRect.Width() - CaptionSize.cx) / 2;
            int y = OuterRect.top + (OuterRect.Height() - CaptionSize.cy) / 2;
            pDC->TextOut(x, y, Caption);
            pDC->SetBkMode(OldBkMode);
            pDC->SetTextColor(OldTextColor);
            pDC->SelectObject(OldFont);
        }
        else
        {

        }
    }

#endif
    CRWGraphicObject::Draw(ProbGridBmpUpdateRect, pDC, ProbabilityGrid);
}

void CStartLine::OnDefaultClick(robPOINT point, float WorldPointsPerPixel, CRobot* Robot)
{
}

void CStartLine::Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid *ProbGrid)
{
    if (!GetVisible())
    {
        return;
    }

    LoadBitmaps(ProbGrid);

    DrawStart(ProbabilityGridUpdateRect, BackBuffer, ProbGrid);

}

void CStartLine::LoadBitmaps(CProbabilityGrid *ProbGrid)
{
    ASSERT(ProbGrid != NULL);

    if (m_lpddsAnimationBmps1[0] != NULL)
    {
        return;
    }

    /*Load the animation bitmaps*/
    m_lpddsAnimationBmps1[0] = ProbGrid->DDrawLoadBitmap(IDB_START_LINE1);

    if (m_lpddsAnimationBmps1[0] == NULL)
    {
        fprintf(stderr, "CStartLine::LoadBitmaps error!! Did not load m_lpddsAnimationBmps1[0]\n");
    }

    m_lpddsAnimationBmps2[0] = ProbGrid->DDrawLoadBitmap(IDB_START_LINE2);

    if (m_lpddsAnimationBmps2[0] == NULL)
    {
        fprintf(stderr, "CStartLine::LoadBitmaps error!! Did not load m_lpddsAnimationBmps2[0]\n");
    }

    m_lpddsAnimationBmps3[0] = ProbGrid->DDrawLoadBitmap(IDB_START_LINE3);

    if (m_lpddsAnimationBmps3[0] == NULL)
    {
        fprintf(stderr, "CStartLine::LoadBitmaps error!! Did not load m_lpddsAnimationBmps3[0]\n");
    }
}

LPDIRECTDRAWSURFACE3 CStartLine::GetStartBitmap(float ZoomLevel)
{
    LPDIRECTDRAWSURFACE3 Bmp;

    if (ZoomLevel == cZoomLevel1)
    {
        Bmp = m_lpddsAnimationBmps1[0];
    }
    else if (ZoomLevel == cZoomLevel2)
    {
        Bmp = m_lpddsAnimationBmps2[0];
    }
    else
    {
        Bmp = m_lpddsAnimationBmps3[0];
    }

    return Bmp;
}

void CStartLine::DrawStart(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid *ProbGrid)
{
    LPDIRECTDRAWSURFACE3	Bmp = NULL;

    Bmp = GetStartBitmap(ProbGrid->GetZoomLevel());

    if (Bmp->IsLost() == DDERR_SURFACELOST)
    {
        Bmp->Restore();
    }

    CRect rcRect, destRect;
    /*Get the bitmap dimensions*/
    int BmpFrameWidth, BmpFrameHeight;

    if (ProbGrid->GetZoomLevel() == cZoomLevel3)
    {
        BmpFrameHeight = BmpFrameWidth = cZoom3BmpHeight;
    }
    else if (ProbGrid->GetZoomLevel() == cZoomLevel2)
    {
        BmpFrameHeight = BmpFrameWidth = cZoom2BmpHeight;
    }
    else
    {
        BmpFrameHeight = BmpFrameWidth = cZoom1BmpHeight;
    }

    rcRect.left = 0;
    rcRect.top = 0;
    rcRect.right = rcRect.left + BmpFrameWidth;
    rcRect.bottom = rcRect.top + BmpFrameHeight;

    /*Get the finish line destination*/
    robPOINT bmpLocation;
    bmpLocation.x = (m_Bounds.left + m_Bounds.right) / 2;
    bmpLocation.y = (m_Bounds.top + m_Bounds.bottom) / 2;

    float temp;
    temp = ProbGrid->RobotToDDSurfaceX(bmpLocation.x, ProbabilityGridUpdateRect.left);
    temp -= BmpFrameWidth / 2.0f;
    destRect.left = Round(temp);
    destRect.right = destRect.left + BmpFrameWidth;
    temp = ProbGrid->RobotToDDSurfaceY(bmpLocation.y, ProbabilityGridUpdateRect.top);
    temp -= BmpFrameHeight / 2.0f;
    destRect.top = Round(temp);
    destRect.bottom = destRect.top + BmpFrameHeight;

    ProbGrid->Blt(BackBuffer, destRect, Bmp, rcRect);
}

void CStartLine::SetBounds(robRECT Bounds)
{
    Bounds.Normalize();
    Bounds.left = (Bounds.left + Bounds.right - cZoom3BmpWidth * cWorldGridPointSize / cZoomLevel3) / 2;
    Bounds.right = Bounds.left + cZoom3BmpWidth * cWorldGridPointSize / cZoomLevel3;
    Bounds.top = (Bounds.top + Bounds.bottom + cZoom3BmpHeight * cWorldGridPointSize / cZoomLevel3) / 2;
    Bounds.bottom = Bounds.top - cZoom3BmpHeight * cWorldGridPointSize / cZoomLevel3;
    m_Bounds = Bounds;
}
