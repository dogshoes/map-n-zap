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
  
  // Calibrate.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "FlowZap.h"
  #include "Calibrate.h"
  #include "util.h"

  /////////////////////////////////////////////////////////////////////////////
  // CCalibrate dialog
  
  IMPLEMENT_DYNAMIC( CCalibrate, CInstruction)
  CCalibrate::CCalibrate(CWnd* pParent /*=NULL*/, int index)
  	: CInstruction(CCalibrate::IDD, pParent, index)
  {
  	//{{AFX_DATA_INIT(CCalibrate)
  	//}}AFX_DATA_INIT
  }
  
  
  void CCalibrate::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CCalibrate)
  	DDX_Control(pDX, IDC_DISABLED_IMAGE, m_DisabledImage);
  	DDX_Control(pDX, IDC_TEXT, m_Text);
  	DDX_Control(pDX, IDC_IMAGE, m_Image);
  	DDX_Control(pDX, IDC_CZONES, m_Czones);
  	DDX_Control(pDX, IDC_EBORDER, m_EtchedBorder);
  	DDX_Control(pDX, IDC_BBORDER, m_BlackBorder);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CCalibrate, CDialog)
  	//{{AFX_MSG_MAP(CCalibrate)
  	ON_WM_LBUTTONDOWN()
  	ON_WM_LBUTTONUP()
  	ON_WM_PAINT()
  	ON_CBN_SELCHANGE(IDC_CZONES, OnSelchangeCzones)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CCalibrate message handlers
  
  BOOL CCalibrate::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	
  	CRect windRect;
  	GetWindowRect (&windRect);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),windRect.Height(), TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),windRect.Height(), TRUE);
  
  	m_Czones.AddString("[CHOOSE CHECKPOINT]");
  	m_Czones.SelectString(-1, "[CHOOSE CHECKPOINT]");
  
  	CInstruction::OnInit();
  	
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CCalibrate::OnLButtonDown(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseClick (point);
  	
  	CDialog::OnLButtonDown(nFlags, point);
  }
  
  void CCalibrate::OnLButtonUp(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseButtonUp (nFlags, point);
  	
  	CDialog::OnLButtonUp(nFlags, point);
  }
  
  
  void CCalibrate::OnPaint() 
  {
  	CPaintDC dc(this); // device context for painting
  	CInstruction :: OnPaint(&dc);
  	
  	// Do not call CDialog::OnPaint() for painting messages
  }
  
  void CCalibrate::OnOK() 
  {
  	// Comment out following line to keep window from closing when enter is hit.
  	//CDialog::OnOK();
  }
  
  void CCalibrate::OnCancel()
  {
  	// Comment out following line to keep window from closing when esc is hit.
  	//CDialog::OnCancel();
  }
  
  
  void CCalibrate::MakeExpanded()
  {
  	m_Expanded = TRUE;
  	if (!m_Enabled) m_DisabledImage.ShowWindow(SW_SHOW);
  	else  m_Image.ShowWindow(SW_SHOW);
  	m_Text.ShowWindow(SW_SHOW);
  	m_Czones.ShowWindow(SW_SHOW);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),m_ExpandedHeight, TRUE);
  }
  
  
  void CCalibrate::MakeCondensed()
  {
  	if (IsSelected() ) return;
  	m_Expanded = FALSE;
  	m_Image.ShowWindow(SW_HIDE);
  	m_DisabledImage.ShowWindow(SW_HIDE);
  	m_Text.ShowWindow(SW_HIDE);
  	m_Czones.ShowWindow(SW_HIDE);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),16, TRUE);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),16, TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),16, TRUE);
  }
  
  
  void CCalibrate::ShowBorder(BOOL raised)
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
  
  
  CString CCalibrate::ToString()
  {
  	CString calibrationZone;
  	m_Czones.GetWindowText(calibrationZone);
  	return "Check In at " + calibrationZone;
  }
  
  CString CCalibrate::ToFile()
  {
  	CString calibrationZone;
  	m_Czones.GetWindowText(calibrationZone);
  	return "Calibrate at " + calibrationZone + " " + BoolToString(m_Enabled);
  }
  
  void CCalibrate::InsertCalibrationZones(CStringList * zones)
  {
  	CString oldValue;
  	m_Czones.GetWindowText(oldValue);
  	m_Czones.ResetContent();
  	CString zone;
  	m_Czones.AddString("[CHOOSE ZONE]");
  	for (int i = 0; i < zones->GetCount(); i ++) {
  		zone = zones->GetAt (zones->FindIndex(i) );	
  		m_Czones.AddString(zone);
  	}
  
  	m_Czones.SelectString (-1, oldValue);
  }
  
  
  void CCalibrate::InsertZone(CString name)
  {
  	m_Czones.AddString(name);
  }
  
  void CCalibrate::Enable(BOOL enabled)
  {
  	m_Enabled = enabled;
  	m_Text.EnableWindow(enabled);
  	m_Czones.EnableWindow(enabled);
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
  
  
  
  void CCalibrate::ChangeCalibrationZone(CString OldCaption, CString NewCaption){
  /************************************************************************************************
  * CREATED: 2/24/98 2:21 PM
  *
  * LAST MODIFIED: 2/24/98 2:01 PM
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
  	m_Czones.GetWindowText(oldValue);
  	m_Czones.AddString(NewCaption);
  	m_Czones.DeleteString( m_Czones.FindStringExact (-1, OldCaption) );
  
  
  	/*
  	If we are changing the name of the selected item, select the new name
  	before we leave
  	*/
  	if (oldValue == OldCaption) {
  		m_Czones.SelectString(-1, NewCaption);
  	}
  
  	else {
  		m_Czones.SelectString(-1, oldValue);
  	}
  
  }
  
  void CCalibrate::RemoveZone(CString OldCaption)
  {
  	int result = m_Czones.FindStringExact (-1, OldCaption);
  	if (result != CB_ERR) m_Czones.DeleteString(result);
  
  }
  
  void CCalibrate::OnSelchangeCzones() 
  {
  	CFlowZap *fc = (CFlowZap*)m_fc;
  	fc->ChangeMadeInFile();
  	
  }
  
  void CCalibrate::ConfigureFromFile(CString Caption)
  {
  	int result = m_Czones.SelectString(-1, Caption);
  	if (result == CB_ERR) {
  
  		CFlowZap *fc = (CFlowZap*)m_fc;
  		fc->AddCalibrationZone(Caption);
  		InsertZone(Caption);
  		m_Czones.SelectString(-1, Caption);
  	}
  }
  
  #ifdef ROBOT_WORLD_PROJECT
  #include "ExecutableRWGraphicObject.h"
  TTaskResult CCalibrate::Execute(CRobot * Robot, CExecutableRWGraphicObjectList * ExecutableObjectList, bool & result)
  {
  	TTaskResult TaskResult;
  	/*Do Calibrate Stuff*/
  	CExecutableRWGraphicObject* ExecutableObject = ExecutableObjectList->FindObject(GetName(), goCalibrationZone);
  	if (ExecutableObject != NULL) {
  		TaskResult = ExecutableObject->Execute(Robot, cComputerControlSpeed);
  		if (TaskResult != TR_TaskComplete) {
  			/*Handle Error Stuff*/
  		}
  	} else {
  		AfxMessageBox("That Check Point doesn't exist");
  		TaskResult = TR_Error;
  	}
  	return TaskResult;
  }
  #endif