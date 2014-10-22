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
 
  // DlgJoystickControl.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "RobotWorld.h"
  #include "DlgJoystickControl.h"
  #include "rothmath.h"

  /*******************************************************************************
  //Created: 11/13/97 S.R.
  //Last Revision: 11/13/97 S.R.
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
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgJoystickControl dialog
  
  IMPLEMENT_DYNCREATE(CDlgJoystickControl, CPropertyPage)
  
  #define GetRobotWorldView() ((CRobotWorldView*)(((CMainFrame*)GetParent())->GetActiveView()))
  #define HeadingToScrollPos(Heading) (1000/360.0*Heading)
  #define ScrollPosToHeading(ScrollPos) (ScrollPos*360.0/1000)
  
  const int cResponsivenessRange = 1;
  const int cTurnGainRange = 3;
  const int cOrientationRange = 7;
  const int cMaxOrientationChars = 3;
  
  CDlgJoystickControl::CDlgJoystickControl()
  	: CPropertyPage(CDlgJoystickControl::IDD)
  {
  	//{{AFX_DATA_INIT(CDlgJoystickControl)
  	m_edtJoystickOrientation = _T("0");
  	m_Enable = FALSE;
  	m_RobotCoordinates = -1;
  	//}}AFX_DATA_INIT
  	m_Responsiveness = 0;
  	m_TurnGain = 0;
  }
  
  
  void CDlgJoystickControl::DoDataExchange(CDataExchange* pDX)
  {
  	CPropertyPage::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgJoystickControl)
  	DDX_Control(pDX, IDC_CHK_JOYSTICK_ENABLE, m_chkEnable);
  	DDX_Control(pDX, IDC_RAD_JOYSTICK_WORLD_COORDS, m_radWorldCoordinates);
  	DDX_Control(pDX, IDC_RAD_JOYSTICK_ROBOT_COORDS, m_radRobotCoordinates);
  	DDX_Control(pDX, IDC_SCR_JOYSTICK_ORIENTATION, m_scrJoystickOrientation);
  	DDX_Control(pDX, IDC_SLI_TURNGAIN, m_sliTurnGain);
  	DDX_Control(pDX, IDC_SLI_JOYSTICKRESPONSIVENESS, m_sliResponsiveness);
  	DDX_Text(pDX, IDC_EDT_JOYSTICK_ORIENTATION, m_edtJoystickOrientation);
  	DDX_Check(pDX, IDC_CHK_JOYSTICK_ENABLE, m_Enable);
  	DDX_Radio(pDX, IDC_RAD_JOYSTICK_ROBOT_COORDS, m_RobotCoordinates);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgJoystickControl, CPropertyPage)
  	//{{AFX_MSG_MAP(CDlgJoystickControl)
  	ON_BN_CLICKED(IDC_BUT_SET_DEFAULTS, OnButSetDefaults)
  	ON_WM_HSCROLL()
  	ON_EN_CHANGE(IDC_EDT_JOYSTICK_ORIENTATION, OnChangeEdtJoystickOrientation)
  	ON_BN_CLICKED(IDC_CHK_JOYSTICK_ENABLE, OnChkJoystickEnable)
  	ON_WM_DESTROY()
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgJoystickControl message handlers
  
  void CDlgJoystickControl::OnButSetDefaults() 
  {
  	m_sliResponsiveness.SetPos(0);
  	m_sliTurnGain.SetPos(cTurnGainRange);
  }
  
  void CDlgJoystickControl::SetControlType(TJoystickControlType JoyControlType)
  /*******************************************************************************
  //Created: 11/13/97 S.R.
  //Last Revision: 11/13/97 S.R.
  //Parameters:
  //	JoyControlType:	the type of joystick control
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //		This function sets all the radio buttons and the enable check box to
  //	reflect the value of JoyControlType.
  //Method:
  //
  *******************************************************************************/
  {
  	if (JoyControlType == jcRobotPwm) JoyControlType = jcRobotVelocity;
  	else if (JoyControlType == jcWorldPwm) JoyControlType = jcWorldVelocity;
  	switch (JoyControlType) {
  	case (jcRobotPwm):
  		m_RobotCoordinates = 0;
  		SetEnable(TRUE);
  		break;
  	case (jcWorldPwm):
  		m_RobotCoordinates = 1;
  		SetEnable(TRUE);
  		break;
  	case (jcRobotVelocity):
  		m_RobotCoordinates = 0;
  		SetEnable(TRUE);
  		break;
  	case (jcWorldVelocity):
  		m_RobotCoordinates = 1;
  		SetEnable(TRUE);
  		break;
  	case (jcNone):
  		SetEnable(FALSE);
  		break;
  	}
  	if (m_hWnd != NULL) UpdateData(FALSE);
  }
  
  float CDlgJoystickControl::GetResponsiveness()
  {
  	if (m_hWnd != NULL) {
  		m_Responsiveness = 2.0f - m_sliResponsiveness.GetPos();
  	}
  	return m_Responsiveness;
  //	return 4-3.0*m_JoystickResponsiveness.GetPos()/float(cResponsivenessRange);
  }
  
  float CDlgJoystickControl::GetTurnGain()
  {
  	if (m_hWnd != NULL) {
  		m_TurnGain = cTurnGainRange + 1.0f - m_sliTurnGain.GetPos();
  	}
  	return m_TurnGain;
  }
  
  double CDlgJoystickControl::GetOrientation()
  /*******************************************************************************
  //Created: 11/13/97 S.R.
  //Last Revision: 11/13/97 S.R.
  //Parameters:
  //	none
  //Return Value:
  //	Orientation in radians.
  ********************************************************************************
  //Description:
  //		This function returns the joystick's orientation in radians.
  //Method:
  //
  *******************************************************************************/
  {
  	if (m_hWnd != NULL) UpdateData(TRUE);
  //	return (atoi(m_edtJoystickOrientation)/360.0*2*pi);
  	return (pi/2);
  }
  
  void CDlgJoystickControl::SetResponsiveness(float Responsiveness)
  {
  	m_Responsiveness = Responsiveness;
  	if (m_hWnd != NULL) m_sliResponsiveness.SetPos(2 - (int)m_Responsiveness);
  }
  
  void CDlgJoystickControl::SetOrientation(double Orientation)
  /*******************************************************************************
  //Created: 11/13/97 S.R.
  //Last Revision: 11/13/97 S.R.
  //Parameters:
  //	Orientation:	the joystick's orientation in radians.
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	Orientation = Orientation /(2*pi)*360;
  	char buffer[cMaxOrientationChars];
  	m_edtJoystickOrientation = itoa((int)Orientation, buffer, 10);
  	if (m_hWnd != NULL) {
  		m_scrJoystickOrientation.SetScrollPos((int)HeadingToScrollPos(Orientation), TRUE);
  		UpdateData(FALSE);
  	}
  }
  
  void CDlgJoystickControl::SetTurnGain(float Gain)
  {
  	m_TurnGain = Gain;
  	if (m_hWnd != NULL) m_sliTurnGain.SetPos(int(cTurnGainRange + 1 - m_TurnGain));
  }
  
  void CDlgJoystickControl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
  /*******************************************************************************
  //Created: 11/13/97 S.R.
  //Last Revision: 11/13/97 S.R.
  //Parameters:
  //	
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //		Handles scrolling the joystick orientation.  It will update the joystick
  //	orientation edit box.
  //Method:
  //
  *******************************************************************************/
  {
  	const int cLineInc = 20;
  	const int cPageInc = 100;
  	int Min, Max;
  	if (pScrollBar == &m_scrJoystickOrientation) {
  		switch (nSBCode) {
  		case SB_LEFT:
  			m_scrJoystickOrientation.GetScrollRange(&Min, &Max);
  			nPos = Min;
  			break;
  		case SB_ENDSCROLL:
  			nPos = m_scrJoystickOrientation.GetScrollPos();
  			break;
  		case SB_LINELEFT:
  			nPos = m_scrJoystickOrientation.GetScrollPos();
  			if (nPos <= cLineInc) nPos = 0;
  			else nPos -= cLineInc;
  			m_scrJoystickOrientation.GetScrollRange(&Min, &Max);
  			if (nPos < (UINT)Min) nPos = Min;
  			break;
  		case SB_LINERIGHT:
  			nPos = m_scrJoystickOrientation.GetScrollPos();
  			nPos += cLineInc;
  			m_scrJoystickOrientation.GetScrollRange(&Min, &Max);
  			if (nPos > (UINT)Max) nPos = Max;
  			break;
  		case SB_PAGELEFT:
  			nPos = m_scrJoystickOrientation.GetScrollPos();
  			nPos -= cPageInc;
  			m_scrJoystickOrientation.GetScrollRange(&Min, &Max);
  			if (nPos < (UINT)Min) nPos = Min;
  			break;
  		case SB_PAGERIGHT:
  			nPos = m_scrJoystickOrientation.GetScrollPos();
  			nPos += cPageInc;
  			m_scrJoystickOrientation.GetScrollRange(&Min, &Max);
  			if (nPos > (UINT)Max) nPos = Max;
  			break;
  		case SB_RIGHT:
  			m_scrJoystickOrientation.GetScrollRange(&Min, &Max);
  			nPos = Max;
  			break;
  		case SB_THUMBPOSITION:
  			break;
  		case SB_THUMBTRACK:
  			break;
  		}
  		m_scrJoystickOrientation.SetScrollPos(nPos, TRUE);
  		char buffer[cMaxOrientationChars];
  		m_edtJoystickOrientation = itoa((int)ScrollPosToHeading(nPos),buffer,10);
  //		m_edtJoystickOrientation = ScrollPosToHeading(nPos);
  		UpdateData(FALSE);
  //		UpdateOrientation(m_edtJoystickOrientation);
  	}
  	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
  }
  
  BOOL CDlgJoystickControl::OnInitDialog() 
  {
  	CPropertyPage::OnInitDialog();
  	
  	m_sliResponsiveness.SetRange(0, cResponsivenessRange);
  	m_sliTurnGain.SetRange(0,cTurnGainRange);
  	m_scrJoystickOrientation.SetScrollRange(0,1000);
  	m_scrJoystickOrientation.SetScrollPos((int)HeadingToScrollPos(atoi(m_edtJoystickOrientation)));
  //	m_sliTurnGain.SetPos(3);
  	SetResponsiveness(m_Responsiveness);
  	SetTurnGain(m_TurnGain);
  	SetEnable(m_Enable);
  //	SetControlType(jcRobotVelocity);
  //	SetControlType(jcNone);
  	UpdateData(FALSE);
  	
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CDlgJoystickControl::OnChangeEdtJoystickOrientation() 
  {
  	// TODO: If this is a RICHEDIT control, the control will not
  	// send this notification unless you override the CPropertyPage::OnInitDialog()
  	// function to send the EM_SETEVENTMASK message to the control
  	// with the ENM_CHANGE flag ORed into the lParam mask.
  	
  	UpdateData(TRUE);
  	m_scrJoystickOrientation.SetScrollPos((int)HeadingToScrollPos(atoi(m_edtJoystickOrientation)));
  //	UpdateHeading(m_edtJoystickOrientation);
  	
  }
  
  TJoystickControlType CDlgJoystickControl::GetControlType()
  {
  	if (m_hWnd != NULL) UpdateData(TRUE);
  	if (!m_Enable) {
  		return jcNone;
  	} else {
  		if (m_RobotCoordinates == 0) {
  			return jcRobotVelocity;
  		} else if (m_RobotCoordinates == 1) {
  			return jcWorldVelocity;
  		} else {
  			return jcNone;
  		}
  	}
  }
  
  void CDlgJoystickControl::OnChkJoystickEnable() 
  {
  	SetEnable(m_chkEnable.GetCheck());
  }
  
  void CDlgJoystickControl::SetEnable(BOOL Enable)
  /*******************************************************************************
  //Created: 11/13/97 S.R.
  //Last Revision: 11/13/97 S.R.
  //Parameters:
  //	Enable:	whether the joystick should be enabled or not.
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //		Depending on Enable, this function enables or disables the radio buttons
  //	that control the joystick control variables.
  //Method:
  //
  *******************************************************************************/
  {
  	m_Enable = Enable;
  	if (m_hWnd != NULL) {
  		if (Enable) {
  			m_radRobotCoordinates.EnableWindow(TRUE);
  			m_radWorldCoordinates.EnableWindow(TRUE);
  			m_sliResponsiveness.EnableWindow(TRUE);
  			if (m_RobotCoordinates)	m_sliTurnGain.EnableWindow(TRUE);
  		} else {
  			m_radRobotCoordinates.EnableWindow(FALSE);
  			m_radWorldCoordinates.EnableWindow(FALSE);
  			m_sliResponsiveness.EnableWindow(FALSE);
  			m_sliTurnGain.EnableWindow(FALSE);
  		}
  		UpdateData(FALSE);
  	}
  }
  
  #ifdef JUNK
  void CDlgJoystickControl::Serialize(CArchive & archive)
  {
  	if (archive.IsStoring()) {
  		if (m_hWnd != NULL) UpdateData(TRUE);
  		archive << m_Enable;
  		int tInt;
  		tInt = (int)GetControlType();
  		archive << tInt;
  		archive << GetOrientation();
  		archive << GetResponsiveness();
  		archive << GetTurnGain();
  	} else {
  		BOOL tBool;
  		double tDouble;
  		float tfloat;
  		int tInt;
  		archive >> m_Enable;
  		archive >> tInt, SetControlType((TJoystickControlType)tInt);
  		archive >> tDouble, SetOrientation(tDouble);
  		archive >> tfloat, SetResponsiveness(tfloat);
  		archive >> tfloat, SetTurnGain(tfloat);
  		SetEnable(m_Enable);
  		if (m_hWnd != NULL) UpdateData(FALSE);
  	}
  }
  #endif
  
  void CDlgJoystickControl::OnDestroy() 
  {
  	UpdateData(TRUE);
  	m_Responsiveness = GetResponsiveness();
  	m_TurnGain = GetTurnGain();
  
  	CPropertyPage::OnDestroy();
  	
  	// TODO: Add your message handler code here
  	
  }
