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

// DlgFindHomebaseCorner.cpp : implementation file
//

#include "stdafx.h"
#include "robotworld.h"
#include "DlgFindHomebaseCorner.h"
#include "Rothmath.h"
#include "Laygo.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFindHomebaseCorner dialog


CDlgFindHomebaseCorner::CDlgFindHomebaseCorner(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgFindHomebaseCorner::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgFindHomebaseCorner)
    m_radDirection = -1;
    //}}AFX_DATA_INIT
    m_RobotHeading = 0;
    m_wndPrevious = NULL;
    m_wndNext = NULL;
}


void CDlgFindHomebaseCorner::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgFindHomebaseCorner)
    DDX_Control(pDX, IDC_RAD_0, m_rad0);
    DDX_Radio(pDX, IDC_RAD_0, m_radDirection);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFindHomebaseCorner, CDialog)
    //{{AFX_MSG_MAP(CDlgFindHomebaseCorner)
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BUT_BACK, OnButBack)
    ON_BN_CLICKED(IDC_BUT_NEXT, OnButNext)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFindHomebaseCorner message handlers

BOOL CDlgFindHomebaseCorner::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetHomebaseHeading(m_RobotHeading);

    if (m_dcDisplayMemory.GetSafeHdc() == NULL)
    {
        if (!m_dcDisplayMemory.CreateCompatibleDC(GetDC()))
        {
            AfxMessageBox("Relocate Robot CreateCompatibleDC failed");
        }
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFindHomebaseCorner::UpdateHeading(double Heading)
/*******************************************************************************
//Created: 07/15/97 S.R.
//Last Revision: 07/15/97 S.R.
//Parameters:
//	Heading:	robot heading in degrees.
//Return Value:
//	none
********************************************************************************
//Description:
//		Updates the robot bitmap to reflect the correct heading.
//Method:
//
*******************************************************************************/
{
    const unsigned long cBmpFrameWidth = 40;
    const unsigned long cBmpFrameHeight = 40;
    const unsigned long cBmpFramePerLine = 5;
    const unsigned long cTotalFrames = 40;

    CDC* dc = GetDC(); // device context for painting
    //	if (Heading == 360) Heading = 0;
    //	Heading = DegreeToRadian(Heading);
    //	Heading = BringAngleInRange(Heading);

    CRect rcRect;
    int Frame = RobotHeadingToBitmapFrame(Heading, cTotalFrames);
    rcRect.left   = Frame % cBmpFramePerLine * cBmpFrameWidth;
    rcRect.top    = Frame / cBmpFramePerLine * cBmpFrameHeight;
    rcRect.right  = rcRect.left + cBmpFrameWidth;
    rcRect.bottom = rcRect.top + cBmpFrameHeight;
    CRect ClientRect;
    GetClientRect(&ClientRect);

    /*Blit Robot*/
    ClientRect.left = ClientRect.Width() / 2 - cBmpFrameWidth / 2;
    CRect RadioClientRect;
    m_rad0.GetWindowRect(&RadioClientRect);
    ScreenToClient(&RadioClientRect);
    ClientRect.top = (RadioClientRect.top + RadioClientRect.bottom) / 2 - cBmpFrameHeight / 2;
    //	ClientRect.top = ClientRect.Height()/2 - cBmpFrameHeight / 2;

    CBitmap* temp = m_dcDisplayMemory.SelectObject(CBitmap::FromHandle(m_hbmRobotBitmap));

    if (temp == NULL)
    {
        AfxMessageBox("FindHomebaseCorner SelectObject Error");
    }

    dc->BitBlt(ClientRect.left, ClientRect.top, rcRect.Width(), rcRect.Height(), &m_dcDisplayMemory, rcRect.left, rcRect.top, SRCCOPY);
    m_dcDisplayMemory.SelectObject(temp);
}

void CDlgFindHomebaseCorner::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    UpdateHeading(m_RobotHeading);

    // Do not call CDialog::OnPaint() for painting messages
}

void CDlgFindHomebaseCorner::SetHomebaseHeading(double Heading)
{
    CWnd* NextRadio;
    m_RobotHeading = Heading;
    UpdateData(TRUE);

    if (m_rad0.m_hWnd == NULL)
    {
        return;
    }

    if (m_RobotHeading == 0)
    {
        if ((m_radDirection != 1) && (m_radDirection != 3))
        {
            m_radDirection = 1;
        }

        m_rad0.EnableWindow(false);
        m_rad0.ShowWindow(SW_HIDE);
        NextRadio = GetDlgItem(IDC_RAD_90);
        NextRadio->EnableWindow(true);
        NextRadio->ShowWindow(SW_SHOW);
        NextRadio = GetDlgItem(IDC_RAD_180);
        NextRadio->EnableWindow(false);
        NextRadio->ShowWindow(SW_HIDE);
        NextRadio = GetDlgItem(IDC_RAD_270);
        NextRadio->EnableWindow(true);
        NextRadio->ShowWindow(SW_SHOW);
    }
    else if (m_RobotHeading == pi / 2)
    {
        if ((m_radDirection != 0) && (m_radDirection != 2))
        {
            m_radDirection = 0;
        }

        m_rad0.EnableWindow(true);
        m_rad0.ShowWindow(SW_SHOW);
        NextRadio = GetDlgItem(IDC_RAD_90);
        NextRadio->EnableWindow(false);
        NextRadio->ShowWindow(SW_HIDE);
        NextRadio = GetDlgItem(IDC_RAD_180);
        NextRadio->EnableWindow(true);
        NextRadio->ShowWindow(SW_SHOW);
        NextRadio = GetDlgItem(IDC_RAD_270);
        NextRadio->EnableWindow(false);
        NextRadio->ShowWindow(SW_HIDE);
    }
    else if (m_RobotHeading == pi)
    {
        if ((m_radDirection != 1) && (m_radDirection != 3))
        {
            m_radDirection = 1;
        }

        m_rad0.EnableWindow(false);
        m_rad0.ShowWindow(SW_HIDE);
        NextRadio = GetDlgItem(IDC_RAD_90);
        NextRadio->EnableWindow(true);
        NextRadio->ShowWindow(SW_SHOW);
        NextRadio = GetDlgItem(IDC_RAD_180);
        NextRadio->EnableWindow(false);
        NextRadio->ShowWindow(SW_HIDE);
        NextRadio = GetDlgItem(IDC_RAD_270);
        NextRadio->EnableWindow(true);
        NextRadio->ShowWindow(SW_SHOW);
    }
    else if (m_RobotHeading == 3 * pi / 2)
    {
        if ((m_radDirection != 0) && (m_radDirection != 2))
        {
            m_radDirection = 0;
        }

        m_rad0.EnableWindow(true);
        m_rad0.ShowWindow(SW_SHOW);
        NextRadio = GetDlgItem(IDC_RAD_90);
        NextRadio->EnableWindow(false);
        NextRadio->ShowWindow(SW_HIDE);
        NextRadio = GetDlgItem(IDC_RAD_180);
        NextRadio->EnableWindow(true);
        NextRadio->ShowWindow(SW_SHOW);
        NextRadio = GetDlgItem(IDC_RAD_270);
        NextRadio->EnableWindow(false);
        NextRadio->ShowWindow(SW_HIDE);
    }

    UpdateData(FALSE);
}

bool CDlgFindHomebaseCorner::GetCornerToLeft()
{
    bool left;

    if (m_hWnd != NULL)
    {
        UpdateData(true);
    }

    if (m_RobotHeading == 0)
    {
        switch (m_radDirection)
        {
        case 1:
            left = true;
            break;

        case 3:
            left = false;
            break;
        }
    }
    else if (m_RobotHeading == pi / 2)
    {
        switch (m_radDirection)
        {
        case 0:
            left = false;
            break;

        case 2:
            left = true;
            break;
        }
    }
    else if (m_RobotHeading == pi)
    {
        switch (m_radDirection)
        {
        case 1:
            left = false;
            break;

        case 3:
            left = true;
            break;
        }
    }
    else if (m_RobotHeading == 3 * pi / 2)
    {
        switch (m_radDirection)
        {
        case 0:
            left = true;
            break;

        case 2:
            left = false;
            break;
        }
    }

    return left;
}

void CDlgFindHomebaseCorner::OnButBack()
{
    ShowWindow(SW_HIDE);
    ASSERT(m_wndPrevious != NULL);
    m_wndPrevious->ShowWindow(SW_SHOW);
    m_Laygo->LoadTipText("HomeBaseTip1.rtf");
}

double CDlgFindHomebaseCorner::GetHomebaseHeading()
{
    return m_RobotHeading;
}

void CDlgFindHomebaseCorner::OnButNext()
{
    ShowWindow(SW_HIDE);
    ASSERT(m_wndNext != NULL);
    m_wndNext->ShowWindow(SW_SHOW);
    m_Laygo->LoadTipText("HomeBaseTip3.rtf");
}
