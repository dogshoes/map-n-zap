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
// Module:     RobotCom.h
//
// Abstract:   Header file for RobotCom class.
//
// Author:     J.C.H.
//
// Revisions:
//    $History: RobotCom.h $
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

//{{AFX_INCLUDES()
#include "cyecom.h"
//}}AFX_INCLUDES
#if !defined(AFX_ROBOTCOM_H__D9770B42_8033_11D1_BCC1_444553540000__INCLUDED_)
#define AFX_ROBOTCOM_H__D9770B42_8033_11D1_BCC1_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RobotCom.h : header file
//

#include "RothMath.h"
#include "rrdim.h"

class CRobotWorldView;
class CRobot;
/////////////////////////////////////////////////////////////////////////////
// RobotCom dialog

class RobotCom : public CDialog
{
        // Construction
    public:
        void SetResponseTimeout(long TimeOut);
        void SetEnableClapDetectionWhileMoving(bool Enable);
        void SetEnableClapDetection(bool Enable);
        void SetEnableDancing(bool Enable);
        void SetRobotAddress(short Address);
        void SendCornerCalibrate(robPOINT P0, double H0, unsigned char FeelWheel, float TravelTime, bool SlipperySurface, short WheelDistanceFromWall = 55);
        void SendPing(void);
        void SendTune(short *Frequency, int *msDuration, unsigned char NumberOfNotes, bool PurgeBuffer, bool StartPlaying);
        void SquareToWall(double Heading, float TravelTime, bool SlipperySurface);
        void SendRequestEX2(void);
        bool SearchForRobot(CDialog& Status);
        void SetExternalOutput(bool On);
        void InitialSearchForRobot(void);
        void AddToHeading(double Delta);
        void AddToPosition(robPOINT Delta);
        void SendIntermittentBeep(bool On);
        void SendRobotOnSeconds(long Seconds);
        void SendRAMData(unsigned short Address, unsigned char NumberBytes, unsigned char *Data);
        void SendEnableObstacleBounce(bool Enable);
        void SendHeadingDestination(double Heading, short Speed, short MotorEnable);
        float GetRadioLevel(void);
        void SendPause(bool p);
        long GetRetryLimit(void);
        void SetRetryLimit(long retry);
        void PowerDown(void);
        void SendFindHomeBase(double CornerHeading, double WallHeading, robPOINT HomeBaseLocation, bool NeedToGetInFrontOfHomeBase = true);
        void SendCornerCalibrate(robPOINT P0, double H0, unsigned char FeelWheel, float TravelTime, bool SlipperySurface);
        void SendFeelingMotor(unsigned char Motor);
        //	void SendWallCalibrate(robPOINT P0, double H0, robPOINT P1, double H1, bool Corner, unsigned char FeelWheel);
        void SendString(CString String);
        void SendRequestState(void);
        void SendInformationToTransmit(long mask);
        void SendVelocityDirection(int Velocity, double Heading);
        RobotCom(CRobotWorldView* pParent, CRobot *Robot);   // standard constructor

        void SendP4(long P4);
        void SendP3(long P3);
        void SendP2(long P2);
        void SendP1(long P1);
        void SendP8(long P);
        void SendP7(long P);
        void SendP6(long P);
        void SendP5(long P);
        void SendBuzzerFrequency(short BuzzerFrequency);
        void SendBuzzerOn(BOOL On);
        void SendPathSegment(BOOL PurgePath, robPOINT* Points, unsigned char nPoints);
        void SendFollowPath(int FollowPath);
        void SendHandleLength(short HandleLength);
        void SendMotorControlConstants(char Kp, char Ki, char Kd, char Kb, char KPwm);
        void SendTurn(double Heading, short Velocity, short Diameter);
        void SendObstacleSensitivity(TObstacleSensitivity Sensitive);
        void SendStraightVelocity(int Velocity, double Heading);
        void SendPositionVelocityDestination(robPOINT Destination, short Velocity);
        void SendMaxSpeed(short MaxSpeed);
        void SendHeading(double Heading);
        void SendPosition(robPOINT Position);
        void SendMotorVelocities(int M0Velocity, int M1Velocity);
        void SendHeadingCorrectionFactor(int Correction);
        void SendFlipHandle(BOOL value);
        void SendMaxPWM(unsigned char MaxPWM);
        void SendHeadingDestination(double Heading, short Velocity);
        void SendStopMotors(void);
        void SendPurgeRingBuffer(void);
        void SendMotorsPWM(int M0PWM, int M1PWM);
        void SendPositionDestination(robPOINT Destination);

        // Dialog Data
        //{{AFX_DATA(RobotCom)
        enum { IDD = IDD_ROBOTCOM };
        CCyeCom	m_CyeCom;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(RobotCom)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        bool m_CommunicationsEstablished;

        CRobot *m_Robot;

        // Generated message map functions
        //{{AFX_MSG(RobotCom)
        afx_msg void OnRadioLevelChangeRrrcomctrl1(float RadioLevel);
        afx_msg void OnBatteryLowRrrcomctrl1();
        afx_msg void OnObstacleDetectedRrrcomctrl1(short RightWheel, short LeftWheel);
        afx_msg void OnNudgeDetectedRrrcomctrl1(long RightWheel, long LeftWheel);
        afx_msg void OnNewPathSegmentRequestRrrcomctrl1();
        afx_msg void OnRobotErrorRrrcomctrl1(long Error);
        afx_msg void OnOnHomeBaseRrrcomctrl1(short OnHomeBase);
        afx_msg void OnBatteryChargingStateChangeRrrcomctrl1(short State);
        afx_msg void OnCommStateChangeRrrcomctrl1(short NewState);
        afx_msg void OnTaskResultRrrcomctrl1(short Result, long X, long Y, double Heading);
        afx_msg void OnSoftwareRevNumberRrrcomctrl1(long RevNumber);
        afx_msg void OnRobotOnRrrcomctrl1();
        afx_msg void OnRobotOnTimeRrrcomctrl1(float SecondsOn);
        afx_msg void OnExternalInputRrrcomctrl1(float Voltage);
        afx_msg void OnRobotStatusChangeRrrcomctrl1(long X, long Y, double Heading, short RightVelocity, short LeftVelocity, float RightCurrent, float LeftCurrent, float BatteryVoltage);
        afx_msg void OnUnknownMessageRrrcomctrl1(const VARIANT FAR& Buffer);
        afx_msg void OnMicrophoneVolumeCyecomctrl1(short Volume);
        DECLARE_EVENTSINK_MAP()
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    private:
        bool m_FirstCommunicationsEstablished;
        void WaitForCommunicationsEstablished(int seconds);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTCOM_H__D9770B42_8033_11D1_BCC1_444553540000__INCLUDED_)
