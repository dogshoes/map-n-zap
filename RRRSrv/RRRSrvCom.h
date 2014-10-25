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
// Module:     RRRSrvCom.h
//
// Abstract:   RRRSrv serial communications class header file.
//
// Author:     J.C.H
//
// Revisions:
//    $History: RRRSrvCom.h $
//
// *****************  Version 18  *****************
// User: Harnden      Date: 10/17/99   Time: 3:02p
// Updated in $/RRR/RRRSrv
// Rewrote message timeout and delivery logic to avoid duplicate messages.
//
// *****************  Version 17  *****************
// User: Harnden      Date: 10/16/99   Time: 5:30p
// Updated in $/RRR/RRRSrv
// Fixed RetryLimit get and set.
//
// *****************  Version 16  *****************
// User: Harnden      Date: 10/13/99   Time: 10:53p
// Updated in $/RRR/RRRSrv
// Added BAd Address counter to Protocol Diagnostics dialog.
//
// *****************  Version 15  *****************
// User: Harnden      Date: 10/12/99   Time: 12:23a
// Updated in $/RRR/RRRSrv
// Added multi robot support.
//
// *****************  Version 14  *****************
// User: Harnden      Date: 8/09/99    Time: 10:46p
// Updated in $/RRR/RRRSrv
// Added new robot status packet support for software revision 9.
//
// *****************  Version 13  *****************
// User: Harnden      Date: 8/09/99    Time: 10:03p
// Updated in $/RRR/RRRSrv
// Merged 1P5 changes into mainstream. Included changes from Stephan in
// RRRSrvCom.cpp.
//
// *****************  Version 13  *****************
// User: Harnden      Date: 4/27/99    Time: 2:07a
// Updated in $/RRR1P5 Branch/RRRSrv
// Incorporated fixes made to 1.04 software to make work on Pony.
//
// *****************  Version 11  *****************
// User: Harnden      Date: 12/19/98   Time: 3:04p
// Updated in $/RRR/RRRSrv
// Added message priority support.
//
// *****************  Version 10  *****************
// User: Harnden      Date: 12/07/98   Time: 12:22a
// Updated in $/RRR/RRRSrv
// Added IDLE support to radio level logic.
//
// *****************  Version 9  *****************
// User: Harnden      Date: 12/03/98   Time: 12:07p
// Updated in $/RRR/RRRSrv
// Added RadioLevel support and fixed about box displays.
//
// *****************  Version 8  *****************
// User: Harnden      Date: 8/21/98    Time: 9:38p
// Updated in $/RRR/RRRSrv
// Added INVERTDTR parameter to rrrsrv.ini file to invert the DTR toggling
// logic.
//
// *****************  Version 7  *****************
// User: Harnden      Date: 8/03/98    Time: 10:57p
// Updated in $/RRR/RRRSrv
// Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
// PowerDown command. Added send and receive of raw data. Added
// CommStateChange event. Purge outgoing message queue when communications
// link is lost.
//
// *****************  Version 6  *****************
// User: Harnden      Date: 5/26/98    Time: 10:51p
// Updated in $/RRR/RRRSrv
// First radio interface release sent to Cycle Time Corp.
//
// *****************  Version 5  *****************
// User: Harnden      Date: 5/25/98    Time: 10:19a
// Updated in $/RRRSrv
// Working radio interface.
//
// *****************  Version 4  *****************
// User: Harnden      Date: 3/22/98    Time: 7:38p
// Updated in $/RRRSrv
// Updated status to include work at CTC.
//
// *****************  Version 3  *****************
// User: Harnden      Date: 1/12/98    Time: 12:45a
// Updated in $/RRRSrv
// First delivery of ActiveX interface.
//
// *****************  Version 2  *****************
// User: Harnden      Date: 1/05/98    Time: 12:50a
// Updated in $/RRRSrv
// Full send message protocol support (except ExecutePath).
//
// Copyright 1997 Cycle Time Corp
//
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
