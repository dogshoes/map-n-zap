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
   // MainFrm.h : interface of the CMainFrame class
   //
   /////////////////////////////////////////////////////////////////////////////
   
   #include "trayicon.h"
   #include "ProtocolDiagnostics.h"
   
   class CMainFrame : public CMDIFrameWnd
   {
   	DECLARE_DYNAMIC(CMainFrame)
   public:
   	CMainFrame();
   
   // Attributes
   public:
   
   // Operations
   public:
   
   // Overrides
   	// ClassWizard generated virtual function overrides
   	//{{AFX_VIRTUAL(CMainFrame)
   	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   	//}}AFX_VIRTUAL
   
   // Implementation
   public:
   	virtual ~CMainFrame();
   #ifdef _DEBUG
   	virtual void AssertValid() const;
   	virtual void Dump(CDumpContext& dc) const;
   #endif
   
      CTrayIcon m_trayIcon;
   
   protected:  // control bar embedded members
   	CStatusBar  m_wndStatusBar;
   	CToolBar    m_wndToolBar;
      BOOL m_SendState;                   // set to TRUE when sending a message
      BOOL m_RecvState;                   // set to TRUE when receiving a message
      ProtocolDiagnostics *m_oProtDialog; // protocol diagnostics
   
      enum TIMER_CONSTANTS
      {
         SEND_TIMER=1,
         RECV_TIMER=2,
      };
   
      HICON m_hSend;
      HICON m_hRecv;
      HICON m_hIdle;
      HICON m_hSendRecv;
      void UpdateIconState();
   
   // Generated message map functions
   protected:
   	//{{AFX_MSG(CMainFrame)
   	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   	afx_msg void OnTimer(UINT nIDEvent);
   	//}}AFX_MSG
      afx_msg LRESULT OnCopyData(WPARAM wParam, LPARAM lParam);
      afx_msg LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);
      afx_msg LRESULT OnSending(WPARAM uID, LPARAM lEvent);
      afx_msg LRESULT OnReceiving(WPARAM uID, LPARAM lEvent);
   	afx_msg void OnProtocolDiagnostics();
      afx_msg LRESULT OnMsgReceived(WPARAM uID, LPARAM lEvent);
      afx_msg LRESULT OnMsgTimeout(WPARAM uID, LPARAM lEvent);
   	DECLARE_MESSAGE_MAP()
   };
   
   /////////////////////////////////////////////////////////////////////////////
