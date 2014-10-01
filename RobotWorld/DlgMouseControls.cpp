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
 
  // DlgMouseControls.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "Robot.h"
  #include "DlgMouseControls.h"
  #include "RRDim.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgMouseControls dialog
  IMPLEMENT_DYNCREATE(CDlgMouseControls, CPropertyPage)
  
  CDlgMouseControls::CDlgMouseControls()
  	: CPropertyPage(CDlgMouseControls::IDD)
  {
  	//{{AFX_DATA_INIT(CDlgMouseControls)
  	m_edtKb = 0;
  	m_edtKd = 0;
  	m_edtKi = 0;
  	m_edtKp = 0;
  	m_edtKPwm = 0;
  	m_edtHandleLength = short(cRRWheelBase/cTPI);
  	m_chkFlipHandle = FALSE;
  	//}}AFX_DATA_INIT
  }
  
  
  void CDlgMouseControls::DoDataExchange(CDataExchange* pDX)
  {
  	CPropertyPage::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgMouseControls)
  	DDX_Text(pDX, IDC_EDTKB, m_edtKb);
  	DDX_Text(pDX, IDC_EDTKD, m_edtKd);
  	DDX_Text(pDX, IDC_EDTKI, m_edtKi);
  	DDX_Text(pDX, IDC_EDTKP, m_edtKp);
  	DDX_Text(pDX, IDC_EDTKPWM, m_edtKPwm);
  	DDX_Text(pDX, IDC_EDTHANDLELENGTH, m_edtHandleLength);
  	DDX_Check(pDX, IDC_CHKFLIPHANDLE, m_chkFlipHandle);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgMouseControls, CPropertyPage)
  	//{{AFX_MSG_MAP(CDlgMouseControls)
  	ON_BN_CLICKED(IDC_BUTSENDMOTOCONDEFAULTS, OnButSendMotoConDefaults)
  	ON_BN_CLICKED(IDC_CHKFLIPHANDLE, OnChkFlipHandle)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgMouseControls message handlers
  
  void CDlgMouseControls::OnButSendMotoConDefaults() 
  {
  	m_edtKp = cInitKp;
  	m_edtKi = cInitKi;
  	m_edtKd = cInitKd;
  	m_edtKb = cInitKb;
  	m_edtKPwm = cInitKpwm;
  	m_edtHandleLength = short(cRRWheelBase/cTPI);
  	UpdateData(FALSE);
  }
  
  const short cKpMax = 127;
  const short cKiMax = 127;
  const short cKdMax = 127;
  const short cKPwmMax = 127;
  const short cKbMax = 127;
  const short cHandleLengthMax = 30;
  
  void CDlgMouseControls::OnChkFlipHandle() 
  {
  //	UpdateData(TRUE);
  //	m_Robot->SetFlipHandle(m_chkFlipHandle);
  }
  
  BOOL CDlgMouseControls::OnInitDialog() 
  {
  	CPropertyPage::OnInitDialog();
  	
  	UpdateData(FALSE);
  
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CDlgMouseControls::SetMotorControlConstants(char Kp, char Ki, char Kd, char Kb, char Kpwm)
  {
  	if (Kp > cKpMax) Kp = cKpMax;
  	m_edtKp = Kp;
  	if (Ki > cKiMax) Ki = cKiMax;
  	m_edtKi = Ki;
  	if (Kd > cKdMax) Kd = cKdMax;
  	m_edtKd = Kd;
  	if (Kb > cKbMax) Kb = cKbMax;
  	m_edtKb = Kb;
  	if (Kpwm > cKPwmMax) Kpwm = cKPwmMax;
  	m_edtKPwm = Kpwm;
  	if (m_hWnd != NULL) UpdateData(FALSE);
  }
  
  void CDlgMouseControls::SetFlipHandle(bool FlipHandle)
  {
  	m_chkFlipHandle = FlipHandle;
  	if (m_hWnd != NULL) UpdateData(FALSE);
  }
  
  void CDlgMouseControls::SetHandleLength(short HandleLength)
  {
  	m_edtHandleLength = short(HandleLength / cTPI);
  	if (m_edtHandleLength > cHandleLengthMax) m_edtHandleLength = cHandleLengthMax;
  	if (m_hWnd != NULL) UpdateData(FALSE);
  }
  
  void CDlgMouseControls::GetMotorControlConstants(char& Kp, char& Ki, char& Kd, char& Kb, char& Kpwm)
  {
  	if (m_hWnd != NULL) UpdateData(TRUE);
  	if (m_edtKp > cKpMax) m_edtKp = cKpMax;
  	if (m_edtKi > cKiMax) m_edtKi = cKiMax;
  	if (m_edtKd > cKdMax) m_edtKd = cKdMax;
  	if (m_edtKb > cKbMax) m_edtKb = cKbMax;
  	if (m_edtKPwm > cKPwmMax) m_edtKPwm = cKPwmMax;
  	Kp = char(m_edtKp);
  	Ki = char(m_edtKi);
  	Kd = char(m_edtKd);
  	Kb = char(m_edtKb);
  	Kpwm = char(m_edtKPwm);
  }
  
  bool CDlgMouseControls::GetFlipHandle(void)
  {
  	if (m_hWnd != NULL) UpdateData(TRUE);
  	return m_chkFlipHandle != 0;
  }
  
  short CDlgMouseControls::GetHandleLength(void)
  {
  	if (m_hWnd != NULL) UpdateData(TRUE);
  	if (m_edtHandleLength > cHandleLengthMax) m_edtHandleLength = cHandleLengthMax;
  	return short(m_edtHandleLength * cTPI);
  }
