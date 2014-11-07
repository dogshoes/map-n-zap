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
// Module:     RRRComEngine.h
// Abstract:   RRRSrv protocol engine class header file.
// Author:     J.C.H
// Copyright 1999 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

#pragma once

class RRRCList;

class RRRComEngine : public CObject
{
        DECLARE_DYNAMIC(RRRComEngine)

    public:
        RRRComEngine(short address, RRRCList *cList, HWND hMainWnd, HWND hLocalWnd, short Encoding, BOOL FullDuplex);
        ~RRRComEngine();

        void OnMessageReceived(RRRMsg &Buffer, short RawMsgLen);
        void SendNextMsg(RRRMsg &Msg);
        void QueueMsg(RRRMsg &Msg);
        void SetResponseTime(long newTime);
        long GetResponseTime();
        void ServiceReceiveTimeout();
        void ServiceReceivedMessage(RRRMsg &Buffer);

        short GetCommState()
        {
            return m_CommState;
        }
        void GetCommStatistics(long &RetryLimit, long &ISTRTRecv, long &ISTRTSent,
                               long &ASTRTRecv, long &ASTRTSent, long &AckRecv, long &PollsSent,
                               long &MsgRecv, long &MsgSent, long &SendRetries, long &TotalSendRetries,
                               unsigned char &RcvMsgNum, unsigned char &SndMsgNum);
        long GetRetryLimit()
        {
            return m_lRetryLimit;
        }
        void SetRetryLimit(long NewValue)
        {
            if (NewValue >= 0)
            {
                m_lRetryLimit = NewValue;
            }
        }

        // message counters
        //long m_lAcksSent;
        //long m_lNaksSent;
        long m_lPollsSent;
        long m_lSendRetries;
        long m_lTotalSendRetries;
        long m_lISTRTSent;
        long m_lASTRTSent;
        long m_lISTRTRecv;
        long m_lASTRTRecv;
        long m_lMsgSent;
        long m_lMsgRecv;
        long m_lAckRecv;
        long m_RobotSoftwareRevNumber;
        unsigned char ChargeState;
        unsigned char OldChargeState;
        unsigned char RequestPath;
        float ExternalAnalog;
        float OldExternalAnalog;

    protected:
        enum SUCCESS_LEVEL_CONSTANTS
        {
            SUCCESS_ARRAY_LEN = 10,
            SEND_FAILED = 0,
            SEND_SUCCESSFUL = 1,
            SEND_IDLE = 2,
        };
        void OnCommOpen();
        void OnCommClose();
        void OnConnect(long wHnd, LPCTSTR ControlName);
        int BytesToInt(unsigned char Byte0, unsigned char Byte1);
        long int BytesToLong(unsigned char Byte0, unsigned char Byte1, unsigned char Byte2, unsigned char Byte3);
        double RobotToWorldUnits(long RobotUnit);
        void UpdateSuccessLevel(SUCCESS_LEVEL_CONSTANTS LastState);

        short m_RobotAddress;               // associated robot address
        RRRCList *m_oCList;                 // connection list object
        unsigned char m_RcvMsgNum;          // last received message number
        unsigned char m_SndMsgNum;          // last sent message number
        COMM_STATES m_CommState;            // current protocol interface state
        COMM_STATES m_OldCommState;         // old protocol interface state
        HWND m_hLocalWnd;                   // local simulation window handle (MainFrame)
        HWND m_hMainWnd;                    // main window handle
        short m_Encoding;                   // type of encoding method used per message
        BOOL m_FullDuplex;                  // set to FALSE to enable polling
        long m_lRetryLimit;                 // maximum number of allowed retries

        // outgoing protocol high priority message queue
        CArray<RRRMsg, RRRMsg&> m_ProtMsgQueue;
        // outgoing protocol low priority message queue
        CArray<RRRMsg, RRRMsg&> m_ProtLowPriMsgQueue;
        RRRMsg m_ActiveMessage;             // current message being sent by protocol engine
        CByteArray m_SuccessLevel;          // contains response state of last ten sends...
        short m_CurrentSuccessCount;        // contains number of successfully sent messages in array
        short m_CurrentFailCount;           // contains number of unsuccessfully sent messages in array
        unsigned char m_LastMsgSent;        // type of last message to be sent

        friend class ProtocolDiagnostics;
};