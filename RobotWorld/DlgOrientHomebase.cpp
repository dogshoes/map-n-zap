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

// DlgOrientHomebase.cpp : implementation file
//

#include "stdafx.h"
#include "robotworld.h"
#include "DlgOrientHomebase.h"
#include "Rothmath.h"
#include "Laygo.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgOrientHomebase dialog


CDlgOrientHomebase::CDlgOrientHomebase(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgOrientHomebase::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgOrientHomebase)
    m_radDirection = -1;
    //}}AFX_DATA_INIT
}


void CDlgOrientHomebase::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgOrientHomebase)
    DDX_Control(pDX, IDC_RAD_0, m_rad0);
    DDX_Radio(pDX, IDC_RAD_0, m_radDirection);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOrientHomebase, CDialog)
    //{{AFX_MSG_MAP(CDlgOrientHomebase)
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_RAD_0, OnRad0)
    ON_BN_CLICKED(IDC_RAD_180, OnRad180)
    ON_BN_CLICKED(IDC_RAD_270, OnRad270)
    ON_BN_CLICKED(IDC_RAD_90, OnRad90)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOrientHomebase message handlers

BOOL CDlgOrientHomebase::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_RobotHeading = 0;
    m_radDirection = 0;
    UpdateData(FALSE);

    if (m_dcDisplayMemory.GetSafeHdc() == NULL)
    {
        if (!m_dcDisplayMemory.CreateCompatibleDC(GetDC()))
        {
            AfxMessageBox("Relocate Robot CreateCompatibleDC failed");
        }

        //		m_OldBitmap = m_dcDisplayMemory->GetCurrentBitmap();
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOrientHomebase::UpdateHeading(double Heading)
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
        AfxMessageBox("OrientHomebase SelectObject Error");
    }

    dc->BitBlt(ClientRect.left, ClientRect.top, rcRect.Width(), rcRect.Height(), &m_dcDisplayMemory, rcRect.left, rcRect.top, SRCCOPY);
    m_dcDisplayMemory.SelectObject(temp);
}

void CDlgOrientHomebase::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    UpdateHeading(m_RobotHeading);
    // Do not call CDialog::OnPaint() for painting messages
}

void CDlgOrientHomebase::OnRad0()
{
    m_RobotHeading = 0;
    RedrawWindow();
}

void CDlgOrientHomebase::OnRad180()
{
    m_RobotHeading = pi;
    RedrawWindow();
}

void CDlgOrientHomebase::OnRad270()
{
    m_RobotHeading = 3 * pi / 2;
    RedrawWindow();
}

void CDlgOrientHomebase::OnRad90()
{
    m_RobotHeading = pi / 2;
    RedrawWindow();
}

double CDlgOrientHomebase::GetHomebaseHeading()
{
    return m_RobotHeading;
}

void CDlgOrientHomebase::OnOK()
{
    //	CBitmap blah;
    //	blah.CreateBitmap(10,10,1,1,NULL)
    //	CBitmap* temp = NULL;
    //	temp = m_dcDisplayMemory.SelectObject(m_OldBitmap);
    m_wndNext->SetHomebaseHeading(m_RobotHeading);
    m_wndNext->ShowWindow(SW_SHOW);
    m_Laygo->LoadTipText("HomeBaseTip2.rtf");
    CDialog::OnOK();
}
