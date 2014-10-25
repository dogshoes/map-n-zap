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
#include "stdafx.h"
#include "CTDPath.h"
#include "RRDim.h"
const unsigned short cMaxPathSize = 65530;

bool CCTDPath::AddArc(robPOINT Start, robPOINT End, robPOINT OnArc)
/*******************************************************************************
//Created: 10/21/97 S.R.
//Last Revision: 10/23/97 S.R.
//Parameters:
//	Start:	point to start the arc
//	End:		point to end the arc
//	OnArc:	a third point on the arc
//Return Value:
//	false if no room in path for the straight line.  True otherwise.
********************************************************************************
//Description:
//		This function adds a circular arc to the robot's connect the dots path.
//	The arc is defined by the three points Start, End and OnArc.
//Method:
//		Please see the hand-written notes "Robot Connect the dot path: Creating an arc from 3 points"
//	created 10/21/97.
*******************************************************************************/
{
    robPOINT Center;
    double Radius;
    FindCircle(Start, End, OnArc, &Center, &Radius);
    double ThetaStart, ThetaEnd, Theta3;
    ThetaStart = atan2(Start.y - Center.y, Start.x - Center.x);

    if (ThetaStart < 0)
    {
        ThetaStart += 2 * pi;
    }

    ThetaEnd = atan2(End.y - Center.y, End.x - Center.x);

    if (ThetaEnd < 0)
    {
        ThetaEnd += 2 * pi;
    }

    Theta3 = atan2(OnArc.y - Center.y, OnArc.x - Center.x);

    if (Theta3 < 0)
    {
        Theta3 += 2 * pi;
    }

    double PhiEnd, Phi3;
    PhiEnd = ThetaEnd - ThetaStart;

    if (PhiEnd < 0)
    {
        PhiEnd += 2 * pi;
    }

    Phi3 = Theta3 - ThetaStart;

    if (Phi3 < 0)
    {
        Phi3 += 2 * pi;
    }

    /*Find which way to rotate*/
    bool PositiveAngularRotation;

    if (Phi3 < PhiEnd)
    {
        PositiveAngularRotation = true;
    }
    else
    {
        PositiveAngularRotation = false;
    }

    /*Find the length of the arc*/
    double ArcLength;

    if (PositiveAngularRotation)
    {
        ArcLength = Radius * PhiEnd;
    }
    else
    {
        ArcLength = Radius * (2 * pi - PhiEnd);
    }

    /*Find the angular difference between the points*/
    double AngDiff = m_PointSpacing / Radius;
    /*Find the number of points in the path*/
    unsigned short PathLength = unsigned short(ArcLength / m_PointSpacing);

    if (((long)PathLength) + m_Size >= cMaxPathSize)
    {
        ASSERT(false);
        return false;
    }

    /*New Path Segment creation*/
    unsigned short IndexOffset;

    if (m_Path == NULL)
    {
        m_Path = new robPOINT[PathLength];
        m_Size = PathLength;
        IndexOffset = 0;
    }
    else
    {
        robPOINT* NewPath = new robPOINT[PathLength + m_Size];

        for (unsigned short i = 0; i < m_Size; i++)
        {
            NewPath[i] = m_Path[i];
        }

        delete m_Path;
        m_Path = NewPath;
        IndexOffset = m_Size;
        m_Size += PathLength;
    }

    double theta = ThetaStart;

    for (unsigned short i = IndexOffset; i < m_Size; i++)
    {
        m_Path[i].x = Radius * cos(theta) + Center.x;
        m_Path[i].y = Radius * sin(theta) + Center.y;

        if (PositiveAngularRotation)
        {
            theta += AngDiff;
        }
        else
        {
            theta -= AngDiff;
        }
    }

    return true;
}


bool CCTDPath::AddPoint(robPOINT p)
/*******************************************************************************
//Created: 01/07/98 S.R.
//Last Revision: 01/07/98 S.R.
//Parameters:
//	p:	point to add to the CTDPath
//Return Value:
//	false if no room in path for the straight line.  true otherwise.
********************************************************************************
//Description:
//		This function adds a point to a connect-the-dots path.
//Method:
//
*******************************************************************************/
{
    /*NOTE!!! change CCTDPath::AddStraightLine so it just uses AddPoint twice.*/
    unsigned short PathLength;
    double c;
    double s;
    unsigned short IndexOffset;

    if (m_Path == NULL)
    {
        m_Path = new robPOINT;
        m_Size = 1;
        m_Path[0] = p;
    }
    else
    {
        PathLength = (unsigned short)Round(Distance(m_Path[m_Size - 1], p) / m_PointSpacing);

        if (((long)PathLength) + m_Size >= cMaxPathSize)
        {
            ASSERT(false);
            return false;
        }

        //		if (PathLength > 0) {
        double theta = atan2(p.y - m_Path[m_Size - 1].y, p.x - m_Path[m_Size - 1].x);
        c = cos(theta) * m_PointSpacing;
        s = sin(theta) * m_PointSpacing;
        /*Copy old path to new, larger array*/
        robPOINT* NewPath = new robPOINT[PathLength + m_Size];

        for (unsigned short i = 0; i < m_Size; i++)
        {
            NewPath[i] = m_Path[i];
        }

        delete m_Path;
        m_Path = NewPath;
        /*Start appending to path*/
        IndexOffset = m_Size;
        m_Size += PathLength;

        for (unsigned short i = IndexOffset; i < m_Size; i++)
        {
            m_Path[i].x = m_Path[i - 1].x + c;
            m_Path[i].y = m_Path[i - 1].y + s;
        }

        /*Make sure it gets to p*/
        m_Path[m_Size - 1] = p;
        //		}
    }

    return true;
}

CCTDPath::CCTDPath(unsigned int Speed)
{
#ifdef JUNK
    TPathSpeed PathSpeed;

    if (Speed < 125)
    {
        PathSpeed = ps100;
    }
    else if (Speed < 175)
    {
        PathSpeed = ps150;
    }
    else if (Speed < 225)
    {
        PathSpeed = ps200;
    }
    else if (Speed < 275)
    {
        PathSpeed = ps250;
    }
    else
    {
        PathSpeed = ps300;
    }

    switch (PathSpeed)
    {
    case ps100:
        m_Speed = 6;
        m_PointSpacing = 10;
        break;

    case ps150:
        m_Speed = 10;
        m_PointSpacing = 10;
        break;

    case ps200:
        m_Speed = 40;
        m_PointSpacing = 10;
        break;

    case ps250:
        m_PointSpacing = 20;
        m_Speed = 18;
        break;

    case ps300:
        m_PointSpacing = 20;
        m_Speed = 22;
        break;
    }

#endif
    SetSpeed(Speed);
    m_Size = 0;
    //	m_Speed = 50;
    m_PointSpacing = 20;
    m_Path = NULL;
    m_PathPosition = 0;
}

bool CCTDPath::AddStraightLine(robPOINT p1, robPOINT p2)
/*******************************************************************************
//Created: 10/20/97 S.R.
//Last Revision: 10/20/97 S.R.
//Parameters:
//	p1, p2:	two points defining the line to add to the CTDPath
//Return Value:
//	false if no room in path for the straight line.  True otherwise.
********************************************************************************
//Description:
//		This function adds a straight line to a connect-the-dots path.
//Method:
//
*******************************************************************************/
{
    unsigned short PathLength = unsigned short(Distance(p1, p2) / m_PointSpacing + 1);

    if (((long)PathLength) + m_Size >= cMaxPathSize)
    {
        ASSERT(false);
        return false;
    }

    double theta = atan2(p2.y - p1.y, p2.x - p1.x);
    double c = cos(theta) * m_PointSpacing;
    double s = sin(theta) * m_PointSpacing;
    unsigned short IndexOffset;

    if (m_Path == NULL)
    {
        m_Path = new robPOINT[PathLength];
        m_Size = PathLength;
        IndexOffset = 0;
    }
    else
    {
        robPOINT* NewPath = new robPOINT[PathLength + m_Size];

        for (unsigned short i = 0; i < m_Size; i++)
        {
            NewPath[i] = m_Path[i];
        }

        delete m_Path;
        m_Path = NewPath;
        IndexOffset = m_Size;
        m_Size += PathLength;
    }

    m_Path[IndexOffset].x = p1.x + c;
    m_Path[IndexOffset].y = p1.y + s;

    for (unsigned short i = IndexOffset + 1; i < m_Size; i++)
    {
        m_Path[i].x = m_Path[i - 1].x + c;
        m_Path[i].y = m_Path[i - 1].y + s;
    }

    return true;
}


CCTDPath::~CCTDPath()
{
    delete m_Path;
}

unsigned short CCTDPath::GetSpeed(void)
{
    return m_Speed;
};

void CCTDPath::SetSpeed(short Speed)
{
    if (Speed < 2 * cMinSpeed)
    {
        m_Speed = 2 * cMinSpeed;
    }
    else
    {
        m_Speed = Speed;
    }
}

unsigned short CCTDPath::FindClosestPoint(robPOINT Point)
{
    unsigned short ClosestPosition;
    double MinDist, dist;

    /*Take care of empty, 1 point path*/
    if (m_Size <= 1)
    {
        return 0;
    }

    MinDist = Distance(m_Path[0], Point);
    ClosestPosition = 0;

    for (unsigned short i = 1; i < m_Size; i++)
    {
        dist = Distance(m_Path[i], Point);

        if (dist < MinDist)
        {
            MinDist = dist;
            ClosestPosition = i;
        }
    }

    return ClosestPosition;
}

unsigned short CCTDPath::FindClosestPoint(robPOINT Point, unsigned short StartPosition, unsigned short NumPositionsToSearch)
{
    unsigned short ClosestPosition;
    double MinDist, dist;

    /*Take care of empty, 1 point path*/
    if (m_Size <= StartPosition)
    {
        return m_Size;
    }

    MinDist = Distance(m_Path[StartPosition], Point);
    ClosestPosition = StartPosition;

    if ((((long)StartPosition) + NumPositionsToSearch) > m_Size)
    {
        NumPositionsToSearch = m_Size - StartPosition;
    }

    for (unsigned short i = StartPosition + 1; i < StartPosition + NumPositionsToSearch; i++)
    {
        dist = Distance(m_Path[i], Point);

        if (dist < MinDist)
        {
            MinDist = dist;
            ClosestPosition = i;
        }
    }

    return ClosestPosition;
}

CCTDPath::CCTDPath(CCTDPath& CTDPath)
{
    m_Path = new robPOINT[CTDPath.m_Size];
    m_PathPosition = CTDPath.m_PathPosition;
    m_PointSpacing = CTDPath.m_PointSpacing;
    m_Size = CTDPath.m_Size;
    m_Speed = CTDPath.m_Speed;

    for (unsigned short i = 0; i < m_Size; i++)
    {
        m_Path[i] = CTDPath.m_Path[i];
    }

    return;
}

bool CCTDPath::operator !=(CCTDPath P2)
{
    return !(operator ==(P2));
}

bool CCTDPath::operator ==(CCTDPath P2)
{
    if (P2.m_Size != m_Size)
    {
        return false;
    }

    for (unsigned short i = 0; i < m_Size; i++)
    {
        if (m_Path[i] != P2.m_Path[i])
        {
            return false;
        }
    }

    return true;
}
