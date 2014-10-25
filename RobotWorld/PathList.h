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
// PathList.h: interface for the CPathList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHLIST_H__B3B46FE0_9318_11D1_B231_0040F6B87672__INCLUDED_)
#define AFX_PATHLIST_H__B3B46FE0_9318_11D1_B231_0040F6B87672__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Robot.h"
#include "rothmath.h"

class Path;
class World;
class CExecutableRWGraphicObject;

class CPathListNode
{
    public:
        void Serialize(CArchive & archive);
        ~CPathListNode();
        CPathListNode();
        CPathListNode* m_Next;
        Path*	m_Path;
        CExecutableRWGraphicObject* m_ToERWGObject;
        bool GoesBetween(robPOINT P1, robPOINT P2, bool& Forward);
    private:

    protected:

};

class CPathList
{
    public:
        void RemovePathsTo(robPOINT Point);
        void Add(Path* NewPath);
        void Remove(CPathListNode* Node);
        void Remove(Path* RemovePath);
        unsigned long NumberOfNodes(void);
        void Serialize(CArchive & archive);
        Path* GetPathBetween(robPOINT P1, robPOINT P2, bool& Forward);
        void Purge(void);
        CPathList();
        virtual ~CPathList();
        static World* world;
    private:
        void Add(CPathListNode* NewNode);
        CPathListNode* m_Head;
};

#endif // !defined(AFX_PATHLIST_H__B3B46FE0_9318_11D1_B231_0040F6B87672__INCLUDED_)
