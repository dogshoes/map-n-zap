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
// RRRSrvDoc.cpp : implementation of the CRRRSrvDoc class
//

#include "stdafx.h"
#include "RRRSrv.h"

#include "RRRSrvDoc.h"
#include "CyeCom.h"

/////////////////////////////////////////////////////////////////////////////
// CRRRSrvDoc

IMPLEMENT_DYNCREATE(CRRRSrvDoc, CDocument)

BEGIN_MESSAGE_MAP(CRRRSrvDoc, CDocument)
    //{{AFX_MSG_MAP(CRRRSrvDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CRRRSrvDoc, CDocument)
    //{{AFX_DISPATCH_MAP(CRRRSrvDoc)
    DISP_PROPERTY_EX(CRRRSrvDoc, "ResponseTimeout", GetResponseTimeout, SetResponseTimeout, VT_I4)
    DISP_FUNCTION(CRRRSrvDoc, "ServerVersion", ServerVersion, VT_BSTR, VTS_NONE)
    DISP_FUNCTION(CRRRSrvDoc, "Connect", Connect, VT_I4, VTS_I4 VTS_I2 VTS_BSTR)
    DISP_FUNCTION(CRRRSrvDoc, "Disconnect", Disconnect, VT_I2, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "RegisterSimClient", RegisterSimClient, VT_I4, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "SetMotorVelocities", SetMotorVelocities, VT_EMPTY, VTS_I2 VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SetRobotHeadingCorrectionFactor", SetRobotHeadingCorrectionFactor, VT_EMPTY, VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "FlipHandle", FlipHandle, VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION(CRRRSrvDoc, "SetMaxPWM", SetMaxPWM, VT_EMPTY, VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "StopMotors", StopMotors, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CRRRSrvDoc, "SetMotorsPWM", SetMotorsPWM, VT_EMPTY, VTS_I2 VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "MoveTo", MoveTo, VT_EMPTY, VTS_I4 VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "MoveToRelative", MoveToRelative, VT_EMPTY, VTS_I4 VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "ResetPosition", ResetPosition, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "Stop", Stop, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CRRRSrvDoc, "Pause", Pause, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CRRRSrvDoc, "Resume", Resume, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CRRRSrvDoc, "GetRadioStatus", GetRadioStatus, VT_I2, VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SetRadioStatus", SetRadioStatus, VT_EMPTY, VTS_I2 VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "GetRobotStatus", GetRobotStatus, VT_I2, VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SendP1", SendP1, VT_EMPTY, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "SendP2", SendP2, VT_EMPTY, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "SendP3", SendP3, VT_EMPTY, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "SendP4", SendP4, VT_EMPTY, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "SendBuzzerFrequency", SendBuzzerFrequency, VT_EMPTY, VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SendBuzzerOn", SendBuzzerOn, VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION(CRRRSrvDoc, "SendFollowPath", SendFollowPath, VT_EMPTY, VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SendHandleLength", SendHandleLength, VT_EMPTY, VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SendMotorControlConstants", SendMotorControlConstants, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SendTurn", SendTurn, VT_EMPTY, VTS_R8 VTS_I2 VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SendStraightVelocity", SendStraightVelocity, VT_EMPTY, VTS_I2 VTS_R8)
    DISP_FUNCTION(CRRRSrvDoc, "SendMaxSpeed", SendMaxSpeed, VT_EMPTY, VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SendHeading", SendHeading, VT_EMPTY, VTS_R8)
    DISP_FUNCTION(CRRRSrvDoc, "SendMotorVelocities", SendMotorVelocities, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "PurgeRingBuffer", PurgeRingBuffer, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CRRRSrvDoc, "SendPosition", SendPosition, VT_EMPTY, VTS_R8 VTS_R8)
    DISP_FUNCTION(CRRRSrvDoc, "SendPositionDestination", SendPositionDestination, VT_EMPTY, VTS_R8 VTS_R8)
    DISP_FUNCTION(CRRRSrvDoc, "ExecutePath", ExecutePath, VT_EMPTY, VTS_BOOL VTS_DISPATCH)
    DISP_FUNCTION(CRRRSrvDoc, "SetRobotStatus", SetRobotStatus, VT_EMPTY, VTS_I2 VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SendSensitiveObstacleDetection", SendSensitiveObstacleDetection, VT_EMPTY, VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SendP5", SendP5, VT_EMPTY, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "SendP6", SendP6, VT_EMPTY, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "SendP7", SendP7, VT_EMPTY, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "SendP8", SendP8, VT_EMPTY, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "SendVelocityDirection", SendVelocityDirection, VT_EMPTY, VTS_I2 VTS_R8 VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SetRobotDestinationHeading", SetRobotDestinationHeading, VT_EMPTY, VTS_R8 VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SendPositionVelocityDestination", SendPositionVelocityDestination, VT_EMPTY, VTS_R8 VTS_R8 VTS_I2 VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SendInformationToTransmit", SendInformationToTransmit, VT_EMPTY, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "SendRequestState", SendRequestState, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CRRRSrvDoc, "PowerDown", PowerDown, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CRRRSrvDoc, "SendMessage", SendMessage, VT_EMPTY, VTS_VARIANT)
    DISP_FUNCTION(CRRRSrvDoc, "GetRetryLimit", GetRetryLimit, VT_I4, VTS_NONE)
    DISP_FUNCTION(CRRRSrvDoc, "SetRetryLimit", SetRetryLimit, VT_EMPTY, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "GetPort", GetPort, VT_BSTR, VTS_NONE)
    DISP_FUNCTION(CRRRSrvDoc, "SetPort", SetPort, VT_EMPTY, VTS_BSTR)
    DISP_FUNCTION(CRRRSrvDoc, "SquareToWall", SquareToWall, VT_EMPTY, VTS_R8 VTS_R4 VTS_BOOL)
    DISP_FUNCTION(CRRRSrvDoc, "SetFeelingMotor", SetFeelingMotor, VT_EMPTY, VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "CornerCalibrate", CornerCalibrate, VT_EMPTY, VTS_I4 VTS_I4 VTS_R8 VTS_I2 VTS_R4 VTS_BOOL)
    DISP_FUNCTION(CRRRSrvDoc, "FindHomeBase", FindHomeBase, VT_EMPTY, VTS_R8 VTS_R8 VTS_I4 VTS_I4 VTS_BOOL)
    DISP_FUNCTION(CRRRSrvDoc, "SetHeadingDestination", SetHeadingDestination, VT_EMPTY, VTS_R8 VTS_I2 VTS_I2)
    DISP_FUNCTION(CRRRSrvDoc, "SetEnableObstacleBounce", SetEnableObstacleBounce, VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION(CRRRSrvDoc, "SetRAMData", SetRAMData, VT_EMPTY, VTS_I4 VTS_VARIANT)
    DISP_FUNCTION(CRRRSrvDoc, "SetRobotOnSeconds", SetRobotOnSeconds, VT_EMPTY, VTS_I4)
    DISP_FUNCTION(CRRRSrvDoc, "SetIntermittentBeep", SetIntermittentBeep, VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION(CRRRSrvDoc, "AddToPosition", AddToPosition, VT_EMPTY, VTS_R8 VTS_R8)
    DISP_FUNCTION(CRRRSrvDoc, "AddToHeading", AddToHeading, VT_EMPTY, VTS_R8)
    DISP_FUNCTION(CRRRSrvDoc, "SetExternalOutput", SetExternalOutput, VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION(CRRRSrvDoc, "RequestEX2Value", RequestEX2Value, VT_EMPTY, VTS_NONE)
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IRRRSrv to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {F300B092-6343-11D0-8461-707D42000000}
static const IID IID_ICyeSrv =
{ 0xf300b092, 0x6343, 0x11d0, { 0x84, 0x61, 0x70, 0x7d, 0x42, 0x0, 0x0, 0x0 } };

BEGIN_INTERFACE_MAP(CRRRSrvDoc, CDocument)
INTERFACE_PART(CRRRSrvDoc, IID_ICyeSrv, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRRRSrvDoc construction/destruction

CRRRSrvDoc::CRRRSrvDoc()
{
    // TODO: add one-time construction code here
    // get pointer to serial interface class
    CRRRSrvApp *SrvApp = (CRRRSrvApp *)AfxGetApp();

    EnableAutomation();

    AfxOleLockApp();
    //   if (SrvApp->m_oRRRCom == NULL)
    //      SrvApp->m_oRRRCom = new RRRSrvCom("COM1");
    rcom = SrvApp->m_oRRRCom;
    RobotAddress = - 1;                 // init address to unknown
}

CRRRSrvDoc::~CRRRSrvDoc()
{
    AfxOleUnlockApp();
}

BOOL CRRRSrvDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
    {
        return FALSE;
    }

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRRRSrvDoc serialization

void CRRRSrvDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: add storing code here
    }
    else
    {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CRRRSrvDoc diagnostics

#ifdef _DEBUG
void CRRRSrvDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CRRRSrvDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRRRSrvDoc commands

BSTR CRRRSrvDoc::ServerVersion()
{
    CString strResult = PROGRAM_VERSION;

    return strResult.AllocSysString();
}

long CRRRSrvDoc::Connect(long wHnd, short NodeAddress, LPCTSTR ControlName)
{
    // save target address
    RobotAddress = NodeAddress;
    return rcom->Connect(wHnd, NodeAddress, ControlName);
}

short CRRRSrvDoc::Disconnect(long ConnectId)
{
    return rcom->Disconnect(ConnectId);
}

long CRRRSrvDoc::RegisterSimClient(long SimWHandle)
{
    // TODO: Add your dispatch handler code here
    CRRRSrvApp *SrvApp = (CRRRSrvApp *)AfxGetApp();
    HWND mainwnd = SrvApp->m_pMainWnd->m_hWnd;
    rcom->EnableSimMsgXfer((HWND)SimWHandle, mainwnd);

    return (long)mainwnd;
}

void CRRRSrvDoc::SetMotorVelocities(short M0Velocity, short M1Velocity)
{
    // TODO: Add your dispatch handler code here

}

void CRRRSrvDoc::SetRobotHeadingCorrectionFactor(short Correction)
{
    RobotCom.SendHeadingCorrectionFactor(RobotAddress, Correction);
}

void CRRRSrvDoc::FlipHandle(BOOL Value)
{
    RobotCom.SendFlipHandle(RobotAddress, Value);
}

void CRRRSrvDoc::SetMaxPWM(short MaxPWM)
{
    RobotCom.SendMaxPWM(RobotAddress, (unsigned char)MaxPWM);
}

void CRRRSrvDoc::StopMotors()
{
    RobotCom.SendStopMotors(RobotAddress);
}

void CRRRSrvDoc::SetMotorsPWM(short M0PWM, short M1PWM)
{
    RobotCom.SendMotorsPWM(RobotAddress, M0PWM, M1PWM);
}

void CRRRSrvDoc::MoveTo(long X, long Y)
{
    // TODO: Add your dispatch handler code here

}

void CRRRSrvDoc::MoveToRelative(long X, long Y)
{
    // TODO: Add your dispatch handler code here

}

void CRRRSrvDoc::ResetPosition(long X, long Y, long H)
{
    // TODO: Add your dispatch handler code here

}

void CRRRSrvDoc::Stop()
{
    // TODO: Add your dispatch handler code here

}

void CRRRSrvDoc::Pause()
{
    RobotCom.Pause(RobotAddress);
}

void CRRRSrvDoc::Resume()
{
    RobotCom.Resume(RobotAddress);
}

short CRRRSrvDoc::GetRadioStatus(short rNum)
{
    // TODO: Add your dispatch handler code here

    return 0;
}

void CRRRSrvDoc::SetRadioStatus(short rNum, short nNewValue)
{
    // TODO: Add your dispatch handler code here

}

short CRRRSrvDoc::GetRobotStatus(short rNum)
{
    // TODO: Add your dispatch handler code here

    return 0;
}

void CRRRSrvDoc::SetRobotStatus(short rNum, short nNewValue)
{
    // TODO: Add your dispatch handler code here

}

void CRRRSrvDoc::SendP1(long P1)
{
    RobotCom.SendP1(RobotAddress, P1);
}

void CRRRSrvDoc::SendP2(long P2)
{
    RobotCom.SendP2(RobotAddress, P2);
}

void CRRRSrvDoc::SendP3(long P3)
{
    RobotCom.SendP3(RobotAddress, P3);
}

void CRRRSrvDoc::SendP4(long P4)
{
    RobotCom.SendP4(RobotAddress, P4);
}

void CRRRSrvDoc::SendBuzzerFrequency(short BuzzerFrequency)
{
    RobotCom.SendBuzzerFrequency(RobotAddress, BuzzerFrequency);
}

void CRRRSrvDoc::SendBuzzerOn(BOOL BuzzerOn)
{
    RobotCom.SendBuzzerOn(RobotAddress, BuzzerOn);
}

void CRRRSrvDoc::SendFollowPath(short FollowPath)
{
    RobotCom.SendFollowPath(RobotAddress, FollowPath);
}

void CRRRSrvDoc::SendHandleLength(short HandleLength)
{
    RobotCom.SendHandleLength(RobotAddress, HandleLength);
}

void CRRRSrvDoc::SendMotorControlConstants(short Kp, short Ki, short Kd, short Kb, short Kpwm)
{
    RobotCom.SendMotorControlConstants(RobotAddress, (unsigned char)Kp,
                                       (unsigned char)Ki, (unsigned char)Kd, (unsigned char)Kb,
                                       (unsigned char)Kpwm);
}

void CRRRSrvDoc::SendTurn(double Heading, short Velocity, short Diameter)
{
    RobotCom.SendTurn(RobotAddress, Heading, Velocity, Diameter);
}

void CRRRSrvDoc::SendStraightVelocity(short Velocity, double Heading)
{
    RobotCom.SendStraightVelocity(RobotAddress, Velocity, Heading);
}

void CRRRSrvDoc::SendMaxSpeed(short MaxSpeed)
{
    RobotCom.SendMaxSpeed(RobotAddress, MaxSpeed);
}

void CRRRSrvDoc::SendHeading(double Heading)
{
    RobotCom.SendHeading(RobotAddress, Heading);
}

void CRRRSrvDoc::SendMotorVelocities(short M0Velocity, short M1Velocity, short Priority)
{
    RobotCom.SendMotorVelocities(RobotAddress, M0Velocity, M1Velocity, Priority);
}

void CRRRSrvDoc::PurgeRingBuffer()
{
    RobotCom.SendPurgeRingBuffer(RobotAddress);
}

void CRRRSrvDoc::SendSensitiveObstacleDetection(short Sensitive)
{
    RobotCom.SendSensitiveObstacleDetection(RobotAddress, Sensitive);
}

void CRRRSrvDoc::SendPosition(double X, double Y)
{
    robPOINT Position;
    Position.x = X;
    Position.y = Y;
    RobotCom.SendPosition(RobotAddress, Position);
}

void CRRRSrvDoc::SendPositionDestination(double X, double Y)
{
    robPOINT Destination;
    Destination.x = X;
    Destination.y = Y;
    RobotCom.SendPositionDestination(RobotAddress, Destination);
}

void CRRRSrvDoc::ExecutePath(BOOL PurgeOld, LPDISPATCH PathObject)
{
    PathObject->AddRef();
    ICyePath Path(PathObject);
    short nPoints = Path.GetNumPoints();
    robPOINT *Points = new robPOINT[nPoints];

    for (short i = 0; i < nPoints; i++)
    {
        Points[i].x = Path.GetX(i);
        Points[i].y = Path.GetY(i);
    }

    RobotCom.SendPathSegment(RobotAddress, PurgeOld, Points, nPoints);
    delete Points;
}


void CRRRSrvDoc::SendP5(long P5)
{
    RobotCom.SendP5(RobotAddress, P5);
}

void CRRRSrvDoc::SendP6(long P6)
{
    RobotCom.SendP6(RobotAddress, P6);
}

void CRRRSrvDoc::SendP7(long P7)
{
    RobotCom.SendP7(RobotAddress, P7);
}

void CRRRSrvDoc::SendP8(long P8)
{
    RobotCom.SendP8(RobotAddress, P8);
}

void CRRRSrvDoc::SendVelocityDirection(short Velocity, double Heading, short Priority)
{
    RobotCom.SendVelocityDirection(RobotAddress, Velocity, Heading, Priority);
}

void CRRRSrvDoc::SetRobotDestinationHeading(double Heading, short Velocity)
{
    RobotCom.SendHeadingDestination(RobotAddress, Heading, Velocity);
}

void CRRRSrvDoc::SendPositionVelocityDestination(double X, double Y, short Velocity, short Priority)
{
    robPOINT Destination;
    Destination.x = X;
    Destination.y = Y;
    RobotCom.SendPositionVelocityDestination(RobotAddress, Destination, Velocity, Priority);
}

long CRRRSrvDoc::GetResponseTimeout()
{
    return rcom->GetResponseTime();
}

void CRRRSrvDoc::SetResponseTimeout(long nNewValue)
{
    rcom->SetResponseTime(nNewValue);
}

void CRRRSrvDoc::SendInformationToTransmit(long Mask)
{
    RobotCom.SendInformationToTransmit(RobotAddress, Mask);
}

void CRRRSrvDoc::SendRequestState()
{
    RobotCom.SendRequestState(RobotAddress);
}

void CRRRSrvDoc::PowerDown()
{
    RobotCom.SendPowerOff(RobotAddress);
}

void CRRRSrvDoc::SendMessage(const VARIANT FAR& MsgData)
{
    if (MsgData.vt == (VT_ARRAY | VT_UI1))
    {
        // now verify the array
        COleSafeArray array(MsgData);

        if (array.GetDim() == 1)
        {
            DWORD Size = array.GetOneDimSize();

            if (Size >= 1)
            {
                RRRMsg Msg(RobotAddress, '\000', Size);
                unsigned char *data;
                array.AccessData((void **)&data);

                for (unsigned short i = 0; i < Size; i++)
                {
                    Msg.SetAt(i, data[i]);
                }

                array.UnaccessData();
                RobotCom.SendRawData(RobotAddress, Msg);
            }
        }
    }
}

long CRRRSrvDoc::GetRetryLimit()
{
    return rcom->GetRetryLimit(RobotAddress);
}

void CRRRSrvDoc::SetRetryLimit(long NewLimit)
{
    rcom->SetRetryLimit(RobotAddress, NewLimit);
}

BSTR CRRRSrvDoc::GetPort()
{
    CString strResult = rcom->GetPort();
    return strResult.AllocSysString();
}

void CRRRSrvDoc::SetPort(LPCTSTR NewPort)
{
    CRRRSrvApp *theApp = (CRRRSrvApp *)AfxGetApp();

    if (theApp->m_sPortName != NewPort)
    {
        // delete the old interface and make a new with with this port
        HWND m_hWnd = rcom->GetMainWnd();
        // remove the old interface
        delete theApp->m_oRRRCom;
        // save the new port
        theApp->m_sPortName = NewPort;
        // create the interface
        theApp->m_oRRRCom = new RRRSrvCom(m_hWnd, theApp->m_MyAddress,
                                          theApp->m_sPortName, theApp->m_BaudRate,
                                          theApp->m_EnableDTR, theApp->m_DTROnDelay,
                                          theApp->m_DTROffDelay, theApp->m_Encoding, theApp->m_FullDuplex,
                                          theApp->m_InvertDTR);
        // place the interface pointer in rcom
        rcom = theApp->m_oRRRCom;
        // provide new interface pointer to RobotCom
        RobotCom.ResetInterface(rcom);
        // save the parameters
        theApp->SaveParameters();
    }
}

void CRRRSrvDoc::SquareToWall(double Heading, float TravelTime, BOOL SlipperySurface)
{
    RobotCom.SquareToWall(RobotAddress, Heading, TravelTime, SlipperySurface);
}

void CRRRSrvDoc::SetFeelingMotor(short Motor)
{
    RobotCom.SetFeelingMotor(RobotAddress, (unsigned char)Motor);
}

void CRRRSrvDoc::CornerCalibrate(long X, long Y, double HO, short FeelWheel, float TravelTime, BOOL SlipperySurface)
{
    RobotCom.CornerCalibrate(RobotAddress, X, Y, HO, (unsigned char)FeelWheel, TravelTime, SlipperySurface);
}

void CRRRSrvDoc::FindHomeBase(double CornerHeading, double WallHeading, long HomeBaseLocationX, long HomeBaseLocationY, BOOL NeedToGetInFrontOfHomeBase)
{
    RobotCom.FindHomeBase(RobotAddress, CornerHeading, WallHeading, HomeBaseLocationX, HomeBaseLocationY, NeedToGetInFrontOfHomeBase);
}

void CRRRSrvDoc::SetHeadingDestination(double Heading, short Speed, short MotorEnable)
{
    RobotCom.SetHeadingDestination(RobotAddress, Heading, Speed, MotorEnable);
}

void CRRRSrvDoc::SetEnableObstacleBounce(BOOL Enable)
{
    RobotCom.SetEnableObstacleBounce(RobotAddress, Enable);
}

void CRRRSrvDoc::SetRAMData(long Address, const VARIANT FAR& Data)
{
    if (Data.vt == (VT_ARRAY | VT_UI1))
    {
        // now verify the array
        COleSafeArray array(Data);

        if (array.GetDim() == 1)
        {
            DWORD Size = array.GetOneDimSize();

            if (Size >= 1)
            {
                unsigned char *data;
                array.AccessData((void **)&data);
                RobotCom.SetRAMData(RobotAddress, (unsigned short)Address, (unsigned char)Size, data);
                array.UnaccessData();
            }
        }
    }
}

void CRRRSrvDoc::SetRobotOnSeconds(long Seconds)
{
    RobotCom.SetRobotOnSeconds(RobotAddress, Seconds);
}

void CRRRSrvDoc::SetIntermittentBeep(BOOL On)
{
    RobotCom.SetIntermittentBeep(RobotAddress, On);
}

void CRRRSrvDoc::AddToPosition(double DeltaX, double DeltaY)
{
    RobotCom.AddToPosition(RobotAddress, DeltaX, DeltaY);
}

void CRRRSrvDoc::AddToHeading(double Delta)
{
    RobotCom.AddToHeading(RobotAddress, Delta);
}

void CRRRSrvDoc::SetExternalOutput(BOOL On)
{
    RobotCom.SetExternalOutput(RobotAddress, On);
}

void CRRRSrvDoc::RequestEX2Value()
{
    RobotCom.RequestEX2Value(RobotAddress);
}
