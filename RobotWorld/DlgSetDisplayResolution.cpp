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

  // DlgSetDisplayResolution.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "DlgSetDisplayResolution.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgSetDisplayResolution dialog
  
  
  CDlgSetDisplayResolution::CDlgSetDisplayResolution(CWnd* pParent /*=NULL*/)
  	: CDialog(CDlgSetDisplayResolution::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CDlgSetDisplayResolution)
  	m_DisplayResolution = _T("");
  	//}}AFX_DATA_INIT
  }
  
  
  void CDlgSetDisplayResolution::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgSetDisplayResolution)
  	DDX_Control(pDX, IDC_CMB_DISPLAY_RESOLUTION, m_cmbDisplayResolution);
  	DDX_CBString(pDX, IDC_CMB_DISPLAY_RESOLUTION, m_DisplayResolution);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgSetDisplayResolution, CDialog)
  	//{{AFX_MSG_MAP(CDlgSetDisplayResolution)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgSetDisplayResolution message handlers
  
  
  BOOL CDlgSetDisplayResolution::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	
  	POSITION Current = m_Resolutions->GetHeadPosition();
  	unsigned int i = 0;
  	while (Current != NULL) {
  		CString CurrentString = m_Resolutions->GetNext(Current);
  		m_cmbDisplayResolution.InsertString(i, CurrentString);
  		i++;
  	}
  	m_cmbDisplayResolution.SetCurSel(0);
  	
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
