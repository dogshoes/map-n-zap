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
// Module:     RRRComEngine.cpp
//
// Abstract:   RRRSrv serial communications class.
//
// Author:     J.C.H
//
// Revisions:
//    $History: RRRComEngine.cpp $
//
// *****************  Version 4  *****************
// User: Harnden      Date: 10/25/99   Time: 10:56p
// Updated in $/RRR/RRRSrv
// Added MicrophoneVolume event. Implemented SetRobotAddress. Removed all
// references to RobotAdress setting in CyeSrv.ini file.
//
// *****************  Version 3  *****************
// User: Harnden      Date: 10/13/99   Time: 10:57p
// Updated in $/RRR/RRRSrv
// Removed invalid reference to RRRCom.h.
//
// *****************  Version 2  *****************
// User: Harnden      Date: 10/12/99   Time: 12:23a
// Updated in $/RRR/RRRSrv
// Added multi robot support.
//
// *****************  Version 1  *****************
// User: Harnden      Date: 10/03/99   Time: 3:10p
// Created in $/RRR/RRRSrv
//
// Copyright 1999 Cycle Time Corp
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RRRSrv.h"

#include "RRRComEngine.h"
#include "Robot Communication.h"
//#include "RRRCom.h"

/////////////////////////////////////////////////////////////////////////////
// RRRComEngine

IMPLEMENT_DYNAMIC(RRRComEngine, CObject)

/////////////////////////////////////////////////////////////////////////////
// RRRCom construction/destruction

RRRComEngine::RRRComEngine(short address, RRRCList *cList, HWND hMainWnd, HWND hLocalWnd, short Encoding, BOOL FullDuplex)
    : m_RobotAddress(address)
    , m_hMainWnd(hMainWnd)
    , m_hLocalWnd(hLocalWnd)
    , m_Encoding(Encoding)
    , m_FullDuplex(FullDuplex)
{
    m_oCList = cList;
    m_lRetryLimit = RRRCom::MAX_RETRIES;
    m_CommState = ISTRT_STATE;          // init state to initiate sequence
    m_OldCommState = ISTRT_STATE;       // init old state to initiate sequence

    // zero message counters
    //m_lAcksSent = 0;
    //m_lNaksSent = 0;
    m_lPollsSent = 0;
    m_lSendRetries = 0;
    m_lTotalSendRetries = 0;
    m_lISTRTSent = 0;
    m_lASTRTSent = 0;
    m_lISTRTRecv = 0;
    m_lASTRTRecv = 0;
    m_lMsgSent = 0;
    m_lMsgRecv = 0;
    m_lAckRecv = 0;
    m_LastMsgSent = ISTRT_REQUEST;
    // init radio quality status to idle
    m_CurrentSuccessCount = 0;
    m_CurrentFailCount = 0;

    for (short i = 0; i < SUCCESS_ARRAY_LEN; i++)
    {
        m_SuccessLevel.Add(SEND_IDLE);
    }

    // init revision number to wtf...
    m_RobotSoftwareRevNumber = -1;
    ChargeState = 0xff;
    OldChargeState = 0xff;
    RequestPath = false;
    // init analog values to unlikely data
    ExternalAnalog = -10000.0f;
    OldExternalAnalog = -10000.0f;
}

RRRComEngine::~RRRComEngine()
{
}

void RRRComEngine::UpdateSuccessLevel(SUCCESS_LEVEL_CONSTANTS LastState)
{
    // remove oldest state
    m_SuccessLevel.RemoveAt(0);
    m_SuccessLevel.Add(LastState);
    // calculate new success level
    short NewSuccessCount = 0;
    short NewFailCount = 0;

    for (short i = 0; i < m_SuccessLevel.GetSize(); i++)
    {
        switch (m_SuccessLevel.GetAt(i))
        {
        case SEND_SUCCESSFUL:
            ++NewSuccessCount;
            break;

        case SEND_FAILED:
            ++NewFailCount;
            break;
        }
    }

    // see if success level has changed...
    if ((m_CurrentSuccessCount != NewSuccessCount) ||
            (m_CurrentFailCount != NewFailCount))
    {
        m_CurrentSuccessCount = NewSuccessCount;
        m_CurrentFailCount = NewFailCount;
        // send new rate to all clients
        m_oCList->PostAllClients(RRR_RADIOLEVELCHANGE, m_CurrentSuccessCount, m_CurrentFailCount, m_RobotAddress);
    }
}

void RRRComEngine::ServiceReceiveTimeout()
{
    // update the success rate
    switch (m_LastMsgSent)
    {
    case POLL_REQUEST:
        UpdateSuccessLevel(SEND_IDLE);
        break;

    default:
        UpdateSuccessLevel(SEND_FAILED);
        break;
    }
}

/*******************************************************************************
//Created: 04/16/96 S.R.
//Last Revision: 04/16/96 S.R.
//Parameters:
//	Byte0		The low byte of the number
//	Byte1		The high byte of the number
//Return Value:
//	The ingeger made up of the individual bytes in Byte0 and Byte 1.
********************************************************************************
//Description:
//		This function takes two individual bytes and turns them into an integer.
//Method:
//		The function uses the or and bit shift operations to glue the two bytes
//	together.
*******************************************************************************/
int RRRComEngine::BytesToInt(unsigned char Byte0, unsigned char Byte1)
{
    return (((int)Byte0) | (((int)Byte1) << 8));
}

/*******************************************************************************
//Created: 04/16/96 S.R.
//Last Revision: 04/16/96 S.R.
//Parameters:
//	Byte0..Byte3	The individual bytes to join together.  0 is the low order byte.
//Return Value:
//	The long integer made up of the individual bytes in Byte0..Byte3.
********************************************************************************
//Description:
//	This function takes four individual bytes and turns them into a long integer.
//Method:
//		The function uses the or and bit shift operations to glue the four bytes
//	together.
*******************************************************************************/
long int RRRComEngine::BytesToLong(unsigned char Byte0, unsigned char Byte1,
                                   unsigned char Byte2, unsigned char Byte3)
{
    long int Result = Byte0;
    Result |= ((long int)Byte1) << 8;
    Result |= ((long int)Byte2) << 16;
    Result |= ((long int)Byte3) << 24;
    return Result;
}

double RRRComEngine::RobotToWorldUnits(long RobotUnit)
{
    return (RobotUnit * cTPI / cRobotEncoderTicksPerInch);
}


void RRRComEngine::ServiceReceivedMessage(RRRMsg &Buffer)
{
    // static long int ReturnValue[3];
    RRR_CHECKSUM_MSG Msg;
    memset(&Msg, 0, sizeof(Msg));
    Msg.CommandNum = Buffer.GetAt(0);
    BOOL SendMsg = TRUE;
    // save the message number to send acknowledgement
    m_RcvMsgNum = (unsigned char)Buffer.GetSndMsgNum();

    // Service the message
    if (Buffer.GetLength() != 0)
    {
        UpdateSuccessLevel(SEND_SUCCESSFUL);

        switch (Buffer.GetAt(0))
        {
        case cPositionInput:
        {
            static long LastPosition[3] = {0x7fffffff, 0x7fffffff, 0x7fffffff};
            static long NumUpdates = 0;
            static long NumSkips = 0;

            if (Buffer.GetLength() >= 11)
            {
                ++NumUpdates;
                Msg.ReturnValue[0] = (long)RobotToWorldUnits(BytesToLong(Buffer.GetAt(1), Buffer.GetAt(2),
                                     Buffer.GetAt(3), Buffer.GetAt(4)));
                Msg.ReturnValue[1] = (long)RobotToWorldUnits(BytesToLong(Buffer.GetAt(5), Buffer.GetAt(6),
                                     Buffer.GetAt(7), Buffer.GetAt(8)));
                Msg.ReturnValue[2] = BytesToInt(Buffer.GetAt(9), Buffer.GetAt(10));
                SendMsg = FALSE;

                for (short i = 0; i <= 2; i++)
                {
                    if (Msg.ReturnValue[i] != LastPosition[i])
                    {
                        SendMsg = TRUE;
                    }
                }

                if (!SendMsg)
                {
                    ++NumSkips;
                }
            }
        }

            // service the piggyback ack
        if (m_SndMsgNum == Buffer.GetRcvMsgNum())
        {
            m_CommState = IDLE_STATE;
        }

        break;

        /*One char input*/
        case cNudgeInput:
            Msg.ReturnValue[0] = Buffer.GetAt(1);

            // service the piggyback ack
            if (m_SndMsgNum == Buffer.GetRcvMsgNum())
            {
                m_CommState = IDLE_STATE;
            }

            break;

        /*Two char input*/
        case cObstacleInput:
            Msg.ReturnValue[0] = Buffer.GetAt(1);
            Msg.ReturnValue[1] = Buffer.GetAt(2);

            // service the piggyback ack
            if (m_SndMsgNum == Buffer.GetRcvMsgNum())
            {
                m_CommState = IDLE_STATE;
            }

            break;

        case cEncodersInput:
            Msg.ReturnValue[0] = Buffer.GetAt(1);
            Msg.ReturnValue[1] = Buffer.GetAt(2);

            // service the piggyback ack
            if (m_SndMsgNum == Buffer.GetRcvMsgNum())
            {
                m_CommState = IDLE_STATE;
            }

            break;

        /*One long input*/
        case cRingBufferInput:
        case cSoftwareRevNumber:         //Serial input command character for Software Revision Number
        case cRobotError:
            Msg.ReturnValue[0] = BytesToLong(Buffer.GetAt(1), Buffer.GetAt(2),
                                             Buffer.GetAt(3), Buffer.GetAt(4));

            if (Buffer.GetAt(0) == cSoftwareRevNumber)
            {
                // save the robot's software revision number for later decisions
                m_RobotSoftwareRevNumber = Msg.ReturnValue[0];
            }

            // service the piggyback ack
            if (m_SndMsgNum == Buffer.GetRcvMsgNum())
            {
                m_CommState = IDLE_STATE;
            }

            break;

        /*Two int input*/
        case cVelocityInput:
        case cAccelerationInput:
        case cPwmInput:
            Msg.ReturnValue[0] = BytesToInt(Buffer.GetAt(1), Buffer.GetAt(2));
            Msg.ReturnValue[1] = BytesToInt(Buffer.GetAt(3), Buffer.GetAt(4));

            // service the piggyback ack
            if (m_SndMsgNum == Buffer.GetRcvMsgNum())
            {
                m_CommState = IDLE_STATE;
            }

            break;

        /*Three int input*/
        case cAnalogInput:
            Msg.ReturnValue[0] = BytesToInt(Buffer.GetAt(1), Buffer.GetAt(2));
            Msg.ReturnValue[1] = BytesToInt(Buffer.GetAt(3), Buffer.GetAt(4));
            Msg.ReturnValue[2] = BytesToInt(Buffer.GetAt(5), Buffer.GetAt(6));

            // service the piggyback ack
            if (m_SndMsgNum == Buffer.GetRcvMsgNum())
            {
                m_CommState = IDLE_STATE;
            }

            break;

        /*No data input*/
        case cSendNewPathSegment:
        case cLowBatteryMessage:
        case cRobotOn:                   //Serial input command character for Robot On

            // service the piggyback ack
            if (m_SndMsgNum == Buffer.GetRcvMsgNum())
            {
                m_CommState = IDLE_STATE;
            }

            break;

        case cRobotAtHome:
        case cBatteryChargeState:
            Msg.ReturnValue[0] = Buffer.GetAt(1);

            // service the piggyback ack
            if (m_SndMsgNum == Buffer.GetRcvMsgNum())
            {
                m_CommState = IDLE_STATE;
            }

            break;

        case cRobotStatusUpdate:
        {
            Msg.ReturnValue[0] = (long)RobotToWorldUnits(BytesToLong(Buffer.GetAt(1), Buffer.GetAt(2),
                                 Buffer.GetAt(3), Buffer.GetAt(4)));
            Msg.ReturnValue[1] = (long)RobotToWorldUnits(BytesToLong(Buffer.GetAt(5), Buffer.GetAt(6),
                                 Buffer.GetAt(7), Buffer.GetAt(8)));
            // heading
            //Msg.ReturnValue[2] = BytesToInt(Buffer.GetAt(9),Buffer.GetAt(10));
            double Heading = BytesToInt(Buffer.GetAt(9), Buffer.GetAt(10));
            Heading = Heading * 2.0 * pi / cNHeadings;
            Msg.dReturnValue[0] = Heading;
            Msg.ReturnValue[3] = (char)Buffer.GetAt(11);
            Msg.ReturnValue[3] *= 3;
            Msg.ReturnValue[4] = (char)Buffer.GetAt(12);
            Msg.ReturnValue[4] *= 3;
            float BatteryData = Buffer.GetAt(13);
            BatteryData = (BatteryData * 0.01919f) + 9.7f;
            Msg.fReturnValue[0] = BatteryData;
            unsigned char MotorCurrents = Buffer.GetAt(14);
            char M0I = MotorCurrents & 0xf;
            //Updated 4/27/99 S.R.
            // if the sign bit was set, extend the sign
            //           if (MotorCurrents & 8)
            //               M0I |= 0xf0;
            char M1I = MotorCurrents >> 4;
            // if the sign bit was set, extend the sign
            //            if (MotorCurrents & 0x80)
            //               M1I |= 0xf0;
            //Msg.ReturnValue[5] = M0I*61/15;
            float RightCurrent = float(M0I * 61 / 15);
            RightCurrent = RightCurrent / 1023.0f * 50.0f;
            Msg.fReturnValue[1] = RightCurrent;
            //Msg.ReturnValue[6] = M1I*61/15;
            float LeftCurrent = float(M1I * 61 / 15);
            LeftCurrent = LeftCurrent / 1023.0f * 50.0f;
            Msg.fReturnValue[2] = LeftCurrent;

            // check for extra event info from the new firmware
            if (m_RobotSoftwareRevNumber >= 9)
            {
                unsigned int temp = BytesToInt(Buffer.GetAt(15), Buffer.GetAt(16));
                ChargeState = (temp >> 10) & 0x3;
                //            if (!ChargeState) {
                //					ChargeState = false;
                //            }
                RequestPath = (temp >> 12) & 1;

                if (RequestPath)
                {
                    RequestPath = true;
                }

                ExternalAnalog = float(temp & 0x03FF);
                /*S.R. Message needs to include ExternalAnalog, ChargeState and RequestPath*/
                ExternalAnalog = ExternalAnalog / 1023.0f * 5.0f;
            }

            // check for the sound volume byte
            if ((m_RobotSoftwareRevNumber >= 10) && (Buffer.GetLength() >= 18))
            {
                unsigned char SoundVolume = Buffer.GetAt(17);
                Msg.SoundVolume = SoundVolume;
            }

            // service the piggyback ack
            if (m_SndMsgNum == Buffer.GetRcvMsgNum())
            {
                m_CommState = IDLE_STATE;
            }
        }
        break;

        case cTaskResultInput:           //Serial input command character for Task Result
        {
            Msg.ReturnValue[0] = Buffer.GetAt(1);
            //Updated 4/27/99 S.R.
            Msg.ReturnValue[1] = (long)RobotToWorldUnits(BytesToLong(Buffer.GetAt(2), Buffer.GetAt(3), Buffer.GetAt(4), Buffer.GetAt(5)));
            Msg.ReturnValue[2] = (long)RobotToWorldUnits(BytesToLong(Buffer.GetAt(6), Buffer.GetAt(7), Buffer.GetAt(8), Buffer.GetAt(9)));
            double Heading = BytesToInt(Buffer.GetAt(10), Buffer.GetAt(11));
            Heading *= 2.0 * pi / cNHeadings;
            Msg.dReturnValue[0] = Heading;
        }

            // service the piggyback ack
        if (m_SndMsgNum == Buffer.GetRcvMsgNum())
        {
            m_CommState = IDLE_STATE;
        }

        break;

        case cRobotOnSeconds:            //Serial input command character for Robot On Seconds
        {
            float OnTime = float(BytesToLong(Buffer.GetAt(1), Buffer.GetAt(2),
                                             Buffer.GetAt(3), Buffer.GetAt(4)));
            Msg.fReturnValue[0] = OnTime * 0.953f;
        }

            // service the piggyback ack
        if (m_SndMsgNum == Buffer.GetRcvMsgNum())
        {
            m_CommState = IDLE_STATE;
        }

        break;

        case cSendEX2:                   //Serial input command character for Send External Input
        {
            float Voltage = float(BytesToInt(Buffer.GetAt(1), Buffer.GetAt(2)));
            Msg.fReturnValue[0] = Voltage / 1023.0f * 5.0f;
        }

            // service the piggyback ack
        if (m_SndMsgNum == Buffer.GetRcvMsgNum())
        {
            m_CommState = IDLE_STATE;
        }

        break;


        // This section services the protocol messages
        case ISTRT_REQUEST:              // initiate start request received
            m_CommState = ASTRT_STATE;    // set the state to acknowledge the initiate start message
            SendMsg = FALSE;              // don't pass on control messages
            ++m_lISTRTRecv;
            break;

        case ASTRT_REQUEST:              // acknowledge initiate start request received
            m_CommState = IDLE_STATE;     // set the state to idle, we are ready to rock...
            SendMsg = FALSE;              // don't pass on control messages
            ++m_lASTRTRecv;
            break;

        case ACK_REQUEST:                // ack received
            ++m_lAckRecv;

            // is this is the correct message number, set the idle state
            if (m_SndMsgNum == Buffer.GetRcvMsgNum())
            {
                m_CommState = IDLE_STATE;
            }

            SendMsg = FALSE;              // don't pass on control messages
            break;

        default:
        {
            //Msg.CommandNum = cInvalidInputString;
            // bump the received counter
            ++m_lMsgRecv;
            // use COPYDATA message send
            COPYDATASTRUCT msg;
            msg.dwData = RRR_RAWDATA_CDMSG;
            msg.cbData = Buffer.GetLength();
            msg.lpData = (void *)Buffer.GetBufferAddr();
            m_oCList->SendAllClients(WM_COPYDATA, (WPARAM)m_hLocalWnd, (LPARAM)&msg, m_RobotAddress);

            // service the piggyback ack
            if (m_SndMsgNum == Buffer.GetRcvMsgNum())
            {
                m_CommState = IDLE_STATE;
            }
        }
        break;
        }

        if (SendMsg)
        {
            // bump the received counter
            ++m_lMsgRecv;
            // use COPYDATA message send
            COPYDATASTRUCT msg;
            msg.dwData = RRR_CHECKSUM_CDMSG;
            msg.cbData = sizeof(Msg);
            msg.lpData = &Msg;
            m_oCList->SendAllClients(WM_COPYDATA, (WPARAM)m_hLocalWnd, (LPARAM)&msg, m_RobotAddress);

            // check for extra data
            if (m_RobotSoftwareRevNumber >= 9)
            {
                /*When ChargeState changes from charging to off charger, send an on home base message*/
                if (((ChargeState == 1) || (ChargeState == 2)) && ((OldChargeState != 2) && (OldChargeState != 1)))
                {
                    Msg.CommandNum = cRobotAtHome;
                    Msg.ReturnValue[0] = 1;
                    m_oCList->SendAllClients(WM_COPYDATA, (WPARAM)m_hLocalWnd, (LPARAM)&msg, m_RobotAddress);
                }

                /*When ChargeState changes from off charger to on charger, send an off home base message*/
                if ((ChargeState == 0) && (OldChargeState != 0))
                {
                    Msg.CommandNum = cRobotAtHome;
                    Msg.ReturnValue[0] = 0;
                    m_oCList->SendAllClients(WM_COPYDATA, (WPARAM)m_hLocalWnd, (LPARAM)&msg, m_RobotAddress);
                }

                if (ChargeState != OldChargeState)
                {
                    Msg.CommandNum = cBatteryChargeState;
                    Msg.ReturnValue[0] = ChargeState;
                    OldChargeState = ChargeState;
                    m_oCList->SendAllClients(WM_COPYDATA, (WPARAM)m_hLocalWnd, (LPARAM)&msg, m_RobotAddress);
                }

                if (RequestPath)
                {
                    Msg.CommandNum = cSendNewPathSegment;
                    RequestPath = false;
                    m_oCList->SendAllClients(WM_COPYDATA, (WPARAM)m_hLocalWnd, (LPARAM)&msg, m_RobotAddress);
                }

                if (ExternalAnalog != OldExternalAnalog)
                {
                    Msg.CommandNum = cSendEX2;
                    Msg.fReturnValue[0] = ExternalAnalog;
                    OldExternalAnalog = ExternalAnalog;
                    m_oCList->SendAllClients(WM_COPYDATA, (WPARAM)m_hLocalWnd, (LPARAM)&msg, m_RobotAddress);
                }
            }
        }
    }

    if (m_CommState != m_OldCommState)  // check for change in protocol interface state
    {
        // tell all clients the new state
        m_oCList->PostAllClients(RRR_COMMSTATECHANGE, m_CommState, m_OldCommState, m_RobotAddress);
        m_OldCommState = m_CommState;    // save the new state
    }

}

void RRRComEngine::OnMessageReceived(RRRMsg &Buffer, short RawMsgLen)
{
    if (IsWindow(m_hMainWnd))
    {
        PostMessage(m_hMainWnd, RRR_RECEIVING, 0, 0);

        switch (m_Encoding)
        {
        case RRRCom::CHECKSUM_ENCODING:
        {
            ServiceReceivedMessage(Buffer);
        }
        break;

        case RRRCom::CRC_ENCODING:
        {
            // service the received message
            ServiceReceivedMessage(Buffer);
        }
        break;                           // End CRC encoding case
        }
    }
}

void RRRComEngine::OnCommOpen()
{
}

void RRRComEngine::OnCommClose()
{
}

void RRRComEngine::OnConnect(long wHnd, LPCTSTR ControlName)
{
    // tell this client what the current comm state is...
    PostMessage((HWND)wHnd, RRR_COMMSTATECHANGE, m_CommState, m_OldCommState);
}

void RRRComEngine::SendNextMsg(RRRMsg &Msg)
{
    Msg.SetDestAddress(m_RobotAddress);

    switch (m_CommState)
    {
    case ISTRT_STATE:                   // initiating startup sequence
        m_RcvMsgNum = 0;                 // zero last received message number
        m_SndMsgNum = 0;                 // zero last sent message number
        Msg.Add(ISTRT_REQUEST);          // send the initiate start request
        ++m_lISTRTSent;
        break;

    case ASTRT_STATE:                   // acknowledge start sequence
        m_RcvMsgNum = 0;                 // zero last received message number
        m_SndMsgNum = 0;                 // zero last sent message number
        Msg.Add(ASTRT_REQUEST);          // send the acknowledge start message
        m_CommState = IDLE_STATE;        // set the state to idle
        ++m_lASTRTSent;
        break;

    case IDLE_STATE:                    // connected and operational state

        // is there any outgoing high priority data to be sent?
        if (m_ProtMsgQueue.GetSize())
        {
            ++m_lMsgSent;
            // yes, grab the first message
            Msg = m_ProtMsgQueue[0];
            m_ProtMsgQueue.RemoveAt(0);
            // bump the message counter
            ++m_SndMsgNum;
            // keep message counter down to 4 bits
            m_SndMsgNum &= 0xf;
            m_ActiveMessage = Msg;
            // set state to waiting for ack
            m_CommState = ACK_WAIT_STATE;
            m_lSendRetries = 0;           // zero the retry counter
        }
        else
        {
            // is there any outgoing low priority data to be sent?
            if (m_ProtLowPriMsgQueue.GetSize())
            {
                ++m_lMsgSent;
                // yes, grab the first message
                Msg = m_ProtLowPriMsgQueue[0];
                m_ProtLowPriMsgQueue.RemoveAt(0);
                // bump the message counter
                ++m_SndMsgNum;
                // keep message counter down to 4 bits
                m_SndMsgNum &= 0xf;
                m_ActiveMessage = Msg;
                // set state to waiting for ack
                m_CommState = ACK_WAIT_STATE;
                m_lSendRetries = 0;           // zero the retry counter
            }
            else
            {
                // no data to be sent, just send a poll message
                ++m_lPollsSent;
                Msg.Add(POLL_REQUEST);
            }
        }

        break;

    case ACK_WAIT_STATE:                // waiting for receipt acknowledge
        ++m_lSendRetries;
        ++m_lTotalSendRetries;

        if ((m_lSendRetries > m_lRetryLimit) && (m_lRetryLimit > 0))
        {
            // blow back into ISTRT state, communications link is lost
            m_CommState = ISTRT_STATE;    // set the state to initiate start
            m_RcvMsgNum = 0;              // zero last received message number
            m_SndMsgNum = 0;              // zero last sent message number
            Msg.Add(ISTRT_REQUEST);       // send the initiate start request
            ++m_lISTRTSent;
            m_ProtMsgQueue.RemoveAll();   // empty the pending message queues
            m_ProtLowPriMsgQueue.RemoveAll();
        }
        else
        {
            // retry limit not exceeded
            // was the last message a high priority message?
            if (m_ActiveMessage.HighPriority())
            {
                // yes, resend the last message
                Msg = m_ActiveMessage;
            }
            else
            {
                // is there any outgoing high priority data to be sent?
                if (m_ProtMsgQueue.GetSize())
                {
                    ++m_lMsgSent;
                    // yes, grab the first message
                    Msg = m_ProtMsgQueue[0];
                    m_ProtMsgQueue.RemoveAt(0);
                    // bump the message counter
                    ++m_SndMsgNum;
                    // keep message counter down to 4 bits
                    m_SndMsgNum &= 0xf;
                    m_ActiveMessage = Msg;
                    // set state to waiting for ack
                    m_CommState = ACK_WAIT_STATE;
                    m_lSendRetries = 0;           // zero the retry counter
                }
                else
                {
                    // is there any outgoing low priority data to be sent?
                    if (m_ProtLowPriMsgQueue.GetSize())
                    {
                        ++m_lMsgSent;
                        // yes, grab the first message
                        Msg = m_ProtLowPriMsgQueue[0];
                        m_ProtLowPriMsgQueue.RemoveAt(0);
                        // bump the message counter
                        ++m_SndMsgNum;
                        // keep message counter down to 4 bits
                        m_SndMsgNum &= 0xf;
                        m_ActiveMessage = Msg;
                        // set state to waiting for ack
                        m_CommState = ACK_WAIT_STATE;
                        m_lSendRetries = 0;           // zero the retry counter
                    }
                    else
                    {
                        // no new data, so resend the last message
                        Msg = m_ActiveMessage;
                    }
                }
            }
        }

        break;
    }

    // Save the last message type sent
    m_LastMsgSent = Msg.GetAt(0);
    Msg.SetRcvMsgNum(m_RcvMsgNum);
    Msg.SetSndMsgNum(m_SndMsgNum);
}

void RRRComEngine::QueueMsg(RRRMsg &Msg)
{
    // make sure this is going to local robot
    Msg.SetDestAddress(m_RobotAddress);

    switch (m_Encoding)
    {
    case RRRCom::CRC_ENCODING:
    {
        // is polling enabled?
        if (!m_FullDuplex)
        {
            // yes, queue the message locally and let protocol
            // decide when to send it...
            // but, only send message when link is established...
            // otherwise, through message away
            if ((m_CommState == IDLE_STATE) || (m_CommState == ACK_WAIT_STATE))
            {
                if (Msg.HighPriority())
                {
                    m_ProtMsgQueue.Add(Msg);
                }
                else
                {
                    m_ProtLowPriMsgQueue.Add(Msg);
                }
            }
        }
    }
    break;                           // End CRC encoding case
    }
}

void RRRComEngine::GetCommStatistics(long &RetryLimit,
                                     long &ISTRTRecv, long &ISTRTSent, long &ASTRTRecv, long &ASTRTSent,
                                     long &AckRecv, long &PollsSent, long &MsgRecv, long &MsgSent,
                                     long &SendRetries, long &TotalSendRetries,
                                     unsigned char &RcvMsgNum, unsigned char &SndMsgNum)
{
    RetryLimit = m_lRetryLimit;
    ISTRTRecv = m_lISTRTRecv;
    ISTRTSent = m_lISTRTSent;
    ASTRTRecv = m_lASTRTRecv;
    ASTRTSent = m_lASTRTSent;
    AckRecv = m_lAckRecv;
    PollsSent = m_lPollsSent;
    MsgRecv = m_lMsgRecv;
    MsgSent = m_lMsgSent;
    SendRetries = m_lSendRetries;
    TotalSendRetries = m_lTotalSendRetries;
    RcvMsgNum = m_RcvMsgNum;
    SndMsgNum = m_SndMsgNum;
}
