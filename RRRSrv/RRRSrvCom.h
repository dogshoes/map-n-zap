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
// Module:     RRRSrvCom.h
// Abstract:   RRRSrv serial communications class header file.
// Author:     J.C.H
// Copyright 1997 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

#ifndef RRRSRVCOM
#define RRRSRVCOM

#include "RRRComm.h"
#include "RRRComEngine.h"

class RRRSrvCom : public RRRCom
{
        DECLARE_DYNAMIC(RRRSrvCom)

    public:
        RRRSrvCom(HWND hMainWnd, short address, LPCTSTR portname, long BaudRate, BOOL EnableDTR, long DTROnDelay, long DTROffDelay, short Encoding, short FullDuplex, BOOL InvertDTR);
        ~RRRSrvCom();

        long Connect(long wHnd, short NodeAddress, LPCTSTR ControlName);
        short Disconnect(long ConnectId);

        void OnMessageReceived(RRRMsg &Buffer, short RawMsgLen);
        void SendNextMsg();
        void SendMsg(RRRMsg &Msg);
        void SetResponseTime(long newTime);
        long GetResponseTime();
        void ServiceReceiveTimeout();
        void GetAddressList(CArray<short, short> &CurrentAddressList);
        long GetRetryLimit(short NodeAddress);
        void SetRetryLimit(short NodeAddress, long NewValue);

        // status support functions
        short GetCommState(short NodeAddress);
        short GetCommStatistics(short NodeAddress, long &RetryLimit, long &ResponseTime,
                                long &ISTRTRecv, long &ISTRTSent, long &ASTRTRecv, long &ASTRTSent,
                                long &AckRecv, long &PollsSent, long &MsgRecv, long &MsgSent,
                                long &SendRetries, long &TotalSendRetries, long &OutChars,
                                long &InChars, long &CommResets, unsigned char &RcvMsgNum,
                                unsigned char &SndMsgNum, long &InvalidAddressCount);

    protected:
        void OnCommOpen();
        void OnCommClose();

        long m_lResponseTime;               // current message response time
        short m_LastSendIndex;              // last active address index
        short m_LastSendAddress;            // last active address
        BOOL m_bCommIdle;                   // set to TRUE when com is idle
        long m_lInvalidAddressCount;        // incremented when a message is received with
        // an unexpected address value...
        CArray<short, short> AddressList;   // list of active addresses

        CTypedPtrMap<CMapWordToOb, WORD, RRRComEngine*> EngineMap;
};
#endif
/////////////////////////////////////////////////////////////////////////////
