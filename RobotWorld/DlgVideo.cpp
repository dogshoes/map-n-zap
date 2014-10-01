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
 
  // DlgVideo.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "DlgVideo.h"
  #include "vfw.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgVideo dialog
  
  
  CDlgVideo::CDlgVideo(CWnd* pParent /*=NULL*/)
  	: CDialog(CDlgVideo::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CDlgVideo)
  		// NOTE: the ClassWizard will add member initialization here
  	//}}AFX_DATA_INIT
  
  }
  
  
  void CDlgVideo::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgVideo)
  	// DDX_Control(pDX, IDOK, m_isok);
  	// DDX_Control(pDX, IDC_MSIAVCTRL1, m_audiovideo);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgVideo, CDialog)
  	//{{AFX_MSG_MAP(CDlgVideo)
  	ON_BN_CLICKED(IDC_VIDEOSOURCEBUTTON, OnVideosourcebutton)
  	ON_BN_CLICKED(IDC_VIDEODISPLAYBUTTON, OnVideodisplaybutton)
  	ON_BN_CLICKED(IDC_VIDEOFORMATBUTTON, OnVideoformatbutton)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgVideo message handlers
  
  BOOL CDlgVideo::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	
  	// TODO: Add extra initialization here
  	
  	// Create a video capture window here.
  	h_wndCapture = capCreateCaptureWindow("Capture", WS_CHILD | WS_VISIBLE, 0, 0, 160, 120, 
  		GetSafeHwnd(), ID_VIDEO_WINDOW);
  	if (!h_wndCapture)
  		AfxMessageBox("Oops");
  	capDriverConnect(h_wndCapture, 1);
  	// Get capture status
  	if (!capDriverGetCaps(h_wndCapture, &CapDrvCaps, sizeof(CAPDRIVERCAPS)))
  		AfxMessageBox("Oops");
  	// set preview capture rate to 66ms
  	capPreviewRate(h_wndCapture, 66);
  	// turn on preview mode
  	capPreview(h_wndCapture, TRUE);
  
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CDlgVideo::OnVideosourcebutton() 
  {
  	// TODO: Add your control notification handler code here
  	AfxMessageBox("In video source");
  
  	if (CapDrvCaps.fHasDlgVideoSource)
  		capDlgVideoSource(h_wndCapture);
  	else
  		AfxMessageBox("No video source!");
  }
  
  void CDlgVideo::OnVideodisplaybutton() 
  {
  	// TODO: Add your control notification handler code here
  
  	if (CapDrvCaps.fHasDlgVideoDisplay)
  		capDlgVideoDisplay(h_wndCapture); 
  	else
  		AfxMessageBox("No video display!");
   
  }
  
  void CDlgVideo::OnVideoformatbutton() 
  {
  	// TODO: Add your control notification handler code here
  
  
  	if (CapDrvCaps.fHasDlgVideoFormat) 
  	{
  		capDlgVideoFormat(h_wndCapture); 
  
  		// Are there new image dimensions?
  		capGetStatus(h_wndCapture, &CapStatus, sizeof (CAPSTATUS));
  		
  		// If so, notify the parent of a size change.
  		CWnd *capturewin = CWnd::FromHandle(h_wndCapture);
  		capturewin->MoveWindow(0, 0, CapStatus.uiImageWidth, CapStatus.uiImageHeight);
  		
  	} 
  	else
  		AfxMessageBox("No video format!");
  }
