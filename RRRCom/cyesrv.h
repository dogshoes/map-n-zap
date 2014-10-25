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
// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// ICyeSrv wrapper class

class ICyeSrv : public COleDispatchDriver
{
    public:
        ICyeSrv() {}		// Calls COleDispatchDriver default constructor
        ICyeSrv(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
        ICyeSrv(const ICyeSrv& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

        // Attributes
    public:
        long GetResponseTimeout();
        void SetResponseTimeout(long);

        // Operations
    public:
        CString ServerVersion();
        long Connect(long wHnd, short NodeAddress, LPCTSTR ControlName);
        short Disconnect(long ConnectId);
        long RegisterSimClient(long SimWHandle);
        void SetMotorVelocities(short M0Velocity, short M1Velocity);
        void SetRobotHeadingCorrectionFactor(short Correction);
        void FlipHandle(long Value);
        void SetMaxPWM(short MaxPWM);
        void StopMotors();
        void SetMotorsPWM(short M0PWM, short M1PWM);
        void MoveTo(long X, long Y);
        void MoveToRelative(long X, long Y);
        void ResetPosition(long X, long Y, long H);
        void Stop();
        void Pause();
        void Resume();
        short GetRadioStatus(short rNum);
        void SetRadioStatus(short rNum, short nNewValue);
        short GetRobotStatus(short rNum);
        void SendP1(long P1);
        void SendP2(long P2);
        void SendP3(long P3);
        void SendP4(long P4);
        void SendBuzzerFrequency(short BuzzerFrequency);
        void SendBuzzerOn(long BuzzerOn);
        void SendFollowPath(short FollowPath);
        void SendHandleLength(short HandleLength);
        void SendMotorControlConstants(short Kp, short Ki, short Kd, short Kb, short Kpwm);
        void SendTurn(double Heading, short Velocity, short Diameter);
        void SendStraightVelocity(short Velocity, double Heading);
        void SendMaxSpeed(short MaxSpeed);
        void SendHeading(double Heading);
        void SendMotorVelocities(short M0Velocity, short M1Velocity, short Priority);
        void PurgeRingBuffer();
        void SendPosition(double X, double Y);
        void SendPositionDestination(double X, double Y);
        void ExecutePath(long PurgeOld, LPDISPATCH PathObject);
        void SetRobotStatus(short rNum, short nNewValue);
        void SendSensitiveObstacleDetection(short Sensitive);
        void SendP5(long P5);
        void SendP6(long P6);
        void SendP7(long P7);
        void SendP8(long P8);
        void SendVelocityDirection(short Velocity, double Heading, short Priority);
        void SetRobotDestinationHeading(double Heading, short Velocity);
        void SendPositionVelocityDestination(double X, double Y, short Velocity, short Priority);
        void SendInformationToTransmit(long Mask);
        void SendRequestState();
        void PowerDown();
        void SendMessage(const VARIANT& MsgData);
        long GetRetryLimit();
        void SetRetryLimit(long NewLimit);
        CString GetPort();
        void SetPort(LPCTSTR NewPort);
        void SquareToWall(double Heading, float TravelTime, long SlipperySurface);
        void SetFeelingMotor(short Motor);
        void CornerCalibrate(long X, long Y, double HO, short FeelWheel, float TravelTime, long SlipperySurface);
        void FindHomeBase(double CornerHeading, double WallHeading, long HomeBaseLocationX, long HomeBaseLocationY, long NeedToGetInFrontOfHomeBase);
        void SetHeadingDestination(double Heading, short Speed, short MotorEnable);
        void SetEnableObstacleBounce(long Enable);
        void SetRAMData(long Address, const VARIANT& Data);
        void SetRobotOnSeconds(long Seconds);
        void SetIntermittentBeep(long On);
        void AddToPosition(double DeltaX, double DeltaY);
        void AddToHeading(double Delta);
        void SetExternalOutput(long On);
        void RequestEX2Value();
};
