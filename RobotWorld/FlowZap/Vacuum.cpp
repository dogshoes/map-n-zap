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
  // Vacuum.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "FlowZap.h"
  #include "Vacuum.h"
  #include "util.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif 
  
  /////////////////////////////////////////////////////////////////////////////
  // CVacuum dialog
  
  IMPLEMENT_DYNAMIC( CVacuum, CInstruction)
  CVacuum::CVacuum(CWnd* pParent /*=NULL*/, int index)
  	: CInstruction(CVacuum::IDD, pParent, index)
  {
  	//{{AFX_DATA_INIT(CVacuum)
  		// NOTE: the ClassWizard will add member initialization here
  	//}}AFX_DATA_INIT
  }
  
  
  void CVacuum::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CVacuum)
  	DDX_Control(pDX, IDC_IMAGE_DISABLED, m_DisabledImage);
  	DDX_Control(pDX, IDC_TEXT, m_Text);
  	DDX_Control(pDX, IDC_PATCHLIST, m_PatchList);
  	DDX_Control(pDX, IDC_IMAGE, m_Image);
  	DDX_Control(pDX, IDC_EBORDER, m_EtchedBorder);
  	DDX_Control(pDX, IDC_BBORDER, m_BlackBorder);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CVacuum, CDialog)
  	//{{AFX_MSG_MAP(CVacuum)
  	ON_WM_LBUTTONDOWN()
  	ON_WM_LBUTTONUP()
  	ON_WM_PAINT()
  	ON_CBN_SELCHANGE(IDC_PATCHLIST, OnSelchangePatchlist)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CVacuum message handlers
  
  void CVacuum::MakeCondensed()
  {
  	if (IsSelected() ) return;
  	m_Expanded = FALSE;
  	m_Image.ShowWindow(SW_HIDE);
  	m_DisabledImage.ShowWindow(SW_HIDE);
  	m_Text.ShowWindow(SW_HIDE);
  	m_PatchList.ShowWindow(SW_HIDE);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),16, TRUE);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),16, TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),16, TRUE);
  }
  
  void CVacuum::MakeExpanded()
  {
  	m_Expanded = TRUE;
  	if (!m_Enabled) m_DisabledImage.ShowWindow(SW_SHOW);
  	else  m_Image.ShowWindow(SW_SHOW);
  	m_Text.ShowWindow(SW_SHOW);
  	m_PatchList.ShowWindow(SW_SHOW);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),m_ExpandedHeight, TRUE);
  }
  
  void CVacuum::ShowBorder(BOOL raised)
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
  
  
  
  CString CVacuum::ToString()
  {
  	CString patchName;
  	m_PatchList.GetWindowText (patchName);
  	return "Vacuum " + patchName;
  }
  
  
  
  CString CVacuum::ToFile()
  {
  	return ToString() + " " + BoolToString (m_Enabled);
  }
  
  
  
  void CVacuum::OnLButtonDown(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseClick (point);
  	
  	CDialog::OnLButtonDown(nFlags, point);
  }
  
  void CVacuum::OnLButtonUp(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseButtonUp (nFlags, point);
  	
  	CDialog::OnLButtonUp(nFlags, point);
  }
  
  BOOL CVacuum::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	
  	CRect windRect;
  	GetWindowRect (&windRect);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),windRect.Height(), TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),windRect.Height(), TRUE);
  
  	CInstruction::OnInit();
  
  	m_PatchList.AddString("[CHOOSE VACUUM PATCH]");
  	m_PatchList.SelectString(-1, "[CHOOSE VACUUM PATCH]");
  	
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CVacuum::OnPaint() 
  {
  	CPaintDC dc(this); // device context for painting
  	CInstruction::OnPaint(&dc);	
  	
  	// Do not call CDialog::OnPaint() for painting messages
  }
  
  void CVacuum::OnOK() 
  {
  	// Comment out following line to keep window from closing when enter is hit.
  	//CDialog::OnOK();
  }
  
  void CVacuum::OnCancel()
  {
  	// Comment out following line to keep window from closing when esc is hit.
  	//CDialog::OnCancel();
  }
  
  void CVacuum::InsertVacuumPatches(CStringList * patches)
  {
  	CString oldValue;
  	m_PatchList.GetWindowText(oldValue);
  	m_PatchList.ResetContent();
  	CString patch;
  	m_PatchList.AddString("[CHOOSE VACUUM PATCH]");
  	for (int i = 0; i < patches->GetCount(); i ++) {
  		patch = patches->GetAt (patches->FindIndex(i) );	
  		m_PatchList.AddString(patch);
  	}
  	m_PatchList.SelectString(-1, oldValue);
  
  }
  
  void CVacuum::Enable(BOOL enabled)
  {
  	m_Enabled = enabled;
  	m_Text.EnableWindow(enabled);
  	m_PatchList.EnableWindow(enabled);
  
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
  
  void CVacuum::InsertPatch(CString name)
  {
  	m_PatchList.AddString(name);
  }
  
  void CVacuum::ChangeVacuumPatch(CString OldCaption, CString NewCaption){
  /************************************************************************************************
  * CREATED: 2/24/98 2:24 PM
  *
  * LAST MODIFIED: 2/24/98 2:24 PM
  *
  * PARAMETERS
  *	OldCaption		the name of the caption to change
  *	NewCaption		the name to change OldCaption to
  * RETURN
  *	void			null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	changes the name of a zone in the drop down list box.
  * METHOD:
  *
  ************************************************************************************************/
  
  	CString oldValue = " ";
  	m_PatchList.GetWindowText(oldValue);
  	m_PatchList.AddString(NewCaption);
  	m_PatchList.DeleteString( m_PatchList.FindStringExact (-1, OldCaption) );
  
  
  	/*
  	If we are changing the name of the selected item, select the new name
  	before we leave
  	*/
  	if (oldValue == OldCaption) {
  		m_PatchList.SelectString(-1, NewCaption);
  	}
  
  	else {
  		m_PatchList.SelectString(-1, oldValue);
  	}
  
  
  }
  
  void CVacuum::RemovePatch(CString OldCaption)
  {
  	int result = m_PatchList.FindStringExact (-1, OldCaption);
  	if (result != CB_ERR) m_PatchList.DeleteString(result);
  
  }
  
  
  
  void CVacuum::OnSelchangePatchlist() 
  {
  	CFlowZap *fc = (CFlowZap*)m_fc;
  	fc->ChangeMadeInFile();
  	
  }
  
  void CVacuum::ConfigureFromFile(CString Caption)
  {
  	int result = m_PatchList.SelectString(-1, Caption);
  	if (result == CB_ERR) {
  
  		CFlowZap *fc = (CFlowZap*)m_fc;
  		fc->AddVacuumPatch(Caption);
  		InsertPatch(Caption);
  		m_PatchList.SelectString(-1, Caption);
  	}
  }
  
  #ifdef ROBOT_WORLD_PROJECT
  #include "ExecutableRWGraphicObject.h"
  TTaskResult CVacuum::Execute(CRobot * Robot, CExecutableRWGraphicObjectList * ExecutableObjectList, bool & result)
  {
  	TTaskResult TaskResult;
  	/*Do Vacuuming stuff*/
  	CExecutableRWGraphicObject* ExecutableObject = ExecutableObjectList->FindObject(GetName(), goVacuumPatch);
  	if (ExecutableObject != NULL) {
  		TaskResult = ExecutableObject->Execute(Robot, cComputerControlSpeed);
  		if (TaskResult != TR_TaskComplete) {
  			/*Handle Error Stuff*/
  		}
  	} else {
  		AfxMessageBox("That Vacuum Patch doesn't exist");
  		TaskResult = TR_Error;
  	}
  	return TaskResult;
  }
  #endif