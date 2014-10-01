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
  ///////////////////////////////////////////////////////////////////////////////
  //
  // Module:     RobotCom.cpp
  //
  // Abstract:   Invisible modeless dialog that houses the interface to the 
  //             RRRCom ActiveX control.
  //
  // Author:     J.C.H.
  //
  // Revisions:
  //    $History: RobotCom.cpp $
  // 
  // *****************  Version 4  *****************
  // User: Harnden      Date: 2/01/98    Time: 11:08p
  // Updated in $/RobotWorld
  // Corrected parameter passed to CreateDispatch in ExecutePath.
  // 
  // *****************  Version 3  *****************
  // User: Harnden      Date: 1/12/98    Time: 12:45a
  // Updated in $/RobotWorld
  // First delivery of ActiveX interface.
  // 
  // *****************  Version 2  *****************
  // User: Harnden      Date: 1/03/98    Time: 5:58p
  // Updated in $/RobotWorld
  // Completion of all ActiveX method interfaces for RobotWorld.
  // 
  // *****************  Version 1  *****************
  // User: Harnden      Date: 12/29/97   Time: 10:29a
  // Created in $/RobotWorld
  //
  // Copyright 1997 Cycle Time Corp
  //
  ///////////////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "RobotWorldDoc.h"
  #include "RobotWorldView.h"
  //#include "Robot.h"
  #include "RobotCom.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  enum PCXMIT_DATA {
  	SET_RAM_DATA_COMMAND_CHAR = 'n',
  	SET_ENABLE_OBSTACLE_BOUNCE_COMMAND_CHAR = 'q',
  	SET_HEADING_DESTINATION_EXTENDED = 't',
  	SEND_WALL_CALIBRATE_COMMAND_CHAR = 'w',
  	SEND_SQUARE_TO_WALL_COMMAND_CHAR = 'W',
  	SEND_FEELING_MOTOR_COMMAND_CHAR = 'C',
  	SET_EX1_COMMAND_CHAR = 'y',
  	GET_EX2_COMMAND_CHAR = 'x',
  	SEND_CORNER_CALIBRATE_COMMAND_CHAR = 'Z',
  	SEND_FIND_HOME_BASE_COMMAND_CHAR = 'z',
  	SEND_PAUSE_COMMAND_CHAR = 'u',
  	SET_ROBOT_ON_SECONDS_COMMAND_CHAR = 'l',
  	SET_INTERMITTENT_BEEP_ON_COMMAND_CHAR = 'k',
  	SET_POSITION_DELTA_COMMAND_CHAR = 'j',
  	SET_HEADING_DELTA_COMMAND_CHAR = 'J'
  };
  /////////////////////////////////////////////////////////////////////////////
  // RobotCom dialog
  const int cWaitForCommunicationsEstablished = 4;
  
  
  RobotCom::RobotCom(CRobotWorldView* pParent /*=NULL*/, CRobot* Robot)
  	: CDialog(RobotCom::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(RobotCom)
  		// NOTE: the ClassWizard will add member initialization here
  	//}}AFX_DATA_INIT
  
     // Create the modeless dialog
  	m_Robot = Robot;
     Create(RobotCom::IDD, pParent);
  	m_CommunicationsEstablished = false;
  	m_FirstCommunicationsEstablished = false;
  }
  
  
  void RobotCom::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(RobotCom)
  	DDX_Control(pDX, IDC_CYECOMCTRL1, m_CyeCom);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(RobotCom, CDialog)
  	//{{AFX_MSG_MAP(RobotCom)
  		// NOTE: the ClassWizard will add message map macros here
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // RobotCom message handlers
  
  void RobotCom::SendP4(long P4)
  {
  //	m_CyeCom.SendP4(P4);
  }
  
  void RobotCom::SendP3(long P3)
  {
  //	m_CyeCom.SendP3(P3);
  }
  
  void RobotCom::SendP2(long P2)
  {
  //	m_CyeCom.SendP2(P2);
  }
  
  void RobotCom::SendP1(long P1)
  {
  //	m_CyeCom.SendP1(P1);
  }
  
  void RobotCom::SendP5(long P)
  {
  //	m_CyeCom.SendP5(P);
  }
  
  void RobotCom::SendP6(long P)
  {
  //	m_CyeCom.SendP6(P);
  }
  
  void RobotCom::SendP7(long P)
  {
  //	m_CyeCom.SendP7(P);
  }
  
  void RobotCom::SendP8(long P)
  {
  //	m_CyeCom.SendP8(P);
  }
  
  void RobotCom::SendBuzzerFrequency(short BuzzerFrequency)
  {
     m_CyeCom.SetBuzzerFrequency(BuzzerFrequency);
  }
  
  void RobotCom::SendBuzzerOn(BOOL On)
  {
     m_CyeCom.SetBuzzerOn(On);
  }
  
  void RobotCom::SendPathSegment(BOOL PurgePath, robPOINT* Points, unsigned char nPoints)
  {
     // Create and fill a path object
     ICyePath Path;
     Path.CreateDispatch("CyeCom.Path");
     Path.SetNumPoints(nPoints);
     for (short i=0;i < nPoints;i++)
     {
        Path.SetX(i, Points[i].x);
        Path.SetY(i, Points[i].y);
     }
     m_CyeCom.LoadPathSegment(PurgePath, Path);
  }
  
  void RobotCom::SendFollowPath(int FollowPath)
  {
     m_CyeCom.FollowPath(FollowPath);
  }
  
  void RobotCom::SendHandleLength(short HandleLength)
  {
     m_CyeCom.SetHandleLength(HandleLength);
  }
  
  void RobotCom::SendMotorControlConstants(char Kp, char Ki, char Kd, char Kb, char KPwm)
  {
     m_CyeCom.SetMotorControlConstants(Kp, Ki, Kd, Kb, KPwm);
  }
  
  void RobotCom::SendTurn(double Heading, short Velocity, short Diameter)
  {
     m_CyeCom.MoveCurve(Heading, Velocity, Diameter);
  }
  
  void RobotCom::SendObstacleSensitivity(TObstacleSensitivity Sensitive)
  {
     m_CyeCom.SetObstacleSensitivity(Sensitive);
  }
  
  void RobotCom::SendStraightVelocity(int Velocity, double Heading)
  {
     m_CyeCom.MoveStraight(Velocity, Heading);
  }
  
  void RobotCom::SendPositionVelocityDestination(robPOINT Destination, short Velocity)
  {
     m_CyeCom.MoveTo(Destination.x, Destination.y, Velocity);
  }
  
  void RobotCom::SendMaxSpeed(short MaxSpeed)
  {
     m_CyeCom.SetMaxSpeed(MaxSpeed);
  }
  
  void RobotCom::SendHeading(double Heading)
  {
     m_CyeCom.SetHeading(Heading);
  }
  
  void RobotCom::SendPosition(robPOINT Position)
  {
  //	m_CyeCom.SetY(Position.x);
     m_CyeCom.SetPosition((long)Position.x, (long)Position.y);
  }
  
  void RobotCom::SendMotorVelocities(int M0Velocity, int M1Velocity)
  {
     m_CyeCom.SetWheelVelocities(M0Velocity, M1Velocity);
  }
  
  void RobotCom::SendHeadingCorrectionFactor(int Correction)
  {
     m_CyeCom.SetHeadingCorrectionFactor(Correction);
  }
  
  void RobotCom::SendFlipHandle(BOOL value)
  {
  	m_CyeCom.SetFlipHandle(value);
  }
  
  void RobotCom::SendMaxPWM(unsigned char MaxPWM)
  {
  //	m_CyeCom.SetMaxPWM(MaxPWM);
  }
  
  void RobotCom::SendHeadingDestination(double Heading, short Velocity)
  {
  	m_CyeCom.TurnTo(Heading, Velocity);
  }
  
  void RobotCom::SendStopMotors(void)
  {
     m_CyeCom.Stop();
  }
  
  void RobotCom::SendPurgeRingBuffer(void)
  {
  //	m_CyeCom.PurgeRingBuffer();
  }
  
  void RobotCom::SendMotorsPWM(int M0PWM, int M1PWM)
  {
     m_CyeCom.SetPWM(M0PWM, M1PWM);
  }
  
  void RobotCom::SendPositionDestination(robPOINT Destination)
  {
  //   m_CyeCom.SendPositionDestination(Destination.x, Destination.y);
  }
  
  
  BEGIN_EVENTSINK_MAP(RobotCom, CDialog)
      //{{AFX_EVENTSINK_MAP(RobotCom)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 157 /* RadioLevelChange */, OnRadioLevelChangeRrrcomctrl1, VTS_R4)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 158 /* BatteryLow */, OnBatteryLowRrrcomctrl1, VTS_NONE)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 160 /* ObstacleDetected */, OnObstacleDetectedRrrcomctrl1, VTS_I2 VTS_I2)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 161 /* NudgeDetected */, OnNudgeDetectedRrrcomctrl1, VTS_I4 VTS_I4)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 168 /* NewPathSegmentRequest */, OnNewPathSegmentRequestRrrcomctrl1, VTS_NONE)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 169 /* RobotError */, OnRobotErrorRrrcomctrl1, VTS_I4)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 170 /* OnHomeBase */, OnOnHomeBaseRrrcomctrl1, VTS_I2)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 172 /* BatteryChargingStateChange */, OnBatteryChargingStateChangeRrrcomctrl1, VTS_I2)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 174 /* CommStateChange */, OnCommStateChangeRrrcomctrl1, VTS_I2)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 179 /* TaskResult */, OnTaskResultRrrcomctrl1, VTS_I2 VTS_I4 VTS_I4 VTS_R8)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 180 /* SoftwareRevNumber */, OnSoftwareRevNumberRrrcomctrl1, VTS_I4)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 181 /* RobotOn */, OnRobotOnRrrcomctrl1, VTS_NONE)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 182 /* RobotOnTime */, OnRobotOnTimeRrrcomctrl1, VTS_R4)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 183 /* ExternalInput */, OnExternalInputRrrcomctrl1, VTS_R4)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 171 /* RobotStatusChange */, OnRobotStatusChangeRrrcomctrl1, VTS_I4 VTS_I4 VTS_R8 VTS_I2 VTS_I2 VTS_R4 VTS_R4 VTS_R4)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 173 /* UnknownMessage */, OnUnknownMessageRrrcomctrl1, VTS_VARIANT)
  	ON_EVENT(RobotCom, IDC_CYECOMCTRL1, 197 /* MicrophoneVolume */, OnMicrophoneVolumeCyecomctrl1, VTS_I2)
  	//}}AFX_EVENTSINK_MAP
  END_EVENTSINK_MAP()
  
  void RobotCom::OnObstacleDetectedRrrcomctrl1(short RightWheel, short LeftWheel) 
  {
  	m_Robot->OnObstacleDetected(RightWheel, LeftWheel);
  }
  
  void RobotCom::OnNudgeDetectedRrrcomctrl1(long RightWheel, long LeftWheel) 
  {
  	m_Robot->OnNudgeDetected(RightWheel != 0, LeftWheel != 0);
  }
  
  void RobotCom::OnNewPathSegmentRequestRrrcomctrl1() 
  {
  	m_Robot->OnNewPathSegmentRequest();
  }
  
  void RobotCom::OnRobotErrorRrrcomctrl1(long Error) 
  {
  	ASSERT(FALSE);
  	m_Robot->OnError(Error);
  }
  
  void RobotCom::SendVelocityDirection(int Velocity, double Heading)
  {
  	m_CyeCom.MoveDirection(Velocity, Heading);
  }
  
  void RobotCom::SendInformationToTransmit(long mask)
  {
  //	m_CyeCom.SendInformationToTransmit(mask);
  }
  
  void RobotCom::SendRequestState()
  {
  	m_CyeCom.RequestState();
  }
  
  void RobotCom::SendString(CString String)
  {
  	DWORD Elements = String.GetLength();
  
  	unsigned char* Data = new unsigned char[Elements];
  //	SafeArray.AccessData((void**)(&Data));
  	for (unsigned int i = 0; i < Elements; i++) {
  		Data[i] = String.GetAt(i);
  	}
  
  //	SafeArray.Create(VT_UI1, 1, &Elements);
  	COleSafeArray SafeArray;
  	SafeArray.CreateOneDim(VT_UI1, Elements, Data, 0);
  	delete Data;
  
  	m_CyeCom.SendMessage(SafeArray);
  }
  
  void RobotCom::OnOnHomeBaseRrrcomctrl1(short OnHomeBase) 
  {
  	m_Robot->OnOnHomeBase(OnHomeBase);
  }
  
  void RobotCom::OnUnknownMessageRrrcomctrl1(const VARIANT FAR& Buffer) 
  {
  	COleSafeArray SafeArray(Buffer);
  
  	CString String('\000', SafeArray.GetOneDimSize());
  	unsigned char* Data;
  	SafeArray.AccessData((void**)&Data);
  	for (int i = 0; i < String.GetLength(); i++) {
  		String.SetAt(i, Data[i]);
  	}
  	SafeArray.UnaccessData();
  
  	m_Robot->OnUnknownMessage(String);
  }
  
  char* LongToBytes(long int X)
  /*******************************************************************************
  //Created: 04/16/96 S.R.
  //Last Revision: 04/16/96 S.R.
  //Parameters:
  //	X		The long integer to break up into individual bytes.
  //Return Value:
  //	an array of characters containing the individual bytes of X.
  //	!!!!!!!!!!!Do not delte the resulting pointer!!!!!!!!!!!
  ********************************************************************************
  //Description:
  //	This function takes a long integer and breaks it up into its individual bytes.
  //	!!!!!Do Not Delete the Pointer Returned!!!!!!
  //Method:
  //		This function uses the bit shift operations to strip out the individual
  //	bytes of the long integer.  It then returns an array of characters containing
  //	the individual characters.  This array must be deleted or memory leaks will
  //	occur.
  *******************************************************************************/
  {
  	static char Result[4];
  	Result[0] = (char)X;
  	Result[1] = (char)(X>>8);
  	Result[2] = (char)(X>>16);
  	Result[3] = (char)(X>>24);
  	return Result;
  }
  
  int RadianToEncoderHeading(double Heading)
  /*******************************************************************************
  //Created: 01/30/97 S.R.
  //Last Revision: 01/30/97 S.R.
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
  	Heading = BringAngleInRange(Heading);
  /*Transform radian heading into a robot heading*/
  	Heading = Heading * cNHeadings / (2*pi);
  	if (Heading >= cNHeadings) Heading -= cNHeadings;
  	if (Heading >= cNHeadings) {
  		AfxMessageBox("RadianToEncoderHeading:  Heading Out Of Range",MB_OK,0);
  	}
  	return Round(Heading);
  }
  
  long WorldToRobotUnits(double WorldUnit)
  {
  	return Round(WorldUnit * cRobotEncoderTicksPerInch / cTPI);
  }
  
  /*void RobotCom::SendWallCalibrate(robPOINT P0, double H0, robPOINT P1, double H1, bool Corner, unsigned char FeelWheel)
  {
  	CString String(SEND_WALL_CALIBRATE_COMMAND_CHAR, 23);
  	char* Numbers;
  	Numbers = LongToBytes(WorldToRobotUnits(P0.x));
  	String.SetAt(1, Numbers[0]);
  	String.SetAt(2, Numbers[1]);
  	String.SetAt(3, Numbers[2]);
  	String.SetAt(4, Numbers[3]);
  	Numbers = LongToBytes(WorldToRobotUnits(P0.y));
  	String.SetAt(5, Numbers[0]);
  	String.SetAt(6, Numbers[1]);
  	String.SetAt(7, Numbers[2]);
  	String.SetAt(8, Numbers[3]);
  	Numbers = LongToBytes(RadianToEncoderHeading(H0));
  	String.SetAt(9, Numbers[0]);
  	String.SetAt(10, Numbers[1]);
  	Numbers = LongToBytes(WorldToRobotUnits(P1.x));
  	String.SetAt(11, Numbers[0]);
  	String.SetAt(12, Numbers[1]);
  	String.SetAt(13, Numbers[2]);
  	String.SetAt(14, Numbers[3]);
  	Numbers = LongToBytes(WorldToRobotUnits(P1.y));
  	String.SetAt(15, Numbers[0]);
  	String.SetAt(16, Numbers[1]);
  	String.SetAt(17, Numbers[2]);
  	String.SetAt(18, Numbers[3]);
  	Numbers = LongToBytes(RadianToEncoderHeading(H1));
  	String.SetAt(19, Numbers[0]);
  	String.SetAt(20, Numbers[1]);
  	String.SetAt(21, Corner);
  	String.SetAt(22, FeelWheel);
  	SendString(String);
  }
  */
  
  void RobotCom::SquareToWall(double Heading, float TravelTime, bool SlipperySurface)
  /*Heading = heading to wall*/
  /*TravelTime = time to travel forward looking for wall (in seconds)*/
  /*SlipperySurface = whether the floor is slippery (wood) or not*/
  {
  /*	CString String(SEND_SQUARE_TO_WALL_COMMAND_CHAR, 6);
  	char* Numbers = LongToBytes(RadianToEncoderHeading(Heading));
  	String.SetAt(1, Numbers[0]);
  	String.SetAt(2, Numbers[1]);
  	String.SetAt(3, SlipperySurface);
  
  	Numbers = LongToBytes(TravelTime*1000/50);
  	String.SetAt(4, Numbers[0]);
  	String.SetAt(5, Numbers[1]);
  //	String.SetAt(6, Numbers[2]);
  //	String.SetAt(7, Numbers[3]);
  
  	SendString(String);
  */
  	m_CyeCom.SquareToWall(Heading, TravelTime, SlipperySurface);
  }
  
  void RobotCom::SendFeelingMotor(unsigned char Motor)
  {
  /*	CString String(SEND_FEELING_MOTOR_COMMAND_CHAR, 2);
  	String.SetAt(1, Motor);
  	SendString(String);
  */
  	m_CyeCom.SetFeelingMotor(Motor);
  }
  
  void RobotCom::SendCornerCalibrate(robPOINT P0, double H0, unsigned char FeelWheel, float TravelTime, bool SlipperySurface)
  /*P0 = location of center of calibration wall*/
  /*H0 = heading of wall*/
  /*FeelWheel = wheel to feel corner with*/
  /*TravelTime = time to travel parallel to wall to find corner (in seconds)*/
  /*SlipperySurface = whether the floor is slippery (wood) or not*/
  {
  /*	P0.x -= cRRNoseLength * cos(H0);
  	P0.y -= cRRNoseLength * sin(H0);
  
  	P0.x = P0.x - (cCornerWallBackupDistance) * cos(H0);
  	P0.y = P0.y - (cCornerWallBackupDistance) * sin(H0);
  
  	if (FeelWheel == 0) H0 += pi/2;
  	else H0 -= pi/2;
  
  	CString String(SEND_CORNER_CALIBRATE_COMMAND_CHAR, 15);
  	char* Numbers;
  	Numbers = LongToBytes(WorldToRobotUnits(P0.x));
  	String.SetAt(1, Numbers[0]);
  	String.SetAt(2, Numbers[1]);
  	String.SetAt(3, Numbers[2]);
  	String.SetAt(4, Numbers[3]);
  	Numbers = LongToBytes(WorldToRobotUnits(P0.y));
  	String.SetAt(5, Numbers[0]);
  	String.SetAt(6, Numbers[1]);
  	String.SetAt(7, Numbers[2]);
  	String.SetAt(8, Numbers[3]);
  	Numbers = LongToBytes(RadianToEncoderHeading(H0));
  	String.SetAt(9, Numbers[0]);
  	String.SetAt(10, Numbers[1]);
  	String.SetAt(11, FeelWheel);
  	String.SetAt(12, SlipperySurface);
  	Numbers = LongToBytes(TravelTime*1000/50);
  	String.SetAt(13, Numbers[0]);
  	String.SetAt(14, Numbers[1]);
  
  	SendString(String);
  */
  	m_CyeCom.CornerCalibrate((long)P0.x, (long)P0.y, H0, FeelWheel, TravelTime, SlipperySurface);
  }
  
  void RobotCom::SendFindHomeBase(double CornerHeading, double WallHeading, robPOINT HomeBaseLocation, bool NeedToGetInFrontOfHomeBase)
  {
  	m_CyeCom.FindHomeBase(CornerHeading, WallHeading, (long)HomeBaseLocation.x, (long)HomeBaseLocation.y, NeedToGetInFrontOfHomeBase);
  /*	robPOINT P1 = HomeBaseLocation;
  	if (NeedToGetInFrontOfHomeBase) {
  		P1.x -= (cRRWidth + 6*cTPI) * cos(WallHeading);
  		P1.x -= 0.5*cTPI * cos(CornerHeading);
  		P1.y -= (cRRWidth + 6*cTPI) * sin(WallHeading);
  		P1.y -= 0.5*cTPI * sin(CornerHeading);
  	} else {
  		P1.x -= (6*cTPI) * cos(WallHeading);
  		P1.y -= (6*cTPI) * sin(WallHeading);
  	}
  	robPOINT P2;
  //	P2.x = -3.5*cTPI*cos(WallHeading);
  //	P2.y = -3.5*cTPI*sin(WallHeading);
  	P2.x = 0;
  	P2.y = 0;
  
  	robPOINT P3;
  //	P3.x = 1*cTPI*cos(WallHeading + pi/2);
  //	P3.y = 1*cTPI*sin(WallHeading + pi/2);
  	P3.x = 0;
  	P3.y = 0;
  
  
  	CString String(SEND_FIND_HOME_BASE_COMMAND_CHAR, 12);
  	char* Numbers;
  	Numbers = LongToBytes(WorldToRobotUnits(P1.x));
  	String.SetAt(1, Numbers[0]);
  	String.SetAt(2, Numbers[1]);
  	String.SetAt(3, Numbers[2]);
  	String.SetAt(4, Numbers[3]);
  	Numbers = LongToBytes(WorldToRobotUnits(P1.y));
  	String.SetAt(5, Numbers[0]);
  	String.SetAt(6, Numbers[1]);
  	String.SetAt(7, Numbers[2]);
  	String.SetAt(8, Numbers[3]);
  
  	Numbers = LongToBytes(RadianToEncoderHeading(WallHeading));
  	String.SetAt(9, Numbers[0]);
  	String.SetAt(10, Numbers[1]);
  
  	String.SetAt(11, NeedToGetInFrontOfHomeBase);
  
  //	String.SetAt(11, NeedToGetInFrontOfHomeBase);
  	SendString(String);
  */
  }
  
  void RobotCom::PowerDown()
  {
     m_CyeCom.PowerDown();
  }
  
  void RobotCom::SetRetryLimit(long retry)
  {
  	m_CyeCom.SetRetryLimit(retry);
  }
  
  long RobotCom::GetRetryLimit()
  {
  	return m_CyeCom.GetRetryLimit();
  }
  
  void RobotCom::SendPause(bool p)
  {
  /*	CString String(SEND_PAUSE_COMMAND_CHAR, 2);
  	String.SetAt(1, p);
  	SendString(String);
  */
  	if (p) m_CyeCom.Pause();
  	else m_CyeCom.Resume();
  }
  
  float RobotCom::GetRadioLevel()
  {
  	return m_CyeCom.GetRadioLevel();
  }
  
  void RobotCom::SendHeadingDestination(double Heading, short Speed, short MotorEnable)
  {
  /*	CString String(SET_HEADING_DESTINATION_EXTENDED, 6);
  
  	char* Numbers;
  
  	Numbers = LongToBytes(RadianToEncoderHeading(Heading));
  	String.SetAt(1, Numbers[0]);
  	String.SetAt(2, Numbers[1]);
  
  	if (Speed < 0) Speed = -Speed;
  	Numbers = LongToBytes(Speed);
  	String.SetAt(3, Numbers[0]);
  	String.SetAt(4, Numbers[1]);
  
  	String.SetAt(5, MotorEnable);
  	SendString(String);
  */
  	m_CyeCom.SetHeadingDestination(Heading, Speed, MotorEnable);
  }
  
  void RobotCom::SendEnableObstacleBounce(bool Enable)
  {
  /*	CString String(SET_ENABLE_OBSTACLE_BOUNCE_COMMAND_CHAR,2);
  	String.SetAt(1, Enable);
  	SendString(String);
  */
  	m_CyeCom.SetEnableObstacleBounce(Enable);
  }
  
  void RobotCom::SendRAMData(unsigned short Address, unsigned char NumberBytes, unsigned char *Data)
  {
  	ASSERT (NumberBytes < 15);
  	CString String(SET_RAM_DATA_COMMAND_CHAR, NumberBytes + 4);
  	String.SetAt(1, NumberBytes);
  	
  	char* Numbers;
  	Numbers = LongToBytes(Address);
  	String.SetAt(2, Numbers[0]);
  	String.SetAt(3, Numbers[1]);
  
  	for (unsigned char i = 0; i < NumberBytes; i++) {
  		String.SetAt(i+4, Data[i]);
  	}
  	SendString(String);
  }
  
  
  void RobotCom::SendRobotOnSeconds(long Seconds)
  {
  /*	CString String(SET_ROBOT_ON_SECONDS_COMMAND_CHAR, 5);
  	char* Numbers;
  	Numbers = LongToBytes(Seconds / 0.953f);
  	String.SetAt(1, Numbers[0]);
  	String.SetAt(2, Numbers[1]);
  	String.SetAt(3, Numbers[2]);
  	String.SetAt(4, Numbers[3]);
  	SendString(String);
  */
  	m_CyeCom.SetRobotOnSeconds(Seconds);
  }
  
  void RobotCom::SendIntermittentBeep(bool On)
  {
  /*	CString String(SET_INTERMITTENT_BEEP_ON_COMMAND_CHAR, 2);
  	String.SetAt(1, On);
  	SendString(String);
  */
  	m_CyeCom.SetIntermittentBeep(On);
  }
  
  void RobotCom::AddToPosition(robPOINT Delta)
  {
  /*	long X = WorldToRobotUnits(Delta.x);
  	long Y = WorldToRobotUnits(Delta.y);
  	CString String(SET_POSITION_DELTA_COMMAND_CHAR, 9);
  	char* Numbers = LongToBytes(X);
  	String.SetAt(1, Numbers[0]);
  	String.SetAt(2, Numbers[1]);
  	String.SetAt(3, Numbers[2]);
  	String.SetAt(4, Numbers[3]);
  	Numbers = LongToBytes(Y);
  	String.SetAt(5, Numbers[0]);
  	String.SetAt(6, Numbers[1]);
  	String.SetAt(7, Numbers[2]);
  	String.SetAt(8, Numbers[3]);
  	SendString(String);
  */
  	m_CyeCom.AddToPosition(Delta.x, Delta.y);
  }
  
  void RobotCom::AddToHeading(double Delta)
  {
  /*	int Heading = RadianToEncoderHeading(Delta);
  	char* Numbers = LongToBytes(Heading);
  
  	CString String(SET_HEADING_DELTA_COMMAND_CHAR, 3);
  //	Numbers = LongToBytes(Delta);
  	String.SetAt(1, Numbers[0]);
  	String.SetAt(2, Numbers[1]);
  	SendString(String);
  */
  	m_CyeCom.AddToHeading(Delta);
  }
  
  void RobotCom::InitialSearchForRobot()
  {
  //	return;
  //	m_CyeCom.SetCommPort("COM1");
  //	return;
  	CString OriginalCommPort = m_CyeCom.GetCommPort();
  
  	CDialog Searching(IDD_COM_PORT_SEARCH_STATUS, NULL);
  	Searching.Create(IDD_COM_PORT_SEARCH_STATUS, NULL);
  	Searching.ShowWindow(SW_SHOW);
  	Searching.GetDlgItem(IDC_EDT_COM_PORT)->SetWindowText(OriginalCommPort);
  	WaitForCommunicationsEstablished(2*cWaitForCommunicationsEstablished);
  	if (!m_CommunicationsEstablished) {
  		CDialog Warning(IDD_COM_PORT_SEARCH, NULL);
  		if (Warning.DoModal() == IDOK) {
  			bool FoundRobot = SearchForRobot(Searching);
  			bool KeepSearching = true;
  			while ( (!FoundRobot) && (KeepSearching) ) {
  				CDialog Warning2(IDD_COM_PORT_SEARCH_FAILED, NULL);
  				if (Warning2.DoModal() == IDOK) {
  					FoundRobot = SearchForRobot(Searching);
  				} else {
  					KeepSearching = false;
  				}
  			}
  			if (!FoundRobot) m_CyeCom.SetCommPort(OriginalCommPort);
  		}
  	}
  }
  
  void RobotCom::WaitForCommunicationsEstablished(int seconds)
  {
  	if (seconds == 0) return;
  	MSG message;
  	CTime StartTime = CTime::GetCurrentTime();
  	CTimeSpan WaitTime(0,0,0,seconds);
  	while ( (!m_CommunicationsEstablished) && ((CTime::GetCurrentTime() - StartTime) < WaitTime) ) {
  	//	while (PeekMessage(&message,NULL,0,0,PM_REMOVE)) {
  		do {
  			GetMessage(&message, NULL, 0, 0);
  			TranslateMessage(&message);
  			DispatchMessage(&message);
  		} while (::PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE));
  
  		long lIdle = 0;
  		while ( AfxGetApp()->OnIdle(lIdle++ ) );
  	}
  	return;
  }
  
  
  void RobotCom::SetExternalOutput(bool On)
  {
  /*	CString String(SET_EX1_COMMAND_CHAR, 2);
  	String.SetAt(1, On);
  	SendString(String);
  */
  	m_CyeCom.SetExternalOutput(On);
  }
  
  void RobotCom::OnCommStateChangeRrrcomctrl1(short NewState) 
  {
  	if (NewState >= 1) {	//NewState == 1 => ASTRT state
  		m_CommunicationsEstablished = true;
  		if (!m_FirstCommunicationsEstablished) {
  			m_FirstCommunicationsEstablished = true;
  			m_Robot->OnCommunicationsEstablished();
  		}
  	}
  }
  
  bool RobotCom::SearchForRobot(CDialog& Status)
  {
  	m_CommunicationsEstablished = false;
  //	CDialog Status(IDD_COM_PORT_SEARCH_STATUS, NULL);
  //	Status.Create(IDD_COM_PORT_SEARCH_STATUS, NULL);
  //	Status.ShowWindow(SW_SHOW);
  	Status.GetDlgItem(IDC_EDT_COM_PORT)->SetWindowText("COM1");
  	m_CyeCom.SetCommPort("COM1");
  	WaitForCommunicationsEstablished(cWaitForCommunicationsEstablished);
  	if (!m_CommunicationsEstablished) {
  		m_CyeCom.SetCommPort("COM2");
  		Status.GetDlgItem(IDC_EDT_COM_PORT)->SetWindowText("COM2");
  	} else return true;
  	WaitForCommunicationsEstablished(cWaitForCommunicationsEstablished);
  	if (!m_CommunicationsEstablished) {
  		m_CyeCom.SetCommPort("COM3");
  		Status.GetDlgItem(IDC_EDT_COM_PORT)->SetWindowText("COM3");
  	} else return true;
  	WaitForCommunicationsEstablished(cWaitForCommunicationsEstablished);
  	if (!m_CommunicationsEstablished) {
  		m_CyeCom.SetCommPort("COM4");
  		Status.GetDlgItem(IDC_EDT_COM_PORT)->SetWindowText("COM4");
  	} else return true;
  	if (!m_CommunicationsEstablished) return false;
  	else return true;
  }
  
  void RobotCom::SendRequestEX2()
  {
  //	CString String(GET_EX2_COMMAND_CHAR, 1);
  //	SendString(String);
  	m_CyeCom.RequestExternalInput();
  }
  
  void RobotCom::OnRadioLevelChangeRrrcomctrl1(float RadioLevel) 
  {
  	m_Robot->OnRadioLevelChange(RadioLevel);
  }
  
  void RobotCom::OnBatteryLowRrrcomctrl1() 
  {
  	m_Robot->OnBatteryLow();
  }
  
  void RobotCom::OnBatteryChargingStateChangeRrrcomctrl1(short State) 
  {
  	m_Robot->OnBatteryChargingStateChange(State);
  }
  
  void RobotCom::OnTaskResultRrrcomctrl1(short Result, long X, long Y, double Heading) 
  {
  	robPOINT Position;
  	Position.x = X;
  	Position.y = Y;
  	m_Robot->OnTaskResult(TInstructionResult(Result), Position, Heading);
  }
  
  void RobotCom::OnSoftwareRevNumberRrrcomctrl1(long RevNumber) 
  {
  	m_Robot->OnSoftwareRevNumber(1.0f + RevNumber / 100.0f);
  }
  
  void RobotCom::OnRobotOnRrrcomctrl1() 
  {
  	m_Robot->OnRobotOn();
  }
  
  void RobotCom::OnRobotOnTimeRrrcomctrl1(float SecondsOn) 
  {
  	m_Robot->OnRobotOnTime((long)SecondsOn);
  }
  
  void RobotCom::OnExternalInputRrrcomctrl1(float Voltage) 
  {
  	m_Robot->OnExternalInput(Voltage);
  }
  
  void RobotCom::OnRobotStatusChangeRrrcomctrl1(long X, long Y, double Heading, short RightVelocity, short LeftVelocity, float RightCurrent, float LeftCurrent, float BatteryVoltage) 
  {
  	m_Robot->OnPositionChange(X, Y, Heading);
  	m_Robot->OnVelocityChange(RightVelocity, LeftVelocity);
  	m_Robot->OnAnalogUpdate(RightCurrent, LeftCurrent, BatteryVoltage);	
  }
  
  void RobotCom::SendTune(short *Frequency, int *msDuration, unsigned char NumberOfNotes, bool PurgeBuffer, bool StartPlaying)
  /*NumberOfNotes has to be less than 5*/
  {
  	ASSERT(NumberOfNotes <= 5);
  	if (NumberOfNotes > 5) NumberOfNotes = 5;
  	CString OutputString('c', 4*NumberOfNotes + 4);
  	OutputString.SetAt(0, 'G');
  	OutputString.SetAt(1, PurgeBuffer);
  	OutputString.SetAt(2, StartPlaying);
  	OutputString.SetAt(3, NumberOfNotes);
  	for (int i = 0; i < NumberOfNotes; i++) {
  		char* Bytes = LongToBytes(Frequency[i]);
  		OutputString.SetAt(4*i + 4, Bytes[0]);
  		OutputString.SetAt(4*i + 5, Bytes[1]);
  		if ( (msDuration[i] < 50) && (msDuration[i] > 0) ) {
  			Bytes = LongToBytes(1);
  		} else {
  			Bytes = LongToBytes(Round(msDuration[i] / 50));
  		}
  		OutputString.SetAt(4*i + 6, Bytes[0]);
  		OutputString.SetAt(4*i + 7, Bytes[1]);
  	}
  	SendString(OutputString);
  }
  
  void RobotCom::SendPing()
  {
  	CString OutputString('m', 1);
  	SendString(OutputString);
  }
  
  void RobotCom::SendCornerCalibrate(robPOINT P0, double H0, unsigned char FeelWheel, float TravelTime, bool SlipperySurface, short WheelDistanceFromWall)
  {
  	const char SEND_CORNER_CALIBRATE_COMMAND_CHAR = 'Z';
  
  	double P0x, P0y;
  	P0x = P0.x - (WheelDistanceFromWall + cRRWidth/2 + cFollowCTDPathHandleLength) * cos(H0);
  	P0y = P0.y - (WheelDistanceFromWall + cRRWidth/2 + cFollowCTDPathHandleLength) * sin(H0);
  
  	if (FeelWheel == 0) H0 += pi/2;
  	else H0 -= pi/2;
  
  	CString String('i', 15);
     String.SetAt(0, SEND_CORNER_CALIBRATE_COMMAND_CHAR);
  	char* Numbers;
  	Numbers = LongToBytes(WorldToRobotUnits(P0x));
  	String.SetAt(1, Numbers[0]);
  	String.SetAt(2, Numbers[1]);
  	String.SetAt(3, Numbers[2]);
  	String.SetAt(4, Numbers[3]);
  	Numbers = LongToBytes(WorldToRobotUnits(P0y));
  	String.SetAt(5, Numbers[0]);
  	String.SetAt(6, Numbers[1]);
  	String.SetAt(7, Numbers[2]);
  	String.SetAt(8, Numbers[3]);
  	Numbers = LongToBytes(RadianToEncoderHeading(H0));
  	String.SetAt(9, Numbers[0]);
  	String.SetAt(10, Numbers[1]);
  	String.SetAt(11, FeelWheel);
  	String.SetAt(12, SlipperySurface);
  	Numbers = LongToBytes(long(TravelTime*1000/50));
  	String.SetAt(13, Numbers[0]);
  	String.SetAt(14, Numbers[1]);
  	SendString(String);
  }
  
  void RobotCom::SetRobotAddress(short Address)
  {
  	m_CyeCom.SetRobotAddress(Address);
  }
  
  void RobotCom::SetEnableDancing(bool Enable)
  {
  	const char COMMAND_CHAR_ENABLE_DANCING = 'w';
  	CString String (COMMAND_CHAR_ENABLE_DANCING, 2);
  	String.SetAt(1, Enable);
  	SendString(String);
  }
  
  void RobotCom::SetEnableClapDetection(bool Enable)
  {
  	const char COMMAND_CHAR_ENABLE_CLAPPING = 'E';
  	CString String (COMMAND_CHAR_ENABLE_CLAPPING, 2);
  	String.SetAt(1, Enable);
  	SendString(String);
  }
  
  void RobotCom::OnMicrophoneVolumeCyecomctrl1(short Volume) 
  {
  	m_Robot->OnMicrophoneVolume(Volume);
  }
  
  void RobotCom::SetEnableClapDetectionWhileMoving(bool Enable)
  {
  	const char COMMAND_CHAR_ENABLE_CLAP_DETECT_WHILE_MOVING = 'g';
  	CString String (COMMAND_CHAR_ENABLE_CLAP_DETECT_WHILE_MOVING, 2);
  	String.SetAt(1, Enable);
  	SendString(String);
  }
  
  void RobotCom::SetResponseTimeout(long TimeOut)
  {
  	m_CyeCom.SetResponseTimeout(TimeOut);
  }
