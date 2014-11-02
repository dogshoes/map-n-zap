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
// Module:     RRRComm.cpp
// Abstract:   Implementation file for serial communications class.
// Author:     J.C.H
// Copyright 1997 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RRRSrv.h"
#include "RRRComm.h"
#include "RRRConst.h"
#include <fstream>

#ifdef _DEBUG
#define PRINTOUTPUT
#endif

#ifdef PRINTOUTPUT
#include <fstream>
std::ofstream IO("SerialComm (No Transparency, STX or ETX).txt");
std::ofstream RAW("SerialComm (Raw Data).txt");
#endif
/////////////////////////////////////////////////////////////////////////////
// RRRCom

IMPLEMENT_DYNAMIC(RRRCom, CObject)

/////////////////////////////////////////////////////////////////////////////
// RRRCom thread controlling funtion
UINT SerialInt(LPVOID pParam)
{
    RRRCom* rcom = (RRRCom*)pParam;
    UINT status = 1;

    if (rcom == NULL)
    {
        status = 1;
    }
    else
    {
        status = rcom->SerialThread();
    }

    return status;	// thread completed successfully
}

/////////////////////////////////////////////////////////////////////////////
// RRRCom TimerThread inter character timout function
UINT TimerInt(LPVOID pParam)
{
    RRRCom* rcom = (RRRCom*)pParam;
    UINT status = 1;

    if (rcom == NULL)
    {
        status = 1;
    }
    else
    {
        status = rcom->TimerThread();
    }

    return status;	// thread completed successfully
}

/////////////////////////////////////////////////////////////////////////////
// RRRCom MsgTimerThread inter character timout function
UINT MsgTimerInt(LPVOID pParam)
{
    RRRCom* rcom = (RRRCom*)pParam;
    UINT status = 1;

    if (rcom == NULL)
    {
        status = 1;
    }
    else
    {
        status = rcom->MsgTimerThread();
    }

    return status;	// thread completed successfully
}

/////////////////////////////////////////////////////////////////////////////
// RRRCom construction/destruction

RRRCom::RRRCom(HWND hMainWnd, short address, LPCTSTR portname, long BaudRate, BOOL EnableDTR, long DTROnDelay, long DTROffDelay, short Encoding, BOOL FullDuplex, BOOL InvertDTR) :
    m_BaudRate(BaudRate),
    m_EnableDTR(EnableDTR),
    m_DTROnDelay(DTROnDelay),
    m_DTROffDelay(DTROffDelay),
    m_Encoding(Encoding),
    m_FullDuplex(FullDuplex),
    m_hMainWnd(hMainWnd),
    m_MyAddress(address),
    m_InvertDTR(InvertDTR)
{
    // initialize connection counter
    m_lNumConnects = 0;
    m_sPort = portname;
    m_lInChars = 0;                     // zerou character counters
    m_lOutChars = 0;
    m_lReadErrors = 0;                  // zero read error counter
    m_lWriteErrors = 0;                 // zero write error counter
    m_lSkippedWrites = 0;               // zero write skip counter
    m_lSkippedReads = 0;                // zero read skip counter
    m_bDSRState = FALSE;                // init DSR to off
    m_hSimWhnd = 0;                     // disable message based send
    m_hLocalWhnd = 0;
    m_CommState = ISTRT_STATE;          // init state to initiate sequence
    m_OldCommState = ISTRT_STATE;       // init old state to initiate sequence
    m_lCommResets = 0;                  // zero number of resets
    CRRRSrvApp *theApp = (CRRRSrvApp *)AfxGetApp();
    m_PreFlag = theApp->m_PreFlag;      // number of pre flag characters per transmission
    m_PostFlag = theApp->m_PostFlag;    // number of post flag characters per transmission
    m_bResponsePending = FALSE;         // set to TRUE when response message is expected

    PIState = PISTX;                    // initialize parse input state

    //
    // Create all the event handles needed to synchronize
    // the serial interface thread.
    //
    m_hExitThread = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hThreadDead = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hTThreadDead = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hMsgThreadDead = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hSendMsg = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hSimRcvMsg = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hSimCommOpen = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hResetPort = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hTimerCmd = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hMsgTimerCmd = CreateEvent(NULL, FALSE, FALSE, NULL);

    //
    // Start the serial interface thread
    //
    m_tSThread = AfxBeginThread(SerialInt, this);
    // Start the timer thread
    //
    m_tTThread = AfxBeginThread(TimerInt, this);
    // Start the message timer thread
    //
    m_tMsgThread = AfxBeginThread(MsgTimerInt, this);
}

RRRCom::~RRRCom()
{
    // kill the communications thread
    ShutdownCommunications();
}

void RRRCom::ShutdownCommunications()
{
    //
    // Kill the serial interface thread...
    //
    if (m_tSThread)                     // is the thread object valid?
    {
        DWORD ExitCode;                  // is the thread still running?

        if (GetExitCodeThread(m_tSThread->m_hThread, &ExitCode))
        {
            if (ExitCode == STILL_ACTIVE)
            {
                // tell thread to exit
                SetEvent(m_hExitThread);
                // wait for the thread to exit
                WaitForSingleObject(m_hThreadDead, 5000);
                Sleep(50);                    // sleep a little longer JIC
            }
        }

        m_tSThread = NULL;
    }

    //
    // Kill the timer thread...
    //
    if (m_tTThread)                     // is the thread object valid?
    {
        DWORD ExitCode;                  // is the thread still running?

        if (GetExitCodeThread(m_tTThread->m_hThread, &ExitCode))
        {
            if (ExitCode == STILL_ACTIVE)
            {
                // tell thread to exit
                SetEvent(m_hExitThread);
                // wait for the thread to exit
                WaitForSingleObject(m_hTThreadDead, 5000);
                Sleep(50);                    // sleep a little longer JIC
            }
        }

        // mark thread as dead
        m_tTThread = NULL;
    }

    //
    // Kill the message timer thread...
    //
    if (m_tMsgThread)                     // is the thread object valid?
    {
        DWORD ExitCode;                  // is the thread still running?

        if (GetExitCodeThread(m_tMsgThread->m_hThread, &ExitCode))
        {
            if (ExitCode == STILL_ACTIVE)
            {
                // tell thread to exit
                SetEvent(m_hExitThread);
                // wait for the thread to exit
                WaitForSingleObject(m_hMsgThreadDead, 5000);
                Sleep(50);                    // sleep a little longer JIC
            }
        }

        // mark thread as dead
        m_tMsgThread = NULL;
    }
}

void RRRCom::SetPort(LPCTSTR portname)
{
    m_sPort = portname;
}

///////////////////////////////////////////////////////////////////////////////
//
// CalcCRC(void *pBuffer, short ulSize)
//
//   Abstract:  Calculates a buffer's CRC value.
//
// Parameters:
//    Name        I/O   Default     Description
//    -------------------------------------------------------------------------
//    pBuffer      in               The source buffer address
//    ulSize       in               Number of characters in source buffer
//
//    Returns:  WORD - buffer's CRC value
//
//
///////////////////////////////////////////////////////////////////////////////

WORD RRRCom::CalcCRC(RRRMsg &pBuffer)
{
    WORD wCRC16a[16] =
    {
        0000000,	0140301,	0140601,	0000500,
        0141401,	0001700,	0001200,	0141101,
        0143001,	0003300,	0003600,	0143501,
        0002400,	0142701,	0142201,	0002100,
    };
    WORD wCRC16b[16] =
    {
        0000000,	0146001,	0154001,	0012000,
        0170001,	0036000,	0024000,	0162001,
        0120001,	0066000,	0074000,	0132001,
        0050000,	0116001,	0104001,	0043000,
    };

    PBYTE	pb;
    BYTE	bTmp;
    WORD   pulSeed = 0;
    short ulSize = pBuffer.GetLength();

    for (pb = (BYTE *)pBuffer.GetBuffer(1); ulSize; ulSize--, pb++)
    {
        bTmp = (BYTE)(((WORD) * pb) ^ (pulSeed));	// Xor CRC with new char
        pulSeed = ((pulSeed) >> 8) ^ wCRC16a[bTmp & 0x0F] ^ wCRC16b[bTmp >> 4];
    }

    return pulSeed;
}

RRRMsg RRRCom::EncodeMsg(RRRMsg &Msg)
{
    RRRMsg EMsg;

    // encode message using selected encoding scheme
    switch (m_Encoding)
    {
    case CHECKSUM_ENCODING:
    {
        /*Get the length of the string*/
        unsigned char nChars = Msg.GetLength();
        /*Start calculating the CheckSum*/
        unsigned char CheckSum = nChars;
        EMsg.Add(STX);
        EMsg.Add(nChars);

        for (short i = 0; i < nChars; i++)
        {
            EMsg.Add(Msg.GetAt(i));
            CheckSum += Msg.GetAt(i);
        }

        EMsg.Add(CheckSum);
#ifdef OLD
        /*Initialize the transmission string*/
        CString TXString('\000', nChars + 3);
        /*Set the STX char*/
        TXString.SetAt(0, STX);
        /*Set the string length char*/
        TXString.SetAt(1, nChars);

        /*Calculate the check sum and set the rest of the transmission string*/
        for (unsigned char i = 0; i < nChars; i++)
        {
            CheckSum += SerString->GetAt(i);
            TXString.SetAt(i + 2, SerString->GetAt(i));
        }

        /*Set the check sum character*/
        TXString.SetAt(nChars + 2, CheckSum);
        /*Send Out STX + String Length + string + the check sum*/
#endif
    }
    break;

    case CRC_ENCODING:
    {
        RRRMsg NewMsg;
        short i;
        unsigned char Addr;
        unsigned char DestAddr = (unsigned char)Msg.GetDestAddress();
        Addr = (unsigned char)(((m_MyAddress << 4) | (DestAddr & 0xf)) & 0xff);
        NewMsg.Add(Addr);
        unsigned char MsgNum;
        MsgNum = (unsigned char)(((Msg.GetRcvMsgNum() << 4) | (Msg.GetSndMsgNum() & 0xf)) & 0xff);
        NewMsg.Add(MsgNum);
        // append the actual message
        NewMsg += Msg;
        // calculate and add the CRC
        WORD CRC = CalcCRC(NewMsg);
        //RRRMsg NewMsg = Msg;
        NewMsg.Add(char(CRC & 0xff));
        NewMsg.Add(char(CRC >> 8));

        for (i = 0; i < m_PreFlag; i++) // add pre flag characters
        {
            EMsg.Add(FLAG);
        }

        EMsg.Add(STX);                      // place start of text at start of actual message data

        for (i = 0; i < NewMsg.GetLength(); i++) // copy text in using transparency substitution
        {
            switch (NewMsg.GetAt(i))
            {
            case ESC:
                EMsg.Add(ESC);
                EMsg.Add(ESC);
                break;

            case STX:
                EMsg.Add(ESC);
                EMsg.Add(ESC_STX);
                break;

            case ETX:
                EMsg.Add(ESC);
                EMsg.Add(ESC_ETX);
                break;

            default:
                EMsg.Add(NewMsg.GetAt(i));
                break;
            }
        }

        EMsg.Add(ETX);

        for (i = 0; i < m_PostFlag; i++) // add post flag characters
        {
            EMsg.Add(FLAG);
        }

#ifdef PRINTOUTPUT

        for (i = PRE_FLAG; i < EMsg.GetLength() - POST_FLAG; i++)
        {
            RAW.width(3);
            RAW << (int)EMsg.GetAt(i) << " ";
        }

        RAW << "\n";
#endif
    }
    break;                     // end CRC encoding case
    }                             // end message encoding switch

    return EMsg;
}

//
// Message comming into this routine has already had the STX and ETX removed
// by the receive logic...
//
BOOL RRRCom::DecodeMsg(const RRRMsg &Msg, RRRMsg &DMsg)
{
    BOOL Status = TRUE;
#ifdef PRINTOUTPUT
    RAW.width(3);
    RAW << (int)STX << " ";

    for (short j = 0; j < Msg.GetLength(); j++)
    {
        RAW.width(3);
        RAW << (int)Msg.GetAt(j) << " ";
    }

    RAW.width(3);
    RAW << (int)ETX << " ";
    RAW << "\n";
#endif

    for (short i = 0; (i < Msg.GetLength()) && (Status == TRUE); i++)
    {
        switch (Msg.GetAt(i))
        {
        case ESC:
            ++i;

            if (i >= Msg.GetLength())
            {
                Status = FALSE;
            }
            else
            {
                switch (Msg.GetAt(i))
                {
                case ESC:
                    DMsg.Add(ESC);
                    break;

                case ESC_STX:
                    DMsg.Add(STX);
                    break;

                case ESC_ETX:
                    DMsg.Add(ETX);
                    break;

                default:
                    Status = FALSE;
                    break;
                }
            }

            break;

        default:
            DMsg.Add(Msg.GetAt(i));
            break;
        }
    }

    if (Status)
    {
        if (DMsg.GetLength() <= 2)
        {
            Status = FALSE;
        }
        else
        {
            if (CalcCRC(DMsg) != 0)
            {
                DMsg.Empty();
                Status = FALSE;
            }
            else
            {
                // extract the source and destination addresses
                unsigned char Addr = DMsg.GetAt(0);
                DMsg.SetSrcAddress(Addr >> 4);
                DMsg.SetDestAddress(Addr & 0xf);
                // extract the send and receive message numbers
                unsigned char MsgNum = DMsg.GetAt(1);
                DMsg.SetRcvMsgNum(MsgNum >> 4);
                DMsg.SetSndMsgNum(MsgNum & 0xf);
                // strip off the address and message number fields
                DMsg = DMsg.Right(DMsg.GetLength() - 2);
                // strip off the CRC
                DMsg = DMsg.Left(DMsg.GetLength() - 2);
            }
        }
    }

    return Status;                      // return decode status to caller
}

//
// Sift through received characters looking for a message...
// All messages are framed by STX & ETX
//
void RRRCom::ParseInput(const unsigned char *inbuf, DWORD dwRead)
{
    // parse input based on encoding scheme
    switch (m_Encoding)
    {
    case CHECKSUM_ENCODING:
    {
        // make sure parse state is valid
        switch (PIState)
        {
        case PISTX:                // Looking for start of message
        case PILEN:                // Looking for message length
        case PIGETCHARS:           // buffering message characters
            break;

        default:
            PIState = PISTX;
            break;
        }

        // inter character timeout logic
        // if already in the middle of a message
        if (PIState != PISTX)
        {
            // if more than three seconds have elapsed
            CTimeSpan ts = CTime(time(NULL)) - PILastCharTime;

            if (ts.GetTotalSeconds() >= 3)
            {
                // throw away last message and start a new one...
                PIState = PISTX;
            }
        }

        PILastCharTime = time(NULL);

        // now parse the message
        for (unsigned short i = 0; i < dwRead; i++)
        {
            switch (PIState)
            {
            case PISTX:                // Looking for start of message
                if (inbuf[i] == STX)    // if this is a start of message
                {
                    PIState = PILEN;     // start parsing
                    m_sRcvBuffer.Empty();// blank out the receive buffer
                }

                break;

            case PILEN:                // Looking for message length
                PIStrLength = inbuf[i];
                PICheckSum = PIStrLength;
                PIState = PIGETCHARS;   // start parsing
                break;

            case PIGETCHARS:           // buffering message characters
                if (m_sRcvBuffer.GetLength() < PIStrLength)
                {
                    // place character in buffer
                    m_sRcvBuffer.Add(inbuf[i]);
                    // update the check sum
                    PICheckSum += inbuf[i];
                }
                else
                {
                    if (PICheckSum != inbuf[i])
                    {
                        // this is a check sum error
                    }
                    else
                    {
                        // lock the semaphore to protect access
                        CSingleLock lock(&m_MsgLock, TRUE);

                        if (!m_bResponsePending)
                        {
                            // bump the skip counter
                            ++m_lSkippedReads;
                        }
                        else
                        {
                            // stop the pending message timer
                            StopMsgTimer();
                            m_bResponsePending = FALSE;
                            // the message is valid, deal with it
                            // Service the successfully decoded message
                            RRRMsg *Buffer = new RRRMsg(m_sRcvBuffer);
                            PostMessage(m_hMainWnd, RRR_MSG_RECV, (WPARAM)Buffer, m_sRcvBuffer.GetLength() + 1);
                        }

                        // release the message lock semaphore
                        lock.Unlock(1);
                    }

                    // reset parse state to look for more
                    PIState = PISTX;
                }

                break;
            }
        }
    }
    break;                           // end checksum encoding case

    case CRC_ENCODING:
    {
        // make sure parse state is valid
        switch (PIState)
        {
        case PISTX:                // Looking for start of message
        case PIGETCHARS:           // buffering message characters
            break;

        default:
            PIState = PISTX;
            break;
        }

        // inter character timeout logic
        // if already in the middle of a message
        if (PIState != PISTX)
        {
            // if more than three seconds have elapsed
            CTimeSpan ts = CTime(time(NULL)) - PILastCharTime;

            if (ts.GetTotalSeconds() >= 3)
            {
                // throw away last message and start a new one...
                PIState = PISTX;
            }
        }

        PILastCharTime = time(NULL);

        for (unsigned short i = 0; i < dwRead; i++)
        {
            switch (PIState)
            {
            case PISTX:                // Looking for start of message
                if (inbuf[i] == STX)    // if this is a start of message
                {
                    PIState = PIGETCHARS;// start parsing
                    m_sRcvBuffer.Empty();// blank out the receive buffer
                }

                break;

            case PIGETCHARS:           // get characters up to ETX
            {
                switch (inbuf[i])
                {
                case STX:
                    m_sRcvBuffer.Empty();         // blank out the receive buffer
                    break;

                case ETX:
                    if (m_sRcvBuffer.GetLength()) // decode the receive buffer if it has something in it
                    {
                        RRRMsg Msg;                // decode the message

                        if (DecodeMsg(m_sRcvBuffer, Msg))
                        {
                            // don't pay any attention if my own message
                            if (Msg.GetSrcAddress() != m_MyAddress)
                            {
                                // lock the semaphore to protect access
                                CSingleLock lock(&m_MsgLock, TRUE);

                                if (!m_bResponsePending)
                                {
                                    // bump the skip counter
                                    ++m_lSkippedReads;
                                }
                                else
                                {
                                    // Service the successfully decoded message
                                    RRRMsg *Buffer = new RRRMsg(Msg);
                                    PostMessage(m_hMainWnd, RRR_MSG_RECV, (WPARAM)Buffer, m_sRcvBuffer.GetLength() + 1);
                                    m_bResponsePending = FALSE;
                                }

                                // release the message lock semaphore
                                lock.Unlock(1);
                            }
                        }
                    }

                    PIState = PISTX;
                    break;

                default:
                    m_sRcvBuffer.Add(inbuf[i]);   // buffer the message...

                    if (m_sRcvBuffer.BufferFull())// if the buffer is full..
                    {
                        // hang it up and start over
                        m_sRcvBuffer.Empty();      // blank out the receive buffer
                        PIState = PISTX;           // go back to looking for a new message
                    }

                    break;
                }
            }
            break;

            default:
                PIState = PISTX;
                break;
            }
        }
    }
    break;                           // end CRC encoding case
    }                                   // end parse input switch
}

HANDLE RRRCom::OpenPort()
{
    m_sRcvBuffer.Empty();               // blank out the receive buffer
    HANDLE ph = CreateFile(m_sPort,
                           GENERIC_READ | GENERIC_WRITE,
                           0,
                           0,
                           OPEN_EXISTING,
                           FILE_FLAG_OVERLAPPED,
                           0);

    if (ph != INVALID_HANDLE_VALUE)
    {
        DCB dcb = {0};

        dcb.DCBlength = sizeof(dcb);

        //
        // get current DCB settings
        //
        if (GetCommState(ph, &dcb))
        {
            //
            // update DCB rate, byte size, parity, and stop bits size
            //
            switch (m_BaudRate)
            {
            case 19200:
                dcb.BaudRate = CBR_19200;
                m_usPerChar = 500;
                break;

            case 9600:
                dcb.BaudRate = CBR_9600;
                m_usPerChar = 1000;
                break;

            case 4800:
                dcb.BaudRate = CBR_4800;
                m_usPerChar = 2000;
                break;

            case 2400:
                dcb.BaudRate = CBR_2400;
                m_usPerChar = 4000;
                break;

            case 1200:
                dcb.BaudRate = CBR_1200;
                m_usPerChar = 8000;
                break;

            case 300:
                dcb.BaudRate = CBR_300;
                m_usPerChar = 32000;
                break;

            default:
                dcb.BaudRate = CBR_9600;
                m_usPerChar = 1000;
                break;
            }

            dcb.ByteSize = 8;
            dcb.Parity   = NOPARITY;
            dcb.StopBits = ONESTOPBIT;

            //
            // update event flags
            //
            //      if (EVENTFLAGS(TTYInfo) & EV_RXFLAG)
            //         dcb.EvtChar = FLAGCHAR(TTYInfo);
            //      else
            //         dcb.EvtChar = '\0';

            //
            // update flow control settings
            //
            dcb.fDtrControl     = DTR_CONTROL_DISABLE;
            dcb.fRtsControl     = RTS_CONTROL_ENABLE;

            dcb.fOutxCtsFlow    = FALSE;
            dcb.fOutxDsrFlow    = FALSE;
            dcb.fDsrSensitivity = FALSE;
            dcb.fOutX           = FALSE;
            dcb.fInX            = FALSE;
            dcb.fTXContinueOnXoff = TRUE;
            //      dcb.XonChar         = XONCHAR(TTYInfo);
            //      dcb.XoffChar        = XOFFCHAR(TTYInfo);
            //      dcb.XonLim          = XONLIMIT(TTYInfo);
            //      dcb.XoffLim         = XOFFLIMIT(TTYInfo);

            //
            // DCB settings not in the user's control
            //
            dcb.fParity = FALSE;

            //
            // set new state
            //
            if (SetCommState(ph, &dcb))
            {

                UpdateModemState(ph);
            }

            //
            // set new timeouts
            //
            COMMTIMEOUTS ct;
            ct.ReadIntervalTimeout = 8;
            ct.ReadTotalTimeoutMultiplier = 2;
            ct.ReadTotalTimeoutConstant = 50;
            ct.WriteTotalTimeoutMultiplier = 0;
            ct.WriteTotalTimeoutConstant = 0;

            if (!SetCommTimeouts(ph, &ct))
            {
                DWORD LastError = GetLastError();
            }
        }
    }

    return ph;
}

void RRRCom::UpdateModemState(HANDLE hComm)
{
    DWORD ModemState;

    if (hComm)
    {
        if (GetCommModemStatus(hComm, &ModemState))
        {
            if (ModemState & MS_DSR_ON)
            {
                m_bDSRState = TRUE;
            }
            else
            {
                m_bDSRState = FALSE;
            }
        }
    }
}

void RRRCom::ServiceStatusEvent(HANDLE hComm, DWORD CommEvent)
{
    switch (CommEvent)
    {
    case EV_BREAK:	                     // A break was detected on input.
        break;

    case EV_CTS:                        // The CTS (clear-to-send) signal changed state.
        UpdateModemState(hComm);         // update the status of the modem lines
        break;

    case EV_DSR:                        // The DSR (data-set-ready) signal changed state.
        UpdateModemState(hComm);         // update the status of the modem lines
        break;

    case EV_ERR:                        // A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY.
    {
        DWORD Errors;
        COMSTAT Stat;
        ClearCommError(hComm, &Errors, &Stat);
        printf("Comm error detected!");
    }
    break;

    case EV_RING:                       // A ring indicator was detected.
        UpdateModemState(hComm);         // update the status of the modem lines
        break;

    case EV_RLSD:                       // The RLSD (receive-line-signal-detect) signal changed state.
        UpdateModemState(hComm);         // update the status of the modem lines
        break;

    case EV_RXCHAR:                     // A character was received and placed in the input buffer.
        break;

    case EV_RXFLAG:                     // The event character was received and placed in the input buffer. The event character is specified in the device's DCB structure, which is applied to a serial port by using the SetCommState function.
        break;

    case EV_TXEMPTY:                    // The last character in the output buffer was sent.
#ifdef JUNK
        if (m_EnableDTR)
        {
            // yes, sleep for a bit
            Sleep(m_DTROffDelay);

            // Now, turn DTR off
            if (m_InvertDTR)
            {
                EscapeCommFunction(m_hComm, SETDTR);
            }
            else
            {
                EscapeCommFunction(m_hComm, CLRDTR);
            }
        }

#endif
        break;
    }
}

void RRRCom::SendMsg(RRRMsg &Msg, long ResponseTimeout)
{
    if (IsWindow(m_hMainWnd) && Msg.GetLength())
    {
        // lock the semaphore to protect access
        CSingleLock lock(&m_MsgLock, TRUE);

        if (m_bResponsePending)
        {
            // this shouldn't happen, bump the skip counter
            ++m_lSkippedWrites;
        }
        else
        {
            PostMessage(m_hMainWnd, RRR_SENDING, 0, 0);
            // set the response pending flag
            m_bResponsePending = TRUE;
            // save the outgoing message
            m_oSndMsg = Msg;
            // trigger the send
            SetEvent(m_hSendMsg);
            // start the message timer
            StartMsgTimer(ResponseTimeout);
        }

        // release the message lock semaphore
        lock.Unlock(1);
    }
}

void RRRCom::SimMessageReceived(RRRMsg &Msg)
{
    // lock the semaphore to protect access
    CSingleLock lock(&m_MsgLock, TRUE);
    // add the message to the queue
    m_SimRcvMsgQueue.Add(Msg);
    // release the message lock semaphore
    lock.Unlock(1);
    SetEvent(m_hSimRcvMsg);             // set simulate receive message event
}


void RRRCom::EnableSimMsgXfer(HWND m_simWnd, HWND m_localWnd)
{
    m_hSimWhnd = m_simWnd;
    m_hLocalWhnd = m_localWnd;
    SetEvent(m_hSimCommOpen);           // force the comm open event
}

UINT RRRCom::SerialThread()
{
    UINT ExitStatus = 0;
    BOOL ForceExit = FALSE;
    DWORD WaitResult;
    HANDLE m_hComm = INVALID_HANDLE_VALUE;
    BOOL fWaitingOnRead = FALSE;
    OVERLAPPED osReader = {0};
    DWORD dwRead;                       // bytes actually read
    DWORD dwOvRes;                      // overlapped status result
    OVERLAPPED osStatus = {0};          // overlapped structure for status operations
    BOOL fWaitingOnStat = FALSE;        // flag for pending status event
    OVERLAPPED osWrite = {0};           // Overlapped write structure
    DWORD dwWritten;                    // bytes actually written
    DWORD dwCommEvent = 0;              // result from WaitCommEvent
    unsigned char inBuf[128] = {0};     // input buffer
    DWORD LastCommMask = 0xffffffff;
    BOOL Simulate = FALSE;              // set initial simulate state

    // Create the events
    // osReader.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    osStatus.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    HANDLE EventList[] = {m_hExitThread, osReader.hEvent, osStatus.hEvent,
                          osWrite.hEvent, m_hSendMsg, m_hSimRcvMsg, m_hSimCommOpen,
                          m_hResetPort
                         };


    m_hComm = OpenPort();

    //
    // Invalid handle signifies port not available, wait for the setup...
    //
    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        m_hComm = 0;
        fWaitingOnRead = TRUE;           // don't allow post of read request
        fWaitingOnStat = TRUE;           // don't allow post of status event
    }
    else
    {
        OnCommOpen();                    // service the open event
    }

    do
    {
        //
        // had a read been posted
        //
        if (!fWaitingOnRead)
        {
            if (!ReadFile(m_hComm, inBuf, 100, &dwRead, &osReader))
            {
                DWORD LastError = GetLastError();

                if (LastError != ERROR_IO_PENDING)	  // read not delayed?
                {
                    printf("Bogus error: value=%x", LastError);
                }

                fWaitingOnRead = TRUE;
            }
            else                          // read completed immediately
            {
                // wait no more than 500 ms. for next character
                StartTimer(500);
                m_lInChars += dwRead;      // bump the character counter
                ParseInput(inBuf, dwRead); // parse the input
            }
        }

        // DWORD CommMask = EV_TXEMPTY | EV_RXCHAR | EV_DSR;
        DWORD CommMask = EV_DSR | EV_CTS | EV_RING | EV_RLSD | EV_ERR;

        if (CommMask != LastCommMask)
        {
            if (SetCommMask(m_hComm, CommMask))
            {
                LastCommMask = CommMask;
            }
        }

        if (!fWaitingOnStat)
        {
            if (!WaitCommEvent(m_hComm, &dwCommEvent, &osStatus))
            {
                DWORD LastError = GetLastError();

                if (LastError != ERROR_IO_PENDING)	  // Wait not delayed?
                {
                    //               ErrorReporter("WaitCommEvent");
                }
                else
                {
                    fWaitingOnStat = TRUE;
                }
            }
            else
            {
                // WaitCommEvent returned immediately
                ServiceStatusEvent(m_hComm, dwCommEvent);
            }
        }



        // Only enter loop if all pending I/O is ready...
        if (fWaitingOnRead && fWaitingOnStat)
        {
            // Wait for an event to arrive
            WaitResult = MsgWaitForMultipleObjects(8, EventList, FALSE,
                                                   INFINITE, 0);

            // Service the event
            switch (WaitResult)
            {
            case WAIT_OBJECT_0:           // exit thread event
                ForceExit = TRUE;
                break;

            case WAIT_OBJECT_0+1:         // Read event
            {
                if (!GetOverlappedResult(m_hComm, &osReader, &dwRead, FALSE))
                {
                    //                  if (GetLastError() == ERROR_OPERATION_ABORTED)
                    DWORD LastError = GetLastError();
                    ++m_lReadErrors;
                }
                else
                {
                    if (dwRead == 0)
                    {
                        printf("empty message read...");
                    }
                    else
                    {
                        // wait no more than 500 ms. for next character
                        StartTimer(500);
                        // read completed successfully
                        m_lInChars += dwRead;
                        // parse the input
                        ParseInput(inBuf, dwRead);
#ifdef LOOPBACK

                        if (!WriteFile(m_hComm, inBuf, dwRead, &dwWritten, &osWrite))
                        {
                            if (GetLastError() != ERROR_IO_PENDING)
                            {
                                // Write failed
                                dwWritten = 0;
                            }
                        }

#endif
                    }
                }

                ResetEvent(osReader.hEvent);
                fWaitingOnRead = FALSE;
            }
            break;                     // end read event

            case WAIT_OBJECT_0+2:         // Status event
            {
                if (!GetOverlappedResult(m_hComm, &osStatus, &dwOvRes, FALSE))
                {
                    if (GetLastError() == ERROR_OPERATION_ABORTED)
                        ;// UpdateStatus("WaitCommEvent aborted\r\n");
                    else
                        ;//ErrorInComm("GetOverlappedResult (in Reader)");
                }
                else       // status check completed successfully
                {
                    ServiceStatusEvent(m_hComm, dwCommEvent);
                }

                // clear the event semaphore
                ResetEvent(osStatus.hEvent);
                fWaitingOnStat = FALSE;
            }
            break;

            case WAIT_OBJECT_0+3:         // Write complete event
            {
                if (!GetOverlappedResult(m_hComm, &osWrite, &dwWritten, FALSE))
                {
                    dwWritten = 0;
                }
                else
                {
                    m_lOutChars += dwWritten;
                }

                // Clear the write in progress flag
                ResetEvent(osWrite.hEvent);

                if (m_EnableDTR)
                {
                    // yes, sleep for a bit
                    Sleep(m_DTROffDelay);

                    // Now, turn DTR off
                    if (m_InvertDTR)
                    {
                        EscapeCommFunction(m_hComm, SETDTR);
                    }
                    else
                    {
                        EscapeCommFunction(m_hComm, CLRDTR);
                    }
                }
            }
            break;                     // end write complete event

            case WAIT_OBJECT_0+4:         // Send a message
            {
                RRRMsg Msg = m_oSndMsg;
                // encode the message
                Msg = EncodeMsg(Msg);

                if (m_hSimWhnd == 0)
                {
                    // is DTR toggling enabled?
                    if (m_EnableDTR)
                    {
                        // Yes, turn DTR on
                        if (m_InvertDTR)
                        {
                            if (EscapeCommFunction(m_hComm, CLRDTR))
                            {
                                // it worked, let it settle...
                                Sleep(m_DTROnDelay);
                            }
                        }
                        else
                        {
                            if (EscapeCommFunction(m_hComm, SETDTR))
                            {
                                // it worked, let it settle...
                                Sleep(m_DTROnDelay);
                            }
                        }
                    }

                    // send using serial interface
                    if (!WriteFile(m_hComm, Msg.GetBuffer(1), Msg.GetLength(), &dwWritten, &osWrite))
                    {
                        if (GetLastError() != ERROR_IO_PENDING)
                        {
                            // Write failed, need to deal with this...
                            dwWritten = 0;
                        }
                        else
                        {
                            if (m_EnableDTR || (!m_FullDuplex))
                            {
                                // calculate the sleep time in milliseconds per send
                                DWORD SleepTime = m_usPerChar;
                                // calc time in microseconds for whole message
                                SleepTime *= Msg.GetLength();
                                // convert to milliseconds
                                SleepTime /= 1000;
                                // get DTR up for every message
                                Sleep(SleepTime);
                            }
                        }
                    }
                }
                else
                {
                    // use COPYDATA message send
                    COPYDATASTRUCT msg;
                    msg.dwData = 0;
                    msg.cbData = Msg.GetLength();
                    msg.lpData = (void *)Msg.GetBuffer(1);
                    ::SendMessage(m_hSimWhnd, WM_COPYDATA, (WPARAM)m_hLocalWhnd, (LPARAM)&msg);
                }
            }
            break;                     // end send message event

            case WAIT_OBJECT_0+5:         // simulate receive message
            {
                RRRMsg Msg;
                // lock the semaphore to protect access
                CSingleLock lock(&m_MsgLock, TRUE);

                if (m_SimRcvMsgQueue.GetSize())
                {
                    Msg = m_SimRcvMsgQueue[0];
                    m_SimRcvMsgQueue.RemoveAt(0);
                }

                // release the message lock semaphore
                lock.Unlock(1);

                // Only parse the message if there is a message...
                if (Msg.GetLength())
                {
                    ParseInput((const unsigned char *)Msg.GetBuffer(1), Msg.GetLength());
                }
            }
            break;                     // end sim receive message event

            case WAIT_OBJECT_0+6:         // simulate comm open
            {
                Simulate = TRUE;        // set the simulate flag
                OnCommOpen();           // prime the pump
            }
            break;                     // end sim receive message event

            case WAIT_OBJECT_0+7:         // reset communications interface
            {
                ++m_lCommResets;
                CloseHandle(m_hComm);
                m_hComm = OpenPort();

                //
                // Invalid handle signifies port not available, wait for the setup...
                //
                if (m_hComm == INVALID_HANDLE_VALUE)
                {
                    m_hComm = 0;
                    fWaitingOnRead = TRUE;           // don't allow post of read request
                    fWaitingOnStat = TRUE;           // don't allow post of status event
                }
            }
            break;                     // end reset communications interface

            case WAIT_TIMEOUT:            // wait timer expired
                break;

            case WAIT_ABANDONED_0:           // should not happen!
            default:
                ForceExit = TRUE;             // blow out.. something is wrong
                break;
            }
        }

    }
    while (!ForceExit);

    if (m_hComm != INVALID_HANDLE_VALUE)
    {
        OnCommClose();                   // service the close event
        CloseHandle(m_hComm);
    }
    else
    {
        if (Simulate)
        {
            OnCommClose();    // service the close event
        }
    }

    //
    // close event handles
    //
    CloseHandle(osReader.hEvent);
    CloseHandle(osStatus.hEvent);

    SetEvent(m_hThreadDead);            // set the thread dead event
    return ExitStatus;
}

void RRRCom::StartTimer(long TimerDuration)
{
    // save the new timer value
    m_lTimerDuration = TimerDuration;
    // set the event to trigger the timer command
    SetEvent(m_hTimerCmd);
}

void RRRCom::StopTimer()
{
    // save the new timer value
    m_lTimerDuration = INFINITE;
    // set the event to trigger the timer command
    SetEvent(m_hTimerCmd);
}

void RRRCom::StartMsgTimer(long TimerDuration)
{
    // save the new timer value
    m_lMsgTimerDuration = TimerDuration;
    // set the event to trigger the timer command
    SetEvent(m_hMsgTimerCmd);
}

void RRRCom::StopMsgTimer()
{
    // save the new timer value
    m_lMsgTimerDuration = INFINITE;
    // set the event to trigger the timer command
    SetEvent(m_hMsgTimerCmd);
}

UINT RRRCom::TimerThread()
{
    BOOL ForceExit = FALSE;
    DWORD WaitResult;

    HANDLE EventList[] = {m_hExitThread, m_hTimerCmd};

    m_lTimerDuration = INFINITE;

    do
    {
        // Wait for an event to arrive
        WaitResult = MsgWaitForMultipleObjects(2, EventList, FALSE,
                                               m_lTimerDuration, 0);

        // Service the event
        switch (WaitResult)
        {
        case WAIT_OBJECT_0:           // exit thread event
            ForceExit = TRUE;
            break;

        case WAIT_OBJECT_0+1:         // timer command
            // just restart timer with new duration
            break;

        case WAIT_TIMEOUT:
        {
            SetEvent(m_hResetPort);
            m_lTimerDuration = INFINITE;
        }
        break;
        }
    }
    while (!ForceExit);

    SetEvent(m_hTThreadDead);            // set the thread dead event
    return 0;
}

UINT RRRCom::MsgTimerThread()
{
    BOOL ForceExit = FALSE;
    DWORD WaitResult;

    HANDLE EventList[] = {m_hExitThread, m_hMsgTimerCmd};

    m_lMsgTimerDuration = INFINITE;

    do
    {
        // redefine infinity to be 1 second...
        if (m_lMsgTimerDuration > 1000)
        {
            m_lMsgTimerDuration = 1000;
        }

        // Wait for an event to arrive
        WaitResult = MsgWaitForMultipleObjects(2, EventList, FALSE,
                                               m_lMsgTimerDuration, 0);

        // Service the event
        switch (WaitResult)
        {
        case WAIT_OBJECT_0:           // exit thread event
            ForceExit = TRUE;
            break;

        case WAIT_OBJECT_0+1:         // timer command
            // just restart timer with new duration
            break;

        case WAIT_TIMEOUT:
        {
            // lock the semaphore to protect access
            CSingleLock lock(&m_MsgLock, TRUE);
            //if (m_bResponsePending)
            {
                PostMessage(m_hMainWnd, RRR_MSG_TIMEOUT, 0, 0);
            }
            m_bResponsePending = FALSE;
            m_lMsgTimerDuration = INFINITE;
            // release the message lock semaphore
            lock.Unlock(1);
        }
        break;
        }
    }
    while (!ForceExit);

    SetEvent(m_hMsgThreadDead);            // set the thread dead event
    return 0;
}