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
// PathList.cpp: implementation of the CPathList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PathList.h"
#include "Path.h"
#include "ExecutableRWGraphicObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

World* CPathList::world = NULL;

CPathList::CPathList()
{
    m_Head = NULL;
}

CPathList::~CPathList()
{
    Purge();
}

CPathListNode::CPathListNode()
{
    m_ToERWGObject = NULL;
    m_Next = NULL;
    m_Path = NULL;
}

void CPathList::Purge()
{
    while (m_Head != NULL)
    {
        CPathListNode* NextNode = m_Head->m_Next;
        delete m_Head;
        m_Head = NextNode;
    }
}

CPathListNode::~CPathListNode()
{
    delete m_Path;
}

void CPathList::Serialize(CArchive & archive)
{
    CPathListNode* CurrentNode;

    if (archive.IsStoring())
    {
        archive << NumberOfNodes();
        CurrentNode = m_Head;

        while (CurrentNode != NULL)
        {
            CurrentNode->Serialize(archive);
            CurrentNode = CurrentNode->m_Next;
        }
    }
    else
    {
        unsigned long NumberOfNodes;
        archive >> NumberOfNodes;

        while (NumberOfNodes)
        {
            CurrentNode = new CPathListNode;
            CurrentNode->Serialize(archive);
            Add(CurrentNode);
            NumberOfNodes--;
        }
    }
}

void CPathListNode::Serialize(CArchive & archive)
{
    int nPoints;
    double x, y;

    if (archive.IsStoring())
    {
        nPoints = m_Path->GetNumberOfPoints();
        archive << nPoints;

        for (int i = 0; i < nPoints; i++)
        {
            m_Path->GetPoint(i, x, y);
            archive << x << y;
        }

        archive << m_ToERWGObject;
    }
    else
    {
        Path* NewPath;
        archive >> nPoints;

        if (nPoints == 1)
        {
            archive >> x >> y;
            NewPath = new Path(CPathList::world, x, y, x, y);
        }
        else if (nPoints < 1)
        {
            NewPath = NULL;
            ASSERT(FALSE);
        }
        else
        {
            double x2, y2;
            archive >> x >> y;
            archive >> x2 >> y2;
            nPoints -= 2;
            NewPath = new Path(CPathList::world, x, y, x2, y2);

            for (; nPoints; nPoints--)
            {
                archive >> x >> y;
                NewPath->AppendPoint(x, y);
            }
        }

        archive >> m_ToERWGObject;
        m_Path = NewPath;
    }
}

unsigned long CPathList::NumberOfNodes()
{
    CPathListNode* CurrentNode = m_Head;
    unsigned long NumberOfNodes = 0;

    while (CurrentNode != NULL)
    {
        NumberOfNodes++;
        CurrentNode = CurrentNode->m_Next;
    }

    return NumberOfNodes;
}

void CPathList::Add(CPathListNode * NewNode)
{
    NewNode->m_Next = m_Head;
    m_Head = NewNode;
}

Path* CPathList::GetPathBetween(robPOINT P1, robPOINT P2, bool& Forward)
{
    bool done = false;
    CPathListNode* CurrentNode = m_Head;

    while ((CurrentNode != NULL) && (!done))
    {
        done = CurrentNode->GoesBetween(P1, P2, Forward);

        if (!done)
        {
            CurrentNode = CurrentNode->m_Next;
        }
    }

    Path* BetweenPath = NULL;

    if (CurrentNode != NULL)
    {
        BetweenPath = CurrentNode->m_Path;
    }

    return BetweenPath;
}

bool CPathListNode::GoesBetween(robPOINT P1, robPOINT P2, bool& Forward)
{
    bool Between = false;

    robPOINT PathP1, PathP2;
    m_Path->GetPoint(0, PathP1.x, PathP1.y);
    m_Path->GetPoint(m_Path->GetNumberOfPoints() - 1, PathP2.x, PathP2.y);

    const double close = 12.0 * cTPI;

    if (Distance(P1, PathP1) <= close)
    {
        if (Distance(P2, PathP2) <= close)
        {
            Between = true;
            Forward = true;
        }
    }
    else if (Distance(P2, PathP1) <= close)
    {
        if (Distance(P1, PathP2) <= close)
        {
            Between = true;
            Forward = false;
        }
    }

    return Between;
}

void CPathList::Remove(Path *RemovePath)
{
    if (m_Head == NULL)
    {
        return;
    }

    CPathListNode* CurrentNode;

    if (m_Head->m_Path == RemovePath)
    {
        CurrentNode = m_Head->m_Next;
        delete m_Head;
        m_Head = CurrentNode;
        return;
    }

    CurrentNode = m_Head->m_Next;
    CPathListNode* PreviousNode = m_Head;

    while (CurrentNode != NULL)
    {
        if (CurrentNode->m_Path == RemovePath)
        {
            PreviousNode->m_Next = CurrentNode->m_Next;
            delete CurrentNode;
            return;
        }

        CurrentNode = CurrentNode->m_Next;
    }
}

void CPathList::Add(Path *NewPath)
{
    if (NewPath != NULL)
    {
        CPathListNode *NewNode = new CPathListNode();
        NewNode->m_Path = NewPath;
        Add(NewNode);
    }
}

void CPathList::RemovePathsTo(robPOINT RemovePoint)
{
    if (m_Head == NULL)
    {
        return;
    }

    CPathListNode* CurrentNode;
    robPOINT CurrentPoint1, CurrentPoint2;
    bool RemovedHead = false;

    do
    {
        CurrentNode = m_Head;
        CurrentNode->m_Path->GetPoint(0, CurrentPoint1.x, CurrentPoint1.y);
        CurrentNode->m_Path->GetPoint(m_Head->m_Path->GetNumberOfPoints() - 1, CurrentPoint2.x, CurrentPoint2.y);

        if ((Distance(CurrentPoint1, RemovePoint) <= cCloseDistance) || (Distance(CurrentPoint2, RemovePoint) <= cCloseDistance))
        {
            CurrentNode = m_Head->m_Next;
            delete m_Head;
            m_Head = CurrentNode;
            RemovedHead = true;
        }
        else
        {
            RemovedHead = false;
        }
    }
    while (RemovedHead && (m_Head != NULL));

    if (m_Head != NULL)
    {
        CurrentNode = m_Head->m_Next;
        CPathListNode* PreviousNode = m_Head;

        while (CurrentNode != NULL)
        {
            CurrentNode->m_Path->GetPoint(0, CurrentPoint1.x, CurrentPoint1.y);
            CurrentNode->m_Path->GetPoint(m_Head->m_Path->GetNumberOfPoints() - 1, CurrentPoint2.x, CurrentPoint2.y);

            if ((Distance(CurrentPoint1, RemovePoint) <= cCloseDistance) || (Distance(CurrentPoint2, RemovePoint) <= cCloseDistance))
            {
                PreviousNode->m_Next = CurrentNode->m_Next;
                delete CurrentNode;
                CurrentNode = PreviousNode;
            }

            CurrentNode = CurrentNode->m_Next;
        }
    }
}
