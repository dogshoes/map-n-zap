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

// Execute.cpp : implementation file
//

#include "stdafx.h"
#include "FlowZap.h"
#include "Execute.h"
#include "Instruction.h"
#include "Stop.h"
#include "Goto.h"
#include "Beep.h"
#include "Calibrate.h"
#include "Vacuum.h"
#include "Wait.h"
#include "Branch.h"

#ifdef ROBOT_WORLD_PROJECT
#include "Robot.h"
#include "ExecutableRWGraphicObject.h"
//#include "Label.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CExecute dialog


CExecute::CExecute(CWnd* pParent /*=NULL*/, CRobot* Robot, CExecutableRWGraphicObjectList* ExecutableObjectList)
    : CDialog(CExecute::IDD, pParent)
{
    //{{AFX_DATA_INIT(CExecute)
    //}}AFX_DATA_INIT
    m_Pause = false;
    m_Parent = pParent;
    m_Robot = Robot;
    m_ExecutableObjectList = ExecutableObjectList;
}


void CExecute::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CExecute)
    DDX_Control(pDX, IDB_PLAY_PAUSE, m_PauseButton);
    DDX_Control(pDX, IDB_PLAY_STOP, m_StopButton);
    DDX_Control(pDX, IDB_PLAY_START, m_PlayButton);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExecute, CDialog)
    //{{AFX_MSG_MAP(CExecute)
    ON_BN_CLICKED(IDB_PLAY_PAUSE, OnPlayPause)
    ON_BN_CLICKED(IDB_PLAY_START, OnPlayStart)
    ON_BN_CLICKED(IDB_PLAY_STOP, OnPlayStop)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExecute message handlers

BOOL CExecute::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_PauseButton.SetIcon(::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_PLAY_PAUSE)));
    m_PlayButton.SetIcon(::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_PLAY_START)));
    m_StopButton.SetIcon(::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_PLAY_STOP)));
    //m_PauseButton.EnableWindow(FALSE);
    //m_StopButton.EnableWindow(FALSE);

    // CG: The following block was added by the ToolTips component.
    {
        // Create the ToolTip control.
        m_tooltip.Create(this, TTS_ALWAYSTIP);
        m_tooltip.Activate(TRUE);

        // TODO: Use one of the following forms to add controls:
        // m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
        m_tooltip.AddTool(GetDlgItem(IDB_PLAY_STOP), "Stop Execution of Program");
        m_tooltip.AddTool(GetDlgItem(IDB_PLAY_START), "Start Execution of Program");
        m_tooltip.AddTool(GetDlgItem(IDB_PLAY_PAUSE), "Pause Execution of Program");
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CExecute::OnPlayPause()
{
    m_PlayButton.EnableWindow(TRUE);
    m_PlayButton.SetFocus();
    m_PauseButton.EnableWindow(TRUE);
    m_StopButton.EnableWindow(TRUE);
    m_Pause = !m_Pause;
#ifdef ROBOT_WORLD_PROJECT
    m_Robot->Pause(m_Pause);
#endif
}
#ifdef ROBOT_WORLD_PROJECT

void CExecute::OnPlayStart()
{
    if (m_Pause)
    {
        m_Pause = false;
        m_Robot->Pause(false);
        return;
    }

    m_PlayButton.EnableWindow(FALSE);
    m_PauseButton.EnableWindow(TRUE);
    m_PauseButton.SetFocus();
    m_StopButton.EnableWindow(TRUE);

    CFlowZap *fz = (CFlowZap *)m_Parent;
    CInstruction* in = fz->Play();
    bool result;
    bool stop = false;
    bool OldEStopVisible = m_Robot->GetEStopVisible();
    m_Robot->SetEStopVisible(true);

    while (!stop)
    {
        if (in->IsKindOf(RUNTIME_CLASS(CStop)))
        {
            stop = true;
        }
        else
        {
            //		if (ExecuteInstruction(in, result) != TR_TaskComplete) {
#ifdef _DEBUG
            afxDump << in->ToString() + "\n";
#endif

            if (in->Execute(m_Robot, m_ExecutableObjectList, result) != TR_TaskComplete)
            {
                stop = true;
            }
            else
            {
                in = fz->GetInstructionToExecute(result);
            }

            ASSERT(in != NULL);
        }
    }

    OnPlayStop();
    m_Robot->SetEStopVisible(OldEStopVisible);
}

#else

void CExecute::OnPlayStart() {}

#endif

void CExecute::OnPlayStop()
{
    m_PlayButton.EnableWindow(TRUE);
    m_PlayButton.SetFocus();
    m_PauseButton.EnableWindow(FALSE);
    m_StopButton.EnableWindow(FALSE);
    ((CFlowZap*)m_Parent)->Stop();
#ifdef ROBOT_WORLD_PROJECT
    m_Robot->EStop();
    m_Robot->SetBuzzerFrequency(0);
#endif
}

BOOL CExecute::PreTranslateMessage(MSG* pMsg)
{
    // CG: The following block was added by the ToolTips component.
    {
        // Let the ToolTip process this message.
        m_tooltip.RelayEvent(pMsg);
    }
    return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}
