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
 
  // DlgRobotError.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "DlgRobotError.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgRobotError dialog
  
  
  CDlgRobotError::CDlgRobotError(CWnd* pParent /*=NULL*/)
  	: CDialog(CDlgRobotError::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CDlgRobotError)
  		// NOTE: the ClassWizard will add member initialization here
  	//}}AFX_DATA_INIT
  }
  
  
  void CDlgRobotError::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgRobotError)
  	DDX_Control(pDX, IDOK, m_butOK);
  	DDX_Control(pDX, IDC_ICO_ERROR, m_icoError);
  	DDX_Control(pDX, IDC_EDT_ERROR_TEXT, m_edtErrorText);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgRobotError, CDialog)
  	//{{AFX_MSG_MAP(CDlgRobotError)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgRobotError message handlers
  
  void CDlgRobotError::DisplayText(CString Text)
  {
  	m_TextList.AddTail(Text);
  	if (!IsWindowVisible()) {
  		ShowWindow(SW_SHOW);
  		SetErrorWindowText(Text);
  	}
  }
  
  void CDlgRobotError::OnOK() 
  {
  	m_TextList.RemoveHead();
  	if (m_TextList.IsEmpty()) ShowWindow(SW_HIDE);
  	else SetErrorWindowText(m_TextList.GetHead());
  	
  //	CDialog::OnOK();
  }
  
  void CDlgRobotError::DisplayText(UINT nID)
  {
  	CString Text;
  	Text.LoadString(nID);
  	DisplayText(Text);
  }
  
  void CDlgRobotError::SetErrorWindowText(CString Text)
  {
  	m_icoError.SetIcon(AfxGetApp()->LoadStandardIcon(IDI_EXCLAMATION));
  
  /*Calculate minimum value for right side of dialog*/
  	CRect OKRect;
  	m_butOK.GetWindowRect(&OKRect);
  	CRect IconRect;
  	m_icoError.GetWindowRect(&IconRect);
  	int MinDialogRight = IconRect.right + OKRect.Width() + 30;
  
  /*Calculate Edit window's rectangle*/
  	CRect EditWindowRect;
  	m_edtErrorText.GetWindowRect(&EditWindowRect);
  	ScreenToClient(&EditWindowRect);
  
  	CDC* pDC = GetDC();
  	CFont* OldFont = pDC->SelectObject(GetFont());
  
  	CSize TextSize = pDC->GetTextExtent(Text);
  	EditWindowRect.right = EditWindowRect.left + TextSize.cx + 10;
  	EditWindowRect.bottom = EditWindowRect.top + TextSize.cy;
  	m_edtErrorText.MoveWindow(EditWindowRect, TRUE);
  /*Calculate dialog window's rectangle*/
  	CRect DialogRect;
  	GetWindowRect(DialogRect);
  	ClientToScreen(&EditWindowRect);
  
  	const int cMargin = 10;
  	DialogRect.right = EditWindowRect.right + cMargin;
  	if (DialogRect.right < MinDialogRight) DialogRect.right = MinDialogRight;
  	MoveWindow(DialogRect, TRUE);
  
  /*Center OK button*/
  	ScreenToClient(&OKRect);
  	int temp;
  	temp = OKRect.Width();
  	OKRect.left = (DialogRect.Width() - temp)/2;
  	OKRect.right = OKRect.left + temp;
  	m_butOK.MoveWindow(OKRect, TRUE);
  	
  	m_edtErrorText.SetWindowText(Text);
  	pDC->SelectObject(OldFont);
  }
  
  CDlgRobotError::~CDlgRobotError()
  {
  
  }
