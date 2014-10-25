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

// DlgEditCalibrationZone.cpp : implementation file
//

#include "stdafx.h"
#include "robotworld.h"
#include "DlgEditCalibrationZone.h"

const int cMaxScrollCornerSize = 1000;
const float cMaxCornerSize = 7.0f;
const float cMinCornerSize = 2.0f;
const int cMaxCornerSizeChars = 4;
/////////////////////////////////////////////////////////////////////////////
// CDlgEditCalibrationZone dialog


CDlgEditCalibrationZone::CDlgEditCalibrationZone(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgEditCalibrationZone::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgEditCalibrationZone)
    m_edtCalZoneName = _T("");
    m_radFloorType = 1;
    m_edtCornerSize = _T("");
    //}}AFX_DATA_INIT
    m_AdjustCheckPoint = false;
    m_CornerSize = 0.0f;
    m_HasCorner = false;
}


void CDlgEditCalibrationZone::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgEditCalibrationZone)
    DDX_Control(pDX, IDC_SCR_CORNER_SIZE, m_scrCornerSize);
    DDX_Text(pDX, IDC_EDT_CALZONE_NAME, m_edtCalZoneName);
    DDX_Radio(pDX, IDC_WOOD_FLOOR, m_radFloorType);
    DDX_Text(pDX, IDC_EDT_CORNER_SIZE, m_edtCornerSize);
    DDV_MaxChars(pDX, m_edtCornerSize, 3);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEditCalibrationZone, CDialog)
    //{{AFX_MSG_MAP(CDlgEditCalibrationZone)
    ON_BN_CLICKED(IDC_ADJUST_CHECKPOINT, OnAdjustCheckpoint)
    ON_EN_CHANGE(IDC_EDT_CORNER_SIZE, OnChangeEdtCornerSize)
    ON_WM_HSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEditCalibrationZone message handlers

void CDlgEditCalibrationZone::OnAdjustCheckpoint()
{
    m_AdjustCheckPoint = true;
    OnOK();
}

void CDlgEditCalibrationZone::SetHasCorner(bool Corner)
{
    m_HasCorner = Corner;

    if (GetSafeHwnd() != NULL)
    {
        m_scrCornerSize.EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_CORNER_SIZE)->EnableWindow(TRUE);
    }
}

void CDlgEditCalibrationZone::OnChangeEdtCornerSize()
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    m_CornerSize = (float)atof(m_edtCornerSize);

    if (m_CornerSize < cMinCornerSize)
    {
        m_CornerSize = cMinCornerSize;
        char buffer[cMaxCornerSizeChars];
        m_edtCornerSize = gcvt(m_CornerSize, 2, buffer);
        UpdateData(FALSE);
    }

    if (m_CornerSize > cMaxCornerSize)
    {
        m_CornerSize = cMaxCornerSize;
        char buffer[cMaxCornerSizeChars];
        m_edtCornerSize = gcvt(m_CornerSize, 2, buffer);
        UpdateData(FALSE);
    }

    m_scrCornerSize.SetScrollPos(CornerSizeToScrollPos(m_CornerSize), TRUE);

}

float CDlgEditCalibrationZone::GetCornerSize()
{
    return m_CornerSize;
}

void CDlgEditCalibrationZone::SetCornerSize(float CornerSize)
{
    if (CornerSize < cMinCornerSize)
    {
        CornerSize = cMinCornerSize;
    }

    if (CornerSize > cMaxCornerSize)
    {
        CornerSize = cMaxCornerSize;
    }

    m_CornerSize = CornerSize;

    if (GetSafeHwnd() != NULL)
    {
        m_scrCornerSize.SetScrollPos(CornerSizeToScrollPos(m_CornerSize), TRUE);
        char buffer[cMaxCornerSizeChars];
        m_edtCornerSize = gcvt(m_CornerSize, 2, buffer);
        UpdateData(FALSE);
    }
}

float CDlgEditCalibrationZone::ScrollPosToCornerSize(UINT nPos)
{
    return cMinCornerSize + (cMaxCornerSize - cMinCornerSize) / cMaxScrollCornerSize * nPos;
}

UINT CDlgEditCalibrationZone::CornerSizeToScrollPos(float CornerSize)
{
    return UINT((CornerSize - cMinCornerSize) * cMaxScrollCornerSize / (cMaxCornerSize - cMinCornerSize));
}

void CDlgEditCalibrationZone::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    const int cLineInc = 10;
    const int cPageInc = 100;
    //	static UINT OldPos = 0;
    int Min, Max;

    switch (nSBCode)
    {
    case SB_LEFT:
        m_scrCornerSize.GetScrollRange(&Min, &Max);
        nPos = Min;
        break;

    case SB_ENDSCROLL:
        nPos = m_scrCornerSize.GetScrollPos();
        break;

    case SB_LINELEFT:
        nPos = m_scrCornerSize.GetScrollPos();

        if (nPos <= cLineInc)
        {
            nPos = 0;
        }
        else
        {
            nPos -= cLineInc;
        }

        m_scrCornerSize.GetScrollRange(&Min, &Max);

        if (nPos < (UINT)Min)
        {
            nPos = (UINT)Min;
        }

        break;

    case SB_LINERIGHT:
        nPos = m_scrCornerSize.GetScrollPos();
        nPos += cLineInc;
        m_scrCornerSize.GetScrollRange(&Min, &Max);

        if (nPos > (UINT)Max)
        {
            nPos = (UINT)Max;
        }

        break;

    case SB_PAGELEFT:
        nPos = m_scrCornerSize.GetScrollPos();
        nPos -= cPageInc;
        m_scrCornerSize.GetScrollRange(&Min, &Max);

        if (nPos < (UINT)Min)
        {
            nPos = (UINT)Min;
        }

        break;

    case SB_PAGERIGHT:
        nPos = m_scrCornerSize.GetScrollPos();
        nPos += cPageInc;
        m_scrCornerSize.GetScrollRange(&Min, &Max);

        if (nPos > (UINT)Max)
        {
            nPos = (UINT)Max;
        }

        break;

    case SB_RIGHT:
        m_scrCornerSize.GetScrollRange(&Min, &Max);
        nPos = Max;
        break;

    case SB_THUMBPOSITION:
        break;

    case SB_THUMBTRACK:
        break;
    }

    m_scrCornerSize.SetScrollPos(nPos, TRUE);
    char buffer[cMaxCornerSizeChars];
    m_CornerSize = ScrollPosToCornerSize(nPos);
    m_edtCornerSize = gcvt(m_CornerSize, 2, buffer);
    UpdateData(FALSE);

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDlgEditCalibrationZone::OnInitDialog()
{
    CDialog::OnInitDialog();

    if (m_HasCorner)
    {
        m_scrCornerSize.EnableWindow(TRUE);
        m_scrCornerSize.SetScrollRange(0, cMaxScrollCornerSize);
        GetDlgItem(IDC_EDT_CORNER_SIZE)->EnableWindow(TRUE);
        SetCornerSize(m_CornerSize);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
