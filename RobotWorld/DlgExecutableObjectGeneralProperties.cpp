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
 
  // DlgExecutableObjectGeneralProperties.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "DlgExecutableObjectGeneralProperties.h"
  #include "ExecutableRWGraphicObject.h"
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgExecutableObjectGeneralProperties dialog
  
  
  CDlgExecutableObjectGeneralProperties::CDlgExecutableObjectGeneralProperties(CWnd* pParent /*=NULL*/)
  	: CDialog(CDlgExecutableObjectGeneralProperties::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CDlgExecutableObjectGeneralProperties)
  	m_edtName = _T("");
  	//}}AFX_DATA_INIT
  	m_NumberOfClaps = -1;
  	m_EditingObjectType = goNoObject;
  }
  
  
  void CDlgExecutableObjectGeneralProperties::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgExecutableObjectGeneralProperties)
  	DDX_Control(pDX, IDC_COM_NUMBER_OF_CLAPS, m_comNumberOfClaps);
  	DDX_Text(pDX, IDC_EDT_NAME, m_edtName);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgExecutableObjectGeneralProperties, CDialog)
  	//{{AFX_MSG_MAP(CDlgExecutableObjectGeneralProperties)
  	ON_WM_CREATE()
  	ON_CBN_SELCHANGE(IDC_COM_NUMBER_OF_CLAPS, OnSelchangeComNumberOfClaps)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgExecutableObjectGeneralProperties message handlers
  
  void CDlgExecutableObjectGeneralProperties::SetName(CString Name)
  {
  	if (GetSafeHwnd() != NULL) {
  		UpdateData(TRUE);
  		m_edtName = Name;
  		UpdateData(FALSE);
  	} else {
  		m_edtName = Name;
  	}
  }
  
  CString CDlgExecutableObjectGeneralProperties::GetName()
  {
  	return m_edtName;
  }
  
  
  void CDlgExecutableObjectGeneralProperties::SetNumberOfClaps(int Claps)
  {
  	m_NumberOfClaps = Claps;
  }
  
  
  int CDlgExecutableObjectGeneralProperties::GetNumberOfClaps()
  {
  	return m_NumberOfClaps;
  }
  
  int CDlgExecutableObjectGeneralProperties::OnCreate(LPCREATESTRUCT lpCreateStruct) 
  {
  	if (CDialog::OnCreate(lpCreateStruct) == -1)
  		return -1;
  	
  	return 0;
  }
  
  #define NumberOfClapsToListSelection(Claps) ((Claps == -1) ? 0 : (m_NumberOfClaps - cMinNumberOfClaps + 1))
  #define ClapsListSelectionToNumberOfClaps(Sel) ( (Sel == 0) ? -1 : (Sel - 1 + cMinNumberOfClaps))
  
  BOOL CDlgExecutableObjectGeneralProperties::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	
  	switch ((TRWGraphicObjectType)m_EditingObjectType) {
  	case goPosition:
  		SetWindowText("Edit Hot Point Properties");
  		GetDlgItem(IDC_STA_NAME)->SetWindowText("Hot Point Name");
  		break;
  	case goLinePath:
  		SetWindowText("Edit Line Path Properties");
  		GetDlgItem(IDC_STA_NAME)->SetWindowText("Line Path Name");
  		break;
  	default:
  		ASSERT(FALSE);
  		break;
  	}
  
  	m_comNumberOfClaps.AddString("None Selected");
  	for (int i = cMinNumberOfClaps - 1; i < cMaxNumberOfClaps; i++) {
  		CString ClapInfo;
  		ClapInfo.Format("%d", i+1);
  		m_comNumberOfClaps.AddString(ClapInfo);
  	}
  	
  	m_comNumberOfClaps.SetCurSel(NumberOfClapsToListSelection(m_NumberOfClaps));
  
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CDlgExecutableObjectGeneralProperties::SetEditingObjectType(int Type)
  {
  	m_EditingObjectType = Type;
  }
  
  void CDlgExecutableObjectGeneralProperties::OnSelchangeComNumberOfClaps() 
  {
  	m_NumberOfClaps = ClapsListSelectionToNumberOfClaps(m_comNumberOfClaps.GetCurSel());
  }
