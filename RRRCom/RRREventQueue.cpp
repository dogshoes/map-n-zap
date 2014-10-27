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
// RRRCList.cpp : implementation of the RRRCListEntry & RRRCList classes
//

#include "stdafx.h"

#include "RRREventQueue.h"

/////////////////////////////////////////////////////////////////////////////
// RRRCListEntry

IMPLEMENT_DYNAMIC(RRREventQueueEntry, CObject)

/////////////////////////////////////////////////////////////////////////////
// RRRCListEntry construction/destruction

RRREventQueueEntry::RRREventQueueEntry(UINT msg, WPARAM wparam, LPARAM lparam)
    : Msg(msg), wParam(wparam), lParam(lparam)
{
}

RRREventQueueEntry::~RRREventQueueEntry()
{
}

/////////////////////////////////////////////////////////////////////////////
// RRRCList

IMPLEMENT_DYNAMIC(RRREventQueue, CObArray)

/////////////////////////////////////////////////////////////////////////////
// RRRCListEntry construction/destruction

RRREventQueue::RRREventQueue()
{
}

RRREventQueue::~RRREventQueue()
{
    for (short i = 0; i < GetSize(); i++)
    {
        RRREventQueueEntry *entry = (RRREventQueueEntry *)ElementAt(i);
        delete entry;
    }

    RemoveAll();
}

void RRREventQueue::AddEntry(UINT Msg, WPARAM wParam, LPARAM lParam)
{
    // lock the critical section
    CSingleLock lock(&m_oSem, TRUE);

    // create and add the entry
    RRREventQueueEntry *entry = new RRREventQueueEntry(Msg, wParam, lParam);
    Add(entry);

    // release the critical section
    lock.Unlock(1);
}

void RRREventQueue::RepostEvents(HWND hWnd)
{
    CDWordArray windows;
    // lock the critical section
    CSingleLock lock(&m_oSem, TRUE);

    for (short i = 0; i < GetSize(); i++)
    {
        RRREventQueueEntry *entry = (RRREventQueueEntry *)ElementAt(i);
        PostMessage(hWnd, entry->Msg, entry->wParam, entry->lParam);
        delete entry;
    }

    // clear the list
    RemoveAll();
    // release the critical section
    lock.Unlock(1);
}