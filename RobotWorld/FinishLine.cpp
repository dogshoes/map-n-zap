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
// FinishLine.cpp: implementation of the CFinishLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "robotworld.h"
#include "FinishLine.h"
#include "Robot.h"
#include "WorldGridConsts.h"
#include "global.h"
#include "PathList.h"
#include "DlgLabelName.h"
#include "..\vendor\ddraw.h"
#include "..\vendor\ddutil.h"
#include "Probability Grid.h"
#include "RobotWorldView.h"

IMPLEMENT_SERIAL(CFinishLine, CExecutableRWGraphicObject, VERSIONABLE_SCHEMA | VERSION_NUMBER)

const int cZoom3BmpHeight = 72;
const int cZoom3BmpWidth = 72;
const int cZoom2BmpHeight = 36;
const int cZoom2BmpWidth = 36;
const int cZoom1BmpHeight = 18;
const int cZoom1BmpWidth = 18;

LPDIRECTDRAWSURFACE3 CFinishLine::m_lpddsAnimationBmps1[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
LPDIRECTDRAWSURFACE3 CFinishLine::m_lpddsAnimationBmps2[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
LPDIRECTDRAWSURFACE3 CFinishLine::m_lpddsAnimationBmps3[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
LPDIRECTDRAWSURFACE3 CFinishLine::m_lpddsTimerBmps[4] = {NULL, NULL, NULL, NULL};
DWORD CFinishLine::m_TransparencyColor = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFinishLine::CFinishLine() : CExecutableRWGraphicObject()
{
    m_Bounds.left = -MinWidth() / 2;
    m_Bounds.top = MinHeight() / 2;
    m_Bounds.right = MinWidth() / 2;
    m_Bounds.bottom = -MinHeight() / 2;
    m_Running = false;
    SetTimerPosition(0);
    SetCaption(0);
}

CFinishLine::~CFinishLine()
{

}

void CFinishLine::Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbGrid)
{
    if (!GetVisible())
    {
        return;
    }

    /*Draw the timer text*/
    int cTextGap = 3;

    if (GetVisible())
    {
        CFont CaptionFont;
        CaptionFont.CreatePointFont(110, "Arial", pDC);

        CFont* OldFont = pDC->SelectObject(&CaptionFont);
        CPoint SurfaceOffset = ProbGridBmpUpdateRect.TopLeft();
        int OldBkMode = pDC->SetBkMode(TRANSPARENT);
        COLORREF OldTextColor = pDC->SetTextColor(COLORREF(0xFF));
        CSize CaptionSize = pDC->GetTextExtent(GetCaption());
        CaptionSize.cy += cTextGap;
        robPOINT CaptionDimensions;
        CaptionDimensions.x = ProbGrid->ClientSizeToRobotSize(CaptionSize.cx);
        CaptionDimensions.y = ProbGrid->ClientSizeToRobotSize(CaptionSize.cy);

        /*Calculate the caption bounds*/
        switch (GetTimerPosition())
        {
        case 0:
            m_CaptionBounds.top = m_Bounds.top + ProbGrid->ClientSizeToRobotSize(GetTimerBoxHeight(ProbGrid->GetZoomLevel()) - 1);
            m_CaptionBounds.left = (m_Bounds.left + m_Bounds.right) / 2 - ProbGrid->ClientSizeToRobotSize(GetTimerBoxWidth(ProbGrid->GetZoomLevel()) / 2 - 7);
            break;

        case 1:
            m_CaptionBounds.top = (m_Bounds.top + m_Bounds.bottom) / 2 + ProbGrid->ClientSizeToRobotSize(GetTimerBoxHeight(ProbGrid->GetZoomLevel()) / 2 - 1);
            m_CaptionBounds.left = m_Bounds.right + ProbGrid->ClientSizeToRobotSize(12);
            break;

        case 2:
            m_CaptionBounds.top = m_Bounds.bottom - ProbGrid->ClientSizeToRobotSize(5);
            m_CaptionBounds.left = (m_Bounds.left + m_Bounds.right) / 2 - ProbGrid->ClientSizeToRobotSize(GetTimerBoxWidth(ProbGrid->GetZoomLevel()) / 2 - 7);
            break;

        case 3:
            m_CaptionBounds.top = (m_Bounds.top + m_Bounds.bottom) / 2 + ProbGrid->ClientSizeToRobotSize(GetTimerBoxHeight(ProbGrid->GetZoomLevel()) / 2);
            m_CaptionBounds.left = m_Bounds.left - ProbGrid->ClientSizeToRobotSize(GetTimerBoxWidth(ProbGrid->GetZoomLevel()) - 7);
            break;
        }

        m_CaptionBounds.bottom = m_CaptionBounds.top - CaptionDimensions.y;
        m_CaptionBounds.right = m_CaptionBounds.left + CaptionDimensions.x;

        int x = (int)ProbGrid->RobotToDDSurfaceX(m_CaptionBounds.left, SurfaceOffset.x);
        int y = (int)ProbGrid->RobotToDDSurfaceY(m_CaptionBounds.top, SurfaceOffset.y);
        pDC->TextOut(x, y, GetCaption());
        pDC->SetBkMode(OldBkMode);
        pDC->SetTextColor(OldTextColor);
        pDC->SelectObject(OldFont);

        if (m_EditMode != emNone)
        {
            CRectTracker Tracker = SetupTracker(ProbGrid, ProbGridBmpUpdateRect.TopLeft());
            Tracker.Draw(pDC);
        }
    }

#ifdef IMPLEMENTED
    CExecutableRWGraphicObject::Draw(ProbGridBmpUpdateRect, pDC, ProbGrid);

    if (m_IsVisible)
    {
        /*Check if bounds are inside the update rectangle*/
        CRect BmpBounds;
        BmpBounds.left = ProbGrid->RobotToBitmapX(m_Bounds.left);
        BmpBounds.right = ProbGrid->RobotToBitmapX(m_Bounds.right);
        BmpBounds.top = ProbGrid->RobotToBitmapY(m_Bounds.top);
        BmpBounds.bottom = ProbGrid->RobotToBitmapY(m_Bounds.bottom);
        BmpBounds.InflateRect(ProbGrid->m_ZoomLevel, ProbGrid->m_ZoomLevel);
        BmpBounds.NormalizeRect();

        if (BmpBounds.IntersectRect(&BmpBounds, &ProbGridBmpUpdateRect))
        {
            CBrush BorderBrush(COLORREF(0x0));
            /*Draw border rectangle*/
            CRect OuterRect;
            robPOINT RobotPoint;
            POINT point;
            CPoint SurfaceOffset = ProbGridBmpUpdateRect.TopLeft();
            RobotPoint.x = m_Bounds.left, RobotPoint.y = m_Bounds.top;
            point = ProbGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
            OuterRect.left = point.x;
            OuterRect.top = point.y;
            RobotPoint.x = m_Bounds.right, RobotPoint.y = m_Bounds.bottom;
            point = ProbGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
            OuterRect.right = point.x;
            OuterRect.bottom = point.y;

            pDC->FrameRect(&OuterRect, &BorderBrush);
            point = OuterRect.TopLeft();
            POINT point2;
            CRect Rect;
            const short cRectDim = 40;
            short RectDim = cRectDim / cWorldGridPointSize * ProbGrid->m_ZoomLevel;
            bool EvenColumn = true;

            while (point.x < OuterRect.right)
            {
                if (EvenColumn)
                {
                    point.y = OuterRect.top;
                }
                else
                {
                    point.y = OuterRect.top + RectDim;
                }

                EvenColumn = !EvenColumn;
                point2.x = point.x + RectDim;

                if (point2.x > OuterRect.right)
                {
                    point2.x = OuterRect.right;
                }

                while (point.y < OuterRect.bottom)
                {
                    point2.y = point.y + RectDim;

                    if (point2.y > OuterRect.bottom)
                    {
                        point2.y = OuterRect.bottom;
                    }

                    Rect.TopLeft() = point;
                    Rect.BottomRight() = point2;
                    pDC->FillSolidRect(Rect, 0);
                    point.y += 2 * RectDim;
                }

                point.x += RectDim;
            }

            BorderBrush.DeleteObject();
        }
    }

#endif
}

void CFinishLine::OnDefaultClick(robPOINT point, float WorldPointsPerPixel, CRobot* Robot)
{
    //	SetStartTime();
    SetFinishTime();
    return;
}

void CFinishLine::SetCaption(CString Caption)
{
    m_Caption = Caption;
}

void CFinishLine::SetCaption(unsigned char Minutes, float Seconds)
{
    char buff[] = "0000";
    CString Caption("000.0");

    Seconds += Minutes * 60;

    if (Seconds > 999.9)
    {
        Seconds = 999.9f;
    }

    int temp = (int)Seconds;
    itoa(temp, buff, 10);
    Caption = buff;
    Caption += ".";

    Seconds = Seconds - temp;
    temp = (int)Seconds * 10;
    itoa(temp, buff, 10);
    Caption += buff;
    SetCaption(Caption);
}

void CFinishLine::SetCaption(float Seconds)
{
    SetCaption(0, Seconds);
}

void CFinishLine::SetStartTime()
{
    SYSTEMTIME TimeStruct;
    GetSystemTime(&TimeStruct);
    m_StartTime = CTime(TimeStruct);
    m_StartMilliseconds = TimeStruct.wMilliseconds;
    SetRunning(true);
}

void CFinishLine::SetFinishTime()
{
    if (GetRunning())
    {
        UpdateTime();
    }

    SetRunning(false);
}

void CFinishLine::SetRunning(bool Running)
{
    m_Running = Running;
    SetAnimate(Running);
}

bool CFinishLine::GetRunning()
{
    return m_Running;
}

void CFinishLine::OnErase()
{
    CExecutableRWGraphicObject::OnErase();
}

void CFinishLine::UpdateTime()
{
    SYSTEMTIME TimeStruct;
    GetSystemTime(&TimeStruct);
    CTime TNow = CTime(TimeStruct);
    short FinishMilliseconds = TimeStruct.wMilliseconds;

    CTimeSpan TimeDiff = TNow - m_StartTime;

    if (FinishMilliseconds < m_StartMilliseconds)
    {
        TimeDiff -= CTimeSpan(0, 0, 0, 1);
        FinishMilliseconds = FinishMilliseconds - m_StartMilliseconds + 1000;
    }
    else
    {
        FinishMilliseconds = FinishMilliseconds - m_StartMilliseconds;
    }

    SetCaption(TimeDiff.GetMinutes(), TimeDiff.GetSeconds() + FinishMilliseconds / 1000.0f);
    //	Redraw();
}

void CFinishLine::SetBounds(robRECT Bounds)
{
    Bounds.Normalize();
    Bounds.left = (Bounds.left + Bounds.right - cZoom3BmpWidth * cWorldGridPointSize / cZoomLevel3) / 2;
    Bounds.right = Bounds.left + cZoom3BmpWidth * cWorldGridPointSize / cZoomLevel3;
    Bounds.top = (Bounds.top + Bounds.bottom + cZoom3BmpHeight * cWorldGridPointSize / cZoomLevel3) / 2;
    Bounds.bottom = Bounds.top - cZoom3BmpHeight * cWorldGridPointSize / cZoomLevel3;
    m_Bounds = Bounds;
}

void CFinishLine::Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid *ProbGrid)
{
    if (!GetVisible())
    {
        return;
    }

    LoadBitmaps(ProbGrid);

    DrawFinish(ProbabilityGridUpdateRect, BackBuffer, ProbGrid);

    DrawTimerBox(ProbabilityGridUpdateRect, BackBuffer, ProbGrid);
}

void CFinishLine::LoadBitmaps(CProbabilityGrid *ProbGrid)
{
    ASSERT(ProbGrid != NULL);

    if (m_lpddsAnimationBmps1[0] != NULL)
    {
        return;
    }

    /*Load the animation bitmaps*/
    m_lpddsAnimationBmps1[0] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE10);
    m_lpddsAnimationBmps1[1] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE11);
    m_lpddsAnimationBmps1[2] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE12);
    m_lpddsAnimationBmps1[3] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE13);
    m_lpddsAnimationBmps1[4] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE14);
    m_lpddsAnimationBmps1[5] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE15);
    m_lpddsAnimationBmps1[6] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE16);
    m_lpddsAnimationBmps1[7] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE17);

    for (unsigned int i = 0; i < 8; i++)
    {
        if (m_lpddsAnimationBmps1[i] == NULL)
        {
            fprintf(stderr, "CFinishLine::LoadBitmaps error!! Did not load m_lpddsAnimationBmps1[%d]\n", i);
        }
    }

    m_lpddsAnimationBmps2[0] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE20);
    m_lpddsAnimationBmps2[1] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE21);
    m_lpddsAnimationBmps2[2] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE22);
    m_lpddsAnimationBmps2[3] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE23);
    m_lpddsAnimationBmps2[4] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE24);
    m_lpddsAnimationBmps2[5] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE25);
    m_lpddsAnimationBmps2[6] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE26);
    m_lpddsAnimationBmps2[7] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE27);

    for (int i = 0; i < 8; i++)
    {
        if (m_lpddsAnimationBmps2[i] == NULL)
        {
            fprintf(stderr, "CFinishLine::LoadBitmaps error!! Did not load m_lpddsAnimationBmps2[%d]\n", i);
        }
    }

    m_lpddsAnimationBmps3[0] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE30);
    m_lpddsAnimationBmps3[1] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE31);
    m_lpddsAnimationBmps3[2] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE32);
    m_lpddsAnimationBmps3[3] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE33);
    m_lpddsAnimationBmps3[4] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE34);
    m_lpddsAnimationBmps3[5] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE35);
    m_lpddsAnimationBmps3[6] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE36);
    m_lpddsAnimationBmps3[7] = ProbGrid->DDrawLoadBitmap(IDB_FINISH_LINE37);

    for (int i = 0; i < 8; i++)
    {
        if (m_lpddsAnimationBmps3[i] == NULL)
        {
            fprintf(stderr, "CFinishLine::LoadBitmaps error!! Did not load m_lpddsAnimationBmps3[%d]\n", i);
        }
    }

    /*Get the transparency color*/
    m_TransparencyColor = ProbGrid->GetSurfaceTopCornerColor(m_lpddsAnimationBmps1[0]);

    /*Load the timer box bitmaps*/
    m_lpddsTimerBmps[0] = ProbGrid->DDrawLoadBitmap(IDB_TIMER_TOP);

    if (m_lpddsTimerBmps[0] == NULL)
    {
        fprintf(stderr, "CFinishLine::LoadBitmaps error!!  Did not load m_lpddsTimerBmps[0]\n");
    }

    m_lpddsTimerBmps[1] = ProbGrid->DDrawLoadBitmap(IDB_TIMER_RIGHT);

    if (m_lpddsTimerBmps[1] == NULL)
    {
        fprintf(stderr, "CFinishLine::LoadBitmaps error!!  Did not load m_lpddsTimerBmps[1]\n");
    }

    m_lpddsTimerBmps[2] = ProbGrid->DDrawLoadBitmap(IDB_TIMER_BOTTOM);

    if (m_lpddsTimerBmps[2] == NULL)
    {
        fprintf(stderr, "CFinishLine::LoadBitmaps error!!  Did not load m_lpddsTimerBmps[2]\n");
    }

    m_lpddsTimerBmps[3] = ProbGrid->DDrawLoadBitmap(IDB_TIMER_LEFT);

    if (m_lpddsTimerBmps[3] == NULL)
    {
        fprintf(stderr, "CFinishLine::LoadBitmaps error!!  Did not load m_lpddsTimerBmps[3]\n");
    }
}

LPDIRECTDRAWSURFACE3 CFinishLine::GetFinishBitmap(float ZoomLevel)
{
    LPDIRECTDRAWSURFACE3* BmpArray;

    if (ZoomLevel == cZoomLevel1)
    {
        BmpArray = m_lpddsAnimationBmps1;
    }
    else if (ZoomLevel == cZoomLevel2)
    {
        BmpArray = m_lpddsAnimationBmps2;
    }
    else
    {
        BmpArray = m_lpddsAnimationBmps3;
    }

    LPDIRECTDRAWSURFACE3 Bmp = NULL;

    if (GetAnimate())
    {
        switch (GetAnimationFrame())
        {
        case 0:
        case 1:
        case 2:
            Bmp = BmpArray[GetAnimationFrame()];
            break;

        case 3:
        case 4:
            Bmp = BmpArray[3];
            break;

        case 5:
        case 6:
        case 7:
            Bmp = BmpArray[GetAnimationFrame() - 1];
            break;

        case 8:
        case 9:
            Bmp = BmpArray[7];
            break;

        default:
            ASSERT(FALSE);
            break;
        }
    }
    else
    {
        Bmp = BmpArray[7];
    }

    return Bmp;
}

void CFinishLine::DrawFinish(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid *ProbGrid)
{
    const unsigned long cTransparencyColor = 0x00FF00;

    LPDIRECTDRAWSURFACE3	Bmp = NULL;

    Bmp = GetFinishBitmap(ProbGrid->GetZoomLevel());

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

    DDCOLORKEY ColorKey;
    ColorKey.dwColorSpaceHighValue = m_TransparencyColor;
    ColorKey.dwColorSpaceLowValue = m_TransparencyColor;
    ProbGrid->TransparentBlt(BackBuffer, destRect, Bmp, rcRect, ColorKey);
}

short CFinishLine::GetTimerBoxWidth(float ZoomLevel)
{
    short Width = 0;

    switch (GetTimerPosition())
    {
    case 0:
    case 2:
        Width = 49;
        break;

    case 1:
    case 3:
        Width = 54;
        break;

    default:
        ASSERT(FALSE);
        break;
    }

    return Width;
}

short CFinishLine::GetTimerBoxHeight(float ZoomLevel)
{
    short Height = 0;

    switch (GetTimerPosition())
    {
    case 0:
    case 2:
        Height = 22;
        break;

    case 1:
    case 3:
        Height = 17;
        break;

    default:
        ASSERT(FALSE);
        break;
    }

    return Height;
}

void CFinishLine::DrawTimerBox(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid *ProbGrid)
{
    CRect rcRect, destRect;

    /*Get the bitmap dimensions*/
    short BmpFrameWidth;
    short BmpFrameHeight;
    BmpFrameWidth = GetTimerBoxWidth(ProbGrid->GetZoomLevel());
    BmpFrameHeight = GetTimerBoxHeight(ProbGrid->GetZoomLevel());
    rcRect.left = 0;
    rcRect.top = 0;
    rcRect.right = rcRect.left + BmpFrameWidth;
    rcRect.bottom = rcRect.top + BmpFrameHeight;
    /*Get the right bitmap*/
    LPDIRECTDRAWSURFACE3	Bmp = NULL;

    Bmp = m_lpddsTimerBmps[GetTimerPosition()];

    if (Bmp->IsLost() == DDERR_SURFACELOST)
    {
        Bmp->Restore();
    }

    /*Find the destination rectangles*/
    float temp;
    robPOINT bmpLocation;

    switch (GetTimerPosition())
    {
    case 0:
        bmpLocation.x = (m_Bounds.left + m_Bounds.right) / 2;
        bmpLocation.y = (m_Bounds.top);

        temp = ProbGrid->RobotToDDSurfaceX(bmpLocation.x, ProbabilityGridUpdateRect.left);
        temp -= BmpFrameWidth / 2;
        destRect.left = Round(temp);
        destRect.right = destRect.left + BmpFrameWidth;
        destRect.bottom = (long)ProbGrid->RobotToDDSurfaceY(bmpLocation.y, ProbabilityGridUpdateRect.top);
        destRect.top = destRect.bottom - BmpFrameHeight;
        break;

    case 1:
        bmpLocation.x = m_Bounds.right;
        bmpLocation.y = (m_Bounds.top + m_Bounds.bottom) / 2;

        destRect.left = (long)ProbGrid->RobotToDDSurfaceX(bmpLocation.x, ProbabilityGridUpdateRect.left);
        destRect.right = destRect.left + BmpFrameWidth;
        temp = ProbGrid->RobotToDDSurfaceY(bmpLocation.y, ProbabilityGridUpdateRect.top);
        temp -= BmpFrameHeight / 2;
        destRect.top = Round(temp);
        destRect.bottom = destRect.top + BmpFrameHeight;
        break;

    case 2:
        bmpLocation.x = (m_Bounds.left + m_Bounds.right) / 2;
        bmpLocation.y = (m_Bounds.bottom);

        temp = ProbGrid->RobotToDDSurfaceX(bmpLocation.x, ProbabilityGridUpdateRect.left);
        temp -= BmpFrameWidth / 2;
        destRect.left = Round(temp);
        destRect.right = destRect.left + BmpFrameWidth;
        destRect.top = (long)ProbGrid->RobotToDDSurfaceY(bmpLocation.y, ProbabilityGridUpdateRect.top);
        destRect.bottom = destRect.top + BmpFrameHeight;
        break;

    case 3:
        bmpLocation.x = m_Bounds.left;
        bmpLocation.y = (m_Bounds.top + m_Bounds.bottom) / 2;

        destRect.left = (long)ProbGrid->RobotToDDSurfaceX(bmpLocation.x, ProbabilityGridUpdateRect.left) - BmpFrameWidth;
        destRect.right = destRect.left + BmpFrameWidth;
        temp = ProbGrid->RobotToDDSurfaceY(bmpLocation.y, ProbabilityGridUpdateRect.top);
        temp -= BmpFrameHeight / 2;
        destRect.top = Round(temp);
        destRect.bottom = destRect.top + BmpFrameHeight;
        break;
    }

    DDCOLORKEY DDColorKey;
    DDColorKey.dwColorSpaceHighValue = m_TransparencyColor;
    DDColorKey.dwColorSpaceLowValue = m_TransparencyColor;
    ProbGrid->TransparentBlt(BackBuffer, destRect, Bmp, rcRect, DDColorKey);
}

void CFinishLine::Serialize(CArchive &archive)
{
    UINT nVersion = archive.GetObjectSchema();
    archive.SetObjectSchema(nVersion);

    if (archive.IsLoading() && (nVersion <= 2))
    {
        CRWGraphicObject::Serialize(archive);
    }
    else
    {
        CExecutableRWGraphicObject::Serialize(archive);
    }

    SetCaption(0);
}
