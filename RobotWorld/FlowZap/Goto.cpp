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
  // Goto.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "FlowZap.h"
  #include "Goto.h"
  #include "Util.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  /////////////////////////////////////////////////////////////////////////////
  // CGoto dialog
  IMPLEMENT_DYNAMIC( CGoto, CInstruction)
  
  CGoto::CGoto(CWnd* pParent /*=NULL*/, int index)
  	: CInstruction(CGoto::IDD, pParent, index)
  {
  	m_fc = pParent;
  	//{{AFX_DATA_INIT(CGoto)
  	//}}AFX_DATA_INIT
  }
  
  
  void CGoto::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CGoto)
  	DDX_Control(pDX, IDC_TEXT3, m_Text3);
  	DDX_Control(pDX, IDC_TEXT2, m_Text2);
  	DDX_Control(pDX, IDC_SPEED, m_SpeedControl);
  	DDX_Control(pDX, IDC_DISABLED_IMAGE, m_DisabledImage);
  	DDX_Control(pDX, IDC_EBORDER, m_EtchedBorder);
  	DDX_Control(pDX, IDC_BBORDER, m_BlackBorder);
  	DDX_Control(pDX, IDC_TEXT1, m_Text1);
  	DDX_Control(pDX, IDC_IMAGE, m_Image);
  	DDX_Control(pDX, IDC_LABEL, m_LabelName);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CGoto, CDialog)
  	//{{AFX_MSG_MAP(CGoto)
  	ON_WM_LBUTTONDOWN()
  	ON_WM_PAINT()
  	ON_WM_LBUTTONUP()
  	ON_CBN_SELCHANGE(IDC_LABEL, OnSelchangeLabel)
  	ON_WM_HSCROLL()
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CGoto message handlers
  
  void CGoto::OnLButtonDown(UINT nFlags, CPoint point) 
  {
  	
  	CInstruction :: MouseClick (point);
  	
  	CDialog::OnLButtonDown(nFlags, point);
  }
  
  void CGoto::OnPaint() 
  {
  	CPaintDC dc(this); // device context for painting
  	
  	CInstruction::OnPaint(&dc);
  	
  	// Do not call CDialog::OnPaint() for painting messages
  }
  
  CString CGoto::ToString()
  {
  	CString location;
  	CString speed;
  	m_Text3.GetWindowText(speed);
  	m_LabelName.GetWindowText(location);
  	return "Go to " + location + " at " + speed;
  }
  
  CString CGoto::ToFile()
  {
  	CString location;
  	int speed = m_SpeedControl.GetPos();
  	m_LabelName.GetWindowText(location);
  	return "Goto "  + location + " " + IntToString(speed)  + " " + BoolToString (m_Enabled);
  }
  
  void CGoto::MakeCondensed()
  {
  	if (IsSelected() ) return;
  	m_Expanded = FALSE;
  	m_LabelName.ShowWindow(SW_HIDE);
  	m_Image.ShowWindow(SW_HIDE);
  	m_DisabledImage.ShowWindow(SW_HIDE);
  	m_Text1.ShowWindow(SW_HIDE);
  	m_Text2.ShowWindow(SW_HIDE);
  	m_Text3.ShowWindow(SW_HIDE);
  	m_SpeedControl.ShowWindow(SW_HIDE);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),16, TRUE);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),16, TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),16, TRUE);
  }
  
  void CGoto::MakeExpanded()
  {
  	m_Expanded = TRUE;
  	m_LabelName.ShowWindow(SW_SHOW);
  	if (!m_Enabled) m_DisabledImage.ShowWindow(SW_SHOW);
  	else  m_Image.ShowWindow(SW_SHOW);
  	m_Text1.ShowWindow(SW_SHOW);
  	m_Text2.ShowWindow(SW_SHOW);
  	m_Text3.ShowWindow(SW_SHOW);
  	m_SpeedControl.ShowWindow(SW_SHOW);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),m_ExpandedHeight, TRUE);
  }
  
  void CGoto::Enable(BOOL enabled)
  {
  	m_Enabled = enabled;
  	m_LabelName.EnableWindow(enabled);
  	m_Text1.EnableWindow(enabled);
  	m_Text2.EnableWindow(enabled);
  	m_Text3.EnableWindow(enabled);
  	m_SpeedControl.EnableWindow(enabled);
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
  
  BOOL CGoto::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),windRect.Height(), TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),windRect.Height(), TRUE);
  
  
  	m_LabelName.AddString("[CHOOSE TARGET]");
  	m_LabelName.SelectString(-1, "[CHOOSE TARGET]");
  
  	m_SpeedControl.SetRange(20, 100);
  	m_SpeedControl.SetPos(80);
  
  	CInstruction::OnInit();
  	
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CGoto::OnLButtonUp(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseButtonUp (nFlags, point);
  	CDialog::OnLButtonUp(nFlags, point);
  }
  
  void CGoto::OnOK() 
  {
  	// Comment out following line to keep window from closing when enter is hit.
  	//CDialog::OnOK();
  }
  
  void CGoto::OnCancel()
  {
  	// Comment out following line to keep window from closing when esc is hit.
  	//CDialog::OnCancel();
  }
  
  void CGoto::ShowBorder(BOOL raised)
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
  
  void CGoto::InsertLabelNames(CStringList *labelNames)
  {
  	CString oldValue;
  	m_LabelName.GetWindowText(oldValue);
  	m_LabelName.ResetContent();
  	CString label;
  	m_LabelName.AddString("[CHOOSE HOTSPOT]");
  	for (int i = 0; i < labelNames->GetCount(); i ++) {
  		label = labelNames->GetAt (labelNames->FindIndex(i) );	
  		m_LabelName.AddString(label);
  	}
  
  	m_LabelName.SelectString(-1, oldValue);
  }
  
  
  
  
  void CGoto::InsertLabel(CString name)
  {
  	m_LabelName.AddString(name);
  }
  
  void CGoto::ConfigureFromFile(CString Caption, int pos)
  {
  	Configure(pos);
  	int result = m_LabelName.SelectString(-1, Caption);
  	if (result == CB_ERR) {
  
  		CFlowZap *fc = (CFlowZap*)m_fc;
  		fc->AddLabel (Caption);
  		InsertLabel(Caption);
  		m_LabelName.SelectString(-1, Caption);
  	}
  }
  
  
  
  
  void CGoto::ChangeLabel(CString OldCaption, CString NewCaption)
  {
  /************************************************************************************************
  * CREATED: 2/24/98 1:53 PM
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
  *	changes the name of a label in the drop down list box.
  * METHOD:
  *
  ************************************************************************************************/
  
  	CString oldValue = " ";
  	m_LabelName.GetWindowText(oldValue);
  	m_LabelName.AddString(NewCaption);
  	m_LabelName.DeleteString( m_LabelName.FindStringExact (-1, OldCaption) );
  
  
  	/*
  	If we are changing the name of the selected item, select the new name
  	before we leave
  	*/
  	if (oldValue == OldCaption) {
  		m_LabelName.SelectString(-1, NewCaption);
  	}
  
  	else {
  		m_LabelName.SelectString(-1, oldValue);
  	}
  }
  
  
  
  
  void CGoto::RemoveLabel(CString OldCaption)
  {
  	CString currentValue;
  	m_LabelName.GetWindowText(currentValue);
  
  	int result = m_LabelName.FindStringExact (-1, OldCaption);
  	if (result != CB_ERR)m_LabelName.DeleteString(result);
  
  	if (currentValue == OldCaption) m_LabelName.SelectString (-1, "[CHOOSE TARGET]");
  }
  
  
  
  void CGoto::OnSelchangeLabel() 
  {
  	CFlowZap *fc = (CFlowZap*)m_fc;
  	fc->ChangeMadeInFile();
  	
  }
  
  #ifdef ROBOT_WORLD_PROJECT
  
  #include "ExecutableRWGraphicObject.h"
  #include "HomeBase.h"
  
  TTaskResult CGoto::Execute(CRobot * Robot, CExecutableRWGraphicObjectList* ExecutableObjectList, bool & result)
  {
  	TTaskResult TaskResult;
  	result = true;
  	/*Do Goto label stuff*/
  	CExecutableRWGraphicObject* ExecutableObject;
  	if (GetName().Left(9) == "Home Base") {
  		ExecutableObject = ExecutableObjectList->FindObject( GetName(), goCalibrationZone);
  		if (ExecutableObject == NULL) {
  			ExecutableObject = ExecutableObjectList->FindObject( GetName(), goPosition);
  		}
  	} else {
  		ExecutableObject = ExecutableObjectList->FindObject( GetName(), goPosition);
  	}
  	if (ExecutableObject != NULL) {
  //		((CFlowZap*)m_fc)->SetActiveRWGraphicObject(ExecutableObject);
  //		ExecutableObject->SetAnimate(true);
  
  		CString txtSpeed;
  		m_Text3.GetWindowText(txtSpeed);
  		short Speed = atoi(txtSpeed);
  //		Speed = Speed * Robot->GetMaxSpeed() / 100;
  		Speed = Speed * cMaxSpeed / 100;
  //		if (GetName() == "Home Base") {
  		if ( ExecutableObject->IsKindOf(RUNTIME_CLASS(CHomeBase)) ) {
  			TaskResult = ((CHomeBase*)ExecutableObject)->Dock(Robot, Speed);
  		} else {
  			TaskResult = ExecutableObject->Execute(Robot, Speed);
  		}
  		if (TaskResult != TR_TaskComplete) {
  			/*Handle Error Stuff*/
  		}
  
  //		((CFlowZap*)m_fc)->SetActiveRWGraphicObject(NULL);
  //		ExecutableObject->SetAnimate(false);
  	} else {
  		AfxMessageBox("That Hot Point doesn't exist");
  		TaskResult = TR_Error;
  	}
  	return TaskResult;
  }
  #endif
  
  void CGoto::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
  {
  //	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK) {
  //		int pos = (int)nPos;
  //		Configure (pos);	
  //	}
  	
  
  	if (nSBCode == SB_ENDSCROLL) {
  		// user has lifted up on the mouse after dragging.
  		CFlowZap *fc = (CFlowZap*)m_fc;
  		fc->ChangeMadeInFile();
  	}
  	
  	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
  	Configure(m_SpeedControl.GetPos());
  }
  
  void CGoto::Configure(int pos)
  {
  	m_SpeedControl.SetPos(pos);
  	m_Text3.SetWindowText (IntToString(pos) + "% speed");
  }
  
