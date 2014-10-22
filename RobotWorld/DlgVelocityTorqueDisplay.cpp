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
 
  // DlgVelocityTorqueDisplay.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "DlgVelocityTorqueDisplay.h"

  /////////////////////////////////////////////////////////////////////////////
  // CDlgVelocityTorqueDisplay dialog
  
  
  CDlgVelocityTorqueDisplay::CDlgVelocityTorqueDisplay(CWnd* pParent /*=NULL*/)
  	: CDialog(CDlgVelocityTorqueDisplay::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CDlgVelocityTorqueDisplay)
  	m_edtLeftTorque = _T("");
  	m_edtLeftVelocity = _T("");
  	m_edtRightTorque = _T("");
  	m_edtRightVelocity = _T("");
  	//}}AFX_DATA_INIT
  }
  
  
  void CDlgVelocityTorqueDisplay::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgVelocityTorqueDisplay)
  	DDX_Text(pDX, IDC_LEFT_TORQUE, m_edtLeftTorque);
  	DDX_Text(pDX, IDC_LEFT_VELOCITY, m_edtLeftVelocity);
  	DDX_Text(pDX, IDC_RIGHT_TORQUE, m_edtRightTorque);
  	DDX_Text(pDX, IDC_RIGHT_VELOCITY, m_edtRightVelocity);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgVelocityTorqueDisplay, CDialog)
  	//{{AFX_MSG_MAP(CDlgVelocityTorqueDisplay)
  	ON_WM_PAINT()
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgVelocityTorqueDisplay message handlers
  
  void CDlgVelocityTorqueDisplay::SetVelocity(float Left, float Right)
  {
  	Left /= 10;
  	Right /= 10;
  	m_edtLeftVelocity.Format("%5.1f", Left);
  	m_edtRightVelocity.Format("%5.1f", Right);
  	if (m_hWnd != NULL) {
  		UpdateData(FALSE);
  	}
  }
  
  void CDlgVelocityTorqueDisplay::SetTorque(float Left, float Right)
  {
  	m_edtLeftTorque.Format("%2f", Left);
  	m_edtRightTorque.Format("%2f", Right);
  	if (m_hWnd != NULL) {
  		UpdateData(FALSE);
  	}
  }
  
  BOOL CDlgVelocityTorqueDisplay::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	
  	if (m_dcDisplayMemory.GetSafeHdc() == NULL) {
  	/*Load the background bitmap*/
  		if (!m_bmpBackground.LoadBitmap(IDB_VELOCITY_TORQUE)) {
  			AfxMessageBox("CDlgVelocityTorqueDisplay Load Background Bitmap failed");
  		}
  	/*create a back buffer display context*/
  		if (!m_dcDisplayMemory.CreateCompatibleDC(GetDC())) {
  			AfxMessageBox("CDlgVelocityTorqueDisplay CreateCompatibleDC failed");
  		}
  	}
  
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CDlgVelocityTorqueDisplay::OnPaint() 
  {
  	CPaintDC dc(this); // device context for painting
  	
  	PaintBackground(GetDC());	
  	// Do not call CDialog::OnPaint() for painting messages
  }
  
  void CDlgVelocityTorqueDisplay::PaintBackground(CDC* pDC)
  {
  	if (pDC == NULL) return;
  	if (m_dcDisplayMemory.GetSafeHdc() == NULL) return;
  	CRect ClientRect;
  	GetClientRect(&ClientRect);
  	CBitmap* OldBitmap = m_dcDisplayMemory.SelectObject(&m_bmpBackground);
  	if (OldBitmap == NULL) {
  		AfxMessageBox("CDlgVelocityTorqueDisplay SelectObject Error");
  	}
  	BITMAP BitmapInfo;
  	m_bmpBackground.GetBitmap(&BitmapInfo);
  /*Blt bitmap to center of dialog bar*/
  	int x,y;
  	x = (ClientRect.Width() - BitmapInfo.bmWidth) / 2;
  	y = (ClientRect.Height() - BitmapInfo.bmHeight) / 2;
  	pDC->BitBlt( x, y, BitmapInfo.bmWidth, BitmapInfo.bmHeight, &m_dcDisplayMemory, 0, 0, SRCCOPY );
  
  	m_dcDisplayMemory.SelectObject(OldBitmap);
  }