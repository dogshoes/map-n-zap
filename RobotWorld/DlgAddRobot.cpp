
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
// DlgAddRobot.cpp : implementation file
//

#include "stdafx.h"
#include "robotworld.h"
#include "DlgAddRobot.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRobot dialog


CDlgAddRobot::CDlgAddRobot(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgAddRobot::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgAddRobot)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_BmpRobotColors[0] = &m_BmpOrange;
    m_BmpRobotColors[1] = &m_BmpYellow;
    m_BmpRobotColors[2] = &m_BmpBlack;
    m_BmpRobotColors[3] = &m_BmpChrome;
    m_HighlightedBmp = m_BmpRobotColors[0];
}


void CDlgAddRobot::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgAddRobot)
    DDX_Control(pDX, IDC_COM_ROBOT_STYLE, m_comRobotStyle);
    DDX_Control(pDX, IDC_BMP_YELLOW, m_BmpYellow);
    DDX_Control(pDX, IDC_BMP_ORANGE, m_BmpOrange);
    DDX_Control(pDX, IDC_BMP_CHROME, m_BmpChrome);
    DDX_Control(pDX, IDC_BMP_BLACK, m_BmpBlack);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddRobot, CDialog)
    //{{AFX_MSG_MAP(CDlgAddRobot)
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    ON_CBN_SELCHANGE(IDC_COM_ROBOT_STYLE, OnSelchangeComRobotStyle)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRobot message handlers

void CDlgAddRobot::OnLButtonDown(UINT nFlags, CPoint point)
{
    CRect WindowRect;

    for (char i = 0; i < cNumRobotColors; i++)
    {
        m_BmpRobotColors[i]->GetWindowRect(&WindowRect);
        ScreenToClient(&WindowRect);

        if (WindowRect.PtInRect(point))
        {
            if (m_BmpRobotColors[i]->IsWindowVisible())
            {
                m_HighlightedBmp = m_BmpRobotColors[i];
                RedrawWindow();
            }

            break;
        }
    }

    CDialog::OnLButtonDown(nFlags, point);
}

short CDlgAddRobot::GetRobotColor()
{
    if (m_HighlightedBmp == &m_BmpBlack)
    {
        return 0;
    }

    if (m_HighlightedBmp == &m_BmpOrange)
    {
        return 1;
    }

    if (m_HighlightedBmp == &m_BmpYellow)
    {
        return 2;
    }

    if (m_HighlightedBmp == &m_BmpChrome)
    {
        return 3;
    }

    ASSERT(FALSE);
    return 0;
}

void CDlgAddRobot::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    HighlightBitmap(&dc, m_HighlightedBmp);
}

void CDlgAddRobot::HighlightBitmap(CDC *pDC, CStatic *Bitmap)
{
    const int cPenWidth = 4;

    if (Bitmap == NULL)
    {
        return;
    }

    CRect WindowRect;
    Bitmap->GetWindowRect(&WindowRect);
    ScreenToClient(&WindowRect);
    CPen NewPen(PS_SOLID, cPenWidth, COLORREF(0));
    CPen* OldPen = pDC->SelectObject(&NewPen);
    CPoint P1, P2;
    P1.x = WindowRect.left - cPenWidth;
    P1.y = WindowRect.top - cPenWidth;
    P2.y = P1.y;
    P2.x = WindowRect.right + cPenWidth;
    pDC->MoveTo(P1);
    pDC->LineTo(P2);
    P2.y = WindowRect.bottom + cPenWidth;
    pDC->LineTo(P2);
    P2.x = WindowRect.left - cPenWidth;
    pDC->LineTo(P2);
    pDC->LineTo(P1);
    pDC->SelectObject(OldPen);
}

int CDlgAddRobot::GetRobotVersion()
{
    return m_RobotStyle;
}

BOOL CDlgAddRobot::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_RobotStyle = m_comRobotStyle.GetCount();
    m_comRobotStyle.SetCurSel(m_comRobotStyle.GetCount() - 1);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAddRobot::OnSelchangeComRobotStyle()
{
    m_RobotStyle = m_comRobotStyle.GetCurSel();

    if (m_RobotStyle == CB_ERR)
    {
        m_RobotStyle = 0;
    }

    m_RobotStyle++;
}
