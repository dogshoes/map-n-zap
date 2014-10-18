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
 // Module:     RRRSrvCom.cpp
 //
 // Abstract:   RRRSrv serial communications class.
 //
 // Author:     J.C.H	
 //
 // Revisions:
 //    $History: RRRSrvCom.cpp $
 // 
 // *****************  Version 28  *****************
 // User: Harnden      Date: 11/04/99   Time: 1:26a
 // Updated in $/RRR/RRRSrv
 // Changed polling mechasim so message timer always runs...
 // 
 // *****************  Version 27  *****************
 // User: Harnden      Date: 10/17/99   Time: 3:02p
 // Updated in $/RRR/RRRSrv
 // Rewrote message timeout and delivery logic to avoid duplicate messages.
 // 
 // *****************  Version 26  *****************
 // User: Harnden      Date: 10/16/99   Time: 5:30p
 // Updated in $/RRR/RRRSrv
 // Fixed RetryLimit get and set.
 // 
 // *****************  Version 25  *****************
 // User: Harnden      Date: 10/13/99   Time: 10:53p
 // Updated in $/RRR/RRRSrv
 // Added BAd Address counter to Protocol Diagnostics dialog.
 // 
 // *****************  Version 24  *****************
 // User: Harnden      Date: 10/12/99   Time: 12:23a
 // Updated in $/RRR/RRRSrv
 // Added multi robot support.
 // 
 // *****************  Version 23  *****************
 // User: Harnden      Date: 8/25/99    Time: 12:30a
 // Updated in $/RRR/RRRSrv
 // Changes from Stephan.
 // 
 // *****************  Version 22  *****************
 // User: Harnden      Date: 8/09/99    Time: 10:46p
 // Updated in $/RRR/RRRSrv
 // Added new robot status packet support for software revision 9.
 // 
 // *****************  Version 21  *****************
 // User: Harnden      Date: 8/09/99    Time: 10:03p
 // Updated in $/RRR/RRRSrv
 // Merged 1P5 changes into mainstream. Included changes from Stephan in
 // RRRSrvCom.cpp.
 // 
 // *****************  Version 21  *****************
 // User: Harnden      Date: 4/27/99    Time: 2:07a
 // Updated in $/RRR1P5 Branch/RRRSrv
 // Incorporated fixes made to 1.04 software to make work on Pony.
 // 
 // *****************  Version 19  *****************
 // User: Harnden      Date: 3/07/99    Time: 8:46p
 // Updated in $/RRR/RRRSrv
 // Added fix for reception of unknown messages with piggy back acks.
 // 
 // *****************  Version 18  *****************
 // User: Harnden      Date: 3/07/99    Time: 12:12p
 // Updated in $/RRR/RRRSrv
 // Added all functionality in Stephan's document. Converted name to
 // CyeSrv.exe.
 // 
 // *****************  Version 17  *****************
 // User: Harnden      Date: 12/19/98   Time: 3:04p
 // Updated in $/RRR/RRRSrv
 // Added message priority support.
 // 
 // *****************  Version 16  *****************
 // User: Harnden      Date: 12/07/98   Time: 12:22a
 // Updated in $/RRR/RRRSrv
 // Added IDLE support to radio level logic.
 // 
 // *****************  Version 15  *****************
 // User: Harnden      Date: 12/03/98   Time: 12:07p
 // Updated in $/RRR/RRRSrv
 // Added RadioLevel support and fixed about box displays.
 // 
 // *****************  Version 14  *****************
 // User: Harnden      Date: 11/28/98   Time: 9:38a
 // Updated in $/RRR/RRRSrv
 // Last minute changes and fix version info for release.
 // 
 // *****************  Version 13  *****************
 // User: Harnden      Date: 9/09/98    Time: 7:14a
 // Updated in $/RRR/RRRSrv
 // Added RetryLimit property. Fixed buffer overrun problem in receive
 // logic. Tweaked RobotStatusUpdate logic.
 // 
 // *****************  Version 12  *****************
 // User: Harnden      Date: 8/21/98    Time: 9:38p
 // Updated in $/RRR/RRRSrv
 // Added INVERTDTR parameter to rrrsrv.ini file to invert the DTR toggling
 // logic.
 // 
 // *****************  Version 11  *****************
 // User: Harnden      Date: 8/03/98    Time: 10:57p
 // Updated in $/RRR/RRRSrv
 // Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
 // PowerDown command. Added send and receive of raw data. Added
 // CommStateChange event. Purge outgoing message queue when communications
 // link is lost.
 // 
 // *****************  Version 10  *****************
 // User: Harnden      Date: 7/06/98    Time: 10:28p
 // Updated in $/RRR/RRRSrv
 // Added Heading parameter to RobotStatusUpdate event.
 // 
 // *****************  Version 9  *****************
 // User: Harnden      Date: 7/06/98    Time: 12:44a
 // Updated in $/RRR/RRRSrv
 // Added new RobotStatusUpdate event and changed SendFollowPath from BOOL
 // to short.
 // 
 // *****************  Version 8  *****************
 // User: Harnden      Date: 6/22/98    Time: 9:11p
 // Updated in $/RRR/RRRSrv
 // Added three new messages and one event.
 // 
 // *****************  Version 7  *****************
 // User: Harnden      Date: 5/26/98    Time: 10:51p
 // Updated in $/RRR/RRRSrv
 // First radio interface release sent to Cycle Time Corp.
 // 
 // *****************  Version 6  *****************
 // User: Harnden      Date: 5/25/98    Time: 10:19a
 // Updated in $/RRRSrv
 // Working radio interface.
 // 
 // *****************  Version 5  *****************
 // User: Harnden      Date: 3/22/98    Time: 7:38p
 // Updated in $/RRRSrv
 // Updated status to include work at CTC.
 // 
 // *****************  Version 4  *****************
 // User: Harnden      Date: 1/17/98    Time: 4:32p
 // Updated in $/RRRSrv
 // First working copy tested against RobotSim.
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
 
 #include "stdafx.h"
 #include "RRRSrv.h"
 
 #include "RRRSrvCom.h"
 #include "Robot Communication.h"
 
 /////////////////////////////////////////////////////////////////////////////
 // RRRSrvCom
 
 IMPLEMENT_DYNAMIC(RRRSrvCom, CObject)
 
 /////////////////////////////////////////////////////////////////////////////
 // RRRCom construction/destruction
 
 RRRSrvCom::RRRSrvCom(HWND hMainWnd, short address, LPCTSTR portname, long BaudRate, BOOL EnableDTR, long DTROnDelay, long DTROffDelay, short Encoding, short FullDuplex, BOOL InvertDTR) :
    RRRCom(hMainWnd, address, portname, BaudRate, EnableDTR, DTROnDelay, DTROffDelay, Encoding, FullDuplex, InvertDTR)
 {
    m_lResponseTime = RESPONSE_TIME;
    m_LastSendIndex = -1;
    m_LastSendAddress = -1;
    m_bCommIdle = TRUE;
    m_lInvalidAddressCount = 0;
 }
    
 RRRSrvCom::~RRRSrvCom()
 {
    ShutdownCommunications();
 }
 
 long RRRSrvCom::Connect(long wHnd, short NodeAddress, LPCTSTR ControlName)
 {
    ++m_lNumConnects;
    m_oCList.AddEntry(m_lNumConnects, wHnd, ControlName, NodeAddress);
    
    // tell this client what the current comm state is...
    PostMessage((HWND)wHnd, RRR_COMMSTATECHANGE, m_CommState, m_OldCommState);
 
    RRRComEngine *Engine=NULL;
    if (EngineMap.Lookup(NodeAddress,Engine) == FALSE)
    {
       Engine = new RRRComEngine(NodeAddress, &m_oCList, m_hMainWnd, m_hLocalWhnd, m_Encoding, m_FullDuplex);
       EngineMap.SetAt(NodeAddress, Engine);
       AddressList.Add(NodeAddress);
    }
    // prime the pump if nothings happening
    // messages only sent from timeout or successful receive
    //if (m_bCommIdle)
    //   SendNextMsg();
 
    return m_lNumConnects;
 }
 
 short RRRSrvCom::Disconnect(long ConnectId)
 {
    // check to see what address this client was talking to
    short Address=m_oCList.GetEntryAddress(ConnectId);
    // delete the entry
    m_oCList.DeleteEntry(ConnectId);
    // no check to see if this address is still being used
    if (!m_oCList.AddressInUse(Address))
    {
       // no, it's not, delete the attached engine
       RRRComEngine *Engine=NULL;
       // look up the engine class instance
       if (EngineMap.Lookup(Address,Engine))
       {
          // delete the class instance
          delete Engine;
          // remove the key in the map
          EngineMap.RemoveKey(Address);
          // remove the address from the address list
          for (short i=0;i < AddressList.GetSize();i++)
          {
             if (Address == AddressList.GetAt(i))
                AddressList.RemoveAt(i);
          }
       }
    }
    return 0;
 }
 
 void CALLBACK TimerCallback(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
 {
    // This is called as a result of a response timeout
    RRRSrvCom *rcom = (RRRSrvCom *)dwUser;
    rcom->ServiceReceiveTimeout();
 }
 
 void RRRSrvCom::ServiceReceiveTimeout()
 {
    // look up the matching engine
    RRRComEngine *Engine=NULL;
    if (EngineMap.Lookup(m_LastSendAddress,Engine))
    {
       Engine->ServiceReceiveTimeout();
    }
    // just send the next message...
    SendNextMsg();
 }
 
 void RRRSrvCom::OnMessageReceived(RRRMsg &Buffer, short RawMsgLen)
 {
    if (IsWindow(m_hMainWnd))
    {
       PostMessage(m_hMainWnd, RRR_RECEIVING,0,0);
       // look up the correct engine
       short Address = Buffer.GetSrcAddress();
       RRRComEngine *Engine=NULL;
       // look up the engine class instance
       if (!EngineMap.Lookup(Address,Engine))
       {
          CString Buffer;
          Buffer.Format("Unknown address in received buffer. Address =%i",Address);
          OutputDebugString(Buffer);
          ++m_lInvalidAddressCount;
       }
       else
       {
          
          switch(m_Encoding)
          {
          case CHECKSUM_ENCODING:
             {
                Engine->ServiceReceivedMessage(Buffer);
             }
             break;
          case CRC_ENCODING:
             {
                // service the received message
                Engine->ServiceReceivedMessage(Buffer);
                // send the next message
                SendNextMsg();
             }
             break;                           // End CRC encoding case
          }
       }
    }
 }
 
 void RRRSrvCom::OnCommOpen()
 {
    if (!m_FullDuplex)
    {
       // Send the first poll message
       //if (m_bCommIdle)
       //   SendNextMsg();
    }
 }
 
 void RRRSrvCom::OnCommClose()
 {
 }
 
 void RRRSrvCom::SendNextMsg()
 {
    m_bCommIdle = TRUE;
    if (AddressList.GetSize())
    {
       // bump the address index
       ++m_LastSendIndex;
       // make sure its in range
       if ((m_LastSendIndex >= AddressList.GetSize()) ||
          (m_LastSendIndex < 0))
          m_LastSendIndex = 0;
       // get the next target address
       m_LastSendAddress = AddressList.GetAt(m_LastSendIndex);
       // look up the matching engine
       RRRComEngine *Engine=NULL;
       if (EngineMap.Lookup(m_LastSendAddress,Engine))
       {
          m_bCommIdle = FALSE;
          RRRMsg Msg;
          Engine->SendNextMsg(Msg);
          // Save the last message type sent
          RRRCom::SendMsg(Msg, m_lResponseTime);
       }
    }
 }
 
 void RRRSrvCom::SendMsg(RRRMsg &Msg)
 {
    short Address=Msg.GetDestAddress();
    // look up the matching engine
    RRRComEngine *Engine=NULL;
    if (EngineMap.Lookup(Address,Engine))
    {
       // queue the outgoing message
       Engine->QueueMsg(Msg);
    }
    // prime the pump if nothings happening
    //if (m_bCommIdle)
    //   SendNextMsg();
 }
 
 void RRRSrvCom::SetResponseTime(long newTime)
 {
    if ((newTime > MIN_RESPONSE_TIME) && (newTime <= MAX_RESPONSE_TIME))
    {
       m_lResponseTime = newTime;
    }
 }
 
 long RRRSrvCom::GetResponseTime()
 {
    return m_lResponseTime;
 }
 
 void RRRSrvCom::GetAddressList(CArray<short,short> &CurrentAddressList)
 {
    CurrentAddressList.RemoveAll();
    for (short i=0;i < AddressList.GetSize();i++)
    {
       CurrentAddressList.Add(AddressList.GetAt(i));
    }
 }
 
 short RRRSrvCom::GetCommState(short NodeAddress)
 {
    short CommState=UNKNOWN_STATE;
    RRRComEngine *Engine=NULL;
    if (EngineMap.Lookup(NodeAddress,Engine))
    {
       CommState=Engine->GetCommState();
    }
    return CommState;
 }
 
 short RRRSrvCom::GetCommStatistics(short NodeAddress, long &RetryLimit, long &ResponseTime,
                                    long &ISTRTRecv, long &ISTRTSent, long &ASTRTRecv, long &ASTRTSent,
                                    long &AckRecv, long &PollsSent, long &MsgRecv, long &MsgSent,
                                    long &SendRetries, long &TotalSendRetries, long &OutChars,
                                    long &InChars, long &CommResets,
                                    unsigned char &RcvMsgNum, unsigned char &SndMsgNum, 
                                    long &InvalidAddressCount)
 {
    short status=-1;
    RRRComEngine *Engine=NULL;
    if (EngineMap.Lookup(NodeAddress,Engine))
    {
       status = 0;
       ResponseTime = m_lResponseTime;
       OutChars = m_lOutChars;
       InChars = m_lInChars;
       CommResets = m_lCommResets;
       InvalidAddressCount = m_lInvalidAddressCount;
       Engine->GetCommStatistics(RetryLimit, ISTRTRecv, 
          ISTRTSent, ASTRTRecv, ASTRTSent, AckRecv, PollsSent, 
          MsgRecv, MsgSent, SendRetries, TotalSendRetries,
          RcvMsgNum, SndMsgNum);
    }
    return status;
 }
 
 long RRRSrvCom::GetRetryLimit(short NodeAddress)
 {
    long RetryLimit = RRRCom::MAX_RETRIES;
    RRRComEngine *Engine=NULL;
    if (EngineMap.Lookup(NodeAddress,Engine))
    {
       RetryLimit = Engine->GetRetryLimit();
    }
    return RetryLimit;
 }
 
 void RRRSrvCom::SetRetryLimit(short NodeAddress, long NewValue)
 {
    RRRComEngine *Engine=NULL;
    if (EngineMap.Lookup(NodeAddress,Engine))
    {
       Engine->SetRetryLimit(NewValue);
    }
 }
