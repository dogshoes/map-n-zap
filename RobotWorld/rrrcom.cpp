// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "rrrcom.h"

/////////////////////////////////////////////////////////////////////////////
// CRRRCom

IMPLEMENT_DYNCREATE(CRRRCom, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CRRRCom properties

CString CRRRCom::GetVersion()
{
    CString result;
    GetProperty(0x64, VT_BSTR, (void*)&result);
    return result;
}

void CRRRCom::SetVersion(LPCTSTR propVal)
{
    SetProperty(0x64, VT_BSTR, propVal);
}

long CRRRCom::GetX()
{
    long result;
    GetProperty(0x65, VT_I4, (void*)&result);
    return result;
}

void CRRRCom::SetX(long propVal)
{
    SetProperty(0x65, VT_I4, propVal);
}

long CRRRCom::GetY()
{
    long result;
    GetProperty(0x66, VT_I4, (void*)&result);
    return result;
}

void CRRRCom::SetY(long propVal)
{
    SetProperty(0x66, VT_I4, propVal);
}

long CRRRCom::GetHeading()
{
    long result;
    GetProperty(0x67, VT_I4, (void*)&result);
    return result;
}

void CRRRCom::SetHeading(long propVal)
{
    SetProperty(0x67, VT_I4, propVal);
}

short CRRRCom::GetNumRobots()
{
    short result;
    GetProperty(0x68, VT_I2, (void*)&result);
    return result;
}

void CRRRCom::SetNumRobots(short propVal)
{
    SetProperty(0x68, VT_I2, propVal);
}

float CRRRCom::GetVelocity()
{
    float result;
    GetProperty(0x69, VT_R4, (void*)&result);
    return result;
}

void CRRRCom::SetVelocity(float propVal)
{
    SetProperty(0x69, VT_R4, propVal);
}

float CRRRCom::GetRadioLevel()
{
    float result;
    GetProperty(0x6a, VT_R4, (void*)&result);
    return result;
}

void CRRRCom::SetRadioLevel(float propVal)
{
    SetProperty(0x6a, VT_R4, propVal);
}

float CRRRCom::GetBatteryLevel()
{
    float result;
    GetProperty(0x6b, VT_R4, (void*)&result);
    return result;
}

void CRRRCom::SetBatteryLevel(float propVal)
{
    SetProperty(0x6b, VT_R4, propVal);
}

long CRRRCom::GetResponseTimeout()
{
    long result;
    GetProperty(0x6c, VT_I4, (void*)&result);
    return result;
}

void CRRRCom::SetResponseTimeout(long propVal)
{
    SetProperty(0x6c, VT_I4, propVal);
}

long CRRRCom::GetRetryLimit()
{
    long result;
    GetProperty(0x99, VT_I4, (void*)&result);
    return result;
}

void CRRRCom::SetRetryLimit(long propVal)
{
    SetProperty(0x99, VT_I4, propVal);
}

short CRRRCom::GetCommState()
{
    short result;
    GetProperty(0x9c, VT_I2, (void*)&result);
    return result;
}

void CRRRCom::SetCommState(short propVal)
{
    SetProperty(0x9c, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CRRRCom operations

void CRRRCom::SetWheelVelocities(short RightWheel, short LeftWheel)
{
    static BYTE parms[] =
        VTS_I2 VTS_I2;
    InvokeHelper(0x6d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 RightWheel, LeftWheel);
}

void CRRRCom::SetHeadingCorrectionFactor(short HeadingCorrectionFactor)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x6e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 HeadingCorrectionFactor);
}

void CRRRCom::SetFlipHandle(long FlipHandle)
{
    static BYTE parms[] =
        VTS_I4;
    InvokeHelper(0x6f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 FlipHandle);
}

void CRRRCom::TurnTo(double Heading, short Speed)
{
    static BYTE parms[] =
        VTS_R8 VTS_I2;
    InvokeHelper(0x71, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Heading, Speed);
}

void CRRRCom::StopMotors()
{
    InvokeHelper(0x72, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CRRRCom::SetPWM(short RightPWM, short LeftPWM)
{
    static BYTE parms[] =
        VTS_I2 VTS_I2;
    InvokeHelper(0x73, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 RightPWM, LeftPWM);
}

void CRRRCom::Stop()
{
    InvokeHelper(0x77, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CRRRCom::Pause()
{
    InvokeHelper(0x78, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CRRRCom::Resume()
{
    InvokeHelper(0x79, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CRRRCom::SetBuzzerFrequency(short BuzzerFrequency)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x80, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 BuzzerFrequency);
}

void CRRRCom::SetBuzzerOn(long BuzzerOn)
{
    static BYTE parms[] =
        VTS_I4;
    InvokeHelper(0x81, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 BuzzerOn);
}

void CRRRCom::FollowPath(short Speed)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x82, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Speed);
}

void CRRRCom::SetHandleLength(short HandleLength)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x83, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 HandleLength);
}

void CRRRCom::SetMotorControlConstants(short Kp, short Ki, short Kd, short Kb, short Kpwm)
{
    static BYTE parms[] =
        VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2;
    InvokeHelper(0x84, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Kp, Ki, Kd, Kb, Kpwm);
}

void CRRRCom::MoveCurve(double Heading, short Speed, short Diameter)
{
    static BYTE parms[] =
        VTS_R8 VTS_I2 VTS_I2;
    InvokeHelper(0x85, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Heading, Speed, Diameter);
}

void CRRRCom::MoveStraight(short Speed, double Heading)
{
    static BYTE parms[] =
        VTS_I2 VTS_R8;
    InvokeHelper(0x86, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Speed, Heading);
}

void CRRRCom::SetMaxSpeed(short MaxSpeed)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x87, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 MaxSpeed);
}

void CRRRCom::SendHeading(double Heading)
{
    static BYTE parms[] =
        VTS_R8;
    InvokeHelper(0x88, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Heading);
}

void CRRRCom::SendMotorVelocities(short M0Velocity, short M1Velocity, short Priority)
{
    static BYTE parms[] =
        VTS_I2 VTS_I2 VTS_I2;
    InvokeHelper(0x89, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 M0Velocity, M1Velocity, Priority);
}

void CRRRCom::SetObstacleSensitivity(short Sensitivity)
{
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x8b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Sensitivity);
}

void CRRRCom::MoveTo(double X, double Y, short Speed, short Priority)
{
    static BYTE parms[] =
        VTS_R8 VTS_R8 VTS_I2 VTS_I2;
    InvokeHelper(0x8c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 X, Y, Speed, Priority);
}

void CRRRCom::SetPosition(long X, long Y)
{
    static BYTE parms[] =
        VTS_I4 VTS_I4;
    InvokeHelper(0x8d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 X, Y);
}

void CRRRCom::LoadPathSegment(long PurgeOld, LPDISPATCH PathObject)
{
    static BYTE parms[] =
        VTS_I4 VTS_DISPATCH;
    InvokeHelper(0x8f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 PurgeOld, PathObject);
}

void CRRRCom::MoveDirection(short Speed, double Heading, short Priority)
{
    static BYTE parms[] =
        VTS_I2 VTS_R8 VTS_I2;
    InvokeHelper(0x94, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 Speed, Heading, Priority);
}

void CRRRCom::RequestState()
{
    InvokeHelper(0x96, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CRRRCom::PowerDown()
{
    InvokeHelper(0x97, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CRRRCom::SendMessage(const VARIANT& MsgData)
{
    static BYTE parms[] =
        VTS_VARIANT;
    InvokeHelper(0x98, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 &MsgData);
}

CString CRRRCom::GetCommPort()
{
    CString result;
    InvokeHelper(0x9a, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
    return result;
}

void CRRRCom::SetCommPort(LPCTSTR NewPort)
{
    static BYTE parms[] =
        VTS_BSTR;
    InvokeHelper(0x9b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
                 NewPort);
}

void CRRRCom::AboutBox()
{
    InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// IRRRPath properties

short IRRRPath::GetNumPoints()
{
    short result;
    GetProperty(0x64, VT_I2, (void*)&result);
    return result;
}

void IRRRPath::SetNumPoints(short propVal)
{
    SetProperty(0x64, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IRRRPath operations

double IRRRPath::GetX(short Index)
{
    double result;
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms,
                 Index);
    return result;
}

void IRRRPath::SetX(short Index, double newValue)
{
    static BYTE parms[] =
        VTS_I2 VTS_R8;
    InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
                 Index, newValue);
}

double IRRRPath::GetY(short Index)
{
    double result;
    static BYTE parms[] =
        VTS_I2;
    InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms,
                 Index);
    return result;
}

void IRRRPath::SetY(short Index, double newValue)
{
    static BYTE parms[] =
        VTS_I2 VTS_R8;
    InvokeHelper(0x66, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
                 Index, newValue);
}
