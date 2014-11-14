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
// RRRSrvDoc.h : interface of the CRRRSrvDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include "RobotComm.h"

class CRRRSrvDoc : public CDocument
{
    protected: // create from serialization only
        CRRRSrvDoc();
        DECLARE_DYNCREATE(CRRRSrvDoc)

        // Attributes
    public:

        // Operations
    public:

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CRRRSrvDoc)
    public:
        virtual BOOL OnNewDocument();
        virtual void Serialize(CArchive& ar);
        //}}AFX_VIRTUAL

        // Implementation
    public:
        virtual ~CRRRSrvDoc();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

    protected:
        RRRSrvCom *rcom;                    // pointer to serial interface class
        CRobotComm RobotCom;                // robot communications message framing class
        short RobotAddress;                 // address of target robot for this class instance

        // Generated message map functions
    protected:
        //{{AFX_MSG(CRRRSrvDoc)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

        // Generated OLE dispatch map functions
        //{{AFX_DISPATCH(CRRRSrvDoc)
        afx_msg long GetResponseTimeout();
        afx_msg void SetResponseTimeout(long nNewValue);
        afx_msg BSTR ServerVersion();
        afx_msg long Connect(long wHnd, short NodeAddress, LPCTSTR ControlName);
        afx_msg short Disconnect(long ConnectId);
        afx_msg long RegisterSimClient(long SimWHandle);
        afx_msg void SetMotorVelocities(short M0Velocity, short M1Velocity);
        afx_msg void SetRobotHeadingCorrectionFactor(short Correction);
        afx_msg void FlipHandle(BOOL Value);
        afx_msg void SetMaxPWM(short MaxPWM);
        afx_msg void StopMotors();
        afx_msg void SetMotorsPWM(short M0PWM, short M1PWM);
        afx_msg void MoveTo(long X, long Y);
        afx_msg void MoveToRelative(long X, long Y);
        afx_msg void ResetPosition(long X, long Y, long H);
        afx_msg void Stop();
        afx_msg void Pause();
        afx_msg void Resume();
        afx_msg short GetRadioStatus(short rNum);
        afx_msg void SetRadioStatus(short rNum, short nNewValue);
        afx_msg short GetRobotStatus(short rNum);
        afx_msg void SendP1(long P1);
        afx_msg void SendP2(long P2);
        afx_msg void SendP3(long P3);
        afx_msg void SendP4(long P4);
        afx_msg void SendBuzzerFrequency(short BuzzerFrequency);
        afx_msg void SendBuzzerOn(BOOL BuzzerOn);
        afx_msg void SendFollowPath(short FollowPath);
        afx_msg void SendHandleLength(short HandleLength);
        afx_msg void SendMotorControlConstants(short Kp, short Ki, short Kd, short Kb, short Kpwm);
        afx_msg void SendTurn(double Heading, short Velocity, short Diameter);
        afx_msg void SendStraightVelocity(short Velocity, double Heading);
        afx_msg void SendMaxSpeed(short MaxSpeed);
        afx_msg void SendHeading(double Heading);
        afx_msg void SendMotorVelocities(short M0Velocity, short M1Velocity, short Priority);
        afx_msg void PurgeRingBuffer();
        afx_msg void SendPosition(double X, double Y);
        afx_msg void SendPositionDestination(double X, double Y);
        afx_msg void ExecutePath(BOOL PurgeOld, LPDISPATCH PathObject);
        afx_msg void SetRobotStatus(short rNum, short nNewValue);
        afx_msg void SendSensitiveObstacleDetection(short Sensitive);
        afx_msg void SendP5(long P5);
        afx_msg void SendP6(long P6);
        afx_msg void SendP7(long P7);
        afx_msg void SendP8(long P8);
        afx_msg void SendVelocityDirection(short Velocity, double Heading, short Priority);
        afx_msg void SetRobotDestinationHeading(double Heading, short Velocity);
        afx_msg void SendPositionVelocityDestination(double X, double Y, short Velocity, short Priority);
        afx_msg void SendInformationToTransmit(long Mask);
        afx_msg void SendRequestState();
        afx_msg void PowerDown();
        afx_msg void SendMessage(const VARIANT FAR& MsgData);
        afx_msg long GetRetryLimit();
        afx_msg void SetRetryLimit(long NewLimit);
        afx_msg BSTR GetPort();
        afx_msg void SetPort(LPCTSTR NewPort);
        afx_msg void SquareToWall(double Heading, float TravelTime, BOOL SlipperySurface);
        afx_msg void SetFeelingMotor(short Motor);
        afx_msg void CornerCalibrate(long X, long Y, double HO, short FeelWheel, float TravelTime, BOOL SlipperySurface);
        afx_msg void FindHomeBase(double CornerHeading, double WallHeading, long HomeBaseLocationX, long HomeBaseLocationY, BOOL NeedToGetInFrontOfHomeBase);
        afx_msg void SetHeadingDestination(double Heading, short Speed, short MotorEnable);
        afx_msg void SetEnableObstacleBounce(BOOL Enable);
        afx_msg void SetRAMData(long Address, const VARIANT FAR& Data);
        afx_msg void SetRobotOnSeconds(long Seconds);
        afx_msg void SetIntermittentBeep(BOOL On);
        afx_msg void AddToPosition(double DeltaX, double DeltaY);
        afx_msg void AddToHeading(double Delta);
        afx_msg void SetExternalOutput(BOOL On);
        afx_msg void RequestEX2Value();
        //}}AFX_DISPATCH
        DECLARE_DISPATCH_MAP()
        DECLARE_INTERFACE_MAP()
};