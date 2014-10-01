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

#include "stdafx.h"
#include "cyesrv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// ICyeSrv properties

long ICyeSrv::GetResponseTimeout()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void ICyeSrv::SetResponseTimeout(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// ICyeSrv operations

CString ICyeSrv::ServerVersion()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long ICyeSrv::Connect(long wHnd, short NodeAddress, LPCTSTR ControlName)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		wHnd, NodeAddress, ControlName);
	return result;
}

short ICyeSrv::Disconnect(long ConnectId)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		ConnectId);
	return result;
}

long ICyeSrv::RegisterSimClient(long SimWHandle)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		SimWHandle);
	return result;
}

void ICyeSrv::SetMotorVelocities(short M0Velocity, short M1Velocity)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 M0Velocity, M1Velocity);
}

void ICyeSrv::SetRobotHeadingCorrectionFactor(short Correction)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Correction);
}

void ICyeSrv::FlipHandle(long Value)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Value);
}

void ICyeSrv::SetMaxPWM(short MaxPWM)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MaxPWM);
}

void ICyeSrv::StopMotors()
{
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICyeSrv::SetMotorsPWM(short M0PWM, short M1PWM)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 M0PWM, M1PWM);
}

void ICyeSrv::MoveTo(long X, long Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void ICyeSrv::MoveToRelative(long X, long Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void ICyeSrv::ResetPosition(long X, long Y, long H)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y, H);
}

void ICyeSrv::Stop()
{
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICyeSrv::Pause()
{
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICyeSrv::Resume()
{
	InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

short ICyeSrv::GetRadioStatus(short rNum)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		rNum);
	return result;
}

void ICyeSrv::SetRadioStatus(short rNum, short nNewValue)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 rNum, nNewValue);
}

short ICyeSrv::GetRobotStatus(short rNum)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		rNum);
	return result;
}

void ICyeSrv::SendP1(long P1)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 P1);
}

void ICyeSrv::SendP2(long P2)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 P2);
}

void ICyeSrv::SendP3(long P3)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 P3);
}

void ICyeSrv::SendP4(long P4)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 P4);
}

void ICyeSrv::SendBuzzerFrequency(short BuzzerFrequency)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 BuzzerFrequency);
}

void ICyeSrv::SendBuzzerOn(long BuzzerOn)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 BuzzerOn);
}

void ICyeSrv::SendFollowPath(short FollowPath)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FollowPath);
}

void ICyeSrv::SendHandleLength(short HandleLength)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 HandleLength);
}

void ICyeSrv::SendMotorControlConstants(short Kp, short Ki, short Kd, short Kb, short Kpwm)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Kp, Ki, Kd, Kb, Kpwm);
}

void ICyeSrv::SendTurn(double Heading, short Velocity, short Diameter)
{
	static BYTE parms[] =
		VTS_R8 VTS_I2 VTS_I2;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Heading, Velocity, Diameter);
}

void ICyeSrv::SendStraightVelocity(short Velocity, double Heading)
{
	static BYTE parms[] =
		VTS_I2 VTS_R8;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Velocity, Heading);
}

void ICyeSrv::SendMaxSpeed(short MaxSpeed)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MaxSpeed);
}

void ICyeSrv::SendHeading(double Heading)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Heading);
}

void ICyeSrv::SendMotorVelocities(short M0Velocity, short M1Velocity, short Priority)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 M0Velocity, M1Velocity, Priority);
}

void ICyeSrv::PurgeRingBuffer()
{
	InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICyeSrv::SendPosition(double X, double Y)
{
	static BYTE parms[] =
		VTS_R8 VTS_R8;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void ICyeSrv::SendPositionDestination(double X, double Y)
{
	static BYTE parms[] =
		VTS_R8 VTS_R8;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void ICyeSrv::ExecutePath(long PurgeOld, LPDISPATCH PathObject)
{
	static BYTE parms[] =
		VTS_I4 VTS_DISPATCH;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PurgeOld, PathObject);
}

void ICyeSrv::SetRobotStatus(short rNum, short nNewValue)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 rNum, nNewValue);
}

void ICyeSrv::SendSensitiveObstacleDetection(short Sensitive)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Sensitive);
}

void ICyeSrv::SendP5(long P5)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 P5);
}

void ICyeSrv::SendP6(long P6)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 P6);
}

void ICyeSrv::SendP7(long P7)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 P7);
}

void ICyeSrv::SendP8(long P8)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 P8);
}

void ICyeSrv::SendVelocityDirection(short Velocity, double Heading, short Priority)
{
	static BYTE parms[] =
		VTS_I2 VTS_R8 VTS_I2;
	InvokeHelper(0x2d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Velocity, Heading, Priority);
}

void ICyeSrv::SetRobotDestinationHeading(double Heading, short Velocity)
{
	static BYTE parms[] =
		VTS_R8 VTS_I2;
	InvokeHelper(0x2e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Heading, Velocity);
}

void ICyeSrv::SendPositionVelocityDestination(double X, double Y, short Velocity, short Priority)
{
	static BYTE parms[] =
		VTS_R8 VTS_R8 VTS_I2 VTS_I2;
	InvokeHelper(0x2f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y, Velocity, Priority);
}

void ICyeSrv::SendInformationToTransmit(long Mask)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x30, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Mask);
}

void ICyeSrv::SendRequestState()
{
	InvokeHelper(0x31, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICyeSrv::PowerDown()
{
	InvokeHelper(0x32, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICyeSrv::SendMessage(const VARIANT& MsgData)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x33, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &MsgData);
}

long ICyeSrv::GetRetryLimit()
{
	long result;
	InvokeHelper(0x34, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void ICyeSrv::SetRetryLimit(long NewLimit)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x35, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 NewLimit);
}

CString ICyeSrv::GetPort()
{
	CString result;
	InvokeHelper(0x36, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void ICyeSrv::SetPort(LPCTSTR NewPort)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 NewPort);
}

void ICyeSrv::SquareToWall(double Heading, float TravelTime, long SlipperySurface)
{
	static BYTE parms[] =
		VTS_R8 VTS_R4 VTS_I4;
	InvokeHelper(0x38, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Heading, TravelTime, SlipperySurface);
}

void ICyeSrv::SetFeelingMotor(short Motor)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Motor);
}

void ICyeSrv::CornerCalibrate(long X, long Y, double HO, short FeelWheel, float TravelTime, long SlipperySurface)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R8 VTS_I2 VTS_R4 VTS_I4;
	InvokeHelper(0x3a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y, HO, FeelWheel, TravelTime, SlipperySurface);
}

void ICyeSrv::FindHomeBase(double CornerHeading, double WallHeading, long HomeBaseLocationX, long HomeBaseLocationY, long NeedToGetInFrontOfHomeBase)
{
	static BYTE parms[] =
		VTS_R8 VTS_R8 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x3b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CornerHeading, WallHeading, HomeBaseLocationX, HomeBaseLocationY, NeedToGetInFrontOfHomeBase);
}

void ICyeSrv::SetHeadingDestination(double Heading, short Speed, short MotorEnable)
{
	static BYTE parms[] =
		VTS_R8 VTS_I2 VTS_I2;
	InvokeHelper(0x3c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Heading, Speed, MotorEnable);
}

void ICyeSrv::SetEnableObstacleBounce(long Enable)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Enable);
}

void ICyeSrv::SetRAMData(long Address, const VARIANT& Data)
{
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x3e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Address, &Data);
}

void ICyeSrv::SetRobotOnSeconds(long Seconds)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Seconds);
}

void ICyeSrv::SetIntermittentBeep(long On)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x40, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 On);
}

void ICyeSrv::AddToPosition(double DeltaX, double DeltaY)
{
	static BYTE parms[] =
		VTS_R8 VTS_R8;
	InvokeHelper(0x41, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 DeltaX, DeltaY);
}

void ICyeSrv::AddToHeading(double Delta)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x42, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Delta);
}

void ICyeSrv::SetExternalOutput(long On)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x43, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 On);
}

void ICyeSrv::RequestEX2Value()
{
	InvokeHelper(0x44, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
