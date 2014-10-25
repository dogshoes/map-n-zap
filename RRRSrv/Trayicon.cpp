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
// Module:     Trayicon.cpp
//
// Abstract:   CTrayIcon Copyright 1996 Microsoft Systems Journal.
//             If this code works, it was written by Paul DiLascia.
//             If not, I don't know who wrote it.
//
// Author:     J.C.H
//
// Revisions:
//    $History: Trayicon.cpp $
//
// *****************  Version 2  *****************
// User: Harnden      Date: 3/22/98    Time: 7:38p
// Updated in $/RRRSrv
// Updated status to include work at CTC.
//
// *****************  Version 1  *****************
// User: Harnden      Date: 3/22/98    Time: 9:59a
// Created in $/RRRSrv
//
// Copyright 1997 Cycle Time Corp
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "trayicon.h"
#include <afxpriv.h>		// for AfxLoadString

IMPLEMENT_DYNAMIC(CTrayIcon, CCmdTarget)

CTrayIcon::CTrayIcon(UINT uID)
{
    // Initialize NOTIFYICONDATA
    memset(&m_nid, 0 , sizeof(m_nid));
    m_nid.cbSize = sizeof(m_nid);
    m_nid.uID = uID;	// never changes after construction

    // Use resource string as tip if there is one
    AfxLoadString(uID, m_nid.szTip, sizeof(m_nid.szTip));
}

CTrayIcon::~CTrayIcon()
{
    SetIcon(0); // remove icon from system tray
}

//////////////////
// Set notification window. It must created already.
//
void CTrayIcon::SetNotificationWnd(CWnd* pNotifyWnd, UINT uCbMsg)
{
    // If the following assert fails, you're probably
    // calling me before you created your window. Oops.
    ASSERT(pNotifyWnd == NULL || ::IsWindow(pNotifyWnd->GetSafeHwnd()));
    m_nid.hWnd = pNotifyWnd->GetSafeHwnd();

    ASSERT(uCbMsg == 0 || uCbMsg >= WM_USER);
    m_nid.uCallbackMessage = uCbMsg;
}

//////////////////
// This is the main variant for setting the icon.
// Sets both the icon and tooltip from resource ID
// To remove the icon, call SetIcon(0)
//
BOOL CTrayIcon::SetIcon(UINT uID)
{
    HICON hicon = NULL;

    if (uID)
    {
        AfxLoadString(uID, m_nid.szTip, sizeof(m_nid.szTip));
        hicon = AfxGetApp()->LoadIcon(uID);
    }

    return SetIcon(hicon, NULL);
}

//////////////////
// Common SetIcon for all overloads.
//
BOOL CTrayIcon::SetIcon(HICON hicon, LPCSTR lpTip)
{
    UINT msg;
    m_nid.uFlags = 0;

    // Set the icon
    if (hicon)
    {
        // Add or replace icon in system tray
        msg = m_nid.hIcon ? NIM_MODIFY : NIM_ADD;
        m_nid.hIcon = hicon;
        m_nid.uFlags |= NIF_ICON;
    }
    else     // remove icon from tray
    {
        if (m_nid.hIcon == NULL)
        {
            return TRUE;    // already deleted
        }

        msg = NIM_DELETE;
    }

    // Use the tip, if any
    if (lpTip)
    {
        strncpy(m_nid.szTip, lpTip, sizeof(m_nid.szTip));
    }

    if (m_nid.szTip[0])
    {
        m_nid.uFlags |= NIF_TIP;
    }

    // Use callback if any
    if (m_nid.uCallbackMessage && m_nid.hWnd)
    {
        m_nid.uFlags |= NIF_MESSAGE;
    }

    // Do it
    BOOL bRet = Shell_NotifyIcon(msg, &m_nid);

    if (msg == NIM_DELETE || !bRet)
    {
        m_nid.hIcon = NULL;    // failed
    }

    return bRet;
}

/////////////////
// Default event handler handles right-menu and doubleclick.
// Call this function from your own notification handler.
//
LRESULT CTrayIcon::OnTrayNotification(WPARAM wID, LPARAM lEvent)
{
    if (wID != m_nid.uID ||
            (lEvent != WM_RBUTTONUP && lEvent != WM_LBUTTONDBLCLK))
    {
        return 0;
    }

    // If there's a resource menu with the same ID as the icon, use it as
    // the right-button popup menu. CTrayIcon will interprets the first
    // item in the menu as the default command for WM_LBUTTONDBLCLK
    //
    CMenu menu;

    if (!menu.LoadMenu(m_nid.uID))
    {
        return 0;
    }

    CMenu* pSubMenu = menu.GetSubMenu(0);

    if (!pSubMenu)
    {
        return 0;
    }

    if (lEvent == WM_RBUTTONUP)
    {

        // Make first menu item the default (bold font)
        ::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

        // Display the menu at the current mouse location. There's a "bug"
        // (Microsoft calls it a feature) in Windows 95 that requires calling
        // SetForegroundWindow. To find out more, search for Q135788 in MSDN.
        //
        CPoint mouse;
        GetCursorPos(&mouse);
        ::SetForegroundWindow(m_nid.hWnd);
        ::TrackPopupMenu(pSubMenu->m_hMenu, 0, mouse.x, mouse.y, 0,
                         m_nid.hWnd, NULL);

    }
    else    // double click: execute first menu item
    {
        ::SendMessage(m_nid.hWnd, WM_COMMAND, pSubMenu->GetMenuItemID(0), 0);
    }

    return 1; // handled
}
