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
// LinePath.cpp: implementation of the CLinePath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "robotworld.h"
#include "LinePath.h"
#include "Probability Grid.h"
#include "DlgExecutableObjectGeneralProperties.h"
#include "CTDPath.h"
#include "RobotWorldView.h"
#include "global.h"
#include "PathDisplay.h"

IMPLEMENT_SERIAL(CLinePath, CExecutableRWGraphicObject, VERSIONABLE_SCHEMA | VERSION_NUMBER)

const short cTrackerHandleSize = 7;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLinePath::CLinePath() : CExecutableRWGraphicObject()
{
}

CLinePath::~CLinePath()
{

}

void CLinePath::AddPoint(robPOINT Point)
{
    if (m_PointList.IsEmpty())
    {
        SetEntryPoint(Point);
        SetExitPoint(Point);
        m_PointList.AddTail(Point);
    }
    else
    {
        SetExitPoint(Point);
        robPOINT EndPoint = m_PointList.GetTail();

        if ((Point.x != EndPoint.x) || (Point.y != EndPoint.y))
        {
            m_PointList.AddTail(Point);
        }
    }

    CalculateBounds();
}

void CLinePath::Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbabilityGrid)
{
    POSITION Position;

    if (m_IsVisible)
    {
        DrawPath(ProbGridBmpUpdateRect, pDC, ProbabilityGrid);
        /*Draw the caption*/
        int cTextGap = 3;

        if (m_IsVisible)
        {
            if (m_ShowCaption)
            {
                CFont CaptionFont;

                if (ProbabilityGrid->m_ZoomLevel == cZoomLevel3)
                {
                    CaptionFont.CreatePointFont(110, "Arial", pDC);
                }
                else if (ProbabilityGrid->m_ZoomLevel == cZoomLevel2)
                {
                    CaptionFont.CreatePointFont(70, "Arial", pDC);
                }
                else if (ProbabilityGrid->m_ZoomLevel == cZoomLevel1)
                {
                    CaptionFont.CreatePointFont(40, "Arial", pDC);
                }

                CFont* OldFont = pDC->SelectObject(&CaptionFont);
                CPoint SurfaceOffset = ProbGridBmpUpdateRect.TopLeft();
                int OldBkMode = pDC->SetBkMode(TRANSPARENT);
                COLORREF OldTextColor = pDC->SetTextColor(COLORREF(0xFF));
                CString Caption = GetCaption();

                if (m_NumberOfClaps > 1)
                {
                    CString NumberOfClaps;
                    NumberOfClaps.Format(", %d Claps", m_NumberOfClaps);
                    Caption += NumberOfClaps;
                }

                CSize CaptionSize = pDC->GetTextExtent(Caption);
                CaptionSize.cy += cTextGap;
                robPOINT CaptionDimensions;
                CaptionDimensions.x = ProbabilityGrid->ClientSizeToRobotSize(CaptionSize.cx);
                CaptionDimensions.y = ProbabilityGrid->ClientSizeToRobotSize(CaptionSize.cy);

                robPOINT P0, P1;
                Position = m_PointList.GetHeadPosition();

                if (Position != NULL)
                {
                    P0 = m_PointList.GetNext(Position);

                    if (Position != NULL)
                    {
                        P1 = m_PointList.GetNext(Position);

                        if (P0 == P1)
                        {
                            m_CaptionBounds.left = P0.x;
                            m_CaptionBounds.top = P0.y - ProbabilityGrid->ClientSizeToRobotSize(cTextGap);
                            m_CaptionBounds.right = m_CaptionBounds.left + CaptionDimensions.x;
                            m_CaptionBounds.bottom = m_CaptionBounds.top - CaptionDimensions.y;
                        }
                        else
                        {
                            double phi = atan2(P1.y - P0.y, P1.x - P0.x);

                            if (phi >= 0)
                            {
                                m_CaptionBounds.top = P0.y - ProbabilityGrid->ClientSizeToRobotSize(cTextGap);
                                m_CaptionBounds.bottom = m_CaptionBounds.top - CaptionDimensions.y;

                                if (phi <= pi / 2)
                                {
                                    m_CaptionBounds.right = P0.x;
                                    m_CaptionBounds.left = m_CaptionBounds.right - CaptionDimensions.x;
                                }
                                else
                                {
                                    m_CaptionBounds.left = P0.x;
                                    m_CaptionBounds.right = m_CaptionBounds.left + CaptionDimensions.x;
                                }
                            }
                            else
                            {
                                m_CaptionBounds.bottom = P0.y + ProbabilityGrid->ClientSizeToRobotSize(cTextGap);
                                m_CaptionBounds.top = m_CaptionBounds.bottom + CaptionDimensions.y;

                                if (phi >= -pi / 2)
                                {
                                    m_CaptionBounds.right = P0.x;
                                    m_CaptionBounds.left = m_CaptionBounds.right - CaptionDimensions.x;
                                }
                                else
                                {
                                    m_CaptionBounds.left = P0.x;
                                    m_CaptionBounds.right = m_CaptionBounds.left + CaptionDimensions.x;
                                }
                            }
                        }
                    }
                    else
                    {
                        m_CaptionBounds.left = P0.x;
                        m_CaptionBounds.top = P0.y - ProbabilityGrid->ClientSizeToRobotSize(cTextGap);
                        m_CaptionBounds.right = m_CaptionBounds.left + CaptionDimensions.x;
                        m_CaptionBounds.bottom = m_CaptionBounds.top - CaptionDimensions.y;
                    }
                }

                int x = (int)ProbabilityGrid->RobotToDDSurfaceX(m_CaptionBounds.left, SurfaceOffset.x);
                int y = (int)ProbabilityGrid->RobotToDDSurfaceY(m_CaptionBounds.top, SurfaceOffset.y);
                pDC->TextOut(x, y, Caption);
                pDC->SetTextColor(OldTextColor);
                pDC->SetBkMode(OldBkMode);
                pDC->SelectObject(OldFont);
            }

            if (m_EditMode != emNone)
            {
                CPolyLineTracker Tracker = SetupPolyLineTracker(ProbabilityGrid, ProbGridBmpUpdateRect.TopLeft());
                Tracker.Draw(pDC);
            }
        }
    }
}

void CLinePath::DrawPath(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbGrid)
{
    POSITION Position = m_PointList.GetHeadPosition();

    if (Position == NULL)
    {
        return;
    }

    int PenWidth;

    if (ProbGrid->GetZoomLevel() == cZoomLevel3)
    {
        PenWidth = 3;
    }
    else if (ProbGrid->GetZoomLevel() == cZoomLevel2)
    {
        PenWidth = 2;
    }
    else
    {
        PenWidth = 1;
    }

    CPen CenterPen(PS_SOLID, PenWidth, COLORREF(0x03D7FD));
    CPen* OldPen = pDC->SelectObject(&CenterPen);
    int OldROP2 = pDC->SetROP2(R2_COPYPEN);
    robPOINT WorldPoint;
    CPoint SurfaceOffset = ProbGridBmpUpdateRect.TopLeft();

    /*Draw central line*/
    if (Position != NULL)
    {
        WorldPoint = m_PointList.GetNext(Position);
        pDC->MoveTo(ProbGrid->RobotToDDSurface(WorldPoint, SurfaceOffset));
    }

    while (Position != NULL)
    {
        WorldPoint = m_PointList.GetNext(Position);
        pDC->LineTo(ProbGrid->RobotToDDSurface(WorldPoint, SurfaceOffset));
    }

    CPen OuterLinePen(PS_DOT, 1, COLORREF(0x0000FF));
    pDC->SelectObject(&OuterLinePen);
    /*Draw the outer line*/
    DrawOuterLines(SurfaceOffset, pDC, ProbGrid, &m_PointList);

    robPOINT P1, P2;
    CPoint P;
    pDC->SetROP2(OldROP2);
    pDC->SelectObject(OldPen);
}

unsigned int CLinePath::NumPoints()
{
    return m_PointList.GetCount();
}

void CLinePath::CalculateBounds()
{
    POSITION Position = m_PointList.GetHeadPosition();
    robPOINT WorldPoint;

    if (Position != NULL)
    {
        WorldPoint = m_PointList.GetNext(Position);
        m_Bounds.top = m_Bounds.bottom = WorldPoint.y;
        m_Bounds.left = m_Bounds.right = WorldPoint.x;
    }

    while (Position != NULL)
    {
        WorldPoint = m_PointList.GetNext(Position);
        m_Bounds.Include(WorldPoint);
    }
}

void CLinePath::SetBounds(robRECT bounds)
{
    return;
}

void CLinePath::EditProperties()
{
    CDlgExecutableObjectGeneralProperties DlgProperties;
    DlgProperties.SetName(m_Caption);
    DlgProperties.SetNumberOfClaps(m_NumberOfClaps);
    DlgProperties.SetEditingObjectType(GetObjectType());

    if (DlgProperties.DoModal() == IDOK)
    {
        /*Change the label name*/
        SetCaption(DlgProperties.GetName());
        SetNumberOfClaps(DlgProperties.GetNumberOfClaps());
    }
}

TTaskResult CLinePath::SpecialFunction(CRobot * Robot, short Speed)
{
    CCTDPath* Path = new CCTDPath(Speed);
    POSITION Position = NULL;

    if (Distance(GetEntryPoint(), Robot->GetPosition()) <= Distance(GetExitPoint(), Robot->GetPosition()))
    {
        Position = m_PointList.GetHeadPosition();

        while (Position != NULL)
        {
            AddPointToPath(m_PointList.GetNext(Position), &Path, Robot);
        }
    }
    else
    {
        Position = m_PointList.GetTailPosition();

        while (Position != NULL)
        {
            AddPointToPath(m_PointList.GetPrev(Position), &Path, Robot);
        }
    }

    Robot->AddCTDPathToInstructions(Path, cModPositionZone, FALSE);
    TTaskResult TaskResult = Robot->FollowPathWithObstacleAvoidance(Speed, m_ExecutableObjectList->m_ProbabilityGrid, true);
    return TaskResult;
}

bool CLinePath::InsideObject(robPOINT point, float WorldPointsPerPixel)
{
    if (InsideExecuteZone(point, WorldPointsPerPixel))
    {
        return true;
    }

    if (m_ShowCaption)
    {
        return m_CaptionBounds.Inside(point);
    }

    return false;
}

bool CLinePath::InsideExecuteZone(robPOINT point, float WorldPointsPerPixel)
{
    float WidthObject = cTrackerHandleSize * WorldPointsPerPixel / 2;
    POSITION Position = m_PointList.GetHeadPosition();
    robPOINT WorldPoint, PreviousWorldPoint;
    TLine LineSegment;
    PreviousWorldPoint = m_PointList.GetNext(Position);

    if (Distance(PreviousWorldPoint, point) <= WidthObject)
    {
        return true;
    }

    while (Position != NULL)
    {
        WorldPoint = m_PointList.GetNext(Position);
        FindLineFrom2Points(WorldPoint, PreviousWorldPoint, &LineSegment);
        robPOINT Intersection = FindPerpendicularIntersection(LineSegment, point);

        if (PointIsBetween(Intersection, WorldPoint, PreviousWorldPoint))
        {
            if (PerpendicularDistanceFromLine(LineSegment, point) <= WidthObject)
            {
                return true;
            }
        }
        else
        {
            if (Distance(point, WorldPoint) <= WidthObject)
            {
                return true;
            }

            if (Distance(point, PreviousWorldPoint) <= WidthObject)
            {
                return true;
            }
        }

        PreviousWorldPoint = WorldPoint;
    }

    return false;
}

CPolyLineTracker CLinePath::SetupPolyLineTracker(CProbabilityGrid* ProbabilityGrid, CPoint SurfaceOffset)
{
    CPolyLineTracker Tracker;
    CPoint ClientPoint;
    /*Setup line tracker points*/
    POSITION RobotPointsPosition = m_PointList.GetHeadPosition();
    POSITION ClientPointsPosition = Tracker.m_PointList.GetHeadPosition();

    while (RobotPointsPosition != NULL)
    {
        ClientPoint = ProbabilityGrid->RobotToDDSurface(m_PointList.GetNext(RobotPointsPosition), SurfaceOffset);

        if (ClientPointsPosition == NULL)
        {
            Tracker.m_PointList.AddTail(ClientPoint);
        }
        else
        {
            Tracker.m_PointList.SetAt(ClientPointsPosition, ClientPoint);
            Tracker.m_PointList.GetNext(ClientPointsPosition);
        }
    }

    /*Set handle size*/
    Tracker.m_nHandleSize = cTrackerHandleSize;
    return Tracker;
}

CPolyLineTracker CLinePath::SetupPolyLineTracker(CRobotWorldView* View)
{
    CPolyLineTracker Tracker;
    CPoint ClientPoint;
    /*Setup line tracker points*/
    POSITION RobotPointsPosition = m_PointList.GetHeadPosition();
    POSITION ClientPointsPosition = Tracker.m_PointList.GetHeadPosition();

    while (RobotPointsPosition != NULL)
    {
        ClientPoint = View->RobotToClientCoords(m_PointList.GetNext(RobotPointsPosition));

        if (ClientPointsPosition == NULL)
        {
            Tracker.m_PointList.AddTail(ClientPoint);
        }
        else
        {
            Tracker.m_PointList.SetAt(ClientPointsPosition, ClientPoint);
            Tracker.m_PointList.GetNext(ClientPointsPosition);
        }
    }

    /*Set handle size*/
    Tracker.m_nHandleSize = cTrackerHandleSize;
    return Tracker;
}

void CLinePath::Serialize(CArchive & archive)
{
    int NumPoints;
    POSITION Position;
    robPOINT WorldPoint;
    CExecutableRWGraphicObject::Serialize(archive);

    if (archive.IsStoring())
    {
        archive << m_PointList.GetCount();
        Position = m_PointList.GetHeadPosition();

        while (Position != NULL)
        {
            WorldPoint = m_PointList.GetNext(Position);
            archive << WorldPoint.x;
            archive << WorldPoint.y;
        }
    }
    else
    {
        archive >> NumPoints;

        for (int i = 0; i < NumPoints; i++)
        {
            archive >> WorldPoint.x;
            archive >> WorldPoint.y;
            m_PointList.AddTail(WorldPoint);
        }
    }
}

BOOL CLinePath::SetTrackerCursor(CRobotWorldView* pWnd, UINT nHitTest)
{
    CPolyLineTracker Tracker = SetupPolyLineTracker(pWnd);
    return Tracker.SetCursor(pWnd, nHitTest);
}

void CLinePath::OnEditClick(robPOINT point, CRobotWorldView * View)
{
    m_EditMode = emSelected;

    CPolyLineTracker Tracker = SetupPolyLineTracker(View);
    int SelectedPoint = Tracker.FindSelectedPoint(View->RobotToClientCoords(point));

    View->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);

    if (Tracker.Track(View, View->RobotToClientCoords(point)))
    {
        if (SelectedPoint >= 0)
        {
            robPOINT WorldPoint;
            WorldPoint = View->ClientToRobotCoords(Tracker.m_PointList.GetAt(Tracker.m_PointList.FindIndex(SelectedPoint)));
            MovePoint(SelectedPoint, WorldPoint);
        }
        else
        {
            POSITION LineTrackerPosition, LinePathPosition;
            LineTrackerPosition = Tracker.m_PointList.GetHeadPosition();
            LinePathPosition = m_PointList.GetHeadPosition();

            while (LinePathPosition != NULL)
            {
                robPOINT WorldPoint = View->ClientToRobotCoords(Tracker.m_PointList.GetNext(LineTrackerPosition));
                MovePoint(LinePathPosition, WorldPoint);
                m_PointList.GetNext(LinePathPosition);
            }

            CalculateBounds();
        }

        View->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
    }
}

void CLinePath::MovePoint(int Index, robPOINT Position)
{
    if (Index >= 0)
    {
        if (Index == 0)
        {
            SetEntryPoint(Position);
        }

        if (Index == m_PointList.GetCount() - 1)
        {
            SetExitPoint(Position);
        }

        if (Index >= m_PointList.GetCount())
        {
            return;
        }

        m_PointList.SetAt(m_PointList.FindIndex(Index), Position);
        CalculateBounds();
    }
}

void CLinePath::SetEditMode(TEditMode EditMode)
{
    m_EditMode = EditMode;
}

void CLinePath::MovePoint(POSITION PointListPosition, robPOINT Point)
{
    if (PointListPosition == NULL)
    {
        return;
    }

    if (PointListPosition == m_PointList.GetHeadPosition())
    {
        SetEntryPoint(Point);
    }

    if (PointListPosition == m_PointList.GetTailPosition())
    {
        SetExitPoint(Point);
    }

    m_PointList.SetAt(PointListPosition, Point);
    CalculateBounds();
}

void CLinePath::AddPointToPath(robPOINT P, CCTDPath **RobotPath, CRobot* Robot)
{
    if (!(*RobotPath)->AddPoint(P))
    {
        Robot->AddCTDPathToInstructions(*RobotPath, cModPositionZone, FALSE);
        *RobotPath = new CCTDPath((*RobotPath)->GetSpeed());
        (*RobotPath)->AddPoint(P);
    }
}

bool CLinePath::IsPathBetween(robPOINT Start, CExecutableRWGraphicObject* Destination)
{
    bool IsPath = false;

    if (Distance(Start, GetEntryPoint()) <= cCloseDistance)
    {
        if (Destination->DistanceToEntryPoint(GetExitPoint()) <= cCloseDistance)
        {
            IsPath = true;
        }
    }
    else if (Distance(Start, GetExitPoint()) <= cCloseDistance)
    {
        if (Destination->DistanceToEntryPoint(GetEntryPoint()) <= cCloseDistance)
        {
            IsPath = true;
        }
    }

    return IsPath;
}

double CLinePath::DistanceToEntryPoint(robPOINT From)
{
    double MinDist = Distance(From, GetEntryPoint());
    double Dist2 = Distance(From, GetExitPoint());

    if (Dist2 < MinDist)
    {
        MinDist = Dist2;
    }

    return MinDist;
}

Path* CLinePath::CreatePathToEntryPoint(robPOINT From, CRobot* Robot)
{
    Path* Result = NULL;

    if (Distance(From, GetEntryPoint()) <= Distance(From, GetExitPoint()))
    {
        Result = Robot->CreatePath(From, GetEntryPoint());
    }
    else
    {
        Result = Robot->CreatePath(From, GetExitPoint());
    }

    return Result;
}

CGoalSet* CLinePath::FindPathToEntryPoint(robPOINT From, CRobot* Robot, bool& GoodPath)
{
    CGoalSet* Result = NULL;

    if (Distance(From, GetEntryPoint()) <= Distance(From, GetExitPoint()))
    {
        Result = Robot->FindPathNow(From, GetEntryPoint(), cFindPathIterationsDefault, cFindPathRepeatDefault, true);
    }
    else
    {
        Result = Robot->FindPathNow(From, GetExitPoint(), cFindPathIterationsDefault, cFindPathRepeatDefault, true);
    }

    GoodPath = true;
    return Result;
}

robPOINT CLinePath::GetEntryPoint(robPOINT From)
{
    robPOINT EntryPoint;

    if (Distance(From, GetEntryPoint()) <= Distance(From, GetExitPoint()))
    {
        EntryPoint = GetEntryPoint();
    }
    else
    {
        EntryPoint = GetExitPoint();
    }

    return EntryPoint;
}