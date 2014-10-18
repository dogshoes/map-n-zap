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
  // Motion.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "flowzap.h"
  #include "FlowZapResource.h"
  #include "Motion.h"
  #include "util.h"

  /////////////////////////////////////////////////////////////////////////////
  // CMotion dialog
  
  IMPLEMENT_DYNAMIC( CMotion, CInstruction)
  CMotion::CMotion(CWnd* pParent /*=NULL*/, int index)
  	: CInstruction(CMotion::IDD, pParent, index)
  {
  	//{{AFX_DATA_INIT(CMotion)
  	//}}AFX_DATA_INIT
  }
  
  
  void CMotion::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CMotion)
  	DDX_Control(pDX, IDC_TEXT_ROTATE, m_TextRotate);
  	DDX_Control(pDX, IDC_TEXT_OCLOCK, m_TextOClock);
  	DDX_Control(pDX, IDC_TEXT_GO, m_TextGo);
  	DDX_Control(pDX, IDC_DIRECTION, m_Direction);
  	DDX_Control(pDX, IDC_UNITS, m_Units);
  	DDX_Control(pDX, IDC_ROTATE, m_Rotate);
  	DDX_Control(pDX, IDC_MOTION, m_Motion);
  	DDX_Control(pDX, IDC_IMAGE, m_Image);
  	DDX_Control(pDX, IDC_IMAGE_DISABLED, m_DisabledImage);
  	DDX_Control(pDX, IDC_EBORDER, m_EtchedBorder);
  	DDX_Control(pDX, IDC_DISTANCE, m_Distance);
  	DDX_Control(pDX, IDC_DEGREES, m_Degrees);
  	DDX_Control(pDX, IDC_BBORDER, m_BlackBorder);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CMotion, CDialog)
  	//{{AFX_MSG_MAP(CMotion)
  	ON_WM_PAINT()
  	ON_WM_LBUTTONDOWN()
  	ON_WM_LBUTTONUP()
  	ON_BN_CLICKED(IDC_MOTION, OnClickMotion)
  	ON_BN_CLICKED(IDC_ROTATE, OnClickRotate)
  	ON_CBN_SELCHANGE(IDC_DEGREES, OnSelchangeDegrees)
  	ON_CBN_SELCHANGE(IDC_DIRECTION, OnSelchangeDirection)
  	ON_EN_CHANGE(IDC_DISTANCE, OnChangeDistance)
  	ON_CBN_SELCHANGE(IDC_UNITS, OnSelchangeUnits)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CMotion message handlers
  
  void CMotion::Enable(BOOL enabled)
  {
  	m_Enabled = enabled;
  
  	//enable or disable everything first
  	m_Degrees.EnableWindow(enabled);
  	m_Distance.EnableWindow(enabled);
  	m_Motion.EnableWindow(enabled);
  	m_Rotate.EnableWindow(enabled);
  	m_Units.EnableWindow(enabled);
  	m_TextRotate.EnableWindow(enabled);
  	m_TextGo.EnableWindow(enabled);
  	m_TextOClock.EnableWindow(enabled);
  
  	m_Direction.EnableWindow(enabled);
  		
  	if (!m_Motion.GetCheck() )EnableMotionControls(FALSE);
  	if (!m_Rotate.GetCheck() )EnableRotationControls(FALSE);
  
  
  
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
  
  void CMotion::MakeCondensed()
  {
  	if (IsSelected() ) return;
  	m_Expanded = FALSE;
  	m_Degrees.ShowWindow(SW_HIDE);
  	m_Distance.ShowWindow(SW_HIDE);
  	m_Units.ShowWindow(SW_HIDE);
  	m_Image.ShowWindow(SW_HIDE);
  	m_DisabledImage.ShowWindow(SW_HIDE);
  	m_TextRotate.ShowWindow(SW_HIDE);
  	m_TextGo.ShowWindow(SW_HIDE);m_Direction.ShowWindow(SW_HIDE);
  	m_TextOClock.ShowWindow(SW_HIDE);m_Motion.ShowWindow(SW_HIDE);
  	m_Rotate.ShowWindow(SW_HIDE);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),16, TRUE);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),16, TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),16, TRUE);
  }
  
  void CMotion::MakeExpanded()
  {
  	m_Expanded = TRUE;
  	m_Degrees.ShowWindow(SW_SHOW);
  	m_Distance.ShowWindow(SW_SHOW);
  	m_Units.ShowWindow(SW_SHOW);
  	if (!m_Enabled) m_DisabledImage.ShowWindow(SW_SHOW);
  	else  m_Image.ShowWindow(SW_SHOW);
  	m_TextRotate.ShowWindow(SW_SHOW);
  	m_TextGo.ShowWindow(SW_SHOW);m_Direction.ShowWindow(SW_SHOW);
  	m_TextOClock.ShowWindow(SW_SHOW);m_Motion.ShowWindow(SW_SHOW);
  	m_Rotate.ShowWindow(SW_SHOW);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),m_ExpandedHeight, TRUE);
  }
  
  void CMotion::ShowBorder(BOOL raised)
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
  
  CString CMotion::ToString()
  {
  	int sum = 0;
  	CString conjunction, first, second, angle, distance, units, direction, go;
  	
  
  	if (m_Rotate.GetCheck() ) {
  		m_Degrees.GetWindowText(angle);
  		first = "Rotate to " + angle + " o'clock";
  		
  		sum++;
  	}
  
  	else first = "";
  	
  
  	if (m_Motion.GetCheck() ) {
  		m_Distance.GetWindowText(distance);
  		m_Direction.GetWindowText (direction);
  		if (m_Units.GetCurSel() == 0) distance += '\"';
  		else distance += "\'";
  		if (sum == 1) go = "Go ";
  		else          go = "Go ";
  		second = go + direction + " " + distance;
  		sum++;
  	}
  
  	else second = "";
  
  	if (sum == 0)		{
  		conjunction = "No motion selected";
  	}
  	else if (sum == 1)	conjunction = "";
  	else				conjunction = "then"; 
  
  	return first + " " + conjunction + " " + second;
  }
  
  
  
  
  CString CMotion::ToFile()
  {
  	BOOL writeRotate, writeMove;
  	CString rotate = "", moveDistance = "", moveDirection = "", moveUnits = "";
  	writeRotate = m_Rotate.GetCheck();
  	writeMove = m_Motion.GetCheck();
  
  	m_Degrees.GetWindowText(rotate); // this is the o'clock
  	m_Direction.GetWindowText(moveDirection); // this is forwards/backwards
  	m_Distance.GetWindowText(moveDistance); // this is the distance to move
  	m_Units.GetWindowText(moveUnits); // this is feet/inches
  
  	return "Motion: " + BoolToString (writeRotate) + " " + BoolToString(writeMove) + " " + 
  		   rotate + " " + moveDirection + " " + moveDistance + " " + moveUnits + " " + BoolToString (m_Enabled);;
  }
  
  
  BOOL CMotion::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),windRect.Height(), TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),windRect.Height(), TRUE);
  	
  	CInstruction::OnInit();
  	
  	m_Motion.SetCheck(1); // initialize with checkbox checked
  	m_Rotate.SetCheck(1); // initialize with checkbox checked
  
  
  	for (int i = 1; i <= 12; i++) m_Degrees.AddString (IntToString(i) );
  
  	m_Degrees.SelectString (-1, "12");
  
  	m_Direction.AddString ("forward");
  	m_Direction.AddString ("backward");
  	m_Direction.SelectString(-1, "forward");
  
  	m_Units.AddString("inches");
  	m_Units.AddString("feet");
  	m_Units.SelectString (-1, "inches");
  
  	m_Distance.SetWindowText ("6");
  	m_Distance.LimitText(2);
  	
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CMotion::OnPaint() 
  {
  	CPaintDC dc(this); // device context for painting
  	
  	CInstruction::OnPaint(&dc);
  	
  	// Do not call CDialog::OnPaint() for painting messages
  }
  
  void CMotion::OnOK() 
  {
  	// Comment out following line to keep window from closing when enter is hit.
  	//CDialog::OnOK();
  }
  
  void CMotion::OnCancel()
  {
  	// Comment out following line to keep window from closing when esc is hit.
  	//CDialog::OnCancel();
  }
  
  void CMotion::OnLButtonDown(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseClick (point);
  	
  	CDialog::OnLButtonDown(nFlags, point);
  }
  
  void CMotion::OnLButtonUp(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseButtonUp (nFlags, point);
  	
  	CDialog::OnLButtonUp(nFlags, point);
  }
  
  void CMotion::OnClickMotion() {
  
  	// indicate that a change has been made in the file
  	CFlowZap *fc = (CFlowZap*)m_fc;
  	fc->ChangeMadeInFile();
  
  
  	if (m_Motion.GetCheck() )	EnableMotionControls (TRUE);
  	else						EnableMotionControls (FALSE);
  	
  }
  
  void CMotion::OnClickRotate() {
  
  	// indicate that a change has been made in the file
  	CFlowZap *fc = (CFlowZap*)m_fc;
  	fc->ChangeMadeInFile();
  
  	if (m_Rotate.GetCheck() )	EnableRotationControls (TRUE);
  	else						EnableRotationControls (FALSE);
  		
  	
  }
  
  void CMotion::ConfigureFromFile(BOOL rotate, BOOL motion, CString degrees, CString direction, CString distance, CString units)
  {
  	m_Rotate.SetCheck (rotate); // true or false
  	m_Motion.SetCheck (motion);
  
  	m_Degrees.SelectString (-1, degrees);
  	m_Direction.SelectString(-1, direction);
  	m_Distance.SetWindowText(distance);
  	m_Units.SelectString (-1, units);
  
  
  	EnableMotionControls(motion);
  	EnableRotationControls (rotate);
  	
  
  }
  
  
  
  #ifdef ROBOT_WORLD_PROJECT
  TTaskResult CMotion::Execute(CRobot * Robot, CExecutableRWGraphicObjectList * ExecutableObjectList, bool & result)
  {
  	TTaskResult TaskResult;
  	
  	TRobotInfo RobotInfo = Robot->GetRobotInfo();
  	
  	int sum = 0;
  	CString conjunction, first, second, angle, distance, units, direction, go;
  	
  	double RotationAngle = GetRotationAngle();
  	if (m_Rotate.GetCheck() ) {
  		Robot->AddHeadingToInstructions(RotationAngle, NULL, cModHeadingZone);
  	} else {
  		RotationAngle = Robot->GetHeading();
  	}
  	if (m_Motion.GetCheck() ) {
  		robPOINT GoTo;
  		double Distance = GetTravelDistance();
  		if (Distance < 0) Distance -= Robot->GetHandleLength();
  		else Distance += Robot->GetHandleLength();
  
  		GoTo = Robot->m_Position;
  		GoTo.x += Distance * cos(RotationAngle);
  		GoTo.y += Distance * sin(RotationAngle);
  		Robot->AddPositionVelocityToInstructions(GoTo, NULL, cModPositionZone, cComputerControlSpeed, TRUE, 0);
  	}
  
  	TaskResult = Robot->FollowInstructions(TRUE, FALSE, IR_InstructionComplete);
  
  	Robot->SetRobotInfo(RobotInfo);
  	return TaskResult;
  }
  
  double CMotion::GetRotationAngle()
  {
  	CString angleText;
  	m_Degrees.GetWindowText(angleText);
  	double angle = atof(angleText);
  	if (angle <= 3) {
  		angle = (3 - angle)*pi/6;
  	} else {
  		angle = pi * (-angle/6 + 2.5);
  	}
  	return angle;
  }
  
  double CMotion::GetTravelDistance()
  {
  	double distance;
  	CString distanceText;
  	CString directionText;
  	m_Distance.GetWindowText(distanceText);
  	distance = atof(distanceText);
  	m_Direction.GetWindowText (directionText);
  	if (directionText == "backward") distance = -distance;
  	if (m_Units.GetCurSel() == 0) distance *= cTPI;
  	else distance *= 12*cTPI;
  	return distance;
  }
  #endif
  
  
  
  void CMotion::OnSelchangeDegrees() 
  {
  	// indicate that a change has been made in the file
  	CFlowZap *fz = (CFlowZap*)m_fc;
  	fz->ChangeMadeInFile();
  	
  }
  
  void CMotion::OnSelchangeDirection() 
  {
  	// indicate that a change has been made in the file
  	CFlowZap *fz = (CFlowZap*)m_fc;
  	fz->ChangeMadeInFile();
  	
  }
  
  void CMotion::OnChangeDistance() 
  {
  	CFlowZap *fz = (CFlowZap*)m_fc;
  	// indicate that a change has been made in the file
  	if (m_Distance.GetModify () ) {
  		fz->ChangeMadeInFile ();
  	}
  	
  	
  }
  
  void CMotion::OnSelchangeUnits() 
  {
  	// indicate that a change has been made in the file
  	CFlowZap *fz = (CFlowZap*)m_fc;
  	fz->ChangeMadeInFile();
  	
  }
  
  void CMotion::EnableRotationControls(BOOL enabled)
  {
  	m_TextOClock.EnableWindow(enabled);
  	m_Degrees.EnableWindow(enabled);
  	m_TextRotate.EnableWindow(enabled);
  }
  
  void CMotion::EnableMotionControls(BOOL enabled)
  {
  	m_Distance.EnableWindow(enabled);
  	m_Direction.EnableWindow(enabled);
  	m_Units.EnableWindow(enabled);
  	m_TextGo.EnableWindow(enabled);
  }
