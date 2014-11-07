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

#pragma once

/// <summary>
/// An entry for the OCX instance list.
/// </summary>
class RRRCListEntry : public CObject
{
    DECLARE_DYNAMIC(RRRCListEntry)

    public:
        /// <summary>
        /// Construct a new OCX instance list entry.
        /// </summary>
        /// <param name="ConnectId">Connection identifier.</param>
        /// <param name="hWnd">Window handle.</param>
        /// <param name="ControlName">Connected control's instance name.</param>
        /// <param name="NodeAddress">Node address control is attached to.</param>
        RRRCListEntry(long ConnectId, long hWnd, LPCTSTR ControlName, short NodeAddress);

        /// <summary>
        /// Destroy this connection list entry.
        /// </summary>
        ~RRRCListEntry();

        /// <summary>
        /// Connection identifier.
        /// </summary>
        long m_lConnectId;

        /// <summary>
        /// Window handle.
        /// </summary>
        long m_hWnd;

        /// <summary>
        /// Connected control's instance name.
        /// </summary>
        CString m_sControlName; 

        /// <summary>
        /// Node address control is attached to.
        /// </summary>
        short m_NodeAddress;
};

/// <summary>
/// A collection of CyeCom.ocx controls connected to this CyeSrv.exe.  This is used to dispatch window messages
/// to these controls.
/// </summary>
/// <remarks>
/// One instance of CyeSrv.exe can service requests from mutliple applications each running their own
/// instance of CyeCom.ocx.  This list class will broker dispatching messages out to connected OCX controls.
/// </remarks>
class RRRCList : public CObArray
{
    DECLARE_DYNAMIC(RRRCList)

    public:
        /// <summary>
        /// Construct a new OCX instance list.
        /// </summary>
        RRRCList();

        /// <summary>
        /// Destroy the OCX instance list.
        /// </summary>
        ~RRRCList();

        /// <summary>
        /// Add a newly established CyeCom.ocx to the list.
        /// </summary>
        /// <remarks>
        /// The robot's node address cannot be changed for an entry after it's been added.  The entry must be
        /// deleted and added again with the changed node address.
        /// </remarks>
        /// <param name="ConnectId">A unique connection identifier.</param>
        /// <param name="wHnd">The window handle of the established OCX which to send messages to.</param>
        /// <param name="ControlName">The ambient display name of the connected OCX.</param>
        /// <param name="NodeAddress">The robot's address which is being commicated with.</param>
        void AddEntry(long ConnectId, long wHnd, LPCTSTR ControlName, short NodeAddress);

        /// <summary>
        /// Remove the link to a CyeCom.ocx.
        /// </summary>
        void DeleteEntry(long ConnectId);

        /// <summary>
        /// Get a connection's robot node address.
        /// </summary>
        /// <param name="ConnectId">The connection to look up the robot's node address for.</param>
        /// <returns>The robot's node address.  Will return -1 if no connection was registered for the supplied identifier.</returns>
        short GetEntryAddress(long ConnectId);

        /// <summary>
        /// Check to see if a connection registered with this list is already communicating with a specific
        /// robot's node address.
        /// </summary>
        /// <param name="Address">The robot node address to look up.</param>
        /// <returns>An indication whether that address is in use.</returns>
        BOOL AddressInUse(short Address);

        /// <summary>
        /// Call PostMessage() against all connected controls which are registered under the supplied 
        /// robot node address.
        /// </summary>
        /// <param name="Msg">The message to send.</param>
        /// <param name="wParam">Additional message-specific information.</param>
        /// <param name="lParam">Additional message-specific information.</param>
        /// <param name="NodeAddress">The robot's node address which generated this message.</param>
        void PostAllClients(UINT Msg, WPARAM wParam, LPARAM lParam, short NodeAddress);

        /// <summary>
        /// Call SendMessage() against all connected controls which are registered under the supplied 
        /// robot node address.
        /// </summary>
        /// <remarks>
        /// This is primarily used to dispatch WM_COPYDATA messages.
        /// </remarks>
        /// <param name="Msg">The message to send.</param>
        /// <param name="wParam">Additional message-specific information.</param>
        /// <param name="lParam">Additional message-specific information.</param>
        /// <param name="NodeAddress">The robot's node address which generated this message.</param>
        void SendAllClients(UINT Msg, WPARAM wParam, LPARAM lParam, short NodeAddress);

    private:
        /// <summary>
        /// Multithreading semaphore.
        /// </summary>
        /// <remarks>
        /// Use when modifying the internal collection of connected OCX controls.
        /// </remarks>
        CSemaphore m_oSem;
};