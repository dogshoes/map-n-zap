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
 
 
  // DlgEStop.cpp : implementation file
  //
  
  /*******************************************************************************
  //Created: 02/27/97 S.R.
  //Last Revision: 02/27/97 S.R.
  //Parameters:
  //	none
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  
  #include "stdafx.h"
  #include "RobotWorld Resource.h"
  #include "Robot.h"
  //#include "RobotWorld.h"
  #include "DlgEStop.h"
  #include "RothUtil.h"

  #define ESTOP_WINDOW_SECTION "ESTOP_WINDOW_DIM"
  /////////////////////////////////////////////////////////////////////////////
  // CDlgEStop dialog
  
  
  CDlgEStop::CDlgEStop(CWnd* pParent /*=NULL*/)
  	: CDialog(CDlgEStop::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CDlgEStop)
  		// NOTE: the ClassWizard will add member initialization here
  	//}}AFX_DATA_INIT
  }
  
  
  void CDlgEStop::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgEStop)
  		// NOTE: the ClassWizard will add DDX and DDV calls here
  	//}}AFX_DATA_MAP
  	m_Robot = NULL;
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgEStop, CDialog)
  	//{{AFX_MSG_MAP(CDlgEStop)
  	ON_BN_CLICKED(IDC_ESTOP, OnEstop)
  	ON_WM_CREATE()
  	ON_WM_DESTROY()
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgEStop message handlers
  
  BEGIN_EVENTSINK_MAP(CDlgEStop, CDialog)
      //{{AFX_EVENTSINK_MAP(CDlgEStop)
  	//}}AFX_EVENTSINK_MAP
  END_EVENTSINK_MAP()
  
  BOOL CDlgEStop::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	
  	m_butEStop.AutoLoad(IDC_ESTOP, this);
  	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  	
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CDlgEStop::OnEstop() 
  {
  	m_Robot->EStop();	
  }
  
  int CDlgEStop::OnCreate(LPCREATESTRUCT lpCreateStruct) 
  {
  	if (CDialog::OnCreate(lpCreateStruct) == -1)
  		return -1;
  	
  	CPoint DesktopSize = GetDesktopSize();
  	CRect EStopRect;
  	EStopRect.top = AfxGetApp()->GetProfileInt(ESTOP_WINDOW_SECTION, "TOP", -1);
  	EStopRect.left = AfxGetApp()->GetProfileInt(ESTOP_WINDOW_SECTION, "LEFT", -1);
  //	EStopRect.bottom = AfxGetApp()->GetProfileInt(ESTOP_WINDOW_SECTION, "BOTTOM", -1);
  //	EStopRect.right = AfxGetApp()->GetProfileInt(ESTOP_WINDOW_SECTION, "RIGHT", -1);
  //	BITMAP Bitmap = GetBitmap
  	if ( (EStopRect.top != -1) && (EStopRect.left != -1) ) {
  	/*Make sure the old EStopRect is inside the desktop area*/
  		if ( (EStopRect.top < DesktopSize.y) && (EStopRect.left < DesktopSize.x) ) {
  			SetWindowPos(NULL, EStopRect.left, EStopRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
  		}
  	}
  	
  	
  	return 0;
  }
  
  void CDlgEStop::OnDestroy() 
  {
  	CDialog::OnDestroy();
  	
  	CRect EStopRect;
  	GetWindowRect(EStopRect);
  	AfxGetApp()->WriteProfileInt(ESTOP_WINDOW_SECTION, "TOP", EStopRect.top);
  	AfxGetApp()->WriteProfileInt(ESTOP_WINDOW_SECTION, "LEFT", EStopRect.left);
  	AfxGetApp()->WriteProfileInt(ESTOP_WINDOW_SECTION, "BOTTOM", EStopRect.bottom);
  	AfxGetApp()->WriteProfileInt(ESTOP_WINDOW_SECTION, "RIGHT", EStopRect.right);
  	
  }
