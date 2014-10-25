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
// PropRobotColor.cpp : implementation file
//

#include "stdafx.h"
#include "robotworld.h"
#include "PropRobotColor.h"

/////////////////////////////////////////////////////////////////////////////
// CPropRobotColor property page

IMPLEMENT_DYNCREATE(CPropRobotColor, CPropertyPage)

CPropRobotColor::CPropRobotColor() : CPropertyPage(CPropRobotColor::IDD)
{
    //{{AFX_DATA_INIT(CPropRobotColor)
    //}}AFX_DATA_INIT
    m_BmpRobotColors[0] = &m_Bmp000;
    m_BmpRobotColors[1] = &m_Bmp011;
    m_BmpRobotColors[2] = &m_Bmp022;
    m_BmpRobotColors[3] = &m_Bmp033;
    m_BmpRobotColors[4] = &m_Bmp044;
    m_BmpRobotColors[5] = &m_Bmp200;
    m_BmpRobotColors[6] = &m_Bmp300;
    m_BmpRobotColors[7] = &m_BmpChrome;
    m_BmpRobotColors[8] = &m_Bmp08;
    m_HighlightedBmp = m_BmpRobotColors[0];
}

CPropRobotColor::~CPropRobotColor()
{
}

void CPropRobotColor::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropRobotColor)
    DDX_Control(pDX, IDC_BMP_CHROME, m_BmpChrome);
    DDX_Control(pDX, IDC_BMP08, m_Bmp08);
    DDX_Control(pDX, IDC_BMP300, m_Bmp300);
    DDX_Control(pDX, IDC_BMP200, m_Bmp200);
    DDX_Control(pDX, IDC_BMP044, m_Bmp044);
    DDX_Control(pDX, IDC_BMP033, m_Bmp033);
    DDX_Control(pDX, IDC_BMP022, m_Bmp022);
    DDX_Control(pDX, IDC_BMP011, m_Bmp011);
    DDX_Control(pDX, IDC_BMP000, m_Bmp000);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropRobotColor, CPropertyPage)
    //{{AFX_MSG_MAP(CPropRobotColor)
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropRobotColor message handlers

void CPropRobotColor::OnLButtonDown(UINT nFlags, CPoint point)
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

    CPropertyPage::OnLButtonDown(nFlags, point);
}

void CPropRobotColor::HighlightBitmap(CDC* pDC, CStatic * Bitmap)
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

void CPropRobotColor::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    HighlightBitmap(&dc, m_HighlightedBmp);
    // Do not call CPropertyPage::OnPaint() for painting messages
}

void CPropRobotColor::SetRobotColor(int RobotColor)
{
    switch (RobotColor)
    {
    case 0:
        m_HighlightedBmp = &m_Bmp011;
        break;

    case 1:
        m_HighlightedBmp = &m_Bmp200;
        break;

    case 2:
        m_HighlightedBmp = &m_Bmp300;
        break;

    case 3:
        m_HighlightedBmp = &m_BmpChrome;
        break;

    default:
        ASSERT(FALSE);
        m_HighlightedBmp = &m_Bmp011;
        break;
    }
}

int CPropRobotColor::GetRobotColor()
{
    if (m_HighlightedBmp == &m_Bmp011)
    {
        return 0;
    }

    if (m_HighlightedBmp == &m_Bmp200)
    {
        return 1;
    }

    if (m_HighlightedBmp == &m_Bmp300)
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
