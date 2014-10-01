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
 ///////////////////////////////////////////////////////////////////////////////
 //
 // Module:     RRRCList.cpp
 //
 // Abstract:   implementation of the RRRCListEntry & RRRCList classes
 //
 // Author:     J.C.H	
 //
 // Revisions:
 //    $History: RRRClist.cpp $
 // 
 // *****************  Version 4  *****************
 // User: Harnden      Date: 10/12/99   Time: 12:23a
 // Updated in $/RRR/RRRSrv
 // Added multi robot support.
 // 
 // *****************  Version 3  *****************
 // User: Harnden      Date: 1/17/98    Time: 4:32p
 // Updated in $/RRRSrv
 // First working copy tested against RobotSim.
 // 
 // *****************  Version 2  *****************
 // User: Harnden      Date: 1/12/98    Time: 12:45a
 // Updated in $/RRRSrv
 // First delivery of ActiveX interface.
 //
 // Copyright 1997 Cycle Time Corp
 //
 ///////////////////////////////////////////////////////////////////////////////
 
 #include "stdafx.h"
 
 #include "RRRClist.h"
 
 #ifdef _DEBUG
 #define new DEBUG_NEW
 #undef THIS_FILE
 static char THIS_FILE[] = __FILE__;
 #endif
 
 /////////////////////////////////////////////////////////////////////////////
 // RRRCListEntry
 
 IMPLEMENT_DYNAMIC(RRRCListEntry, CObject)
 
 /////////////////////////////////////////////////////////////////////////////
 // RRRCListEntry construction/destruction
 
 RRRCListEntry::RRRCListEntry(long ConnectId, long hWnd, LPCTSTR ControlName, short NodeAddress) 
 : m_lConnectId(ConnectId)
 , m_hWnd(hWnd)
 , m_sControlName(ControlName)
 , m_NodeAddress(NodeAddress)
 {
 }
 
 RRRCListEntry::~RRRCListEntry()
 {
 }
 
 /////////////////////////////////////////////////////////////////////////////
 // RRRCList
 
 IMPLEMENT_DYNAMIC(RRRCList, CObArray)
 
 /////////////////////////////////////////////////////////////////////////////
 // RRRCListEntry construction/destruction
 
 RRRCList::RRRCList()
 {
 }
 
 RRRCList::~RRRCList()
 {
    for (short i=0;i < GetSize(); i++)
    {
       RRRCListEntry *entry=(RRRCListEntry *)ElementAt(i);
       delete entry;
    }
    RemoveAll();
 }
 
 void RRRCList::AddEntry(long ConnectId, long wHnd, LPCTSTR ControlName, short NodeAddress)
 {
    // lock the critical section
    CSingleLock lock(&m_oSem, TRUE);
 
    // create and add the entry
    RRRCListEntry *entry = new RRRCListEntry(ConnectId, wHnd, ControlName, NodeAddress);
    Add(entry);
 
    // release the critical section
    lock.Unlock(1);
 }
 
 void RRRCList::DeleteEntry(long ConnectId)
 {
    BOOL Deleted=FALSE;
    // lock the critical section
    CSingleLock lock(&m_oSem, TRUE);
 
    for (short i=0;(i<GetSize()) && (!Deleted);i++)
    {
       RRRCListEntry *entry = (RRRCListEntry *)ElementAt(i);
       if (entry->m_lConnectId == ConnectId)
       {
          delete entry;
          RemoveAt(i);
          Deleted=TRUE;
       }
    }
 
    // release the critical section
    lock.Unlock(1);
 }
 
 short RRRCList::GetEntryAddress(long ConnectId)
 {
    short Address=-1;
    // lock the critical section
    CSingleLock lock(&m_oSem, TRUE);
 
    for (short i=0;(i<GetSize()) && (Address == -1);i++)
    {
       RRRCListEntry *entry = (RRRCListEntry *)ElementAt(i);
       if (entry->m_lConnectId == ConnectId)
       {
          Address = entry->m_NodeAddress;
       }
    }
 
    // release the critical section
    lock.Unlock(1);
    return Address;
 }
 
 BOOL RRRCList::AddressInUse(short Address)
 {
    BOOL InUse=FALSE;
    // lock the critical section
    CSingleLock lock(&m_oSem, TRUE);
 
    for (short i=0;i<GetSize();i++)
    {
       RRRCListEntry *entry = (RRRCListEntry *)ElementAt(i);
       if (entry->m_NodeAddress == Address)
       {
          InUse = TRUE;
       }
    }
 
    // release the critical section
    lock.Unlock(1);
    return InUse;
 }
 
 void RRRCList::PostAllClients(UINT Msg, WPARAM wParam, LPARAM lParam, short NodeAddress)
 {
    CDWordArray windows;
    // lock the critical section
    CSingleLock lock(&m_oSem, TRUE);
 
    for (short i=0;i<GetSize();i++)
    {
       RRRCListEntry *entry = (RRRCListEntry *)ElementAt(i);
       if (NodeAddress == entry->m_NodeAddress)
          windows.Add(entry->m_hWnd);
    }
    // release the critical section
    lock.Unlock(1);
    for (i=0; i<windows.GetSize();i++)
       PostMessage((HWND)windows[i], Msg, wParam, lParam);
 }
 
 void RRRCList::SendAllClients(UINT Msg, WPARAM wParam, LPARAM lParam, short NodeAddress)
 {
    CDWordArray windows;
    // lock the critical section
    CSingleLock lock(&m_oSem, TRUE);
 
    for (short i=0;i<GetSize();i++)
    {
       RRRCListEntry *entry = (RRRCListEntry *)ElementAt(i);
       if (NodeAddress == entry->m_NodeAddress)
          windows.Add(entry->m_hWnd);
    }
    // release the critical section
    lock.Unlock(1);
    for (i=0; i<windows.GetSize();i++)
       SendMessage((HWND)windows[i], Msg, wParam, lParam);
 }
