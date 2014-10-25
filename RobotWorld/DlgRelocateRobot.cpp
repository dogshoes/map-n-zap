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

// DlgRelocateRobot.cpp : implementation file
//

#include "stdafx.h"
#include "RobotWorld.h"
#include "DlgRelocateRobot.h"
#include "RobotWorldView.h"
#include "RobotWorld Resource.h"
//#include "Probability Grid.h"
#include "rothmath.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgRelocateRobot dialog

#define HeadingToScrollPos(Heading) (1000/360.0*Heading)
#define ScrollPosToHeading(ScrollPos) (ScrollPos*360.0/1000)

const int cMaxHeadingChars = 4;

CDlgRelocateRobot::CDlgRelocateRobot(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgRelocateRobot::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgRelocateRobot)
    m_edtRobotHeading = _T("0");
    //}}AFX_DATA_INIT
    m_RobotHeading = 0;
}


void CDlgRelocateRobot::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgRelocateRobot)
    DDX_Control(pDX, IDC_SCRROBOTHEADING, m_scrRobotHeading);
    DDX_Text(pDX, IDC_EDTROBOTHEADING, m_edtRobotHeading);
    DDV_MaxChars(pDX, m_edtRobotHeading, cMaxHeadingChars);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRelocateRobot, CDialog)
    //{{AFX_MSG_MAP(CDlgRelocateRobot)
    ON_WM_HSCROLL()
    ON_EN_CHANGE(IDC_EDTROBOTHEADING, OnChangeEdtRobotHeading)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRelocateRobot message handlers

BOOL CDlgRelocateRobot::OnInitDialog()
/*******************************************************************************
//Created: 07/15/97 S.R.
//Last Revision: 07/15/97 S.R.
//Parameters:
//	none
//Return Value:
//	none
********************************************************************************
//Description:
//	Initializes the dialog
//Method:
//
*******************************************************************************/
{
    CDialog::OnInitDialog();

    m_scrRobotHeading.SetScrollRange(0, 1000);
    m_scrRobotHeading.SetScrollPos((int)HeadingToScrollPos(m_RobotHeading));
    UpdateData(FALSE);

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

void CDlgRelocateRobot::SetRobotHeading(float Heading)
/*******************************************************************************
//Created: 07/15/97 S.R.
//Last Revision: 07/15/97 S.R.
//Parameters:
//	Heading:	robot heading in radians.
//Return Value:
//	none
********************************************************************************
//Description:
//	sets the robot heading, updates the heading scroll bar and the heading edit
//	box.
//Method:
//
*******************************************************************************/
{
    Heading = (float)RadianToDegree(Heading);
    m_scrRobotHeading.SetScrollPos((int)HeadingToScrollPos(Heading));
    m_RobotHeading = RadianToDegree(Heading);
    char buffer[cMaxHeadingChars];
    m_edtRobotHeading = itoa((int)m_RobotHeading, buffer, 10);
    UpdateHeading((float)m_RobotHeading);
    UpdateData(FALSE);
}

void CDlgRelocateRobot::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
/*******************************************************************************
//Created: 07/15/97 S.R.
//Last Revision: 07/15/97 S.R.
//Parameters:
//Return Value:
//	none
********************************************************************************
//Description:
//		Handles the scroll messages.
//Method:
//
*******************************************************************************/
{
    const int cLineInc = 10;
    const int cPageInc = 10;
    //	static UINT OldPos = 0;
    int Min, Max;

    switch (nSBCode)
    {
    case SB_LEFT:
        m_scrRobotHeading.GetScrollRange(&Min, &Max);
        nPos = Min;
        break;

    case SB_ENDSCROLL:
        nPos = m_scrRobotHeading.GetScrollPos();
        break;

    case SB_LINELEFT:
        nPos = m_scrRobotHeading.GetScrollPos();

        if (nPos <= cLineInc)
        {
            nPos = 0;
        }
        else
        {
            nPos -= cLineInc;
        }

        m_scrRobotHeading.GetScrollRange(&Min, &Max);

        if (nPos < (UINT)Min)
        {
            nPos = Min;
        }

        break;

    case SB_LINERIGHT:
        nPos = m_scrRobotHeading.GetScrollPos();
        nPos += cLineInc;
        m_scrRobotHeading.GetScrollRange(&Min, &Max);

        if (nPos > (UINT)Max)
        {
            nPos = Max;
        }

        break;

    case SB_PAGELEFT:
        nPos = m_scrRobotHeading.GetScrollPos();
        nPos -= cPageInc;
        m_scrRobotHeading.GetScrollRange(&Min, &Max);

        if (nPos < (UINT)Min)
        {
            nPos = Min;
        }

        break;

    case SB_PAGERIGHT:
        nPos = m_scrRobotHeading.GetScrollPos();
        nPos += cPageInc;
        m_scrRobotHeading.GetScrollRange(&Min, &Max);

        if (nPos > (UINT)Max)
        {
            nPos = Max;
        }

        break;

    case SB_RIGHT:
        m_scrRobotHeading.GetScrollRange(&Min, &Max);
        nPos = Max;
        break;

    case SB_THUMBPOSITION:
        break;

    case SB_THUMBTRACK:
        break;
    }

    m_scrRobotHeading.SetScrollPos(nPos, TRUE);
    char buffer[cMaxHeadingChars];
    m_RobotHeading = ScrollPosToHeading(nPos);
    m_edtRobotHeading = itoa((int)m_RobotHeading, buffer, 10);
    UpdateData(FALSE);
    UpdateHeading((float)ScrollPosToHeading(nPos));

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgRelocateRobot::OnChangeEdtRobotHeading()
/*******************************************************************************
//Created: 07/15/97 S.R.
//Last Revision: 07/15/97 S.R.
//Parameters:
//	none
//Return Value:
//	none
********************************************************************************
//Description:
//		Message handler for when robot heading edit box is changed.  It changes
//	the robot heading and robot heading scroll bar to match what's in the edit box.
//Method:
//
*******************************************************************************/
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function to send the EM_SETEVENTMASK message to the control
    // with the ENM_CHANGE flag ORed into the lParam mask.

    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    m_RobotHeading = atoi(m_edtRobotHeading);
    m_scrRobotHeading.SetScrollPos((int)HeadingToScrollPos(m_RobotHeading));
    UpdateHeading((float)m_RobotHeading);
}

void CDlgRelocateRobot::OnOK()
/*******************************************************************************
//Created: 07/15/97 S.R.
//Last Revision: 07/15/97 S.R.
//Parameters:
//	none
//Return Value:
//	none
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    CDialog::OnOK();

    if (m_RobotHeading == m_InitialDegreeHeading)
    {
        m_RobotHeading = m_InitialHeading;
    }
    else
    {
        m_RobotHeading = DegreeToRadian(m_RobotHeading);
    }

    m_RobotHeading = BringAngleInRange(m_RobotHeading);
}

void CDlgRelocateRobot::UpdateHeading(float Heading)
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

    if (Heading == 360)
    {
        Heading = 0;
    }

    Heading = (float)DegreeToRadian(Heading);
    Heading = (float)BringAngleInRange(Heading);

    CRect rcRect;
    int Frame = RobotHeadingToBitmapFrame(Heading, cTotalFrames);
    rcRect.left   = Frame % cBmpFramePerLine * cBmpFrameWidth;
    rcRect.top    = Frame / cBmpFramePerLine * cBmpFrameHeight;
    rcRect.right  = rcRect.left + cBmpFrameWidth;
    rcRect.bottom = rcRect.top + cBmpFrameHeight;
    CRect ClientRect;
    GetClientRect(&ClientRect);

    /*Blit Robot*/
    ClientRect.left = 30;
    ClientRect.top = ClientRect.Height() / 2 - cBmpFrameHeight / 2;

    CBitmap* temp = m_dcDisplayMemory.SelectObject(CBitmap::FromHandle(m_hbmRobotBitmap));

    if (temp == NULL)
    {
        AfxMessageBox("Relocate Robot SelectObject Error");
    }

    dc->BitBlt(ClientRect.left, ClientRect.top, rcRect.Width(), rcRect.Height(), &m_dcDisplayMemory, rcRect.left, rcRect.top, SRCCOPY);
    m_dcDisplayMemory.SelectObject(temp);
    /*Update the view's robot*/
    robPOINT RobotPosition;
    RobotPosition.x = m_RobotX;
    RobotPosition.y = m_RobotY;
    m_View->RedrawRobot(RobotPosition, Heading);
}

BOOL CDlgRelocateRobot::Create(UINT idDialogID, CRobotWorldView* View, double RobotX, double RobotY, double Heading)
/*******************************************************************************
//Created: 07/15/97 S.R.
//Last Revision: 07/15/97 S.R.
//Parameters:
//	idDialogID:	robot's dialog ID
//	View:			dialog's parent view.
//	Heading: heading in radians.
//Return Value:
//	none
********************************************************************************
//Description:
//	creates a relocate robot dialog.
//Method:
//
*******************************************************************************/
{
    if (!CDialog::Create(idDialogID, View))
    {
        AfxMessageBox("RelocateRobot Dialog Creationg Failed");
    }

    m_InitialHeading = Heading;
    m_View = View;
    m_RobotHeading = RadianToDegree(Heading);
    m_InitialDegreeHeading = (unsigned int)m_RobotHeading;
    m_RobotX = RobotX;
    m_RobotY = RobotY;
    char buffer[cMaxHeadingChars];
    m_edtRobotHeading = itoa((int)m_RobotHeading, buffer, 10);
    return TRUE;
}

int CDlgRelocateRobot::DoModal(CRobotWorldView * View, double RobotX, double RobotY, double Heading)
/*******************************************************************************
//Created: 07/15/97 S.R.
//Last Revision: 07/15/97 S.R.
//Parameters:
//	View:	pointer to dialog's parent view.
//	Heading:	robot's heading in radians.
//Return Value:
//	none
********************************************************************************
//Description:
//	creates a modal relocate robot dialog box.
//Method:
//
*******************************************************************************/
{
    m_View = View;
    m_RobotX = RobotX;
    m_RobotY = RobotY;
    m_InitialHeading = Heading;
    m_RobotHeading = RadianToDegree(Heading);
    m_InitialDegreeHeading = (unsigned int)m_RobotHeading;
    char buffer[cMaxHeadingChars];
    m_edtRobotHeading = itoa((int)m_RobotHeading, buffer, 10);
    return (CDialog::DoModal());
}

void CDlgRelocateRobot::OnPaint()
/*******************************************************************************
//Created: 07/15/97 S.R.
//Last Revision: 07/15/97 S.R.
//Parameters:
//	none
//Return Value:
//	none
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    CPaintDC dc(this); // device context for painting

    UpdateHeading((float)m_RobotHeading);

    // Do not call CDialog::OnPaint() for painting messages
}

void CDlgRelocateRobot::SetRobotBitmap(HBITMAP RobotBitmap)
{
    m_hbmRobotBitmap = RobotBitmap;
}
