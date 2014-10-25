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

// ExternalOutput.cpp : implementation file
//

#include "stdafx.h"
#include "flowzap.h"
#include "ExternalOutput.h"
#include "util.h"

/////////////////////////////////////////////////////////////////////////////
// CExternalOutput dialog


CExternalOutput::CExternalOutput(CWnd* pParent /*=NULL*/, int index)
    : CInstruction(CExternalOutput::IDD, pParent, index)
{
    //{{AFX_DATA_INIT(CExternalOutput)
    m_strExternalOutput = _T("");
    //}}AFX_DATA_INIT
}


void CExternalOutput::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CExternalOutput)
    DDX_Control(pDX, IDC_BBORDER, m_BlackBorder);
    DDX_Control(pDX, IDC_EBORDER, m_EtchedBorder);
    DDX_Control(pDX, IDC_TXT_EXTERNAL_OUTPUT, m_Text);
    DDX_Control(pDX, IDC_IMAGE_DISABLED, m_DisabledImage);
    DDX_Control(pDX, IDC_IMAGE, m_Image);
    DDX_Control(pDX, IDC_COM_EXTERNAL_OUTPUT, m_comExternalOutput);
    DDX_CBString(pDX, IDC_COM_EXTERNAL_OUTPUT, m_strExternalOutput);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExternalOutput, CInstruction)
    //{{AFX_MSG_MAP(CExternalOutput)
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExternalOutput message handlers

void CExternalOutput::OnOK()
{
    //	CInstruction::OnOK();
}

void CExternalOutput::OnCancel()
{
    //	CInstruction::OnCancel();
}

void CExternalOutput::OnLButtonUp(UINT nFlags, CPoint point)
{
    CInstruction :: MouseButtonUp(nFlags, point);

    CInstruction::OnLButtonUp(nFlags, point);
}

void CExternalOutput::OnLButtonDown(UINT nFlags, CPoint point)
{
    CInstruction :: MouseClick(point);

    CInstruction::OnLButtonDown(nFlags, point);
}

void CExternalOutput::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CInstruction::OnPaint(&dc);

    // Do not call CInstruction::OnPaint() for painting messages
}

CString CExternalOutput::ToString()
{
    CString string;

    if (m_comExternalOutput.GetCurSel() == 0)
    {
        string = "Turn External Output On";
    }
    else
    {
        string = "Turn External Output Off";
    }

    return string;
}

CString CExternalOutput::ToFile()
{
    return ToString() + " " + BoolToString(m_Enabled);
}

void CExternalOutput::MakeCondensed()
{
    if (IsSelected())
    {
        return;
    }

    m_Expanded = FALSE;
    m_Image.ShowWindow(SW_HIDE);
    m_DisabledImage.ShowWindow(SW_HIDE);
    m_comExternalOutput.ShowWindow(SW_HIDE);
    m_Text.ShowWindow(SW_HIDE);
    CRect windRect;
    GetWindowRect(&windRect);
    MoveWindow(windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(), 16, TRUE);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), 16, TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), 16, TRUE);
}


void CExternalOutput::MakeExpanded()
{
    m_Expanded = TRUE;

    if (!m_Enabled)
    {
        m_DisabledImage.ShowWindow(SW_SHOW);
        m_Image.ShowWindow(SW_HIDE);
    }
    else
    {
        m_DisabledImage.ShowWindow(SW_HIDE);
        m_Image.ShowWindow(SW_SHOW);
    }

    m_comExternalOutput.ShowWindow(SW_SHOW);
    m_Text.ShowWindow(SW_SHOW);
    CRect windRect;
    GetWindowRect(&windRect);
    MoveWindow(windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(), m_ExpandedHeight, TRUE);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), m_ExpandedHeight, TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), m_ExpandedHeight, TRUE);
}

void CExternalOutput::ShowBorder(BOOL raised)
{
    if (raised)
    {
        m_BlackBorder.ShowWindow(SW_HIDE);
        m_EtchedBorder.ShowWindow(SW_SHOW);
    }
    else
    {
        m_BlackBorder.ShowWindow(SW_SHOW);
        m_EtchedBorder.ShowWindow(SW_HIDE);
    }
}

void CExternalOutput::Enable(BOOL enabled)
{
    m_Enabled = enabled;

    m_comExternalOutput.EnableWindow(enabled);
    m_Text.EnableWindow(enabled);

    if (m_Expanded)
    {
        if (m_Enabled)
        {
            m_DisabledImage.ShowWindow(SW_HIDE);
            m_Image.ShowWindow(SW_SHOW);
        }
        else
        {
            m_DisabledImage.ShowWindow(SW_SHOW);
            m_Image.ShowWindow(SW_HIDE);
        }
    }

    RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
}

BOOL CExternalOutput::OnInitDialog()
{
    CInstruction::OnInitDialog();

    CRect windRect;
    GetWindowRect(&windRect);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), windRect.Height(), TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), windRect.Height(), TRUE);

    m_comExternalOutput.ResetContent();
    m_comExternalOutput.AddString("On");
    m_comExternalOutput.AddString("Off");
    m_comExternalOutput.SetCurSel(0);
    CInstruction::OnInit();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CExternalOutput::SetOutputOn(bool On)
{
    if (On)
    {
        m_comExternalOutput.SetCurSel(0);
    }
    else
    {
        m_comExternalOutput.SetCurSel(1);
    }
}

#ifdef ROBOT_WORLD_PROJECT
TTaskResult CExternalOutput::Execute(CRobot * Robot, CExecutableRWGraphicObjectList* ExecutableObjectList, bool & result)
{
    result = true;
    Robot->SetExternalOutput(m_comExternalOutput.GetCurSel() == 0);
    return TR_TaskComplete;
}
#endif
