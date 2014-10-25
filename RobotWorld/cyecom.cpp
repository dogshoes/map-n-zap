
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
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "cyecom.h"

/////////////////////////////////////////////////////////////////////////////
// CCyeCom

IMPLEMENT_DYNCREATE(CCyeCom, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CCyeCom properties

CString CCyeCom::GetVersion()
{
    CString result;
    GetProperty(0x64, VT_BSTR, (void*)&result);
    return result;
}

void CCyeCom::SetVersion(LPCTSTR propVal)
{
    SetProperty(0x64, VT_BSTR, propVal);
}

long CCyeCom::GetX()
{
    long result;
    GetProperty(0x65, VT_I4, (void*)&result);
    return result;
}

void CCyeCom::SetX(long propVal)
{
    SetProperty(0x65, VT_I4, propVal);
}

long CCyeCom::GetY()
{
    long result;
    GetProperty(0x66, VT_I4, (void*)&result);
    return result;
}

void CCyeCom::SetY(long propVal)
{
    SetProperty(0x66, VT_I4, propVal);
}

double CCyeCom::GetHeading()
{
    double result;
    GetProperty(0x67, VT_R8, (void*)&result);
    return result;
}

void CCyeCom::SetHeading(double propVal)
{
    SetProperty(0x67, VT_R8, propVal);
}

short CCyeCom::GetRobotAddress()
{
    short result;
    GetProperty(0x68, VT_I2, (void*)&result);
    return result;
}

void CCyeCom::SetRobotAddress(short propVal)
{
    SetProperty(0x68, VT_I2, propVal);
}

float CCyeCom::GetRadioLevel()
{
    float result;
    GetProperty(0x6a, VT_R4, (void*)&result);
    return result;
}

void CCyeCom::SetRadioLevel(float propVal)
{
    SetProperty(0x6a, VT_R4, propVal);
}

float CCyeCom::GetBatteryLevel()
{
    float result;
    GetProperty(0x6b, VT_R4, (void*)&result);
    return result;
}

void CCyeCom::SetBatteryLevel(float propVal)
{
    SetProperty(0x6b, VT_R4, propVal);
}

long CCyeCom::GetResponseTimeout()
{
    long result;
    GetProperty(0x6c, VT_I4, (void*)&result);
    return result;
}

void CCyeCom::SetResponseTimeout(long propVal)
{
    SetProperty(0x6c, VT_I4, propVal);
}

long CCyeCom::GetRetryLimit()
{
    long result;
    GetProperty(0x99, VT_I4, (void*)&result);
    return result;
}

void CCyeCom::SetRetryLimit(long propVal)
{
    SetProperty(0x99, VT_I4, propVal);
}

short CCyeCom::GetCommState()
{
    short result;
    GetProperty(0x9c, VT_I2, (void*)&result);
    return result;
}

void CCyeCom::SetCommState(short propVal)
{
    SetProperty(0x9c, VT_I2, propVal);
}

float CCyeCom::GetRightCurrent()
{
    float result;
    GetProperty(0xaf, VT_R4, (void*)&result);
    return result;
}

void CCyeCom::SetRightCurrent(float propVal)
{
    SetProperty(0xaf, VT_R4, propVal);
}

float CCyeCom::GetLeftCurrent()
{
    float result;
    GetProperty(0xb0, VT_R4, (void*)&result);
    return result;
}

void CCyeCom::SetLeftCurrent(float propVal)
{
    SetProperty(0xb0, VT_R4, propVal);
}

short CCyeCom::GetLeftVelocity()
{
    short result;
    GetProperty(0xb1, VT_I2, (void*)&result);
    return result;
}

void CCyeCom::SetLeftVelocity(short propVal)
{
    SetProperty(0xb1, VT_I2, propVal);
}

short CCyeCom::GetRightVelocity()
{
    short result;
    GetProperty(0xb2, VT_I2, (void*)&result);
    return result;
}

void CCyeCom::SetRightVelocity(short propVal)
{
    SetProperty(0xb2, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CCyeCom operations

void CCyeCom::SetHeadingCorrectionFactor(short HeadingCorrectionFactor)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x6e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 HeadingCorrectionFactor);
}

void CCyeCom::SetFlipHandle(long FlipHandle)
{
    static BYTE parms[] =
        VTS_I4;
    InvokeHelper(0x6f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 FlipHandle);
}

void CCyeCom::TurnTo(double Heading, short Speed)
{
    static BYTE parms[] =
        VTS_R8 VTS_I2;
    InvokeHelper(0x71, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Heading, Speed);
}

void CCyeCom::SetPWM(short RightPWM, short LeftPWM)
{
    static BYTE parms[] =
        VTS_I2 VTS_I2;
    InvokeHelper(0x73, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 RightPWM, LeftPWM);
}

void CCyeCom::Stop()
{
    InvokeHelper(0x77, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CCyeCom::Pause()
{
    InvokeHelper(0x78, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CCyeCom::Resume()
{
    InvokeHelper(0x79, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CCyeCom::SetBuzzerFrequency(short BuzzerFrequency)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x80, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 BuzzerFrequency);
}

void CCyeCom::SetBuzzerOn(long BuzzerOn)
{
    static BYTE parms[] =
        VTS_I4;
    InvokeHelper(0x81, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 BuzzerOn);
}

void CCyeCom::FollowPath(short Speed)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x82, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Speed);
}

void CCyeCom::SetHandleLength(short HandleLength)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x83, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 HandleLength);
}

void CCyeCom::SetMotorControlConstants(short Kp, short Ki, short Kd, short Kb, short Kpwm)
{
    static BYTE parms[] =
        VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2;
    InvokeHelper(0x84, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Kp, Ki, Kd, Kb, Kpwm);
}

void CCyeCom::MoveCurve(double Heading, short Speed, short Diameter)
{
    static BYTE parms[] =
        VTS_R8 VTS_I2 VTS_I2;
    InvokeHelper(0x85, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Heading, Speed, Diameter);
}

void CCyeCom::MoveStraight(short Speed, double Heading)
{
    static BYTE parms[] =
        VTS_I2 VTS_R8;
    InvokeHelper(0x86, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Speed, Heading);
}

void CCyeCom::SetMaxSpeed(short MaxSpeed)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x87, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 MaxSpeed);
}

void CCyeCom::SetWheelVelocities(short RightVelocity, short LeftVelocity)
{
    static BYTE parms[] =
        VTS_I2 VTS_I2;
    InvokeHelper(0x6d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 RightVelocity, LeftVelocity);
}

void CCyeCom::SetObstacleSensitivity(short Sensitivity)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x8b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Sensitivity);
}

void CCyeCom::MoveTo(double X, double Y, short Speed)
{
    static BYTE parms[] =
        VTS_R8 VTS_R8 VTS_I2;
    InvokeHelper(0x8c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 X, Y, Speed);
}

void CCyeCom::SetPosition(long X, long Y)
{
    static BYTE parms[] =
        VTS_I4 VTS_I4;
    InvokeHelper(0x8d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 X, Y);
}

void CCyeCom::LoadPathSegment(long PurgeOld, LPDISPATCH PathObject)
{
    static BYTE parms[] =
        VTS_I4 VTS_DISPATCH;
    InvokeHelper(0x8f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 PurgeOld, PathObject);
}

void CCyeCom::MoveDirection(short Speed, double Heading)
{
    static BYTE parms[] =
        VTS_I2 VTS_R8;
    InvokeHelper(0x94, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Speed, Heading);
}

void CCyeCom::RequestState()
{
    InvokeHelper(0x96, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CCyeCom::PowerDown()
{
    InvokeHelper(0x97, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CCyeCom::SendMessage(const VARIANT& MsgData)
{
    static BYTE parms[] =
        VTS_VARIANT;
    InvokeHelper(0x98, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 &MsgData);
}

CString CCyeCom::GetCommPort()
{
    CString result;
    InvokeHelper(0x9a, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
    return result;
}

void CCyeCom::SetCommPort(LPCTSTR NewPort)
{
    static BYTE parms[] =
        VTS_BSTR;
    InvokeHelper(0x9b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 NewPort);
}

void CCyeCom::SquareToWall(double Heading, float TravelTime, long SlipperySurface)
{
    static BYTE parms[] =
        VTS_R8 VTS_R4 VTS_I4;
    InvokeHelper(0xb8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Heading, TravelTime, SlipperySurface);
}

void CCyeCom::SetFeelingMotor(short Motor)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0xb9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Motor);
}

void CCyeCom::CornerCalibrate(long X, long Y, double HO, short FeelWheel, float TravelTime, long SlipperySurface)
{
    static BYTE parms[] =
        VTS_I4 VTS_I4 VTS_R8 VTS_I2 VTS_R4 VTS_I4;
    InvokeHelper(0xba, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 X, Y, HO, FeelWheel, TravelTime, SlipperySurface);
}

void CCyeCom::FindHomeBase(double CornerHeading, double WallHeading, long HomeBaseLocationX, long HomeBaseLocationY, long NeedToGetInFrontOfHomeBase)
{
    static BYTE parms[] =
        VTS_R8 VTS_R8 VTS_I4 VTS_I4 VTS_I4;
    InvokeHelper(0xbb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 CornerHeading, WallHeading, HomeBaseLocationX, HomeBaseLocationY, NeedToGetInFrontOfHomeBase);
}

void CCyeCom::SetHeadingDestination(double Heading, short Speed, short MotorEnable)
{
    static BYTE parms[] =
        VTS_R8 VTS_I2 VTS_I2;
    InvokeHelper(0xbc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Heading, Speed, MotorEnable);
}

void CCyeCom::SetEnableObstacleBounce(long Enable)
{
    static BYTE parms[] =
        VTS_I4;
    InvokeHelper(0xbd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Enable);
}

void CCyeCom::SetRAMData(long Address, const VARIANT& Data)
{
    static BYTE parms[] =
        VTS_I4 VTS_VARIANT;
    InvokeHelper(0xbe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Address, &Data);
}

void CCyeCom::SetRobotOnSeconds(long Seconds)
{
    static BYTE parms[] =
        VTS_I4;
    InvokeHelper(0xbf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Seconds);
}

void CCyeCom::SetIntermittentBeep(long On)
{
    static BYTE parms[] =
        VTS_I4;
    InvokeHelper(0xc0, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 On);
}

void CCyeCom::AddToPosition(double DeltaX, double DeltaY)
{
    static BYTE parms[] =
        VTS_R8 VTS_R8;
    InvokeHelper(0xc1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 DeltaX, DeltaY);
}

void CCyeCom::AddToHeading(double Delta)
{
    static BYTE parms[] =
        VTS_R8;
    InvokeHelper(0xc2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Delta);
}

void CCyeCom::SetExternalOutput(long On)
{
    static BYTE parms[] =
        VTS_I4;
    InvokeHelper(0xc3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 On);
}

void CCyeCom::RequestExternalInput()
{
    InvokeHelper(0xc4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CCyeCom::AboutBox()
{
    InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// ICyePath properties

short ICyePath::GetNumPoints()
{
    short result;
    GetProperty(0x64, VT_I2, (void*)&result);
    return result;
}

void ICyePath::SetNumPoints(short propVal)
{
    SetProperty(0x64, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// ICyePath operations

double ICyePath::GetX(short Index)
{
    double result;
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms,
                 Index);
    return result;
}

void ICyePath::SetX(short Index, double newValue)
{
    static BYTE parms[] =
        VTS_I2 VTS_R8;
    InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
                 Index, newValue);
}

double ICyePath::GetY(short Index)
{
    double result;
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms,
                 Index);
    return result;
}

void ICyePath::SetY(short Index, double newValue)
{
    static BYTE parms[] =
        VTS_I2 VTS_R8;
    InvokeHelper(0x66, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
                 Index, newValue);
}
