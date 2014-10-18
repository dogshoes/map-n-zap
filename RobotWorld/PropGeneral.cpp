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
  // PropGeneral.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "PropGeneral.h"
  #include "Robot.h"
  #include "RRDim.h"
  #include <fstream>

  /////////////////////////////////////////////////////////////////////////////
  // CPropGeneral property page
  
  IMPLEMENT_DYNCREATE(CPropGeneral, CPropertyPage)
  
  CPropGeneral::CPropGeneral() : CPropertyPage(CPropGeneral::IDD)
  {
  	//{{AFX_DATA_INIT(CPropGeneral)
  	m_chkShowCalibrationResults = FALSE;
  	m_chkIntermittentBeep = FALSE;
  	m_chkFlipHandle = FALSE;
  	m_chkLearnRoom = FALSE;
  	m_chkAnimateRobot = FALSE;
  	m_chkEnableDancing = FALSE;
  	//}}AFX_DATA_INIT
  }
  
  CPropGeneral::~CPropGeneral()
  {
  }
  
  void CPropGeneral::DoDataExchange(CDataExchange* pDX)
  {
  	CPropertyPage::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CPropGeneral)
  	DDX_Check(pDX, IDC_CHK_SHOW_CALIBRATION_RESULTS, m_chkShowCalibrationResults);
  	DDX_Check(pDX, IDC_CHK_INTERMITTENT_BEEP, m_chkIntermittentBeep);
  	DDX_Check(pDX, IDC_CHKFLIPHANDLE, m_chkFlipHandle);
  	DDX_Check(pDX, IDC_CHK_LEARN_ROOM, m_chkLearnRoom);
  	DDX_Check(pDX, IDC_CHK_ANIMATE_ROBOT, m_chkAnimateRobot);
  	DDX_Check(pDX, IDC_CHK_ENABLE_DANCING, m_chkEnableDancing);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CPropGeneral, CPropertyPage)
  	//{{AFX_MSG_MAP(CPropGeneral)
  	ON_BN_CLICKED(IDC_BUT_POWER_OFF, OnButPowerOff)
  	ON_BN_CLICKED(IDC_BUT_ROBOT_TEST, OnButRobotTest)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CPropGeneral message handlers
  
  void CPropGeneral::SetShowCalibrationResults(bool value)
  {
  	m_chkShowCalibrationResults = value;
  	if (m_hWnd != NULL) UpdateData(FALSE);
  }
  
  bool CPropGeneral::GetShowCalibrationResults()
  {
  	if (m_hWnd != NULL) UpdateData(TRUE);
  	return m_chkShowCalibrationResults != 0;
  }
  
  void CPropGeneral::SetIntermittentBeep(bool On)
  {
  	m_chkIntermittentBeep = On;
  	if (m_hWnd != NULL) UpdateData(FALSE);
  }
  
  bool CPropGeneral::GetIntermittentBeep()
  {
  	if (m_hWnd != NULL) UpdateData(TRUE);
  	return m_chkIntermittentBeep != 0;
  }
  
  void CPropGeneral::SetFlipHandle(bool FlipHandle)
  {
  	m_chkFlipHandle = FlipHandle;
  	if (m_hWnd != NULL) UpdateData(FALSE);
  }
  
  bool CPropGeneral::GetFlipHandle()
  {
  	if (m_hWnd != NULL) UpdateData(TRUE);
  	return m_chkFlipHandle != 0;
  }
  
  void CPropGeneral::SetLearnRoom(bool On)
  {
  	m_chkLearnRoom = On;
  	if (m_hWnd != NULL) UpdateData(FALSE);
  }
  
  bool CPropGeneral::GetLearnRoom()
  {
  	if (m_hWnd != NULL) UpdateData(TRUE);
  	return m_chkLearnRoom != 0;
  }
  
  void CPropGeneral::OnButPowerOff() 
  {
  	m_Robot->TurnOffPower();		
  }
  
  void CPropGeneral::OnButRobotTest() 
  {
	std::ofstream output(((CRobotWorldApp*)AfxGetApp())->m_DefaultPath + "/Test Robot.txt");
  	if (!m_Robot->GetCommunicationsEstablished()) {
  		AfxMessageBox("Test Robot Error: The PC has not established\ncommunications with the robot");
  		output << "The PC has not established communications with the robot.\n";
  	} else {
  		if (m_Robot->GetOnHomeBase()) {
  			TRobotInfo OldRobotInfo = m_Robot->GetRobotInfo();
  			m_Robot->SetHandleLength(cFollowCTDPathHandleLength);
  
  			m_Robot->AddSquareToWallToInstructions(m_Robot->GetHeading(), false);
  			if (m_Robot->FollowInstructions(true, true, IR_InstructionComplete, true) == TR_TaskComplete) {
  				robPOINT OriginalPosition = m_Robot->GetTaskResultPosition();
  				double OriginalHeading = m_Robot->GetTaskResultHeading();
  
  				robPOINT Dest = m_Robot->GetHandlePosition(-cFollowCTDPathHandleLength);
  				Dest.x -= 24*cTPI*cos(m_Robot->GetHeading());
  				Dest.y -= 24*cTPI*sin(m_Robot->GetHeading());
  		//		m_Robot->AddPositionVelocityToInstructions(Dest, NULL, cMinPositionZone, cComputerControlSpeed, true, 0);
  				m_Robot->AddVelocitiesToInstructions(-100, -100, 3000);
  				if (m_Robot->FollowInstructions(true, true, IR_InstructionComplete, true) == TR_TaskComplete) {
  					Dest.x = OriginalPosition.x + 10*cTPI * cos(OriginalHeading);
  					Dest.y = OriginalPosition.y + 10*cTPI * sin(OriginalHeading);
  					m_Robot->AddVelocitiesToInstructions(100, 100, 5000);
  		//			m_Robot->AddPositionVelocityToInstructions(Dest, NULL, cMinPositionZone, cObstacleDetectionSpeed, false, 0);
  					if (m_Robot->FollowInstructions(true, true, IR_HitObstacle, true) == TR_TaskComplete) {
  						m_Robot->AddSquareToWallToInstructions(m_Robot->GetHeading(), false);
  						if (m_Robot->FollowInstructions(true, true ,IR_InstructionComplete, true) == TR_TaskComplete) {
  							robPOINT FinalPosition = m_Robot->GetTaskResultPosition();
  							double FinalHeading = m_Robot->GetTaskResultHeading();
  
  							double DHeading = AbsAngularSeparation(OriginalHeading, FinalHeading);
  							double DPosition = sqrt(sqr(FinalPosition.x - OriginalPosition.x) + sqr(FinalPosition.y - OriginalPosition.y));
  							output << "Testing Robot Diagnostic\n";
  							output << "DHeading = " << DHeading << "(radian)\n";
  							output << "DPosition = " << DPosition << "(1/10 Inch)\n";
  							if ( DHeading >= (6.0/180*pi) ) {
  								AfxMessageBox("Robot failed diagnostic test");
  								output << "Robot failed diagnostic test.  DHeading >= 6 Degrees\n";
  							} else if ( DHeading >= (3.0/180*pi) ) {
  								AfxMessageBox("Robot may be out of spec");
  								output << "Robot may be out of spec.  3 <= DHeading < 6 Degrees\n";
  							} else {
  								AfxMessageBox("Robot is working correctly");
  								output << "Robot is working correctly.\n";
  							}
  						} else {
  							AfxMessageBox("The robot failed to square to the wall the second time.  Diagnostic failed.");
  							output << "The robot failed to square to the wall the second time.  Diagnostic failed.\n";
  						}
  					} else {
  						AfxMessageBox("The robot failed to run into the wall.  Diagnostic failed.");
  						output << "The robot failed to run into the wall.  Diagnostic failed.\n";
  					}
  				} else {
  					AfxMessageBox("The robot failed to back up from the wall.  Diagnostic failed.");
  					output << "The robot failed to back up from the wall.  Diagnostic failed.\n";
  				}
  			} else {
  				AfxMessageBox("The robot failed to square to the wall.  Diagnostic failed.");
  				output << "The robot failed to square to the wall.  Diagnostic failed.\n";
  			}
  			m_Robot->SetRobotInfo(OldRobotInfo);
  		} else {
  			AfxMessageBox("Put the robot on the home base first.\nThen retry the test");
  			output << "The robot wasn't on the home base.\n";
  		}
  	}
  }
  
  bool CPropGeneral::GetAnimateRobot()
  {
  	if (m_hWnd != NULL) UpdateData(TRUE);
  	return m_chkAnimateRobot != 0;
  }
  
  void CPropGeneral::SetAnimateRobot(bool Animate)
  {
  	m_chkAnimateRobot = Animate;
  	if (m_hWnd != NULL) UpdateData(FALSE);
  }
  
  void CPropGeneral::SetEnableDancing(bool Enable)
  {
  	m_chkEnableDancing = Enable;
  	if (m_hWnd != NULL) UpdateData(FALSE);
  }
  
  bool CPropGeneral::GetEnableDancing()
  {
  	if (m_hWnd != NULL) UpdateData(TRUE);
  	return m_chkEnableDancing != 0;
  }
