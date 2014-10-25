
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
// DlgAddHomebase.cpp : implementation file
//

#include "stdafx.h"
#include "robotworld.h"
#include "DlgAddHomebase.h"
#include "Robot.h"
#include "RobotWorldView.h"
#include "Probability Grid.h"
#include "DlgErrorCheckingWoodWithVacuum.h"
#include "HomeBase.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAddHomeBase dialog

const unsigned long cBmpFrameWidth = 40;
const unsigned long cBmpFrameHeight = 40;
const unsigned long cBmpFramePerLine = 5;
const unsigned long cTotalFrames = 40;

const char *cAddHomeBaseInstructions = "  Before creating the new home base, make sure that Cye is checked in.  Then drag Cye onto the new home base.  An easy way to get Cye on the new home base is by dragging Cye to a position close to and in front of the home base.  Then go to Cye and push him the rest of the way.  While pushing Cye, push down slightly to make sure the wheels do not slip.  Make sure Cye is on the home base by checking that the battery charging indicator is on.\r\n  Once Cye is on the new home base, choose the appropriate floor type and the direction to the home base's corner.  Press OK to create the new home base.";
CDlgAddHomeBase::CDlgAddHomeBase(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgAddHomeBase::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgAddHomeBase)
    m_radFloorType = 1;
    m_radLeft = 0;
    m_edtAddHomeBaseInstructions = _T("");
    //}}AFX_DATA_INIT
}


void CDlgAddHomeBase::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgAddHomeBase)
    DDX_Radio(pDX, IDC_WOOD_FLOOR, m_radFloorType);
    DDX_Radio(pDX, IDC_RAD_LEFT, m_radLeft);
    DDX_Text(pDX, IDC_EDT_ADD_HOMEBASE_INSTRUCTIONS, m_edtAddHomeBaseInstructions);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddHomeBase, CDialog)
    //{{AFX_MSG_MAP(CDlgAddHomeBase)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddHomeBase message handlers


void CDlgAddHomeBase::OnOK()
{
    CDialog::OnOK();
    CRobot* Robot = m_View->GetActiveRobot();

    if (Robot != NULL)
    {
        TRobotInfo RobotInfo = Robot->GetRobotInfo();

        switch (m_radFloorType)
        {
        case 0:
            Robot->SetFloorType(ftWood);
            break;

        case 1:
            Robot->SetFloorType(ftThinCarpet);
            break;

        case 2:
            Robot->SetFloorType(ftThickCarpet);
            break;
        }

        if ((Robot->GetFloorType() == ftWood) && (Robot->GetAttachmentType() == atVacuum))
        {
            CDlgErrorCheckingWoodWithVacuum Warning;
            Warning.DoModal();
            return;
        }

        CHomeBase* HomeBase = new CHomeBase;
        bool NeedToAdjust = !Robot->GetOnHomeBase();

        if (HomeBase->Create(Robot, m_radLeft == 0, 0, false))
        {
            m_View->m_ProbabilityGrid->m_ExecutableObjectList->Add(HomeBase);
            m_View->m_ProbabilityGrid->m_RWGraphicObjectList->Add(HomeBase);
            HomeBase->SetCaption("HomeBase");

            if (NeedToAdjust)
            {
                HomeBase->Adjust(Robot);
            }

            //			CalZone->EditProperties();
        }
        else
        {
            delete HomeBase;
        }

        Robot->SetRobotInfo(RobotInfo);
    }

    m_View->RedrawWindow();
    m_View->SetAddingHomeBase(false);
    CDialog::OnOK();
}

void CDlgAddHomeBase::SetRobotHeading(double Heading)
{
    m_RobotHeading = Heading;

    CRect ClientRect;
    CWnd* pwnd = GetDlgItem(IDC_FRM_BITMAP);
    pwnd->GetWindowRect(&ClientRect);
    ScreenToClient(&ClientRect);
    int xCenter = (ClientRect.left + ClientRect.right) / 2;
    int yCenter = (ClientRect.top + ClientRect.bottom) / 2;

    pwnd = GetDlgItem(IDC_RAD_LEFT);
    pwnd->GetClientRect(&ClientRect);
    int y;
    y = int(yCenter - cBmpFrameHeight * sin(Heading + pi / 2) + ClientRect.Height() / 2);
    pwnd->SetWindowPos(NULL, int(xCenter + cBmpFrameWidth * cos(Heading + pi / 2) - ClientRect.Width() / 2), int(yCenter - cBmpFrameHeight * sin(Heading + pi / 2) - ClientRect.Height() / 2), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    pwnd = GetDlgItem(IDC_RAD_RIGHT);
    pwnd->GetClientRect(&ClientRect);
    y = int(yCenter - cBmpFrameHeight * sin(Heading - pi / 2) + ClientRect.Height() / 2);
    pwnd->SetWindowPos(NULL, int(xCenter + cBmpFrameWidth * cos(Heading - pi / 2) - ClientRect.Width() / 2), int(yCenter - cBmpFrameHeight * sin(Heading - pi / 2) - ClientRect.Height() / 2), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void CDlgAddHomeBase::SetRobotBitmap(HBITMAP RobotBitmap)
{
    m_hbmRobotBitmap = RobotBitmap;
}

BOOL CDlgAddHomeBase::OnInitDialog()
{
    CDialog::OnInitDialog();

    //	m_scrCornerSize.SetScrollRange(0, cMaxScrollCornerSize);
    //	SetCornerSize(m_CornerSize);
    if (m_dcDisplayMemory.GetSafeHdc() == NULL)
    {
        if (!m_dcDisplayMemory.CreateCompatibleDC(GetDC()))
        {
            AfxMessageBox("CreateCalibrationWall1 CreateCompatibleDC failed");
        }
    }

    UpdateData(TRUE);
    //	char chars[130];
    //	for (unsigned char i = 1; i < 60; i++) {
    //		chars[2*i] = 49+i;
    //		chars[2*i+1] = i;
    //	}
    //	chars[i] = 0;
    //	m_edtAddHomeBaseInstructions = chars;
    m_edtAddHomeBaseInstructions = cAddHomeBaseInstructions;
    UpdateData(FALSE);
    //	GetWindowRect(&m_OriginalRect);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAddHomeBase::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    DrawRobot(m_RobotHeading);
    // Do not call CDialog::OnPaint() for painting messages
}

void CDlgAddHomeBase::DrawRobot(double Heading)
/*******************************************************************************
********************************************************************************
//Description:
//		Updates the robot bitmap to reflect the correct heading.
//Method:
//
*******************************************************************************/
{
    UpdateData(TRUE);
    //	if (!m_chkCornerCalibration) return;
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

    /*Blit Robot*/
    CRect ClientRect;
    CWnd* pwnd = GetDlgItem(IDC_FRM_BITMAP);
    pwnd->GetWindowRect(&ClientRect);
    ScreenToClient(&ClientRect);
    int xCenter = (ClientRect.left + ClientRect.right) / 2 - cBmpFrameWidth / 2;
    int yCenter = (ClientRect.top + ClientRect.bottom) / 2 - cBmpFrameHeight / 2;
    //	CRect RadioClientRect;
    //	m_rad0.GetWindowRect(&RadioClientRect);
    //	ScreenToClient(&RadioClientRect);
    //	ClientRect.top = (RadioClientRect.top + RadioClientRect.bottom) / 2 - cBmpFrameHeight / 2;
    //	ClientRect.top = ClientRect.Height()/2 - cBmpFrameHeight / 2;

    CBitmap* temp = m_dcDisplayMemory.SelectObject(CBitmap::FromHandle(m_hbmRobotBitmap));

    if (temp == NULL)
    {
        AfxMessageBox("FindHomebaseCorner SelectObject Error");
    }

    dc->BitBlt(xCenter, yCenter, rcRect.Width(), rcRect.Height(), &m_dcDisplayMemory, rcRect.left, rcRect.top, SRCCOPY);
    //	dc->BitBlt(0, 0, ClientRect.Width(), ClientRect.Height(), &m_dcDisplayMemory, 0, 0, SRCCOPY );
    m_dcDisplayMemory.SelectObject(temp);
}

void CDlgAddHomeBase::OnCancel()
{
    m_View->SetAddingHomeBase(false);
    CDialog::OnCancel();
}
