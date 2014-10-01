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
 
  // DlgPropFloor.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "DlgPropFloor.h"
  #include "RRDim.h"
  #include "Robot.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgPropFloor dialog
  
  const float cMaxSlipPercent = 5.0f;
  const float cMinSlipPercent = -1.0f;
  const int cMaxScrollPos = 100;
  const char cPercentFormat[] = "%3.2f";
  
  CDlgPropFloor::CDlgPropFloor()
  	: CPropertyPage(CDlgPropFloor::IDD)
  {
  	//{{AFX_DATA_INIT(CDlgPropFloor)
  	m_edtNormalSlipPercent = _T("0");
  	m_edtThickSlipPercent = _T("0");
  	m_FloorType = -1;
  	m_edtApproximateSlipCompensation = _T("");
  	//}}AFX_DATA_INIT
  }
  
  
  void CDlgPropFloor::DoDataExchange(CDataExchange* pDX)
  {
  	CPropertyPage::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgPropFloor)
  	DDX_Control(pDX, IDC_SCR_THICK_SLIP_PERCENT, m_scrThickSlipPercent);
  	DDX_Control(pDX, IDC_SCR_NORMAL_SLIP_PERCENT, m_scrNormalSlipPercent);
  	DDX_Text(pDX, IDC_EDT_NORMAL_SLIP_PERCENT, m_edtNormalSlipPercent);
  	DDX_Text(pDX, IDC_EDT_THICK_SLIP_PERCENT, m_edtThickSlipPercent);
  	DDX_Radio(pDX, IDC_RAD_WOOD_FLOOR, m_FloorType);
  	DDX_Text(pDX, IDC_EDT_APPROX_SLIP_COMP, m_edtApproximateSlipCompensation);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgPropFloor, CPropertyPage)
  	//{{AFX_MSG_MAP(CDlgPropFloor)
  	ON_BN_CLICKED(IDC_BUT_SET_DEFAULTS, OnButSetDefaults)
  	ON_WM_HSCROLL()
  	ON_EN_CHANGE(IDC_EDT_NORMAL_SLIP_PERCENT, OnChangeEdtNormalSlipPercent)
  	ON_EN_CHANGE(IDC_EDT_THICK_SLIP_PERCENT, OnChangeEdtThickSlipPercent)
  	ON_BN_CLICKED(IDC_BUT_FIND_SLIP_COMPENSATION, OnButFindSlipCompensation)
  	ON_BN_CLICKED(IDC_ESTOP, OnEStop)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgPropFloor message handlers
  
  void CDlgPropFloor::OnButSetDefaults() 
  {
  	SetNormalSlipPercent(m_Robot->SlipPercent(float(m_Robot->GetThinCarpetDefaultHeadingCorrectionFactor())));
  	SetThickSlipPercent(m_Robot->SlipPercent(float(m_Robot->GetThickCarpetDefaultHeadingCorrectionFactor())));
  }
  
  void CDlgPropFloor::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
  {
  	const int cLineInc = 10;
  	const int cPageInc = 10;
  //	static UINT OldPos = 0;
  	int Min, Max;
  
  	switch (nSBCode) {
  	case SB_LEFT:
  		pScrollBar->GetScrollRange(&Min, &Max);
  		nPos = Min;
  		break;
  	case SB_ENDSCROLL:
  		nPos = pScrollBar->GetScrollPos();
  		break;
  	case SB_LINELEFT:
  		nPos = pScrollBar->GetScrollPos();
  		if (nPos <= cLineInc) nPos = 0;
  		else nPos -= cLineInc;
  		pScrollBar->GetScrollRange(&Min, &Max);
  		if (nPos < (UINT)Min) nPos = Min;
  		break;
  	case SB_LINERIGHT:
  		nPos = pScrollBar->GetScrollPos();
  		nPos += cLineInc;
  		pScrollBar->GetScrollRange(&Min, &Max);
  		if (nPos > (UINT)Max) nPos = Max;
  		break;
  	case SB_PAGELEFT:
  		nPos = pScrollBar->GetScrollPos();
  		nPos -= cPageInc;
  		pScrollBar->GetScrollRange(&Min, &Max);
  		if (nPos < (UINT)Min) nPos = Min;
  		break;
  	case SB_PAGERIGHT:
  		nPos = pScrollBar->GetScrollPos();
  		nPos += cPageInc;
  		pScrollBar->GetScrollRange(&Min, &Max);
  		if (nPos > (UINT)Max) nPos = Max;
  		break;
  	case SB_RIGHT:
  		pScrollBar->GetScrollRange(&Min, &Max);
  		nPos = Max;
  		break;
  	case SB_THUMBPOSITION:
  		break;
  	case SB_THUMBTRACK:
  		break;
  	}
  	pScrollBar->SetScrollPos(nPos, TRUE);
  
  	if (pScrollBar->GetSafeHwnd() == m_scrNormalSlipPercent.GetSafeHwnd()) {
  		m_edtNormalSlipPercent.Format(cPercentFormat,ScrollPosToSlipPercent(nPos));
  		UpdateData(FALSE);
  	} else if (pScrollBar->GetSafeHwnd() == m_scrThickSlipPercent.GetSafeHwnd()) {
  		m_edtThickSlipPercent.Format(cPercentFormat, ScrollPosToSlipPercent(nPos));
  		UpdateData(FALSE);
  	}
  	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
  }
  
  float CDlgPropFloor::ScrollPosToSlipPercent(int Pos)
  {
  	return Pos * (cMaxSlipPercent - cMinSlipPercent) / cMaxScrollPos + cMinSlipPercent;
  //	return Pos * cMaxSlipPercent / cMaxScrollPos;
  }
  
  int CDlgPropFloor::SlipPercentToScrollPos(float Slip)
  {
  	return int((Slip - cMinSlipPercent) * cMaxScrollPos / (cMaxSlipPercent - cMinSlipPercent));
  // 	return int(Slip * cMaxScrollPos / cMaxSlipPercent);
  }
  
  BOOL CDlgPropFloor::OnInitDialog() 
  {
  	CPropertyPage::OnInitDialog();
  
  	m_butEStop.AutoLoad(IDC_ESTOP, this);
  	
  	m_scrNormalSlipPercent.SetScrollRange(0, cMaxScrollPos, FALSE);
  	float Slip = (float)atof(m_edtNormalSlipPercent);
  	if (Slip > cMaxSlipPercent) {
  		Slip = cMaxSlipPercent;
  	}
  	m_scrNormalSlipPercent.SetScrollPos(SlipPercentToScrollPos(Slip));
  
  	m_scrThickSlipPercent.SetScrollRange(0, cMaxScrollPos, FALSE);
  	Slip = (float)atof(m_edtThickSlipPercent);
  	if (Slip > cMaxSlipPercent) {
  		Slip = cMaxSlipPercent;
  	}
  	m_scrThickSlipPercent.SetScrollPos(SlipPercentToScrollPos(Slip));
  	
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CDlgPropFloor::OnChangeEdtNormalSlipPercent() 
  {
  	UpdateData(TRUE);
  	float Slip = (float)atof(m_edtNormalSlipPercent);
  	if (Slip > cMaxSlipPercent) {
  		Slip = cMaxSlipPercent;
  		m_edtNormalSlipPercent.Format(cPercentFormat,Slip);
  	}
  	m_scrNormalSlipPercent.SetScrollPos(SlipPercentToScrollPos(Slip));
  }
  
  void CDlgPropFloor::OnChangeEdtThickSlipPercent() 
  {
  	UpdateData(TRUE);
  	float Slip = (float)atof(m_edtThickSlipPercent);
  	if (Slip > cMaxSlipPercent) {
  		Slip = cMaxSlipPercent;
  		m_edtThickSlipPercent.Format(cPercentFormat,Slip);
  	}
  	m_scrThickSlipPercent.SetScrollPos(SlipPercentToScrollPos(Slip));
  }
  
  
  void CDlgPropFloor::SetNormalSlipPercent(float Slip)
  {
  	if (Slip < cMinSlipPercent) Slip = cMinSlipPercent;
  	else if (Slip > cMaxSlipPercent) Slip = cMaxSlipPercent;
  	m_edtNormalSlipPercent.Format(cPercentFormat,Slip);
  	if (m_scrNormalSlipPercent.m_hWnd != NULL) {
  		m_scrNormalSlipPercent.SetScrollPos(SlipPercentToScrollPos(Slip));
  	}
  	if (m_hWnd != NULL) {
  		UpdateData(FALSE);
  	}
  }
  
  void CDlgPropFloor::SetThickSlipPercent(float Slip)
  {
  	if (Slip < 0) Slip = 0;
  	else if (Slip > cMaxSlipPercent) Slip = cMaxSlipPercent;
  	m_edtThickSlipPercent.Format(cPercentFormat,Slip);
  	if (m_scrThickSlipPercent.m_hWnd != NULL) {
  		m_scrThickSlipPercent.SetScrollPos(SlipPercentToScrollPos(Slip));
  	}
  	if (m_hWnd != NULL) {
  		UpdateData(FALSE);
  	}
  }
  
  float CDlgPropFloor::GetNormalSlipPercent()
  {
  	if (m_hWnd != NULL) {
  		UpdateData(TRUE);
  	}
  	return (float)atof(m_edtNormalSlipPercent);
  }
  
  float CDlgPropFloor::GetThickSlipPercent()
  {
  	if (m_hWnd != NULL) {
  		UpdateData(TRUE);
  	}
  	return (float)atof(m_edtThickSlipPercent);
  }
  
  void CDlgPropFloor::SetFloorType(int Floor)
  {
  	m_FloorType = Floor;
  	if (m_hWnd != NULL) {
  		UpdateData(FALSE);
  	}
  }
  
  int CDlgPropFloor::GetFloorType()
  {
  	if (m_hWnd != NULL) {
  		UpdateData(TRUE);
  	}
  	return m_FloorType;
  }
  
  void CDlgPropFloor::SetRobot(CRobot *Robot)
  {
  #ifdef _DEBUG
  	ASSERT(Robot != NULL);
  #endif
  	m_Robot = Robot;
  }
  
  void CDlgPropFloor::OnButFindSlipCompensation() 
  {
  	m_butEStop.ShowWindow(SW_SHOW);
  	GetDlgItem(IDC_BUT_FIND_SLIP_COMPENSATION)->ShowWindow(SW_HIDE);
  	GetDlgItem(IDC_EDT_APPROX_SLIP_COMP)->ShowWindow(SW_HIDE);
  	int HeadingCorrectionFactor;
  	UpdateData(TRUE);
  	m_edtApproximateSlipCompensation = "";
  	UpdateData(FALSE);
  	TTaskResult TaskResult = m_Robot->CalibrateFloorSurface(&HeadingCorrectionFactor, false);
  	if (TaskResult == TR_TaskComplete) {
  		m_edtApproximateSlipCompensation.Format("%f", m_Robot->SlipPercent((float)HeadingCorrectionFactor));
  		UpdateData(FALSE);
  	} else {
  		m_edtApproximateSlipCompensation = "Error";
  	}
  	m_butEStop.ShowWindow(SW_HIDE);
  	GetDlgItem(IDC_BUT_FIND_SLIP_COMPENSATION)->ShowWindow(SW_SHOW);
  	GetDlgItem(IDC_EDT_APPROX_SLIP_COMP)->ShowWindow(SW_SHOW);
  }
  
  void CDlgPropFloor::OnEStop() 
  {
  	ASSERT(m_Robot != NULL);
  	m_Robot->EStop();
  }
