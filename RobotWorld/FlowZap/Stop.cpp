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
// Stop.cpp : implementation file
//

#include "stdafx.h"
#include "FlowZap.h"
#include "FlowZapResource.h"
#include "Stop.h"
#include "util.h"

/////////////////////////////////////////////////////////////////////////////
// CStop dialog

IMPLEMENT_DYNAMIC(CStop, CInstruction)
CStop::CStop(CWnd* pParent /*=NULL*/, int index)
    : CInstruction(CStop::IDD, pParent, index)
{
    //{{AFX_DATA_INIT(CStop)
    //}}AFX_DATA_INIT
}


void CStop::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CStop)
    DDX_Control(pDX, IDC_IMAGE_DISABLED, m_DisabledImage);
    DDX_Control(pDX, IDC_EBORDER, m_EtchedBorder);
    DDX_Control(pDX, IDC_BBORDER, m_BlackBorder);
    DDX_Control(pDX, IDC_IMAGE, m_Image);
    DDX_Control(pDX, IDC_TEXT1, m_Text1);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStop, CDialog)
    //{{AFX_MSG_MAP(CStop)
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    ON_WM_LBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStop message handlers

BOOL CStop::OnInitDialog()
{
    CDialog::OnInitDialog();
    CInstruction::OnInit();

    CRect windRect;
    GetWindowRect(&windRect);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), windRect.Height(), TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), windRect.Height(), TRUE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CStop::OnLButtonDown(UINT nFlags, CPoint point)
{
    CInstruction :: MouseClick(point);

    CDialog::OnLButtonDown(nFlags, point);
}


void CStop::OnLButtonUp(UINT nFlags, CPoint point)
{
    CInstruction :: MouseButtonUp(nFlags, point);
    CDialog::OnLButtonUp(nFlags, point);
}


void CStop::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CInstruction::OnPaint(&dc);

    // Do not call CDialog::OnPaint() for painting messages
}

void CStop::OnOK()
{
    // Comment out following line to keep window from closing when enter is hit.
    //CDialog::OnOK();
}

void CStop::OnCancel()
{
    // Comment out following line to keep window from closing when esc is hit.
    //CDialog::OnCancel();
}

CString CStop::ToString()
{
    return "Stop program";
}

CString CStop::ToFile()
{
    return ToString() + " " + BoolToString(m_Enabled);
}


void CStop::MakeCondensed()
{
    if (IsSelected())
    {
        return;
    }

    m_Expanded = FALSE;
    m_Image.ShowWindow(SW_HIDE);
    m_DisabledImage.ShowWindow(SW_HIDE);
    m_Text1.ShowWindow(SW_HIDE);

    CRect windRect;
    GetWindowRect(&windRect);
    MoveWindow(windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(), 16, TRUE);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), 16, TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), 16, TRUE);
}


void CStop::MakeExpanded()
{
    m_Expanded = TRUE;

    if (!m_Enabled)
    {
        m_DisabledImage.ShowWindow(SW_SHOW);
    }
    else
    {
        m_Image.ShowWindow(SW_SHOW);
    }

    m_Text1.ShowWindow(SW_SHOW);

    CRect windRect;
    GetWindowRect(&windRect);
    MoveWindow(windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(), m_ExpandedHeight, TRUE);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), m_ExpandedHeight, TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), m_ExpandedHeight, TRUE);
}





void CStop::ShowBorder(BOOL raised)
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

void CStop::Enable(BOOL enabled)
{
    m_Enabled = enabled;
    m_Text1.EnableWindow(enabled);

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
