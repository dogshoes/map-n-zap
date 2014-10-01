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
  // Pallette.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "flowzap.h"
  #include "Pallette.h"
  
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  #define dNUM_ICONS 11
  
  const COLORREF Background = RGB (0, 103, 223);
  
  /////////////////////////////////////////////////////////////////////////////
  // CPallette dialog
  
  
  CPallette::CPallette(CWnd* pParent /*=NULL*/)
  	: CDialog(CPallette::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CPallette)
  	//}}AFX_DATA_INIT
  	m_Parent = pParent;
  	m_PrevIcon = NULL;
  }
  
  
  void CPallette::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CPallette)
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CPallette, CDialog)
  	//{{AFX_MSG_MAP(CPallette)
  	ON_WM_MOUSEMOVE()
  	ON_WM_LBUTTONDOWN()
  	ON_WM_LBUTTONUP()
  	ON_WM_PAINT()
  	ON_WM_ACTIVATE()
  	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify )
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CPallette message handlers
  
  
  BOOL CPallette::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
  {
  	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
  	UINT nID =pNMHDR->idFrom;
     if (pTTT->uFlags & TTF_IDISHWND)    {
          // idFrom is actually the HWND of the tool
          nID = ::GetDlgCtrlID((HWND)nID);
          if(nID)        {
              pTTT->lpszText = MAKEINTRESOURCE(nID);
              pTTT->hinst = AfxGetResourceHandle();
              return(TRUE);
          }
  	}
  	return(FALSE);
  }
  
  void CPallette::OnMouseMove(UINT nFlags, CPoint point) {
  /************************************************************************************************
  * DATE CREATED: 
  *	15 Jan 98 5:06PM EST
  * LAST MODIFIED:
  *	15 Jan 98 5:06PM EST
  * PARAMETERS:
  *	point of mouse location
  * RETURN
  *	void
  ************************************************************************************************
  * DESCRIPTION
  *	if a file is open, then call Adjust Display
  * METHOD:
  *
  ************************************************************************************************/
  	CDialog::OnMouseMove(nFlags, point);
  }
  
  
  
  
  
  
  
  void CPallette::OnLButtonDown(UINT nFlags, CPoint point) {
  /************************************************************************************************
  * DATE CREATED: 
  *	15 Jan 98 5:24PM EST
  * LAST MODIFIED:
  *	20 Jan 98 4:26PM EST
  * PARAMETERS:
  *	point of mouse location
  * RETURN
  *	void
  ************************************************************************************************
  * DESCRIPTION
  *	determines where the mouse click is and sets the correct mousemode
  *   returns capture to parent window
  * METHOD:
  *
  ************************************************************************************************/
  	CDialog::OnLButtonDown(nFlags, point);
  }
  
  
  
  
  
  
  void CPallette::OnLButtonUp(UINT nFlags, CPoint point) {
  /************************************************************************************************
  * DATE CREATED: 
  *	15 Jan 98 5:24PM EST
  * LAST MODIFIED:
  *	19 Jan 98 1:07PM EST
  * PARAMETERS:
  *	point of mouse location
  * RETURN
  *	void
  ************************************************************************************************
  * DESCRIPTION
  *	sets the cursor
  *   returns capture to parent window
  * METHOD:
  *
  ************************************************************************************************/
  	((CFlowZap *)m_Parent)->m_MouseMode = mmFZDefault;
  	((CFlowZap *)m_Parent)->SetCursor();
  	ReleaseCapture();
  	
  	CDialog::OnLButtonUp(nFlags, point);
  }
  
  
  
  
  
  BOOL CPallette::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	// CG: The following block was added by the ToolTips component.
  	{
  		// Create the ToolTip control.
  		m_tooltip.Create(this);
  		m_tooltip.Activate(TRUE);
  
  		// TODO: Use one of the following forms to add controls:
  		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
  		//m_tooltip.AddTool(GetDlgItem(IDC_BKGR), "TESTING");
  		m_butHotSpot.AutoLoad(IDC_BUT_HOT_POINT, this);
  		m_butExternalOutput.AutoLoad(IDC_BUT_EXTERNAL_OUTPUT, this);
  		m_butWait.AutoLoad(IDC_BUT_WAIT, this);
  		m_butVacuum.AutoLoad(IDC_BUT_VACUUM, this);
  		m_butSubroutine.AutoLoad(IDC_BUT_SUBROUTINE, this);
  		m_butStop.AutoLoad(IDC_BUT_STOP, this);
  		m_butRepeat.AutoLoad(IDC_BUT_REPEAT, this);
  		m_butPath.AutoLoad(IDC_BUT_PATH, this);
  		m_butMotion.AutoLoad(IDC_BUT_MOTION, this);
  		m_butCalibrate.AutoLoad(IDC_BUT_CALIBRATE, this);
  		m_butBranch.AutoLoad(IDC_BUT_BRANCH, this);
  		m_butBeep.AutoLoad(IDC_BUT_BEEP, this);
  
  		m_tooltip.AddTool(GetDlgItem(IDC_BUT_HOT_POINT), "Insert Hot Point");
  		m_tooltip.AddTool(GetDlgItem(IDC_BUT_CALIBRATE), "Insert Check Point");
  		m_tooltip.AddTool(GetDlgItem(IDC_BUT_VACUUM), "Insert Vacuum Path");
  		m_tooltip.AddTool(GetDlgItem(IDC_BUT_EXTERNAL_OUTPUT), "Insert External Output");
  		m_tooltip.AddTool(GetDlgItem(IDC_BUT_PATH), "Insert Line Path");
  		m_tooltip.AddTool(GetDlgItem(IDC_BUT_MOTION), "Insert Motion");
  		m_tooltip.AddTool(GetDlgItem(IDC_BUT_WAIT), "Insert Wait");
  		m_tooltip.AddTool(GetDlgItem(IDC_BUT_BEEP), "Insert Beep");
  		m_tooltip.AddTool(GetDlgItem(IDC_BUT_BRANCH), "Insert Branch");
  		m_tooltip.AddTool(GetDlgItem(IDC_BUT_REPEAT), "Insert Repeat");
  		m_tooltip.AddTool(GetDlgItem(IDC_BUT_SUBROUTINE), "Insert Subprogram");
  		m_tooltip.AddTool(GetDlgItem(IDC_BUT_STOP), "Insert Stop");
  	}
  	EnableToolTips();
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  BOOL CPallette::PreTranslateMessage(MSG* pMsg)
  {
  	// CG: The following block was added by the ToolTips component.
  	{
  		// Let the ToolTip process this message.
  		m_tooltip.RelayEvent(pMsg);
  	}
  	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
  }
  
  void CPallette::OnOK() 
  {
  	// Comment out following line to keep window from closing when enter is hit.
  	//CDialog::OnOK();
  }
  
  void CPallette::OnCancel()
  {
  	// Comment out following line to keep window from closing when esc is hit.
  	//CDialog::OnCancel();
  }
  
  
  void CPallette::OnPaint() 
  {
  	CPaintDC dc(this); // device context for painting
  	
  	/* 
  	**  Get the rectangle for this object : r
  	*/
  	CRect r;
  	GetClientRect(&r);
  	/* 
  	**  Draw the blue background for this object
  	*/
  	// create the pen and brush
  	CPen cp (PS_SOLID, 1, Background);
  	CBrush cb;
  	cb.CreateSolidBrush(Background);
  
  	// select the pen and brush
  	CBrush *oldBrush     = dc.SelectObject (&cb);
  	CPen   *oldPen = dc.SelectObject(&cp);
  
  
  	dc.Rectangle (0,0, r.Width(), r.Height() -2);
  	
  	dc.SelectObject (oldPen);
  	dc.SelectObject (oldBrush);
  }
  
  void CPallette::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
  {
  	CDialog::OnActivate(nState, pWndOther, bMinimized);
  	
  	// TODO: Add your message handler code here
  	
  }
  
  void CPallette::OnPalletteItemClicked(UINT nFlags, CPalletteItem* PalletteItem)
  {
  	if (((CFlowZap*)m_Parent)->m_FileIsOpen) {
  		if      (PalletteItem == &m_butWait) ((CFlowZap *)m_Parent)->m_MouseMode = mmFZWait;
  		else if (PalletteItem == &m_butRepeat) ((CFlowZap *)m_Parent)->m_MouseMode = mmFZRepeat;
  		else if (PalletteItem == &m_butBeep) ((CFlowZap *)m_Parent)->m_MouseMode = mmFZBeep;
  		else if (PalletteItem == &m_butStop) ((CFlowZap *)m_Parent)->m_MouseMode = mmFZStop;
  		else if (PalletteItem == &m_butBranch) ((CFlowZap *)m_Parent)->m_MouseMode = mmFZBranch;
  		else if (PalletteItem == &m_butHotSpot) ((CFlowZap *)m_Parent)->m_MouseMode = mmFZGoto;
  		else if (PalletteItem == &m_butSubroutine) ((CFlowZap *)m_Parent)->m_MouseMode = mmFZSub;
  		else if (PalletteItem == &m_butVacuum) ((CFlowZap *)m_Parent)->m_MouseMode = mmFZVac;
  		else if (PalletteItem == &m_butCalibrate) ((CFlowZap *)m_Parent)->m_MouseMode = mmFZCal;
  		else if (PalletteItem == &m_butPath) ((CFlowZap *)m_Parent)->m_MouseMode = mmFZPath;
  		else if (PalletteItem == &m_butMotion) ((CFlowZap *)m_Parent)->m_MouseMode = mmFZMotion;
  		else if (PalletteItem == &m_butExternalOutput) ((CFlowZap *)m_Parent)->m_MouseMode = mmFZExternalOutput;
  		
  		((CFlowZap *)m_Parent)->SetCursor();
  	}
  	ReleaseCapture();
  	((CFlowZap *)m_Parent)->SetCapture();
  }
