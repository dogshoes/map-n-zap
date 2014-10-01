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
   // Module:     RRRSrv.h
   //
   // Abstract:   main header file for the RRRSRV application
   //
   // Author:     J.C.H	
   //
   // Revisions:
   //    $History: RRRSrv.h $
   // 
   // *****************  Version 8  *****************
   // User: Harnden      Date: 10/25/99   Time: 10:56p
   // Updated in $/RRR/RRRSrv
   // Added MicrophoneVolume event. Implemented SetRobotAddress. Removed all
   // references to RobotAdress setting in CyeSrv.ini file.
   // 
   // *****************  Version 7  *****************
   // User: Harnden      Date: 10/12/99   Time: 12:23a
   // Updated in $/RRR/RRRSrv
   // Added multi robot support.
   // 
   // *****************  Version 6  *****************
   // User: Harnden      Date: 11/16/98   Time: 12:41a
   // Updated in $/RRR/RRRSrv
   // Added dynamic serial port change logic.
   // 
   // *****************  Version 5  *****************
   // User: Harnden      Date: 8/21/98    Time: 9:38p
   // Updated in $/RRR/RRRSrv
   // Added INVERTDTR parameter to rrrsrv.ini file to invert the DTR toggling
   // logic.
   // 
   // *****************  Version 4  *****************
   // User: Harnden      Date: 5/25/98    Time: 10:19a
   // Updated in $/RRRSrv
   // Working radio interface.
   // 
   // *****************  Version 3  *****************
   // User: Harnden      Date: 3/22/98    Time: 7:38p
   // Updated in $/RRRSrv
   // Updated status to include work at CTC.
   // 
   // *****************  Version 2  *****************
   // User: Harnden      Date: 1/05/98    Time: 12:50a
   // Updated in $/RRRSrv
   // Full send message protocol support (except ExecutePath).
   //
   // Copyright 1997 Cycle Time Corp
   //
   ///////////////////////////////////////////////////////////////////////////////
   
   #ifndef __AFXWIN_H__
   	#error include 'stdafx.h' before including this file for PCH
   #endif
   
   #include "resource.h"                  // main symbols
   #include "RRRConst.h"                  // RRR constant definitions
   #include "RRRSrvCom.h"                 // serial interface class
   
   #define PROGRAM_VERSION "0.1"
   #define GENERAL_SECTION "GENERAL"
   #define PORTNAME "PORT"
   #define DTRENABLE "DTRENABLE"
   #define DTRONDELAY "DTRONDELAY"
   #define DTROFFDELAY "DTROFFDELAY"
   #define BAUDRATE "BAUDRATE"
   #define ENCODE "ENCODE"
   #define FULLDUPLEX "FULLDUPLEX"
   #define LOCALADDRESS "LOCALADDRESS"
   #define ROBOTADDRESS "ROBOTADDRESS"
   #define INVERTDTR "INVERTDTR"
   #define PREFLAGCHARS "PRE_FLAG"
   #define POSTFLAGCHARS "POST_FLAG"
   
   /////////////////////////////////////////////////////////////////////////////
   // CRRRSrvApp:
   // See RRRSrv.cpp for the implementation of this class
   //
   
   class CRRRSrvApp : public CWinApp
   {
   public:
   	CRRRSrvApp();
   
   // Overrides
   	// ClassWizard generated virtual function overrides
   	//{{AFX_VIRTUAL(CRRRSrvApp)
   	public:
   	virtual BOOL InitInstance();
   	//}}AFX_VIRTUAL
   
   // Implementation
      enum NODE_ADDRESSES
      {
         DEFAULT_ROBOT_ADDRESS=1,
         DEFAULT_PC_ADDRESS=2,
      };
   
   	COleTemplateServer m_server;
      RRRSrvCom *m_oRRRCom;               // serial interface object
      CString m_sPortName;                // attached port name
      long m_BaudRate;                    // communications interface baud rate
      BOOL m_EnableDTR;                   // set to TRUE to enable DTR toggle on send
      long m_DTROnDelay;                  // sleep delay after turning on DTR
      long m_DTROffDelay;                 // sleep delay before turning off DTR
      short m_Encoding;                   // message encoding method to be used
      BOOL m_FullDuplex;                  // TRUE enables full duplex communication
      short m_MyAddress;                  // local address
      //short m_RobotAddress;               // robot address
      BOOL m_InvertDTR;                   // set to TRUE to invert DTR encoding
      short m_PreFlag;                    // number of pre flag characters per transmission
      short m_PostFlag;                   // number of post flag characters per transmission
   
      void SaveParameters();              // saves configuration parameters to INI file
   
   		// Server object for document creation
   
   	//{{AFX_MSG(CRRRSrvApp)
   	afx_msg void OnAppAbout();
   		// NOTE - the ClassWizard will add and remove member functions here.
   		//    DO NOT EDIT what you see in these blocks of generated code !
   	//}}AFX_MSG
   	DECLARE_MESSAGE_MAP()
   };
   
   
   /////////////////////////////////////////////////////////////////////////////
