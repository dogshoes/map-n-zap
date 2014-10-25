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
// RRRComCtl.cpp : Implementation of the CRRRComCtrl OLE control class.

#include "stdafx.h"
#include "RRRCom.h"
#include "RRRConst.h"
#include "RRRComCtl.h"
#include "RRRComPpg.h"
#include "RRRDispID.h"

IMPLEMENT_DYNCREATE(CRRRComCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CRRRComCtrl, COleControl)
    //{{AFX_MSG_MAP(CRRRComCtrl)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
    ON_MESSAGE(RRR_VELOCITY_CHANGE, OnVelocityChange)
    ON_MESSAGE(WM_COPYDATA, OnCopyData)
    ON_MESSAGE(RRR_LOCAL_COPYDATA, OnLocalCopyData)
    ON_MESSAGE(RRR_COMMSTATECHANGE, OnCommStateChange)
    ON_MESSAGE(RRR_RADIOLEVELCHANGE, OnRadioLevelChange)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CRRRComCtrl, COleControl)
    //{{AFX_DISPATCH_MAP(CRRRComCtrl)
    //}}AFX_DISPATCH_MAP
    DISP_PROPERTY_NOTIFY_ID(CRRRComCtrl, "ResponseTimeout", RRRCTL_RESPONSETIMEOUT, m_responseTimeout, OnResponseTimeoutChanged, VT_I4)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "Version", RRRCTL_VERSION, GetVersion, SetVersion, VT_BSTR)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "X", RRRCTL_X, GetX, SetX, VT_I4)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "Y", RRRCTL_Y, GetY, SetY, VT_I4)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "Heading", RRRCTL_HEADING, GetHeading, SetHeading, VT_R8)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "RobotAddress", RRRCTL_ROBOTADDRESS, GetRobotAddress, SetRobotAddress, VT_I2)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "RadioLevel", RRRCTL_RADIOLEVEL, GetRadioLevel, SetRadioLevel, VT_R4)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "BatteryLevel", RRRCTL_BATTERYLEVEL, GetBatteryLevel, SetBatteryLevel, VT_R4)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "RetryLimit", RRRCTL_RETRYLIMIT, GetRetryLimit, SetRetryLimit, VT_I4)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "CommState", RRRCTL_COMMSTATE, GetCommState, SetCommState, VT_I2)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "RightCurrent", RRRCTL_RIGHTCURRENT, GetRightCurrent, SetRightCurrent, VT_R4)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "LeftCurrent", RRRCTL_LEFTCURRENT, GetLeftCurrent, SetLeftCurrent, VT_R4)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "LeftVelocity", RRRCTL_LEFTVELOCITY, GetLeftVelocity, SetLeftVelocity, VT_I2)
    DISP_PROPERTY_EX_ID(CRRRComCtrl, "RightVelocity", RRRCTL_RIGHTVELOCITY, GetRightVelocity, SetRightVelocity, VT_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetHeadingCorrectionFactor", RRRCTL_SETHEADINGCORRECTIONFACTOR, SetHeadingCorrectionFactor, VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetFlipHandle", RRRCTL_SETFLIPHANDLE, SetFlipHandle, VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetMaxPWM", RRRCTL_SETMAXPWM, SetMaxPWM, VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "TurnTo", RRRCTL_TURNTO, TurnTo, VT_EMPTY, VTS_R8 VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetPWM", RRRCTL_SETPWM, SetPWM, VT_EMPTY, VTS_I2 VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "Stop", RRRCTL_STOP, Stop, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CRRRComCtrl, "Pause", RRRCTL_PAUSE, Pause, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CRRRComCtrl, "Resume", RRRCTL_RESUME, Resume, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CRRRComCtrl, "SendP1", RRRCTL_SENDP1, SendP1, VT_EMPTY, VTS_I4)
    DISP_FUNCTION_ID(CRRRComCtrl, "SendP2", RRRCTL_SENDP2, SendP2, VT_EMPTY, VTS_I4)
    DISP_FUNCTION_ID(CRRRComCtrl, "SendP3", RRRCTL_SENDP3, SendP3, VT_EMPTY, VTS_I4)
    DISP_FUNCTION_ID(CRRRComCtrl, "SendP4", RRRCTL_SENDP4, SendP4, VT_EMPTY, VTS_I4)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetBuzzerFrequency", RRRCTL_SETBUZZERFREQUENCY, SetBuzzerFrequency, VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetBuzzerOn", RRRCTL_SETBUZZERON, SetBuzzerOn, VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION_ID(CRRRComCtrl, "FollowPath", RRRCTL_FOLLOWPATH, FollowPath, VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetHandleLength", RRRCTL_SETHANDLELENGTH, SetHandleLength, VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetMotorControlConstants", RRRCTL_SETMOTORCONTROLCONSTANTS, SetMotorControlConstants, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "MoveCurve", RRRCTL_MOVECURVE, MoveCurve, VT_EMPTY, VTS_R8 VTS_I2 VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "MoveStraight", RRRCTL_MOVESTRAIGHT, MoveStraight, VT_EMPTY, VTS_I2 VTS_R8)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetMaxSpeed", RRRCTL_SETMAXSPEED, SetMaxSpeed, VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetWheelVelocities", RRRCTL_SETWHEELVELOCITIES, SetWheelVelocities, VT_EMPTY, VTS_I2 VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "PurgeRingBuffer", RRRCTL_PURGERINGBUFFER, PurgeRingBuffer, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetObstacleSensitivity", RRRCTL_SETOBSTACLESENSITIVITY, SetObstacleSensitivity, VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "MoveTo", RRRCTL_MOVETO, MoveTo, VT_EMPTY, VTS_R8 VTS_R8 VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetPosition", RRRCTL_SETPOSITION, SetPosition, VT_EMPTY, VTS_I4 VTS_I4)
    DISP_FUNCTION_ID(CRRRComCtrl, "SendPositionDestination", RRRCTL_SENDPOSITIONDESTINATION, SendPositionDestination, VT_EMPTY, VTS_R8 VTS_R8)
    DISP_FUNCTION_ID(CRRRComCtrl, "LoadPathSegment", RRRCTL_LOADPATHSEGMENT, LoadPathSegment, VT_EMPTY, VTS_BOOL VTS_DISPATCH)
    DISP_FUNCTION_ID(CRRRComCtrl, "SendP5", RRRCTL_SENDP5, SendP5, VT_EMPTY, VTS_I4)
    DISP_FUNCTION_ID(CRRRComCtrl, "SendP6", RRRCTL_SENDP6, SendP6, VT_EMPTY, VTS_I4)
    DISP_FUNCTION_ID(CRRRComCtrl, "SendP7", RRRCTL_SENDP7, SendP7, VT_EMPTY, VTS_I4)
    DISP_FUNCTION_ID(CRRRComCtrl, "SendP8", RRRCTL_SENDP8, SendP8, VT_EMPTY, VTS_I4)
    DISP_FUNCTION_ID(CRRRComCtrl, "MoveDirection", RRRCTL_MOVEDIRECTION, MoveDirection, VT_EMPTY, VTS_I2 VTS_R8)
    DISP_FUNCTION_ID(CRRRComCtrl, "SendInformationToTransmit", RRRCTL_SENDINFORMATIONTOTRANSMIT, SendInformationToTransmit, VT_EMPTY, VTS_I4)
    DISP_FUNCTION_ID(CRRRComCtrl, "RequestState", RRRCTL_REQUESTSTATE, RequestState, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CRRRComCtrl, "PowerDown", RRRCTL_POWERDOWN, PowerDown, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CRRRComCtrl, "SendMessage", RRRCTL_SENDMESSAGE, SendMessage, VT_EMPTY, VTS_VARIANT)
    DISP_FUNCTION_ID(CRRRComCtrl, "GetCommPort", RRRCTL_GETCOMMPORT, GetCommPort, VT_BSTR, VTS_NONE)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetCommPort", RRRCTL_SETCOMMPORT, SetCommPort, VT_EMPTY, VTS_BSTR)
    DISP_FUNCTION_ID(CRRRComCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CRRRComCtrl, "SquareToWall", RRRCTL_SQUARETOWALL, SquareToWall, VT_EMPTY, VTS_R8 VTS_R4 VTS_I4)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetFeelingMotor", RRRCTL_SETFEELINGMOTOR, SetFeelingMotor, VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "CornerCalibrate", RRRCTL_CORNERCALIBRATE, CornerCalibrate, VT_EMPTY, VTS_I4 VTS_I4 VTS_R8 VTS_I2 VTS_R4 VTS_BOOL)
    DISP_FUNCTION_ID(CRRRComCtrl, "FindHomeBase", RRRCTL_FINDHOMEBASE, FindHomeBase, VT_EMPTY, VTS_R8 VTS_R8 VTS_I4 VTS_I4 VTS_BOOL)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetHeadingDestination", RRRCTL_SETHEADINGDESTINATION, SetHeadingDestination, VT_EMPTY, VTS_R8 VTS_I2 VTS_I2)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetEnableObstacleBounce", RRRCTL_SETENABLEOBSTACLEBOUNCE, SetEnableObstacleBounce, VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetRAMData", RRRCTL_SETRAMDATA, SetRAMData, VT_EMPTY, VTS_I4 VTS_VARIANT)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetRobotOnSeconds", RRRCTL_SETROBOTONSECONDS, SetRobotOnSeconds, VT_EMPTY, VTS_I4)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetIntermittentBeep", RRRCTL_SETINTERMITTENTBEEP, SetIntermittentBeep, VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION_ID(CRRRComCtrl, "AddToPosition", RRRCTL_ADDTOPOSITION, AddToPosition, VT_EMPTY, VTS_R8 VTS_R8)
    DISP_FUNCTION_ID(CRRRComCtrl, "AddToHeading", RRRCTL_ADDTOHEADING, AddToHeading, VT_EMPTY, VTS_R8)
    DISP_FUNCTION_ID(CRRRComCtrl, "SetExternalOutput", RRRCTL_SETEXTERNALOUTPUT, SetExternalOutput, VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION_ID(CRRRComCtrl, "RequestExternalInput", RRRCTL_REQUESTEXTERNALINPUT, RequestExternalInput, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CRRRComCtrl, COleControl)
    //{{AFX_EVENT_MAP(CRRRComCtrl)
    EVENT_CUSTOM("MicrophoneVolume", FireMicrophoneVolume, VTS_I2)
    //}}AFX_EVENT_MAP
    EVENT_CUSTOM_ID("RadioLevelChange", RRRCTL_RADIOLEVELCHANGE, FireRadioLevelChange, VTS_R4)
    EVENT_CUSTOM_ID("BatteryLow", RRRCTL_BATTERYLOW, FireBatteryLow, VTS_NONE)
    EVENT_CUSTOM_ID("PositionChange", RRRCTL_POSITIONCHANGE, FirePositionChange, VTS_I4  VTS_I4  VTS_I4)
    EVENT_CUSTOM_ID("ObstacleDetected", RRRCTL_OBSTACLEDETECTED, FireObstacleDetected, VTS_I2  VTS_I2)
    EVENT_CUSTOM_ID("NudgeDetected", RRRCTL_NUDGEDETECTED, FireNudgeDetected, VTS_BOOL  VTS_BOOL)
    EVENT_CUSTOM_ID("VelocityChange", RRRCTL_VELOCITYCHANGE, FireVelocityChange, VTS_I2  VTS_I2)
    EVENT_CUSTOM_ID("AccelerationChange", RRRCTL_ACCELERATIONCHANGE, FireAccelerationChange, VTS_I2  VTS_I2)
    EVENT_CUSTOM_ID("EncoderUpdate", RRRCTL_ENCODERUPDATE, FireEncoderUpdate, VTS_I4  VTS_I4)
    EVENT_CUSTOM_ID("AnalogUpdate", RRRCTL_ANALOGUPDATE, FireAnalogUpdate, VTS_I2  VTS_I2  VTS_I2)
    EVENT_CUSTOM_ID("PWMUpdate", RRRCTL_PWMUPDATE, FirePWMUpdate, VTS_I2  VTS_I2)
    EVENT_CUSTOM_ID("RingBufferDump", RRRCTL_RINGBUFFERDUMP, FireRingBufferDump, VTS_I4)
    EVENT_CUSTOM_ID("NewPathSegmentRequest", RRRCTL_NEWPATHSEGMENTREQUEST, FireNewPathSegmentRequest, VTS_NONE)
    EVENT_CUSTOM_ID("RobotError", RRRCTL_ROBOTERROR, FireRobotError, VTS_I4)
    EVENT_CUSTOM_ID("OnHomeBase", RRRCTL_ONHOMEBASE, FireOnHomeBase, VTS_I2)
    EVENT_CUSTOM_ID("RobotStatusChange", RRRCTL_ROBOTSTATUSCHANGE, FireRobotStatusChange, VTS_I4  VTS_I4  VTS_R8  VTS_I2  VTS_I2  VTS_R4  VTS_R4  VTS_R4)
    EVENT_CUSTOM_ID("BatteryChargingStateChange", RRRCTL_BATTERYCHARGINGSTATECHANGE, FireBatteryChargingStateChange, VTS_I2)
    EVENT_CUSTOM_ID("UnknownMessage", RRRCTL_UNKNOWNMESSAGE, FireUnknownMessage, VTS_VARIANT)
    EVENT_CUSTOM_ID("CommStateChange", RRRCTL_COMMSTATECHANGE, FireCommStateChange, VTS_I2)
    EVENT_CUSTOM_ID("TaskResult", RRRCTL_TASKRESULT, FireTaskResult, VTS_I2  VTS_I4  VTS_I4  VTS_R8)
    EVENT_CUSTOM_ID("SoftwareRevNumber", RRRCTL_SOFTWAREREVNUMBER, FireSoftwareRevNumber, VTS_I4)
    EVENT_CUSTOM_ID("RobotOn", RRRCTL_ROBOTON, FireRobotOn, VTS_NONE)
    EVENT_CUSTOM_ID("RobotOnTime", RRRCTL_ROBOTONTIME, FireRobotOnTime, VTS_R4)
    EVENT_CUSTOM_ID("ExternalInput", RRRCTL_EXTERNALINPUT, FireExternalInput, VTS_R4)
    EVENT_CUSTOM_ID("MicrophoneVolume", RRRCTL_MICROPHONEVOLUME, FireMicrophoneVolume, VTS_I2)
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CRRRComCtrl, 1)
    PROPPAGEID(CRRRComPropPage::guid)
END_PROPPAGEIDS(CRRRComCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CRRRComCtrl, "CyeCOM.CyeComCtrl.1",
                       0x1bb9c513, 0x657f, 0x11d0, 0x84, 0x65, 0xca, 0x28, 0x1d, 0, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CRRRComCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCyeCom =
{ 0x1bb9c511, 0x657f, 0x11d0, { 0x84, 0x65, 0xca, 0x28, 0x1d, 0, 0, 0 } };
const IID BASED_CODE IID_DCyeComEvents =
{ 0x1bb9c512, 0x657f, 0x11d0, { 0x84, 0x65, 0xca, 0x28, 0x1d, 0, 0, 0 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwRRRComOleMisc =
    OLEMISC_INVISIBLEATRUNTIME |
    OLEMISC_SETCLIENTSITEFIRST |
    OLEMISC_INSIDEOUT |
    OLEMISC_CANTLINKINSIDE |
    OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CRRRComCtrl, IDS_RRRCOM, _dwRRRComOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CRRRComCtrl::CRRRComCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CRRRComCtrl

BOOL CRRRComCtrl::CRRRComCtrlFactory::UpdateRegistry(BOOL bRegister)
{
    // TODO: Verify that your control follows apartment-model threading rules.
    // Refer to MFC TechNote 64 for more information.
    // If your control does not conform to the apartment-model rules, then
    // you must modify the code below, changing the 6th parameter from
    // afxRegApartmentThreading to 0.

    if (bRegister)
        return AfxOleRegisterControlClass(
                   AfxGetInstanceHandle(),
                   m_clsid,
                   m_lpszProgID,
                   IDS_RRRCOM,
                   IDB_RRRCOM,
                   afxRegApartmentThreading,
                   _dwRRRComOleMisc,
                   _tlid,
                   _wVerMajor,
                   _wVerMinor);
    else
    {
        return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
    }
}


/////////////////////////////////////////////////////////////////////////////
// CRRRComCtrl::CRRRComCtrl - Constructor

CRRRComCtrl::CRRRComCtrl()
{
    InitializeIIDs(&IID_DCyeCom, &IID_DCyeComEvents);

    // TODO: Initialize your control's instance data here.
    CBitmap MyIcon;
    MyIcon.LoadBitmap(IDB_BIGRRRCOM);
    BITMAP MyBm;
    MyIcon.GetBitmap(&MyBm);
    m_pImage.CreateFromBitmap(&MyIcon);
    SetInitialSize(MyBm.bmWidth, MyBm.bmHeight);
    m_lConnectId = -1;                  // init control to disconnected
    EventsFrozen = FALSE;               // clear the events frozen flag
    m_sCommState = 0;                   // init state to not connected
    m_bCommStateEventPending = TRUE;    // force the first state event
    m_bRadioLevelEventPending = TRUE;   // force initial send of radio level
    m_RadioLevel = RADIO_IDLE;
    m_SoundVolume = -1;                 // force a volume update as soon as one is received

    // init status variables
    m_X = 0;
    m_Y = 0;
    m_Heading = 0;
    m_RightVelocity = 0;
    m_LeftVelocity = 0;
    m_RightCurrent = 0;
    m_LeftCurrent = 0;
    m_BatteryVoltage = 0;
}


/////////////////////////////////////////////////////////////////////////////
// CRRRComCtrl::~CRRRComCtrl - Destructor

CRRRComCtrl::~CRRRComCtrl()
{
    // detach the control from the OLE server
    DetachControl();
}


/////////////////////////////////////////////////////////////////////////////
// CRRRComCtrl::OnDraw - Drawing function

void CRRRComCtrl::OnDraw(
    CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
    // TODO: Replace the following code with your own drawing code.
    //	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
    //	pdc->Ellipse(rcBounds);
    m_pImage.Render(pdc, rcBounds, rcInvalid);
}


/////////////////////////////////////////////////////////////////////////////
// CRRRComCtrl::DoPropExchange - Persistence support

void CRRRComCtrl::DoPropExchange(CPropExchange* pPX)
{
    ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
    COleControl::DoPropExchange(pPX);

    // TODO: Call PX_ functions for each persistent custom property.
    PX_Long(pPX, "ResponseTimeout", m_responseTimeout, 100);
    PX_Long(pPX, "RetryLimit", m_retryLimit, 7);
    PX_Short(pPX, "RobotAddress", m_robotAddress, DEFAULT_ROBOT_ADDRESS);

    //
    // if this is being loaded in "User Mode" i.e. a live application
    // the control window must be created in order to receive
    // messages. This is done here because the properties must
    // be loaded first before creating the window...
    //
    if (pPX->IsLoading())
    {
        RecreateControlWindow();
    }
}


/////////////////////////////////////////////////////////////////////////////
// CRRRComCtrl::OnResetState - Reset control to default state

void CRRRComCtrl::OnResetState()
{
    COleControl::OnResetState();  // Resets defaults found in DoPropExchange

    // TODO: Reset any other control state here.
}

afx_msg LRESULT CRRRComCtrl::OnCopyData(WPARAM wParam, LPARAM lParam)
{
    COPYDATASTRUCT *cd = (COPYDATASTRUCT *)lParam;
    unsigned long result = 0;

    COPYDATASTRUCT *localcd = new COPYDATASTRUCT;
    localcd->dwData = cd->dwData;
    localcd->cbData = cd->cbData;

    if (cd->lpData == NULL)
    {
        localcd->lpData = NULL;
    }
    else
    {
        localcd->lpData = new char[cd->cbData];
        memcpy(localcd->lpData, cd->lpData, cd->cbData);
    }

    PostMessage(RRR_LOCAL_COPYDATA, (WPARAM)localcd, 0);
    return result;
}

afx_msg LRESULT CRRRComCtrl::OnLocalCopyData(WPARAM wParam, LPARAM lParam)
{
    COPYDATASTRUCT *cd = (COPYDATASTRUCT *)wParam;

    if (EventsFrozen)
    {
        CSingleLock Semp(&m_oSemp);
        Semp.Lock();
        CDEventArray.Add(cd);
        Semp.Unlock();
    }
    else
    {
        switch (cd->dwData)
        {
        case RRR_CHECKSUM_CDMSG:
        {
            RRR_CHECKSUM_MSG *Msg = (RRR_CHECKSUM_MSG *)cd->lpData;

            switch (Msg->CommandNum)
            {
            case cPositionInput:       //Serial input command character for a position packet
                FirePositionChange(Msg->ReturnValue[0], Msg->ReturnValue[1], Msg->ReturnValue[2]);
                break;

            case cNudgeInput:          //Serial input command character for a nudge packet
                switch (Msg->ReturnValue[0])
                {
                case 0:
                    FireNudgeDetected(TRUE, FALSE);
                    break;

                case 1:
                    FireNudgeDetected(FALSE, TRUE);
                    break;
                }

                break;

            case cObstacleInput:       //Serial input command character for an obstacle packet
                FireObstacleDetected((short)Msg->ReturnValue[0], (short)Msg->ReturnValue[1]);
                break;

            case cVelocityInput:       //Serial input command character for a velocity packet
                FireVelocityChange((short)Msg->ReturnValue[0], (short)Msg->ReturnValue[1]);
                break;

            case cAccelerationInput:   //Serial input command character for an acceleration packet
                FireAccelerationChange((short)Msg->ReturnValue[0], (short)Msg->ReturnValue[1]);
                break;

            case cEncodersInput:       //Serial input command character for an encoders packet
                FireEncoderUpdate(Msg->ReturnValue[0], Msg->ReturnValue[1]);
                break;

            case cAnalogInput:         //Serial input command character for an analog packet
                FireAnalogUpdate((short)Msg->ReturnValue[0], (short)Msg->ReturnValue[1], (short)Msg->ReturnValue[2]);
                break;

            case cPwmInput:            //Serial input command character for a pwm packet
                FirePWMUpdate((short)Msg->ReturnValue[0], (short)Msg->ReturnValue[1]);
                break;

            case cRingBufferInput:     //Serial input command character for a ring buffer dump
                FireRingBufferDump(Msg->ReturnValue[0]);
                break;

            case cSendNewPathSegment:  //Serial input command character for sending a new robot path segment
                FireNewPathSegmentRequest();
                break;

            case cInvalidInputString:  //returned when the command character from a serial string was invalid
                break;

            case cLowBatteryMessage:   //Serial input command character for low battery message
                FireBatteryChange();
                break;

            case cRobotError:          // generic robot error detected
                FireRobotError(Msg->ReturnValue[0]);
                break;

            case cRobotAtHome:         // robot is at home base
                FireOnHomeBase((short)Msg->ReturnValue[0]);
                break;

            case cRobotStatusUpdate:   // robot status update
                m_X = Msg->ReturnValue[0];
                m_Y = Msg->ReturnValue[1];
                m_Heading = Msg->dReturnValue[0];
                m_RightVelocity = (short)Msg->ReturnValue[3];
                m_LeftVelocity = (short)Msg->ReturnValue[4];
                m_RightCurrent = Msg->fReturnValue[1];
                m_LeftCurrent = Msg->fReturnValue[2];
                m_BatteryVoltage = Msg->fReturnValue[0];
                FireRobotStatusChange(m_X, m_Y,
                                      m_Heading,
                                      m_RightVelocity, m_LeftVelocity,
                                      m_RightCurrent, m_LeftCurrent,
                                      m_BatteryVoltage);

                // check for a volume change...
                if (Msg->SoundVolume != m_SoundVolume)
                {
                    // save new volume
                    m_SoundVolume = Msg->SoundVolume;
                    // notify the caller
                    FireMicrophoneVolume(m_SoundVolume);
                }

                break;

            case cBatteryChargeState:  // battery charge state change
                FireBatteryChargingStateChange((short)Msg->ReturnValue[0]);
                break;

            case cTaskResultInput:		//Task Result
                FireTaskResult((short)Msg->ReturnValue[0], Msg->ReturnValue[1], Msg->ReturnValue[2], Msg->dReturnValue[0]);
                break;

            case cSoftwareRevNumber: 	//Software Revision Number
                FireSoftwareRevNumber(Msg->ReturnValue[0]);
                break;

            case cRobotOn:		         //Robot On
                FireRobotOn();
                break;

            case cRobotOnSeconds:		//Robot On Seconds
                FireRobotOnTime(Msg->fReturnValue[0]);
                break;

            case cSendEX2:		         //Send External Input
                FireExternalInput(Msg->fReturnValue[0]);
                break;
            }
        }
        break;

        case RRR_RAWDATA_CDMSG:
        {
            if (cd->lpData && (cd->cbData > 0))
            {
                COleSafeArray array;
                array.CreateOneDim(VT_UI1, cd->cbData, cd->lpData);
                FireUnknownMessage(array);
            }
        }
        break;
        }

        // delete the message buffers
        if (cd->lpData)
        {
            delete cd->lpData;
        }

        delete cd;
    }

    return 0;
}

afx_msg LRESULT CRRRComCtrl::OnCommStateChange(WPARAM wParam, LPARAM lParam)
{
    m_sCommState = wParam;              // save the new state

    if (EventsFrozen)
    {
        m_bCommStateEventPending = TRUE;
    }
    else
    {
        FireCommStateChange(wParam);
        m_bCommStateEventPending = FALSE;
    }

    return 0;
}

afx_msg LRESULT CRRRComCtrl::OnRadioLevelChange(WPARAM wParam, LPARAM lParam)
{
    if ((wParam == 0) && (lParam == 0))
    {
        m_RadioLevel = RADIO_IDLE;
    }
    else
    {
        float TotalSends = float(wParam + lParam);
        float SuccessFullSends = float(wParam);
        m_RadioLevel = (SuccessFullSends / TotalSends) * 100.0f;
    }

    if (EventsFrozen)
    {
        m_bRadioLevelEventPending = TRUE;
    }
    else
    {
        FireRadioChange(m_RadioLevel);
        m_bRadioLevelEventPending = FALSE;
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CRRRComCtrl::AboutBox - Display an "About" box to the user

void CRRRComCtrl::AboutBox()
{
    CDialog dlgAbout(IDD_ABOUTBOX_RRRCOM);
    dlgAbout.DoModal();
}

void CRRRComCtrl::AttachControl()
{
    if (m_oCyeSrv.m_lpDispatch == NULL)
    {
        m_oCyeSrv.CreateDispatch("CyeSrv.Document");
    }

    if (m_oCyeSrv.m_lpDispatch)
    {
        m_sVersion = m_oCyeSrv.ServerVersion();
    }

    m_lConnectId = m_oCyeSrv.Connect((long)m_hWnd, m_robotAddress, AmbientDisplayName());
    m_oCyeSrv.SetResponseTimeout(m_responseTimeout);
    m_oCyeSrv.SetRetryLimit(m_retryLimit);
}

void CRRRComCtrl::DetachControl()
{
    m_oCyeSrv.Disconnect(m_lConnectId);
}


/////////////////////////////////////////////////////////////////////////////
// CRRRComCtrl message handlers

BSTR CRRRComCtrl::GetVersion()
{
    return m_sVersion.AllocSysString();
}

void CRRRComCtrl::SetVersion(LPCTSTR lpszNewValue)
{
    SetNotSupported();
}

BOOL CRRRComCtrl::OnSetExtent(LPSIZEL lpSizeL)
{
    // TODO: Add your specialized code here and/or call the base class

    //	return COleControl::OnSetExtent(lpSizeL);
    return FALSE;
}


int CRRRComCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (COleControl::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    // Attach the control to the OLE server
    AttachControl();

    return 0;
}

afx_msg LRESULT CRRRComCtrl::OnVelocityChange(WPARAM wParam, LPARAM lParam)
{
    if (!EventsFrozen)
    {
        float velocity = *(float *)&wParam;
        //FireVelocityChange(&velocity);
    }
    else
    {
        // queue the event to be delivered at a later date
        m_oEventQueue.AddEntry(RRR_VELOCITY_CHANGE, wParam, lParam);
    }

    return 0;
}

void CRRRComCtrl::SetHeadingCorrectionFactor(short HeadingCorrectionFactor)
{
    m_oCyeSrv.SetRobotHeadingCorrectionFactor(HeadingCorrectionFactor);
}

void CRRRComCtrl::SetFlipHandle(BOOL FlipHandle)
{
    m_oCyeSrv.FlipHandle(FlipHandle);
}

void CRRRComCtrl::SetMaxPWM(short MaxPWM)
{
    m_oCyeSrv.SetMaxPWM(MaxPWM);
}

void CRRRComCtrl::TurnTo(double Heading, short Speed)
{
    m_oCyeSrv.SetRobotDestinationHeading(Heading, Speed);
}

void CRRRComCtrl::Stop()
{
    m_oCyeSrv.StopMotors();
}

void CRRRComCtrl::SetPWM(short RightPWM, short LeftPWM)
{
    m_oCyeSrv.SetMotorsPWM(RightPWM, LeftPWM);
}

void CRRRComCtrl::Pause()
{
    m_oCyeSrv.Pause();
}

void CRRRComCtrl::Resume()
{
    m_oCyeSrv.Resume();
}

long CRRRComCtrl::GetX()
{
    return m_X;
}

void CRRRComCtrl::SetX(long nNewValue)
{
    SetNotSupported();
}

long CRRRComCtrl::GetY()
{
    return m_Y;
}

void CRRRComCtrl::SetY(long nNewValue)
{
    SetNotSupported();
}

double CRRRComCtrl::GetHeading()
{
    return m_Heading;
}

void CRRRComCtrl::SetHeading(double nNewValue)
{
    m_oCyeSrv.SendHeading(nNewValue);
}

short CRRRComCtrl::GetRobotAddress()
{
    // TODO: Add your property handler here

    return m_robotAddress;
}

void CRRRComCtrl::SetRobotAddress(short nNewValue)
{
    // TODO: Add your property handler here
    if ((nNewValue < 0) || (nNewValue > 15))
    {
        ThrowError(CTL_E_INVALIDPROPERTYVALUE, "Robot address out of range.");
    }
    else
    {
        m_robotAddress = nNewValue;
        // disconnect from the old address
        DetachControl();
        // reconnect with the new address
        AttachControl();
        SetModifiedFlag();
    }
}

float CRRRComCtrl::GetRadioLevel()
{
    return m_RadioLevel;
}

void CRRRComCtrl::SetRadioLevel(float newValue)
{
    SetNotSupported();
}

float CRRRComCtrl::GetBatteryLevel()
{
    return m_BatteryVoltage;
}

void CRRRComCtrl::SetBatteryLevel(float newValue)
{
    SetNotSupported();
}

void CRRRComCtrl::OnFreezeEvents(BOOL bFreeze)
{
    // TODO: Add your specialized code here and/or call the base class
    EventsFrozen = bFreeze;

    if (!bFreeze)                       // are events being unfrozen?
    {
        ResendEvents();    // yes, resend any pending events
    }

    COleControl::OnFreezeEvents(bFreeze);
}

void CRRRComCtrl::ResendEvents()
{
    // repost any pending events
    m_oEventQueue.RepostEvents(m_hWnd);

    if (m_bCommStateEventPending)
    {
        m_bCommStateEventPending = FALSE;
        FireCommStateChange(m_sCommState);
    }

    if (m_bRadioLevelEventPending)
    {
        m_bRadioLevelEventPending = FALSE;
        FireRadioChange(m_RadioLevel);
    }

    void *Event = NULL;

    do
    {
        Event = NULL;
        CSingleLock Semp(&m_oSemp);
        Semp.Lock();

        if (CDEventArray.GetSize())
        {
            Event = CDEventArray.GetAt(0);
            CDEventArray.RemoveAt(0);
        }

        Semp.Unlock();

        if (Event)
        {
            PostMessage(RRR_LOCAL_COPYDATA, (WPARAM)Event, 0);
        }
    }
    while (Event);
}

void CRRRComCtrl::SendP1(long P1)
{
    m_oCyeSrv.SendP1(P1);
}

void CRRRComCtrl::SendP2(long P2)
{
    m_oCyeSrv.SendP2(P2);
}

void CRRRComCtrl::SendP3(long P3)
{
    m_oCyeSrv.SendP3(P3);
}

void CRRRComCtrl::SendP4(long P4)
{
    m_oCyeSrv.SendP4(P4);
}

void CRRRComCtrl::SetBuzzerFrequency(short BuzzerFrequency)
{
    m_oCyeSrv.SendBuzzerFrequency(BuzzerFrequency);
}

void CRRRComCtrl::SetBuzzerOn(BOOL BuzzerOn)
{
    m_oCyeSrv.SendBuzzerOn(BuzzerOn);
}

void CRRRComCtrl::FollowPath(short Speed)
{
    m_oCyeSrv.SendFollowPath(Speed);
}

void CRRRComCtrl::SetHandleLength(short HandleLength)
{
    m_oCyeSrv.SendHandleLength(HandleLength);
}

void CRRRComCtrl::SetMotorControlConstants(short Kp, short Ki, short Kd, short Kb, short Kpwm)
{
    m_oCyeSrv.SendMotorControlConstants(Kp, Ki, Kd, Kb, Kpwm);
}

void CRRRComCtrl::MoveCurve(double Heading, short Speed, short Diameter)
{
    m_oCyeSrv.SendTurn(Heading, Speed, Diameter);
}

void CRRRComCtrl::MoveStraight(short Speed, double Heading)
{
    m_oCyeSrv.SendStraightVelocity(Speed, Heading);
}

void CRRRComCtrl::SetMaxSpeed(short MaxSpeed)
{
    m_oCyeSrv.SendMaxSpeed(MaxSpeed);
}

void CRRRComCtrl::SetWheelVelocities(short RightVelocity, short LeftVelocity)
{
    m_oCyeSrv.SendMotorVelocities(RightVelocity, LeftVelocity, HIGH_PRIORITY);
}

void CRRRComCtrl::PurgeRingBuffer()
{
    m_oCyeSrv.PurgeRingBuffer();
}

void CRRRComCtrl::SetObstacleSensitivity(short Sensitivity)
{
    m_oCyeSrv.SendSensitiveObstacleDetection(Sensitivity);
}

void CRRRComCtrl::MoveTo(double x, double y, short Speed)
{
    m_oCyeSrv.SendPositionVelocityDestination(x, y, Speed, HIGH_PRIORITY);
}

void CRRRComCtrl::SetPosition(long x, long y)
{
    m_oCyeSrv.SendPosition(x, y);
}

void CRRRComCtrl::SendPositionDestination(double x, double y)
{
    m_oCyeSrv.SendPositionDestination(x, y);
}

void CRRRComCtrl::LoadPathSegment(BOOL PurgeOld, LPDISPATCH PathObject)
{
    m_oCyeSrv.ExecutePath(PurgeOld, PathObject);
}

void CRRRComCtrl::SendP5(long P5)
{
    m_oCyeSrv.SendP5(P5);
}

void CRRRComCtrl::SendP6(long P6)
{
    m_oCyeSrv.SendP6(P6);
}

void CRRRComCtrl::SendP7(long P7)
{
    m_oCyeSrv.SendP7(P7);
}

void CRRRComCtrl::SendP8(long P8)
{
    m_oCyeSrv.SendP8(P8);
}

void CRRRComCtrl::MoveDirection(short Speed, double Heading)
{
    m_oCyeSrv.SendVelocityDirection(Speed, Heading, HIGH_PRIORITY);
}

void CRRRComCtrl::OnResponseTimeoutChanged()
{
    // Send the change down to the lower level
    m_oCyeSrv.SetResponseTimeout(m_responseTimeout);
    // get back the adjusted value...
    m_responseTimeout = m_oCyeSrv.GetResponseTimeout();
    SetModifiedFlag();
}

void CRRRComCtrl::SendInformationToTransmit(long Mask)
{
    m_oCyeSrv.SendInformationToTransmit(Mask);
}

void CRRRComCtrl::RequestState()
{
    m_oCyeSrv.SendRequestState();
}

void CRRRComCtrl::PowerDown()
{
    m_oCyeSrv.PowerDown();
}

void CRRRComCtrl::SendMessage(const VARIANT FAR& MsgData)
{
    // First make sure it's a SafeArray
    if (MsgData.vt != (VT_ARRAY | VT_UI1))
    {
        ThrowError(CTL_E_ILLEGALFUNCTIONCALL,
                   "Invalid VARIANT type passed to SendMessage. Expected VT_ARRAY | VT_UI1.");
    }
    else
    {
        // now verify the array
        COleSafeArray array(MsgData);

        if (array.GetDim() != 1)
        {
            ThrowError(CTL_E_ILLEGALFUNCTIONCALL,
                       "Invalid VARIANT SafeArray dimensions passed to SendMessage. Expected single dimensional array.");
        }
        else
        {
            DWORD Size = array.GetOneDimSize();

            if (Size < 1)
            {
                ThrowError(CTL_E_ILLEGALFUNCTIONCALL,
                           "Empty VARIANT SafeArray passed to SendMessage.");
            }
            else
            {
                // everything seems fine, send the message
                m_oCyeSrv.SendMessage(MsgData);
            }
        }
    }
}

long CRRRComCtrl::GetRetryLimit()
{
    m_retryLimit = m_oCyeSrv.GetRetryLimit();
    return m_retryLimit;
}

void CRRRComCtrl::SetRetryLimit(long nNewValue)
{
    m_oCyeSrv.SetRetryLimit(nNewValue);
    m_retryLimit = m_oCyeSrv.GetRetryLimit();
    SetModifiedFlag();
}

BSTR CRRRComCtrl::GetCommPort()
{
    CString strResult = m_oCyeSrv.GetPort();
    return strResult.AllocSysString();
}

void CRRRComCtrl::SetCommPort(LPCTSTR NewPort)
{
    // don't do anything if requesting the same port
    if (m_oCyeSrv.GetPort() != NewPort)
    {
        DetachControl();                 // disconnect this control
        m_oCyeSrv.SetPort(NewPort);      // setup the new port
        AttachControl();                 // reconnect the control
    }
}

short CRRRComCtrl::GetCommState()
{
    return m_sCommState;
}

void CRRRComCtrl::SetCommState(short nNewValue)
{
    SetNotSupported();
}

float CRRRComCtrl::GetRightCurrent()
{
    return m_RightCurrent;
}

void CRRRComCtrl::SetRightCurrent(float nNewValue)
{
    SetNotSupported();
}

float CRRRComCtrl::GetLeftCurrent()
{
    return m_LeftCurrent;
}

void CRRRComCtrl::SetLeftCurrent(float nNewValue)
{
    SetNotSupported();
}

short CRRRComCtrl::GetLeftVelocity()
{
    return m_LeftVelocity;
}

void CRRRComCtrl::SetLeftVelocity(short nNewValue)
{
    SetNotSupported();
}

short CRRRComCtrl::GetRightVelocity()
{
    return m_RightVelocity;
}

void CRRRComCtrl::SetRightVelocity(short nNewValue)
{
    SetNotSupported();
}

void CRRRComCtrl::SquareToWall(double Heading, float TravelTime, long SlipperySurface)
{
    m_oCyeSrv.SquareToWall(Heading, TravelTime, SlipperySurface);
}

void CRRRComCtrl::SetFeelingMotor(short Motor)
{
    m_oCyeSrv.SetFeelingMotor(Motor);
}

void CRRRComCtrl::CornerCalibrate(long X, long Y, double HO, short FeelWheel, float TravelTime, BOOL SlipperySurface)
{
    m_oCyeSrv.CornerCalibrate(X, Y, HO, FeelWheel, TravelTime, SlipperySurface);
}

void CRRRComCtrl::FindHomeBase(double CornerHeading, double WallHeading, long HomeBaseLocationX, long HomeBaseLocationY, BOOL NeedToGetInFrontOfHomeBase)
{
    m_oCyeSrv.FindHomeBase(CornerHeading, WallHeading, HomeBaseLocationX, HomeBaseLocationY, NeedToGetInFrontOfHomeBase);
}

void CRRRComCtrl::SetHeadingDestination(double Heading, short Speed, short MotorEnable)
{
    m_oCyeSrv.SetHeadingDestination(Heading, Speed, MotorEnable);
}

void CRRRComCtrl::SetEnableObstacleBounce(BOOL Enable)
{
    m_oCyeSrv.SetEnableObstacleBounce(Enable);
}

void CRRRComCtrl::SetRAMData(long Address, const VARIANT FAR& Data)
{
    m_oCyeSrv.SetRAMData(Address, Data);
}

void CRRRComCtrl::SetRobotOnSeconds(long Seconds)
{
    m_oCyeSrv.SetRobotOnSeconds(Seconds);
}

void CRRRComCtrl::SetIntermittentBeep(BOOL On)
{
    m_oCyeSrv.SetIntermittentBeep(On);
}

void CRRRComCtrl::AddToPosition(double DeltaX, double DeltaY)
{
    m_oCyeSrv.AddToPosition(DeltaX, DeltaY);
}

void CRRRComCtrl::AddToHeading(double Delta)
{
    m_oCyeSrv.AddToHeading(Delta);
}

void CRRRComCtrl::SetExternalOutput(BOOL On)
{
    m_oCyeSrv.SetExternalOutput(On);
}

void CRRRComCtrl::RequestExternalInput()
{
    m_oCyeSrv.RequestEX2Value();
}
