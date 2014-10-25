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
// Start.cpp : implementation file
//

#include "stdafx.h"
#include "FlowZap.h"
#include "Start.h"
#include "util.h"

/////////////////////////////////////////////////////////////////////////////
// CStart dialog

IMPLEMENT_DYNAMIC(CStart, CInstruction)
CStart::CStart(CWnd* pParent /*=NULL*/, int index)
    : CInstruction(CStart::IDD, pParent, index)
{
    //{{AFX_DATA_INIT(CStart)
    //}}AFX_DATA_INIT
}


void CStart::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CStart)
    DDX_Control(pDX, IDC_BORDER, m_Border);
    DDX_Control(pDX, IDC_IMAGE, m_Image);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStart, CDialog)
    //{{AFX_MSG_MAP(CStart)
    ON_WM_PAINT()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStart message handlers

CString CStart::ToString()
{
    return "Start program";
}

CString CStart::ToFile()
{
    return ToString() + " " + BoolToString(m_Enabled);
}

void CStart::MakeCondensed()
{
    m_Expanded = FALSE;
    m_Image.ShowWindow(SW_HIDE);

    CRect windRect;
    GetWindowRect(&windRect);
    MoveWindow(windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(), 16, TRUE);
    m_Border.MoveWindow(0, 0, windRect.Width(), 16, TRUE);
}

void CStart::MakeExpanded()
{
    m_Expanded = TRUE;
    m_Image.ShowWindow(SW_SHOW);

    CRect windRect;
    GetWindowRect(&windRect);
    MoveWindow(windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(), m_ExpandedHeight, TRUE);
    m_Border.MoveWindow(0, 0, windRect.Width(), m_ExpandedHeight, TRUE);
}

BOOL CStart::OnInitDialog()
{
    CDialog::OnInitDialog();
    CInstruction::OnInit();


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CStart::ShowBorder(BOOL b)
{

}

void CStart::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    CInstruction::OnPaint(&dc);

    // Do not call CDialog::OnPaint() for painting messages
}

void CStart::OnLButtonUp(UINT nFlags, CPoint point)
{
    //CInstruction :: MouseButtonUp (nFlags, point);
    CDialog::OnLButtonUp(nFlags, point);
}

void CStart::OnOK()
{
    // Comment out following line to keep window from closing when enter is hit.
    //CDialog::OnOK();
}

void CStart::OnCancel()
{
    // Comment out following line to keep window from closing when esc is hit.
    //CDialog::OnCancel();
}



void CStart::Enable(BOOL enabled)
{

}

void CStart::OnMouseMove(UINT nFlags, CPoint point)
{
    //((CFlowZap*)m_fc)->UnhighlightIcons();

    CDialog::OnMouseMove(nFlags, point);
}
