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
  // Sub.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "FlowZap.h"
  #include "Sub.h"
  #include "Wait.h" 
  #include "util.h"

  /////////////////////////////////////////////////////////////////////////////
  // CSub dialog
  
  IMPLEMENT_DYNAMIC( CSub, CInstruction)
  CSub::CSub(CWnd* pParent /*=NULL*/, int index)
  	: CInstruction (CWait::IDD, pParent, index)
  {
  	//{{AFX_DATA_INIT(CSub)
  	//}}AFX_DATA_INIT
  }
  
  
  void CSub::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CSub)
  	DDX_Control(pDX, IDC_TEXT, m_Text);
  	DDX_Control(pDX, IDC_IMAGE_DISABLED, m_DisabledImage);
  	DDX_Control(pDX, IDC_CHOOSEPROGRAM, m_ChooseProgram);
  	DDX_Control(pDX, IDI_IMAGE, m_Image);
  	DDX_Control(pDX, IDC_EBORDER, m_EtchedBorder);
  	DDX_Control(pDX, IDC_BBORDER, m_BlackBorder);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CSub, CDialog)
  	//{{AFX_MSG_MAP(CSub)
  	ON_WM_LBUTTONDOWN()
  	ON_WM_LBUTTONUP()
  	ON_WM_PAINT()
  	ON_BN_CLICKED(IDC_CHOOSEPROGRAM, OnButtonChooseProgram)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CSub message handlers
  
  BOOL CSub::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	
  	CRect windRect;
  	GetWindowRect (&windRect);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),windRect.Height(), TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),windRect.Height(), TRUE);
  
  	m_NameOfFile = "CHOOSE FILE";
  
  	CInstruction::OnInit();
  
  	
  	// CG: The following block was added by the ToolTips component.
  	{
  		// Create the ToolTip control.
  		m_tooltip.Create(this);
  		m_tooltip.Activate(TRUE);
  
  		// TODO: Use one of the following forms to add controls:
  		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
  		m_tooltip.AddTool(GetDlgItem(IDC_CHOOSEPROGRAM), "Click to select a sub-program to insert");
  		m_tooltip.AddTool(GetDlgItem(IDC_TEXT), "Click and drag to move");
  	}
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CSub::OnLButtonDown(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseClick (point);
  	
  	CDialog::OnLButtonDown(nFlags, point);
  }
  
  void CSub::OnLButtonUp(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseButtonUp (nFlags, point);
  	
  	CDialog::OnLButtonUp(nFlags, point);
  }
  
  void CSub::OnPaint() 
  {
  	CPaintDC dc(this); // device context for painting
  	CInstruction::OnPaint(&dc);	
  	// Do not call CDialog::OnPaint() for painting messages
  }
  
  void CSub::OnOK() 
  {
  	// Comment out following line to keep window from closing when enter is hit.
  	//CDialog::OnOK();
  }
  
  void CSub::OnCancel()
  {
  	// Comment out following line to keep window from closing when esc is hit.
  	//CDialog::OnCancel();
  }
  
  void CSub::ShowBorder(BOOL raised)
  {
  	if (raised) {
  		m_BlackBorder.ShowWindow (SW_HIDE);
  		m_EtchedBorder.ShowWindow (SW_SHOW);
  	}
  	else        {
  		m_BlackBorder.ShowWindow (SW_SHOW);
  		m_EtchedBorder.ShowWindow (SW_HIDE);
  	}
  }
  
  CString CSub::ToString()
  {
  	return "Run " + GetFileNameFromPath(m_NameOfFile);
  }
  
  void CSub::MakeExpanded()
  {
  	m_Expanded = TRUE;
  	if (!m_Enabled) m_DisabledImage.ShowWindow(SW_SHOW);
  	else  m_Image.ShowWindow(SW_SHOW);
  	m_Text.ShowWindow(SW_SHOW);
  	m_ChooseProgram.ShowWindow(SW_SHOW);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),m_ExpandedHeight, TRUE);
  }
  
  void CSub::MakeCondensed()
  {
  	if (IsSelected() ) return;
  	m_Expanded = FALSE;
  	m_Image.ShowWindow(SW_HIDE);
  	m_DisabledImage.ShowWindow(SW_HIDE);
  	m_Text.ShowWindow(SW_HIDE);
  	m_ChooseProgram.ShowWindow(SW_HIDE);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),16, TRUE);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),16, TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),16, TRUE);
  }
  
  void CSub::OnButtonChooseProgram() 
  {
  	CFileDialog *fd = new CFileDialog(TRUE, "zap", NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR , "Zap! Files (*.zap)|*.zap||", this);
  	CString DefaultPath = ((CFlowZap*)m_fc)->GetDefaultZapDirectory();
  	fd->m_ofn.lpstrInitialDir = DefaultPath;
  	int response = fd->DoModal();
  	if (response == IDOK) {
  		m_NameOfFile = fd->GetPathName();
  		CString filename = GetFileNameFromPath (m_NameOfFile);
  		m_ChooseProgram.SetWindowText( "[" + filename + "]");
  		CFlowZap *fc = (CFlowZap*)m_fc;
  		fc->ChangeMadeInFile();
  		m_tooltip.AddTool(GetDlgItem(IDC_CHOOSEPROGRAM), m_NameOfFile);
  		
  	}
  	delete fd;
  	fd = NULL;
  
  
  	
  	
  }
  
  
  void CSub::Configure(CString pathname)
  {
  	m_NameOfFile = pathname;
  	CString fn = GetFileNameFromPath(pathname);
  	m_tooltip.AddTool(GetDlgItem(IDC_CHOOSEPROGRAM), m_NameOfFile);
  	m_ChooseProgram.SetWindowText( "[" + fn + "]");
  }
  
  
  CString CSub::ToFile()
  {
  	return "Execute Program " + m_NameOfFile + " " + BoolToString (m_Enabled);
  }	
  
  
  
  void CSub::Enable(BOOL enabled)
  {
  	m_Enabled = enabled;
  	m_Text.EnableWindow(enabled);
  	m_ChooseProgram.EnableWindow(enabled);
  
  	if (m_Expanded) {
  		if (m_Enabled) {
  			m_DisabledImage.ShowWindow(SW_HIDE);
  			m_Image.ShowWindow(SW_SHOW);
  		}
  		else {
  			m_DisabledImage.ShowWindow(SW_SHOW);
  			m_Image.ShowWindow(SW_HIDE);
  		}
  	}
  
  	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
  }
  
  
  
  BOOL CSub::PreTranslateMessage(MSG* pMsg)
  {
  	// CG: The following block was added by the ToolTips component.
  	{
  		// Let the ToolTip process this message.
  		m_tooltip.RelayEvent(pMsg);
  	}
  	return CInstruction::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
  }
  
  CString CSub::GetFileNameFromPath(CString path)
  {
  	CStringList words(10);
  	Split ("\\", m_NameOfFile, words);
  	CString filename = words.GetAt (words.FindIndex(words.GetCount() - 1) ) ;
  	return filename;
  }
