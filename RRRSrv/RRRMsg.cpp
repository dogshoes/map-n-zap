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
// Module:     RRRMsg.cpp
// Abstract:   Implementation of the RRR Message class.
// Author:     J.C.H
// Copyright 1997 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RRRMsg.h"

/////////////////////////////////////////////////////////////////////////////
// RRRCom

IMPLEMENT_DYNAMIC(RRRMsg, CObject)

/////////////////////////////////////////////////////////////////////////////
// RRRMsg construction/destruction

RRRMsg::RRRMsg()
{
    Empty();
}

RRRMsg::RRRMsg(short address)
{
    Empty();
    DestAddr = address;
}

RRRMsg::RRRMsg(const RRRMsg& src)
{
    buflen = src.buflen;
    bufindex = src.bufindex;
    DestAddr = src.DestAddr;
    SrcAddr = src.SrcAddr;
    Priority = src.Priority;
    RcvMsgNum = src.RcvMsgNum;
    SndMsgNum = src.SndMsgNum;

    memcpy(buffer, src.buffer, sizeof(buffer));
}

RRRMsg::RRRMsg(short address, unsigned char ch, int nRepeat = 1)
{
    Empty();
    DestAddr = address;
    buflen = nRepeat;

    if (buflen > MAX_MSG_LEN)
    {
        buflen = MAX_MSG_LEN;
    }

    memset(buffer, ch, buflen);
}

RRRMsg::~RRRMsg()
{
}

RRRMsg& RRRMsg::operator=(const RRRMsg &src)
{
    buflen = src.buflen;
    bufindex = src.bufindex;
    DestAddr = src.DestAddr;
    SrcAddr = src.SrcAddr;
    Priority = src.Priority;
    RcvMsgNum = src.RcvMsgNum;
    SndMsgNum = src.SndMsgNum;

    memcpy(buffer, src.buffer, sizeof(buffer));

    return *this;
}

RRRMsg& RRRMsg::operator+=(const RRRMsg &src)
{
    VERIFY((src.buflen + buflen) < MAX_MSG_LEN);

    memcpy(&buffer[buflen], src.buffer, src.buflen);

    buflen += src.buflen;
    bufindex = 0;

    return *this;
}

RRRMsg RRRMsg::Left(int nCount) const
{
    if (nCount < 0)
    {
        nCount = 0;
    }
    else if (nCount > buflen)
    {
        nCount = buflen;
    }

    RRRMsg dest;

    for (short i = 0; i < nCount; i++)
    {
        dest.Add(buffer[i]);
    }

    dest.DestAddr = DestAddr;
    dest.SrcAddr = SrcAddr;
    dest.Priority = Priority;
    dest.RcvMsgNum = RcvMsgNum;
    dest.SndMsgNum = SndMsgNum;

    return dest;
}

RRRMsg RRRMsg::Right(int nCount) const
{
    if (nCount < 0)
    {
        nCount = 0;
    }
    else if (nCount > buflen)
    {
        nCount = buflen;
    }

    short sindex = buflen - nCount;

    RRRMsg dest;

    for (short i = sindex; i < buflen; i++)
    {
        dest.Add(buffer[i]);
    }

    dest.DestAddr = DestAddr;
    dest.SrcAddr = SrcAddr;
    dest.Priority = Priority;
    dest.RcvMsgNum = RcvMsgNum;
    dest.SndMsgNum = SndMsgNum;

    return dest;
}

int RRRMsg::GetLength() const
{
    return buflen;
}

LPCTSTR RRRMsg::GetBuffer(int nMinBufLength)
{
    return (LPCTSTR)buffer;
}

void RRRMsg::Empty()
{
    buflen = 0;
    bufindex = 0;
    DestAddr = 0;
    SrcAddr = 0;
    RcvMsgNum = 0;
    SndMsgNum = 0;
    Priority = HIGH_PRIORITY;

    memset(buffer, 0, sizeof(buffer));
}

unsigned char RRRMsg::GetAt(short index) const
{
    VERIFY(index < MAX_MSG_LEN);
    VERIFY(index >= 0);

    return buffer[index];
}

void RRRMsg::SetAt(short index, unsigned char ch)
{
    VERIFY(index < MAX_MSG_LEN);
    VERIFY(index >= 0);

    buffer[index] = ch;
}

unsigned char RRRMsg::GetChar()
{
    VERIFY(bufindex < buflen);
    VERIFY(bufindex >= 0);

    unsigned char ch = buffer[bufindex];
    ++bufindex;

    return ch;
}

void RRRMsg::Add(unsigned char ch)
{
    if (buflen < MAX_MSG_LEN)
    {
        if (buflen < 0)
        {
            buflen = 0;
        }

        buffer[buflen] = ch;
        ++buflen;
    }
}

long RRRMsg::GetLong()
{
    VERIFY((bufindex + 3) < buflen);
    VERIFY(bufindex >= 0);

    long value = *(long *)(&buffer[bufindex]);
    bufindex += 4;

    return value;
}

void RRRMsg::AddLong(long value)
{
    VERIFY((buflen + 3) < MAX_MSG_LEN);
    VERIFY(buflen >= 0);

    *(long *)(&buffer[buflen]) = value;
    buflen += 4;
}

void RRRMsg::SetDestAddress(short address)
{
    DestAddr = address;
}

short RRRMsg::GetDestAddress()
{
    return DestAddr;
}

void RRRMsg::SetSrcAddress(short address)
{
    SrcAddr = address;
}

short RRRMsg::GetSrcAddress()
{
    return SrcAddr;
}

void RRRMsg::SetRcvMsgNum(unsigned char num)
{
    RcvMsgNum = num;
}

unsigned char RRRMsg::GetRcvMsgNum()
{
    return RcvMsgNum;
}

void RRRMsg::SetSndMsgNum(unsigned char num)
{
    SndMsgNum = num;
}

unsigned char RRRMsg::GetSndMsgNum()
{
    return SndMsgNum;
}

const unsigned char *RRRMsg::GetBufferAddr()
{
    return buffer;
}

BOOL RRRMsg::BufferFull()
{
    return buflen == MAX_MSG_LEN;
}

void RRRMsg::SetPriority(short priority)
{
    if (priority == LOW_PRIORITY)
    {
        Priority = LOW_PRIORITY;
    }
    else
    {
        Priority = HIGH_PRIORITY;
    }
}

short RRRMsg::GetPriority()
{
    return Priority;
}

BOOL RRRMsg::HighPriority()
{
    if (Priority == LOW_PRIORITY)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}