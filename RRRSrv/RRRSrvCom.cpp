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
// Module:     RRRSrvCom.cpp
// Abstract:   RRRSrv serial communications class.
// Author:     J.C.H
// Copyright 1997 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RRRSrv.h"
#include "RRRSrvCom.h"
#include "RobotComm.h"

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

    RRRComEngine *Engine = NULL;

    if (EngineMap.Lookup(NodeAddress, Engine) == FALSE)
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
    short Address = m_oCList.GetEntryAddress(ConnectId);
    // delete the entry
    m_oCList.DeleteEntry(ConnectId);

    // no check to see if this address is still being used
    if (!m_oCList.AddressInUse(Address))
    {
        // no, it's not, delete the attached engine
        RRRComEngine *Engine = NULL;

        // look up the engine class instance
        if (EngineMap.Lookup(Address, Engine))
        {
            // delete the class instance
            delete Engine;
            // remove the key in the map
            EngineMap.RemoveKey(Address);

            // remove the address from the address list
            for (short i = 0; i < AddressList.GetSize(); i++)
            {
                if (Address == AddressList.GetAt(i))
                {
                    AddressList.RemoveAt(i);
                }
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
    RRRComEngine *Engine = NULL;

    if (EngineMap.Lookup(m_LastSendAddress, Engine))
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
        PostMessage(m_hMainWnd, RRR_RECEIVING, 0, 0);
        // look up the correct engine
        short Address = Buffer.GetSrcAddress();
        RRRComEngine *Engine = NULL;

        // look up the engine class instance
        if (!EngineMap.Lookup(Address, Engine))
        {
            CString Buffer;
            Buffer.Format("Unknown address in received buffer. Address =%i", Address);
            OutputDebugString(Buffer);
            ++m_lInvalidAddressCount;
        }
        else
        {

            switch (m_Encoding)
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
        {
            m_LastSendIndex = 0;
        }

        // get the next target address
        m_LastSendAddress = AddressList.GetAt(m_LastSendIndex);
        // look up the matching engine
        RRRComEngine *Engine = NULL;

        if (EngineMap.Lookup(m_LastSendAddress, Engine))
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
    short Address = Msg.GetDestAddress();
    // look up the matching engine
    RRRComEngine *Engine = NULL;

    if (EngineMap.Lookup(Address, Engine))
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

void RRRSrvCom::GetAddressList(CArray<short, short> &CurrentAddressList)
{
    CurrentAddressList.RemoveAll();

    for (short i = 0; i < AddressList.GetSize(); i++)
    {
        CurrentAddressList.Add(AddressList.GetAt(i));
    }
}

short RRRSrvCom::GetCommState(short NodeAddress)
{
    short CommState = UNKNOWN_STATE;
    RRRComEngine *Engine = NULL;

    if (EngineMap.Lookup(NodeAddress, Engine))
    {
        CommState = Engine->GetCommState();
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
    short status = -1;
    RRRComEngine *Engine = NULL;

    if (EngineMap.Lookup(NodeAddress, Engine))
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
    RRRComEngine *Engine = NULL;

    if (EngineMap.Lookup(NodeAddress, Engine))
    {
        RetryLimit = Engine->GetRetryLimit();
    }

    return RetryLimit;
}

void RRRSrvCom::SetRetryLimit(short NodeAddress, long NewValue)
{
    RRRComEngine *Engine = NULL;

    if (EngineMap.Lookup(NodeAddress, Engine))
    {
        Engine->SetRetryLimit(NewValue);
    }
}