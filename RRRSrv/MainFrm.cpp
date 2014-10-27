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
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RRRSrv.h"
#include "MainFrm.h"
#include "RRRConst.h"

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_COPYDATA, OnCopyData)
    ON_MESSAGE(RRR_TRAY_NOTIFICATIONS, OnTrayNotification)
    ON_MESSAGE(RRR_SENDING, OnSending)
    ON_MESSAGE(RRR_RECEIVING, OnReceiving)
    ON_COMMAND(ID_PROTOCOL_DIAGNOSTICS, OnProtocolDiagnostics)
    ON_MESSAGE(RRR_MSG_RECV, OnMsgReceived)
    ON_MESSAGE(RRR_MSG_TIMEOUT, OnMsgTimeout)
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
    : m_trayIcon(IDR_TRAYICON)
{
    m_oProtDialog = NULL;
}

CMainFrame::~CMainFrame()
{
    if (m_oProtDialog)
    {
        delete m_oProtDialog;
        m_oProtDialog = NULL;
    }

    CRRRSrvApp *app = (CRRRSrvApp *)AfxGetApp();

    if (app->m_oRRRCom)
    {
        delete app->m_oRRRCom;
        app->m_oRRRCom = NULL;
    }
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    if (!m_wndToolBar.Create(this) ||
            !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    if (!m_wndStatusBar.Create(this) ||
            !m_wndStatusBar.SetIndicators(indicators,
                                          sizeof(indicators) / sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    // TODO: Remove this if you don't want tool tips or a resizeable toolbar
    m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
                             CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

    // TODO: Delete these three lines if you don't want the toolbar to
    //  be dockable
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);

    m_SendState = FALSE;
    m_RecvState = FALSE;

    CRRRSrvApp *theApp = (CRRRSrvApp *)AfxGetApp();
    theApp->m_oRRRCom = new RRRSrvCom(m_hWnd, theApp->m_MyAddress, theApp->m_sPortName, theApp->m_BaudRate, theApp->m_EnableDTR, theApp->m_DTROnDelay, theApp->m_DTROffDelay, theApp->m_Encoding, theApp->m_FullDuplex, theApp->m_InvertDTR);

    // load up the icons
    /*Modified S.R.  I modified how the icons are loaded so we can use 16x16 bit icons.  They look better on the tray*/
    m_hSend = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SEND), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
    //	if (!m_hSend) {
    //		DWORD Error = GetLastError();
    //		m_hSend = AfxGetApp()->LoadIcon(IDI_SEND);
    //	}
    m_hRecv = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SEND), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
    //	if (!m_hRecv) {
    //		m_hRecv = AfxGetApp()->LoadIcon(IDI_RECEIVE);
    //	}
    m_hIdle = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_UPALIVE), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
    //   if (!m_hIdle) {
    //		m_hIdle = AfxGetApp()->LoadIcon(IDI_UPALIVE);
    //	}
    m_hSendRecv = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SENDRECV), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
    //	if (!m_hSendRecv) {
    //		m_hSendRecv = AfxGetApp()->LoadIcon(IDI_SENDRECV);
    //	}

    // Set up tray icon
    m_trayIcon.SetNotificationWnd(this, RRR_TRAY_NOTIFICATIONS);
    UpdateIconState();
    m_oProtDialog = new ProtocolDiagnostics(this);

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

afx_msg LRESULT CMainFrame::OnCopyData(WPARAM wParam, LPARAM lParam)
{
    COPYDATASTRUCT *cd = (COPYDATASTRUCT *)lParam;
    unsigned long result = 0;

    if (cd->cbData)
    {
        RRRMsg Msg;
        char *ptr = (char *)cd->lpData;

        for (unsigned short i = 0; i < cd->cbData; i++)
        {
            Msg.Add(*ptr);
            ++ptr;
        }

        CRRRSrvApp *app = (CRRRSrvApp *)AfxGetApp();

        if (app->m_oRRRCom)
        {
            app->m_oRRRCom->SimMessageReceived(Msg);
        }


        result = cd->cbData;
    }

    return result;
}

//////////////////
// Handle notification from tray icon: display a message.
//
afx_msg LRESULT CMainFrame::OnTrayNotification(WPARAM uID, LPARAM lEvent)
{
#ifdef JUNK

    if (m_bShowTrayNotifications)
    {
        static LPCSTR MouseMessages[] = { "WM_MOUSEMOVE",
                                          "WM_LBUTTONDOWN", "WM_LBUTTONUP", "WM_LBUTTONDBLCLK",
                                          "WM_RBUTTONDOWN", "WM_RBUTTONUP", "WM_RBUTTONDBLCLK",
                                          "WM_MBUTTONDOWN", "WM_MBUTTONUP", "WM_MBUTTONDBLCLK"
                                        };

        CString s;
        s.Format("Tray notification: ID=%d, lEvent=0x%04x %s\r\n",
                 uID, lEvent, WM_MOUSEFIRST <= lEvent && lEvent <= WM_MOUSELAST ?
                 MouseMessages[lEvent - WM_MOUSEFIRST] : "(Unknown)");

        m_wndEdit.SetSel(-1, -1);     // end of edit text
        m_wndEdit.ReplaceSel(s);      // append string..
        m_wndEdit.SendMessage(EM_SCROLLCARET); // ..and make visible
    }

    // let tray icon do default stuff
#endif
    return m_trayIcon.OnTrayNotification(uID, lEvent);
}

void CMainFrame::UpdateIconState()
{
    if (m_SendState)
    {
        if (m_RecvState)
        {
            m_trayIcon.SetIcon(m_hSendRecv, "RobotWorld Interface");
        }
        else
        {
            m_trayIcon.SetIcon(m_hSend, "RobotWorld Interface");
        }
    }
    else
    {
        if (m_RecvState)
        {
            m_trayIcon.SetIcon(m_hRecv, "RobotWorld Interface");
        }
        else
        {
            m_trayIcon.SetIcon(m_hIdle, "RobotWorld Interface");
        }
    }
}

afx_msg LRESULT CMainFrame::OnSending(WPARAM uID, LPARAM lEvent)
{
    if (!m_SendState)
    {
        m_SendState = TRUE;
        // update the display state
        UpdateIconState();
    }

    // trigger the timer to turn off the led
    KillTimer(SEND_TIMER);
    SetTimer(SEND_TIMER, 100, NULL);
    return 0;
}

afx_msg LRESULT CMainFrame::OnReceiving(WPARAM uID, LPARAM lEvent)
{
    if (!m_RecvState)
    {
        m_RecvState = TRUE;
        // update the display state
        UpdateIconState();
    }

    // trigger the timer to turn off the led
    KillTimer(RECV_TIMER);
    SetTimer(RECV_TIMER, 100, NULL);
    return 0;
}


void CMainFrame::OnTimer(UINT nIDEvent)
{
    switch (nIDEvent)
    {
    case SEND_TIMER:
        m_SendState = FALSE;
        UpdateIconState();
        break;

    case RECV_TIMER:
        m_RecvState = FALSE;
        UpdateIconState();
        break;

    default:
        CMDIFrameWnd::OnTimer(nIDEvent);
    }
}

void CMainFrame::OnProtocolDiagnostics()
{
    // show the diagnostics window
    m_oProtDialog->ShowWindow(SW_SHOW);
}

afx_msg LRESULT CMainFrame::OnMsgReceived(WPARAM uID, LPARAM lEvent)
{
    RRRMsg *Msg = (RRRMsg *)uID;
    CRRRSrvApp *SrvApp = (CRRRSrvApp *)AfxGetApp();
    SrvApp->m_oRRRCom->OnMessageReceived(*Msg, (short)lEvent);
    delete Msg;
    return 0;
}

afx_msg LRESULT CMainFrame::OnMsgTimeout(WPARAM uID, LPARAM lEvent)
{
    CRRRSrvApp *SrvApp = (CRRRSrvApp *)AfxGetApp();
    SrvApp->m_oRRRCom->ServiceReceiveTimeout();
    return 0;
}