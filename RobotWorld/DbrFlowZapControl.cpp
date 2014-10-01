
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
  /*******************************************************************
  COPYRIGHT 1999 Probotics, Inc.
      All title and copyrights in and to the Map-N-Zap program (including
  but not limited to any images, animations, audio, music, text, and
  dynamic link libraries) incorporated into the Map-N-Zap program, the
  accompanying printed materials, and any copies of the Map-N-Zap program
  are owned by Probotics, Inc. The Map-N-Zap program is protected by
  copyright laws. Therefore, you must treat the Map-N-Zap program and it's
  includes like any other copyrighted material.
  ********************************************************************/
  // DbrFlowZapControl.cpp: implementation of the CDbrFlowZapControl class.
  //
  //////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "DbrFlowZapControl.h"
  #include "MainFrm.h"
  #include "global.h"
  
  #ifdef _DEBUG
  #undef THIS_FILE
  static char THIS_FILE[]=__FILE__;
  #define new DEBUG_NEW
  #endif
  
  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////
  
  
  /////////////////////////////////////////////////////////////////////////////
  // CDbrFlowZapControl dialog
  
  
  CDbrFlowZapControl::CDbrFlowZapControl()
  	: CDialogBar()
  {
  	m_PlayState = bsEnabled;
  	m_PauseState = bsEnabled;
  	m_StopState = bsEnabled;
  	m_ShowFZState = bsEnabled;
  	m_ButtonFocused = bfNone;
  }
  
  
  BEGIN_MESSAGE_MAP(CDbrFlowZapControl, CDialogBar)
  	//{{AFX_MSG_MAP(CDbrFlowZapControl)
  	ON_WM_PAINT()
  	ON_WM_LBUTTONDOWN()
  	ON_WM_LBUTTONUP()
  	ON_WM_MOUSEMOVE()
  	ON_WM_LBUTTONDBLCLK()
  	ON_WM_SIZE()
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDbrFlowZapControl message handlers
  
  
  BOOL CDbrFlowZapControl::Create(CWnd *pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
  {
  	BOOL result = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
  
  	if (result) {
  /*		CButton* button;
  		button = (CButton*)GetDlgItem(IDC_BUT_FZC_PLAY);
  		HICON icon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FZC_PLAY), IMAGE_ICON, 17, 17, LR_DEFAULTCOLOR);
  		if (icon == NULL) GetLastError();
  		button->SetIcon(icon);
  		button = (CButton*)GetDlgItem(IDC_BUT_FZC_PAUSE);
  		button->SetIcon((HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FZC_PAUSE), IMAGE_ICON, 17, 17, LR_DEFAULTCOLOR));
  		button = (CButton*)GetDlgItem(IDC_BUT_FZC_STOP);
  		button->SetIcon((HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FZC_STOP), IMAGE_ICON, 17, 17, LR_DEFAULTCOLOR));
  */
  		if (m_dcDisplayMemory.GetSafeHdc() == NULL) {
  		/*Load the initial robot status bitmap*/
  			if (!m_bmpPlay.LoadBitmap(IDB_FZ_PLAY_ENABLED)) {
  				AfxMessageBox("DbrFlowZapControl  Load Play Bitmap failed");
  			}
  			if (!m_bmpPause.LoadBitmap(IDB_FZ_PAUSE_ENABLED)) {
  				AfxMessageBox("DbrFlowZapControl  Load Pause Bitmap failed");
  			}
  			if (!m_bmpStop.LoadBitmap(IDB_FZ_STOP_ENABLED)) {
  				AfxMessageBox("DbrFlowZapControl  Load Stop Bitmap failed");
  			}
  			if (!m_bmpShowFZ.LoadBitmap(IDB_FZ_SHOW_FZ_ENABLED)) {
  				AfxMessageBox("DbrFlowZapControl  Load Stop Bitmap failed");
  			}
  			if (!m_bmpBackground.LoadBitmap(IDB_FZ_EXECUTE_BACKGROUND)) {
  				AfxMessageBox("DbrFlowZapControl  Load Stop Bitmap failed");
  			}
  		/*create a back buffer display context*/
  			if (!m_dcDisplayMemory.CreateCompatibleDC(GetDC())) {
  				AfxMessageBox("DbrFlowZapControl CreateCompatibleDC failed");
  			}
  
  		}
  
  		// CG: The following block was added by the ToolTips component.
  		{
  			// Create the ToolTip control.
  			m_tooltip.Create(this);
  			m_tooltip.Activate(TRUE);
  
  			// TODO: Use one of the following forms to add controls:
  			// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
  			//m_tooltip.AddTool(GetDlgItem(IDC_BKGR), "TESTING");
  
  			m_tooltip.AddTool(this, "Play Zap", m_recPlay, IDC_BUT_FZPLAY);
  			m_tooltip.AddTool(this, "Stop Zap", m_recStop, IDC_BUT_FZSTOP);
  			m_tooltip.AddTool(this, "Pause Zap", m_recPause, IDC_BUT_FZPAUSE);
  			m_tooltip.AddTool(this, "Show Zap Window", m_recShowFZ, IDC_BUT_FZSHOW);
  		}
  		
  	}
  //	DWORD HelpId = GetWindowContextHelpId();
  //	HelpId = 0x20000 + IDD_FLOWZAP_CONTROL;
  //	ASSERT(SetWindowContextHelpId(HelpId));
  	return result;
  }
  
  void CDbrFlowZapControl::SetFileList(CStringList FileList)
  {
  	POSITION Current;
  	Current = FileList.GetHeadPosition();
  	CComboBox* m_comFileList = GetComFileList();
  	m_comFileList->ResetContent();
  	while (Current != NULL) {
  		m_comFileList->AddString(FileList.GetAt(Current));
  	}
  }
  
  CComboBox* CDbrFlowZapControl::GetComFileList()
  {
  	return (CComboBox*)GetDlgItem(ID_COM_FLOWZAP_FILE);
  }
  
  /*
  CButton* CDbrFlowZapControl::GetPlay()
  {
  	return (CButton*)GetDlgItem(IDC_BUT_FZC_PLAY);
  }
  
  
  CButton* CDbrFlowZapControl::GetPause()
  {
  	return (CButton*)GetDlgItem(IDC_BUT_FZC_PAUSE);
  }
  
  CButton* CDbrFlowZapControl::GetStop()
  {
  	return (CButton*)GetDlgItem(IDC_BUT_FZC_STOP);
  }
  */
  
  void CDbrFlowZapControl::OnPaint() 
  {
  	DWORD HelpId = GetWindowContextHelpId();
  	CDialogBar::OnPaint();
  	CPaintDC dc(this); // device context for painting
  	
  	CDC* pDC = GetDC();
  	PaintBackground(pDC);
  }
  
  void CDbrFlowZapControl::PaintBackground(CDC *pDC)
  {
  	CRect ClientRect;
  	GetClientRect(&ClientRect);
  	CBitmap* OldBitmap = m_dcDisplayMemory.SelectObject(&m_bmpBackground);
  	if (OldBitmap == NULL) {
  		AfxMessageBox("DbrFlowZapControl SelectObject Error");
  	}
  	BITMAP BitmapInfo;
  	m_bmpBackground.GetBitmap(&BitmapInfo);
  /*Blt bitmap to center of dialog bar*/
  	int x,y;
  	x = (ClientRect.Width() - BitmapInfo.bmWidth) / 2;
  	y = (ClientRect.Height() - BitmapInfo.bmHeight) / 2;
  	pDC->BitBlt( x, y, BitmapInfo.bmWidth, BitmapInfo.bmHeight, &m_dcDisplayMemory, 0, 0, SRCCOPY );
  
  /*Draw play button*/
  	if ( ((CMainFrame*)GetTopLevelFrame())->GetPlay() || (m_ButtonFocused == bfPlay) ) {
  		if (m_PlayState != bsPressed) {
  			SetPlayState(bsPressed);
  		}
  	} else {
  		if (m_PlayState != bsEnabled) {
  			SetPlayState(bsEnabled);
  		}
  	}
  	m_bmpPlay.GetBitmap(&BitmapInfo);
  	m_dcDisplayMemory.SelectObject(&m_bmpPlay);
  	pDC->BitBlt( x, y, BitmapInfo.bmWidth, BitmapInfo.bmHeight, &m_dcDisplayMemory, 0, 0, SRCCOPY );
  	CRect OldRect = m_recPlay;
  	bool NeedToSetToolTipRects = false;
  	m_recPlay.left = x;
  	m_recPlay.right = x + BitmapInfo.bmWidth;
  	m_recPlay.top = y;
  	m_recPlay.bottom = y + BitmapInfo.bmHeight;
  	if (m_recPlay != OldRect) NeedToSetToolTipRects = true;
  	x += BitmapInfo.bmWidth;
  /*Draw stop button*/
  	m_bmpStop.GetBitmap(&BitmapInfo);
  	m_dcDisplayMemory.SelectObject(&m_bmpStop);
  	pDC->BitBlt( x, y, BitmapInfo.bmWidth, BitmapInfo.bmHeight, &m_dcDisplayMemory, 0, 0, SRCCOPY );
  	OldRect = m_recStop;
  	m_recStop.left = x;
  	m_recStop.right = x + BitmapInfo.bmWidth;
  	m_recStop.top = y;
  	m_recStop.bottom = y + BitmapInfo.bmHeight;
  	if (OldRect != m_recStop) NeedToSetToolTipRects = true;
  	x += BitmapInfo.bmWidth;
  /*Draw pause button*/
  	OldRect = m_recPause;
  	if ( ((CMainFrame*)GetTopLevelFrame())->GetPause() || (m_ButtonFocused == bfPause) ) {
  		if (m_PauseState != bsPressed) {
  			SetPauseState(bsPressed);
  		}
  	} else {
  		if (m_PauseState != bsEnabled) {
  			SetPauseState(bsEnabled);
  		}
  	}
  	m_bmpPause.GetBitmap(&BitmapInfo);
  	m_dcDisplayMemory.SelectObject(&m_bmpPause);
  	pDC->BitBlt( x, y, BitmapInfo.bmWidth, BitmapInfo.bmHeight, &m_dcDisplayMemory, 0, 0, SRCCOPY );
  	m_recPause.left = x;
  	m_recPause.right = x + BitmapInfo.bmWidth;
  	m_recPause.top = y;
  	m_recPause.bottom = y + BitmapInfo.bmHeight;
  	if (OldRect != m_recPause) NeedToSetToolTipRects = true;
  	x += BitmapInfo.bmWidth;
  /*Draw Show FlowZap button*/
  	m_bmpBackground.GetBitmap(&BitmapInfo);
  	x = (ClientRect.Width() + BitmapInfo.bmWidth) / 2;
  	m_bmpShowFZ.GetBitmap(&BitmapInfo);
  	x -= BitmapInfo.bmWidth;
  	m_dcDisplayMemory.SelectObject(&m_bmpShowFZ);
  	pDC->BitBlt( x, y, BitmapInfo.bmWidth, BitmapInfo.bmHeight, &m_dcDisplayMemory, 0, 0, SRCCOPY );
  	OldRect = m_recShowFZ;
  	m_recShowFZ.left = x;
  	m_recShowFZ.right = x + BitmapInfo.bmWidth;
  	m_recShowFZ.top = y;
  	m_recShowFZ.bottom = y + BitmapInfo.bmHeight;
  	if (OldRect != m_recShowFZ) NeedToSetToolTipRects = true;
  	m_dcDisplayMemory.SelectObject(OldBitmap);
  
  	if (NeedToSetToolTipRects) SetToolTipRects();
  }
  
  void CDbrFlowZapControl::OnLButtonDown(UINT nFlags, CPoint point) 
  {
  	if ( m_recPlay.PtInRect(point) ) {
  		if (m_PlayState != bsDisabled) {
  			m_ButtonFocused = bfPlay;
  			SetPlayState(bsPressed);
  			SetCapture();
  		}
  	} else if ( m_recStop.PtInRect(point) ) {
  		if (m_StopState != bsDisabled) {
  			m_ButtonFocused = bfStop;
  			SetStopState(bsPressed);
  			SetCapture();
  		}
  	} else if ( m_recPause.PtInRect(point) ) {
  		if (m_PauseState != bsDisabled) {
  			m_ButtonFocused = bfPause;
  			SetPauseState(bsPressed);
  			SetCapture();
  		}
  	} else if ( m_recShowFZ.PtInRect(point) ) {
  		if (m_ShowFZState != bsDisabled) {
  			m_ButtonFocused = bfShowFZ;
  			SetShowFZState(bsPressed);
  			SetCapture();
  		}
  	} else {
  		m_ButtonFocused = bfNone;
  		CDialogBar::OnLButtonDown(nFlags, point);
  	}
  }
  
  void CDbrFlowZapControl::OnLButtonUp(UINT nFlags, CPoint point) 
  {
  	if (m_ButtonFocused != bfNone) {
  		ReleaseCapture();
  		switch (m_ButtonFocused) {
  		case bfPlay:
  			if (m_recPlay.PtInRect(point)) {
  				OnPlay();
  			}
  			RedrawWindow(&m_recPlay, NULL, RDW_INVALIDATE);
  //			SetPlayState(bsEnabled);
  			break;
  		case bfPause:
  			if (m_recPause.PtInRect(point)) {
  				OnPause();
  			}
  			RedrawWindow(&m_recPause, NULL, RDW_INVALIDATE);
  //			SetPauseState(bsEnabled);
  			break;
  		case bfStop:
  			if (m_recStop.PtInRect(point)) {
  				OnStop();
  			}
  			SetStopState(bsEnabled);
  			break;
  		case bfShowFZ:
  			if (m_recShowFZ.PtInRect(point)) {
  				OnShowFZ();
  			}
  			SetShowFZState(bsEnabled);
  			break;
  		}
  		m_ButtonFocused = bfNone;
  	}
  	CDialogBar::OnLButtonUp(nFlags, point);
  }
  
  void CDbrFlowZapControl::OnLButtonDblClk(UINT nFlags, CPoint point) 
  {
  	if ( m_recPlay.PtInRect(point) ) {
  	} else if ( m_recStop.PtInRect(point) ) {
  	} else if ( m_recPause.PtInRect(point) ) {
  	} else if ( m_recShowFZ.PtInRect(point) ) {
  	} else {
  		CDialogBar::OnLButtonDblClk(nFlags, point);
  	}
  }
  
  void CDbrFlowZapControl::OnMouseMove(UINT nFlags, CPoint point) 
  {
  	// TODO: Add your message handler code here and/or call default
  	
  	CDialogBar::OnMouseMove(nFlags, point);
  }
  
  void CDbrFlowZapControl::OnPlay()
  {
  	((CMainFrame*)GetTopLevelFrame())->OnFzcButPlay();
  }
  
  void CDbrFlowZapControl::OnPause()
  {
  	((CMainFrame*)GetTopLevelFrame())->OnFzcButPause();
  }
  
  void CDbrFlowZapControl::OnStop()
  {
  	((CMainFrame*)GetTopLevelFrame())->OnFzcButStop();
  }
  
  void CDbrFlowZapControl::OnShowFZ(void)
  {
  	((CMainFrame*)GetTopLevelFrame())->OnFzcShowFZ();
  }
  
  void CDbrFlowZapControl::SetPlayEnable(bool Enable)
  {
  	if (Enable) {
  		SetPlayState(bsEnabled);
  	} else {
  		SetPlayState(bsDisabled);
  	}
  }
  
  void CDbrFlowZapControl::SetPauseEnable(bool Enable)
  {
  	if (Enable) {
  		SetPauseState(bsEnabled);
  	} else {
  		SetPauseState(bsDisabled);
  	}
  }
  
  void CDbrFlowZapControl::SetStopEnable(bool Enable)
  {
  	if (Enable) {
  		SetStopState(bsEnabled);
  	} else {
  		SetStopState(bsDisabled);
  	}
  }
  
  void CDbrFlowZapControl::SetShowFZEnable(bool Enable)
  {
  	if (Enable) {
  		SetShowFZState(bsEnabled);
  	} else {
  		SetShowFZState(bsDisabled);
  	}
  }
  
  void CDbrFlowZapControl::SetPlayState(TButtonState State)
  {
  	if (State != m_PlayState) {
  		m_bmpPlay.DeleteObject();
  		switch (State) {
  		case bsEnabled:
  			m_bmpPlay.LoadBitmap(IDB_FZ_PLAY_ENABLED);
  			break;
  		case bsDisabled:
  			m_bmpPlay.LoadBitmap(IDB_FZ_PLAY_DISABLED);
  			break;
  		case bsPressed:
  			m_bmpPlay.LoadBitmap(IDB_FZ_PLAY_PUSHED);
  			break;
  		}
  		RedrawWindow(&m_recPlay, NULL, RDW_INVALIDATE);
  	}
  	m_PlayState = State;
  }
  
  void CDbrFlowZapControl::SetPauseState(TButtonState State)
  {
  	if (State != m_PauseState) {
  		m_bmpPause.DeleteObject();
  		switch (State) {
  		case bsEnabled:
  			m_bmpPause.LoadBitmap(IDB_FZ_PAUSE_ENABLED);
  			break;
  		case bsDisabled:
  			m_bmpPause.LoadBitmap(IDB_FZ_PAUSE_DISABLED);
  			break;
  		case bsPressed:
  			m_bmpPause.LoadBitmap(IDB_FZ_PAUSE_PUSHED);
  			break;
  		}
  		RedrawWindow(&m_recPause, NULL, RDW_INVALIDATE);
  	}
  	m_PauseState = State;
  }
  
  void CDbrFlowZapControl::SetStopState(TButtonState State)
  {
  	if (State != m_StopState) {
  		m_bmpStop.DeleteObject();
  		switch (State) {
  		case bsEnabled:
  			m_bmpStop.LoadBitmap(IDB_FZ_STOP_ENABLED);
  			break;
  		case bsDisabled:
  			m_bmpStop.LoadBitmap(IDB_FZ_STOP_DISABLED);
  			break;
  		case bsPressed:
  			m_bmpStop.LoadBitmap(IDB_FZ_STOP_PUSHED);
  			break;
  		}
  		RedrawWindow(&m_recStop, NULL, RDW_INVALIDATE);
  	}
  	m_StopState = State;
  }
  
  void CDbrFlowZapControl::SetShowFZState(TButtonState State)
  {
  	if (State != m_ShowFZState) {
  		m_bmpShowFZ.DeleteObject();
  		switch (State) {
  		case bsEnabled:
  			m_bmpShowFZ.LoadBitmap(IDB_FZ_SHOW_FZ_ENABLED);
  			break;
  		case bsDisabled:
  			m_bmpShowFZ.LoadBitmap(IDB_FZ_SHOW_FZ_DISABLED);
  			break;
  		case bsPressed:
  			m_bmpShowFZ.LoadBitmap(IDB_FZ_SHOW_FZ_PUSHED);
  			break;
  		}
  		RedrawWindow(&m_recShowFZ, NULL, RDW_INVALIDATE);
  	}
  	m_ShowFZState = State;
  }
  
  void CDbrFlowZapControl::OnSize(UINT nType, int cx, int cy) 
  {
  	CDialogBar::OnSize(nType, cx, cy);
  	
  	CComboBox* comFileList = GetComFileList();
  	if (comFileList != NULL) {
  		BITMAP BitmapInfo;
  		if (m_bmpBackground.m_hObject != NULL) {
  			m_bmpBackground.GetBitmap(&BitmapInfo);
  		/*Find top right corner of bitmap background*/
  			int x,y;
  			x = (cx - BitmapInfo.bmWidth) / 2;
  			y = (cy - BitmapInfo.bmHeight) / 2;
  			comFileList->MoveWindow(x + 51, y, 106, BitmapInfo.bmHeight);
  		}
  	}
  	
  }
  
  BOOL CDbrFlowZapControl::PreTranslateMessage(MSG* pMsg)
  {
  	// CG: The following block was added by the ToolTips component.
  	{
  		// Let the ToolTip process this message.
  		m_tooltip.RelayEvent(pMsg);
  	}
  	return CDialogBar::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
  }
  
  void CDbrFlowZapControl::SetToolTipRects()
  {
  	CToolInfo ToolInfo;
  	m_tooltip.GetToolInfo(ToolInfo, this, IDC_BUT_FZPLAY);
  	ToolInfo.rect = m_recPlay;
  	m_tooltip.SetToolInfo(&ToolInfo);
  
  	m_tooltip.GetToolInfo(ToolInfo, this, IDC_BUT_FZPAUSE);
  	ToolInfo.rect = m_recPause;
  	m_tooltip.SetToolInfo(&ToolInfo);
  
  	m_tooltip.GetToolInfo(ToolInfo, this, IDC_BUT_FZSTOP);
  	ToolInfo.rect = m_recStop;
  	m_tooltip.SetToolInfo(&ToolInfo);
  
  	m_tooltip.GetToolInfo(ToolInfo, this, IDC_BUT_FZSHOW);
  	ToolInfo.rect = m_recShowFZ;
  	m_tooltip.SetToolInfo(&ToolInfo);
  
  	return;
  }
