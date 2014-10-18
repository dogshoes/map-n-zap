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
 
  // DlgHomeBaseFloorType.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "DlgHomeBaseFloorType.h"
  #include "Laygo.h"

  /////////////////////////////////////////////////////////////////////////////
  // CDlgHomeBaseFloorType dialog
  
  
  CDlgHomeBaseFloorType::CDlgHomeBaseFloorType(CWnd* pParent /*=NULL*/)
  	: CDialog(CDlgHomeBaseFloorType::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CDlgHomeBaseFloorType)
  	m_radFloorType = 1;
  	//}}AFX_DATA_INIT
  	m_wndPrevious = NULL;
  }
  
  
  void CDlgHomeBaseFloorType::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgHomeBaseFloorType)
  	DDX_Radio(pDX, IDC_RAD_WOOD_FLOOR, m_radFloorType);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgHomeBaseFloorType, CDialog)
  	//{{AFX_MSG_MAP(CDlgHomeBaseFloorType)
  	ON_WM_SHOWWINDOW()
  	ON_BN_CLICKED(IDC_BUT_CREATE_HOME_BASE, OnButCreateHomeBase)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgHomeBaseFloorType message handlers
  
  void CDlgHomeBaseFloorType::OnOK() 
  {
  	ShowWindow(SW_HIDE);
  	ASSERT(m_wndPrevious != NULL);
  	m_wndPrevious->ShowWindow(SW_SHOW);
  	m_Laygo->LoadTipText("HomeBaseTip2.rtf");
  //	CDialog::OnOK();
  }
  
  void CDlgHomeBaseFloorType::OnShowWindow(BOOL bShow, UINT nStatus) 
  {
  	CDialog::OnShowWindow(bShow, nStatus);
  	
  	if (bShow) {
  	/*Enable the "create home base" button*/
  //		GetParent()->GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
  	}
  }
  
  void CDlgHomeBaseFloorType::OnButCreateHomeBase() 
  {
  	if (!m_Laygo->HomeBaseCreateFloorTypeAlreadyCalibrated()) {
  		m_Laygo->LoadTipText("HomeBaseTip4.rtf");
  		ShowWindow(SW_HIDE);
  	}
  	((CLaygo*)GetParent())->CreateHomeBase();	
  }
