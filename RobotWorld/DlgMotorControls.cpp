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

// DlgMotorControls.cpp : implementation file
//

#include "stdafx.h"
#include "RobotWorld.h"
#include "RobotWorldView.h"
#include "Probability Grid.h"
#include "CTDPath.h"
#include <math.h>

#include "DlgMotorControls.h"
#include "Path.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMotorControls dialog

#define GetRobotWorldView() (((CRobotWorldView*)m_ParentView))
CDlgMotorControls::CDlgMotorControls(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgMotorControls::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgMotorControls)
    m_edtTotalEncoderTicks = _T("");
    m_edtM0VelMeas = 0;
    m_edtM1Acceleration = 0;
    m_edtM1VelMeas = 0;
    m_edtObstacle = _T("");
    m_chkRecordInformation = FALSE;
    m_edtM1PwmMeas = 0;
    m_edtH = 0;
    m_edtX = 0;
    m_edtY = 0;
    m_edtM1Encoder = 0;
    m_edtM0Encoder = 0;
    m_edtCurrentHeading = 0.0;
    m_edtM0Acceleration = 0;
    m_edtM0PwmMeas = 0;
    m_edtKHeadErr = 0.0;
    m_edtM0Pwm = _T("");
    m_edtM0Speed = _T("");
    m_edtMaxSpeed = _T("");
    m_edtM1Pwm = _T("");
    m_edtM1Speed = _T("");
    m_edtHeading = _T("");
    m_edtStraightHeading = _T("");
    m_edtKb = 8;
    m_edtKd = 5;
    m_edtKi = 6;
    m_edtKp = 3;
    m_edtKPwm = 0;
    m_edtHandleLength = (short)cRRWheelBase;
    m_BuzzerFrequency = 0;
    m_edtP1 = 1500;
    m_edtP2 = 100;
    m_edtP3 = 90;
    m_edtP4 = 500;
    m_edtCorrection = _T("");
    m_edtRepeat = _T("3");
    m_edtStartX = _T("-100");
    m_edtStartY = _T("0");
    m_edtEndX = _T("-200");
    m_edtEndY = _T("200");
    m_edtIterations = _T("100");
    m_edtEndDist = _T("");
    m_edtFreeDist = _T("");
    m_edtMinFound = _T("");
    m_edtStartDist = _T("");
    m_chkStopPath = FALSE;
    m_edtAvgFound = _T("");
    m_edtFindCalZoneX = _T("");
    m_edtFindCalZoneY = _T("");
    m_edtBatteryLevel = 0.0;
    m_edtVelocityDirectionHeading = _T("");
    m_edtVelocityDirectionVelocity = _T("");
    m_chkFlipHandle = FALSE;
    m_chkSendAD = FALSE;
    m_chkSendPosition = FALSE;
    m_chkSendVelocity = FALSE;
    m_chkSendAccel = FALSE;
    m_edtP8 = 0;
    m_edtP5 = 800;
    m_edtP6 = 51;
    m_edtP7 = 250;
    m_edtM0AD = 0.0f;
    m_edtM1AD = 0.0f;
    m_edtPathBuffer = 0;
    m_edtM0Obstacle = _T("");
    m_edtM1Obstacle = _T("");
    m_edtAnalogInput = 0.0f;
    m_chkRobotAnimation = TRUE;
    m_edtOutput = _T("");
    m_edtMicrophoneVolume = 0;
    //}}AFX_DATA_INIT
    m_M0Encoder = 0;
    m_M1Encoder = 0;
    m_PathPosition = 0;

    m_OldHScrollPos = 0;
    m_OldVScrollPos = 0;
    /*Create path*/
    //	double radius = 300;
    double radius = 636.6;
    double theta = -pi / 2;
#define cPS 200
    m_Path = new robPOINT[cPS];

    for (unsigned char i = 0; i < cPS; i++)
    {
        m_Path[i].x = radius * cos(theta);
        m_Path[i].y = radius * sin(theta) + radius;
        //		m_Path[i].x = 10*i;
        //		m_Path[i].y = 0;
        theta += pi / 100;
    }
}


void CDlgMotorControls::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMotorControls)
    DDX_Text(pDX, IDC_EDTTOTALENCODERTICKS, m_edtTotalEncoderTicks);
    DDX_Text(pDX, IDC_EDTM0VELMEAS, m_edtM0VelMeas);
    DDX_Text(pDX, IDC_EDTM1ACCELERATION, m_edtM1Acceleration);
    DDX_Text(pDX, IDC_EDTM1VELMEAS, m_edtM1VelMeas);
    DDX_Text(pDX, IDC_EDTOBSTACLE, m_edtObstacle);
    DDX_Check(pDX, IDC_CHKRECORDINFO, m_chkRecordInformation);
    DDX_Text(pDX, IDC_EDTM1PWMMEAS, m_edtM1PwmMeas);
    DDX_Text(pDX, IDC_EDTH, m_edtH);
    DDX_Text(pDX, IDC_EDTX, m_edtX);
    DDX_Text(pDX, IDC_EDTY, m_edtY);
    DDX_Text(pDX, IDC_EDTM1ENCODER, m_edtM1Encoder);
    DDX_Text(pDX, IDC_EDTM0ENCODER, m_edtM0Encoder);
    DDX_Text(pDX, IDC_EDTCURRENTHEADING, m_edtCurrentHeading);
    DDX_Text(pDX, IDC_EDTM0ACCELERATION, m_edtM0Acceleration);
    DDX_Text(pDX, IDC_EDTM0PWMMEAS, m_edtM0PwmMeas);
    DDX_Text(pDX, IDC_EDTKHEADERR, m_edtKHeadErr);
    DDX_Text(pDX, IDC_EDTM0PWM, m_edtM0Pwm);
    DDX_Text(pDX, IDC_EDTM0SPEED, m_edtM0Speed);
    DDX_Text(pDX, IDC_EDTMAXSPEED, m_edtMaxSpeed);
    DDX_Text(pDX, IDC_EDTM1PWM, m_edtM1Pwm);
    DDX_Text(pDX, IDC_EDTM1SPEED, m_edtM1Speed);
    DDX_Text(pDX, IDC_EDTHEADING, m_edtHeading);
    DDX_Text(pDX, IDC_EDTSTRAIGHTHEADING, m_edtStraightHeading);
    DDX_Text(pDX, IDC_EDTKB, m_edtKb);
    DDX_Text(pDX, IDC_EDTKD, m_edtKd);
    DDX_Text(pDX, IDC_EDTKI, m_edtKi);
    DDX_Text(pDX, IDC_EDTKP, m_edtKp);
    DDX_Text(pDX, IDC_EDTKPWM, m_edtKPwm);
    DDX_Text(pDX, IDC_EDTHANDLELENGTH, m_edtHandleLength);
    DDX_Text(pDX, IDC_EDTBUZZERFREQUENCY, m_BuzzerFrequency);
    DDX_Text(pDX, IDC_EDT_PARM1, m_edtP1);
    DDX_Text(pDX, IDC_EDT_PARM2, m_edtP2);
    DDX_Text(pDX, IDC_EDT_PARM3, m_edtP3);
    DDX_Text(pDX, IDC_EDT_PARM4, m_edtP4);
    DDX_Text(pDX, IDC_EDT_CORRECTION, m_edtCorrection);
    DDX_Text(pDX, IDC_EDT_REPEAT, m_edtRepeat);
    DDX_Text(pDX, IDC_EDT_START_X, m_edtStartX);
    DDX_Text(pDX, IDC_EDT_START_Y, m_edtStartY);
    DDX_Text(pDX, IDC_EDT_END_X, m_edtEndX);
    DDX_Text(pDX, IDC_EDT_END_Y, m_edtEndY);
    DDX_Text(pDX, IDC_EDT_ITERATIONS, m_edtIterations);
    DDX_Text(pDX, IDC_EDT_END_DIST, m_edtEndDist);
    DDX_Text(pDX, IDC_EDT_FREE_DIST, m_edtFreeDist);
    DDX_Text(pDX, IDC_EDT_MIN_FOUND, m_edtMinFound);
    DDX_Text(pDX, IDC_EDT_START_DIST, m_edtStartDist);
    DDX_Check(pDX, IDC_CHK_STOP_PATH, m_chkStopPath);
    DDX_Text(pDX, IDC_EDT_AVG_FOUND, m_edtAvgFound);
    DDX_Text(pDX, IDC_EDT_FIND_CAL_ZONE_X, m_edtFindCalZoneX);
    DDX_Text(pDX, IDC_EDT_FIND_CAL_ZONE_Y, m_edtFindCalZoneY);
    DDX_Text(pDX, IDC_EDT_BATTERY_AD, m_edtBatteryLevel);
    DDX_Text(pDX, IDC_EDT_VELOCITY_DIRECTION_HEADING, m_edtVelocityDirectionHeading);
    DDX_Text(pDX, IDC_EDT_VELOCITY_DIRECTION_VELOCITY, m_edtVelocityDirectionVelocity);
    DDX_Check(pDX, IDC_CHKFLIPHANDLE, m_chkFlipHandle);
    DDX_Check(pDX, IDC_CHK_SEND_AD, m_chkSendAD);
    DDX_Check(pDX, IDC_CHK_SEND_POSITION, m_chkSendPosition);
    DDX_Check(pDX, IDC_CHK_SEND_VELOCITIES, m_chkSendVelocity);
    DDX_Check(pDX, IDC_CHK_SEND_ACCEL, m_chkSendAccel);
    DDX_Text(pDX, IDC_EDT_PARM8, m_edtP8);
    DDX_Text(pDX, IDC_EDT_PARM5, m_edtP5);
    DDX_Text(pDX, IDC_EDT_PARM6, m_edtP6);
    DDX_Text(pDX, IDC_EDT_PARM7, m_edtP7);
    DDX_Text(pDX, IDC_EDTM0AD, m_edtM0AD);
    DDX_Text(pDX, IDC_EDTM1AD, m_edtM1AD);
    DDX_Text(pDX, IDC_EDT_PATH_BUFFER, m_edtPathBuffer);
    DDX_Text(pDX, IDC_EDT_M0OBST, m_edtM0Obstacle);
    DDX_Text(pDX, IDC_EDT_M1OBST, m_edtM1Obstacle);
    DDX_Text(pDX, IDC_ANALOG_INPUT, m_edtAnalogInput);
    DDX_Check(pDX, IDC_CHK_ROBOT_ANIMATION, m_chkRobotAnimation);
    DDX_Text(pDX, IDC_EDT_OUTPUT, m_edtOutput);
    DDX_Text(pDX, IDC_EDT_MICROPHONE_VOLUME, m_edtMicrophoneVolume);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMotorControls, CDialog)
    //{{AFX_MSG_MAP(CDlgMotorControls)
    ON_BN_CLICKED(IDC_BUTSETPWM, OnButSetPWM)
    ON_BN_CLICKED(IDC_BUTSETHEADING, OnButSetHeading)
    ON_BN_CLICKED(IDC_BUTSTOPMOTORS, OnButStopMotors)
    ON_BN_CLICKED(IDC_CHKFLIPHANDLE, OnChkFlipHandle)
    ON_BN_CLICKED(IDC_BUTSETSPEED, OnButSetSpeed)
    ON_BN_CLICKED(IDC_BUTSHOWHEADING, OnButShowHeading)
    ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
    ON_BN_CLICKED(IDC_BUTCALIBRATEHEADING, OnButCalibrateHeading)
    ON_BN_CLICKED(IDC_BUTDO360, OnButDo360)
    ON_BN_CLICKED(IDC_BUTRESETPOSITION, OnButResetPosition)
    ON_BN_CLICKED(IDC_BUTCREATEDOORWAY, OnButCreateDoorway)
    ON_BN_CLICKED(IDC_BUTCREATEWALL, OnButcreateWall)
    ON_BN_CLICKED(IDC_BUTCALIBRATE, OnButCalibrate)
    ON_BN_CLICKED(IDC_BUTSENDDEFAULTCORRECTION, OnButSendDefaultCorrection)
    ON_BN_CLICKED(IDC_BUTSETMAXSPEED, OnButSetMaxSpeed)
    ON_BN_CLICKED(IDC_BUTSETSTRAIGHTHEADING, OnButSetStraightHeading)
    ON_BN_CLICKED(IDC_BUTPURGERINGBUFFER, OnButPurgeRingBuffer)
    ON_BN_CLICKED(IDC_RADOBSHIGHSENS, OnObstacleHighSensitivity)
    ON_BN_CLICKED(IDC_RADOBSLOWSENS, OnObstacleLowSensitivity)
    ON_BN_CLICKED(IDC_RADOBSMEDSENS, OnObstacleMediumSensitivity)
    ON_BN_CLICKED(IDC_RADOBSNONE, OnObstacleNone)
    ON_BN_CLICKED(IDC_BUTSHOWPOSITION, OnButShowPosition)
    ON_BN_CLICKED(IDC_BUTNOCORRECTION, OnButNoCorrection)
    ON_BN_CLICKED(IDC_BUTSENDMOTORCONTROLCONSTANTS, OnButSendMotorControlConstants)
    ON_BN_CLICKED(IDC_BUTSENDMOTOCONDEFAULTS, OnButSendMotoConDefaults)
    ON_BN_CLICKED(IDC_BUTFOLLOWPATH, OnButFollowPath)
    ON_BN_CLICKED(IDC_CHKRECORDINFO, OnChkRecordInfo)
    ON_BN_CLICKED(IDC_BUTSENDBUZZER, OnButSendBuzzer)
    ON_BN_CLICKED(IDC_BUT_SET, OnButSet)
    ON_BN_CLICKED(IDC_BUT_PARM_DEFAULT, OnButParmDefault)
    ON_BN_CLICKED(IDC_BUT_SEND_CORRECTION, OnButSendCorrection)
    ON_BN_CLICKED(IDC_BUT_WRITE_PROB_GRID, OnButWriteProbGrid)
    ON_BN_CLICKED(IDC_BUT_FIND_PATH, OnButFindPath)
    ON_BN_CLICKED(IDC_BUT_FIND_CAL_ZONE, OnButFindCalZone)
    ON_BN_CLICKED(IDC_BUT_SET_VELOCITY_DIRECTION, OnButSetVelocityDirection)
    ON_BN_CLICKED(IDC_BUT_CREATE_CAL_WALL, OnButCreateCalWall)
    ON_BN_CLICKED(IDC_BUT_SEND_VALUES, OnButSendValues)
    ON_BN_CLICKED(IDC_BUT_TEST_MOTORS, OnButTestMotors)
    ON_BN_CLICKED(IDC_BUT_ON, OnButOn)
    ON_BN_CLICKED(IDC_BUT_SET_PATH_BUFFER, OnButSetPathBuffer)
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_BN_CLICKED(IDC_BUT_REQUEST_ANALOG_INPUT, OnButRequestAnalogInput)
    ON_BN_CLICKED(IDC_BUTSTOPMOTORS2, OnButStopMotors)
    ON_BN_CLICKED(IDC_BUTRESETPOSITION2, OnButResetPosition)
    ON_BN_CLICKED(IDC_BUTRESETPOSITION3, OnButResetPosition)
    ON_BN_CLICKED(IDC_CHK_ROBOT_ANIMATION, OnChkRobotAnimation)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMotorControls message handlers

void CDlgMotorControls::OnButSetPWM()
/*******************************************************************************
//Created: 04/30/96 S.R.
//Last Revision: 02/13/97 S.R.
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
    UpdateData(TRUE);
    int M0Pwm, M1Pwm;
    M0Pwm = atoi(m_edtM0Pwm);
    M1Pwm = atoi(m_edtM1Pwm);
    GetRobotWorldView()->GetActiveRobot()->SetPwm(M0Pwm, M1Pwm);
}

void CDlgMotorControls::OnButSetHeading()
{
    UpdateData(TRUE);
    GetRobotWorldView()->GetActiveRobot()->AddHeadingToInstructions(atof(m_edtHeading), NULL, 0, GetRobotWorldView()->GetActiveRobot()->GetMaxSpeed(), 0);
    TInstructionResult InstructionResult = GetRobotWorldView()->GetActiveRobot()->FollowInstructions(TRUE, FALSE);
    //	GetRobotWorldView()->GetActiveRobot()->SendRobotHeading( atoi(LPCTSTR(tempString)) );
}

void CDlgMotorControls::OnButStopMotors()
{
    GetRobotWorldView()->GetActiveRobot()->EStop();
}

void CDlgMotorControls::OnChkFlipHandle()
{
    UpdateData(TRUE);
    GetRobotWorldView()->GetActiveRobot()->SetFlipHandle(m_chkFlipHandle == TRUE);
}


void CDlgMotorControls::OnButSetSpeed()
{
    UpdateData(TRUE);
    GetRobotWorldView()->GetActiveRobot()->SetVelocities(atoi(m_edtM0Speed), atoi(m_edtM1Speed));
}

void CDlgMotorControls::OnButShowHeading()
{
    UpdateData(TRUE);
    m_edtCurrentHeading = m_CurrentHeading;
    UpdateData(FALSE);
}

#include <fcntl.h>
#include <share.h>
#include <sys\stat.h>
#include "VacuumPatch.h"
#include <fstream>
#include "Homebase.h"

void CDlgMotorControls::OnButton1()
{
    CRobot* Robot = GetRobotWorldView()->GetActiveRobot();
    CHomeBase* ClosestHome = GetRobotWorldView()->m_ProbabilityGrid->GetClosestUnoccupiedHomeBase(Robot->GetPosition());
    ClosestHome->Dock(Robot, 200);
#ifdef IMPLEMENTED
    GetRobotWorldView()->GetActiveRobot()->SetCommResponseTimeOut(120);
#endif
#ifdef IMPLEMENTED
    short Frequencies[] = {2000, 1000, 2000, 1000, 2000, 1000, 2000, 1000, 2000, 1000, -1};
    int Durations[] = {500, 500, -500, -500, -500, -500, -500, -500, -500, -500, 1};
    GetRobotWorldView()->GetActiveRobot()->SetEStopVisible(true);
    robPOINT Dest = GetRobotWorldView()->GetActiveRobot()->GetPosition();
    Dest.x += 30000;
    Dest.y += 30000;
    GetRobotWorldView()->GetActiveRobot()->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
    GetRobotWorldView()->GetActiveRobot()->FollowInstructions();
    GetRobotWorldView()->GetActiveRobot()->SetEStopVisible(false);
    return;
#endif
#ifdef IMPLEMENTED
    const short cDanceVel = 200;
    const short cDancePwm = 255;

    for (int i = 0; i < 5; i++)
    {
        GetRobotWorldView()->GetActiveRobot()->AddVelocitiesToInstructions(cDanceVel, 0, 1000);
        GetRobotWorldView()->GetActiveRobot()->AddVelocitiesToInstructions(-cDanceVel, 0, 1000);
        GetRobotWorldView()->GetActiveRobot()->AddVelocitiesToInstructions(0, cDanceVel, 1000);
        GetRobotWorldView()->GetActiveRobot()->AddVelocitiesToInstructions(0, -cDanceVel, 1000);
    }

    //	for (i = 0; i < 5; i++) {
    //		GetRobotWorldView()->GetActiveRobot()->AddPwmToInstructions(cDancePwm, cDancePwm, 1000);
    //		GetRobotWorldView()->GetActiveRobot()->AddPwmToInstructions(-cDancePwm, -cDancePwm, 1000);
    //	}
    GetRobotWorldView()->GetActiveRobot()->FollowInstructions();
    return;

    fprintf(stderr, "BLAAAAAH!!!!");
    return;
#endif
#ifdef IMPLEME
    robPOINT Error(80, 80);
    GetRobotWorldView()->GetActiveRobot()->SetPositionError(Error);
    GetRobotWorldView()->GetActiveRobot()->RepositionRobotOnUnexpectedObstacle(GetRobotWorldView()->m_ProbabilityGrid, M0Front);
    GetRobotWorldView()->RedrawWindow();
#endif
#ifdef _IMPLEMENTED
    /*	short Frequencies[] = {1000, 900, 800, 700, 600};
    	int Durations[] = {200, 200, 200, 200, 200};
    	GetRobotWorldView()->GetActiveRobot()->SendTune(Frequencies, Durations, 5, true, false);
    	short Frequencies2[] = {500, 400, 300, 200, 100};
    	GetRobotWorldView()->GetActiveRobot()->SendTune(Frequencies2, Durations, 5, false, true);
    //	for (int i = 0; i < 44; i++) {
    //		GetRobotWorldView()->GetActiveRobot()->SendTune(Frequencies, Durations, 5);
    //		GetRobotWorldView()->GetActiveRobot()->SendTune(Frequencies2, Durations, 5);
    //	}
    	short Frequencies3[] = {-1};
    	GetRobotWorldView()->GetActiveRobot()->SendTune(Frequencies3, Durations, 1);
    */
    static bool Pause = false;

    if (GetRobotWorldView()->GetActiveRobot()->GetState() != stWaiting)
    {
        Pause = !Pause;
        GetRobotWorldView()->GetActiveRobot()->Pause(Pause);
        return;
    }

#define nNotes 41
    short Frequencies[nNotes];
    int Durations[nNotes];
    int i;

    /*	for (int i = 0; i < 150; i++) {
    		Frequencies[4*i] = 2000;
    		Frequencies[4*i + 1] = 100;
    		Durations[4*i] = 500;
    		Durations[4*i + 1] = 500;
    		Frequencies[4*i + 2] = 1;
    		Frequencies[4*i + 3] = 1000;
    		Durations[4*i + 2] = -1000;
    		Durations[4*i + 3] = -1000;
    	}
    */
    for (i = 0; i < nNotes / 4; i++)
    {
        Frequencies[4 * i] = 2000;
        Frequencies[4 * i + 1] = 2000;
        Frequencies[4 * i + 2] = 500;
        Frequencies[4 * i + 3] = 200;
        Durations[4 * i] = -1000;
        Durations[4 * i + 1] = -1000;
        Durations[4 * i + 2] = 500;
        Durations[4 * i + 3] = 500;

    }

    Frequencies[nNotes - 1] = -1;
    Durations[nNotes - 1] = 100;
    //	short Frequencies[] = {500, 2000, 20, 20, 2000, 2000, 20, 20, 2000, 2000, 20, 20, 2000, 2000, 20, 20, 2000, 400, 500, 2000, 20, 20, 2000, 2000, 20, 20, 2000, 2000, 20, 20, 2000, 2000, 20, 20, 2000, 400, -1};
    //	int Durations[] = {5000, -320, -320, -1000, -1000, -320, -320, -1000, -1000, -320, -320, -1000, -1000, -320, -320, -1000, -1000, 5000, 5000, -320, -320, -1000, -1000, -320, -320, -1000, -1000, -320, -320, -1000, -1000, -320, -320, -1000, -1000, 5000, -1000};
    //	short Frequencies[] = {1000, 990, 980, 970, 960, 950, 940, 930, 920, 910, 900, 910, 920, 930, 940, 950, 960, 970, 980, 990, 1000, -1};
    //	int Durations[] = {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
    //	short Frequencies[] = {1000, 900, 800, 700, 600, 500, 400, 300, 200, 100, -1};
    //	int Durations[] = {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
    GetRobotWorldView()->GetActiveRobot()->SetEStopVisible(true);
    robPOINT Dest = GetRobotWorldView()->GetActiveRobot()->GetPosition();
    Dest.x += 30000;
    Dest.y += 30000;
    //	GetRobotWorldView()->GetActiveRobot()->SetPositionVelocityDestination(Dest, 333);
    GetRobotWorldView()->GetActiveRobot()->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
    GetRobotWorldView()->GetActiveRobot()->FollowInstructions();
    GetRobotWorldView()->GetActiveRobot()->SetEStopVisible(false);

    //	for (short i = 0; i < 12; i++) {
    //	for (short i = 0; i < 1; i++) {
    //		GetRobotWorldView()->GetActiveRobot()->SendTune(Frequencies, Durations, 19);
    //	}
    //	GetRobotWorldView()->GetActiveRobot()->RepositionRobotOnUnexpectedObstacle(GetRobotWorldView()->m_ProbabilityGrid, M0Front);
    //	GetRobotWorldView()->RedrawWindow();
#endif
#ifdef IMPLEMENTED
    RECT UpdateRects[] =
    {
        0, 0, 1116, 574,
        0, 0, 1116, 669,
        0, 35, 211, 56,
        0, 0, 1116, 669,
        532, 300, 596, 360,
        662, 320, 734, 413,
        532, 300, 596, 360,
        288, 194, 734, 413
    };

    GetRobotWorldView()->GetActiveRobot()->m_Position.x = 1004;
    GetRobotWorldView()->GetActiveRobot()->m_Position.y = -363;
    static int i = 4;
    char buffer[10];
    m_edtVelocityDirectionHeading = _itoa(i, buffer, 10);
    UpdateData(FALSE);
    GetRobotWorldView()->GetActiveRobot()->GetParent()->RedrawWindow(&UpdateRects[i], NULL, RDW_INVALIDATE | RDW_NOERASE);

    if (i == 5)
    {
        i = 4;
    }
    else
    {
        i = 5;
    }

    //	if (i == sizeof(UpdateRects) / sizeof(RECT)) i = 0;
    //	else i++;
    return;
#endif

#ifdef IMPLEMENTED
    double Headings[] =
    {
        2.16661562316537e-002,
        1.62496171737403e-002,
        1.62496171737403e-002,
        6.27235222906376,
        6.24526953377419,
        6.18027106507923,
        6.09902297921053,
        5.99069219805226,
        5.87694487783608,
        5.7631975576199,
        5.68194947175119,
        5.63861715928789,
        5.63320062022997,
        5.6440336983458,
        5.66569985457745,
        5.70361562798285,
        5.76861409667781,
        5.85527872160443,
        5.94194334653104,
        6.03944104957348,
        6.15318836978966,
        0.,
        3.79157734053941e-002,
        7.04150077528747e-002,
        0.102914242100355,
        5.62778408117206
    };

    double PointList[] =
    {
        14., 0.,
        25., 0.,
        34., 0.,
        63., 0.,
        73., 0.,
        86., 0.,
        98., -2.,
        108., -4.,
        117., -7.,
        124., -11.,
        133., -17.,
        145., -25.,
        157., -34.,
        168., -43.,
        180., -51.,
        191., -59.,
        203., -66.,
        214., -72.,
        224., -76.,
        234., -79.,
        245., -81.,
        264., -81.,
        275., -81.,
        286., -81.,
        297., -80.,
        754., 102.
    };

    /*	double Headings[] = {
    		2.16661562316537e-002,
    		1.62496171737403e-002,
    		1.62496171737403e-002,
    		6.27235222906376,
    		6.24526953377419,
    		6.18027106507923,
    		6.09902297921053,
    		5.99069219805226,
    		5.87694487783608,
    		5.7631975576199,
    		5.68194947175119,
    		5.63861715928789,
    		5.63320062022997,
    		5.6440336983458,
    		5.66569985457745,
    		5.70361562798285,
    		5.76861409667781,
    		5.85527872160443,
    		5.94194334653104,
    		6.03944104957348,
    		6.15318836978966,
    		0.,
    		3.79157734053941e-002,
    		7.04150077528747e-002,
    		0.102914242100355,
    		0.140830015505749,
    		0.167912710795317,
    		0.178745788911143,
    		0.18957886702697,
    		0.205828484200711,
    		0.211245023258624,
    		0.232911179490278,
    		0.249160796664018,
    		0.249160796664018,
    		0.265410413837758,
    		0.270826952895672,
    		0.281660031011499,
    		0.324992343474806,
    		0.34665849970646,
    		0.379157734053941,
    		0.411656968401421,
    		0.454989280864729,
    		0.482071976154296,
    		0.530820827675517,
    		0.563320062022997,
    		0.601235835428391,
    		0.639151608833786,
    		0.67706738223918,
    		0.714983155644574,
    		0.742065850934141,
    		0.763732007165794,
    		0.763732007165794,
    		0.747482389992054,
    		0.725816233760401,
    		0.655401226007526,
    		0.51998774955969,
    		0.384574273111854,
    		0.232911179490278,
    		9.74977030424419e-002,
    		6.24526953377419,
    		6.12068913544218,
    		5.99610873711017,
    		5.88777795595191,
    		5.77944717479364,
    		5.68194947175119,
    		5.62778408117206
    	};

    	double PointList[] = {
    		14., 0.,
    		25., 0.,
    		34., 0.,
    		63., 0.,
    		73., 0.,
    		86., 0.,
    		98., -2.,
    		108., -4.,
    		117., -7.,
    		124., -11.,
    		133., -17.,
    		145., -25.,
    		157., -34.,
    		168., -43.,
    		180., -51.,
    		191., -59.,
    		203., -66.,
    		214., -72.,
    		224., -76.,
    		234., -79.,
    		245., -81.,
    		264., -81.,
    		275., -81.,
    		286., -81.,
    		297., -80.,
    		311., -78.,
    		325., -77.,
    		339., -74.,
    		354., -71.,
    		367., -68.,
    		381., -66.,
    		394., -63.,
    		404., -60.,
    		416., -57.,
    		423., -55.,
    		428., -54.,
    		430., -54.,
    		435., -52.,
    		449., -47.,
    		463., -42.,
    		475., -36.,
    		488., -31.,
    		500., -25.,
    		513., -17.,
    		526., -9.,
    		540., 0.,
    		554., 9.,
    		567., 19.,
    		581., 31.,
    		594., 42.,
    		608., 56.,
    		621., 68.,
    		633., 79.,
    		646., 91.,
    		658., 101.,
    		667., 108.,
    		678., 113.,
    		689., 116.,
    		699., 118.,
    		710., 118.,
    		720., 118.,
    		729., 115.,
    		737., 113.,
    		744., 108.,
    		751., 104.,
    		754., 102.
    	};
    */
    /*	filebuf fb(_sopen("stdout.txt", _O_CREAT, _SH_DENYWR, _S_IWRITE));
    	cout = &fb;         // fb associated with cout
    	cout << "Hello";
    	cout.flush();
    */
    int NumPoints = sizeof(PointList) / sizeof(double) / 2;
    robPOINT* Positions = new robPOINT[NumPoints];

    for (int i = 0; i < NumPoints; i++)
    {
        Positions[i].x = PointList[2 * i];
        Positions[i].y = PointList[2 * i + 1];
    }

    GetRobotWorldView()->GetActiveRobot()->SetFloorType(ftThinCarpet);
    GetRobotWorldView()->GetActiveRobot()->CalculatePositionAndHeadingError(Positions[0], Headings[0], true, true);

    for (i = 0; i < sizeof(Headings) / sizeof(double); i++)
    {
        GetRobotWorldView()->GetActiveRobot()->CalculatePositionAndHeadingError(Positions[i], Headings[i], false, false);
    }

    delete Positions;
    return;

#endif

#ifdef IMPLEMENTED
    GetRobotWorldView()->GetActiveRobot()->SendRequestEX2();
    return;
    GetRobotWorldView()->GetActiveRobot()->AddToHeading(pi / 2);
    return;
    GetRobotWorldView()->GetActiveRobot()->DisplayErrorText(IDS_ERR_PATH_UNEXPECTED_OBSTACLE); //AfxMessageBox(IDS_ERR_PATH_UNEXPECTED_OBSTACLE, MB_OK, 0);
    GetRobotWorldView()->GetActiveRobot()->DisplayErrorText(IDS_ERR_PATH_TIMEOUT);//AfxMessageBox(IDS_ERR_PATH_TIMEOUT, MB_OK, 0);
    GetRobotWorldView()->GetActiveRobot()->DisplayErrorText(IDS_ERR_PATH_NO_OBSTACLE); //AfxMessageBox(IDS_ERR_PATH_NO_OBSTACLE);
    GetRobotWorldView()->GetActiveRobot()->DisplayErrorText(IDS_ERR_PATH_NO_TIMEOUT); //AfxMessageBox(IDS_ERR_PATH_NO_TIMEOUT);
    GetRobotWorldView()->GetActiveRobot()->DisplayErrorText(IDS_ERR_PATH_NO_ESTOP); //AfxMessageBox(IDS_ERR_PATH_NO_ESTOP);
    GetRobotWorldView()->GetActiveRobot()->DisplayErrorText(IDS_ERR_PATH_GENERAL_ERROR); //AfxMessageBox(IDS_ERR_PATH_GENERAL_ERROR, MB_OK, 0);
    GetRobotWorldView()->GetActiveRobot()->DisplayErrorText("Hello there\nHow're you doing?");
    GetRobotWorldView()->GetActiveRobot()->DisplayErrorText("Hi");

    return;
    //	((CRobotWorldView*)m_ParentView)->OnBatteryChange(3);
    return;

    robPOINT DPosition;
    DPosition.x = -12.55;
    DPosition.y = 39.445;
    double DHeading = 0.02331;
    GetRobotWorldView()->GetActiveRobot()->ShowCalibrationSuccessfulDialog(DPosition, DHeading);
    return;

    unsigned short SirenCodeAddress = 0xA66D;
    unsigned char SirenCodeLength = 34;
    /*	unsigned char SirenCode[] = {0x9B, 0x01, 0xEF, 0x92, 0x00, 0xDF, 0x0C, 0xC9,
    		0xE8, 0x03, 0xEF, 0x5F, 0xCE, 0xC7, 0x01, 0xEF, 0x92, 0x00, 0xF0, 0xC9,
    		0xF4, 0x01, 0xEF, 0x53, 0xCE, 0xB1, 0x01, 0x1C, 0xC7, 0x01, 0xEF, 0x92,
    		0x1C, 0xF0};
    */	unsigned char SirenCode[] = {0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
                                     0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
                                     0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
                                     0xF0, 0xF0
                                   };
    /*Sending Robot RAM data for Siren Code, and executing it*/
    GetRobotWorldView()->GetActiveRobot()->SetRetryLimit(0);
    GetRobotWorldView()->GetActiveRobot()->SendRAMData(0xA66D, 10, SirenCode);
    GetRobotWorldView()->GetActiveRobot()->SendRAMData(0xA66D + 10, 10, SirenCode + 10);
    GetRobotWorldView()->GetActiveRobot()->SendRAMData(0xA66D + 20, 10, SirenCode + 20);
    GetRobotWorldView()->GetActiveRobot()->SendRAMData(0xA66D + 30, 4, SirenCode + 30);

    GetRobotWorldView()->GetActiveRobot()->AddBeepToInstructions(200, 500);
    GetRobotWorldView()->GetActiveRobot()->AddWaitForNudgeToInstructions(0);
    GetRobotWorldView()->GetActiveRobot()->FollowInstructions();

    static bool state = true;

    if (state)
    {
        unsigned char Data[2] = {0, 0};
        GetRobotWorldView()->GetActiveRobot()->SendRAMData(0x8000, 2, Data);
        state = false;
    }
    else
    {
        unsigned char Data[2] = {0x6D, 0xA6};
        GetRobotWorldView()->GetActiveRobot()->SendRAMData(0x8000, 2, Data);
        state = true;
    }

    GetRobotWorldView()->GetActiveRobot()->SetRetryLimit(7);
    return;

    /*Sending Robot RAM data to execute siren code*/
    //	static bool state = true;
    if (state)
    {
        unsigned char Data[2] = {0, 0};
        GetRobotWorldView()->GetActiveRobot()->SendRAMData(0x8000, 2, Data);
        state = false;
    }
    else
    {
        unsigned char Data[2] = {0x6D, 0x56};
        GetRobotWorldView()->GetActiveRobot()->SendRAMData(0x8000, 2, Data);
        state = true;
    }

    return;

    /*Sending robot serial string*/
    CString String('f', 3);
    String.SetAt(1, 255);
    String.SetAt(2, 0);
    GetRobotWorldView()->GetActiveRobot()->SendSerialString(String);
    return;

    /*Reading Intel Hex Files*/
    short checksum = 0;
    ofstream hexfile("hexfile.hex");
    hexfile.setmode(filebuf::text);
    unsigned int offset = 0;
    unsigned short data[21];

    while (offset < 0xFFFF)
    {
        checksum = 0;
        data[0] = 0x10;
        data[1] = (offset) >> 8;
        data[2] = (offset & 0xFF);
        data[3] = 0;

        for (int i = 0; i < 16; i++)
        {
            data[i + 4] = 0x00;
        }

        offset += 0x10;
        checksum = 0;
        hexfile << ":";

        for (i = 0; i < 20; i++)
        {
            checksum += data[i];

            if (data[i] < 0x10)
            {
                hexfile << '0';
            }

            hexfile << hex << data[i];
            //			hexfile << setw(2) << setfill('0') << hex << data[i];
        }

        data[20] = (256 - (checksum % 256)) & 0xFF;

        //		hexfile << setw(2) << setfill('0') << hex << data[20] << "\n";
        if (data[20] < 0x10)
        {
            hexfile << '0';
        }

        hexfile << hex << data[20] << "\n";
    }

    hexfile << ":00000001FF";
    return;

    robPOINT Point(0.5, 0.5);
    double dHeading = 0.00003;
    GetRobotWorldView()->GetActiveRobot()->ShowCalibrationSuccessfulDialog(Point, dHeading);
    return;
    /*Back and Forth
    #define Speed 300
    #define Time 3000
    	GetRobotWorldView()->GetActiveRobot()->AddStraightVelocityToInstructions(0, Speed, NULL, Time);
    	GetRobotWorldView()->GetActiveRobot()->AddStraightVelocityToInstructions(0, -Speed, NULL, Time);
    	GetRobotWorldView()->GetActiveRobot()->AddStraightVelocityToInstructions(0, Speed, NULL, Time);
    	GetRobotWorldView()->GetActiveRobot()->AddStraightVelocityToInstructions(0, -Speed, NULL, Time);
    	GetRobotWorldView()->GetActiveRobot()->AddStraightVelocityToInstructions(0, Speed, NULL, Time);
    	GetRobotWorldView()->GetActiveRobot()->AddStraightVelocityToInstructions(0, -Speed, NULL, Time);
    	GetRobotWorldView()->GetActiveRobot()->AddStraightVelocityToInstructions(0, Speed, NULL, Time);
    	GetRobotWorldView()->GetActiveRobot()->AddStraightVelocityToInstructions(0, -Speed, NULL, Time);
    	GetRobotWorldView()->GetActiveRobot()->AddStraightVelocityToInstructions(0, Speed, NULL, Time);
    	GetRobotWorldView()->GetActiveRobot()->AddStraightVelocityToInstructions(0, -Speed, NULL, Time);
    	GetRobotWorldView()->GetActiveRobot()->FollowInstructions(TRUE, TRUE, PR_InstructionsComplete);
    */
    //	TCoordSystem CoordSystem;
    //	CoordSystem.Phi = 0;
    //	GetRobotWorldView()->GetActiveRobot()->AddTurnToInstructions(5*pi/4, 30, &CoordSystem, 0, 200, 0);
    //	GetRobotWorldView()->GetActiveRobot()->FollowInstructions(TRUE, TRUE);

    /*	robRECT PatchBounds;
    	TEnterSide EnterSide;
    	short PathWidth = 75;
    	const unsigned short cWidth = 800;
    	const unsigned short cHeight = 700;
    	PatchBounds.left = 0;
    	PatchBounds.top = 0;
    	PatchBounds.right = PatchBounds.left + cWidth;
    	PatchBounds.bottom = PatchBounds.top - cHeight;
    	EnterSide = esLftBot;
    	CVacuumPatch VacuumPatch(PatchBounds, EnterSide, PathWidth, 220, GetRobotWorldView()->GetActiveRobot());
    */
    /*	GetRobotWorldView()->GetActiveRobot()->AddWaitForNudgeToInstructions(5000);
    	GetRobotWorldView()->GetActiveRobot()->AddBeepToInstructions(1000, 2000);
    	GetRobotWorldView()->GetActiveRobot()->AddWaitToInstructions(1000);
    	GetRobotWorldView()->GetActiveRobot()->AddBeepToInstructions(1000, 2000);
    	if (GetRobotWorldView()->GetActiveRobot()->FollowInstructions(TRUE, true) == IR_InstructionComplete) {
    		m_edtObstacle = "NUDGED";
    	} else {
    		m_edtObstacle = "Not Nudged";
    	}
    	UpdateData(false);
    */
    CCTDPath* NewPath = new CCTDPath;
    robPOINT p1, p2;
    p1.x = 550;
    p1.y = 0;
    p2.x = 0;
    p2.y = 0;
    //	NewPath->AddStraightLine(p1,p2);
    /*	p1.x = 550;
    	p1.y = 0;
    	p2.x = 550;
    	p2.y = 500;
    	NewPath->AddStraightLine(p1,p2);
    */
    p1.x = GetRobotWorldView()->GetActiveRobot()->m_Position.x;
    p1.y = GetRobotWorldView()->GetActiveRobot()->m_Position.y;
    p2 = p1;
    NewPath->AddStraightLine(p1, p2);
    GetRobotWorldView()->GetActiveRobot()->AddCTDPathToInstructions(NewPath, 0, FALSE, 0);
    GetRobotWorldView()->GetActiveRobot()->FollowInstructions(TRUE, TRUE);
#endif
}

void CDlgMotorControls::DoMessages(void)
/*******************************************************************************
//Created: 10/25/96 S.R.
//Last Revision: 10/29/96 S.R.
//Parameters:
//	none.
//Return Value:
//	none.
********************************************************************************
//Description:
//		This function can be used to process messages while in the middle of an
//	intensive algorithm or while waiting for something to happen.
//Method:
//		The PeekMessage function is used to see if there are any messages, and
//	if there are, they are removed and returned through message.  TranslateMessage
//	and DispatchMessage then take care of the message.
*******************************************************************************/
{
    MSG message;

    if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

void CDlgMotorControls::OnButCalibrateHeading()
{
    m_edtTotalEncoderTicks = "Working";
    UpdateData(FALSE);
    int result;
    int temp = GetRobotWorldView()->GetActiveRobot()->CalibrateFloorSurface(&result);

    char string[20];
    m_edtTotalEncoderTicks = _itoa(result, string, 10);
    m_edtKHeadErr = GetRobotWorldView()->GetActiveRobot()->m_KHeadErr;
    UpdateData(FALSE);
}

void CDlgMotorControls::OnButDo360()
{
    GetRobotWorldView()->GetActiveRobot()->AddHeadingToInstructions(GetRobotWorldView()->GetActiveRobot()->m_Heading + 3 * pi / 4, NULL, pi / 2, 360, 0);
    GetRobotWorldView()->GetActiveRobot()->AddHeadingToInstructions(GetRobotWorldView()->GetActiveRobot()->m_Heading + 5 * pi / 4, NULL, pi / 2, 360, 0);
    GetRobotWorldView()->GetActiveRobot()->AddHeadingToInstructions(GetRobotWorldView()->GetActiveRobot()->m_Heading + 7 * pi / 2, NULL, pi / 2, 360, 0);
    GetRobotWorldView()->GetActiveRobot()->AddHeadingToInstructions(GetRobotWorldView()->GetActiveRobot()->m_Heading, NULL, 0, 360, 0);
    GetRobotWorldView()->GetActiveRobot()->FollowInstructions(TRUE, TRUE);
}

void CDlgMotorControls::OnButResetPosition()
{
    //	fprintf(((CRobotWorldView*)m_ParentView)->m_OutFile, "***ResetPosition***\n");
    robPOINT NewPosition(0, 0);
    GetRobotWorldView()->GetActiveRobot()->SetPosition(NewPosition);
    GetRobotWorldView()->GetActiveRobot()->SetHeading(0);
    GetRobotWorldView()->GetActiveRobot()->m_PositionError.x = 0;
    GetRobotWorldView()->GetActiveRobot()->m_PositionError.y = 0;
    GetRobotWorldView()->GetActiveRobot()->m_HeadingError = 0;
    m_M0Encoder = 0;
    m_M1Encoder = 0;
}

void CDlgMotorControls::OnButCreateDoorway()
{
    robPOINT P0, P1;
    P0.x = 0;
    P0.y = 0;
    P1.x = 360;
    P1.y = 200;
    double H0, H1;
    H0 = pi / 2;
    H1 = -pi / 2;
    //	m_CalibrationZoneHandler->ConstructCalibrationZone(P0, H0, P1, H1, czDoorway);
}

void CDlgMotorControls::OnButcreateWall()
{
    robPOINT P0, P1;
    P0.x = -800;
    P0.y = -100;
    P1.x = -900;
    P1.y = 0;
    double H0, H1;
    H0 = pi / 4;
    H1 = pi / 4;
    robPOINT P2;
    P2.x = -750;
    P2.y = -200;
    double H2 = -pi / 4;
    //	m_CalibrationZoneHandler->ConstructCalibrationZone(P0, H0, P1, H1, czWall);
    //	CCornerWall* NewZone = new CCornerWall(P0,P1,P2,H0,H1,H2);
    //	m_CalibrationZoneHandler->Add(NewZone);
}

void CDlgMotorControls::OnButCalibrate()
{
    //	CCalibrationZone* CloseZone;
    //	CloseZone = m_CalibrationZoneHandler->FindClosestCalibrationZone(GetRobotWorldView()->GetActiveRobot()->m_Position);
    //	GetRobotWorldView()->GetActiveRobot()->CalibratePosition(CloseZone);
}

void CDlgMotorControls::OnButSendDefaultCorrection()
{
    //	GetRobotWorldView()->GetActiveRobot()->SetHeadingCorrectionFactor(26);	//TANK (Prototype I)
    //	GetRobotWorldView()->GetActiveRobot()->SetHeadingCorrectionFactor(8);		//PORSCHE (Prototype II)
    //	GetRobotWorldView()->GetActiveRobot()->SetHeadingCorrectionFactor(-11);	//PORSCHE (Prototype II) (5 encoder blades)
    //	GetRobotWorldView()->GetActiveRobot()->SetHeadingCorrectionFactor(-12);	//PORSCHE (Prototype II) With Sprockets (5 encoder blades)
    //	GetRobotWorldView()->GetActiveRobot()->SetHeadingCorrectionFactor(220);	//PORSCHE (Prototype II) With Sprockets (5 encoder blades), 1160 cNHeadings
    GetRobotWorldView()->GetActiveRobot()->SetHeadingCorrectionFactor(30);	//PORSCHE (Prototype III) With Sprockets (5 encoder blades), 1160 cNHeadings
}

void CDlgMotorControls::OnButSetMaxSpeed()
{
    UpdateData(TRUE);
    GetRobotWorldView()->GetActiveRobot()->SetMaxSpeed(atoi(m_edtMaxSpeed));
}

BOOL CDlgMotorControls::GetFlipHandle(void)
{
    UpdateData(TRUE);
    return m_chkFlipHandle;
}

void CDlgMotorControls::OnButSetStraightHeading()
{
    UpdateData(TRUE);
    GetRobotWorldView()->GetActiveRobot()->AddStraightVelocityToInstructions(atof(LPCTSTR(m_edtStraightHeading)), cComputerControlSpeed, NULL, 5000);
    TInstructionResult InstructionsResult = GetRobotWorldView()->GetActiveRobot()->FollowInstructions(TRUE, TRUE);
}

void CDlgMotorControls::OnButPurgeRingBuffer()
{
    GetRobotWorldView()->GetActiveRobot()->PurgeRingBuffer();
}

void CDlgMotorControls::OnObstacleHighSensitivity()
{
    GetRobotWorldView()->GetActiveRobot()->SetObstacleSensitivity(osHighSensitivity);
}

void CDlgMotorControls::OnObstacleLowSensitivity()
{
    GetRobotWorldView()->GetActiveRobot()->SetObstacleSensitivity(osLowSensitivity);
}

void CDlgMotorControls::OnObstacleMediumSensitivity()
{
    GetRobotWorldView()->GetActiveRobot()->SetObstacleSensitivity(osMediumSensitivity);
}

void CDlgMotorControls::OnObstacleNone()
{
    GetRobotWorldView()->GetActiveRobot()->SetObstacleSensitivity(osNone);
}

void CDlgMotorControls::OnButShowPosition()
{
#define RobotToWorldUnits(RobotUnit) (RobotUnit * cTPI / cRobotEncoderTicksPerInch)

    m_edtH = m_CurrentHeading;
    m_edtX = (long)GetRobotWorldView()->GetActiveRobot()->m_Position.x;
    m_edtY = (long)GetRobotWorldView()->GetActiveRobot()->m_Position.y;
    m_edtM0Encoder = (long)RobotToWorldUnits(m_M0Encoder);
    m_edtM1Encoder = (long)RobotToWorldUnits(m_M1Encoder);
    UpdateData(FALSE);
}

void CDlgMotorControls::OnButNoCorrection()
{
    GetRobotWorldView()->GetActiveRobot()->SetHeadingCorrectionFactor(0);
}

void CDlgMotorControls::OnButSendMotorControlConstants()
{
    UpdateData(TRUE);
    GetRobotWorldView()->GetActiveRobot()->SetMotorControlConstants(m_edtKp, m_edtKi, m_edtKd, m_edtKb, m_edtKPwm);
    GetRobotWorldView()->GetActiveRobot()->SetHandleLength(m_edtHandleLength);
}

void CDlgMotorControls::OnButSendMotoConDefaults()
{
    m_edtKp = 3;
    m_edtKi = 6;
    m_edtKd = 5;
    m_edtKb = 8;
    m_edtKPwm = 0;
    m_edtHandleLength = (short)cRRWheelBase;
    GetRobotWorldView()->GetActiveRobot()->SetMotorControlConstants(m_edtKp, m_edtKi, m_edtKd, m_edtKb, m_edtKPwm);
    UpdateData(FALSE);
}

void CDlgMotorControls::OnButFollowPath()
{
#define cPathSize 10
#define cSize 19
    robPOINT Path[cSize] =
    {
        robPOINT(0, 0),
        robPOINT(10, 10),
        robPOINT(20, 20),
        robPOINT(30, 30),
        robPOINT(40, 40),
        robPOINT(50, 50),
        robPOINT(60, 60),
        robPOINT(70, 70),
        robPOINT(80, 80),
        robPOINT(90, 90),
        robPOINT(100, 100),
        robPOINT(110, 110),
        robPOINT(120, 120),
        robPOINT(130, 130),
        robPOINT(140, 140),
        robPOINT(150, 150),
        robPOINT(160, 160),
        robPOINT(170, 170),
        robPOINT(180, 180),
    };

    /*	GetRobotWorldView()->GetActiveRobot()->GetRobotWorldView()->GetActiveRobot()Comm->SendPathSegment(TRUE, Path[0]);
    	for (unsigned char i = 1; i < cSize; i++) {
    		GetRobotWorldView()->GetActiveRobot()->GetRobotWorldView()->GetActiveRobot()Comm->SendPathSegment(FALSE, Path[i]);
    	}
    */
    //	m_Path[1].x = 100;
    //	m_Path[1].y = 100;
    //	GetRobotWorldView()->GetActiveRobot()->GetRobotWorldView()->GetActiveRobot()Comm->SendPathSegment(TRUE, &m_Path[0], 2);

    //	GetRobotWorldView()->GetActiveRobot()->GetRobotWorldView()->GetActiveRobot()Comm->SendPathSegment(TRUE, &m_Path[0], cPathSize);
    //	m_PathPosition = cPathSize;

    //	for (m_PathPosition = 1; m_PathPosition < cPathSize; m_PathPosition++) {
    //		GetRobotWorldView()->GetActiveRobot()->GetRobotWorldView()->GetActiveRobot()Comm->SendPathSegment(FALSE, m_Path[m_PathPosition]);
    //	}

    //	GetRobotWorldView()->GetActiveRobot()->GetRobotWorldView()->GetActiveRobot()Comm->SendFollowPath(TRUE);
    CCTDPath* NewPath = new CCTDPath;
    robPOINT p1, p2, p3;
    p1 = GetRobotWorldView()->GetActiveRobot()->m_Position;
    p2.x = p1.x + 200 * cos(GetRobotWorldView()->GetActiveRobot()->m_Heading);
    p2.y = p1.y + 200 * sin(GetRobotWorldView()->GetActiveRobot()->m_Heading);
    NewPath->AddStraightLine(p1, p2);
    /*	p1.x = 550;
    	p1.y = 0;
    	p2.x = 550;
    	p2.y = 500;
    	NewPath->AddStraightLine(p1,p2);
    */
    p1.x = 0;
    p1.y = 0;
    p2.x = 0;
    p2.y = 1200;
    p3.x = -600;
    p3.y = 600;
    //	NewPath->AddArc(p1, p2, p3);
    GetRobotWorldView()->GetActiveRobot()->AddCTDPathToInstructions(NewPath, 0, FALSE, 0);
    GetRobotWorldView()->GetActiveRobot()->FollowInstructions(TRUE, TRUE);
    //	GetRobotWorldView()->GetActiveRobot()->StartFollowingCTDPath(NewPath);
}

CDlgMotorControls::~CDlgMotorControls()
{
    delete m_Path;
}

void CDlgMotorControls::OnChkRecordInfo()
{
    UpdateData(TRUE);
}

void CDlgMotorControls::OnButSendBuzzer()
{
    UpdateData(TRUE);
    GetRobotWorldView()->GetActiveRobot()->SetBuzzerFrequency(m_BuzzerFrequency);
}

void CDlgMotorControls::OnButSet()
{
    UpdateData(TRUE);
    GetRobotWorldView()->GetActiveRobot()->SetP1(m_edtP1);
    GetRobotWorldView()->GetActiveRobot()->SetP2(m_edtP2);
    GetRobotWorldView()->GetActiveRobot()->SetP3(m_edtP3);
    GetRobotWorldView()->GetActiveRobot()->SetP4(m_edtP4);
    GetRobotWorldView()->GetActiveRobot()->SetP5(m_edtP5);
    GetRobotWorldView()->GetActiveRobot()->SetP6(m_edtP6);
    GetRobotWorldView()->GetActiveRobot()->SetP7(m_edtP7);
    GetRobotWorldView()->GetActiveRobot()->SetP8(m_edtP8);
}

void CDlgMotorControls::OnButParmDefault()
{
    UpdateData(TRUE);
    m_edtP1 = 1500;
    m_edtP2 = 100;
    m_edtP3 = 90;
    m_edtP4 = 500;
    m_edtP5 = 800;
    m_edtP6 = 51;
    m_edtP7 = 250;
    m_edtP8 = 0;
    GetRobotWorldView()->GetActiveRobot()->SetP1(m_edtP1);
    GetRobotWorldView()->GetActiveRobot()->SetP2(m_edtP2);
    GetRobotWorldView()->GetActiveRobot()->SetP3(m_edtP3);
    GetRobotWorldView()->GetActiveRobot()->SetP4(m_edtP4);
    GetRobotWorldView()->GetActiveRobot()->SetP5(m_edtP5);
    GetRobotWorldView()->GetActiveRobot()->SetP6(m_edtP6);
    GetRobotWorldView()->GetActiveRobot()->SetP7(m_edtP7);
    GetRobotWorldView()->GetActiveRobot()->SetP8(m_edtP8);
    UpdateData(FALSE);
}

void CDlgMotorControls::OnButSendCorrection()
{
    UpdateData(TRUE);
    GetRobotWorldView()->GetActiveRobot()->SetHeadingCorrectionFactor(atoi(m_edtCorrection));
}

void CDlgMotorControls::OnButWriteProbGrid()
{
    ((CRobotWorldView*)m_ParentView)->m_ProbabilityGrid->ProbGridToFile();
}

void CDlgMotorControls::OnButFindPath()
{
#define DO_THREAD
    int i = 0;
    //	double qualities[5];
    UpdateData(TRUE);
    long x1 = atoi(m_edtStartX);
    long y1 = atoi(m_edtStartY);
    long x2 = atoi(m_edtEndX);
    long y2 = atoi(m_edtEndY);

#ifdef DO_THREAD

    robPOINT Start, Stop;

    Start.x = x1;
    Start.y = y1;
    Stop.x = x2;
    Stop.y = y2;

    GetRobotWorldView()->GetActiveRobot()->FindPath(Start, Stop, atoi(m_edtIterations), atoi(m_edtRepeat));

#else

    //#define new DEBUG_NEW
    World* world = ((CRobotWorldView*)m_ParentView)->m_ProbabilityGrid->GetRobotWorldView()->GetActiveRobot()World;
    Path* InitialPath = new Path(world, x1, y1, x2, y2);
    Path* FoundPath;
    //	Path* FoundPath = world->FindPath(InitialPath, 100);
    //	delete InitialPath;

    //	delete FoundPath;
    char* blah = new char[3];
    unsigned int Repeat = atoi(m_edtRepeat);
    unsigned int Iterations = atoi(m_edtIterations);
    bool bStopFindPath = FALSE;
    unsigned int Repeats = 0;
    const unsigned int cMaxRepeats = 5;

    do
    {
        for (i = 0; ((i < Repeat) && (!bStopFindPath)); i++)
        {
            FoundPath = world->FindPath(InitialPath, Iterations);
            delete InitialPath;
            InitialPath = FoundPath;
        }

        FoundPath->GetQualities(qualities);
        Repeats++;
    }
    while ((qualities[MinFound] < cGoodPathMinFoundValue) && (Repeats < cMaxRepeats) && (!bStopFindPath));

    delete FoundPath;
    //#undef new
#endif



    /*
    	TPathThreadInput* PathThreadInput = new TPathThreadInput;
    	PathThreadInput->StartX = atoi(m_edtStartX);
    	PathThreadInput->StartY = atoi(m_edtStartY);
    	PathThreadInput->EndX = atoi(m_edtEndX);
    	PathThreadInput->EndY = atoi(m_edtEndY);
    	PathThreadInput->Iterations = atoi(m_edtIterations);
    	PathThreadInput->Repeat = atoi(m_edtRepeat);
    	PathThreadInput->world = ((CRobotWorldView*)m_ParentView)->m_ProbabilityGrid->GetRobotWorldView()->GetActiveRobot()World;
    //	PathThreadInput->Parent = ((CRobotWorldView*)m_ParentView)->GetSafeHwnd();
    	PathThreadInput->Parent = GetSafeHwnd();
    	Path* FoundPath = ((TPathThreadOutput*)FindPathThread(PathThreadInput))->FoundPath;
    */
}
/*
robPOINT GetStartPoint(CCalibrationZone* CalZone) {
	robPOINT Stop;
	if (CalZone->IsKindOf(RUNTIME_CLASS(CDoorway))) {
		Stop.x = (CalZone->m_P0.x + CalZone->m_P1.x) / 2.0;
		Stop.y = (CalZone->m_P0.y + CalZone->m_P1.y) / 2.0;
	} else {
		Stop.x = (CalZone->m_P0.x + CalZone->m_P1.x) / 2.0 - 2*cRRBodyWidth*cos(CalZone->m_Heading);
		Stop.y = (CalZone->m_P0.y + CalZone->m_P1.y) / 2.0 - 2*cRRBodyWidth*sin(CalZone->m_Heading);
	}
	return Stop;
}

CDlgMotorControls* MotorControls;

void FindPathFunction(Path* FoundPath, UINT FoundPathParam)
{
#define GetWorldView() ((CRobotWorldView*)(MotorControls->m_ParentView))
	static Path* FoundPath0 = NULL;
	static Path* FoundPath1 = NULL;
	static Path* FoundPath2 = NULL;
	switch (FoundPathParam) {
	case 0:
		FoundPath0 = FoundPath;
		break;
	case 1:
		FoundPath1 = FoundPath;
		break;
	case 2:
		FoundPath2 = FoundPath;

		double qualities[5];
		double MinDist;
		Path* MinPath = NULL;
		if (FoundPath0 != NULL) {
			FoundPath0->GetQualities(qualities);
			if (qualities[MinFound] >= FoundPath0->World->OpenValue()) {
				MinDist = qualities[FreeDist];
				MinPath = FoundPath0;
			}
		}
		if (FoundPath1 != NULL) {
			FoundPath1->GetQualities(qualities);
			if (qualities[MinFound] >= FoundPath1->World->OpenValue()) {
				if (qualities[FreeDist] < MinDist) {
					MinDist = qualities[FreeDist];
					MinPath = FoundPath1;
				}
			}
		}
		if (FoundPath2 != NULL) {
			FoundPath2->GetQualities(qualities);
			if (qualities[MinFound] >= FoundPath2->World->OpenValue()) {
				if (qualities[FreeDist] < MinDist) {
					MinDist = qualities[FreeDist];
					MinPath = FoundPath2;
				}
			}
		}
		if (MinPath != NULL) {
			CDC* pdc = GetWorldView()->GetDC();
			GetWorldView()->DrawFoundPath(pdc, MinPath);
		}


		delete FoundPath0;
		FoundPath0 = NULL;
		delete FoundPath1;
		FoundPath1 = NULL;
		delete FoundPath2;
		FoundPath2 = NULL;
		break;
	}
}
*/
void CDlgMotorControls::OnButFindCalZone()
{
    /*	MotorControls = this;

    	int i = 0;
    	double qualities[5];
    	UpdateData(TRUE);
    	long x1 = atoi(m_edtFindCalZoneX);
    	long y1 = atoi(m_edtFindCalZoneY);

    	robPOINT Start, Stop;

    	Start.x = x1;
    	Start.y = y1;
    	CRobotWorldView* WorldView = (CRobotWorldView*)m_ParentView;
    	CCalibrationZone* CalZonesList[3];
    	unsigned int nZones = WorldView->m_CalibrationZoneList->Find3ClosestCalibrationZones(Start, CalZonesList);
    	CDC* pdc = WorldView->GetDC();
    	CPen Pen0(PS_SOLID, 1, COLORREF(0)), Pen1(PS_SOLID, 1, COLORREF(0xFF)), Pen2(PS_SOLID, 1, COLORREF(0xFF0000));
    	POINT start;
    	POINT end;
    	CPen* OldPen = pdc->SelectObject(&Pen0);
    	start = WorldView->RobotToClientCoords(Start);
    	if (nZones > 0) {
    		Stop = GetStartPoint(CalZonesList[0]);
    		end = WorldView->RobotToClientCoords(Stop);
    		pdc->MoveTo(start);
    		pdc->LineTo(end);
    	}
    	if (nZones > 1) {
    		pdc->SelectObject(&Pen1);
    		Stop = GetStartPoint(CalZonesList[1]);
    		end = WorldView->RobotToClientCoords(Stop);
    		pdc->MoveTo(start);
    		pdc->LineTo(end);
    	}
    	if (nZones > 2) {
    		pdc->SelectObject(&Pen2);
    		Stop = GetStartPoint(CalZonesList[2]);
    		end = WorldView->RobotToClientCoords(Stop);
    		pdc->MoveTo(start);
    		pdc->LineTo(end);
    	}
    	pdc->SelectObject(OldPen);
    	Pen0.DeleteObject();
    	Pen1.DeleteObject();
    	Pen2.DeleteObject();
    	GetRobotWorldView()->GetActiveRobot()->FindPath(Start, GetStartPoint(CalZonesList[0]), cFindPathIterationsDefault, cFindPathRepeatDefault, TRUE, FindPathFunction, 0);
    	GetRobotWorldView()->GetActiveRobot()->FindPath(Start, GetStartPoint(CalZonesList[1]), cFindPathIterationsDefault, cFindPathRepeatDefault, TRUE, FindPathFunction, 1);
    	GetRobotWorldView()->GetActiveRobot()->FindPath(Start, GetStartPoint(CalZonesList[2]), cFindPathIterationsDefault, cFindPathRepeatDefault, TRUE, FindPathFunction, 2);
    */
    //	GetRobotWorldView()->GetActiveRobot()->AutoCalibrate();
}

void CDlgMotorControls::OnOK()
{
    const short cDanceVel = 150;
    const short cDancePwm = 255;

    for (int i = 0; i < 5; i++)
    {
        GetRobotWorldView()->GetActiveRobot()->AddVelocitiesToInstructions(cDanceVel, cDanceVel, 1000);
        GetRobotWorldView()->GetActiveRobot()->AddVelocitiesToInstructions(-cDanceVel, -cDanceVel, 1000);
        //		GetRobotWorldView()->GetActiveRobot()->AddVelocitiesToInstructions(0, cDanceVel, 1000);
        //		GetRobotWorldView()->GetActiveRobot()->AddVelocitiesToInstructions(0, -cDanceVel, 1000);
    }

    //	for (i = 0; i < 5; i++) {
    //		GetRobotWorldView()->GetActiveRobot()->AddPwmToInstructions(cDancePwm, cDancePwm, 1000);
    //		GetRobotWorldView()->GetActiveRobot()->AddPwmToInstructions(-cDancePwm, -cDancePwm, 1000);
    //	}
    GetRobotWorldView()->GetActiveRobot()->FollowInstructions();
    return;

    //	CDialog::OnOK();
}

void CDlgMotorControls::OnButSetVelocityDirection()
{
    UpdateData(TRUE);
    GetRobotWorldView()->GetActiveRobot()->SetVelocityDirection((int)atof(LPCTSTR(m_edtVelocityDirectionVelocity)), (int)atof(LPCTSTR(m_edtVelocityDirectionHeading)));
}

void CDlgMotorControls::OnButCreateCalWall()
{
    UpdateData(TRUE);
    robPOINT p1, p2;
    double h1, h2;
    p1.x = atof(m_edtStartX);
    p1.y = atof(m_edtStartY);
    p2.x = atof(m_edtEndX);
    p2.y = atof(m_edtEndY);
    h1 = atoi(m_edtIterations);
    h2 = atoi(m_edtRepeat);
    CWall* CalZone = new CWall(p1, h1);
    GetRobotWorldView()->m_ProbabilityGrid->m_ExecutableObjectList->Add(CalZone);
    GetRobotWorldView()->m_ProbabilityGrid->m_RWGraphicObjectList->Add(CalZone);
    CalZone->SetCaption("CheckPoint");
    GetRobotWorldView()->RedrawWindow();
}

void CDlgMotorControls::OnButSendValues()
{
    UpdateData(TRUE);
    GetRobotWorldView()->GetActiveRobot()->SetInformationToTransmit(m_chkSendVelocity == TRUE, m_chkSendAccel == TRUE, m_chkSendAD == TRUE, m_chkSendPosition == TRUE);
}

void CDlgMotorControls::OnButTestMotors()
{
    //	GetRobotWorldView()->GetActiveRobot()->FreeWheelTest();
}

void CDlgMotorControls::OnButOn()
{
    GetRobotWorldView()->GetActiveRobot()->SetBuzzerOn(true);
}

void CDlgMotorControls::OnButSetPathBuffer()
{
    UpdateData(TRUE);
    GetRobotWorldView()->GetActiveRobot()->SetFindPathBuffer(m_edtPathBuffer * cTPI);
}

void CDlgMotorControls::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    const int cLineInc = 10;
    CRect ViewRect;
    GetClientRect(ViewRect);
    int cPageInc = (ViewRect.Width() - 10);
    //	static UINT OldPos = 0;
    int Min, Max;

    switch (nSBCode)
    {
    case SB_LEFT:
        GetScrollRange(SB_HORZ, &Min, &Max);
        nPos = Min;
        break;

    case SB_ENDSCROLL:
        nPos = m_OldHScrollPos;
        break;

    case SB_LINELEFT:
        nPos = GetScrollPos(SB_HORZ);

        if (nPos <= cLineInc)
        {
            nPos = 0;
        }
        else
        {
            nPos -= cLineInc;
        }

        GetScrollRange(SB_HORZ, &Min, &Max);

        if (nPos < (UINT)Min)
        {
            nPos = Min;
        }

        break;

    case SB_LINERIGHT:
        nPos = GetScrollPos(SB_HORZ);
        nPos += cLineInc;
        GetScrollRange(SB_HORZ, &Min, &Max);

        if (nPos > (UINT)Max)
        {
            nPos = Max;
        }

        break;

    case SB_PAGELEFT:
        nPos = GetScrollPos(SB_HORZ);
        nPos -= cPageInc;
        GetScrollRange(SB_HORZ, &Min, &Max);

        if (nPos < (UINT)Min)
        {
            nPos = Min;
        }

        break;

    case SB_PAGERIGHT:
        nPos = GetScrollPos(SB_HORZ);
        nPos += cPageInc;
        GetScrollRange(SB_HORZ, &Min, &Max);

        if (nPos > (UINT)Max)
        {
            nPos = Max;
        }

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

    ScrollWindow((m_OldHScrollPos - nPos), 0, NULL, NULL);
    SetScrollPos(SB_HORZ, nPos, TRUE);
    UpdateWindow();
    m_OldHScrollPos = nPos;


    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgMotorControls::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    const int cLineInc = 10;
    CRect ViewRect;
    GetClientRect(ViewRect);
    int cPageInc = (ViewRect.Height() - 10);
    //	static UINT OldPos = 0;
    int Min, Max;

    switch (nSBCode)
    {
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

        if (nPos <= cLineInc)
        {
            nPos = 0;
        }
        else
        {
            nPos -= cLineInc;
        }

        GetScrollRange(SB_VERT, &Min, &Max);

        if (nPos < (UINT)Min)
        {
            nPos = Min;
        }

        break;

    case SB_LINERIGHT:
        nPos = GetScrollPos(SB_VERT);
        nPos += cLineInc;
        GetScrollRange(SB_VERT, &Min, &Max);

        if (nPos > (UINT)Max)
        {
            nPos = Max;
        }

        break;

    case SB_PAGELEFT:
        nPos = GetScrollPos(SB_VERT);
        nPos -= cPageInc;
        GetScrollRange(SB_VERT, &Min, &Max);

        if (nPos < (UINT)Min)
        {
            nPos = Min;
        }

        if (nPos > (UINT)Max)
        {
            nPos = Max;
        }

        break;

    case SB_PAGERIGHT:
        nPos = GetScrollPos(SB_VERT);
        nPos += cPageInc;
        GetScrollRange(SB_VERT, &Min, &Max);

        if (nPos > (UINT)Max)
        {
            nPos = Max;
        }

        if (nPos < (UINT)Min)
        {
            nPos = Min;
        }

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

    ScrollWindow(0, (m_OldVScrollPos - nPos), NULL, NULL);
    SetScrollPos(SB_VERT, nPos, TRUE);
    UpdateWindow();
    m_OldVScrollPos = nPos;

    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDlgMotorControls::OnInitDialog()
{
    CDialog::OnInitDialog();

    CRect ClientRect;
    GetClientRect(&ClientRect);
    m_OriginalSize.x = ClientRect.right;
    m_OriginalSize.y = ClientRect.bottom;
    SetScrollRange(SB_HORZ, 0, ClientRect.right);
    SetScrollRange(SB_VERT, 0, ClientRect.bottom);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMotorControls::OnButRequestAnalogInput()
{
    GetRobotWorldView()->GetActiveRobot()->SendRequestEX2();
}

void CDlgMotorControls::OnChkRobotAnimation()
{
    //	UpdateData(TRUE);
    //	GetRobotWorldView()->GetActiveRobot()->SetAnimateEnable(m_chkRobotAnimation == TRUE);
}
