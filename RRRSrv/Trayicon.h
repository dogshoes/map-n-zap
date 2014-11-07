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
// Module:     Trayicon.cpp
// Abstract:   CTrayIcon Copyright 1996 Microsoft Systems Journal.
//             If this code works, it was written by Paul DiLascia.
//             If not, I don't know who wrote it.
// Author:     J.C.H
// Copyright 1997 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// CTrayIcon Copyright 1996 Microsoft Systems Journal.
//
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.

#pragma once

////////////////
// CTrayIcon manages an icon in the Windows 95 system tray.
//
class CTrayIcon : public CCmdTarget
{
    protected:
        DECLARE_DYNAMIC(CTrayIcon)
        NOTIFYICONDATA m_nid;			// struct for Shell_NotifyIcon args

    public:
        CTrayIcon(UINT uID);
        ~CTrayIcon();

        // Call this to receive tray notifications
        void SetNotificationWnd(CWnd* pNotifyWnd, UINT uCbMsg);

        // SetIcon functions. To remove icon, call SetIcon(0)
        //
        BOOL SetIcon(UINT uID); // main variant you want to use
        BOOL SetIcon(HICON hicon, LPCSTR lpTip);
        BOOL SetIcon(LPCTSTR lpResName, LPCSTR lpTip)
        {
            return SetIcon(lpResName ?
                           AfxGetApp()->LoadIcon(lpResName) : NULL, lpTip);
        }
        BOOL SetStandardIcon(LPCTSTR lpszIconName, LPCSTR lpTip)
        {
            return SetIcon(::LoadIcon(NULL, lpszIconName), lpTip);
        }

        virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);
};