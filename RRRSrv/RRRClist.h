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
// Module:     RRRCList.h
// Abstract:   interface of the RRRClistEntry & RRRClist classes
// Author:     J.C.H
// Copyright 1997 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

#ifndef RRRCLIST
#define RRRCLIST

class RRRCListEntry : public CObject
{
        DECLARE_DYNAMIC(RRRCListEntry)

    public:
        RRRCListEntry(long ConnectId, long hWnd, LPCTSTR ControlName, short NodeAddress);
        ~RRRCListEntry();

        // Attributes
    public:

        // Operations
    public:

        // Overrides

        // Implementation
    private:

    public:
        long m_lConnectId;                  // connection id
        long m_hWnd;                        // window handle
        CString m_sControlName;             // connected control's instance name
        short m_NodeAddress;                // node address control is attached to
};

class RRRCList : public CObArray
{
        DECLARE_DYNAMIC(RRRCList)

    public:
        RRRCList();
        ~RRRCList();

        // Attributes
    public:

        // Operations
    public:
        void AddEntry(long ConnectId, long wHnd, LPCTSTR ControlName, short NodeAddress);
        void DeleteEntry(long ConnectId);
        short GetEntryAddress(long ConnectId);
        BOOL AddressInUse(short Address);
        void PostAllClients(UINT Msg, WPARAM wParam, LPARAM lParam, short NodeAddress);
        void SendAllClients(UINT Msg, WPARAM wParam, LPARAM lParam, short NodeAddress);

        // Overrides

        // Implementation
    private:
        CSemaphore m_oSem;                  // multithreading lock support

    public:
};

#endif
/////////////////////////////////////////////////////////////////////////////
