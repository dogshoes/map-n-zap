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
// Module:     RRRComm.h
// Abstract:   Serial communications class header file.
// Author:     J.C.H
// Copyright 1997 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RRRCList.h"
#include "RRRMsg.h"
#include "RRRConst.h"

class RRRCom : public CObject
{
        DECLARE_DYNAMIC(RRRCom)

    public:
        RRRCom(HWND hMainWnd, short address, LPCTSTR portname, long BaudRate, BOOL EnableDTR, long DTROnDelay, long DTROffDelay, short Encoding, BOOL FullDuplex, BOOL InvertDTR);
        ~RRRCom();

        // Attributes
    public:
        enum COMM_CHAR_CONSTANTS
        {
            STX = 2,                         // start of text
            ETX = 3,                         // end of text
            FLAG = 0xFF,                     // flag character
            ESC = 27,                        // escape character
            ESC_STX = '1',
            ESC_ETX = '2',
            PRE_FLAG = 2,                    // num flag chars at start of message
            POST_FLAG = 1,                   // num flag chars at end of message
        };

        enum ENCODING_METHODS
        {
            CHECKSUM_ENCODING = 0,           // Checksum based message encoding
            CRC_ENCODING = 1,                // CRC based message encoding
        };

        // all times in milliseconds
        enum COMM_TIME_CONSTANTS
        {
            XMIT_ENABLE_TIME = 5,            // time to enable transmitter
            FULL_TRIP_INTERVAL = 80,         // total time for send and receive pair
            ONE_WAY_INTERVAL = 40,           // time allowed for a single send
            CONTACT_LOST_INTERVAL = 500,     // total time allowed before connection assumed down
            SINGLE_CHAR_TIME = 1,            // time in milliseconds for a single character
            LOST_SYNC_COUNT = 100,           // max number of time intervals allowed with no comm
            SYNC_NEEDED_COUNT = 40,          // idle time interval before sync is sent
            RESPONSE_TIME = 100,             // total time allowed for sender to respond
            MIN_RESPONSE_TIME = 40,          // minimum total time allowed for sender to respond
            MAX_RESPONSE_TIME = 10000,       // maximum total time allowed for sender to respond
            MAX_RETRIES = 7,                 // maximum number of allowed send retries
        };

        // command message numbers
        enum COMM_COMMANDS
        {
            CMD_POLL = 1,                    // poll command message
            CMD_VELOCITY = 2,                // robot velocity update
        };

        // Operations
    public:

        // Overrides

        // Implementation
    protected:

        HANDLE m_hExitThread;               // Event to force thread to exit
        HANDLE m_hThreadDead;               // Event to single thread death
        HANDLE m_hTThreadDead;              // Event for timer thread death
        HANDLE m_hMsgThreadDead;            // Event for Message timer thread death
        CWinThread *m_tSThread;             // serial interface thread
        CWinThread *m_tTThread;             // timer thread
        CWinThread *m_tMsgThread;           // Message timer thread
        long m_lNumConnects;                // number of connections made
        RRRCList m_oCList;                  // connection list object
        HANDLE m_hComm;                     // communications port handle
        CString m_sPort;                    // communications port name
        unsigned long m_lInChars;           // input character counter
        unsigned long m_lOutChars;          // output character counter
        unsigned long m_lReadErrors;        // read error counter
        unsigned long m_lWriteErrors;       // write error counter
        unsigned long m_lSkippedWrites;     // write skip counter
        unsigned long m_lSkippedReads;      // read skip counter
        BOOL m_bDSRState;                   // current DSR state
        RRRMsg m_sRcvBuffer;                // receive buffer
        HWND m_hSimWhnd;                    // simulation application window handle
        HWND m_hLocalWhnd;                  // local simulation window handle (MainFrame)
        long m_BaudRate;                    // communications interface baud rate
        BOOL m_EnableDTR;                   // set to TRUE to enable DTR toggle on send
        DWORD m_DTROnDelay;                 // sleep delay after turning on DTR
        DWORD m_DTROffDelay;                // sleep delay before turning off DTR
        DWORD m_usPerChar;                  // Microseconds per character sent
        short m_Encoding;                   // type of encoding method used per message
        BOOL m_FullDuplex;                  // set to FALSE to enable polling
        HWND m_hMainWnd;                    // main window handle
        short m_MyAddress;                  // local communications address
        COMM_STATES m_CommState;            // current protocol interface state
        COMM_STATES m_OldCommState;         // old protocol interface state
        BOOL m_InvertDTR;                   // set to TRUE to invert DTR in DTR control mode
        //long m_lRetryLimit;                 // maximum number of allowed retries
        DWORD m_lTimerDuration;             // inter character timout duration
        DWORD m_lMsgTimerDuration;          // message timout duration
        long m_lCommResets;                 // number of times communications has been reset
        short m_PreFlag;                    // number of pre flag characters per transmission
        short m_PostFlag;                   // number of post flag characters per transmission
        BOOL m_bResponsePending;            // set to TRUE when a response is pending
        RRRMsg m_oSndMsg;                   // outgoing message bugger


        // Parse input state values for Checksum encoding

        unsigned char PICheckSum;
        unsigned char PIStrLength;
        enum PISTATES
        {
            PISTX,                           // Looking for start of message
            PILEN,                           // Looking for message length
            PIGETCHARS,                      // buffering message characters
        } PIState;
        CTime PILastCharTime;               // time last character received

        CSemaphore m_MsgLock;               // message queue lock semaphore
        CArray<RRRMsg, RRRMsg&> m_MsgQueue;
        CArray<RRRMsg, RRRMsg&> m_SimRcvMsgQueue;

        void UpdateModemState(HANDLE hComm);
        void ServiceStatusEvent(HANDLE hComm, DWORD CommEvent);
        RRRMsg EncodeMsg(RRRMsg &Msg);
        BOOL DecodeMsg(const RRRMsg &Msg, RRRMsg &DMsg);
        void ParseInput(const unsigned char *inbuf, DWORD dwRead);
        HANDLE OpenPort();
        virtual void OnCommOpen() {}
        virtual void OnCommClose() {}
        WORD CalcCRC(RRRMsg &pBuffer);
        void SendMsg(RRRMsg &Msg, long ResponseTimeout);
        void StartTimer(long TimerDuration);
        void StopTimer();
        void StartMsgTimer(long TimerDuration);
        void StopMsgTimer();

    public:
        void SetPort(LPCTSTR portname);
        CString GetPort()
        {
            return m_sPort.AllocSysString();
        }
        void EnableSimMsgXfer(HWND m_simWnd, HWND m_localWnd);
        void SimMessageReceived(RRRMsg &Msg);
        HWND GetMainWnd()
        {
            return m_hMainWnd;
        }
        void ResetCommunications()
        {
            SetEvent(m_hResetPort);
        }
        void ShutdownCommunications();

        UINT SerialThread();                // Main serial interface thread
        UINT TimerThread();                 // inter character timout thread
        UINT MsgTimerThread();              // Message timeout thread

        HANDLE m_hSendMsg;                  // send message event
        HANDLE m_hSimRcvMsg;                // simulate receive message event
        HANDLE m_hSimCommOpen;              // simulate comm open event
        HANDLE m_hResetPort;                // resets the communications interface
        HANDLE m_hTimerCmd;                 // timer thread command
        HANDLE m_hMsgTimerCmd;              // message timer thread command

        friend class ProtocolDiagnostics;
};