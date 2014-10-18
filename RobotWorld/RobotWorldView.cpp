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
  // RobotWorldView.cpp : implementation of the CRobotWorldView class
  //
  
  /*******************************************************************************
  //Created: 10/30/97 S.R.
  //Last Revision: 10/30/97 S.R.
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
  #include "stdafx.h"
  #include "global.h"
  #include <assert.h>
  #include <math.h>
  #include "SerialCommEvents.h"
  
  #include "RobotWorld.h"
  #include "RobotWorldDoc.h"
  #include "RobotWorldView.h"
  #include "WorldGridConsts.h"
  #include "DlgCreateVacuumPatch.h"
  
  #include "World.h"
  //#include "Label.h"
  #include "Probability Grid.h"
  #include "Red Zone.h"
  #include "VacuumPatch.h"
  
  //#include "DlgLabelName.h"
  #include "DlgExecutableObjectGeneralProperties.h"
  #include "RobotWorld Resource.h"
  #include "Path.h"
  #include "MainFrm.h"
  //#include "VacuumPatchLabel.h"
  
  //#include "CalibrationZoneLabel.h"
  #include "FlowZapResource.h"
  #include "FlowZap.h"
  
  #include "Laygo.h"
  
  #include "PositionLabel.h"
  #include "ExecutableRWGraphicObject.h"
  #include "FinishLine.h"
  #include "StartLine.h"
  #include "HomeBase.h"
  #include "CTDPath.h"
  
  #include "DlgTestRobot.h"
  #include "DlgRelocateRobot.h"
  #include "DlgOrientHomebase.h"
  #include "DlgFindHomebaseCorner.h"
  #include "DlgErrorCheckingWoodWithVacuum.h"
  #include "DlgVelocityTorqueDisplay.h"
  #include "DlgCyeSample.h"
  #include "DlgCreateHomeOnWood.h"
  #include "DlgAddRobot.h"
  #include "DlgAddHomebase.h"
  
  #include "PathDisplay.h"
  
  #include "LinePath.h"
  #include "RothUtil.h"
  
  #include <iomanip>
  
  #ifdef MHCOMM_CONTROL
  #include "Robot Communication.h"
  #endif

  //#define CIRCUIT_TESTING
  
  #define SER_TIMER				1		//Timer identifier for the serial output timer
  #define SER_OUT_INTERVAL	300	//Timer interval for the serial output timer (in ms)
  #define CAL_TIMER				2		//Timer identifier for the calibration check timer
  #define CAL_INTERVAL			5000	//Timer interval for the calibration check (in ms)
  #define ANIMATION_TIMER		3
  #define ANIMATION_INTERVAL	41	//Timer interval for the animation (in ms)
  #define GetMainFrame() ((CMainFrame*)GetParentFrame())
  //#define JOYSTICK_TIMER_INTERVAL 40
  #define JOYSTICK_TIMER_INTERVAL 300
  
  const COLORREF cFindPathPenColor = COLORREF(0xFFFF00);
  
  /////////////////////////////////////////////////////////////////////////////
  // CRobotWorldView
  
  //IMPLEMENT_DYNCREATE(CRobotWorldView, CView)
  IMPLEMENT_SERIAL(CRobotWorldView, CView, VERSIONABLE_SCHEMA | VERSION_NUMBER)
  
  BEGIN_MESSAGE_MAP(CRobotWorldView, CView)
  	ON_WM_CONTEXTMENU()
  	//{{AFX_MSG_MAP(CRobotWorldView)
  	ON_COMMAND(ID_GOHOME, OnGoHome)
  	ON_WM_TIMER()
  	ON_WM_SIZE()
  	ON_WM_VSCROLL()
  	ON_COMMAND(ID_STOP, OnStop)
  	ON_COMMAND(ID_SEARCHAREA, OnSearchArea)
  	ON_COMMAND(ID_CREATECALIBRATIONWALL, OnCreateCalibrationWall)
  	ON_COMMAND(ID_CREATECALIBRATIONDOORWAY, OnCreateCalibrationDoorway)
  	ON_WM_HSCROLL()
  	ON_WM_LBUTTONDOWN()
  	ON_WM_LBUTTONUP()
  	ON_WM_MOUSEMOVE()
  	ON_WM_SETCURSOR()
  	ON_WM_DESTROY()
  	ON_COMMAND(ID_POWER_OFF, OnPowerOff)
  	ON_WM_LBUTTONDBLCLK()
  	ON_COMMAND(ID_VIEW_CYE_PERSONA, OnViewCyePersona)
  	//}}AFX_MSG_MAP
  	// Standard printing commands
  	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
  	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
  	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
     ON_MESSAGE(ID_ROBOT_FIND_PATH_FINISHED, OnRobotFindPathFinished)
     ON_MESSAGE(ID_ROBOT_SHOW_FIND_PATH, OnRobotShowFindPath)
  	ON_MESSAGE(ID_ROBOT_FIND_PATH_STAGE, OnRobotFindPathStage)
     ON_MESSAGE(ID_ROBOT_CHANGE_STATE, OnRobotChangeState)
  	ON_MESSAGE(ID_ROBOT_FIND_PATH_ERROR, OnRobotFindPathError)
  END_MESSAGE_MAP()
  
  BEGIN_EVENTSINK_MAP(CRobotWorldView, CView)
      //{{AFX_EVENTSINK_MAP(CRobotWorldView)
  	//}}AFX_EVENTSINK_MAP
  END_EVENTSINK_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  //	Joystick Stuff
  
  UINT JoystickDeviceID;
  
  void CALLBACK JoystickCaptureTimer(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime );
  
  void RotateXY(int* x, int* y, double RobotHeading, double JoystickHeading)
  {
  	double phi = RobotHeading - JoystickHeading;
  	int temp = int(*x*cos(phi) + *y*sin(phi));
  	*y = int(*y*cos(phi) - *x*sin(phi));
  	*x = temp;
  	return;
  }
  
  void CALLBACK EXPORT CalibrationTimerProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
  {
  	CRobotWorldView* RobotWorldView = (CRobotWorldView*)CWnd::FromHandle(hWnd);
  //	RobotWorldView->OnUpdateRadioLevel();
  //	if (RobotWorldView->GetActiveRobot()->GetAutoCalibrate()) {
  //		RobotWorldView->GetActiveRobot()->AutoCalibrate();
  //	}
  }
  
  
  float JoyDisplacementToMotionParam(int x, float n, float MinX, float MaxX, float MinParam, float MaxParam)
  {
  	float y;
  	if (ABS(x) < MinX) return 0;
  	if (n == 2) {
  		x = ABS(x);
  		y = (MaxParam - MinParam) / SQR((MaxX - MinX)) * (SQR(x) - 2 * MinX * x + SQR(MinX)) + MinParam;
  		if (x < 0) y = -y;
  	} else {
  		y = (MaxParam - MinParam) / (MaxX - MinX) * (x - MinX) + MinParam;
  	}
  	return y;
  }
  
  void CALLBACK JoystickCaptureTimer(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
  /*******************************************************************************
  //Created: 10/30/97 S.R.
  //Last Revision: 10/30/97 S.R.
  //Parameters:
  //	hwnd:	window sending the timer message
  //	uMsg:	the message
  //	idEvent:	event id.
  //	dwTime:	time of event.
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //		
  //Method:
  //
  *******************************************************************************/
  {
  	static int oldM0V = 0, oldM1V = 0, oldM0Pwm = 0, oldM1Pwm = 0;
  	static unsigned int MaxX = 0, MaxY = 0;
  	static double OldDirection = 0;
  	CRobotWorldView* RobotView = (CRobotWorldView*)CWnd::FromHandle(hwnd);
  	CRobot* Robot = RobotView->GetActiveRobot();
  	if (Robot == NULL) return;
  /*If the robot's comm port hasn't been opened yet, don't try to send anything*/
  	if (!Robot->GetCommunicationOn()) return;
  	if ( (Robot->GetState() == stWaiting) ||
  	 (Robot->GetState() == stJoysticking) )
  	{
  		if (RobotView->m_JoystickControlType == jcNone) {
  			if (Robot->GetState() == stJoysticking) {
  				Robot->SetState(stWaiting);
  			}
  			return;
  		}
  		unsigned int X,Y;
  		JOYINFO JoyInfo;
  		MMRESULT temp = joyGetPos(JoystickDeviceID, &JoyInfo);
  		if (temp == JOYERR_NOERROR) {
  			static unsigned int OldButtons = 0;
  			if (JoyInfo.wButtons) {
  			/*Change in button state*/
  				if (JoyInfo.wButtons != OldButtons) {
  					if (JoyInfo.wButtons & JOY_BUTTON1) {
  						Robot->SetBuzzerFrequency(2000);
  					} else if (JoyInfo.wButtons & JOY_BUTTON2) {
  						Robot->SetBuzzerFrequency(1000);
  					} else if (JoyInfo.wButtons & JOY_BUTTON3) {
  						Robot->SetBuzzerFrequency(500);
  					} else if (JoyInfo.wButtons & JOY_BUTTON4) {
  						Robot->SetBuzzerFrequency(250);
  					}
  				}
  			} else {
  				if (OldButtons) {
  					Robot->SetBuzzerFrequency(0);
  				}
  			}
  			OldButtons = JoyInfo.wButtons;
  
  			X = JoyInfo.wXpos;
  			Y = JoyInfo.wYpos;
  			const short cDeadZone = 4000;
  			int x,y;
  			x = X - 32768;
  			y = 32768 - Y;
  			if ( (x<cDeadZone && x>0) || (x>-cDeadZone && x<0) ) x = 0;
  			if ( (y<cDeadZone && y>0) || (y>-cDeadZone && y<0) ) y = 0;
  			int M0Pwm, M1Pwm, M0V, M1V;
  			float forward, right;
  			int K;
  //			int temp;
  			MaxX = unsigned int(Max(abs(x), MaxX));
  			MaxY = unsigned int(Max(abs(y), MaxY));
  		/*Reset obstacle detection when joystick centered*/
  			if ( x == 0 &&  y == 0 ) {
  				if (Robot->GetState() == stJoysticking) {
  					Robot->SetState(stWaiting);
  				}
  				oldM0V = 0;
  				oldM1V = 0;
  				oldM0Pwm = 0;
  				oldM1Pwm = 0;
  				OldDirection = 0;
  				Robot->m_MotorObstacle.ValidObstacle = FALSE;
  			/*Check Laygo stuff*/
  				if (RobotView->m_DlgLaygo != NULL) {
  					if (!RobotView->m_DlgLaygo->IsWindowVisible()) {
  						if ( (MaxX > 20000) || (MaxY > 20000) ) {
  							if (!RobotView->m_DlgLaygo->GetShownJoystick()) {
  								RobotView->ShowLaygoTip(CLaygo::lgoJoystick, false);
  							}
  						}
  					}
  				}
  			} else {
  				if ( !Robot->m_MotorObstacle.ValidObstacle ) {
  					if (Robot->GetState() == stWaiting) {
  						Robot->SetState(stJoysticking);
  					}
  				}
  			}
  		/*Stop when the robot hits an obstacle*/
  			if ( !Robot->m_MotorObstacle.ValidObstacle ) {
  				switch (RobotView->m_JoystickControlType) {
  				case jcRobotPwm:
  					forward = JoyDisplacementToMotionParam(y,RobotView->m_JoystickResponsiveness,cDeadZone,32768,50,cMaxPwm);
  					right = JoyDisplacementToMotionParam(x,RobotView->m_JoystickResponsiveness,cDeadZone,32768,50,cMaxPwm);
  //					M0Pwm = forward - right;
  //					M1Pwm = forward + right;
  					M0Pwm = int(forward - right/RobotView->m_JoystickTurnGain);
  					M1Pwm = int(forward + right/RobotView->m_JoystickTurnGain);
  					K = max(abs(M0Pwm),abs(M1Pwm));
  					if (K > cMaxPwm) {
  						M0Pwm = M0Pwm*cMaxPwm/K;
  						M1Pwm = M1Pwm*cMaxPwm/K;
  					}
  					M0Pwm = (M0Pwm + oldM0Pwm)/2;
  					oldM0Pwm = M0Pwm;
  					M1Pwm = (M1Pwm + oldM1Pwm)/2;
  					oldM1Pwm = M1Pwm;
  					Robot->SetPwm(M0Pwm, M1Pwm);
  					break;
  				case jcWorldPwm:
  					RotateXY(&x, &y, Robot->m_Heading, RobotView->m_JoystickOrientation);
  					forward = JoyDisplacementToMotionParam(y,RobotView->m_JoystickResponsiveness,cDeadZone,32768,50,cMaxPwm);
  					right = JoyDisplacementToMotionParam(x,RobotView->m_JoystickResponsiveness,cDeadZone,32768,50,cMaxPwm);
  					M0Pwm = int(forward - right);
  					M1Pwm = int(forward + right);
  					K = max(abs(M0Pwm),abs(M1Pwm));
  					if (K > cMaxPwm) {
  						M0Pwm = M0Pwm*cMaxPwm/K;
  						M1Pwm = M1Pwm*cMaxPwm/K;
  					}
  					M0Pwm = (M0Pwm + oldM0Pwm)/2;
  					oldM0Pwm = M0Pwm;
  					M1Pwm = (M1Pwm + oldM1Pwm)/2;
  					oldM1Pwm = M1Pwm;
  					Robot->SetPwm(M0Pwm, M1Pwm);
  					break;
  				case jcRobotVelocity:
  					forward = JoyDisplacementToMotionParam(y,RobotView->m_JoystickResponsiveness,cDeadZone, 32768, cMinSpeed, Robot->GetMaxSpeed());
  					right = JoyDisplacementToMotionParam(x,RobotView->m_JoystickResponsiveness,cDeadZone, 32768, cMinSpeed, Robot->GetMaxSpeed());
  					M0V = int(forward - right/RobotView->m_JoystickTurnGain);
  					M1V = int(forward + right/RobotView->m_JoystickTurnGain);
  					K = max(abs(M0V),abs(M1V));
  					if (K > cMaxSpeed) {
  						M0V = M0V*cMaxSpeed/K;
  						M1V = M1V*cMaxSpeed/K;
  					}
  					M0V = (M0V + oldM0V)/2;
  					M1V = (M1V + oldM1V)/2;
  //					if ( (M0V != oldM0V) || (M1V != oldM1V) ) {
  						Robot->SetVelocities(M0V, M1V);
  //					}
  					oldM1V = M1V;
  					oldM0V = M0V;
  					break;
  				case jcWorldVelocity:
  					RotateXY(&x, &y, 0, RobotView->m_JoystickOrientation);
  					forward = JoyDisplacementToMotionParam(y,RobotView->m_JoystickResponsiveness,cDeadZone, 32768, cMinSpeed, Robot->GetMaxSpeed());
  					right = JoyDisplacementToMotionParam(x,RobotView->m_JoystickResponsiveness,cDeadZone, 32768, cMinSpeed, Robot->GetMaxSpeed());
  					
  					M0V = int(sqrt(SQR(forward) + SQR(right)));
  					double theta = atan2(-right, forward);
  //					if ( (theta != OldDirection) || (M0V != oldM0V) ) {
  						Robot->SetVelocityDirection(M0V, theta);
  //					}
  					OldDirection = theta;
  					oldM0V = M0V;
  
  /*					M0V = forward - right/RobotView->m_DlgJoystickControl.GetTurnGain();
  					M1V = forward + right/RobotView->m_DlgJoystickControl.GetTurnGain();
  					K = max(abs(M0V),abs(M1V));
  					if (K > cMaxPwm) {
  						M0V = M0V*cMaxSpeed/K;
  						M1V = M1V*cMaxSpeed/K;
  					}
  					Robot->SetVelocities(M0V, M1V);
  */
  					break;
  				}
  			} else {
  				if (Robot->GetState() == stJoysticking) {
  					Robot->SetState(stWaiting);
  					Robot->SetVelocities(0, 0);
  				}
  			}
  		}
  	}
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // CRobotWorldView construction/destruction
  
  CRobotWorldView::CRobotWorldView()
  {
  	m_DlgAddHomeBase = NULL;
  	m_LastClapTime = CTime::GetCurrentTime();
  	m_DlgLowRadioLevel = NULL;
  	m_LinePathTo = NULL;
  	m_LinePathFrom = NULL;
  	m_OldBungeeOrigin.x = 0, m_OldBungeeOrigin.y = 0;
  	m_OldBungeeDestination.x = m_OldBungeeDestination.y = 0;
  	m_OldBungeeHeading = 0;
  	m_OldBungeeOn = false;
  	m_ReceivedNewPosition = false;
  	m_CurrentRWGraphicObject = NULL;
  	m_GoingHome = false;
  	m_DlgCyeSample = NULL;
  	m_LaygoNumberClicks = 0;
  	m_DlgVelocityTorqueDisplay = NULL;
  	m_PenMode = pmObstacle;
  	m_OnSlipperyFloor = false;
  	m_OldRobotCTDPath = NULL;
  	m_LMouseButtonDown = false;
  //	m_CurrentLinePathEnded = false;
  	m_CurrentLinePath = NULL;
  	m_DragLine = false;
  //	m_PenTool = ptPen;
  //	m_DlgDrawingTools = 0;
  	m_PenSize = psSmall;
  	m_MouseOverObject = false;
  	m_SelectedGraphicObject = NULL;
  	m_DraggingRect = false;
  	m_DlgCreateCalibrationWall = NULL;
  //	m_DlgCreateCalibrationDoorway = NULL;
  //	m_OutFile = NULL;
  	m_OldHScrollPos = 0;
  	m_OldVScrollPos = 0;
  	m_MouseMode = mmDefault;
  	m_ProbabilityGrid = NULL;
  //	m_LabelList = NULL;
  	m_RedZoneList = NULL;
  //	m_SelectedLabel = NULL;
  	m_ViewVacuumPatches = TRUE;
  	m_FindPathShown = NULL;
  	m_DlgFlowZap = NULL;
  	m_DlgLaygo = NULL;
  	m_DlgTestRobot = NULL;
  	m_DlgPropertySheet.Construct("Settings");
  	m_DlgPropertySheet.AddPage(&m_DlgPropRobotColor);
  	m_DlgPropertySheet.AddPage(&m_DlgJoystickControl);
  //	m_DlgPropertySheet.AddPage(&m_DlgMouseControls);
  	m_DlgPropertySheet.AddPage(&m_DlgPropFloor);
  	m_DlgPropertySheet.AddPage(&m_DlgPropGeneral);
  	m_HomeBase = NULL;
  	m_JoystickOrientation = pi/2;
  	m_JoystickTurnGain = 0;
  	m_JoystickResponsiveness = 0;
  	m_JoystickControlType = jcNone;
  //	fprintf(stderr, "CRobotWorldView::Created CRobotWorldView\n");
  }
  
  CRobotWorldView::~CRobotWorldView()
  {
  //	KillTimer(SER_TIMER);
  //	KillTimer(CAL_TIMER);
  	delete m_DlgCreateCalibrationWall;
  //	delete m_DlgCreateCalibrationDoorway;
  	delete m_ProbabilityGrid;
  //	delete m_LabelList;
  	delete m_RedZoneList;
  	if (m_DlgFlowZap != NULL) delete m_DlgFlowZap;
  	if (m_DlgCyeSample != NULL) delete m_DlgCyeSample;
  	delete m_DlgLaygo;
  	delete m_DlgTestRobot;
  	if (m_DlgAddHomeBase != NULL) delete m_DlgAddHomeBase;
  }
  
  BOOL CRobotWorldView::PreCreateWindow(CREATESTRUCT& cs)
  {
  	// TODO: Modify the Window class or styles here by modifying
  	//  the CREATESTRUCT cs
  
  	return CView::PreCreateWindow(cs);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // CRobotWorldView drawing
  
  void CRobotWorldView::SketchRobotError(robPOINT* Corners, robPOINT DPosition, CDC* dc)
  /*******************************************************************************
  //Created: 02/23/97 S.R.
  //Last Revision: 07/11/97 S.R.
  //Parameters:
  //	dc:	The DC where the robot is being displayed.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //	This function sketches the error octagon on the screen.
  //Method:
  //		The function calculates the positions of all the corners of the octagon
  //	then displays it on the screen.
  *******************************************************************************/
  {
  	unsigned char i;
  	double temp[4];
  /*	robPOINT RobotPosition = m_pRobot->m_Position;
  	for (i = 0; i < 4; i++) {
  		Corners[i].x -= RobotPosition.x;
  		Corners[i].y -= RobotPosition.y;
  	}
  */
  	for (i = 0; i < 4; i++) {
  		temp[i] = Corners[i].x;
  	}
  	double MaxX = Max(temp,4);
  	double MinX = Min(temp,4);
  	for (i = 0; i < 4; i++) {
  		temp[i] = Corners[i].y;
  	}
  	double MaxY = Max(temp,4);
  	double MinY = Min(temp,4);
  
  	robPOINT PointArray[8];
  	char j = 0;
  	for (i = 0; i < 4; i++) {
  		if (Corners[i].x == MaxX) {
  			if (Corners[i].y == MaxY) {
  				PointArray[j].x = Corners[i].x + DPosition.x;
  				PointArray[j++].y = Corners[i].y + DPosition.y;
  				PointArray[j].x = Corners[i].x - DPosition.x;
  				PointArray[j++].y = Corners[i].y + DPosition.y;
  			} else if (Corners[i].y == MinY) {
  				PointArray[j].x = Corners[i].x - DPosition.x;
  				PointArray[j++].y = Corners[i].y - DPosition.y;
  				PointArray[j].x = Corners[i].x + DPosition.x;
  				PointArray[j++].y = Corners[i].y - DPosition.y;
  			} else {
  				PointArray[j].x = Corners[i].x + DPosition.x;
  				PointArray[j++].y = Corners[i].y - DPosition.y;
  				PointArray[j].x = Corners[i].x + DPosition.x;
  				PointArray[j++].y = Corners[i].y + DPosition.y;
  			}
  		} else if (Corners[i].x == MinX) {
  			if (Corners[i].y == MaxY) {
  				PointArray[j].x = Corners[i].x - DPosition.x;
  				PointArray[j++].y = Corners[i].y + DPosition.y;
  				PointArray[j].x = Corners[i].x - DPosition.x;
  				PointArray[j++].y = Corners[i].y - DPosition.y;
  			} else if (Corners[i].y == MinY) {
  				PointArray[j].x = Corners[i].x - DPosition.x;
  				PointArray[j++].y = Corners[i].y - DPosition.y;
  				PointArray[j].x = Corners[i].x + DPosition.x;
  				PointArray[j++].y = Corners[i].y - DPosition.y;
  			} else {
  				PointArray[j].x = Corners[i].x - DPosition.x;
  				PointArray[j++].y = Corners[i].y + DPosition.y;
  				PointArray[j].x = Corners[i].x - DPosition.x;
  				PointArray[j++].y = Corners[i].y - DPosition.y;
  			}
  		} else {
  			if (Corners[i].y == MaxY) {
  				PointArray[j].x = Corners[i].x + DPosition.x;
  				PointArray[j++].y = Corners[i].y + DPosition.y;
  				PointArray[j].x = Corners[i].x - DPosition.x;
  				PointArray[j++].y = Corners[i].y + DPosition.y;
  			} else if (Corners[i].y == MinY) {
  				PointArray[j].x = Corners[i].x - DPosition.x;
  				PointArray[j++].y = Corners[i].y - DPosition.y;
  				PointArray[j].x = Corners[i].x + DPosition.x;
  				PointArray[j++].y = Corners[i].y - DPosition.y;
  			}
  		}
  	}
  	if (j != 8) {
  		AfxMessageBox("Error Octagon Error!!",MB_OK,0); 
  		ASSERT(FALSE);
  	}
  //	CRect ClientRect;
  //	GetClientRect(&ClientRect);
  //	ClientRect.left = ClientRect.Width()/2;
  //	ClientRect.top = ClientRect.Height()/2;
  //	POINT WindowPoint[8] = {0,0,100,0,200,0,200,100,100,100,100,200,0,200,0,0};
  	POINT WindowPoint[9];
  	for (j = 0; j < 8; j++) {
  		WindowPoint[j] = m_ProbabilityGrid->RobotToClientCoords(PointArray[j]);
  //		PointArray[j].x *= 4.0 / cWorldGridPointSize;
  //		WindowPoint[j].x = Round(PointArray[j].x + ClientRect.left);
  //		PointArray[j].y *= 4.0 / cWorldGridPointSize;
  //		WindowPoint[j].y = Round(ClientRect.top - PointArray[j].y);
  	}
  	WindowPoint[8] = WindowPoint[0];
  	CGdiObject* OldObject = dc->SelectStockObject(BLACK_PEN);
  //	dc->Polygon(WindowPoint, 8);
  	dc->Polyline(WindowPoint, 9);
  	dc->SelectObject(OldObject);
  
  	return;
  }
  
  void CRobotWorldView::OnDraw(CDC* pDC)
  {
  	CRect UpdateRect;
  //	const COLORREF BungeePenColor = 0xB3B3B3;
  //	const COLORREF BungeePenColor = 0xB2B2B2;
  //	const COLORREF BungeePenColor = 0xACACAC;
  	const COLORREF BungeePenColor = 0x4D4D4D;
  	const COLORREF cRobotPathPenColor = 0xFFFFFF;
  	short HandleLength;
  	CRobotWorldDoc* pDoc = GetDocument();
  	ASSERT_VALID(pDoc);
  /*Erase Old Bungee*/
  	robPOINT RobotPoint;
  	CDC* pWinDC = GetDC();
  	CPen HandlePen(PS_SOLID, 1, BungeePenColor);
  	CPen BungeePen(PS_SOLID, 1, BungeePenColor);
  	CPen* OldWinDCPen = pWinDC->SelectObject(&HandlePen);
  	int OldROP2 = pWinDC->SetROP2(R2_XORPEN);
  	if ( m_OldBungeeOn && (GetActiveRobot() != NULL) ) {
  	/*Calculate positions of the stick part of the handle*/
  		pWinDC->MoveTo(RobotToClientCoords(m_OldBungeeOrigin));
  		HandleLength = GetActiveRobot()->GetHandleLength();
  		RobotPoint.x = m_OldBungeeOrigin.x + HandleLength * cos(m_OldBungeeHeading);
  		RobotPoint.y = m_OldBungeeOrigin.y + HandleLength * sin(m_OldBungeeHeading);
  		pWinDC->LineTo(RobotToClientCoords(RobotPoint));
  	/*Calculate position of the bungee*/
  //		pWinDC->SetROP2(R2_XORPEN);
  		pWinDC->SelectObject(&BungeePen);
  		pWinDC->LineTo(RobotToClientCoords(m_OldBungeeDestination));
  	}
  
  	{
  /*Erase old FindPathShown*/
  		CPen PathPen(PS_SOLID, 1, cFindPathPenColor);
  		pWinDC->SetROP2(R2_XORPEN);
  		CPen* OldDCPen = pWinDC->SelectObject(&PathPen);
  		DrawFoundPath(pWinDC, m_FindPathShown);
  		pWinDC->SelectObject(OldDCPen);
  		PathPen.DeleteObject();
  	}
  /*Erase old Robot Path*/
  	if (m_OldRobotCTDPath != NULL) {
  		CPen PathPen(PS_SOLID, 1, cRobotPathPenColor);
  		pWinDC->SetROP2(R2_XORPEN);
  		CPen* OldDCPen = pWinDC->SelectObject(&PathPen);
  		DrawCTDPath(pWinDC, m_OldRobotCTDPath, true);
  		pWinDC->SelectObject(OldDCPen);
  		PathPen.DeleteObject();
  	}
  
  /*Draw Robot World*/
  	pDC->GetClipBox(&UpdateRect);
  	
  	RECT OriginalUpdateRect;
  	OriginalUpdateRect.top = UpdateRect.top;
  	OriginalUpdateRect.left = UpdateRect.left;
  	OriginalUpdateRect.bottom = UpdateRect.bottom;
  	OriginalUpdateRect.right = UpdateRect.right;
  
  //	robPOINT RobotPosition;
  //	RobotPosition.x = GetActiveRobot()->m_Position.x;
  //	RobotPosition.y = GetActiveRobot()->m_Position.y;
  //	m_ProbabilityGrid->TransferWorldToDisplay(&UpdateRect, RobotPosition, GetActiveRobot()->m_Heading, this);
  	m_ProbabilityGrid->TransferWorldToDisplay(&UpdateRect, this);
  
  //	CRgn ClipRgn;
  //	ClipRgn.CreateRectRgn(UpdateRect.left, UpdateRect.top, UpdateRect.right, UpdateRect.bottom);
  //	pDC->SelectClipRgn(&ClipRgn, DCB_ACCUMULATE);
  //	pDC->SetBoundsRect(UpdateRect, DCB_ACCUMULATE | DCB_ENABLE);
  /*Draw Drag Rect*/
  	static BOOL OldDrag = FALSE;
  	CBrush DragRectBrush(COLORREF(0x0));
  	if (m_DraggingRect) {
  		pWinDC->SetROP2(R2_COPYPEN);
  		pWinDC->FrameRect(m_DragRect, &DragRectBrush);
  		m_OldDragRect = m_DragRect;
  	}
  	if (m_DragLine) {
  		CPen LinePen;
  		LinePen.CreatePen(PS_SOLID, m_DragLineWidth, COLORREF(0x0));
  		CPen* OldPen = pWinDC->SelectObject(&LinePen);
  		pWinDC->SetROP2(R2_COPYPEN);
  		pWinDC->MoveTo(m_DragPoint1);
  		pWinDC->LineTo(m_DragPoint2);
  		m_OldDragRect.TopLeft() = m_DragPoint1;
  		m_OldDragRect.BottomRight() = m_DragPoint2;
  		m_OldDragRect.NormalizeRect();
  		m_OldDragRect.InflateRect(m_DragLineWidth, m_DragLineWidth);
  		pWinDC->SelectObject(OldPen);
  	}
  /*Draw the bungee*/
  	CRobot* Robot = GetActiveRobot();
  	if (Robot != NULL) {
  		if (Robot->GetState() == stDragging) {
  			pWinDC->SelectObject(&HandlePen);
  		/*Calculate positions of the stick part of the handle*/
  			RobotPoint = Robot->GetDrawPosition();
  			pWinDC->MoveTo(RobotToClientCoords(RobotPoint));
  			HandleLength = Robot->GetHandleLength();
  			RobotPoint.x = RobotPoint.x + HandleLength * cos(Robot->GetDrawHeading());
  			RobotPoint.y = RobotPoint.y + HandleLength * sin(Robot->GetDrawHeading());
  			pWinDC->SetROP2(R2_XORPEN);
  			pWinDC->LineTo(RobotToClientCoords(RobotPoint));
  		/*Calculate position of the bungee*/
  			RobotPoint.x = Robot->m_Destination.x;
  			RobotPoint.y = Robot->m_Destination.y;
  			pWinDC->SetROP2(R2_XORPEN);
  			pWinDC->SelectObject(&BungeePen);
  			pWinDC->LineTo(RobotToClientCoords(RobotPoint));
  			m_OldBungeeOrigin = Robot->GetDrawPosition();
  			m_OldBungeeHeading = Robot->GetDrawHeading();
  			m_OldBungeeDestination = Robot->m_Destination;
  			m_OldBungeeOn = true;
  		} else {
  			m_OldBungeeOn = false;
  		}
  	}
  /*Draw robot error*/
  //	SketchRobotError(GetActiveRobot()->GetCorners(), GetActiveRobot()->GetPositionError(), pDC);
  
  /*Draw tracker rectangle if necessary*/
  /*	if (m_SelectedGraphicObject != NULL) {
  		CRectTracker tracker;
  		SetupTracker(&tracker);
  		tracker.Draw(pDC);
  	}
  */
  /*Draw robot's path*/
  //	CInstructionListNode* CurrentInstruction = GetActiveRobot()->GetCurrentInstruction();
  //	if (CurrentInstruction != NULL) {
  //		if (CurrentInstruction->InstructionNodeType == inCTDPathNode) {
  	m_CurrentRobotCTDPath = NULL;
  	if (m_CurrentRobotCTDPath != NULL) {
  		CPen PathPen(PS_SOLID, 1, cRobotPathPenColor);
  		pWinDC->SetROP2(R2_XORPEN);
  		CPen* OldDCPen = pWinDC->SelectObject(&PathPen);
  		DrawCTDPath(pWinDC, m_CurrentRobotCTDPath, true);
  		pWinDC->SelectObject(OldDCPen);
  		PathPen.DeleteObject();
  		if (m_OldRobotCTDPath != NULL) {
  			if (m_OldRobotCTDPath != m_CurrentRobotCTDPath) {
  				delete m_OldRobotCTDPath;
  				m_OldRobotCTDPath = new CCTDPath(*m_CurrentRobotCTDPath);
  			}
  		} else {
  			m_OldRobotCTDPath = new CCTDPath(*m_CurrentRobotCTDPath);
  		}
  	} else {
  		if (m_OldRobotCTDPath != NULL) {
  			delete m_OldRobotCTDPath;
  			m_OldRobotCTDPath = NULL;
  		}
  	}
  /*Draw the find path*/
  	{
  		CPen PathPen(PS_SOLID, 1, cFindPathPenColor);
  		pWinDC->SetROP2(R2_XORPEN);
  		CPen* OldDCPen = pWinDC->SelectObject(&PathPen);
  		DrawFoundPath(pWinDC, m_FindPathShown);
  		pWinDC->SelectObject(OldDCPen);
  		PathPen.DeleteObject();
  	}
  //	OnSetCursor(this,0,0);
  //#define DRAW_UPDATE_RECT
  #ifdef DRAW_UPDATE_RECT
  	CBrush UpdateRectBrush(COLORREF(0xFF));
  	pDC->FrameRect(UpdateRect, &UpdateRectBrush);
  	CBrush OriginalUpdateRectBrush(COLORREF(0xFF00));
  	pDC->FrameRect(&OriginalUpdateRect, &OriginalUpdateRectBrush);
  #endif
  
  #ifdef _DEBUG
  	RECT DestRect;
  	CPoint ClientDest = RobotToClientCoords(m_RobotDestination);
  	DestRect.left = ClientDest.x - 2;
  	DestRect.right = ClientDest.x + 2;
  	DestRect.top = ClientDest.y - 2;
  	DestRect.bottom = ClientDest.y + 2;
  	if (m_FollowingPath) {
  		pDC->FillSolidRect(&DestRect, COLORREF(0xFF00));
  	} else {
  		pDC->FillSolidRect(&DestRect, COLORREF(0xFF));
  	}
  #endif
  
  	pWinDC->SetROP2(OldROP2);
  	pWinDC->SelectObject(OldWinDCPen);
  	HandlePen.DeleteObject();
  	BungeePen.DeleteObject();
  	DragRectBrush.DeleteObject();
  //	ClipRgn.DeleteObject();
  	ReleaseDC(pWinDC);
  
  	static bool FirstDraw = true;
  	if (FirstDraw) {
  		if (GetActiveRobot() != NULL) {
  			FirstDraw = false;
  #ifndef _DEBUG
  			if ( (!GetEnableLaygo()) || (m_DlgLaygo->GetCurrentState() > CLaygo::lgoComPortSearch) ) {
  				GetActiveRobot()->FindComPort();
  			}
  #endif
  		}
  	}
  
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // CRobotWorldView printing
  
  BOOL CRobotWorldView::OnPreparePrinting(CPrintInfo* pInfo)
  {
  	// default preparation
  	return DoPreparePrinting(pInfo);
  }
  
  void CRobotWorldView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
  {
  	// TODO: add extra initialization before printing
  }
  
  void CRobotWorldView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
  {
  	// TODO: add cleanup after printing
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // CRobotWorldView diagnostics
  
  #ifdef _DEBUG
  void CRobotWorldView::AssertValid() const
  {
  	CView::AssertValid();
  }
  
  void CRobotWorldView::Dump(CDumpContext& dc) const
  {
  	CView::Dump(dc);
  }
  
  CRobotWorldDoc* CRobotWorldView::GetDocument() // non-debug version is inline
  {
  	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRobotWorldDoc)));
  	return (CRobotWorldDoc*)m_pDocument;
  }
  #endif //_DEBUG
  
  /////////////////////////////////////////////////////////////////////////////
  // CRobotWorldView message handlers
  
  void CRobotWorldView::OnGoHome() 
  /*******************************************************************************
  //Created: 07/31/96 S.R.
  //Last Revision: 10/29/96 S.R.
  //Parameters:
  //	none
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //	Sends the robot home.
  //Method:
  //		The function first stops the motors then purges the robot's path.  Then
  //	the ObstacleSquaring is reset and the position of home base is put on the
  //	robot's path.
  *******************************************************************************/
  {
  /*Stop the motors in case the robot is squaring to a wall or something strange*/
  	CRobot* Robot;
  	if ( (Robot = GetActiveRobot()) != NULL ) {
  		Robot->EStop();
  		robPOINT HomePoint(0,0);
  		Robot->AddPositionVelocityToInstructions(HomePoint, NULL, 100, 30, FALSE, 0);
  		Robot->FollowInstructions(TRUE, FALSE);
  	}
  }
  
  BOOL CRobotWorldView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
  /*******************************************************************************
  //Created: 07/31/96 S.R.
  //Last Revision: 10/30/97 S.R.
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
  {
  /*Create the window*/
  	BOOL Error = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
  	if (Error == 0) {
  		TRACE0("Failed to create the view\n");
  		return Error;
  	}
  	ModifyStyle(0,WS_CLIPCHILDREN);
  //Add Scroll Bars
  	ModifyStyle(0,WS_VSCROLL | WS_HSCROLL,0);
  	SetScrollBars();
  
  //	m_LabelList = new CLabelList(this);
  //Create the probability grid
  	m_ProbabilityGrid = new CProbabilityGrid(GetParentFrame()->GetSafeHwnd(), this);
  	m_ProbabilityGrid->m_ZoomLevel = 4.0;
  	m_ProbabilityGrid->m_RWGraphicObjectList->m_RobotCurrentPath->m_ParentView = this;
  	m_RedZoneList = new CRedZoneList(m_ProbabilityGrid);
  //Set the serial output timer
  	if (SetTimer(SER_TIMER, SER_OUT_INTERVAL, NULL) == 0) {
  		AfxMessageBox("No Serial Timer Resource");
  	}
  //Set the calibration timer
  	if (SetTimer(CAL_TIMER, CAL_INTERVAL, CalibrationTimerProc) == 0) {
  		AfxMessageBox("No Calibration Timer Resource");
  	}
  //Set the animation timer
  	if (SetTimer(ANIMATION_TIMER, ANIMATION_INTERVAL, NULL) == 0) {
  		AfxMessageBox("No Animation Timer Resource");
  	}
  //Initialize the joystick
  	JOYINFO joyinfo; 
  	UINT wNumDevs; 
  	BOOL bDev1Attached, bDev2Attached; 
   
  	if((wNumDevs = joyGetNumDevs()) == 0) { 
  //		AfxMessageBox("ERR_NODRIVER");
  	} else {
  		bDev1Attached = joyGetPos(JOYSTICKID1,&joyinfo) != JOYERR_UNPLUGGED;
  		bDev2Attached = wNumDevs == 2 && joyGetPos(JOYSTICKID2,&joyinfo) != JOYERR_UNPLUGGED;
  		if(bDev1Attached || bDev2Attached) {	// decide which joystick to use
  			JoystickDeviceID = bDev1Attached ? JOYSTICKID1 : JOYSTICKID2;
  			if (SetTimer( ID_JOYSTICK_TIMER, JOYSTICK_TIMER_INTERVAL, JoystickCaptureTimer ) == 0) {
  				AfxMessageBox("No Joystick Timer Resources Left");
  			}
  		} else {
  //			AfxMessageBox("ERR_NODEVICE");
  		}
  	}
  //Create the battery low dialog
  	m_DlgBatteryLow.Create(IDD_BATTERY_LOW, this);
  
  //Create the motor controls dialog
  	m_DlgMotorControls.Create(IDD_MOTORCONTROLS, this);
  	m_DlgMotorControls.m_Robot = GetActiveRobot();
  	m_DlgMotorControls.ShowWindow(SW_HIDE);
  	m_DlgMotorControls.m_CalibrationZoneHandler = m_ProbabilityGrid->m_ExecutableObjectList;
  	m_DlgMotorControls.m_ParentView = this;
  //Create the Position Error dialog
  //	m_DlgPositionError.Create(IDB_ROBOT3, IDD_POSITIONERROR, this, GetActiveRobot());
  //	m_DlgPositionError.ShowWindow(SW_SHOW);
  //Create the joystick controls dialog
  //	m_DlgJoystickControl.Create(IDD_JOYSTICKCONTROL, this);
  //Create the Vacuum Patch dialog
  //	m_DlgCreateVacuumPatch.Create(IDD_CREATEVACUUMPATCH, this);
  //Initialize the output file variables
  //	m_OutFile = fopen("out.dat","w");
  
  //	m_AllDataFile = fopen("alldata.dat","w");
  //	fprintf(m_AllDataFile,"     X        Y Heading  M0E  M1E  M0V  M1V M0AD M1AD\n");
  //Create the LAYGo window
  	m_DlgLaygo = new CLaygo();
  	m_DlgLaygo->SetRobotWorldView(this);
  	m_DlgLaygo->SetRobotBitmap(m_ProbabilityGrid->GetLargeRobotBitmap());
  	m_DlgLaygo->Create(IDD_LAYGO, this);
  	POINT TopLeft;
  	TopLeft.x = 0;
  	TopLeft.y = 0;
  //	ClientToScreen(&TopLeft);
  	m_DlgLaygo->SetWindowPos(NULL, TopLeft.x, TopLeft.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  //Read the .ini file
  	m_DlgLaygo->LoadState();
  	if ( m_DlgLaygo->GetCurrentState() <= 1) {
  		if (GetEnableLaygo()) {
  			ShowLaygoTip(1, true);
  		}
  	}
  
  /*Create example vacuum patches*/
  /*	robRECT PatchBounds;
  	TEnterSide EnterSide;
  	short PathWidth = 75;
  		const unsigned short cWidth = 800;
  		const unsigned short cHeight = 700;
  		const unsigned short cTurnDiameter = 220;
  //		PatchBounds.left = 0;
  //		PatchBounds.top = 0;
  //		PatchBounds.right = PatchBounds.left + cWidth;
  //		PatchBounds.bottom = PatchBounds.top - cHeight;
  
  		PatchBounds.right = 0+490+220/2+cRRWheelBase/2.0;
  		PatchBounds.bottom = -cRRWheelBase/2.0;
  		PatchBounds.left = -220/2-cRRWheelBase/2.0;
  		PatchBounds.top = PatchBounds.bottom + cHeight;
  		EnterSide = esLftBot;
  		CVacuumPatch* NewPatch = new CVacuumPatch(PatchBounds, EnterSide, PathWidth, cTurnDiameter, m_Robot);
  		m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewPatch);
  
  		PatchBounds.left += cWidth + 100;
  		PatchBounds.top = 0;
  		PatchBounds.right = PatchBounds.left + cWidth;
  		PatchBounds.bottom = PatchBounds.top - cHeight;
  		EnterSide = esBotLft;
  		NewPatch = new CVacuumPatch(PatchBounds, EnterSide, PathWidth, cTurnDiameter, m_Robot);
  		m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewPatch);
  
  		PatchBounds.left += cWidth + 100;
  		PatchBounds.top = 0;
  		PatchBounds.right = PatchBounds.left + cWidth;
  		PatchBounds.bottom = PatchBounds.top - cHeight;
  		EnterSide = esRgtBot;
  		NewPatch = new CVacuumPatch(PatchBounds, EnterSide, PathWidth, cTurnDiameter, m_Robot);
  		m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewPatch);
  
  		PatchBounds.left += cWidth + 100;
  		PatchBounds.top = 0;
  		PatchBounds.right = PatchBounds.left + cWidth;
  		PatchBounds.bottom = PatchBounds.top - cHeight;
  		EnterSide = esTopLft;
  		NewPatch = new CVacuumPatch(PatchBounds, EnterSide, PathWidth, cTurnDiameter, m_Robot);
  		m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewPatch);
  
  		PatchBounds.left = 0;
  		PatchBounds.top = -(cHeight + 100);
  		PatchBounds.right = PatchBounds.left + cWidth;
  		PatchBounds.bottom = PatchBounds.top - cHeight;
  		EnterSide = esLftTop;
  		NewPatch = new CVacuumPatch(PatchBounds, EnterSide, PathWidth, cTurnDiameter, m_Robot);
  		m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewPatch);
  
  		PatchBounds.left += cWidth + 100;
  		PatchBounds.top = -(cHeight + 100);
  		PatchBounds.right = PatchBounds.left + cWidth;
  		PatchBounds.bottom = PatchBounds.top - cHeight;
  		EnterSide = esBotRgt;
  		NewPatch = new CVacuumPatch(PatchBounds, EnterSide, PathWidth, cTurnDiameter, m_Robot);
  		m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewPatch);
  
  		PatchBounds.left += cWidth + 100;
  		PatchBounds.top = -(cHeight + 100);
  		PatchBounds.right = PatchBounds.left + cWidth;
  		PatchBounds.bottom = PatchBounds.top - cHeight;
  		EnterSide = esRgtTop;
  		NewPatch = new CVacuumPatch(PatchBounds, EnterSide, PathWidth, cTurnDiameter, m_Robot);
  		m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewPatch);
  
  		PatchBounds.left += cWidth + 100;
  		PatchBounds.top = -(cHeight + 100);
  		PatchBounds.right = PatchBounds.left + cWidth;
  		PatchBounds.bottom = PatchBounds.top - cHeight;
  		EnterSide = esTopRgt;
  		NewPatch = new CVacuumPatch(PatchBounds, EnterSide, PathWidth, cTurnDiameter, m_Robot);
  		m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewPatch);
  */	
  //	SetShowVelocityTorqueDisplay(true);
  	return TRUE;
  }
  
  BOOL CRobotWorldView::DestroyWindow() 
  /*******************************************************************************
  //Created: 07/31/96 S.R.
  //Last Revision: 08/30/96 S.R.
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
  {
  	return CView::DestroyWindow();
  }
  
  void CRobotWorldView::OnTimer(UINT nIDEvent) 
  /*******************************************************************************
  //Created: 04/16/96 S.R.
  //Last Revision: 12/19/96 S.R.
  //Parameters:
  //	nIDEvent		The ID of the timer event.
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	static robPOINT OldDestination(0,0);
  	switch (nIDEvent) {
  	case SER_TIMER:
  		/*Send out new destination if dragging the robot*/
  		CRobot* Robot;
  		Robot = GetActiveRobot();
  		if (Robot != NULL) {
  			if (Robot->GetState() == stDragging) {
  				if ((OldDestination.x != Robot->m_Destination.x) || (OldDestination.y != Robot->m_Destination.y)) {
  	//				GetActiveRobot()->SetPositionPwmDestination(GetActiveRobot()->m_Destination);
  					Robot->SetPositionVelocityDestination(Robot->m_Destination, Robot->GetMaxSpeed());
  					OldDestination = Robot->m_Destination;
  				}
  			}
  		}
  		break;
  	case ANIMATION_TIMER:
  		m_ProbabilityGrid->AnimateRobots();
  		m_ProbabilityGrid->m_RWGraphicObjectList->UpdateFinishLineTimers();
  //		m_ProbabilityGrid->m_RWGraphicObjectList->AnimateObjects();
  /*		if (GetCurrentRWGraphicObject() != NULL) {
  			if (GetCurrentRWGraphicObject()->GetObjectType() == goPosition) {
  				GetCurrentRWGraphicObject()->Animate();
  			}
  		}
  */	}
  	
  	CView::OnTimer(nIDEvent);
  }
  
  void CRobotWorldView::PlaceNewLabel(POINT WorldPoint)
  /*******************************************************************************
  //Created: 08/02/96 S.R.
  //Last Revision: 10/10/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  }
  
  
  void CRobotWorldView::OnSize(UINT nType, int cx, int cy) 
  /*******************************************************************************
  //Created: 08/06/96 S.R.
  //Last Revision: 08/09/96 S.R.
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
  {
  	CView::OnSize(nType, cx, cy);
  	SetScrollBars();
  }
  
  void CRobotWorldView::ObstacleHandler(short RobotNumber, short M0, short M1)
  /*******************************************************************************
  //Created: 08/15/96 S.R.
  //Last Revision: 10/01/96 S.R.
  //Parameters:
  //
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	short M0V, M1V;
  	CRobot* Robot = GetRobot(RobotNumber);
  	ASSERT(Robot != NULL);
  	if (Robot == NULL) return;
  
  	Robot->GetVelocities(M0V, M1V);
  	if ( (abs(M0V) >= 200) || (abs(M1V) >= 200) ) {
  		CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  		SoundPath += "/sounds/RobCollision.wav";
  		PlaySound (SoundPath, NULL, SND_FILENAME | SND_ASYNC ); 
  	}
  	if (Robot->GetState() == stDragging) {
  		Robot->SetDrag(FALSE, this);
  	}
  /*Add to heading error on collision*/
  	if (M0 || M1) {
  		double HeadingError = Robot->GetHeadingError();
  		HeadingError += 0.00085;
  //		if (!(M0 && M1)) {
  //		/*Collision on one side or the other*/
  //			if (M0) {
  //				if (M0V > 250) {
  //					HeadingError += 0.0017;
  //				} else {
  //					HeadingError += 0.0087;
  //				}
  //			} else {
  //				if (M1V > 250) {
  //					HeadingError += 0.0175;
  //				} else {
  //					HeadingError += 0.0087;
  //				}
  //			}
  //		} else {
  //		/*Collision on both wheels*/
  //			HeadingError += 0.0183;
  //		}
  		Robot->SetHeadingError(HeadingError);
  	}
  //	Beep(1000, 500);
  	m_DlgMotorControls.UpdateData(TRUE);
  	short temp = abs(M0);
  	switch (temp) {
  	case 0:
  		m_DlgMotorControls.m_edtM0Obstacle = "none";
  		break;
  	case 1:
  		m_DlgMotorControls.m_edtM0Obstacle = "Stuck";
  		break;
  	case 2:
  		m_DlgMotorControls.m_edtM0Obstacle = "Decel";
  		break;
  	case 3:
  		m_DlgMotorControls.m_edtM0Obstacle = "AD";
  		break;
  	case 4:
  		m_DlgMotorControls.m_edtM0Obstacle = "OvrCur";
  		break;
  	case 5:
  		m_DlgMotorControls.m_edtM0Obstacle = "LrgDec";
  		break;
  	default:
  		char string[5];
  		gcvt(temp, 4, string);
  		m_DlgMotorControls.m_edtM0Obstacle = string;
  		break;
  	}
  	temp = abs(M1);
  	switch (temp) {
  	case 0:
  		m_DlgMotorControls.m_edtM1Obstacle = "none";
  		break;
  	case 1:
  		m_DlgMotorControls.m_edtM1Obstacle = "Stuck";
  		break;
  	case 2:
  		m_DlgMotorControls.m_edtM1Obstacle = "Decel";
  		break;
  	case 3:
  		m_DlgMotorControls.m_edtM1Obstacle = "AD";
  		break;
  	case 4:
  		m_DlgMotorControls.m_edtM1Obstacle = "OvrCur";
  		break;
  	case 5:
  		m_DlgMotorControls.m_edtM1Obstacle = "LrgDec";
  		break;
  	default:
  		char string[5];
  		gcvt(temp, 4, string);
  		m_DlgMotorControls.m_edtM1Obstacle = string;
  		break;
  	}
  
  //	m_DlgMotorControls.m_edtM0Obstacle = M0;
  //	m_DlgMotorControls.m_edtM1Obstacle = M1;
  	RoverGrid ObstacleGrid;
  	bool MarkObstacle = false;
  	if ( (M0 < 0) && (M1 == 0) ) {
  		MarkObstacle = false;
  		Robot->m_MotorObstacle.MotorObstacle = M0Back;
  		m_DlgMotorControls.m_edtObstacle = "M0 Back";
  	} else if ( (M0 > 0) && (M1 == 0) ) {
  		Robot->m_MotorObstacle.MotorObstacle = M0Front;
  		m_DlgMotorControls.m_edtObstacle = "M0 Front";
  		if (Robot->GetLearnRoom()) {
  			MarkObstacle = true;
  			ObstacleGrid = M0_F;
  		}
  	} else if ( (M0 == 0) && (M1 < 0) ) {
  		MarkObstacle = false;
  		Robot->m_MotorObstacle.MotorObstacle = M1Back;
  		m_DlgMotorControls.m_edtObstacle = "M1Back";
  	} else if ( (M0 == 0) && (M1 > 0) ) {
  		Robot->m_MotorObstacle.MotorObstacle = M1Front;
  		m_DlgMotorControls.m_edtObstacle = "M1 Front";
  		if (Robot->GetLearnRoom()) {
  			MarkObstacle = true;
  			ObstacleGrid = M1_F;
  		}
  	} else if ( (M0 < 0) && (M1 < 0) ) {
  		Robot->m_MotorObstacle.MotorObstacle = M0Back_M1Back;
  		m_DlgMotorControls.m_edtObstacle = "M0 Back M1 Back";
  		if (Robot->GetLearnRoom()) {
  			MarkObstacle = true;
  			ObstacleGrid = M2_B;
  		}
  	} else if ( (M0 > 0) && (M1 < 0) ) {
  		MarkObstacle = false;
  		Robot->m_MotorObstacle.MotorObstacle = M0Front_M1Back;
  		m_DlgMotorControls.m_edtObstacle = "M0 Front M1 Back";
  	} else if ( (M0 < 0) && (M1 > 0) ) {
  		MarkObstacle = false;
  		Robot->m_MotorObstacle.MotorObstacle = M0Back_M1Front;
  		m_DlgMotorControls.m_edtObstacle = "M0 Back M1 Front";
  	} else if ( (M0 > 0) && (M1 > 0) ) {
  		Robot->m_MotorObstacle.MotorObstacle = M0Front_M1Front;
  		m_DlgMotorControls.m_edtObstacle = "M0 Front M1 Front";
  		if (Robot->GetLearnRoom()) {
  			MarkObstacle = true;
  			ObstacleGrid = M2_F;
  		}
  	} else {
  //		fprintf(stderr, "Unknown Obstacle\n");
  		ASSERT(FALSE);
  		MarkObstacle = false;
  		ObstacleGrid = M2_F;
  		m_DlgMotorControls.m_edtObstacle = "Unknown Obstacle";
  	}
  /*Reposition on an unexpected obstacle*/
  	if (Robot->GetEnableRepositionOnUnknownObstacle()) {
  		if (Robot->RepositionRobotOnUnexpectedObstacle(m_ProbabilityGrid, Robot->m_MotorObstacle.MotorObstacle)) {
  		/*Don't mark the obstacle when the robot is relocated*/
  			MarkObstacle = false;
  		}
  	}
  
  	Robot->m_MotorObstacle.ValidObstacle = TRUE;
  	Robot->m_MotorObstacle.Heading = Robot->m_Heading;
  	Robot->m_MotorObstacle.Position = Robot->m_Position;
  
  	if (MarkObstacle) {
  		m_ProbabilityGrid->m_RobotWorld->ObstacleRoverPosition(Robot->m_Position.x,
  			Robot->m_Position.y, Robot->m_Heading,
  			Robot->m_PositionError.x, Robot->m_PositionError.y,
  			Robot->m_HeadingError, ObstacleGrid);
  	}
  	m_DlgMotorControls.UpdateData(FALSE);
  }
  
  void CRobotWorldView::OnStop() 
  /*******************************************************************************
  //Created: 08/19/96 S.R.
  //Last Revision: 02/27/97 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  /*Get rid of the search area if it's there*/
  	ShrinkSearchAreaToZero();
  /*EStop the robot*/
  	if (GetActiveRobot() != NULL) GetActiveRobot()->EStop();
  	RedrawWindow(NULL, NULL, RDW_NOERASE | RDW_INVALIDATE);
  }
  
  void CRobotWorldView::OnSearchArea() 
  /*******************************************************************************
  //Created: 08/20/96 S.R.
  //Last Revision: 09/13/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	SetMouseMode(mmSearchArea);
  //	GetActiveRobot()->SetSearchAlgorithm(SearchArea);
  }
  
  void CRobotWorldView::ShrinkSearchAreaToZero(void)
  /*******************************************************************************
  //Created: 08/21/96 S.R.
  //Last Revision: 08/21/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  //	long points[8];
  //	points[0] = points[2] = points[4] = points[6] = m_RobotWorldWindow->m_Origin.x;
  //	points[1] = points[3] = points[5] = points[7] = m_RobotWorldWindow->m_Origin.y;
  
  }
  
  void CRobotWorldView::StartSearchArea(void)
  /*******************************************************************************
  //Created: 08/21/96 S.R.
  //Last Revision: 09/12/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	double distance1, distance2;
  	unsigned char NearCorner;
  	CRobot* Robot = GetActiveRobot();
  	if (Robot == NULL) return;
  	short HandleLength = Robot->GetHandleLength();
  /*get the four corners of the search area*/
  	long SearchAreaCorners[8];
  	short nparams = 4;
  
  	OrderSearchAreaCorners(SearchAreaCorners);
  /*Find which corner the robot is closest to*/
  	distance1 = sqrt((double)SQR(SearchAreaCorners[0] - Robot->m_Position.x) + SQR(SearchAreaCorners[1] - Robot->m_Position.y));
  	NearCorner = 0;
  	for (unsigned char i = 1; i < 4; i++) {
  		distance2 = sqrt((double)SQR(SearchAreaCorners[2*i] - Robot->m_Position.x) + SQR(SearchAreaCorners[2*i+1] - Robot->m_Position.y));
  		if (distance2 < distance1) {
  			distance1 = distance2;
  			NearCorner = i;
  		}
  	}
  /*Figure out how to approach the corner*/
  	robPOINT Destination0;
  	if ( ABS(SearchAreaCorners[0] - SearchAreaCorners[2]) >= ABS(SearchAreaCorners[1] - SearchAreaCorners[7]) ) {
  	/*Search Area Longer in X direction*/
  		long YSearchLevel;
  	/*Get the X coordinate*/
  		if ( (NearCorner == 0) || (NearCorner == 3) ) {
  			Destination0.x = SearchAreaCorners[0] - HandleLength;
  		} else {
  			Destination0.x = SearchAreaCorners[2] + HandleLength;
  		}
  	/*Get the Y coordinate*/
  		if ( (NearCorner == 0) || (NearCorner == 1) ) {
  			Destination0.y = SearchAreaCorners[1] + cRRWidth/2;
  			YSearchLevel = SearchAreaCorners[1];
  		} else {
  			Destination0.y = SearchAreaCorners[7] - cRRWidth/2;
  			YSearchLevel = SearchAreaCorners[7];
  		}
  		Robot->AddPositionBungeeToInstructions(Destination0,NULL,10,FALSE,0);
  	/*Make the rest of the search path*/
  		MakeXSearchPath(SearchAreaCorners, NearCorner, YSearchLevel);
  	} else {
  	/*Search Area longer in Y direction*/
  	/*Get the Y coordinates*/
  		long XSearchLevel;
  		if ( (NearCorner == 0) || (NearCorner == 1) ) {
  			Destination0.y = SearchAreaCorners[1] - HandleLength;
  		} else {
  			Destination0.y = SearchAreaCorners[5] + HandleLength;
  		}
  	/*Get the X coordinates*/
  		if ( (NearCorner == 0) || (NearCorner == 3) ) {
  			Destination0.x = SearchAreaCorners[0] + cRRWidth/2;
  			XSearchLevel = SearchAreaCorners[0];
  		} else {
  			Destination0.x = SearchAreaCorners[2] - cRRWidth/2;
  			XSearchLevel = SearchAreaCorners[2];
  		}
  		Robot->AddPositionBungeeToInstructions(Destination0,NULL,10,FALSE,0);
  		MakeYSearchPath(SearchAreaCorners, NearCorner, XSearchLevel);
  	}
  }
  
  void CRobotWorldView::MakeXSearchPath(long* SearchAreaCorners, unsigned char NearCorner, long YSearchLevel)
  /*******************************************************************************
  //Created: 08/22/96 S.R.
  //Last Revision: 09/12/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	CRobot* Robot = GetActiveRobot();
  	if (Robot == NULL) return;
  	short HandleLength = Robot->GetHandleLength();
  	robPOINT Destination0, Destination1, Destination2, Destination3;
  	while ( (YSearchLevel - SearchAreaCorners[1]) * (YSearchLevel - SearchAreaCorners[7]) <= 0 ) {
  	/*Get the X coordinates*/
  		if ( (NearCorner == 0) || (NearCorner == 3) ) {
  			Destination0.x = SearchAreaCorners[0];
  			Destination1.x = Destination0.x + (SearchAreaCorners[2] - SearchAreaCorners[0]) / 3;
  			Destination2.x = Destination0.x + (SearchAreaCorners[2] - SearchAreaCorners[0]) * 2 / 3;
  			Destination3.x = SearchAreaCorners[2] + HandleLength;
  		} else {
  			Destination0.x = SearchAreaCorners[2];
  			Destination1.x = Destination0.x - (SearchAreaCorners[2] - SearchAreaCorners[0]) / 3;
  			Destination2.x = Destination0.x - (SearchAreaCorners[2] - SearchAreaCorners[0]) * 2 / 3;
  			Destination3.x = SearchAreaCorners[0] - HandleLength;
  		}
  	/*Get the Y coordinates*/
  		if ( (NearCorner == 0) || (NearCorner == 1) ) {
  			Destination0.y = YSearchLevel + cRRWidth/2 + HandleLength;
  			Destination1.y = YSearchLevel + cRRWidth/2;
  			Destination2.y = Destination1.y;
  			Destination3.y = Destination1.y;
  			YSearchLevel += Round(cRRWidth);
  		} else {
  			Destination0.y = YSearchLevel - cRRWidth/2 - HandleLength;
  			Destination1.y = YSearchLevel - cRRWidth/2;
  			Destination2.y = Destination1.y;
  			Destination3.y = Destination1.y;
  			YSearchLevel -= Round(cRRWidth);
  		}
  	/*Add to path*/
  		Robot->AddPositionBungeeToInstructions(Destination0,NULL,20,FALSE,0);
  		Robot->AddPositionBungeeToInstructions(Destination1,NULL,30,FALSE,0);
  		Robot->AddPositionBungeeToInstructions(Destination2,NULL,30,FALSE,0);
  		Robot->AddPositionBungeeToInstructions(Destination3,NULL,30,FALSE,0);
  	/*Do other corner*/
  		if ( (NearCorner == 1) || (NearCorner == 3) ) NearCorner -= 1;
  		else NearCorner += 1;
  	}
  }
  
  void CRobotWorldView::MakeYSearchPath(long* SearchAreaCorners, unsigned char NearCorner, long XSearchLevel)
  /*******************************************************************************
  //Created: 08/23/96 S.R.
  //Last Revision: 09/12/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	CRobot* Robot = GetActiveRobot();
  	if (Robot == NULL) return;
  	short HandleLength = Robot->GetHandleLength();
  	robPOINT Destination0, Destination1, Destination2, Destination3;
  	while ( (XSearchLevel - SearchAreaCorners[0]) * (XSearchLevel - SearchAreaCorners[2]) <= 0 ) {
  	/*Get the Y coordinates*/
  		if ( (NearCorner == 0) || (NearCorner == 1) ) {
  			Destination0.y = SearchAreaCorners[1];
  			Destination1.y = SearchAreaCorners[1] + (SearchAreaCorners[7] - SearchAreaCorners[1]) / 3;
  			Destination2.y = SearchAreaCorners[1] + (SearchAreaCorners[7] - SearchAreaCorners[1]) * 2 / 3;
  			Destination3.y = SearchAreaCorners[7] + HandleLength;
  		} else {
  			Destination0.y = SearchAreaCorners[7];
  			Destination1.y = SearchAreaCorners[7] - (SearchAreaCorners[7] - SearchAreaCorners[1]) / 3;
  			Destination2.y = SearchAreaCorners[7] - (SearchAreaCorners[7] - SearchAreaCorners[1]) * 2 / 3;
  			Destination3.y = SearchAreaCorners[1] - HandleLength;
  		}
  	/*Get the X coordinates*/
  		Destination1.x = XSearchLevel;
  		Destination2.x = XSearchLevel;
  		Destination3.x = XSearchLevel;
  		if ( (NearCorner == 0) || (NearCorner == 3) ) {
  			Destination0.x = XSearchLevel + cRRWidth/2 + HandleLength;
  			Destination1.x = XSearchLevel + cRRWidth/2;
  			Destination2.x = Destination1.x;
  			Destination3.x = Destination1.x;
  			XSearchLevel += Round(cRRWidth);
  		} else {
  			Destination0.x = XSearchLevel - cRRWidth/2 - HandleLength;
  			Destination1.x = XSearchLevel - cRRWidth/2;
  			Destination2.x = Destination1.x;
  			Destination3.x = Destination1.x;
  			XSearchLevel -= Round(cRRWidth);
  		}
  	/*Add to the path*/
  		Robot->AddPositionBungeeToInstructions(Destination0,NULL,20,FALSE,0);
  		Robot->AddPositionBungeeToInstructions(Destination1,NULL,30,FALSE,0);
  		Robot->AddPositionBungeeToInstructions(Destination2,NULL,30,FALSE,0);
  		Robot->AddPositionBungeeToInstructions(Destination3,NULL,30,FALSE,0);
  	/*Do other corner*/
  		switch (NearCorner) {
  		case (0):
  			NearCorner = 3;
  			break;
  		case (1):
  			NearCorner = 2;
  			break;
  		case (2):
  			NearCorner = 1;
  			break;
  		case (3):
  			NearCorner = 0;
  			break;
  		}
  	}
  }
  
  void CRobotWorldView::OrderSearchAreaCorners(long* SearchAreaCorners)
  /*******************************************************************************
  //Created: 08/23/96 S.R.
  //Last Revision: 08/23/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	long X1, X2, Y1, Y2;
  	X1 = SearchAreaCorners[0];
  	Y1 = SearchAreaCorners[1];
  	X2 = SearchAreaCorners[4];
  	Y2 = SearchAreaCorners[5];
  	if (X1 < X2) {
  		SearchAreaCorners[0] = X1;
  		SearchAreaCorners[2] = X2;
  		SearchAreaCorners[4] = X2;
  		SearchAreaCorners[6] = X1;
  	} else {
  		SearchAreaCorners[0] = X2;
  		SearchAreaCorners[2] = X1;
  		SearchAreaCorners[4] = X1;
  		SearchAreaCorners[6] = X2;
  	}
  	if (Y1 < Y2) {
  		SearchAreaCorners[1] = Y1;
  		SearchAreaCorners[3] = Y1;
  		SearchAreaCorners[5] = Y2;
  		SearchAreaCorners[7] = Y2;
  	} else {
  		SearchAreaCorners[1] = Y2;
  		SearchAreaCorners[3] = Y2;
  		SearchAreaCorners[5] = Y1;
  		SearchAreaCorners[7] = Y1;
  	}
  }
  
  void CRobotWorldView::OnInitialUpdate() 
  /*******************************************************************************
  //Created: 10/10/96 S.R.
  //Last Revision: 07/18/97 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	CView::OnInitialUpdate();
  	m_ProbabilityGrid->RestorePalette();
  	CenterRobot();
  	GetActiveRobot()->EnableComm(true);
  	return;
  }
  
  void CRobotWorldView::OnCreateCalibrationWall() 
  /*******************************************************************************
  //Created: 12/03/96 S.R.
  //Last Revision: 12/05/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	CRobot* Robot = GetActiveRobot();
  	if (Robot != NULL) {
  		if (m_OnSlipperyFloor) {
  			AfxMessageBox("Cannot create a check point on a slippery floor");
  			return;
  		}
  		if (m_DlgCreateCalibrationWall == NULL) {
  			m_DlgCreateCalibrationWall = new CDlgCreateCalibrationWall1(this);
  		}
  		robPOINT RobotError = Robot->GetPositionError();
  		if (sqrt(sqr(RobotError.x) + sqr(RobotError.y)) > 60.0) {
  			if (AfxMessageBox("The robot's position error is large.\nYou should check in before creating this Check Point.\nDo you still want to create this Check Point?", MB_YESNO) != IDYES) return;
  		}
  		m_DlgCreateCalibrationWall->SetRobotBitmap(GetLargeRobotBitmap());
  		m_DlgCreateCalibrationWall->SetRobotHeading(Robot->GetHeading());
  		m_DlgCreateCalibrationWall->ShowWindow(SW_SHOW);
  	}
  }
  
  void CRobotWorldView::OnCreateCalibrationDoorway() 
  /*******************************************************************************
  //Created: 01/10/97 S.R.
  //Last Revision: 01/10/97 S.R.
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
  {
  /*	if (m_DlgCreateCalibrationDoorway == NULL) {
  		m_DlgCreateCalibrationDoorway = new CDlgCreateCalibrationDoorway1(this);
  	}
  	m_DlgCreateCalibrationDoorway->ShowWindow(SW_SHOW);
  */
  }
  
  void CRobotWorldView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
  /*******************************************************************************
  //Created: 08/09/96 S.R.
  //Last Revision: 10/10/96 S.R.
  //Parameters:
  //	nSBCode:	signifies what kind of scroll took place.
  //	nPos:		The final scroll position when the nSBCode is SB_THUMBPOSITION or 
  //				SB_THUMBTRACK.
  //	pScrollBar:	a pointer to the scroll bar.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //	This function takes care of the vertical scrolling for the zoom bar.
  //Method:
  //
  *******************************************************************************/
  {
  	const int cLineInc = 10;
  	CRect ViewRect;
  	GetClientRect(ViewRect);
  	int cPageInc = int((ViewRect.Height() - 10) / m_ProbabilityGrid->m_ZoomLevel);
  //	static UINT OldPos = 0;
  	int Min, Max;
  
  	switch (nSBCode) {
  	case SB_LEFT:
  		GetScrollRange(SB_VERT, &Min, &Max);
  		nPos = Min;
  		break;
  	case SB_ENDSCROLL:
  //		nPos = GetScrollPos(SB_VERT);
  		nPos = m_OldVScrollPos;
  		break;
  	case SB_LINELEFT:
  		nPos = GetScrollPos(SB_VERT);
  		if (nPos <= cLineInc) nPos = 0;
  		else nPos -= cLineInc;
  		GetScrollRange(SB_VERT, &Min, &Max);
  		if (nPos < (UINT)Min) nPos = Min;
  		break;
  	case SB_LINERIGHT:
  		nPos = GetScrollPos(SB_VERT);
  		nPos += cLineInc;
  		GetScrollRange(SB_VERT, &Min, &Max);
  		if (nPos > (UINT)Max) nPos = Max;
  		break;
  	case SB_PAGELEFT:
  		nPos = GetScrollPos(SB_VERT);
  		nPos -= cPageInc;
  		GetScrollRange(SB_VERT, &Min, &Max);
  		if (nPos < (UINT)Min) nPos = Min;
  		break;
  	case SB_PAGERIGHT:
  		nPos = GetScrollPos(SB_VERT);
  		nPos += cPageInc;
  		GetScrollRange(SB_VERT, &Min, &Max);
  		if (nPos > (UINT)Max) nPos = Max;
  		break;
  	case SB_RIGHT:
  		GetScrollRange(SB_VERT, &Min, &Max);
  		nPos = Max;
  		break;
  	case SB_THUMBPOSITION:
  		break;
  	case SB_THUMBTRACK:
  		break;
  	}
  	ScrollWindow(0, int((m_OldVScrollPos - nPos) * m_ProbabilityGrid->m_ZoomLevel), NULL, NULL);
  	SetScrollPos(SB_VERT, nPos, TRUE);
  	UpdateWindow();
  	m_OldVScrollPos = nPos;
  	CView::OnVScroll(nSBCode, nPos, pScrollBar);
  }
  
  void CRobotWorldView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
  {
  	const int cLineInc = 10;
  	CRect ViewRect;
  	GetClientRect(ViewRect);
  	int cPageInc = int((ViewRect.Width() - 10) / m_ProbabilityGrid->m_ZoomLevel);
  //	static UINT OldPos = 0;
  	int Min, Max;
  
  	switch (nSBCode) {
  	case SB_LEFT:
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		nPos = Min;
  		break;
  	case SB_ENDSCROLL:
  		nPos = m_OldHScrollPos;
  		break;
  	case SB_LINELEFT:
  		nPos = GetScrollPos(SB_HORZ);
  		if (nPos <= cLineInc) nPos = 0;
  		else nPos -= cLineInc;
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		if (nPos < (UINT)Min) nPos = Min;
  		break;
  	case SB_LINERIGHT:
  		nPos = GetScrollPos(SB_HORZ);
  		nPos += cLineInc;
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		if (nPos > (UINT)Max) nPos = Max;
  		break;
  	case SB_PAGELEFT:
  		nPos = GetScrollPos(SB_HORZ);
  		nPos -= cPageInc;
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		if (nPos < (UINT)Min) nPos = Min;
  		break;
  	case SB_PAGERIGHT:
  		nPos = GetScrollPos(SB_HORZ);
  		nPos += cPageInc;
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		if (nPos > (UINT)Max) nPos = Max;
  		break;
  	case SB_RIGHT:
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		nPos = Max;
  		break;
  	case SB_THUMBPOSITION:
  		break;
  	case SB_THUMBTRACK:
  		break;
  	}
  	ScrollWindow(int((m_OldHScrollPos - nPos)*m_ProbabilityGrid->m_ZoomLevel), 0, NULL, NULL);
  	SetScrollPos(SB_HORZ, nPos, TRUE);
  	UpdateWindow();
  	m_OldHScrollPos = nPos;
  	
  	CView::OnHScroll(nSBCode, nPos, pScrollBar);
  }
  
  void CRobotWorldView::SetScrollBars()
  {
  	int SBRange;
  	CRect ViewRect;
  	if (m_ProbabilityGrid == NULL) return;
  	GetClientRect(&ViewRect);
  	SBRange = int(cWorldGridYSize - (ViewRect.Height() - 10) / m_ProbabilityGrid->m_ZoomLevel);
  	if (SBRange >= 0) {
  		if (GetScrollPos(SB_VERT) > SBRange) {
  			SetScrollPos(SB_VERT, SBRange, FALSE);
  			m_OldVScrollPos = SBRange;
  		}
  		ShowScrollBar(SB_VERT, TRUE);
  		SetScrollRange(SB_VERT, 0, SBRange, TRUE);
  	} else {
  		SetScrollPos(SB_VERT, 0, FALSE);
  		m_OldVScrollPos = 0;
  		SetScrollRange(SB_VERT, 0, 0, FALSE);
  		ShowScrollBar(SB_VERT, FALSE);
  	}
  	SBRange = int(cWorldGridXSize - (ViewRect.Width() - 10) / m_ProbabilityGrid->m_ZoomLevel);
  	if (SBRange >= 0) {
  		if (GetScrollPos(SB_HORZ) > SBRange) {
  			SetScrollPos(SB_HORZ, SBRange, FALSE);
  			m_OldHScrollPos = SBRange;
  		}
  		ShowScrollBar(SB_HORZ, TRUE);
  		SetScrollRange(SB_HORZ, 0, SBRange, TRUE);
  	} else {
  		SetScrollPos(SB_HORZ, 0, FALSE);
  		m_OldHScrollPos = 0;
  		SetScrollRange(SB_HORZ, 0, 0, FALSE);
  		ShowScrollBar(SB_HORZ, FALSE);
  	}
  }
  
  BOOL CRobotWorldView::OverRobot(robPOINT RobotWorldClick, POINT RobotScreenClick)
  {
  	const unsigned char cMinClickDist = 5;
  	if (GetActiveRobot() != NULL) {
  		if ( (ABS(RobotWorldClick.x - GetActiveRobot()->m_Position.x) < cRRWidth) && (ABS(RobotWorldClick.y - GetActiveRobot()->m_Position.y) < cRRWidth) ) {
  			return TRUE;
  		}
  		POINT RobotScreenPoint = RobotToClientCoords(GetActiveRobot()->m_Position);
  		if ( (ABS(RobotScreenPoint.x - RobotScreenClick.x) < cMinClickDist) && (ABS(RobotScreenPoint.y - RobotScreenClick.y) < cMinClickDist) ) {
  			return TRUE;
  		}
  	}
  	return FALSE;
  }
  
  void CRobotWorldView::OnLButtonDown(UINT nFlags, CPoint point) 
  {
  	m_LMouseButtonDown = true;
  	CRWGraphicObject* ClickedGraphicObject;
  	CRect WindowRect;
  	CRect UpdateRect;
  	robPOINT RobotPoint;
  	RobotPoint = ClientToRobotCoords(point);
  	m_DragPoint1 = point;
  	m_DragPoint2 = point;
  /*Get rid of old tracker rectangle*/
  //	if (m_SelectedGraphicObject != NULL) {
  //		m_SelectedGraphicObject->SetEditMode(emNone);
  //		m_SelectedGraphicObject = NULL;
  //		RedrawWindow(NULL, NULL, RDW_NOERASE | RDW_INVALIDATE);
  //	}
  
  /*Get rid of old selected label*/
  //	if (m_SelectedLabel != NULL) {
  //		m_SelectedLabel = NULL;
  //	}
  	switch (m_MouseMode) {
  	case mmDefault:
  		CRobot* Robot;
  		if ( (Robot = GetActiveRobot()) != NULL ) {
  			if (Robot->GetState() == stWaiting) {
  	//		if (true) {
  				CRobot* ClickedRobot;
  				ClickedRobot = m_ProbabilityGrid->PointInsideRobot(RobotPoint);
  				if ( ClickedRobot != NULL ) {
  					SetActiveRobot(ClickedRobot);
  					if (ClickedRobot->GetState() == stWaiting) {
  					/*Set the new destination*/
  						SetCapture();
  						if (ClickedRobot->GetOnHomeBase()) {
  							bool OldFlipHandle = ClickedRobot->GetFlipHandle();
  							if (ClickedRobot->BackupFromWall() != TR_TaskComplete) {
  							}
  							ClickedRobot->SetFlipHandle(OldFlipHandle);
  						}
  						ReleaseCapture();
  						if (m_LMouseButtonDown) {
  							robPOINT HandlePoint;
  							HandlePoint.x = ClickedRobot->m_Position.x + Robot->GetHandleLength() * cos(ClickedRobot->m_Heading);
  							HandlePoint.y = ClickedRobot->m_Position.y + Robot->GetHandleLength() * sin(ClickedRobot->m_Heading);
  							POINT ScreenPoint = RobotToClientCoords(HandlePoint);
  							ClientToScreen(&ScreenPoint);
  							SetCursorPos(ScreenPoint.x, ScreenPoint.y);
  							ClickedRobot->m_Destination.x = HandlePoint.x;
  							ClickedRobot->m_Destination.y = HandlePoint.y;
  							ClickedRobot->SetDrag(TRUE, this);
  		//					Robot->SetFlipHandle(m_DlgMotorControls.GetFlipHandle());
  		//					Robot->SetHandleLength(m_DlgMotorControls.m_edtHandleLength);
  							ClickedRobot->SetPositionVelocityDestination(ClickedRobot->m_Destination, ClickedRobot->GetMaxSpeed());
  						}
  					}
  				} else {
  					if (Robot->GetState() == stWaiting) {
  						CRWGraphicObject* ClickedGraphicObject;
  						if (ClickedGraphicObject = m_ProbabilityGrid->m_RWGraphicObjectList->PointInsideObject(RobotPoint, (float)m_ProbabilityGrid->ClientSizeToRobotSize(1))) {
  							SetCurrentRWGraphicObject(ClickedGraphicObject);
  							ClickedGraphicObject->OnDefaultClick(RobotPoint, (float)cWorldGridPointSize / m_ProbabilityGrid->GetZoomLevel(), Robot);
  							SetCurrentRWGraphicObject(NULL);
  						} else {
  							SetCapture();
  							if (Robot->GetOnHomeBase()) {
  								bool OldFlipHandle = Robot->GetFlipHandle();
  								if (Robot->BackupFromWall() != TR_TaskComplete) {
  								}
  								Robot->SetFlipHandle(OldFlipHandle);
  							}
  							ReleaseCapture();
  							Robot->m_Destination.x = RobotPoint.x;
  							Robot->m_Destination.y = RobotPoint.y;
  							Robot->SetDrag(TRUE, this);
  							Robot->SetPositionVelocityDestination(Robot->m_Destination, Robot->GetMaxSpeed());
  						}
  					}
  				}
  			}
  		}
  		break;
  	case mmPlaceLabel:
  	case mmPlaceFinishLine:
  	case mmPlaceStartLine:
  		break;
  	case mmPlaceRedZone:
  	case mmSearchArea:
  		m_DraggingRect = true;
  		break;
  	case mmPlaceVacuumPatch:
  		m_DraggingRect = true;
  //		SetCapture();
  		break;
  	case mmMoveRobot:
  		CRobot* ClickedRobot;
  		ClickedRobot = m_ProbabilityGrid->PointInsideRobot(RobotPoint);
  		if ( ClickedRobot != NULL ) {
  			SetActiveRobot(ClickedRobot);
  			ClickedRobot->EStop();
  			ClickedRobot->SetState(stRelocating);
  			m_PreDragRobotPosition = ClickedRobot->m_Position;
  			if (m_SelectedGraphicObject) {
  				m_SelectedGraphicObject->SetEditMode(emNone);
  				m_SelectedGraphicObject = NULL;
  				RedrawWindow(NULL, NULL, RDW_NOERASE | RDW_INVALIDATE);
  			}
  		}
  		break;
  	case mmEdit:
  		if (m_SelectedGraphicObject != NULL) {
  			if ( m_SelectedGraphicObject->GetVisible() && m_SelectedGraphicObject->InsideObject(RobotPoint, (float)m_ProbabilityGrid->ClientSizeToRobotSize(1)) ) {
  				m_SelectedGraphicObject->OnEditClick(RobotPoint, this);
  			} else {
  				m_SelectedGraphicObject->SetEditMode(emNone);
  				m_SelectedGraphicObject = NULL;
  				if (ClickedGraphicObject = m_ProbabilityGrid->m_RWGraphicObjectList->PointInsideObject(RobotPoint, (float)m_ProbabilityGrid->ClientSizeToRobotSize(1))) {
  					m_SelectedGraphicObject = ClickedGraphicObject;
  					m_SelectedGraphicObject->OnEditClick(RobotPoint, this);
  				}
  				RedrawWindow(NULL, NULL, RDW_NOERASE | RDW_INVALIDATE);
  			}
  		} else if (ClickedGraphicObject = m_ProbabilityGrid->m_RWGraphicObjectList->PointInsideObject(RobotPoint, (float)m_ProbabilityGrid->ClientSizeToRobotSize(1))) {
  			m_SelectedGraphicObject = ClickedGraphicObject;
  			m_SelectedGraphicObject->OnEditClick(RobotPoint, this);
  		} else {
  //				if (m_SelectedGraphicObject != NULL) {
  //					m_SelectedGraphicObject->SetEditMode(emNone);
  //					m_SelectedGraphicObject = NULL;
  //				}
  		}
  		break;
  	case mmErase:
  		if (ClickedGraphicObject = m_ProbabilityGrid->m_RWGraphicObjectList->PointInsideObject(RobotPoint, (float)m_ProbabilityGrid->ClientSizeToRobotSize(1))) {
  			if (ClickedGraphicObject != m_HomeBase) {
  				GetDocument()->SetModifiedFlag();
  				ClickedGraphicObject->OnErase();
  				m_ProbabilityGrid->m_RWGraphicObjectList->Remove(ClickedGraphicObject);
  				RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  			}
  		}
  		break;
  	case mmDrawLine:
  	case mmDrawFreehand:
  	case mmDrawRectangle:
  //	case mmClearGrid:
  //	case mmBlockGrid:
  //	case mmUnknownGrid:
  		GetDocument()->SetModifiedFlag();
  		switch (m_MouseMode) {
  		case mmDrawFreehand:
  			short PenWidth;
  			PenWidth = GetPenWidth();
  			switch (GetPenMode()) {
  			case pmClear:
  				m_ProbabilityGrid->SetGridValue(RobotPoint, cMaxProb, PenWidth);
  				break;
  			case pmObstacle:
  				m_ProbabilityGrid->SetGridValue(RobotPoint, cMinProb, PenWidth);
  				break;
  			case pmUnknown:
  				m_ProbabilityGrid->SetGridValue(RobotPoint, (cMaxProb + cMinProb) / 2, PenWidth);
  				break;
  			}
  			break;
  		case mmDrawLine:
  			m_DragPoint1.x = m_DragPoint1.x - m_DragPoint1.x % (int)m_ProbabilityGrid->GetZoomLevel() + long(m_ProbabilityGrid->GetZoomLevel()/2) - GetPenWidth() / 2;
  			m_DragPoint1.y = m_DragPoint1.y - m_DragPoint1.y % (int)m_ProbabilityGrid->GetZoomLevel() + long(m_ProbabilityGrid->GetZoomLevel()/2) - GetPenWidth() / 2;
  			m_DragPoint2 = m_DragPoint1;
  			m_DragLine = true;
  			m_DragLineWidth = int(GetPenWidth() * m_ProbabilityGrid->GetZoomLevel());
  			UpdateRect.TopLeft() = m_DragPoint1;
  			UpdateRect.BottomRight() = m_DragPoint2;
  			UpdateRect.NormalizeRect();
  			UpdateRect.InflateRect(m_DragLineWidth, m_DragLineWidth);
  			RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  			break;
  		case mmDrawRectangle:
  			m_DragRect.left = point.x - point.x % (int)m_ProbabilityGrid->GetZoomLevel();
  			m_DragRect.top = point.y - point.y % (int)m_ProbabilityGrid->GetZoomLevel();
  			m_DragRect.right = long(m_DragRect.left + m_ProbabilityGrid->GetZoomLevel());
  			m_DragRect.bottom = long(m_DragRect.top + m_ProbabilityGrid->GetZoomLevel());
  			m_DraggingRect = true;
  			RedrawWindow(m_DragRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  			break;
  		}
  /*
  		if (GetPenTool() == ptPen) {
  			short PenWidth;
  			PenWidth = GetPenWidth();
  			switch (m_MouseMode) {
  			case mmClearGrid:
  				m_ProbabilityGrid->SetGridValue(RobotPoint, cMaxProb, PenWidth);
  				break;
  			case mmBlockGrid:
  				m_ProbabilityGrid->SetGridValue(RobotPoint, cMinProb, PenWidth);
  				break;
  			case mmUnknownGrid:
  				m_ProbabilityGrid->SetGridValue(RobotPoint, (cMaxProb + cMinProb) / 2, PenWidth);
  				break;
  			}
  			UpdateRect.top = point.y - PenWidth*m_ProbabilityGrid->m_ZoomLevel;
  			UpdateRect.left = point.x - PenWidth*m_ProbabilityGrid->m_ZoomLevel;
  			UpdateRect.bottom = point.y + PenWidth*m_ProbabilityGrid->m_ZoomLevel;
  			UpdateRect.right = point.x + PenWidth*m_ProbabilityGrid->m_ZoomLevel;
  			RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  		} else {
  			if (GetPenTool() == ptRectangle) m_DragRect = true;
  			else if (GetPenTool() == ptLine) m_DragLine = true;
  		}
  */
  		break;
  	default:
  		break;
  	}
  	CView::OnLButtonDown(nFlags, point);
  }
  
  void CRobotWorldView::OnLButtonUp(UINT nFlags, CPoint point) 
  {
  	CRobot* Robot = NULL;
  	m_LMouseButtonDown = false;
  	robPOINT RobotCoord1, RobotCoord2, RobotCoord3;
  	int state;
  	BOOL showLAYGoTip = FALSE;
  	CVacuumPatch* NewPatch;
  	robPOINT RobotPoint;
  	robRECT RobotRect;
  	CRect UpdateRect;
  	CRect WindowRect;
  	RobotPoint = ClientToRobotCoords(point);
  	m_DraggingRect = false;
  	m_DragLine = false;
  	m_DragPoint2 = point;
  	switch (m_MouseMode) {
  	case mmDefault:
  	/*End dragging the robot*/
  		if ( (Robot = GetActiveRobot()) != NULL ) {
  			if (Robot->GetState() == stDragging) {
  				Robot->SetDrag(FALSE, this);
  				Robot->Stop();
  				RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  			}
  		}
  /*		{
  		CDlgOrientHomebase DlgOrientHomebase;
  		DlgOrientHomebase.SetRobotBitmap(m_ProbabilityGrid->GetLargeRobotBitmap());
  		DlgOrientHomebase.DoModal();
  
  		CDlgFindHomebaseCorner DlgFindHomebaseCorner;
  		DlgFindHomebaseCorner.SetRobotBitmap(m_ProbabilityGrid->GetLargeRobotBitmap());
  		DlgFindHomebaseCorner.SetHomebaseHeading(DlgOrientHomebase.GetHomebaseHeading());
  		DlgFindHomebaseCorner.DoModal();
  		if (DlgFindHomebaseCorner.GetCornerToLeft()) {
  			AfxMessageBox("To the left");
  		} else {
  			AfxMessageBox("To the right");
  		}
  		}
  */		// Added for LAYGo by Dave @2:34pm EST 8 April 1998
  		if (m_DlgLaygo->m_EnableLaygo) {
  			if (!m_DlgLaygo->IsWindowVisible()) {
  				state = m_DlgLaygo->GetCurrentState(); // gets the last shown robotip #
  				state ++;
  				showLAYGoTip = ShowTip(state);
  				if (showLAYGoTip) {
  					ShowLaygoTip(state, true);
  				}
  			}
  		}
  		break;
  	case mmPlaceLabel:
  		{
  		CDlgExecutableObjectGeneralProperties DlgProperties;
  		DlgProperties.SetName("Hot Point");
  		DlgProperties.SetNumberOfClaps(-1);
  		DlgProperties.SetEditingObjectType(goPosition);
  		if (DlgProperties.DoModal() == IDOK) {
  			CPositionLabel* NewLabel = new CPositionLabel();
  			m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewLabel);
  			m_ProbabilityGrid->m_ExecutableObjectList->Add(NewLabel);
  			NewLabel->SetEntryPoint(RobotPoint);
  			NewLabel->SetCaption(DlgProperties.GetName());
  			NewLabel->SetNumberOfClaps(DlgProperties.GetNumberOfClaps());
  			NewLabel->SetVisible(true);
  			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
  			GetDocument()->SetModifiedFlag();
  		}
  //		SetMouseMode(mmDefault);
  		}
  		break;
  	case mmPlaceFinishLine:
  		{
  		CFinishLine* NewFinishLine = new CFinishLine();
  		m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewFinishLine);
  		m_ProbabilityGrid->m_ExecutableObjectList->Add(NewFinishLine);
  		RobotRect.left = RobotPoint.x;
  		RobotRect.top = RobotPoint.y;
  		RobotRect.right = RobotPoint.x;
  		RobotRect.bottom = RobotPoint.y;
  		NewFinishLine->SetBounds(RobotRect);
  		NewFinishLine->SetVisible(true);
  		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
  		GetDocument()->SetModifiedFlag();
  		}
  		break;
  	case mmPlaceStartLine:
  		{
  		CStartLine* NewStartLine = new CStartLine();
  		m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewStartLine);
  //		m_ProbabilityGrid->m_ExecutableObjectList->Add(NewStartLine);
  		RobotRect.left = RobotPoint.x;
  		RobotRect.top = RobotPoint.y;
  		RobotRect.right = RobotPoint.x;
  		RobotRect.bottom = RobotPoint.y;
  		NewStartLine->SetBounds(RobotRect);
  		NewStartLine->SetVisible(true);
  		NewStartLine->SetShowCaption(false);
  		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
  		GetDocument()->SetModifiedFlag();
  		}
  		break;
  	case mmPlaceRedZone:
  		{
  		robRECT WorldRect;
  		WindowRect.left = m_DragPoint1.x;
  		WindowRect.top = m_DragPoint1.y;
  		WindowRect.right = m_DragPoint2.x;
  		WindowRect.bottom = m_DragPoint2.y;
  		WorldRect = ClientToRobotCoords(WindowRect);
  
  //		WorldRect.left = RobotPoint.x;
  //		WorldRect.top = RobotPoint.y;
  //		CPoint WindowPoint;
  //		WindowPoint.x = point.x + 100;
  //		WindowPoint.y = point.y + 100;
  //		robPOINT temp = ClientToRobotCoords(WindowPoint);
  //		WorldRect.right = temp.x;
  //		WorldRect.bottom = temp.y;
  		CRedZone* NewRedZone = m_RedZoneList->Add(WorldRect);
  		m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewRedZone);
  		UpdateRect = RobotToClientCoords(NewRedZone->GetBounds());
  		UpdateRect.NormalizeRect();
  		RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  		GetDocument()->SetModifiedFlag();
  		}
  		break;
  	case mmPlaceVacuumPatch:
  		{
  //		ReleaseCapture();
  //		m_DragPoint2 = point;
  		robRECT PatchBounds;
  		robPOINT RobotCoord;
  		RobotCoord = ClientToRobotCoords(m_DragPoint1);
  		PatchBounds.left = RobotCoord.x;
  		PatchBounds.top = RobotCoord.y;
  		RobotCoord = ClientToRobotCoords(m_DragPoint2);
  		PatchBounds.right = RobotCoord.x;
  		PatchBounds.bottom = RobotCoord.y;
  
  //		m_DlgCreateVacuumPatch.ShowWindow(SW_SHOW);
  		if (m_DlgCreateVacuumPatch.DoModal() == IDOK) {
  			NewPatch = new CVacuumPatch(PatchBounds, m_DlgCreateVacuumPatch.m_EnterSide,
  				m_DlgCreateVacuumPatch.GetPathWidth(),m_DlgCreateVacuumPatch.GetTurnDiameter(), m_ProbabilityGrid->m_ExecutableObjectList);
  			NewPatch->SetVacuumingSpeed(short(m_DlgCreateVacuumPatch.m_edtVacuumingSpeed * 10));
  			m_ProbabilityGrid->m_RWGraphicObjectList->Add(NewPatch);
  			m_ProbabilityGrid->m_ExecutableObjectList->Add(NewPatch);
  			NewPatch->SetCaption(m_DlgCreateVacuumPatch.m_edtName);
  
  //			robPOINT LabelPoint = NewPatch->m_EntryPoint;
  //			CVacuumPatchLabel* NewLabel = new CVacuumPatchLabel(m_DlgCreateVacuumPatch.m_edtName, LabelPoint, this, m_LabelList, NewPatch);
  //			m_LabelList->Add(NewLabel);
  //			NewPatch->m_Label = NewLabel;
  
  			UpdateRect = RobotToClientCoords(NewPatch->GetBounds());
  			UpdateRect.NormalizeRect();
  //			RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
  		} else {
  			UpdateRect.TopLeft() = m_DragPoint1;
  			UpdateRect.BottomRight() = m_DragPoint2;
  			UpdateRect.NormalizeRect();
  			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  		}
  //		m_DlgCreateVacuumPatch.ShowWindow(SW_HIDE);
  
  //		SetMouseMode(mmDefault);
  		}
  		break;
  	case mmSearchArea:
  		WindowRect.left = m_DragPoint1.x;
  		WindowRect.top = m_DragPoint1.y;
  		WindowRect.right = m_DragPoint2.x;
  		WindowRect.bottom = m_DragPoint2.y;
  		RobotRect = ClientToRobotCoords(WindowRect);
  		if (GetActiveRobot() != NULL) {
  			GetActiveRobot()->SearchArea(RobotRect);
  		}
  		break;
  	case mmMoveRobot:
  		if ( (Robot = GetActiveRobot()) != NULL ) {
  			if (Robot->GetState() == stRelocating) {
  			//Create the relocate robot dialog
  				double OldHeading = Robot->m_Heading;
  	//			robPOINT OldPosition = Robot->GetPosition();
  				robPOINT OldError = Robot->GetPositionError();
  				CDlgRelocateRobot DlgRelocateRobot;
  				DlgRelocateRobot.SetRobotBitmap(m_ProbabilityGrid->GetLargeRobotBitmap());
  				robPOINT DrawRobotPosition = Robot->GetDrawPosition();
  				int DlgResult = DlgRelocateRobot.DoModal(this, DrawRobotPosition.x, DrawRobotPosition.y, Robot->GetHeading());
  				if (DlgResult == IDOK) {
  					Robot->SetHeading(DlgRelocateRobot.m_RobotHeading);
  	//				Robot->SetPosition(Robot->m_Position);
  					Robot->SetPosition(DrawRobotPosition);
  				} else if (DlgResult == IDCANCEL) {
  					Robot->m_Heading = OldHeading;
  					Robot->m_Position = m_PreDragRobotPosition;
  				}
  	//			UpdateRect = CalculateUpdateRect(OldPosition, OldError, Robot->GetPosition(), Robot->GetPositionError());
  				UpdateRect = CalculateUpdateRect(DrawRobotPosition, OldError, Robot->GetPosition(), Robot->GetPositionError());
  				Robot->SetDrawPosition(Robot->GetPosition());
  				Robot->SetDrawHeading(Robot->GetHeading());
  
  				RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	//			m_ProbabilityGrid->RedrawRobot(Robot->m_Position, Robot->m_Heading, this);
  				Robot->SetState(stWaiting);
  	//			SetMouseMode(mmDefault);
  			}
  		}
  		break;
  	case mmDrawLine:
  	case mmDrawFreehand:
  	case mmDrawRectangle:
  //	case mmClearGrid:
  //	case mmBlockGrid:
  //	case mmUnknownGrid:
  		RobotCoord1 = ClientToRobotCoords(m_DragPoint1);
  		RobotCoord2 = ClientToRobotCoords(m_DragPoint2);
  		int ProbValue;
  		ProbValue = cMaxProb;
  /*		switch (m_MouseMode) {
  		case (mmClearGrid):
  			ProbValue = cMaxProb;
  			break;
  		case (mmBlockGrid):
  			ProbValue = cMinProb;
  			break;
  		case (mmUnknownGrid):
  			ProbValue = (cMaxProb + cMinProb) / 2;
  			break;
  		}
  */
  		switch (GetPenMode()) {
  		case (pmClear):
  			ProbValue = cMaxProb;
  			break;
  		case (pmObstacle):
  			ProbValue = cMinProb;
  			break;
  		case (pmUnknown):
  			ProbValue = (cMaxProb + cMinProb) / 2;
  			break;
  		default:
  			ASSERT(FALSE);
  			break;
  		}
  //		switch (GetPenTool()) {
  		switch (m_MouseMode) {
  //		case ptLine:
  		case mmDrawLine:
  			if (nFlags & MK_CONTROL) {
  				m_ProbabilityGrid->SetGridValue(RobotCoord1, RobotCoord2, ProbValue, GetPenWidth(), true);
  			} else {
  				m_ProbabilityGrid->SetGridValue(RobotCoord1, RobotCoord2, ProbValue, GetPenWidth(), false);
  			}
  			break;
  //		case ptRectangle:
  		case mmDrawRectangle:
  			RobotRect.top = RobotCoord2.y;
  			RobotRect.left = RobotCoord2.x;
  			RobotRect.bottom = RobotCoord1.y;
  			RobotRect.right = RobotCoord1.x;
  			if (nFlags & MK_CONTROL) {
  				m_ProbabilityGrid->SetGridValue(RobotRect, ProbValue, true);
  			} else {
  				m_ProbabilityGrid->SetGridValue(RobotRect, ProbValue, false);
  			}
  			break;
  		}
  		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  		break;
  	case mmPlaceLinePath:
  //		if (!m_CurrentLinePathEnded) {
  			if (m_CurrentLinePath == NULL) {
  				m_LinePathFrom = m_ProbabilityGrid->m_ExecutableObjectList->PointNearExecutableObjectExit(RobotPoint);
  				if (m_LinePathFrom != NULL) {
  					RobotPoint = m_LinePathFrom->GetExitPoint();
  				}
  				m_CurrentLinePath = new CLinePath();
  				m_CurrentLinePath->SetVisible(true);
  				m_ProbabilityGrid->m_RWGraphicObjectList->Add(m_CurrentLinePath);
  				m_ProbabilityGrid->m_ExecutableObjectList->Add(m_CurrentLinePath);
  				m_CurrentLinePath->AddPoint(RobotPoint);
  				m_CurrentLinePath->SetCaption("Line Path");
  				RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  			} else {
  				m_CurrentLinePath->AddPoint(RobotPoint);
  //				if (m_CurrentLinePath->NumPoints() == 5) m_CurrentLinePath = NULL;
  				RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  			}
  			m_DragLine = true;
  			m_DragLineWidth = 1;
  			m_DragPoint1 = point;
  //		} else {
  //			m_CurrentLinePathEnded = false;
  //		}
  		break;
  	default:
  		break;
  	}
  	CView::OnLButtonUp(nFlags, point);
  }
  
  
  void CRobotWorldView::AlignPointsWithProbabilityGridSquares(CRect& Rect)
  {
  	Rect.NormalizeRect();
  	Rect.left = Rect.left - Rect.left % (int)m_ProbabilityGrid->GetZoomLevel();
  	Rect.top = Rect.top - Rect.top % (int)m_ProbabilityGrid->GetZoomLevel();
  	Rect.right = Rect.right - Rect.right % (int)m_ProbabilityGrid->GetZoomLevel() + long(m_ProbabilityGrid->GetZoomLevel());
  	Rect.bottom = Rect.bottom - Rect.bottom % (int)m_ProbabilityGrid->GetZoomLevel() + long(m_ProbabilityGrid->GetZoomLevel());
  }
  
  void CRobotWorldView::OnMouseMove(UINT nFlags, CPoint point) 
  {
  	static CPoint OldPoint(0,0);
  	static bool OldLButtonDown = FALSE;
  	CRect UpdateRect;
  	robPOINT RobotPoint;
  	RobotPoint = ClientToRobotCoords(point);
  	m_MouseOverObject = false;
  	switch (m_MouseMode) {
  	case mmDefault:
  		CRobot* Robot;
  		if ( (Robot = GetActiveRobot()) != NULL) {
  			if (nFlags & MK_LBUTTON) {
  			/*If Dragging, find the new destination*/
  				if (Robot->GetState() == stDragging) {
  					Robot->m_Destination.x = RobotPoint.x;
  					Robot->m_Destination.y = RobotPoint.y;
  				}
  			} else if (Robot->GetState() == stWaiting) {
  				if (!OverRobot(RobotPoint, point)) {
  					if (m_ProbabilityGrid->m_ExecutableObjectList->PointInsideExecutableArea(RobotPoint, (float)cWorldGridPointSize / m_ProbabilityGrid->GetZoomLevel())) {
  						m_MouseOverObject = true;
  					}
  				}
  			}
  		}
  		break;
  	case mmPlaceLabel:
  		break;
  	case mmPlaceFinishLine:
  	case mmPlaceStartLine:
  		break;
  	case mmPlaceRedZone:
  	case mmPlaceVacuumPatch:
  	case mmSearchArea:
  		if (nFlags & MK_LBUTTON) {
  			m_DragPoint2 = point;
  			m_DragRect.TopLeft() = m_DragPoint1;
  			m_DragRect.BottomRight() = m_DragPoint2;
  			m_DragRect.NormalizeRect();
  			UpdateRect = m_DragRect;
  			UpdateRect.UnionRect(UpdateRect, m_OldDragRect);
  			RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  		}
  		break;
  	case mmMoveRobot:
  		if ( (Robot = GetActiveRobot()) != NULL ) {
  			if (Robot->GetState() == stRelocating) {
  	//			Robot->SetPosition(RobotPoint);
  				robPOINT P1, P2;
  				P1 = ClientToRobotCoords(m_DragPoint1);
  				P2 = ClientToRobotCoords(point);
  				m_DragPoint1 = point;
  	//			P1.x = Robot->m_Position.x + P2.x - P1.x;
  	//			P1.y = Robot->m_Position.y + P2.y - P1.y;
  	//			UpdateRect = CalculateUpdateRect(Robot->GetPosition(), Robot->GetPositionError(), P1, Robot->GetPositionError());
  	//			Robot->m_Position = P1;
  				robPOINT DrawRobotPosition = Robot->GetDrawPosition();
  				P1.x = DrawRobotPosition.x + P2.x - P1.x;
  				P1.y = DrawRobotPosition.y + P2.y - P1.y;
  				UpdateRect = CalculateUpdateRect(DrawRobotPosition, Robot->GetPositionError(), P1, Robot->GetPositionError());
  				Robot->SetDrawPosition(P1);
  				RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	//			m_ProbabilityGrid->RedrawRobot(Robot->m_Position, Robot->m_Heading, this);
  			}
  		}
  		break;
  //	case mmClearGrid:
  //	case mmBlockGrid:
  //	case mmUnknownGrid:
  	case mmDrawLine:
  	case mmDrawFreehand:
  	case mmDrawRectangle:
  		if (nFlags & MK_LBUTTON) {
  //			if (GetPenTool() == ptPen) {
  			if (m_MouseMode == mmDrawFreehand) {
  				robPOINT RobotPoint0;
  				CPoint P0;
  				if (OldLButtonDown) {
  					P0 = OldPoint;
  				} else {
  					P0 = m_DragPoint1;
  				}
  				RobotPoint0 = ClientToRobotCoords(P0);
  				int ProbValue = cMaxProb;
  //				switch (m_MouseMode) {
  				switch (GetPenMode()) {
  //				case (mmClearGrid):
  				case (pmClear):
  					ProbValue = cMaxProb;
  					break;
  //				case (mmBlockGrid):
  				case (pmObstacle):
  					ProbValue = cMinProb;
  					break;
  //				case (mmUnknownGrid):
  				case (pmUnknown):
  					ProbValue = cWorldGridInitialValue;
  					break;
  				default:
  					ASSERT(FALSE);
  					break;
  				}
  				if (nFlags & MK_CONTROL) {
  					m_ProbabilityGrid->SetGridValue(RobotPoint0, RobotPoint, ProbValue, GetPenWidth(), true);
  				} else {
  					m_ProbabilityGrid->SetGridValue(RobotPoint0, RobotPoint, ProbValue, GetPenWidth());
  				}
  				UpdateRect.top = P0.y;
  				UpdateRect.left = P0.x;
  				UpdateRect.bottom = point.y;
  				UpdateRect.right = point.x;
  				UpdateRect.NormalizeRect();
  				UpdateRect.InflateRect(int(GetPenWidth()*m_ProbabilityGrid->m_ZoomLevel), int(GetPenWidth()*m_ProbabilityGrid->m_ZoomLevel));
  				RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  			} else {
  				m_DragPoint2 = point;
  				if (m_MouseMode == mmDrawRectangle) {
  					m_DragRect.TopLeft() = m_DragPoint1;
  					m_DragRect.BottomRight() = m_DragPoint2;
  					AlignPointsWithProbabilityGridSquares(m_DragRect);
  					UpdateRect = m_DragRect;
  					UpdateRect.UnionRect(UpdateRect, m_OldDragRect);
  					RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  				} else {
  					m_DragPoint2.x = m_DragPoint2.x - m_DragPoint2.x % (int)m_ProbabilityGrid->GetZoomLevel() + long(m_ProbabilityGrid->GetZoomLevel()/2) - GetPenWidth() / 2;
  					m_DragPoint2.y = m_DragPoint2.y - m_DragPoint2.y % (int)m_ProbabilityGrid->GetZoomLevel() + long(m_ProbabilityGrid->GetZoomLevel()/2) - GetPenWidth() / 2;
  					UpdateRect.TopLeft() = m_DragPoint1;
  					UpdateRect.BottomRight() = m_DragPoint2;
  					UpdateRect.NormalizeRect();
  					UpdateRect.InflateRect(m_DragLineWidth, m_DragLineWidth);
  					UpdateRect.UnionRect(UpdateRect, m_OldDragRect);
  					RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  				}
  			}
  		}
  		break;
  	case mmPlaceLinePath:
  		if (m_CurrentLinePath != NULL) {
  			m_DragPoint2 = point;
  			UpdateRect.top = m_DragPoint1.y;
  			UpdateRect.left = m_DragPoint1.x;
  			UpdateRect.bottom = m_DragPoint2.y;
  			UpdateRect.right = m_DragPoint2.x;
  			UpdateRect.NormalizeRect();
  			UpdateRect.UnionRect(UpdateRect, m_OldDragRect);
  			RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  		}
  		break;
  	default:
  		break;
  	}
  	OldPoint = point;
  	OldLButtonDown = nFlags & MK_LBUTTON;
  	CView::OnMouseMove(nFlags, point);
  }
  
  robPOINT CRobotWorldView::ClientToRobotCoords(CPoint point)
  {
  	return m_ProbabilityGrid->ClientToRobotCoords(point);
  }
  
  CPoint CRobotWorldView::RobotToClientCoords(robPOINT point)
  {
  	return m_ProbabilityGrid->RobotToClientCoords(point);
  }
  
  /*struct TFoundLabelPathParam {
  	CRobot* robot;
  	CLabel* label;
  };
  */
  /*
  void CRobotWorldView::FoundLabelPath(Path* FoundPath, UINT FoundPathParam1, void* FoundPathParam2)
  {
  	CRobot* Robot = ((TFoundLabelPathParam*)FoundPathParam2)->robot;
  	CLabel* Label = ((TFoundLabelPathParam*)FoundPathParam2)->label;
  	delete FoundPathParam2;
  	if (FoundPath != NULL) {
  		Robot->AddPathToInstructions(FoundPath);
  		if (Robot->FollowInstructions(TRUE, TRUE) == IR_InstructionComplete) {
  			if (Label->m_LabelType == ltCalibrationZone) {
  				Robot->CalibratePosition(Label->m_CalibrationZone);
  			}
  		}
  	}
  	delete FoundPath;
  }
  */
  
  void CRobotWorldView::OnZoomView()
  /*******************************************************************************
  //Created: 07/10/97 S.R.
  //Last Revision: 07/10/97 S.R.
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
  {
  /*Redraw the window*/
  //	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
  	CenterRobot();
  }
  
  void CRobotWorldView::CenterRobot()
  {
  	CRobot* Robot = GetActiveRobot();
  	if (Robot != NULL) {
  	/*Do Scrolling*/
  		int HScrollPos, VScrollPos;
  		int Min, Max;
  		CRect ClientRect;
  		GetClientRect(&ClientRect);
  	/*Horizontal Scroll*/
  		HScrollPos = int(Robot->m_Position.x / cWorldGridPointSize + cWorldGridXSize / 2);
  		HScrollPos -= int(ClientRect.Width() / m_ProbabilityGrid->m_ZoomLevel / 2);
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		if (HScrollPos < Min) HScrollPos = Min;
  		if (HScrollPos > Max) HScrollPos = Max;
  		SetScrollPos(SB_HORZ, HScrollPos, TRUE);
  		m_OldHScrollPos = HScrollPos;
  	//	OnHScroll(SB_THUMBPOSITION, HScrollPos, GetScrollBarCtrl(SB_HORZ));
  	/*Vertical Scroll*/
  		VScrollPos = int(-(Robot->m_Position.y / cWorldGridPointSize) + cWorldGridYSize / 2);
  		VScrollPos -= int(ClientRect.Height() / m_ProbabilityGrid->m_ZoomLevel / 2);
  		GetScrollRange(SB_VERT, &Min, &Max);
  		if (VScrollPos < Min) VScrollPos = Min;
  		if (VScrollPos > Max) VScrollPos = Max;
  		SetScrollPos(SB_VERT, VScrollPos, TRUE);
  		m_OldVScrollPos = VScrollPos;
  	//	OnVScroll(SB_THUMBPOSITION, VScrollPos, GetScrollBarCtrl(SB_VERT));
  
  		ScrollWindow(int((m_OldHScrollPos - HScrollPos)*m_ProbabilityGrid->m_ZoomLevel), 
  			int((m_OldVScrollPos - VScrollPos) * m_ProbabilityGrid->m_ZoomLevel), NULL, NULL);
  	//	UpdateWindow();
  		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
  
  
  	/*Move the labels*/
  	//	m_LabelList->RedisplayLabels();
  	/*Move the red zones*/
  	//	CRedZoneListNode* CurrentZoneNode = m_RedZoneList->m_RedZoneListHead;
  	//	while (CurrentZoneNode != NULL) {
  	//		CurrentZoneNode->m_RedZone->Redisplay();
  	//		CurrentZoneNode = CurrentZoneNode->m_Next;
  	//	}
  	}
  }
  
  void CRobotWorldView::Serialize(CArchive & ar)
  {
  	UINT nVersion = 0;
  	CObject::Serialize(ar);
  	unsigned long ltemp;
  /*Disable the robot communications whenever serializing*/
  	GetActiveRobot()->EnableComm(false);
  	if (ar.IsStoring()) {
  		ar.SetObjectSchema(VERSION_NUMBER);
  	/*Take care of stupid version stuff*/
  		ar << (m_ViewVacuumPatches | 0x2);
  		ar << ((UINT)(VERSION_NUMBER));
  		ltemp = m_PenSize;
  		ar << ltemp;
  		ltemp = m_PenMode;
  		ar << ltemp;
  		ar << m_HomeBase;
  		ltemp = m_JoystickControlType;
  		ar << ltemp;
  		ar << m_JoystickResponsiveness;
  		ar << m_JoystickTurnGain;
  		ar << m_JoystickOrientation;
  		ltemp = true;
  		ar << ltemp;
  	} else {
  		New();
  //		UINT nVersion = ar.GetObjectSchema();
  		ar >> m_ViewVacuumPatches;
  	/*Stupid archive doesn't keep track of the version info*/
  		if (m_ViewVacuumPatches < 2) {
  			ar.SetObjectSchema(2);
  		} else {
  			UINT temp;
  			ar >> temp;
  			ar.SetObjectSchema(temp);
  		}
  		nVersion = ar.GetObjectSchema();
  		if (nVersion > VERSION_NUMBER) {
  			AfxMessageBox("This file is not compatible with this version of Map-N-Zap");
  			return;
  		} else if (nVersion < VERSION_NUMBER) {
  			CDialog VersionWarning(IDD_VERSION_WARNING);
  			VersionWarning.DoModal();
  		}
  
  		ar >> ltemp;
  		SetPenSize((TPenSize)ltemp);
  		ar >> ltemp;
  		SetPenMode((TPenMode)ltemp);
  //		if ( (nVersion > 1) && (nVersion != (UINT)-1) ) ar >> m_HomeBase;
  		ar >> m_HomeBase;
  //		m_ProbabilityGrid->m_RWGraphicObjectList->Add(m_HomeBase);
  //		m_ProbabilityGrid->m_ExecutableObjectList->Add(m_HomeBase);
  		ar >> ltemp;
  		m_JoystickControlType = (TJoystickControlType)ltemp;
  		ar >> m_JoystickResponsiveness;
  		ar >> m_JoystickTurnGain;
  		ar >> m_JoystickOrientation;
  		if (nVersion > 4) {
  			ar >> ltemp;
  //			m_AnimateRobotEnable = ltemp != 0;
  		}
  	}
  /*Take care of versioning screw up*/
  	if (ar.IsLoading() && (nVersion <= 2) ) {
  		m_DlgCreateVacuumPatch.SerializeV2(ar);
  		m_ProbabilityGrid->SerializeV2(ar);
  		m_RedZoneList->SerializeV2(ar);
  		GetRobot(0)->SerializeV2(ar);
  	} else {
  		m_DlgCreateVacuumPatch.Serialize(ar);
  		m_ProbabilityGrid->Serialize(ar);
  		m_RedZoneList->Serialize(ar);
  		if (ar.IsLoading() && (nVersion < 9) ) {
  			GetRobot(0)->Serialize(ar);
  		}
  	}
  	GetMainFrame()->OnSetVacuumAttached(GetActiveRobot()->GetAttachmentType() == atVacuum);
  	GetMainFrame()->OnSetWagonAttached(GetActiveRobot()->GetAttachmentType() == atWagon);
  /*Update FlowZap and Laygo whenever reading in a new file*/
  	if ( !(ar.IsStoring()) ) {
  		if (m_DlgFlowZap != NULL) {
  			CStringList* PositionLabelNames = new CStringList;
  			CStringList* VacuumPatchLabelNames = new CStringList;
  			CStringList* CalibrationZoneLabelNames = new CStringList;
  			CStringList* LinePaths = new CStringList;
  			m_ProbabilityGrid->m_ExecutableObjectList->GetCaptionLists(PositionLabelNames, CalibrationZoneLabelNames, VacuumPatchLabelNames, LinePaths);
  			if (m_HomeBase != NULL) PositionLabelNames->AddHead(m_HomeBase->GetCaption());
  			m_DlgFlowZap->SetLabelNames(PositionLabelNames);
  			m_DlgFlowZap->SetVacuumPatches(VacuumPatchLabelNames);
  			m_DlgFlowZap->SetCalibrationZones(CalibrationZoneLabelNames);
  			m_DlgFlowZap->SetLinePaths(LinePaths);
  		}
  		if (m_HomeBase == NULL) {
  			int CurrentState = m_DlgLaygo->GetCurrentState();
  			if (CurrentState > CLaygo::lgoHomeBaseCreateState) {
  				ShowLaygoTip(CLaygo::lgoHomeBaseCreate2State, false);
  			} else if (CurrentState <= CLaygo::lgoHomeBaseCreateState) {
  				ShowLaygoTip(CurrentState, true);
  			}
  		} else {
  			m_DlgLaygo->ShowWindow(SW_HIDE);
  		}
  	}
  	GetActiveRobot()->EnableComm(true);
  	RedrawWindow();
  }
  
  void CRobotWorldView::SetMouseMode(TMouseMode MouseMode)
  {
  	m_MouseMode = MouseMode;
  	if (m_SelectedGraphicObject != NULL) {
  //		CRectTracker tracker;
  //		SetupTracker(&tracker);
  		m_SelectedGraphicObject->SetEditMode(emNone);
  		m_SelectedGraphicObject = NULL;
  		m_DragLine = false;
  		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	}
  	if (MouseMode != mmPlaceLinePath) m_CurrentLinePath = NULL;
  }
  
  TMouseMode CRobotWorldView::GetMouseMode()
  {
  	return m_MouseMode;
  }
  
  void CRobotWorldView::OnContextMenu(CWnd* pWnd, CPoint point)
  {
  	if (pWnd == this) {
  		if (m_SelectedGraphicObject != NULL) {
  			m_SelectedGraphicObject->SetEditMode(emNone);
  			m_SelectedGraphicObject->Redraw();
  			m_SelectedGraphicObject = NULL;
  		}
  //		m_SelectedLabel = NULL;
  		CRWGraphicObject* ClickedGraphicObject;
  		POINT ClientPoint;
  		ClientPoint = point;
  		ScreenToClient(&ClientPoint);
  		robPOINT RobotPoint = ClientToRobotCoords(ClientPoint);
  	/*set up a tracker rectangle around the selected object*/
  		if (ClickedGraphicObject = m_ProbabilityGrid->m_RWGraphicObjectList->PointInsideObject(RobotPoint, float(cWorldGridPointSize / m_ProbabilityGrid->GetZoomLevel()))) {
  //			CRectTracker* tracker;
  			m_SelectedGraphicObject = ClickedGraphicObject;
  			m_SelectedGraphicObject->SetEditMode(emSelected);
  
  //			tracker = m_SelectedGraphicObject->SetupTracker(this);
  			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  		}
  	}
  	// CG: This block was added by the Pop-up Menu component
  	{
  		if (point.x == -1 && point.y == -1) {
  			//keystroke invocation
  			CRect rect;
  			GetClientRect(rect);
  			ClientToScreen(rect);
  
  			point = rect.TopLeft();
  			point.Offset(5, 5);
  		}
  
  		CMenu menu;
  		VERIFY(menu.LoadMenu(CG_IDR_POPUP_ROBOT_WORLD_VIEW));
  
  		CMenu* pPopup = menu.GetSubMenu(0);
  		ASSERT(pPopup != NULL);
  		CWnd* pWndPopupOwner = this;
  
  		while (pWndPopupOwner->GetStyle() & WS_CHILD)
  			pWndPopupOwner = pWndPopupOwner->GetParent();
  
  		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
  	}
  }
  
  BOOL CRobotWorldView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
  {
  	if (pWnd == this && m_SelectedGraphicObject != NULL) {
  //		m_SelectedGraphicObject->SetupTracker(this);
  //		if (tracker->SetCursor(this, nHitTest)) return TRUE;
  		if (m_SelectedGraphicObject->SetTrackerCursor(this, nHitTest)) return TRUE;
  	}
  	if (nHitTest == HTCLIENT && nHitTest != HTVSCROLL && nHitTest != HTHSCROLL) {
  		switch (m_MouseMode) {
  		case mmMoveRobot:
  			::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MM_MOVE_ROBOT)));
  			break;
  		case mmEdit:
  			::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MMEDIT)));
  			break;
  		case mmErase:
  			::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MMERASE)));
  			break;
  		case mmPlaceVacuumPatch:
  			::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MMPLACEVACUUMPATCH)));
  			break;
  		case mmPlaceLinePath:
  			::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MM_PLACE_LINE_PATH)));
  			break;
  		case mmSearchArea:
  			::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MM_SEARCH)));
  			break;
  		case mmPlaceRedZone:
  			HCURSOR cursor;
  			cursor = (HCURSOR)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_MM_PLACE_RED_ZONE), IMAGE_CURSOR, 0, 0, LR_VGACOLOR | LR_SHARED | LR_DEFAULTSIZE);
  			if (cursor == NULL) {
  				DWORD error = GetLastError();
  			} else {
  				::SetCursor(cursor);
  			}
  			break;
  		case mmPlaceLabel:
  			::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MM_PLACE_POSTION_LABEL)));
  			break;
  		case mmPlaceFinishLine:
  			::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MM_PLACE_FINISH_LINE)));
  			break;
  		case mmPlaceStartLine:
  			::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MM_PLACE_START_LINE)));
  			break;
  		case mmDefault:
  			if (!m_MouseOverObject) {
  				::SetCursor(::LoadCursor(NULL,IDC_ARROW));
  			} else {
  				::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MM_HAND)));
  			}
  			break;
  //		case mmClearGrid:
  		case mmDrawLine:
  				::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MM_DRAW_LINE)));
  			break;
  //		case mmBlockGrid:
  		case mmDrawFreehand:
  				::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MM_DRAW_FREEHAND)));
  			break;
  //		case mmUnknownGrid:
  		case mmDrawRectangle:
  				::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MM_DRAW_RECTANGLE)));
  			break;
  		default:
  			::SetCursor(::LoadCursor(NULL,IDC_ARROW));
  			break;
  		}
  		return TRUE;
  	}
  	return CView::OnSetCursor(pWnd, nHitTest, message);
  }
  
  robRECT CRobotWorldView::ClientToRobotCoords(CRect rect)
  /*******************************************************************************
  //Created: 10/27/97 S.R.
  //Last Revision: 10/27/97 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	robRECT frect;
  	robPOINT fpoint;
  	POINT point;
  	point.x = rect.left;
  	point.y = rect.top;
  	fpoint = ClientToRobotCoords(point);
  	frect.left = fpoint.x;
  	frect.top = fpoint.y;
  	point.x = rect.right;
  	point.y = rect.bottom;
  	fpoint = ClientToRobotCoords(point);
  	frect.right = fpoint.x;
  	frect.bottom = fpoint.y;
  	return frect;
  }
  
  RECT CRobotWorldView::RobotToClientCoords(robRECT rect)
  /*******************************************************************************
  //Created: 10/27/97 S.R.
  //Last Revision: 10/27/97 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	RECT NewRect;
  	robPOINT fpoint;
  	POINT point;
  	fpoint.x = rect.left;
  	fpoint.y = rect.top;
  	point = RobotToClientCoords(fpoint);
  	NewRect.left = point.x;
  	NewRect.top = point.y;
  	fpoint.x = rect.right;
  	fpoint.y = rect.bottom;
  	point = RobotToClientCoords(fpoint);
  	NewRect.right = point.x;
  	NewRect.bottom = point.y;
  	return NewRect;
  }
  
  void CRobotWorldView::New()
  /*******************************************************************************
  //Created: 11/14/97 S.R.
  //Last Revision: 11/14/97 S.R.
  //Parameters:
  //	none
  //Return Value:
  //	none
  ********************************************************************************
  //Description:
  //		Call this function whenever the application wants a new document.
  //Method:
  //
  *******************************************************************************/
  {
  //	m_PenTool = ptPen;
  	SetPenSize(psSmall);
  	SetPenMode(pmObstacle);
  	m_SelectedGraphicObject = NULL;
  	m_DraggingRect = false;
  	m_DragLine = false;
  	m_MouseMode = mmDefault;
  	m_HomeBase = NULL;
  
  //	m_LabelList->Purge();
  	m_RedZoneList->Purge();
  	m_ProbabilityGrid->New();
  //	GetActiveRobot()->New();
  
  /*Reset the FlowZap dialog*/
  	if (m_DlgFlowZap != NULL) {
  		m_DlgFlowZap->SetLabelNames(NULL);
  		m_DlgFlowZap->SetVacuumPatches(NULL);
  		m_DlgFlowZap->SetCalibrationZones(NULL);
  		m_DlgFlowZap->SetLinePaths(NULL);
  	}
  	RedrawWindow();
  }
  
  void CRobotWorldView::ContextMenu(CWnd * pwnd, CPoint point)
  {
  	OnContextMenu(pwnd, point);
  }
  
  LRESULT CRobotWorldView::OnRobotFindPathFinished(WPARAM, LPARAM outputparams)
  {
  //	GetActiveRobot()->SetState(stWaiting);
  	TFindPathThreadOutput* Output = (TFindPathThreadOutput*)outputparams;
  	Path* FoundPath = Output->FoundPath;
  	CGoalSet* GoalSet = Output->GoalSet;
  	if (Output->CallBack != NULL) {
  		Output->CallBack(GoalSet, Output->CallBackParam1, Output->CallBackParam2);
  	}
  //	Path* FoundPath = (Path*)foundpath;
  #ifdef _IMPLEMENTED
  	if (GoalSet != NULL) {
  	} else {
  	}
  	if (FoundPath != NULL) {
  		CDC* pdc = GetDC();
  		DrawFoundPath(pdc, FoundPath, true);
  		double qualities[5];
  		FoundPath->GetQualities(qualities);
  		char buffer[10];
  		m_DlgMotorControls.m_edtMinFound = gcvt(qualities[MinFound], 7, buffer);
  		m_DlgMotorControls.m_edtAvgFound = gcvt(qualities[AveFound], 7, buffer);
  		m_DlgMotorControls.m_edtStartDist = gcvt(qualities[StartDist], 7, buffer);
  		m_DlgMotorControls.m_edtEndDist = gcvt(qualities[EndDist], 7, buffer);
  		m_DlgMotorControls.m_edtFreeDist = gcvt(qualities[FreeDist], 7, buffer);
  		
  		robPOINT P1;
  		FoundPath->GetPoint(0, P1.x, P1.y);
  		m_DlgMotorControls.m_edtStartX = gcvt(P1.x, 7, buffer);
  		m_DlgMotorControls.m_edtStartY = gcvt(P1.y, 7, buffer);
  
  		FoundPath->GetPoint(FoundPath->GetNumberOfPoints() - 1, P1.x, P1.y);
  		m_DlgMotorControls.m_edtEndX = gcvt(P1.x, 7, buffer);
  		m_DlgMotorControls.m_edtEndY = gcvt(P1.y, 7, buffer);
  
  		m_DlgMotorControls.UpdateData(FALSE);
  //		delete FoundPath;
  	} else {
  		m_DlgMotorControls.m_edtMinFound = "****";
  		m_DlgMotorControls.m_edtAvgFound = "****";
  		m_DlgMotorControls.m_edtStartDist = "****";
  		m_DlgMotorControls.m_edtEndDist = "****";
  		m_DlgMotorControls.m_edtFreeDist = "****";
  		m_DlgMotorControls.UpdateData(FALSE);
  	}
  	if (Output->CallBack != NULL) {
  		Output->CallBack(FoundPath, Output->CallBackParam1, Output->CallBackParam2, Output->GoodPath);
  	} else {
  		delete FoundPath;
  	}
  #endif
  	delete Output;
  	return 0;
  }
  
  LRESULT CRobotWorldView::OnRobotFindPathStage(WPARAM state, LPARAM blah)
  {
  	switch (state) {
  	case 0:
  		SetStatusBarText("Starting Find Path Calculations");
  		break;
  	case 1:
  		SetStatusBarText("Find Path Shortcut Didn't Work.  Trying Long Cut.");
  		break;
  	case 2:
  		if (blah) {
  			SetStatusBarText("Found a good path.");
  		} else {
  			SetStatusBarText("Did not find a good path.");
  		}
  		break;
  	case 3:
  		SetStatusBarText("Find Path Longcut didn't work.  Trying to go through the Grey.");
  		break;
  	case 4:
  		{
  		CString Status("Starting Find Path Calculations");
  		blah %= 30;
  		blah *= 5;
  		Status = "Starting Find Path Calculations ";
  		for (int i = 0; i < blah; i++) {
  			Status += '*';
  		}
  		SetStatusBarText(Status);
  		}
  		break;
  	}
  	return 0;
  }
  
  
  LRESULT CRobotWorldView::OnRobotFindPathError(WPARAM ErrorNumber, LPARAM)
  {
  	switch (ErrorNumber) {
  	case 1:
  		SetStatusBarText("FIND PATH PROBLEM: Goal is contained in an obstacle area");
  		break;
  	case 2:
  		SetStatusBarText("FIND PATH PROBLEM: Start is contained in an obstacle area");
  		break;
  	case 3:
  		SetStatusBarText("FIND PATH PROBLEM: No path to the goal found.  Consider making more room.");
  		break;
  	default:
  		ASSERT(FALSE);
  		break;
  	}
  	return 0;
  }
  
  
  LRESULT CRobotWorldView::OnRobotShowFindPath(WPARAM state, LPARAM path)
  {
  //	static Path* OldPath = NULL;
  	Path* FoundPath = (Path*)path;
  
  	CDC* pdc = GetDC();
  	CPen PathPen(PS_SOLID, 1, cFindPathPenColor);
  	CPen* OldPen = pdc->SelectObject(&PathPen);
  	int OldROP2 = pdc->SetROP2(R2_XORPEN);
  	
  	if (m_FindPathShown != NULL) {
  		DrawFoundPath(pdc, m_FindPathShown);
  	}
  	if (FoundPath != NULL) {
  		DrawFoundPath(pdc, FoundPath);
  		Path* temp = m_FindPathShown;
  		m_FindPathShown = FoundPath;
  		delete temp;
  	}
  //Check if finished path
  	if (state == 2) {
  		delete m_FindPathShown;
  		m_FindPathShown = NULL;
  	}
  	pdc->SetROP2(OldROP2);
  	pdc->SelectObject(OldPen);
  	PathPen.DeleteObject();
  	return 0;
  }
  
  
  LRESULT CRobotWorldView::OnRobotChangeState(WPARAM State, LPARAM)
  {
  
  	if ((TState)State == stWaiting) {
  		GetMainFrame()->OnEnableRobotPCControl(FALSE);
  	} else {
  		GetMainFrame()->OnEnableRobotPCControl(TRUE);
  	}
  	return 0;
  }
  
  void CRobotWorldView::DrawFoundPath(CDC* pdc, Path * path, bool ShowRobotBounds)
  {
  	robPOINT P1, P2, P3;
  	if (path == NULL) return;
  	path->GetPoint(0, P1.x, P1.y);
  	CPoint P;
  	P = RobotToClientCoords(P1);
  	pdc->MoveTo(P);
  	int nPoints = path->GetNumberOfPoints();
  	for (int i = 1; i < nPoints; i++) {
  
  		path->GetPoint(i, P1.x, P1.y);
  
  		P = RobotToClientCoords(P1);
  		pdc->LineTo(P);
  	}
  	if (ShowRobotBounds) {
  		for (int i = 1; i < nPoints; i++) {
  			path->GetPoint(i-1, P1.x, P1.y);
  			path->GetPoint(i, P2.x, P2.y);
  			double theta;
  			theta = atan2(P2.y - P1.y, P2.x - P1.x);
  			P3.x = P1.x + cRRWidth/2*cos(theta+pi/2);
  			P3.y = P1.y + cRRWidth/2*sin(theta+pi/2);
  			P = RobotToClientCoords(P3);
  			pdc->MoveTo(P);
  			P3.x = P2.x + cRRWidth/2*cos(theta+pi/2);
  			P3.y = P2.y + cRRWidth/2*sin(theta+pi/2);
  			P = RobotToClientCoords(P3);
  			pdc->LineTo(P);
  
  			P3.x = P1.x + cRRWidth/2*cos(theta-pi/2);
  			P3.y = P1.y + cRRWidth/2*sin(theta-pi/2);
  			P = RobotToClientCoords(P3);
  			pdc->MoveTo(P);
  			P3.x = P2.x + cRRWidth/2*cos(theta-pi/2);
  			P3.y = P2.y + cRRWidth/2*sin(theta-pi/2);
  			P = RobotToClientCoords(P3);
  			pdc->LineTo(P);
  		}
  	}
  }
  
  void CRobotWorldView::DrawCTDPath(CDC* pdc, CCTDPath * path, bool ShowRobotBounds)
  {
  	robPOINT P1, P2, P3;
  	if (path == NULL) return;
  	P1 = path->m_Path[0];
  	CPoint P;
  	P = RobotToClientCoords(P1);
  	pdc->MoveTo(P);
  	int nPoints = path->m_Size;
  	for (int i = 1; i < nPoints; i++) {
  
  		P1 = path->m_Path[i];
  
  		P = RobotToClientCoords(P1);
  		pdc->LineTo(P);
  	}
  	if (ShowRobotBounds) {
  		for (int i = 1; i < nPoints; i++) {
  			P1 = path->m_Path[i-1];
  			P2 = path->m_Path[i];
  			double theta;
  			theta = atan2(P2.y - P1.y, P2.x - P1.x);
  			P3.x = P1.x + cRRWidth/2*cos(theta+pi/2);
  			P3.y = P1.y + cRRWidth/2*sin(theta+pi/2);
  			P = RobotToClientCoords(P3);
  			pdc->MoveTo(P);
  			P3.x = P2.x + cRRWidth/2*cos(theta+pi/2);
  			P3.y = P2.y + cRRWidth/2*sin(theta+pi/2);
  			P = RobotToClientCoords(P3);
  			pdc->LineTo(P);
  
  			P3.x = P1.x + cRRWidth/2*cos(theta-pi/2);
  			P3.y = P1.y + cRRWidth/2*sin(theta-pi/2);
  			P = RobotToClientCoords(P3);
  			pdc->MoveTo(P);
  			P3.x = P2.x + cRRWidth/2*cos(theta-pi/2);
  			P3.y = P2.y + cRRWidth/2*sin(theta-pi/2);
  			P = RobotToClientCoords(P3);
  			pdc->LineTo(P);
  		}
  	}
  }
  
  void CRobotWorldView::OnObjectCaptionChanged(CString OldCaption, CString NewCaption, TRWGraphicObjectType ObjectType)
  {
  	GetDocument()->SetModifiedFlag();
  	if (m_DlgFlowZap != NULL) {
  		switch(ObjectType) {
  		case goPosition:
  			m_DlgFlowZap->ChangeLabel(OldCaption, NewCaption);
  			break;
  		case goCalibrationZone:
  			m_DlgFlowZap->ChangeCalibrationZone(OldCaption, NewCaption);
  			break;
  		case goVacuumPatch:
  			m_DlgFlowZap->ChangeVacuumPatch(OldCaption, NewCaption);
  			break;
  		case goLinePath:
  			m_DlgFlowZap->ChangeLinePath(OldCaption, NewCaption);
  			break;
  		default:
  //			fprintf(stderr, "OnObjectCaptionChanged Error!!!! Unhandled ObjectType\n");
  			ASSERT(false);
  			break;
  		}
  	}
  }
  
  void CRobotWorldView::OnObjectErased(CString Caption, TRWGraphicObjectType ObjectType)
  {
  	GetDocument()->SetModifiedFlag();
  	if (m_DlgFlowZap != NULL) {
  		switch(ObjectType) {
  		case goPosition:
  			m_DlgFlowZap->RemoveLabel(Caption);
  			break;
  		case goCalibrationZone:
  			m_DlgFlowZap->RemoveCalibrationZone(Caption);
  			break;
  		case goVacuumPatch:
  			m_DlgFlowZap->RemoveVacuumPatch(Caption);
  			break;
  		case goLinePath:
  			m_DlgFlowZap->RemoveLinePath(Caption);
  			break;
  		default:
  //			fprintf(stderr, "OnObjectErased Error!!!! Unhandled ObjectType\n");
  			ASSERT(false);
  			break;
  		}
  	}
  }
  
  void CRobotWorldView::OnObjectAdded(CString Caption, TRWGraphicObjectType ObjectType)
  {
  	GetDocument()->SetModifiedFlag();
  	if (m_DlgFlowZap != NULL) {
  		switch(ObjectType) {
  		case goPosition:
  			m_DlgFlowZap->AddLabel(Caption);
  			break;
  		case goCalibrationZone:
  			m_DlgFlowZap->AddCalibrationZone(Caption);
  			if (Caption == "Home Base") m_DlgFlowZap->AddLabel(Caption);
  			break;
  		case goVacuumPatch:
  			m_DlgFlowZap->AddVacuumPatch(Caption);
  			break;
  		case goLinePath:
  			m_DlgFlowZap->AddLinePath(Caption);
  			break;
  		default:
  //			fprintf(stderr, "OnObjectAdded Error!!!! Unhandled ObjectType\n");
  			ASSERT(false);
  			break;
  		}
  	}
  }
  
  void CRobotWorldView::DisplayLowRadioLevelDialog()
  {
  	if (m_DlgLowRadioLevel == NULL) {
  		m_DlgLowRadioLevel = new CDialog();
  		m_DlgLowRadioLevel->Create(IDD_LOW_RADIO_LEVEL, this);
  	}
  	m_DlgLowRadioLevel->ShowWindow(SW_SHOW);
  }
  
  CRect CRobotWorldView::CalculateUpdateRect(robPOINT OldRobotPosition, robPOINT OldRobotError, robPOINT NewRobotPosition, robPOINT NewRobotError)
  {
  	CRect UpdateRect;
  
  //Move the update rectangle to include the old robot error
  	robRECT ErrorRect;
  	ErrorRect.left = OldRobotPosition.x - (OldRobotError.x + cRRWidth);
  	ErrorRect.right = OldRobotPosition.x + (OldRobotError.x + cRRWidth);
  	ErrorRect.top = OldRobotPosition.y + (OldRobotError.y + cRRWidth);
  	ErrorRect.bottom = OldRobotPosition.y - (OldRobotError.y + cRRWidth);
  	UpdateRect = RobotToClientCoords(ErrorRect);
  	
  //Move the update rectangle to include the new robot error
  	ErrorRect.left = NewRobotPosition.x - (NewRobotError.x + cRRWidth);
  	ErrorRect.right = NewRobotPosition.x + (NewRobotError.x + cRRWidth);
  	ErrorRect.top = NewRobotPosition.y + (NewRobotError.y + cRRWidth);
  	ErrorRect.bottom = NewRobotPosition.y - (NewRobotError.y + cRRWidth);
  	CRect Rect = RobotToClientCoords(ErrorRect);
  	UpdateRect.UnionRect(&Rect, &UpdateRect);
  	
  	return UpdateRect;
  }
  
  void CRobotWorldView::SetTestRobotDialogVisible(bool Visible)
  {
  	CRobot* Robot = GetActiveRobot();
  	if (Robot != NULL) {
  		if (Visible) {
  			if (m_DlgTestRobot == NULL) {
  				m_DlgTestRobot = new CDlgTestRobot;
  				m_DlgTestRobot->Create(IDD_TEST_ROBOT, this);
  				m_DlgTestRobot->m_Robot = GetActiveRobot();
  			}
  			m_DlgTestRobot->ShowWindow(SW_SHOW);
  		} else {
  			if (m_DlgTestRobot != NULL) m_DlgTestRobot->ShowWindow(SW_HIDE);
  		}
  	}
  }
  
  bool CRobotWorldView::GetTestRobotDialogVisible()
  {
  	if (m_DlgTestRobot == NULL) return false;
  	return (m_DlgTestRobot->IsWindowVisible() == TRUE);
  }
  
  /*void CRobotWorldView::SetMouseControlsDialogVisible(bool Visible)
  {
  	if (Visible) {
  		if (m_DlgMouseControls == NULL) {
  			m_DlgMouseControls = new CDlgMouseControls;
  			m_DlgMouseControls->m_Robot = m_Robot;
  			m_DlgMouseControls->Create(IDD_MOUSE_CONTROLS, this);
  		}
  		m_DlgMouseControls->ShowWindow(SW_SHOW);
  	} else {
  		if (m_DlgMouseControls != NULL) m_DlgMouseControls->ShowWindow(SW_HIDE);
  	}
  }
  */
  
  /*bool CRobotWorldView::GetMouseControlsDialogVisible()
  {
  	if (m_DlgMouseControls == NULL) return false;
  	return (m_DlgMouseControls->IsWindowVisible() == TRUE);
  	return false;
  }
  */
  BOOL CRobotWorldView::ShowTip(int state)
  {
  	robPOINT Error;
  	robPOINT Position;
  	const robPOINT Origin(0,0);
  	const int NextTipClicks = 50;
  	BOOL Show = FALSE;
  
  	if (m_LaygoNumberClicks <= NextTipClicks) m_LaygoNumberClicks++;
  	CRobot* Robot = GetActiveRobot();
  	if (Robot != NULL) {
  		switch (state) {
  		case (CLaygo::lgoWelcome):
  		case (CLaygo::lgoChooseRobotColor):
  		case (CLaygo::lgoComPortSearch):
  		case (CLaygo::lgoHomeBaseCreateState):
  		case (CLaygo::lgoNavigating):
  			Show =  TRUE;
  			break;
  		case (CLaygo::lgoOccupancyGrid):
  			Position = Robot->GetPosition();
  			if (Distance(Position, Origin) > 24*cTPI) Show =  TRUE;
  			break;
  		case (CLaygo::lgoPositionUncertainty):
  			Error = Robot->GetPositionError();
  			if (sqrt(SQR(Error.x) + SQR(Error.y)) > 6*cTPI) Show =  TRUE;
  			break;
  		case (CLaygo::lgoCheckPoint):
  			Error = Robot->GetPositionError();
  			if (sqrt(SQR(Error.x) + SQR(Error.y)) > 12*cTPI) Show =  TRUE;
  			break;
  		case (CLaygo::lgoAdditionalCheckPoints):
  			Position = Robot->GetPosition();
  			if ( Distance(Position, Origin) > 240*cTPI ) Show =  TRUE;
  			if (m_LaygoNumberClicks == NextTipClicks) Show = TRUE;
  			break;
  		case(CLaygo::lgoHotPoints):
  			if ( m_ProbabilityGrid->GetPercentExploredArea() >= 0.5 ) Show =  TRUE;
  			if (m_LaygoNumberClicks == NextTipClicks) Show = TRUE;
  			break;
  		case(CLaygo::lgoVacuumPaths):
  			if ( m_ProbabilityGrid->GetPercentExploredArea() >= 1 ) Show =  TRUE;
  			if (m_LaygoNumberClicks == NextTipClicks) Show = TRUE;
  			break;
  		case(CLaygo::lgoDangerZones):
  			if ( m_ProbabilityGrid->GetPercentExploredArea() >= 1.5 ) Show =  TRUE;
  			if (m_LaygoNumberClicks == NextTipClicks) Show = TRUE;
  			break;
  		case(CLaygo::lgoFlowZap):
  			if (m_ProbabilityGrid->m_RWGraphicObjectList->NumberOfNodes() > 6) Show =  TRUE;
  			if (m_LaygoNumberClicks == NextTipClicks) Show = TRUE;
  			break;
  		default:
  			ASSERT( (state >= 0) && (state <= (CLaygo::cNonlinearLaygoes)) );
  			break;
  		}
  		if (Show == TRUE) m_LaygoNumberClicks = 0;
  	}
  	return Show;
  }
  
  short CRobotWorldView::GetPenWidth()
  {
  //	if (m_DlgDrawingTools != NULL) {
  //		m_PenSize = (TPenSize)m_DlgDrawingTools->GetPenSize();
  //	}
  	if (m_PenSize == psSmall) {
  		if (m_ProbabilityGrid->m_ZoomLevel == cZoomLevel1) {
  			return 4;
  		} else if (m_ProbabilityGrid->m_ZoomLevel == cZoomLevel2) {
  			return 2;
  		} else {
  			return 1;
  		}
  	} else if (m_PenSize == psMedium) {
  		if (m_ProbabilityGrid->m_ZoomLevel == cZoomLevel1) {
  			return 8;
  		} else if (m_ProbabilityGrid->m_ZoomLevel == cZoomLevel2) {
  			return 4;
  		} else {
  			return 2;
  		}
  	} else if (m_PenSize == psLarge) {
  		if (m_ProbabilityGrid->m_ZoomLevel == cZoomLevel1) {
  			return 16;
  		} else if (m_ProbabilityGrid->m_ZoomLevel == cZoomLevel2) {
  			return 8;
  		} else {
  			return 4;
  		}
  	} else {
  		if (m_ProbabilityGrid->m_ZoomLevel == cZoomLevel1) {
  			return 24;
  		} else if (m_ProbabilityGrid->m_ZoomLevel == cZoomLevel2) {
  			return 12;
  		} else {
  			return 6;
  		}
  	}
  }
  
  void CRobotWorldView::SetDrawingToolsVisible(bool Visible)
  {
  /*	if (Visible) {
  		if (m_DlgDrawingTools == NULL) {
  			m_DlgDrawingTools = new CDlgDrawingTools;
  			m_DlgDrawingTools->Create(IDD_DRAWING_TOOLS, this);
  			m_DlgDrawingTools->SetPenSize(m_PenSize);
  			m_DlgDrawingTools->SetPenTool(m_PenTool);
  		}
  		m_DlgDrawingTools->ShowWindow(SW_SHOW);
  	} else {
  		m_DlgDrawingTools->ShowWindow(SW_HIDE);
  	}
  */
  }
  
  void CRobotWorldView::SetPenMode(TPenMode Mode)
  {
  	m_PenMode = Mode;
  	GetMainFrame()->OnSetDrawingPenMode(Mode);
  }
  
  void CRobotWorldView::SetPenSize(TPenSize Size)
  {
  	m_PenSize = Size;
  	GetMainFrame()->OnSetDrawingPenSize(Size);
  }
  
  bool CRobotWorldView::GetDrawingToolsVisible()
  {
  /*	if (m_DlgDrawingTools == NULL) return false;
  	return (m_DlgDrawingTools->IsWindowVisible() == TRUE);
  */ return false;
  }
  
  //CRobotWorldView::TPenTool CRobotWorldView::GetPenTool()
  //{
  //	if (m_DlgDrawingTools == NULL) return m_PenTool;
  //	return m_PenTool;
  //}
  
  void CRobotWorldView::DoModalPropertySheet()
  {
  	CRobot* Robot = GetActiveRobot();
  	if (Robot != NULL) {
  		m_DlgPropFloor.SetRobot(Robot);
  
  		m_DlgPropRobotColor.SetRobotColor(Robot->GetColor());
  		m_DlgJoystickControl.SetControlType(m_JoystickControlType);
  		m_DlgJoystickControl.SetOrientation(m_JoystickOrientation);
  		m_DlgJoystickControl.SetResponsiveness(m_JoystickResponsiveness);
  		m_DlgJoystickControl.SetTurnGain(m_JoystickTurnGain);
  	//	char Kp,Ki,Kd,Kb,KPwm;
  	//	Robot->GetMotorControlConstants(Kp, Ki, Kd, Kb, KPwm);
  	//	m_DlgMouseControls.SetMotorControlConstants(Kp, Ki, Kd, Kb, KPwm);
  	//	m_DlgMouseControls.SetFlipHandle(Robot->GetFlipHandle());
  	//	m_DlgMouseControls.SetHandleLength(Robot->GetHandleLength());
  	//	m_DlgDrawingTools.SetPenSize((short)m_PenSize);
  	//	m_DlgDrawingTools.SetPenTool((short)m_PenTool);
  		m_DlgPropGeneral.SetShowCalibrationResults(Robot->GetShowCalibrationSuccessfulDialog());
  		m_DlgPropGeneral.SetIntermittentBeep(Robot->GetIntermittentBeep());
  		m_DlgPropGeneral.SetFlipHandle(Robot->GetFlipHandle());
  	//	m_DlgPropGeneral.SetViewVelocityTorque(GetShowVelocityTorqueDisplay());
  	//	m_DlgPropGeneral.SetTurnOffRobot(false);
  		m_DlgPropGeneral.SetLearnRoom(Robot->GetLearnRoom());
  		m_DlgPropGeneral.SetAnimateRobot(m_ProbabilityGrid->GetRobotAnimate());
  		m_DlgPropGeneral.SetEnableDancing(Robot->GetEnableDancing());
  		m_DlgPropGeneral.m_Robot = Robot;
  		m_DlgPropFloor.SetNormalSlipPercent(Robot->GetThinCarpetSlipPercent());
  		m_DlgPropFloor.SetThickSlipPercent(Robot->GetThickCarpetSlipPercent());
  		switch (Robot->GetFloorType()) {
  		case (ftWood):
  			m_DlgPropFloor.SetFloorType(0);
  			break;
  		case (ftThinCarpet):
  			m_DlgPropFloor.SetFloorType(1);
  			break;
  		case (ftThickCarpet):
  			m_DlgPropFloor.SetFloorType(2);
  			break;
  		}
  		m_DlgPropertySheet.m_psh.dwFlags |= PSH_HASHELP;
  		int Result = m_DlgPropertySheet.DoModal();
  		if (Result == IDOK) {
  			Robot->SetColor((TRobotColor)m_DlgPropRobotColor.GetRobotColor());
  			m_JoystickControlType = m_DlgJoystickControl.GetControlType();
  			m_JoystickOrientation = m_DlgJoystickControl.GetOrientation();
  			m_JoystickResponsiveness = m_DlgJoystickControl.GetResponsiveness();
  			m_JoystickTurnGain = m_DlgJoystickControl.GetTurnGain();
  	//		m_DlgMouseControls.GetMotorControlConstants(Kp, Ki, Kd, Kb, KPwm);
  	//		Robot->SetMotorControlConstants(Kp, Ki, Kd, Kb, KPwm);
  			SetFlipHandle(m_DlgPropGeneral.GetFlipHandle());
  	//		Robot->SetHandleLength(m_DlgMouseControls.GetHandleLength());
  	//		m_PenSize = (TPenSize)m_DlgDrawingTools.GetPenSize();
  	//		m_PenTool = (TPenTool)m_DlgDrawingTools.GetPenTool();
  			Robot->SetShowCalibrationSuccessfulDialog(m_DlgPropGeneral.GetShowCalibrationResults());
  			Robot->SetIntermittentBeep(m_DlgPropGeneral.GetIntermittentBeep());
  			SetEnableDancing(m_DlgPropGeneral.GetEnableDancing());
  			SetLearnRoom(m_DlgPropGeneral.GetLearnRoom());
  			m_ProbabilityGrid->SetRobotAnimate(m_DlgPropGeneral.GetAnimateRobot());
  	//		SetShowVelocityTorqueDisplay(m_DlgPropGeneral.GetViewVelocityTorque());
  			Robot->SetThinCarpetSlipPercent(m_DlgPropFloor.GetNormalSlipPercent());
  			Robot->SetThickCarpetSlipPercent(m_DlgPropFloor.GetThickSlipPercent());
  			switch (m_DlgPropFloor.GetFloorType()) {
  			case 0:
  				Robot->SetFloorType(ftWood);
  				break;
  			case 1:
  				Robot->SetFloorType(ftThinCarpet);
  				break;
  			case 2:
  				Robot->SetFloorType(ftThickCarpet);
  				break;
  			}
  	//		if (m_DlgPropGeneral.GetTurnOffRobot()) Robot->TurnOffPower();
  		}
  		RedrawWindow();
  	}
  }
  
  void CRobotWorldView::ResetLaygo()
  {
  	m_DlgLaygo->ResetLaygoTips();	
  	if (GetEnableLaygo()) {
  		ShowLaygoTip(1, true);
  	}
  }
  
  void CRobotWorldView::SetEnableLaygo(bool Enable)
  {
  	m_DlgLaygo->m_EnableLaygo = Enable;
  }
  
  bool CRobotWorldView::GetEnableLaygo()
  {
  	return m_DlgLaygo->m_EnableLaygo;
  }
  
  void CRobotWorldView::OnDestroy() 
  {
  	CView::OnDestroy();
  	
  	m_DlgLaygo->SaveState();
  	
  /*Turn off the MhCommControl before destroying the window*/
  //	GetActiveRobot()->m_RobotComm->SetInBufferCount(0);
  //	GetActiveRobot()->m_RobotComm->SetOutBufferCount(0);
  //	GetActiveRobot()->m_RobotComm->SetPortOpen(FALSE);
  //	GetActiveRobot()->m_RobotComm->SetEnabled(FALSE);	
  //	fclose(m_OutFile);
  //	fclose(m_AllDataFile);
  }
  
  HBITMAP CRobotWorldView::GetLargeRobotBitmap()
  {
  	return m_ProbabilityGrid->GetLargeRobotBitmap();
  }
  
  bool CRobotWorldView::CreateHomebase(double Heading, bool CornerLeft, TFloorType FloorType)
  {
  	bool CreateResult = false;
  	CRobot* Robot = GetActiveRobot();
  	if ( (m_HomeBase == NULL) && (Robot != NULL) ) {
  		Robot->SetFloorType(FloorType);
  
  		if (Robot->GetFloorType() == ftWood) {
  			CDlgCreateHomeOnWood Warning;
  			if (Warning.DoModal() != IDOK) return false;
  //			Robot->DisplayErrorText("You cannot create a home base on wood or tile");
  //			return false;
  		}
  
  		if ( (Robot->GetFloorType() == ftWood) && (Robot->GetAttachmentType() == atVacuum) ) {
  			CDlgErrorCheckingWoodWithVacuum Warning;
  			Warning.DoModal();
  			return false;
  		}
  		TRobotInfo OldRobotInfo = Robot->GetRobotInfo();
  
  		//bool OldEStopVisible = Robot->GetEStopVisible();
  		Robot->SetEStopVisible(true);
  		Robot->SetEnableRepositionOnUnknownObstacle(false);
  		robPOINT Origin;
  		Origin.x = 0;
  		Origin.y = 0;
  		//TState OldState = Robot->GetState();
  		Robot->SetState(stExecuting);
  		//bool OldUpdateProbabilityGrid = Robot->m_UpdateProbabilityGrid;
  //		Robot->SetLearnRoom(false);
  		m_HomeBase = new CHomeBase();
  		if (CreateResult = m_HomeBase->Create(Robot, CornerLeft, Heading, true)) {
  			m_ProbabilityGrid->m_ExecutableObjectList->Add(m_HomeBase);
  			m_ProbabilityGrid->m_RWGraphicObjectList->Add(m_HomeBase);
  			m_HomeBase->SetCaption("Home Base");
  		} else {
  			delete m_HomeBase;
  			m_HomeBase = NULL;
  //			m_DlgLaygo->Configure(CLaygo::lgoHomeBaseCreateState);
  //			m_DlgLaygo->ShowWindow(SW_SHOW);
  		}
  		RedrawWindow();
  //		Robot->SetState(OldState);
  //		Robot->m_UpdateProbabilityGrid = OldUpdateProbabilityGrid;
  //		Robot->SetEStopVisible(OldEStopVisible);
  		Robot->SetRobotInfo(OldRobotInfo);
  		return CreateResult;
  	}
  	return true;
  }
  
  void CRobotWorldView::OnPowerOff() 
  {
  	GetActiveRobot()->TurnOffPower();	
  }
  
  void CRobotWorldView::OnNewDocument()
  {
  	New();
  	int CurrentState = m_DlgLaygo->GetCurrentState();
  	ShowLaygoTip(CLaygo::lgoChooseRobotColor, false);
  }
  
  #include "DlgLinePathProperties.h"
  void CRobotWorldView::OnLButtonDblClk(UINT nFlags, CPoint point) 
  {
  	if (m_CurrentLinePath != NULL) {
  		if (m_MouseMode == mmPlaceLinePath) {
  			robPOINT RobotPoint = ClientToRobotCoords(point);
  			
  			m_LinePathTo = m_ProbabilityGrid->m_ExecutableObjectList->PointNearExecutableObjectExit(RobotPoint, m_CurrentLinePath);
  			if (m_LinePathTo == m_CurrentLinePath) m_LinePathTo = NULL;
  			if (m_LinePathTo != NULL) {
  				RobotPoint = m_LinePathTo->GetExitPoint();
  			}
  			CString Caption = m_CurrentLinePath->GetCaption();
  			m_CurrentLinePath->AddPoint(RobotPoint);
  			if (m_LinePathFrom != NULL) {
  				if (m_LinePathTo != NULL) {
  					Caption.Format("%s / %s Path", m_LinePathFrom->GetCaption(), m_LinePathTo->GetCaption());
  				} else {
  					Caption.Format("%s Path", m_LinePathFrom->GetCaption());
  				}
  			} else {
  				if (m_LinePathTo != NULL) {
  					Caption.Format("To %s", m_LinePathTo->GetCaption());
  				}
  			}
  			m_CurrentLinePath->SetCaption(Caption);
  
  			CDlgLinePathProperties LinePathProperties;
  			LinePathProperties.m_edtName = m_CurrentLinePath->GetCaption();
  			if (LinePathProperties.DoModal() == IDOK) {
  				m_CurrentLinePath->SetCaption(LinePathProperties.m_edtName);
  				RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  			} else {
  				m_CurrentLinePath->OnErase();
  				m_ProbabilityGrid->m_RWGraphicObjectList->Remove(m_CurrentLinePath);
  				RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  			}
  			m_CurrentLinePath = NULL;	
  //			m_CurrentLinePathEnded = true;
  			m_DragLine = false;
  			m_LinePathTo = NULL;
  			m_LinePathFrom = NULL;
  		}
  	}
  	CView::OnLButtonDblClk(nFlags, point);
  }
  
  void CRobotWorldView::OnViewFlowZap()
  {
  	if (m_DlgLaygo != NULL) {
  		if (!m_DlgLaygo->IsWindowVisible()) {
  			if ( (GetLaygoTip() <= CLaygo::lgoFlowZap) && (!m_DlgLaygo->GetShownFlowZap()) ) {
  				ShowLaygoTip(CLaygo::lgoFlowZap, false);
  			}
  		}
  	}
  	if (m_DlgFlowZap == NULL) {
  		CreateFlowZap();
  		m_DlgFlowZap->ShowWindow(SW_SHOW);
  	} else {
  		if (m_DlgFlowZap->IsWindowVisible()) {
  			m_DlgFlowZap->ShowWindow(SW_HIDE);
  		} else {
  			m_DlgFlowZap->ShowWindow(SW_SHOW);
  //			m_DlgFlowZap->InitInstructionList();
  		}
  	}
  }
  
  bool CRobotWorldView::GetVacuumAttached()
  {
  	if (GetActiveRobot() == NULL) return false;
  	return (GetActiveRobot()->GetAttachmentType() == atVacuum);
  }
  
  void CRobotWorldView::SetVacuumAttached(bool attached)
  {
  	CRobot* Robot = GetActiveRobot();
  	if (Robot != NULL) {
  		if (attached) Robot->SetAttachmentType(atVacuum);
  		else Robot->SetAttachmentType(atNone);
  	}
  }
  
  bool CRobotWorldView::SetFlowZapFile(CString FileSelected)
  {
  	if (m_DlgFlowZap == NULL) {
  		CreateFlowZap();
  	}
  	return m_DlgFlowZap->OpenFile(FileSelected);
  }
  
  void CRobotWorldView::FlowZapPlay()
  {
  	if (m_DlgFlowZap == NULL) {
  		CreateFlowZap();
  	}
  	m_DlgFlowZap->OnPlayStart();
  }
  
  void CRobotWorldView::FlowZapPause()
  {
  	if (m_DlgFlowZap == NULL) {
  		CreateFlowZap();
  	}
  	m_DlgFlowZap->OnPlayPause();
  }
  
  void CRobotWorldView::FlowZapStop()
  {
  	if (m_DlgFlowZap == NULL) {
  		CreateFlowZap();
  	}
  	m_DlgFlowZap->OnPlayStop();
  }
  
  void CRobotWorldView::CreateFlowZap()
  {
  //Create the FlowZap window
  	m_DlgFlowZap = new CFlowZap(this, m_ProbabilityGrid->m_ExecutableObjectList);
  
  	CStringList* PositionLabelNames = new CStringList;
  	CStringList* VacuumPatchLabelNames = new CStringList;
  	CStringList* CalibrationZoneLabelNames = new CStringList;
  	CStringList* LinePaths = new CStringList;
  	m_ProbabilityGrid->m_ExecutableObjectList->GetCaptionLists(PositionLabelNames, CalibrationZoneLabelNames, VacuumPatchLabelNames, LinePaths);
  //	if (m_HomeBase != NULL) PositionLabelNames->AddHead(m_HomeBase->GetCaption());
  	m_DlgFlowZap->SetLabelNames(PositionLabelNames);
  	m_DlgFlowZap->SetVacuumPatches(VacuumPatchLabelNames);
  	m_DlgFlowZap->SetCalibrationZones(CalibrationZoneLabelNames);
  	m_DlgFlowZap->SetLinePaths(LinePaths);
  
  	m_DlgFlowZap->Create(IDD_FLOWZAP, this);
  }
  
  void CRobotWorldView::OnFlowZapChangeFileOpen(CString OpenFile)
  {
  	GetMainFrame()->OnFzcChangeFileOpen(OpenFile);
  }
  
  void CRobotWorldView::SetStatusBarText(CString Text)
  {
  	GetMainFrame()->GetMessageBar()->SetWindowText(Text);
  }
  
  void CRobotWorldView::SetShowVelocityTorqueDisplay(bool Show)
  {
  	if (m_DlgVelocityTorqueDisplay == NULL) {
  		if (Show) {
  			m_DlgVelocityTorqueDisplay = new CDlgVelocityTorqueDisplay();
  			m_DlgVelocityTorqueDisplay->Create(CDlgVelocityTorqueDisplay::IDD, this);
  			m_DlgVelocityTorqueDisplay->ShowWindow(SW_SHOW);
  		}
  	} else {
  		if (Show) m_DlgVelocityTorqueDisplay->ShowWindow(SW_SHOW);
  		else m_DlgVelocityTorqueDisplay->ShowWindow(SW_HIDE);
  	}
  }
  
  bool CRobotWorldView::GetShowVelocityTorqueDisplay(void)
  {
  	if (m_DlgVelocityTorqueDisplay == NULL) return false;
  	else {
  		return (m_DlgVelocityTorqueDisplay->IsWindowVisible() == TRUE);
  	}
  }
  
  void CRobotWorldView::OnRobotCommunicationEstablished()
  {
  //	GetActiveRobot()->OnInitialCommunicationEstablished();
  	if ( GetEnableLaygo() && (m_DlgLaygo->GetCurrentState() == CLaygo::lgoComPortSearch) ) {
  		MessageBox("Established communications with robot.", "Communications Established", MB_ICONINFORMATION | MB_OK);
  	}
  }
  
  void CRobotWorldView::ShowLaygoTip(int State, bool SaveState)
  {
  	m_DlgLaygo->Configure(State, SaveState);
  	m_DlgLaygo->ShowWindow(SW_SHOW);
  }
  
  int CRobotWorldView::GetLaygoTip()
  {
  	return m_DlgLaygo->GetCurrentState();
  }
  
  void CRobotWorldView::LoadCyeSample(TCyeSample Sample)
  {
  	CDocument* document = GetDocument();
  
  	if (document->SaveModified()) {
  		CString strFlowZapFile = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  		CString  strMapFile = strFlowZapFile;
  		strFlowZapFile += "\\Samples\\";
  		strMapFile += "\\Samples\\";
  		DWORD HelpId = 0;
  
  		switch (Sample) {
  		case csCyeServe:
  			strMapFile += "Home.map";
  			strFlowZapFile += "CyeServe.zap";
  			HelpId = 0x10000 + ID_HELP_SAMPLES_CYESERVE;
  			break;
  		case csCyeTruck:
  			strMapFile += "Home.map";
  			strFlowZapFile += "CyeTruck.zap";
  			HelpId = 0x10000 + ID_HELP_SAMPLES_CYETRUCK;
  			break;
  		case csCyePup:
  			strMapFile += "Home.map";
  			strFlowZapFile += "CyePup.zap";
  			HelpId = 0x10000 + ID_HELP_SAMPLES_CYEPUP;
  			break;
  		case csCyeClock:
  			strMapFile += "Home.map";
  			strFlowZapFile += "CyeClock.zap";
  			HelpId = 0x10000 + ID_HELP_SAMPLES_CYECLOCK;
  			break;
  		case csCyePost:
  			strMapFile += "Office.map";
  			strFlowZapFile += "CyePost.zap";
  			HelpId = 0x10000 + ID_HELP_SAMPLES_CYEPOST;
  			break;
  		case csCyeGuide:
  			strMapFile += "Office2.map";
  			strFlowZapFile += "CyeGuide.zap";
  			HelpId = 0x10000 + ID_HELP_SAMPLES_CYEGUIDE;
  			break;
  		case csCyePatrol:
  			strMapFile += "Office.map";
  			strFlowZapFile += "CyePatrol.zap";
  			HelpId = 0x10000 + ID_HELP_SAMPLES_CYEPATROL;
  			break;
  		default:
  			ASSERT(FALSE);
  			break;
  		}
  
  		CFile CurrentFile;
  		if (!CurrentFile.Open(strMapFile, CFile::modeRead | CFile::shareDenyWrite)) {
  			AfxMessageBox("Could not load sample .map file");
  			return;
  		} else {
  			CArchive ar(&CurrentFile, CArchive::load);
  			ar.m_pDocument = document;
  			document->SetPathName(strMapFile, TRUE);
  			document->Serialize(ar);
  		}
  
  		if (!SetFlowZapFile(strFlowZapFile)) {
  			AfxMessageBox("Could not load sample .zap file");
  			return;
  		}
  		m_DlgFlowZap->ShowWindow(SW_SHOW);
  
  		AfxGetApp()->WinHelp(HelpId);
  	}
  }
  
  bool CRobotWorldView::GetShowRulerGrid()
  {
  	return m_ProbabilityGrid->m_RWGraphicObjectList->GetShowRulerGrid();
  }
  
  void CRobotWorldView::SetShowRulerGrid(bool View)
  {
  	m_ProbabilityGrid->m_RWGraphicObjectList->SetShowRulerGrid(View);
  }
  
  bool CRobotWorldView::GetCommunicationsEstablished()
  {
  	if (GetActiveRobot() == NULL) return false;
  	return GetActiveRobot()->GetCommunicationsEstablished();
  }
  
  void CRobotWorldView::FindComPort()
  {
  	if (GetActiveRobot() != NULL) {
  		GetActiveRobot()->FindComPort();
  	}
  }
  
  void CRobotWorldView::RedrawRobot(robPOINT Position, double Heading)
  {
  	CRobot* Robot = GetActiveRobot();
  	if (Robot != NULL) {
  		CRect UpdateRect = CalculateUpdateRect(Robot->GetDrawPosition(), Robot->GetPositionError(), Position, Robot->GetPositionError());
  		Robot->SetDrawPosition(Position);
  		Robot->SetDrawHeading(Heading);
  
  		RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	}
  }
  
  CRWGraphicObject* CRobotWorldView::GetSelectedGraphicObject()
  {
  	return m_SelectedGraphicObject;
  }
  
  void CRobotWorldView::EditSelectedGraphicObject()
  {
  	if (m_SelectedGraphicObject != NULL) {
  		m_SelectedGraphicObject->EditProperties();
  		m_SelectedGraphicObject->SetEditMode(emNone);
  		m_SelectedGraphicObject = NULL;
  		GetDocument()->SetModifiedFlag();
  		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);
  	}
  }
  
  void CRobotWorldView::DeleteSelectedGraphicObject()
  {
  	if (m_SelectedGraphicObject != NULL) {
  		if (m_SelectedGraphicObject != m_HomeBase) {
  			GetDocument()->SetModifiedFlag();
  			m_SelectedGraphicObject->OnErase();
  			m_ProbabilityGrid->m_RWGraphicObjectList->Remove(m_SelectedGraphicObject);
  			m_SelectedGraphicObject = NULL;
  			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  		}
  	}
  }
  
  bool CRobotWorldView::GetPause()
  {
  	if (GetActiveRobot() == NULL) return false;
  	return GetActiveRobot()->GetPause();
  }
  
  bool CRobotWorldView::GetPlay()
  {
  	bool playing = false;
  	if (m_DlgFlowZap != NULL) {
  		playing = m_DlgFlowZap->GetPlaying();
  	}
  	return playing;
  }
  
  bool CRobotWorldView::SafeAppShutdown()
  {
  	CRobot* Robot = GetActiveRobot();
  	bool Result = true;
  	if (Robot != NULL) {
  		Robot->EStop();
  		Robot->AddWaitForPingToInstructions(5000);
  		if (Robot->FollowInstructions() == IR_InstructionComplete) {
  			Result = true;
  		} else {
  			Result = false;
  		}
  		Robot->EStop();	//This second ESTop is necessary for shutting down when running instructions.  Don't ask me why.
  		GetMainFrame()->PostMessage(WM_CLOSE, 0, 0);
  	}
  	return Result;
  }
  
  void CRobotWorldView::AdjustSelectedCalibrationZone()
  {
  	if (m_SelectedGraphicObject != NULL) {
  		if ( m_SelectedGraphicObject->IsKindOf(RUNTIME_CLASS(CCalibrationZone)) ) {
  			((CCalibrationZone*)(m_SelectedGraphicObject))->Adjust(GetActiveRobot());
  		}
  		m_SelectedGraphicObject->SetEditMode(emNone);
  		m_SelectedGraphicObject = NULL;
  		GetDocument()->SetModifiedFlag();
  		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);
  	}
  }
  
  CCalibrationZone* CRobotWorldView::GetSelectedCalibrationZone()
  {
  	CCalibrationZone* CalZone = NULL;
  	if (m_SelectedGraphicObject != NULL) {
  		if ( m_SelectedGraphicObject->IsKindOf(RUNTIME_CLASS(CCalibrationZone)) ) {
  			CalZone = (CCalibrationZone*)m_SelectedGraphicObject;
  		}
  	}
  	return CalZone;
  }
  
  bool CRobotWorldView::GetAlreadyCalibratedFloorType(TFloorType FloorType)
  {
  	if (GetActiveRobot() != NULL) {
  		return GetActiveRobot()->GetAlreadyCalibratedFloorType(FloorType);
  	} else {
  		return false;
  	}
  }
  
  int CRobotWorldView::GetRobotColor()
  {
  	if (GetActiveRobot() != NULL) {
  		return m_ProbabilityGrid->GetActiveRobot()->GetColor();
  	} else {
  		return 0;
  	}
  }
  
  void CRobotWorldView::OnMicrophoneVolume(short Volume)
  {
  	m_DlgMotorControls.UpdateData(TRUE);
  	m_DlgMotorControls.m_edtMicrophoneVolume = Volume;
  	m_DlgMotorControls.UpdateData(FALSE);
  }
  
  bool CRobotWorldView::GetRobotExternalOutput()
  {
  	if (GetActiveRobot() != NULL) {
  		return GetActiveRobot()->GetExternalOutput();
  	} else {
  		return false;
  	}
  }
  
  void CRobotWorldView::SetRobotExternalOutput(bool On)
  {
  	if (GetActiveRobot() != NULL) {
  		GetActiveRobot()->SetExternalOutput(On);
  	}
  }
  
  void CRobotWorldView::SetActiveRobot(CRobot *Robot)
  {
  	m_ProbabilityGrid->SetActiveRobot(Robot);
  }
  
  CRobot* CRobotWorldView::GetActiveRobot()
  {
  	return m_ProbabilityGrid->GetActiveRobot();
  }
  
  void CRobotWorldView::OnCommunicationsEstablished() 
  {
  	if ( GetEnableLaygo() && (m_DlgLaygo->GetCurrentState() == CLaygo::lgoComPortSearch) ) {
  		MessageBox("Established communications with robot.", "Communications Established", MB_ICONINFORMATION | MB_OK);
  	}
  }
  
  void CRobotWorldView::OnBatteryLow()
  {
  	m_DlgBatteryLow.ShowWindow(SW_SHOW);
  }
  
  void CRobotWorldView::OnMotorCurrentUpdate(float RightMotor, float LeftMotor, CRobot* Robot)
  {
  	if (Robot == GetActiveRobot() ) {
  		if (m_DlgVelocityTorqueDisplay != NULL) m_DlgVelocityTorqueDisplay->SetTorque(LeftMotor, RightMotor);
  	}
  }
  
  void CRobotWorldView::OnMotorVelocityUpdate(short RightVelocity, short LeftVelocity, CRobot* Robot)
  {
  	if (Robot == GetActiveRobot() ) {
  		if (m_DlgVelocityTorqueDisplay != NULL) m_DlgVelocityTorqueDisplay->SetVelocity(LeftVelocity, RightVelocity);
  	}
  }
  
  void CRobotWorldView::AddRobot()
  {
  	CDlgAddRobot AddDialog;
  	if (AddDialog.DoModal() == IDOK) {
  		GetDocument()->SetModifiedFlag(TRUE);
  		CRobot* NewRobot = new CRobot(this, m_ProbabilityGrid);
  		NewRobot->Create(this);
  		NewRobot->SetVersion(AddDialog.GetRobotVersion());
  		NewRobot->SetColor(TRobotColor(AddDialog.GetRobotColor()));
  	//Set Parameters for the new robot.
  		CRobot* ActiveRobot = GetActiveRobot();
  		if (ActiveRobot != NULL) {
  			NewRobot->SetMaxSpeed(ActiveRobot->GetMaxSpeed());
  			NewRobot->SetFlipHandle(ActiveRobot->GetFlipHandle());
  			NewRobot->SetLearnRoom(ActiveRobot->GetLearnRoom());
  			NewRobot->SetEnableDancing(ActiveRobot->GetEnableDancing());
  		}
  		m_ProbabilityGrid->AddRobot(NewRobot);
  		m_ProbabilityGrid->SetActiveRobot(NewRobot);
  	}
  }
  
  CRobot* CRobotWorldView::GetRobot(short Index)
  {
  	return m_ProbabilityGrid->GetRobot(Index);
  }
  
  void CRobotWorldView::SetRobotColor(int RobotColor)
  {
  	if (GetActiveRobot() != NULL) {
  		GetActiveRobot()->SetColor(TRobotColor(RobotColor));
  	}
  }
  
  void CRobotWorldView::OnRadioLevelChange(float RadioLevel, CRobot *Robot)
  {
  	if (m_ProbabilityGrid != NULL) {
  		if (Robot == m_ProbabilityGrid->GetActiveRobot() ) {
  			GetMainFrame()->SetRadioLevel(RadioLevel);
  		}
  	}
  }
  
  void CRobotWorldView::OnBatteryChargingStateChange(int State, CRobot *Robot)
  {
  	if (m_ProbabilityGrid != NULL) {
  		if (Robot == m_ProbabilityGrid->GetActiveRobot() ) {
  			GetMainFrame()->OnBatteryChargeStateChange(State);
  		}
  	}
  }
  
  
  void CRobotWorldView::OnBatteryVoltageUpdate(float BatteryVoltage, CRobot *Robot)
  {
  	if (m_ProbabilityGrid != NULL) {
  		if (Robot == m_ProbabilityGrid->GetActiveRobot() ) {
  			GetMainFrame()->OnSetBatteryLevel(BatteryVoltage);
  		}
  	}
  }
  
  void CRobotWorldView::SetMaxSpeed(short Speed)
  {
  	m_ProbabilityGrid->SetRobotsMaxSpeed(Speed);
  //	GetActiveRobot()->SetMaxSpeed(Speed);
  }
  
  void CRobotWorldView::SetFlipHandle(bool FlipHandle)
  {
  	m_ProbabilityGrid->SetRobotsFlipHandle(FlipHandle);
  }
  
  void CRobotWorldView::SetLearnRoom(bool Learn)
  {
  	m_ProbabilityGrid->SetRobotsUpdateProbabilityGrid(Learn);
  }
  
  void CRobotWorldView::AddHomeBase()
  {
  	CRobot* Robot = GetActiveRobot();
  	if (Robot != NULL) {
  		SetAddingHomeBase(true);
  		if (m_DlgAddHomeBase == NULL) {
  			m_DlgAddHomeBase = new CDlgAddHomeBase();
  			m_DlgAddHomeBase->Create(IDD_ADD_HOMEBASE, this);
  		}
  		m_DlgAddHomeBase->SetRobotBitmap(m_ProbabilityGrid->GetLargeRobotBitmap());
  		m_DlgAddHomeBase->SetRobotHeading(Robot->GetHeading());
  		m_DlgAddHomeBase->SetRobotWorldView(this);
  		m_DlgAddHomeBase->ShowWindow(SW_SHOW);
  	}
  }
  
  void CRobotWorldView::SetEnableDancing(bool Enable)
  {
  	m_ProbabilityGrid->SetRobotsEnableDancing(Enable);
  }
  
  void CRobotWorldView::OnViewCyePersona() 
  {
  	CRobot* Robot = GetActiveRobot();
  	if (Robot != NULL) {
  		Robot->ShowCyePersonaDialog(true);
  	}
  }
  
  void CRobotWorldView::SetAddingHomeBase(bool Adding)
  {
  	m_ProbabilityGrid->SetAddingHomeBase(Adding);
  }
  
  void CRobotWorldView::SetRobotExternalInput(float Voltage)
  {
  	m_DlgMotorControls.UpdateData(TRUE);
  	m_DlgMotorControls.m_edtAnalogInput = Voltage;
  	m_DlgMotorControls.UpdateData(FALSE);
  	GetMainFrame()->OnExternalInputUpdate(Voltage);
  }
