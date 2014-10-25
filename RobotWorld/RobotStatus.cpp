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
// RobotStatus.cpp: implementation of the CRobotStatus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "robotworld.h"
#include "RobotStatus.h"
#include "RobotWorldView.h"
#include "RRDim.h"
#include "MainFrm.h"

const int cSpeedballHeight = 9;
const int cSpeedballWidth = 9;
const RECT cMaxSpeedRect = {236, 5, 305, 16};
const RECT cBatteryChargingStateRect = {0, 0, 97, 19};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDbrRobotStatus, CDialogBar)
    //{{AFX_MSG_MAP(CDbrRobotStatus)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_SIZE()
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDbrRobotStatus::CDbrRobotStatus()
{
    m_MaxSpeedRect.top = 5;
    m_MaxSpeedRect.left = 236;
    m_MaxSpeedRect.bottom = 16;
    m_MaxSpeedRect.right = 305;
    m_SpeedballLocation = 0;
    m_DraggingOffset = 0;
    m_DraggingMaxSpeed = false;

    /*Load the initial robot status bitmap*/
    if (!m_bmpBatteryChargeState.LoadBitmap(IDB_ROBOT_STATUS_NOT_CHARGING))
    {
        AfxMessageBox("RobotStatus  Load Background Bitmap failed");
    }

    /*Load the Speedball bitmap*/
    if (!m_bmpSpeedball.LoadBitmap(IDB_SPEEDBALL))
    {
        AfxMessageBox("RobotStatus Load Speedball Bitmap failed");
    }
}

CDbrRobotStatus::~CDbrRobotStatus()
{

}

void CDbrRobotStatus::OnSetMaxSpeed(short MaxSpeed)
{
    m_SpeedballLocation = MaxSpeedToSpeedballLocation(MaxSpeed);
    RedrawWindow(&m_MaxSpeedRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
    //	OnHScroll(SB_THUMBPOSITION, MaxSpeed, GetScrMaxSpeed());
}

void CDbrRobotStatus::OnBatteryChargeStateChange(short BatteryChargeState)
{
    switch (BatteryChargeState)
    {
    case 0:	/*Not charging*/
        m_bmpBatteryChargeState.DeleteObject();
        m_bmpBatteryChargeState.LoadBitmap(IDB_ROBOT_STATUS_NOT_CHARGING);
        //		GetBatteryChargeState()->SetBitmap(HBITMAP(m_bmpBatteryChargeState));
        break;

    case 1:	/*Charging*/
        m_bmpBatteryChargeState.DeleteObject();
        m_bmpBatteryChargeState.LoadBitmap(IDB_ROBOT_STATUS_CHARGING);
        //		GetBatteryChargeState()->SetBitmap(HBITMAP(m_bmpBatteryChargeState));
        break;

    case 2:	/*Floating*/
        m_bmpBatteryChargeState.DeleteObject();
        m_bmpBatteryChargeState.LoadBitmap(IDB_ROBOT_STATUS_CHARGED);
        //		GetBatteryChargeState()->SetBitmap(HBITMAP(m_bmpBatteryChargeState));
        break;
    }

    RedrawWindow();
}

void CDbrRobotStatus::OnSetBatteryLevel(float BatteryLevel)
{
    if (m_hWnd != NULL)
    {
        char String[10];
        _gcvt(BatteryLevel, 4, String);
        ((CEdit*)GetDlgItem(ID_EDT_BATTERY_LEVEL))->SetWindowText(String);
    }
}

void CDbrRobotStatus::OnPaint()
{
    CDialogBar::OnPaint();

    if (m_dcDisplayMemory.GetSafeHdc() == NULL)
    {
        /*Offset MaxSpeedRect (centering bitmap)*/
        BITMAP BitmapInfo;
        m_bmpBatteryChargeState.GetBitmap(&BitmapInfo);
        CRect ClientRect;
        GetClientRect(&ClientRect);
        int x, y;
        x = (ClientRect.Width() - BitmapInfo.bmWidth) / 2;
        y = (ClientRect.Height() - BitmapInfo.bmHeight) / 2;

        /*create a back buffer display context*/
        if (!m_dcDisplayMemory.CreateCompatibleDC(GetDC()))
        {
            AfxMessageBox("RobotStatus CreateCompatibleDC failed");
        }

        ASSERT(m_dcDisplayMemory.GetSafeHdc() != NULL);
    }

    CPaintDC dc(this); // device context for painting
    PaintBackground(GetDC());
    // Do not call CDialogBar::OnPaint() for painting messages
}

void CDbrRobotStatus::PaintBackground(CDC *pDC)
{
    if (m_dcDisplayMemory.GetSafeHdc() == NULL)
    {
        return;
    }

    CRect ClientRect;
    GetClientRect(&ClientRect);
    CBitmap* temp = m_dcDisplayMemory.SelectObject(&m_bmpBatteryChargeState);

    if (temp == NULL)
    {
        AfxMessageBox("RobotStatus SelectObject Error");
    }

    BITMAP BitmapInfo;
    m_bmpBatteryChargeState.GetBitmap(&BitmapInfo);
    /*Blt bitmap to center of dialog bar*/
    int x, y;
    x = (ClientRect.Width() - BitmapInfo.bmWidth) / 2;
    y = (ClientRect.Height() - BitmapInfo.bmHeight) / 2;
    pDC->BitBlt(x, y, BitmapInfo.bmWidth, BitmapInfo.bmHeight, &m_dcDisplayMemory, 0, 0, SRCCOPY);

    m_dcDisplayMemory.SelectObject(&m_bmpSpeedball);

    if (temp == NULL)
    {
        AfxMessageBox("RobotStatus Speedball SelectObject Error");
    }

    /*Do the max speed ball*/
    CRect OldRect = m_MaxSpeedRect;
    m_MaxSpeedRect.left = cMaxSpeedRect.left + x;
    m_MaxSpeedRect.right = cMaxSpeedRect.right + x;
    m_MaxSpeedRect.top = cMaxSpeedRect.top + y;
    m_MaxSpeedRect.bottom = cMaxSpeedRect.bottom + y;

    if (OldRect != m_MaxSpeedRect)
    {
        /*Set the tooltip rect for the max speed slider*/
        CToolInfo ToolInfo;
        m_tooltip.GetToolInfo(ToolInfo, this, IDC_SCR_MAX_SPEED);
        ToolInfo.rect = m_MaxSpeedRect;
        m_tooltip.SetToolInfo(&ToolInfo);
        /*Set the tooltip rect for the battery charge state*/
        CRect BatteryStateRect;
        BatteryStateRect.left = cBatteryChargingStateRect.left + x;
        BatteryStateRect.right = cBatteryChargingStateRect.right + x;
        BatteryStateRect.top = cBatteryChargingStateRect.top + y;
        BatteryStateRect.bottom = cBatteryChargingStateRect.bottom + y;
        m_tooltip.GetToolInfo(ToolInfo, this, IDC_BATTERY_CHARGING_STATE);
        ToolInfo.rect = BatteryStateRect;
        m_tooltip.SetToolInfo(&ToolInfo);
    }

    CPen BlackPen(PS_SOLID, 1, COLORREF(0));
    CPen SidePen(PS_SOLID, 3, COLORREF(0));
    CPen* OldPen = pDC->SelectObject(&SidePen);

    pDC->MoveTo(m_MaxSpeedRect.left, m_MaxSpeedRect.top);
    pDC->LineTo(m_MaxSpeedRect.left, m_MaxSpeedRect.bottom);

    pDC->MoveTo(m_MaxSpeedRect.right, m_MaxSpeedRect.top);
    pDC->LineTo(m_MaxSpeedRect.right, m_MaxSpeedRect.bottom);

    pDC->SelectObject(&BlackPen);
    pDC->MoveTo(m_MaxSpeedRect.left, (m_MaxSpeedRect.top + m_MaxSpeedRect.bottom) / 2);
    pDC->LineTo(m_MaxSpeedRect.right, (m_MaxSpeedRect.top + m_MaxSpeedRect.bottom) / 2);

    pDC->SelectObject(OldPen);

    x = m_MaxSpeedRect.left + m_SpeedballLocation;
    y = (m_MaxSpeedRect.top + m_MaxSpeedRect.bottom - cSpeedballHeight) / 2;
    pDC->BitBlt(x, y, x + cSpeedballWidth, y + cSpeedballHeight, &m_dcDisplayMemory, 0, 0, SRCCOPY);

    m_dcDisplayMemory.SelectObject(temp);
}

void CDbrRobotStatus::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_MaxSpeedRect.PtInRect(point))
    {
        SetCapture();
        m_DraggingMaxSpeed = true;

        /*Check if clicked right on speedball*/
        if ((point.x >= m_MaxSpeedRect.left + m_SpeedballLocation) && (point.x <= m_MaxSpeedRect.left + m_SpeedballLocation + cSpeedballWidth))
        {
            m_DraggingOffset = (m_MaxSpeedRect.left + m_SpeedballLocation) - point.x;
        }
        else
        {
            m_DraggingOffset = -cSpeedballWidth / 2;
            CPoint DragPoint = point;
            DragPoint.x += m_DraggingOffset;
            m_SpeedballLocation = ClientToSpeedballPoint(DragPoint);
            RedrawWindow(&m_MaxSpeedRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
        }
    }
    else
    {
        m_DraggingMaxSpeed = false;
        CDialogBar::OnLButtonDown(nFlags, point);
    }
}

void CDbrRobotStatus::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_DraggingMaxSpeed)
    {
        ReleaseCapture();
        RedrawWindow(&m_MaxSpeedRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
        m_DraggingMaxSpeed = false;
        CPoint DragPoint = point;
        DragPoint.x += m_DraggingOffset;
        m_SpeedballLocation = ClientToSpeedballPoint(DragPoint);
        ((CMainFrame*)GetParentFrame())->GetRobotWorldView()->SetMaxSpeed(SpeedballLocationToMaxSpeed(m_SpeedballLocation));
    }

    CDialogBar::OnLButtonUp(nFlags, point);
}

void CDbrRobotStatus::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_DraggingMaxSpeed)
    {
        RedrawWindow(&m_MaxSpeedRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
        CPoint DragPoint = point;
        DragPoint.x += m_DraggingOffset;
        m_SpeedballLocation = ClientToSpeedballPoint(DragPoint);
    }
    else
    {
        CDialogBar::OnMouseMove(nFlags, point);
    }
}


int CDbrRobotStatus::ClientToSpeedballPoint(CPoint point)
{
    if (point.x < m_MaxSpeedRect.left)
    {
        return 0;
    }
    else if (point.x > (m_MaxSpeedRect.right - cSpeedballWidth))
    {
        return (m_MaxSpeedRect.Width() - cSpeedballWidth);
    }
    else
    {
        return (point.x - m_MaxSpeedRect.left);
    }
}

short CDbrRobotStatus::SpeedballLocationToMaxSpeed(int SpeedballLocation)
{
    short Speed = (cMaxSpeed - cMinSpeed) * SpeedballLocation / (m_MaxSpeedRect.Width() - cSpeedballWidth) + cMinSpeed;

    if (Speed < 0)
    {
        Speed = 0;
    }
    else if (Speed > cMaxSpeed)
    {
        Speed = cMaxSpeed;
    }

    return Speed;
}

int CDbrRobotStatus::MaxSpeedToSpeedballLocation(short MaxSpeed)
{
    int Location = (MaxSpeed - cMinSpeed) * (m_MaxSpeedRect.Width() - cSpeedballWidth) / (cMaxSpeed - cMinSpeed);

    if (Location > (m_MaxSpeedRect.Width() - cSpeedballWidth))
    {
        Location = m_MaxSpeedRect.Width() - cSpeedballWidth;
    }
    else if (Location < 0)
    {
        Location = 0;
    }

    return Location;
}

void CDbrRobotStatus::SetRadioLevel(float RadioLevel)
{
    CWnd* wndRadioLevel = GetEdtRadioLevel();

    if (wndRadioLevel == NULL)
    {
        return;
    }

    if (RadioLevel <= 100.0f)
    {
        char buffer[10];
        _gcvt(RadioLevel, 5, buffer);
        wndRadioLevel->SetWindowText(buffer);
    }
    else
    {
        wndRadioLevel->SetWindowText("Idle");
    }
}

CEdit* CDbrRobotStatus::GetEdtRadioLevel(void)
{
    if (m_hWnd == NULL)
    {
        return NULL;
    }

    return (CEdit*)GetDlgItem(ID_EDT_RADIO_LEVEL);
}

/*CScrollBar* CDbrRobotStatus::CDbrRobotStatus::GetScrMaxSpeed(void)
{
	if (m_hWnd == NULL) return NULL;
	return (CScrollBar*)GetDlgItem(IDC_SCR_MAX_SPEED);
}
*/

void CDbrRobotStatus::OnSize(UINT nType, int cx, int cy)
{
    CDialogBar::OnSize(nType, cx, cy);

    if (m_bmpBatteryChargeState.m_hObject != NULL)
    {
        BITMAP BitmapInfo;
        m_bmpBatteryChargeState.GetBitmap(&BitmapInfo);
        int x, y;
        x = (cx - BitmapInfo.bmWidth) / 2;
        y = (cy - BitmapInfo.bmHeight) / 2;
        CWnd* control = GetDlgItem(ID_EDT_BATTERY_LEVEL);

        if (control != NULL)
        {
            control->MoveWindow(x + 111, y, 30, BitmapInfo.bmHeight);
            control = GetDlgItem(ID_EDT_RADIO_LEVEL);
            control->MoveWindow(x + 185, y, 30, BitmapInfo.bmHeight);
        }
    }
}

BOOL CDbrRobotStatus::PreTranslateMessage(MSG* pMsg)
{
    // CG: The following block was added by the ToolTips component.
    {
        // Let the ToolTip process this message.
        m_tooltip.RelayEvent(pMsg);
    }
    return CDialogBar::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

int CDbrRobotStatus::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialogBar::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    // CG: The following block was added by the ToolTips component.
    {
        // Create the ToolTip control.
        m_tooltip.Create(this);
        m_tooltip.Activate(TRUE);

        // TODO: Use one of the following forms to add controls:
        //	m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
        //	m_tooltip.AddTool(GetDlgItem(IDC_BKGR), "TESTING");
        CRect Rect;
        m_tooltip.AddTool(this, "Robot Max Speed", Rect, IDC_SCR_MAX_SPEED);
        m_tooltip.AddTool(this, "Battery Charging State", Rect, IDC_BATTERY_CHARGING_STATE);
    }

    return 0;
}
