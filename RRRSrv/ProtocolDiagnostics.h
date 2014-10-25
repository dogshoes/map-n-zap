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
// Module:     ProtocolDiagnostics.h
//
// Abstract:   ProtocolDiagnostics class header.
//
// Author:     J.C.H
//
// Revisions:
//    $History: ProtocolDiagnostics.h $
//
// *****************  Version 5  *****************
// User: Harnden      Date: 10/13/99   Time: 10:53p
// Updated in $/RRR/RRRSrv
// Added BAd Address counter to Protocol Diagnostics dialog.
//
// *****************  Version 4  *****************
// User: Harnden      Date: 10/12/99   Time: 12:23a
// Updated in $/RRR/RRRSrv
// Added multi robot support.
//
// *****************  Version 3  *****************
// User: Harnden      Date: 8/09/99    Time: 10:03p
// Updated in $/RRR/RRRSrv
// Merged 1P5 changes into mainstream. Included changes from Stephan in
// RRRSrvCom.cpp.
//
// *****************  Version 4  *****************
// User: Harnden      Date: 4/27/99    Time: 2:07a
// Updated in $/RRR1P5 Branch/RRRSrv
// Incorporated fixes made to 1.04 software to make work on Pony.
//
// *****************  Version 4  *****************
// User: Harnden      Date: 4/18/99    Time: 10:04p
// Updated in $/RRR1P0 Branch/RRRSrv
// Added protocol diagnostics and fixed some minor problems.
//
// *****************  Version 2  *****************
// User: Harnden      Date: 5/26/98    Time: 10:51p
// Updated in $/RRR/RRRSrv
// First radio interface release sent to Cycle Time Corp.
//
// *****************  Version 1  *****************
// User: Harnden      Date: 5/26/98    Time: 12:07a
// Created in $/RRR/RRRSrv
//
// Copyright 1997 Cycle Time Corp
//
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROTOCOLDIAGNOSTICS_H__AD736842_F414_11D1_BCC1_444553540000__INCLUDED_)
#define AFX_PROTOCOLDIAGNOSTICS_H__AD736842_F414_11D1_BCC1_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class RRRSrvCom;

/////////////////////////////////////////////////////////////////////////////
// ProtocolDiagnostics dialog

class ProtocolDiagnostics : public CDialog
{
        // Construction
    public:
        ProtocolDiagnostics(CWnd* pParent = NULL);   // standard constructor
        RRRSrvCom *m_oRRRCom;

        // Dialog Data
        //{{AFX_DATA(ProtocolDiagnostics)
        enum { IDD = IDD_PROTOCOL };
        CStatic	m_BadAddress;
        CStatic	m_SndMsgNum;
        CStatic	m_RcvMsgNum;
        CListBox	m_AddressList;
        CStatic	m_CommResets;
        CStatic	m_CharsSent;
        CStatic	m_CharsRecv;
        CStatic	m_PollsSent;
        CStatic	m_TotalRetries;
        CStatic	m_SendRetries;
        CStatic	m_MSGSent;
        CStatic	m_MSGRecv;
        CStatic	m_ISTRTSent;
        CStatic	m_ISTRTRecv;
        CStatic	m_ASTRTSent;
        CStatic	m_ASTRTRecv;
        CStatic	m_AckRecv;
        CStatic	m_Timeout;
        CStatic	m_RetryLimit;
        CStatic	m_CommState;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(ProtocolDiagnostics)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        void UpdateDisplay();

        CArray<short, short> OldAddressList;

        enum MYCONSTANTS
        {
            DISPTIMEEVENT = 113,
        };

        // Generated message map functions
        //{{AFX_MSG(ProtocolDiagnostics)
        virtual BOOL OnInitDialog();
        afx_msg void OnTimer(UINT nIDEvent);
        afx_msg void OnResetcomm();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROTOCOLDIAGNOSTICS_H__AD736842_F414_11D1_BCC1_444553540000__INCLUDED_)
