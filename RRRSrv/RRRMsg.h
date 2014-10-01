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
   // Module:     RRRMsg.cpp
   //
   // Abstract:   RRR Message class header file.
   //
   // Author:     J.C.H	
   //
   // Revisions:
   //    $History: RRRMsg.h $
   // 
   // *****************  Version 7  *****************
   // User: Harnden      Date: 10/12/99   Time: 12:23a
   // Updated in $/RRR/RRRSrv
   // Added multi robot support.
   // 
   // *****************  Version 6  *****************
   // User: Harnden      Date: 12/19/98   Time: 3:04p
   // Updated in $/RRR/RRRSrv
   // Added message priority support.
   // 
   // *****************  Version 5  *****************
   // User: Harnden      Date: 9/09/98    Time: 7:14a
   // Updated in $/RRR/RRRSrv
   // Added RetryLimit property. Fixed buffer overrun problem in receive
   // logic. Tweaked RobotStatusUpdate logic.
   // 
   // *****************  Version 4  *****************
   // User: Harnden      Date: 8/03/98    Time: 10:57p
   // Updated in $/RRR/RRRSrv
   // Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
   // PowerDown command. Added send and receive of raw data. Added
   // CommStateChange event. Purge outgoing message queue when communications
   // link is lost.
   // 
   // *****************  Version 3  *****************
   // User: Harnden      Date: 5/25/98    Time: 10:19a
   // Updated in $/RRRSrv
   // Working radio interface.
   // 
   // *****************  Version 2  *****************
   // User: Harnden      Date: 1/05/98    Time: 12:50a
   // Updated in $/RRRSrv
   // Full send message protocol support (except ExecutePath).
   // 
   // *****************  Version 1  *****************
   // User: Harnden      Date: 1/03/98    Time: 6:17p
   // Created in $/RRRSrv
   //
   // Copyright 1997 Cycle Time Corp
   //
   ///////////////////////////////////////////////////////////////////////////////
   
   #ifndef RRRMSG
   #define RRRMSG
   
   #include "RRRConst.h"
   
   class RRRMsg : public CObject
   {
   	DECLARE_DYNAMIC(RRRMsg)
   
   public:
      RRRMsg();
      RRRMsg(short address);
      RRRMsg(const RRRMsg& src);
      RRRMsg(short address, unsigned char ch, int nRepeat);
      ~RRRMsg();
      RRRMsg& operator=(const RRRMsg &src);
      RRRMsg& operator+=(const RRRMsg &src);
      unsigned char GetAt(short index) const;
      void SetAt(short index, unsigned char ch);
      unsigned char GetChar();
      void Add(unsigned char ch);
      int GetLength() const {return buflen;}
      LPCTSTR GetBuffer(int nMinBufLength) {return (LPCTSTR)buffer;}
      void Empty();
      RRRMsg Left(int nCount) const;
      RRRMsg Right(int nCount) const;
      long GetLong();
      void AddLong(long value);
      void SetDestAddress(short address) {DestAddr = address;}
      short GetDestAddress() {return DestAddr;}
      void SetSrcAddress(short address) {SrcAddr = address;}
      short GetSrcAddress() {return SrcAddr;}
      void SetRcvMsgNum(unsigned char  num) {RcvMsgNum = num;}
      unsigned char  GetRcvMsgNum() {return RcvMsgNum;}
      void SetSndMsgNum(unsigned char  num) {SndMsgNum = num;}
      unsigned char  GetSndMsgNum() {return SndMsgNum;}
      const unsigned char *GetBufferAddr() {return buffer;}
      BOOL BufferFull() {return buflen == MAX_MSG_LEN;}
      void SetPriority(short priority) {if (priority == LOW_PRIORITY) Priority = LOW_PRIORITY; else Priority = HIGH_PRIORITY;}
      short GetPriority() {return Priority;}
      BOOL HighPriority() {if (Priority == LOW_PRIORITY) return FALSE; else return TRUE;}
   
      enum RRRMSG_CONSTANTS
      {
         MAX_MSG_LEN=1024,
      };
   
   protected:
      unsigned char buffer[MAX_MSG_LEN];
      short buflen;
      short bufindex;
      short DestAddr;
      short SrcAddr;
      unsigned char RcvMsgNum;
      unsigned char SndMsgNum;
      short Priority;
   };
   #endif
   /////////////////////////////////////////////////////////////////////////////
