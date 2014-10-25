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
// Tool.cpp : implementation file
//

#include "stdafx.h"
#include "flowzap.h"
#include "Tool.h"

/////////////////////////////////////////////////////////////////////////////
// CTool dialog


CTool::CTool(CWnd* pParent /*=NULL*/)
    : CDialog(CTool::IDD, pParent)
    , m_Parent(pParent)
{
    //{{AFX_DATA_INIT(CTool)
    //}}AFX_DATA_INIT
}


void CTool::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTool)
    DDX_Control(pDX, IDC_EXITFLOWZAP, m_ExitFZ);
    DDX_Control(pDX, IDC_OPENFILE, m_OpenFile);
    DDX_Control(pDX, IDC_NEWFILE, m_NewFile);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTool, CDialog)
    //{{AFX_MSG_MAP(CTool)
    ON_BN_CLICKED(IDC_NEWFILE, OnNewfile)
    ON_BN_CLICKED(IDC_OPENFILE, OnOpenfile)
    ON_WM_SHOWWINDOW()
    ON_BN_CLICKED(IDC_EXITFLOWZAP, OnExitflowzap)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTool message handlers

BOOL CTool::OnInitDialog()
{
    CDialog::OnInitDialog();

    // CG: The following block was added by the ToolTips component.
    {
        // Create the ToolTip control.
        m_tooltip.Create(this);
        m_tooltip.Activate(TRUE);

        // TODO: Use one of the following forms to add controls:
        // m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
        m_tooltip.AddTool(GetDlgItem(IDC_NEWFILE),     "Click to open a new file");
        m_tooltip.AddTool(GetDlgItem(IDC_OPENFILE),    "Click to open an existing file");
        m_tooltip.AddTool(GetDlgItem(IDC_EXITFLOWZAP), "Click to exit FlowZap");
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CTool::OnOK()
{
    // Comment out following line to keep window from closing when enter is hit.
    //CDialog::OnOK();
}

void CTool::OnCancel()
{
    // Comment out following line to keep window from closing when esc is hit.
    //CDialog::OnCancel();
}

void CTool::OnNewfile()
{
    CFlowZap * fz = (CFlowZap *)m_Parent;
    ShowWindow(SW_HIDE);
    m_NewFile.ShowWindow(SW_HIDE);
    m_OpenFile.ShowWindow(SW_HIDE);
    fz->InitInstructionList();
    fz->m_FileIsOpen = TRUE;

}

void CTool::OnOpenfile()
{
    CFlowZap * fz = (CFlowZap *)m_Parent;
    ShowWindow(SW_HIDE);
    m_NewFile.ShowWindow(SW_HIDE);
    m_OpenFile.ShowWindow(SW_HIDE);
    //	fz->OnFileOpen();
    fz->UpdateWindowPositions();

}

void CTool::OnExitflowzap()
{
    CFlowZap * fz = (CFlowZap *)m_Parent;
    fz->CloseFlowZap();

}

BOOL CTool::PreTranslateMessage(MSG* pMsg)
{
    // CG: The following block was added by the ToolTips component.
    {
        // Let the ToolTip process this message.
        m_tooltip.RelayEvent(pMsg);
    }
    return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

void CTool::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);

    if (bShow)
    {
        m_NewFile.ShowWindow(SW_SHOW);
        m_OpenFile.ShowWindow(SW_SHOW);
        m_ExitFZ.ShowWindow(SW_SHOW);
    }

    else
    {
        m_NewFile.ShowWindow(SW_HIDE);
        m_OpenFile.ShowWindow(SW_HIDE);
        m_ExitFZ.ShowWindow(SW_HIDE);

    }

}


