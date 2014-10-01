
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
  // DbrRobotExternals.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "DbrRobotExternals.h"
  #include "MainFrm.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  /////////////////////////////////////////////////////////////////////////////
  // CDbrRobotExternals dialog
  
  
  CDbrRobotExternals::CDbrRobotExternals()
  	: CDialogBar()
  {
  	//{{AFX_DATA_INIT(CDbrRobotExternals)
  	m_edtExternalInput = _T("");
  	//}}AFX_DATA_INIT
  	m_NeedToSetUpToolTips = true;
  	m_ActiveHotRect = -1;
  	m_State = 0;
  	m_StateBitmapResources[0] = IDB_ROBOT_EXTERNALS_OFF;
  	m_StateBitmapResources[1] = IDB_ROBOT_EXTERNALS_ON;
  
  	m_BitmapOrigin.x = 0;
  	m_BitmapOrigin.y = 0;
  
  	m_HotPointRects[0].left = 163;
  	m_HotPointRects[0].top = 0;
  	m_HotPointRects[0].right = 207;
  	m_HotPointRects[0].bottom =  19;
  
  	m_HotPointRects[1].left = 121;
  	m_HotPointRects[1].top = 0;
  	m_HotPointRects[1].right = 161;
  	m_HotPointRects[1].bottom =  19;
  }
  
  
  void CDbrRobotExternals::DoDataExchange(CDataExchange* pDX)
  {
  	CDialogBar::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDbrRobotExternals)
  	DDX_Text(pDX, IDC_EDT_EXTERNAL_INPUT, m_edtExternalInput);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDbrRobotExternals, CDialogBar)
  	//{{AFX_MSG_MAP(CDbrRobotExternals)
  	ON_WM_PAINT()
  	ON_WM_LBUTTONDOWN()
  	ON_WM_LBUTTONUP()
  	ON_WM_MOUSEMOVE()
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDbrRobotExternals message handlers
  
  BOOL CDbrRobotExternals::Create(CWnd *pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
  {
  	BOOL result = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
  	if (result) {
  		for (int i = 0; i < cNumberOfStates; i++) {
  			if (!m_bmpState[i].LoadBitmap(m_StateBitmapResources[i])) {
  				ASSERT(FALSE);
  			}
  		}
  		if (!m_dcDisplayMemory.CreateCompatibleDC(GetDC())) {
  			ASSERT(FALSE);
  		}
  	}
  	// CG: The following block was added by the ToolTips component.
  	{
  		// Create the ToolTip control.
  		m_tooltip.Create(this);
  		m_tooltip.Activate(TRUE);
  
  		// TODO: Use one of the following forms to add controls:
  		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
  		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
  		CRect Rect;
  		m_tooltip.AddTool(this, "Turn On External Output", Rect, ID_EXTERNAL_OUTPUT_ON);
  		m_tooltip.AddTool(this, "Turn Off External Output", Rect, ID_EXTERNAL_OUTPUT_OFF);
  	}
  	
  	return result;
  }
  
  void CDbrRobotExternals::OnPaint() 
  {
  	CDialogBar::OnPaint();
  	CPaintDC dc(this); // device context for painting
  	CDC* pDC = GetDC();
  	PaintBackground(pDC);
  }
  
  
  void CDbrRobotExternals::PaintBackground(CDC *pDC)
  {
  	CRect ClientRect;
  	GetClientRect(&ClientRect);
  	CBitmap* OldBitmap = m_dcDisplayMemory.SelectObject(&m_bmpState[m_State]);
  	ASSERT(OldBitmap != NULL);
  
  	BITMAP BitmapInfo;
  	m_bmpState[m_State].GetBitmap(&BitmapInfo);
  /*Blt bitmap to center of dialog bar*/
  	POINT BitmapOrigin;
  	BitmapOrigin.x = (ClientRect.Width() - BitmapInfo.bmWidth) / 2;
  	BitmapOrigin.y = (ClientRect.Height() - BitmapInfo.bmHeight) / 2;
  	pDC->BitBlt( BitmapOrigin.x, BitmapOrigin.y, BitmapInfo.bmWidth, BitmapInfo.bmHeight, &m_dcDisplayMemory, 0, 0, SRCCOPY );
  	m_dcDisplayMemory.SelectObject(OldBitmap);
  
  /*Set up tool tips first time drawn*/
  	if (m_NeedToSetUpToolTips || (m_BitmapOrigin != BitmapOrigin) ) {
  		m_NeedToSetUpToolTips = false;
  		CRect Rect;
  		CToolInfo ToolInfo;
  		Rect.left = BitmapOrigin.x + m_HotPointRects[0].left;
  		Rect.top = BitmapOrigin.y + m_HotPointRects[0].top;
  		Rect.right = BitmapOrigin.x + m_HotPointRects[0].right;
  		Rect.bottom = BitmapOrigin.y + m_HotPointRects[0].bottom;
  		m_tooltip.GetToolInfo(ToolInfo, this, ID_EXTERNAL_OUTPUT_OFF);
  		ToolInfo.rect = Rect;
  		m_tooltip.SetToolInfo(&ToolInfo);
  
  		Rect.left = BitmapOrigin.x + m_HotPointRects[1].left;
  		Rect.top = BitmapOrigin.y + m_HotPointRects[1].top;
  		Rect.right = BitmapOrigin.x + m_HotPointRects[1].right;
  		Rect.bottom = BitmapOrigin.y + m_HotPointRects[1].bottom;
  		m_tooltip.GetToolInfo(ToolInfo, this, ID_EXTERNAL_OUTPUT_ON);
  		ToolInfo.rect = Rect;
  		m_tooltip.SetToolInfo(&ToolInfo);
  	}
  	m_BitmapOrigin = BitmapOrigin;
  
  }
  
  void CDbrRobotExternals::SetState(int State)
  {
  	if (State < 0) {
  		ASSERT(FALSE);
  		m_State = 0;
  	}
  	if (State >= cNumberOfStates) {
  		ASSERT(FALSE);
  		State = 0;
  	}
  	if (State != m_State) {
  		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	}
  	m_State = State;
  }
  
  void CDbrRobotExternals::OnLButtonDown(UINT nFlags, CPoint point) 
  {
  	int ActiveHotRect = -1;
  
  	point.x -= m_BitmapOrigin.x;
  	point.y -= m_BitmapOrigin.y;
  	for (int i = 0; (i < cNumberHotRects) && (ActiveHotRect == -1); i++) {
  		if (m_HotPointRects[i].PtInRect(point)) {
  			m_OldState = m_State;
  			ActiveHotRect = i;
  			SetCapture();
  		}
  	}
  	SetActiveHotRect(ActiveHotRect);
  	if (ActiveHotRect == -1) {
  		CDialogBar::OnLButtonDown(nFlags, point);
  	}
  }
  
  void CDbrRobotExternals::OnLButtonUp(UINT nFlags, CPoint point) 
  {
  	if (m_ActiveHotRect != -1) {
  		ReleaseCapture();
  		int ActiveHotRect = -1;
  		point.x -= m_BitmapOrigin.x;
  		point.y -= m_BitmapOrigin.y;
  		for (int i = 0; (i < cNumberHotRects) && (ActiveHotRect == -1); i++) {
  			if (m_HotPointRects[i].PtInRect(point)) {
  				ActiveHotRect = i;
  			}
  		}
  		if (ActiveHotRect == m_ActiveHotRect) {
  			OnHotRect(ActiveHotRect);
  		} else {
  			SetActiveHotRect(-1);
  			SetState(m_OldState);
  		}
  	}
  	CDialogBar::OnLButtonUp(nFlags, point);
  }
  
  void CDbrRobotExternals::SetActiveHotRect(int Active)
  {
  	if (Active >= 0) {
  		SetState(Active);
  	}
  	m_ActiveHotRect = Active;
  }
  
  
  void CDbrRobotExternals::OnHotRect(int HotRectNumber)
  {
  	m_ActiveHotRect = -1;
  	SetState(HotRectNumber);
  	if (HotRectNumber == 0) {
  		m_wndFrame->SetExternalOutput(false);
  	} else if (HotRectNumber == 1) {
  		m_wndFrame->SetExternalOutput(true);
  	}
  }
  
  void CDbrRobotExternals::OnMouseMove(UINT nFlags, CPoint point) 
  {
  	if (m_ActiveHotRect != -1) {
  		if (m_HotPointRects[m_ActiveHotRect].PtInRect(point)) {
  			SetState(m_ActiveHotRect);
  		} else {
  			SetState(m_OldState);
  		}
  	}
  	CDialogBar::OnMouseMove(nFlags, point);
  }
  
  void CDbrRobotExternals::SetFrameWnd(CMainFrame *Frame)
  {
  	m_wndFrame = Frame;
  }
  
  void CDbrRobotExternals::SetExternalOutput(bool On)
  {
  	if (On) {
  		SetState(1);
  	} else {
  		SetState(0);
  	}
  }
  
  void CDbrRobotExternals::SetExternalInput(float Volts)
  {
  	m_edtExternalInput.Format("%4.2f Volts", Volts);
  	UpdateData(FALSE);
  }
  
  BOOL CDbrRobotExternals::PreTranslateMessage(MSG* pMsg)
  {
  	// CG: The following block was added by the ToolTips component.
  	{
  		// Let the ToolTip process this message.
  		m_tooltip.RelayEvent(pMsg);
  	}
  	return CDialogBar::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
  }
