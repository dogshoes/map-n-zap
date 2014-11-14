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
// Abstract:   RRR Message class header file.
// Author:     J.C.H
// Copyright 1997 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RRRConst.h"

/// <summary>
/// A message to or from the Cye robot.
/// </summary>
class RRRMsg : public CObject
{
    DECLARE_DYNAMIC(RRRMsg)

    public:
        /// <summary>
        /// Construct a new robot message.
        /// </summary>
        /// <returns>A newly constructed RRRMsg.</returns>
        RRRMsg();

        /// <summary>
        /// Construct a new robot message.
        /// </summary>
        /// <param name="address">The destination node's address.</param>
        /// <returns>A newly constructed RRRMsg.</returns>
        RRRMsg(short address);

        /// <summary>
        /// Construct a new robot message copied from an existing message.
        /// </summary>
        /// <param name="src">The message to copy.</param>
        /// <returns>A newly constructed RRRMsg.</returns>
        RRRMsg(const RRRMsg& src);

        /// <summary>
        /// Construct a new robot message.
        /// </summary>
        /// <remarks>
        /// The buffer will remain MAX_MSG_LEN bytes long regardless of the settings here.
        /// </remarks>
        /// <param name="address">The destination node's address.</param>
        /// <param name="ch">The character to initialize the message buffer with.</param>
        /// <param name="nRepeat">The number of bytes to initialize in the buffer.</param>
        /// <returns>A newly constructed RRRMsg.</returns>
        RRRMsg(short address, unsigned char ch, int nRepeat);

        /// <summary>
        /// Destroy this instance of the robot message.
        /// </summary>
        ~RRRMsg();

        /// <summary>
        /// Operator overload for deep cloning a robot message.
        /// </summary>
        RRRMsg& operator=(const RRRMsg &src);

        /// <summary>
        /// Operator overload for combining two robot message buffers together.
        /// </summary>
        RRRMsg& operator+=(const RRRMsg &src);

        /// <summary>
        /// Get a byte in the message buffer at a specific position.
        /// </summary>
        /// <param name="index">The buffer position to get.</param>
        /// <returns>The byte at the supplied buffer position.</returns>
        unsigned char GetAt(short index) const;

        /// <summary>
        /// Set a byte in the message buffer.
        /// </summary>
        /// <param name="index">The buffer position to set.</param>
        /// <param name="ch">The byte to set.</param>
        void SetAt(short index, unsigned char ch);

        /// <summary>
        /// Get the byte in the message buffer at the current position and advance the cursor.
        /// </summary>
        /// <returns>The byte at the current cursor position.</returns>
        unsigned char GetChar();

        /// <summary>
        /// Add a byte to the message buffer starting at the current cursor position and advance the cursor.
        /// </summary>
        /// <param name="value">The byte to add to the buffer.</param>
        void Add(unsigned char ch);

        /// <summary>
        /// Get the size of the message buffer.
        /// </summary>
        /// <returns>Size of the message buffer.</returns>
        int GetLength() const;

        /// <summary>
        /// Get the entire message buffer.
        /// </summary>
        /// <returns>The message buffer.</returns>
        LPCTSTR GetBuffer(int nMinBufLength);
        
        /// <summary>
        /// Reset the message options and buffer to default.
        /// </summary>
        void Empty();
        
        /// <summary>
        /// Clone this message including a selection of the left-most bytes in the buffer.
        /// </summary>
        /// <param name="nCount">The number of bytes to copy from the buffer starting at position 0.</param>
        /// <returns>The created message clone.</returns>
        RRRMsg Left(int nCount) const;
        
        /// <summary>
        /// Clone this message including a selection of the right-most bytes in the buffer.
        /// </summary>
        /// <param name="nCount">The number of bytes to copy from the buffer starting this number of bytes from the end.</param>
        /// <returns>The created message clone.</returns>
        RRRMsg Right(int nCount) const;
        
        /// <summary>
        /// Get the next four bytes from the message buffer as a long and advance the cursor position.
        /// </summary>
        /// <returns>The next four bytes from the message buffer.</returns>
        long GetLong();
        
        /// <summary>
        /// Add a long (four bytes) to the message buffer starting at the current cursor position and advance the cursor position
        /// </summary>
        /// <param name="value">The long to add to the buffer.</param>
        void AddLong(long value);
        
        /// <summary>
        /// Set the destination node's address.
        /// </summary>
        /// <param name="address">The address of the destination node.</param>
        void SetDestAddress(short address);
        
        /// <summary>
        /// Get the destination node's address.
        /// </summary>
        /// <returns>The destination node's address.</returns>
        short GetDestAddress();
        
        /// <summary>
        /// Set the source node's address.
        /// </summary>
        /// <param name="address">The address of the source node.</param>
        void SetSrcAddress(short address);
        
        /// <summary>
        /// Get the source node's address.
        /// </summary>
        /// <returns>The source node's address.</returns>
        short GetSrcAddress();
        
        /// <summary>
        /// Set the received message number for acknowledgement.
        /// </summary>
        /// <param name="num">The received message number.</param>
        void SetRcvMsgNum(unsigned char num);
        
        /// <summary>
        /// Get the received message number for acknowledgement.
        /// </summary>
        /// <returns>The received message number.</returns>
        unsigned char GetRcvMsgNum();
        
        /// <summary>
        /// Set the sent message number for acknowledgement.
        /// </summary>
        /// <param name="num">The sent message number.</param>
        void SetSndMsgNum(unsigned char num);
        
        /// <summary>
        /// Get the sent message number for acknowledgement.
        /// </summary>
        /// <returns>The sent message number.</returns>
        unsigned char GetSndMsgNum();
        
        /// <summary>
        /// Get a pointer to the message buffer.
        /// </summary>
        /// <returns>A pointer to the message buffer.</returns>
        const unsigned char *GetBufferAddr();
        
        /// <summary>
        /// Check whether the buffer is full.
        /// </summary>
        /// <returns>A value indicating whether the buffer is full.</returns>
        BOOL BufferFull();
        
        /// <summary>
        /// Set the message priority.
        /// </summary>
        /// <params name="priority">The priority: either LOW_PRIORITY or HIGH_PRIORITY.</params>
        void SetPriority(short priority);
        
        /// <summary>
        /// Get the message's priority.
        /// </summary>
        /// <returns>The priority: either LOW_PRIORITY or HIGH_PRIORITY.</returns>
        short GetPriority();

        /// <summary>
        /// Check whether this message has a high priority.
        /// </summary>
        /// <returns>A value indicating whether this message has a high priority.</returns>
        BOOL HighPriority();

        enum RRRMSG_CONSTANTS
        {
            /// <summary>
            /// The maximum length for a robot message.
            /// </summary>
            MAX_MSG_LEN = 1024,
        };

    protected:
        /// <summary>
        /// The message buffer.
        /// </summary>
        unsigned char buffer[MAX_MSG_LEN];

        /// <summary>
        /// The expected length of the message buffer set at RRRMsg construction.
        /// </summary>
        /// <remarks>
        /// This must not exceed MAX_MSG_LEN.
        /// </remarks>
        short buflen;

        /// <summary>
        /// The current position of the cursor in the buffer.
        /// </summary>
        short bufindex;

        /// <summary>
        /// The address of the message recipient.
        /// </summary>
        short DestAddr;

        /// <summary>
        /// The address of the message source.
        /// </summary>
        short SrcAddr;

        /// <summary>
        /// The received message number for acknowledgement.
        /// </summary>
        unsigned char RcvMsgNum;

        /// <summary>
        /// The sent message number for acknowledgement.
        /// </summary>
        unsigned char SndMsgNum;

        /// <summary>
        /// The message priority.
        /// </summary>
        short Priority;
};