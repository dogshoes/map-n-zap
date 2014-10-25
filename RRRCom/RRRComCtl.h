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
// RRRComCtl.h : Declaration of the CRRRComCtrl OLE control class.

/////////////////////////////////////////////////////////////////////////////
// CRRRComCtrl : See RRRComCtl.cpp for implementation.

#include "CyeSrv.h"
#include "RRREventQueue.h"
#include "RRRDispID.h"

#define RADIO_IDLE 150.0f

class CRRRComCtrl : public COleControl
{
        DECLARE_DYNCREATE(CRRRComCtrl)

        // Constructor
    public:
        CRRRComCtrl();

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CRRRComCtrl)
    public:
        virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
        virtual void DoPropExchange(CPropExchange* pPX);
        virtual void OnResetState();
        virtual BOOL OnSetExtent(LPSIZEL lpSizeL);
        virtual void OnFreezeEvents(BOOL bFreeze);
        //}}AFX_VIRTUAL

        // Implementation
        enum MY_CONSTANTS
        {
            DEFAULT_ROBOT_ADDRESS = 0,
        };

    protected:
        ~CRRRComCtrl();

        ICyeSrv m_oCyeSrv;                  // server OLE object
        CString m_sVersion;                 // version property string
        CPictureHolder m_pImage;            // control display image
        long m_lConnectId;                  // connection Id
        BOOL EventsFrozen;                  // set to TRUE when events are frozen by container
        RRREventQueue m_oEventQueue;        // event queue
        CPtrArray CDEventArray;             // Copy data pending event array
        CSemaphore m_oSemp;                 // resource control semaphore
        short m_sCommState;                 // current communications state
        BOOL m_bCommStateEventPending;      // set to TRUE when state event pending
        BOOL m_bRadioLevelEventPending;     // set to TRUE when radio level event pending
        float m_RadioLevel;                 // current radio level receive percentage rate (1-100)
        short m_SoundVolume;                // current microphone volume

        long m_X;
        long m_Y;
        double m_Heading;
        short m_RightVelocity;
        short m_LeftVelocity;
        float m_RightCurrent;
        float m_LeftCurrent;
        float m_BatteryVoltage;

        void AttachControl();               // attaches control to OLE server
        void DetachControl();               // detaches from the OLE server
        void ResendEvents();


        DECLARE_OLECREATE_EX(CRRRComCtrl)    // Class factory and guid
        DECLARE_OLETYPELIB(CRRRComCtrl)      // GetTypeInfo
        DECLARE_PROPPAGEIDS(CRRRComCtrl)     // Property page IDs
        DECLARE_OLECTLTYPE(CRRRComCtrl)		// Type name and misc status

        // Message maps
        //{{AFX_MSG(CRRRComCtrl)
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

        // Dispatch maps
        //{{AFX_DISPATCH(CRRRComCtrl)
        //}}AFX_DISPATCH
        long m_responseTimeout;
        long m_retryLimit;
        short m_robotAddress;
        afx_msg void OnResponseTimeoutChanged();
        afx_msg BSTR GetVersion();
        afx_msg void SetVersion(LPCTSTR lpszNewValue);
        afx_msg long GetX();
        afx_msg void SetX(long nNewValue);
        afx_msg long GetY();
        afx_msg void SetY(long nNewValue);
        afx_msg double GetHeading();
        afx_msg void SetHeading(double nNewValue);
        afx_msg short GetRobotAddress();
        afx_msg void SetRobotAddress(short nNewValue);
        afx_msg float GetRadioLevel();
        afx_msg void SetRadioLevel(float newValue);
        afx_msg float GetBatteryLevel();
        afx_msg void SetBatteryLevel(float newValue);
        afx_msg long GetRetryLimit();
        afx_msg void SetRetryLimit(long nNewValue);
        afx_msg float GetRightCurrent();
        afx_msg void SetRightCurrent(float nNewValue);
        afx_msg float GetLeftCurrent();
        afx_msg void SetLeftCurrent(float nNewValue);
        afx_msg short GetLeftVelocity();
        afx_msg void SetLeftVelocity(short nNewValue);
        afx_msg short GetRightVelocity();
        afx_msg void SetRightVelocity(short nNewValue);
        afx_msg void SetHeadingCorrectionFactor(short HeadingCorrectionFactor);
        afx_msg void SetFlipHandle(BOOL FlipHandle);
        afx_msg void SetMaxPWM(short MaxPWM);
        afx_msg void TurnTo(double Heading, short Speed);
        afx_msg void SetPWM(short RightPWM, short LeftPWM);
        afx_msg void Stop();
        afx_msg void Pause();
        afx_msg void Resume();
        afx_msg void SendP1(long P1);
        afx_msg void SendP2(long P2);
        afx_msg void SendP3(long P3);
        afx_msg void SendP4(long P4);
        afx_msg void SetBuzzerFrequency(short BuzzerFrequency);
        afx_msg void SetBuzzerOn(BOOL BuzzerOn);
        afx_msg void FollowPath(short Speed);
        afx_msg void SetHandleLength(short HandleLength);
        afx_msg void SetMotorControlConstants(short Kp, short Ki, short Kd, short Kb, short Kpwm);
        afx_msg void MoveCurve(double Heading, short Speed, short Diameter);
        afx_msg void MoveStraight(short Speed, double Heading);
        afx_msg void SetMaxSpeed(short MaxSpeed);
        afx_msg void SetWheelVelocities(short RightVelocity, short LeftVelocity);
        afx_msg void PurgeRingBuffer();
        afx_msg void SetObstacleSensitivity(short Sensitivity);
        afx_msg void MoveTo(double x, double y, short Speed);
        afx_msg void SetPosition(long x, long y);
        afx_msg void SendPositionDestination(double x, double y);
        afx_msg void LoadPathSegment(BOOL PurgeOld, LPDISPATCH PathObject);
        afx_msg void SendP5(long P5);
        afx_msg void SendP6(long P6);
        afx_msg void SendP7(long P7);
        afx_msg void SendP8(long P8);
        afx_msg void MoveDirection(short Speed, double Heading);
        afx_msg void SendInformationToTransmit(long Mask);
        afx_msg void RequestState();
        afx_msg void PowerDown();
        afx_msg void SendMessage(const VARIANT FAR& MsgData);
        afx_msg void SquareToWall(double Heading, float TravelTime, long SlipperySurface);
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
        afx_msg void RequestExternalInput();
        afx_msg LRESULT OnVelocityChange(WPARAM wParam, LPARAM lParam);
        afx_msg LRESULT OnCopyData(WPARAM wParam, LPARAM lParam);
        afx_msg LRESULT OnLocalCopyData(WPARAM wParam, LPARAM lParam);
        afx_msg LRESULT OnCommStateChange(WPARAM wParam, LPARAM lParam);
        afx_msg LRESULT OnRadioLevelChange(WPARAM wParam, LPARAM lParam);
        afx_msg BSTR GetCommPort();
        afx_msg void SetCommPort(LPCTSTR NewPort);
        afx_msg short GetCommState();
        afx_msg void SetCommState(short nNewValue);
        DECLARE_DISPATCH_MAP()

        afx_msg void AboutBox();

        // Event maps
        //{{AFX_EVENT(CRRRComCtrl)
        //}}AFX_EVENT
        void FireRadioChange(float RadioLevel)
        {
            FireEvent(RRRCTL_RADIOLEVELCHANGE, EVENT_PARAM(VTS_R4), RadioLevel);
        }
        void FireBatteryChange()
        {
            FireEvent(RRRCTL_BATTERYLOW, EVENT_PARAM(VTS_NONE));
        }
        void FirePositionChange(long X, long Y, long Heading)
        {
            FireEvent(RRRCTL_POSITIONCHANGE, EVENT_PARAM(VTS_I4  VTS_I4  VTS_I4), X, Y, Heading);
        }
        void FireObstacleDetected(short RightWheel, short LeftWheel)
        {
            FireEvent(RRRCTL_OBSTACLEDETECTED, EVENT_PARAM(VTS_I2  VTS_I2), RightWheel, LeftWheel);
        }
        void FireNudgeDetected(BOOL RightWheel, BOOL LeftWheel)
        {
            FireEvent(RRRCTL_NUDGEDETECTED, EVENT_PARAM(VTS_BOOL  VTS_BOOL), RightWheel, LeftWheel);
        }
        void FireVelocityChange(short RightWheel, short LeftWheel)
        {
            FireEvent(RRRCTL_VELOCITYCHANGE, EVENT_PARAM(VTS_I2  VTS_I2), RightWheel, LeftWheel);
        }
        void FireAccelerationChange(short RightWheel, short LeftWheel)
        {
            FireEvent(RRRCTL_ACCELERATIONCHANGE, EVENT_PARAM(VTS_I2  VTS_I2), RightWheel, LeftWheel);
        }

        void FireEncoderUpdate(long RightWheel, long LeftWheel)
        {
            FireEvent(RRRCTL_ENCODERUPDATE, EVENT_PARAM(VTS_I4  VTS_I4), RightWheel, LeftWheel);
        }
        void FireAnalogUpdate(short RightWheel, short LeftWheel, short BatteryAD)
        {
            FireEvent(RRRCTL_ANALOGUPDATE, EVENT_PARAM(VTS_I2  VTS_I2  VTS_I2), RightWheel, LeftWheel, BatteryAD);
        }
        void FirePWMUpdate(short RightWheel, short LeftWheel)
        {
            FireEvent(RRRCTL_PWMUPDATE, EVENT_PARAM(VTS_I2  VTS_I2), RightWheel, LeftWheel);
        }
        void FireRingBufferDump(long Value)
        {
            FireEvent(RRRCTL_RINGBUFFERDUMP, EVENT_PARAM(VTS_I4), Value);
        }
        void FireNewPathSegmentRequest()
        {
            FireEvent(RRRCTL_NEWPATHSEGMENTREQUEST, EVENT_PARAM(VTS_NONE));
        }
        void FireRobotError(long Error)
        {
            FireEvent(RRRCTL_ROBOTERROR, EVENT_PARAM(VTS_I4), Error);
        }
        void FireOnHomeBase(short OnHomeBase)
        {
            FireEvent(RRRCTL_ONHOMEBASE, EVENT_PARAM(VTS_I2), OnHomeBase);
        }
        void FireRobotStatusChange(long X, long Y, double Heading, short RightVelocity, short LeftVelocity, float RightCurrent, float LeftCurrent, float BatteryVoltage)
        {
            FireEvent(RRRCTL_ROBOTSTATUSCHANGE, EVENT_PARAM(VTS_I4  VTS_I4  VTS_R8  VTS_I2  VTS_I2  VTS_R4  VTS_R4  VTS_R4), X, Y, Heading, RightVelocity, LeftVelocity, RightCurrent, LeftCurrent, BatteryVoltage);
        }
        void FireBatteryChargingStateChange(short State)
        {
            FireEvent(RRRCTL_BATTERYCHARGINGSTATECHANGE, EVENT_PARAM(VTS_I2), State);
        }
        void FireUnknownMessage(const VARIANT FAR& Buffer)
        {
            FireEvent(RRRCTL_UNKNOWNMESSAGE, EVENT_PARAM(VTS_VARIANT), &Buffer);
        }
        void FireCommStateChange(short NewState)
        {
            FireEvent(RRRCTL_COMMSTATECHANGE, EVENT_PARAM(VTS_I2), NewState);
        }
        void FireTaskResult(short Result, long X, long Y, double Heading)
        {
            FireEvent(RRRCTL_TASKRESULT, EVENT_PARAM(VTS_I2  VTS_I4  VTS_I4  VTS_R8), Result, X, Y, Heading);
        }
        void FireSoftwareRevNumber(long RevNumber)
        {
            FireEvent(RRRCTL_SOFTWAREREVNUMBER, EVENT_PARAM(VTS_I4), RevNumber);
        }
        void FireRobotOn()
        {
            FireEvent(RRRCTL_ROBOTON, EVENT_PARAM(VTS_NONE));
        }
        void FireRobotOnTime(float SecondsOn)
        {
            FireEvent(RRRCTL_ROBOTONTIME, EVENT_PARAM(VTS_R4), SecondsOn);
        }
        void FireExternalInput(float Voltage)
        {
            FireEvent(RRRCTL_EXTERNALINPUT, EVENT_PARAM(VTS_R4), Voltage);
        }
        void FireMicrophoneVolume(short Volume)
        {
            FireEvent(RRRCTL_MICROPHONEVOLUME, EVENT_PARAM(VTS_I2), Volume);
        }
        DECLARE_EVENT_MAP()

        // Dispatch and event IDs
    public:
        enum
        {
            //{{AFX_DISP_ID(CRRRComCtrl)
            //}}AFX_DISP_ID
        };
};
