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
// PathFollow.cpp : implementation file
//

#include "stdafx.h"
#include "flowzap.h"
#include "PathFollow.h"
#include "util.h"

/////////////////////////////////////////////////////////////////////////////
// CPathFollow dialog

IMPLEMENT_DYNAMIC(CPathFollow, CInstruction)
CPathFollow::CPathFollow(CWnd* pParent /*=NULL*/, int index)
    : CInstruction(CPathFollow::IDD, pParent, index)
{
    //{{AFX_DATA_INIT(CPathFollow)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CPathFollow::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPathFollow)
    DDX_Control(pDX, IDC_TEXT2, m_Text2);
    DDX_Control(pDX, IDC_TEXT1, m_Text1);
    DDX_Control(pDX, IDC_SPEED_SLIDER, m_SpeedSlider);
    DDX_Control(pDX, IDC_SPEED, m_SpeedText);
    DDX_Control(pDX, IDC_PATH_LIST, m_PathList);
    DDX_Control(pDX, IDC_IMAGE_DISABLED, m_DisabledImage);
    DDX_Control(pDX, IDC_IMAGE, m_Image);
    DDX_Control(pDX, IDC_BORDER_ETCHED, m_EtchedBorder);
    DDX_Control(pDX, IDC_BORDER_BLACK, m_BlackBorder);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPathFollow, CDialog)
    //{{AFX_MSG_MAP(CPathFollow)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_PAINT()
    ON_WM_HSCROLL()
    ON_CBN_SELCHANGE(IDC_PATH_LIST, OnSelchangePathList)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathFollow message handlers

BOOL CPathFollow::OnInitDialog()
{
    CDialog::OnInitDialog();

    CRect windRect;
    GetWindowRect(&windRect);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), windRect.Height(), TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), windRect.Height(), TRUE);

    CInstruction::OnInit();

    m_PathList.AddString("[CHOOSE PATH]");
    m_PathList.SelectString(-1, "[CHOOSE PATH]");

    m_SpeedSlider.SetRange(20, 100);
    m_SpeedSlider.SetPos(80);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CPathFollow::OnLButtonDown(UINT nFlags, CPoint point)
{
    CInstruction :: MouseClick(point);
    CDialog::OnLButtonDown(nFlags, point);
}

void CPathFollow::OnLButtonUp(UINT nFlags, CPoint point)
{
    CInstruction :: MouseButtonUp(nFlags, point);
    CDialog::OnLButtonUp(nFlags, point);
}

void CPathFollow::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    CInstruction::OnPaint(&dc);
}

void CPathFollow::OnOK()
{
    // Comment out following line to keep window from closing when enter is hit.
    //CDialog::OnOK();
}

void CPathFollow::OnCancel()
{
    // Comment out following line to keep window from closing when enter is hit.
    //CDialog::OnOK();
}


void CPathFollow::MakeCondensed()
{
    // don't condense this instruction if it is selected
    if (IsSelected())
    {
        return;
    }


    m_Expanded = FALSE;

    // hide all the components
    m_PathList.ShowWindow(SW_HIDE);
    m_DisabledImage.ShowWindow(SW_HIDE);
    m_SpeedSlider.ShowWindow(SW_HIDE);
    m_Image.ShowWindow(SW_HIDE);
    m_Text1.ShowWindow(SW_HIDE);
    m_Text2.ShowWindow(SW_HIDE);
    m_SpeedText.ShowWindow(SW_HIDE);

    CRect windRect;
    GetWindowRect(&windRect);
    MoveWindow(windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(), 16, TRUE);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), 16, TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), 16, TRUE);
}


void CPathFollow::MakeExpanded()
{
    m_Expanded = TRUE;
    m_PathList.ShowWindow(SW_SHOW);
    m_Text1.ShowWindow(SW_SHOW);
    m_Text2.ShowWindow(SW_SHOW);
    m_SpeedText.ShowWindow(SW_SHOW);
    m_SpeedSlider.ShowWindow(SW_SHOW);

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

void CPathFollow::Enable(BOOL enabled)
{
    m_Enabled = enabled;
    m_PathList.EnableWindow(enabled);
    m_Text1.EnableWindow(enabled);
    m_Text2.EnableWindow(enabled);
    m_SpeedSlider.EnableWindow(enabled);
    m_SpeedText.EnableWindow(enabled);

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

CString CPathFollow::ToString()
{
    CString strPathName, strSpeed;
    m_PathList.GetWindowText(strPathName);
    m_SpeedText.GetWindowText(strSpeed);
    return "Follow Path: " + strPathName + " at " + strSpeed;
}



CString CPathFollow::ToFile()
{
    CString path = GetName();
    int speed = m_SpeedSlider.GetPos();
    //	m_PathList.GetWindowText(path);
    return "Follow "  + path + " " + IntToString(speed)  + " " + BoolToString(m_Enabled);
}



void CPathFollow::ShowBorder(BOOL raised)
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



void CPathFollow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    //	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK) {
    //		int pos = (int)nPos;
    //		ConfigureSpeed (pos);
    //	}


    if (nSBCode == SB_ENDSCROLL)
    {
        // user has lifted up on the mouse after dragging.
        CFlowZap *fc = (CFlowZap*)m_fc;
        fc->ChangeMadeInFile();
    }

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
    ConfigureSpeed(m_SpeedSlider.GetPos());
}



void CPathFollow::ConfigureSpeed(int pos)
{
    m_SpeedSlider.SetPos(pos);
    m_SpeedText.SetWindowText(IntToString(pos) + "% speed");
}



void CPathFollow::OnSelchangePathList()
{
    CFlowZap *fc = (CFlowZap*)m_fc;
    fc->ChangeMadeInFile();

}

void CPathFollow::Configure(CString caption, int pos)
{
    ConfigureSpeed(pos);
    int result = m_PathList.SelectString(-1, caption);

    if (result == CB_ERR)
    {

        CFlowZap *fc = (CFlowZap*)m_fc;
        //fc->AddLabel (Caption);
        //InsertLabel(Caption);
        //m_LabelName.SelectString(-1, Caption);
    }
}

void CPathFollow::InsertLinePaths(CStringList * paths)
{
    CString oldValue;
    m_PathList.GetWindowText(oldValue);
    m_PathList.ResetContent();
    CString zone;
    m_PathList.AddString("[CHOOSE PATH]");

    for (int i = 0; i < paths->GetCount(); i ++)
    {
        zone = paths->GetAt(paths->FindIndex(i));
        m_PathList.AddString(zone);
    }

    m_PathList.SelectString(-1, oldValue);
}

void CPathFollow::RemoveLabel(CString OldCaption)
{
    CString currentValue;
    m_PathList.GetWindowText(currentValue);

    int result = m_PathList.FindStringExact(-1, OldCaption);

    if (result != CB_ERR)
    {
        m_PathList.DeleteString(result);
    }

    if (currentValue == OldCaption)
    {
        m_PathList.SelectString(-1, "[CHOOSE PATH]");
    }
}

void CPathFollow::InsertLabel(CString name)
{
    m_PathList.AddString(name);
}

void CPathFollow::ChangeLabel(CString OldCaption, CString NewCaption)
{

    CString oldValue = " ";
    m_PathList.GetWindowText(oldValue);
    m_PathList.AddString(NewCaption);
    m_PathList.DeleteString(m_PathList.FindStringExact(-1, OldCaption));


    /*
    If we are changing the name of the selected item, select the new name
    before we leave
    */
    if (oldValue == OldCaption)
    {
        m_PathList.SelectString(-1, NewCaption);
    }

    else
    {
        m_PathList.SelectString(-1, oldValue);
    }
}

#ifdef ROBOT_WORLD_PROJECT

#include "ExecutableRWGraphicObject.h"
//#include "LinePath.h"

TTaskResult CPathFollow::Execute(CRobot * Robot, CExecutableRWGraphicObjectList* ExecutableObjectList, bool & result)
{
    TTaskResult TaskResult;
    result = true;
    /*Do Goto label stuff*/
    CExecutableRWGraphicObject* ExecutableObject;
    ExecutableObject = ExecutableObjectList->FindObject(GetName(), goLinePath);

    if (ExecutableObject != NULL)
    {
        //		CString txtSpeed;
        //		m_SpeedText.GetWindowText(txtSpeed);
        //		float Speed = atoi(txtSpeed);
        //		Speed = Speed * Robot->GetMaxSpeed() / 100;
        short Speed = m_SpeedSlider.GetPos();
        Speed = Speed * cMaxSpeed / 100;
        TaskResult = ExecutableObject->Execute(Robot, Speed);

        if (TaskResult != TR_TaskComplete)
        {
            /*Handle Error Stuff*/
        }
    }
    else
    {
        AfxMessageBox("That Go Path doesn't exist");
        TaskResult = TR_Error;
    }

    return TaskResult;
}
#endif
