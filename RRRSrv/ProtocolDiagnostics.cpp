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
 // Module:     ProtocolDiagnostics.cpp
 //
 // Abstract:   ProtocolDiagnostics dialog class implementation file.
 //
 // Author:     J.C.H	
 //
 // Revisions:
 //    $History: ProtocolDiagnostics.cpp $
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
 
 #include "stdafx.h"
 #include "RRRSrv.h"
 #include "ProtocolDiagnostics.h"
 
 /////////////////////////////////////////////////////////////////////////////
 // ProtocolDiagnostics dialog
 
 
 ProtocolDiagnostics::ProtocolDiagnostics(CWnd* pParent /*=NULL*/)
 	: CDialog(ProtocolDiagnostics::IDD, pParent)
 {
 	//{{AFX_DATA_INIT(ProtocolDiagnostics)
 	//}}AFX_DATA_INIT
 
    m_oRRRCom = ((CRRRSrvApp *)AfxGetApp())->m_oRRRCom;
 
    // create the modeless dialog
    Create( ProtocolDiagnostics::IDD, pParent );
 }
 
 
 void ProtocolDiagnostics::DoDataExchange(CDataExchange* pDX)
 {
 	CDialog::DoDataExchange(pDX);
 	//{{AFX_DATA_MAP(ProtocolDiagnostics)
 	DDX_Control(pDX, IDC_BAD_ADDRESS, m_BadAddress);
 	DDX_Control(pDX, IDC_SND_MSG_NUM, m_SndMsgNum);
 	DDX_Control(pDX, IDC_RCV_MSG_NUM, m_RcvMsgNum);
 	DDX_Control(pDX, IDC_ADDRESSLIST, m_AddressList);
 	DDX_Control(pDX, IDC_COMM_RESETS, m_CommResets);
 	DDX_Control(pDX, IDC_CHARS_SENT, m_CharsSent);
 	DDX_Control(pDX, IDC_CHARS_RECV, m_CharsRecv);
 	DDX_Control(pDX, IDC_POLLS_SENT, m_PollsSent);
 	DDX_Control(pDX, IDC_TOTAL_RETRIES, m_TotalRetries);
 	DDX_Control(pDX, IDC_SEND_RETRIES, m_SendRetries);
 	DDX_Control(pDX, IDC_MSG_SENT, m_MSGSent);
 	DDX_Control(pDX, IDC_MSG_RECV, m_MSGRecv);
 	DDX_Control(pDX, IDC_ISTRT_SENT, m_ISTRTSent);
 	DDX_Control(pDX, IDC_ISTRT_RECV, m_ISTRTRecv);
 	DDX_Control(pDX, IDC_ASTRT_SENT, m_ASTRTSent);
 	DDX_Control(pDX, IDC_ASTRT_RECV, m_ASTRTRecv);
 	DDX_Control(pDX, IDC_ACK_RECV, m_AckRecv);
 	DDX_Control(pDX, IDC_TIMEOUT, m_Timeout);
 	DDX_Control(pDX, IDC_RETRY_LIMIT, m_RetryLimit);
 	DDX_Control(pDX, IDC_COMM_STATE, m_CommState);
 	//}}AFX_DATA_MAP
 }
 
 
 BEGIN_MESSAGE_MAP(ProtocolDiagnostics, CDialog)
 	//{{AFX_MSG_MAP(ProtocolDiagnostics)
 	ON_WM_TIMER()
 	ON_BN_CLICKED(IDC_RESETCOMM, OnResetcomm)
 	//}}AFX_MSG_MAP
 END_MESSAGE_MAP()
 
 void ProtocolDiagnostics::UpdateDisplay()
 {
    CArray<short,short> NewAddressList;
    m_oRRRCom->GetAddressList(NewAddressList);
 
    BOOL ListChanged=TRUE;
    if (NewAddressList.GetSize() == OldAddressList.GetSize())
    {
       ListChanged = FALSE;
       for (short i=0;i < OldAddressList.GetSize();i++)
       {
          if (OldAddressList.GetAt(i) != NewAddressList.GetAt(i))
             ListChanged = TRUE;
       }
    }
 
    if (ListChanged)
    {
       int CurSelection = m_AddressList.GetCurSel();
       CString Text;
       int CurAddress=-1;
       if (CurSelection != LB_ERR)
       {
          CurAddress=OldAddressList.GetAt(CurSelection);
       }
       m_AddressList.ResetContent();
       OldAddressList.RemoveAll();
       for (short i=0;i < NewAddressList.GetSize();i++)
       {
          short Address=NewAddressList.GetAt(i);
          OldAddressList.Add(Address);
          Text.Format("%i",Address);
          m_AddressList.AddString(Text);
       }
       Text.Format("%i",CurAddress);
       short Index=m_AddressList.FindString(-1,Text);
       if (Index == LB_ERR)
          Index = 0;
       m_AddressList.SetCurSel(Index);
    }
 
    int CurSelection = m_AddressList.GetCurSel();
    CString Text;
    int CurAddress=-1;
    if (CurSelection != LB_ERR)
    {
       CurAddress = OldAddressList.GetAt(CurSelection);
    }
    if (CurAddress >= 0)
    {
       switch(m_oRRRCom->GetCommState(CurAddress))
       {
       case ISTRT_STATE:
          m_CommState.SetWindowText("ISTRT");
          break;
       case ASTRT_STATE:
          m_CommState.SetWindowText("ASTRT");
          break;
       case IDLE_STATE:
          m_CommState.SetWindowText("IDLE");
          break;
       case ACK_WAIT_STATE:
          m_CommState.SetWindowText("ACK WAIT");
          break;
       default:
          m_CommState.SetWindowText("?");
          break;
       }
       long RetryLimit;
       long ResponseTime;
       long ISTRTRecv;
       long ISTRTSent;
       long ASTRTRecv;
       long ASTRTSent;
       long AckRecv;
       long PollsSent;
       long MsgRecv;
       long MsgSent;
       long SendRetries;
       long TotalSendRetries;
       long OutChars;
       long InChars;
       long CommResets;
       unsigned char RcvMsgNum;
       unsigned char SndMsgNum;
       long InvalidAddressCount;
       
       if (m_oRRRCom->GetCommStatistics(CurAddress, RetryLimit, ResponseTime,
                                    ISTRTRecv, ISTRTSent, ASTRTRecv, ASTRTSent,
                                    AckRecv, PollsSent, MsgRecv, MsgSent,
                                    SendRetries, TotalSendRetries, OutChars,
                                    InChars, CommResets, RcvMsgNum, SndMsgNum,
                                    InvalidAddressCount) == 0)
       {
          CString csTemp;
          csTemp.Format("%li",RetryLimit);
          m_RetryLimit.SetWindowText(csTemp);
          
          csTemp.Format("%li",ResponseTime);
          m_Timeout.SetWindowText(csTemp);
          
          csTemp.Format("%li",ISTRTRecv);
          m_ISTRTRecv.SetWindowText(csTemp);
          csTemp.Format("%li",ISTRTSent);
          m_ISTRTSent.SetWindowText(csTemp);
          
          csTemp.Format("%li",ASTRTRecv);
          m_ASTRTRecv.SetWindowText(csTemp);
          csTemp.Format("%li",ASTRTSent);
          m_ASTRTSent.SetWindowText(csTemp);
          
          csTemp.Format("%li",AckRecv);
          m_AckRecv.SetWindowText(csTemp);
          csTemp.Format("%li",PollsSent);
          m_PollsSent.SetWindowText(csTemp);
          
          //csTemp.Format("%li",m_oRRRCom->m_lNakRecv);
          //m_NAKRecv.SetWindowText(csTemp);
          //csTemp.Format("%li",m_oRRRCom->m_lNaksSent);
          //m_NAKSent.SetWindowText(csTemp);
          
          csTemp.Format("%li",MsgRecv);
          m_MSGRecv.SetWindowText(csTemp);
          csTemp.Format("%li",MsgSent);
          m_MSGSent.SetWindowText(csTemp);
          
          csTemp.Format("%li",SendRetries);
          m_SendRetries.SetWindowText(csTemp);
          csTemp.Format("%li",TotalSendRetries);
          m_TotalRetries.SetWindowText(csTemp);
          
          csTemp.Format("%li",OutChars);
          m_CharsSent.SetWindowText(csTemp);
          csTemp.Format("%li",InChars);
          m_CharsRecv.SetWindowText(csTemp);
          
          csTemp.Format("%i",SndMsgNum);
          m_SndMsgNum.SetWindowText(csTemp);
          csTemp.Format("%i",RcvMsgNum);
          m_RcvMsgNum.SetWindowText(csTemp);
 
          csTemp.Format("%li",CommResets);
          m_CommResets.SetWindowText(csTemp);
 
          csTemp.Format("%li",InvalidAddressCount);
          m_BadAddress.SetWindowText(csTemp);
       }
    }
 }
 
 /////////////////////////////////////////////////////////////////////////////
 // ProtocolDiagnostics message handlers
 
 BOOL ProtocolDiagnostics::OnInitDialog() 
 {
 	CDialog::OnInitDialog();
 	
 	// TODO: Add extra initialization here
 	
    UpdateDisplay();
 
    SetTimer(DISPTIMEEVENT,1000,NULL);
 
 	return TRUE;  // return TRUE unless you set the focus to a control
 	              // EXCEPTION: OCX Property Pages should return FALSE
 }
 
 void ProtocolDiagnostics::OnTimer(UINT nIDEvent) 
 {
 	// TODO: Add your message handler code here and/or call default
 	UpdateDisplay();
 	CDialog::OnTimer(nIDEvent);
 }
 
 void ProtocolDiagnostics::OnResetcomm() 
 {
    m_oRRRCom->ResetCommunications();
 }
