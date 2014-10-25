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

// Branch.cpp : implementation file
//

#include "stdafx.h"
#include "FlowZap.h"
#include "Branch.h"
#include "Loop.h"
#include "util.h"

const char cstrIfNudged[] = "nudged";
const char cstrIfNotNudged[] = "not nudged";
const char cstrIfInput[] = "input";
const char cstrIfInputHigh[] = "is high";
const char cstrIfInputLow[] = "is low";
const char cstrIfInputChanges[] = "toggles";
const char cstrIfInputGreater[] = "is >";
const char cstrIfInputLess[] = "is <";

/////////////////////////////////////////////////////////////////////////////
// CBranch dialog

IMPLEMENT_DYNAMIC(CBranch, CInstruction)
CBranch::CBranch(CWnd* pParent /*=NULL*/, int index)
    : CInstruction(CBranch::IDD, pParent, index)
{
    //{{AFX_DATA_INIT(CBranch)
    m_TriggerLevel = 2.5f;
    //}}AFX_DATA_INIT
}


void CBranch::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CBranch)
    DDX_Control(pDX, IDC_WAIT_TRIGGER_LEVEL, m_edtTriggerLevel);
    DDX_Control(pDX, IDC_WAIT_TRIGGER_UNITS, m_edtTriggerUnits);
    DDX_Control(pDX, IDC_WAIT_FOR_INPUT, m_comWaitInput);
    DDX_Control(pDX, IDC_TEXT4, m_Text4);
    DDX_Control(pDX, IDC_DISABLED_IMAGE, m_DisabledImage);
    DDX_Control(pDX, IDC_EBORDER, m_EtchedBorder);
    DDX_Control(pDX, IDC_BBORDER, m_BlackBorder);
    DDX_Control(pDX, IDC_TEXT3, m_Text3);
    DDX_Control(pDX, IDC_TEXT2, m_Text2);
    DDX_Control(pDX, IDC_TEXT1, m_Text1);
    DDX_Control(pDX, IDC_IMAGE, m_Image);
    DDX_Control(pDX, IDC_TIME, m_Time);
    DDX_Control(pDX, IDC_TIME_LENGTH, m_TimeLength);
    DDX_Control(pDX, IDC_CONDITION, m_Condition);
    DDX_Text(pDX, IDC_WAIT_TRIGGER_LEVEL, m_TriggerLevel);
    DDV_MinMaxFloat(pDX, m_TriggerLevel, 0.f, 5.f);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBranch, CDialog)
    //{{AFX_MSG_MAP(CBranch)
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    ON_WM_LBUTTONUP()
    ON_CBN_SELCHANGE(IDC_CONDITION, OnSelchangeCondition)
    ON_EN_CHANGE(IDC_TIME, OnChangeTime)
    ON_CBN_SELCHANGE(IDC_TIME_LENGTH, OnSelchangeTimeLength)
    ON_CBN_SELCHANGE(IDC_WAIT_FOR_INPUT, OnSelchangeWaitForInput)
    ON_EN_CHANGE(IDC_WAIT_TRIGGER_LEVEL, OnChangeWaitTriggerLevel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBranch message handlers

void CBranch::OnLButtonDown(UINT nFlags, CPoint point)
{
    CInstruction :: MouseClick(point);

    CDialog::OnLButtonDown(nFlags, point);
}

void CBranch::OnOK()
{
    // Comment out following line to keep window from closing when enter is hit.
    //CDialog::OnOK();
}

void CBranch::OnCancel()
{
    // Comment out following line to keep window from closing when esc is hit.
    //CDialog::OnCancel();
}

BOOL CBranch::OnInitDialog()
{
    CDialog::OnInitDialog();
    CInstruction::OnInit();


    /*
    ** Move the Borders to the correct size
    */
    CRect windRect;
    GetWindowRect(&windRect);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), windRect.Height(), TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), windRect.Height(), TRUE);


    /*
    ** Initialize the condition drop-list
    */
    m_Condition.AddString(cstrIfNudged);
    m_Condition.AddString(cstrIfNotNudged);
    m_Condition.AddString(cstrIfInput);
    m_Condition.SelectString(-1, cstrIfNudged);
    m_comWaitInput.AddString(cstrIfInputHigh);
    m_comWaitInput.AddString(cstrIfInputLow);
    m_comWaitInput.AddString(cstrIfInputChanges);
    m_comWaitInput.AddString(cstrIfInputGreater);
    m_comWaitInput.AddString(cstrIfInputLess);
    m_comWaitInput.SelectString(-1, cstrIfInputHigh);

    /*
    ** Initialize the time-units drop-list
    */
    m_TimeLength.AddString("sec.");
    m_TimeLength.AddString("min.");
    m_TimeLength.AddString("hr.");
    m_TimeLength.SelectString(-1, "sec.");

    /*
    ** Initialize the time-length edit box
    */
    m_Time.SetWindowText("15");
    m_Time.LimitText(2);  // no numbers greater than 99

    AdjustDisplay(bcNudged);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CBranch::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    CInstruction::OnPaint(&dc);

    if (!m_Expanded)
    {
        return;
    }

    CRect r;
    GetWindowRect(&r);
    int y = r.Height() / 2;
    int w = r.Width();

    CWnd* pwnd = GetDlgItem(IDC_TEXT3);
    CRect TextBoxRect;
    pwnd->GetWindowRect(TextBoxRect);
    ScreenToClient(TextBoxRect);
    r.SetRect(TextBoxRect.right, y - 1, w, y + 1);

    //	r.SetRect(200, y-1, w, y+1);

    CPen *oldPen;
    CBrush *oldBrush;
    CBrush cb;
    CPen cp;

    if (m_Loop != NULL)
    {
        if (m_Loop->m_Selected)
        {
            cp.CreatePen(PS_SOLID, 1, COLORREF(0x0000ff));
            oldPen = dc.SelectObject(&cp);
            cb.CreateSolidBrush(COLORREF(0x0000ff));
            oldBrush = dc.SelectObject(&cb);
        }
        else if (!m_Enabled)
        {
            cp.CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
            oldPen = dc.SelectObject(&cp);
            cb.CreateSolidBrush(RGB(150, 150, 150));
            oldBrush = dc.SelectObject(&cb);

        }
        else
        {
            cp.CreatePen(PS_SOLID, 1, m_Loop->m_Color);
            oldPen = dc.SelectObject(&cp);
            cb.CreateSolidBrush(m_Loop->m_Color);
            oldBrush = dc.SelectObject(&cb);
        }
    }
    else
    {
        cp.CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
        oldPen = dc.SelectObject(&cp);
        cb.CreateSolidBrush(RGB(150, 150, 150));
        oldBrush = dc.SelectObject(&cb);
    }


    dc.FillRect(r, &cb);

    dc.SelectObject(oldPen);
    dc.SelectObject(oldBrush);
    cp.DeleteObject();
    cb.DeleteObject();

    // Do not call CDialog::OnPaint() for painting messages
}

CString CBranch::ToString()
{
    CString condition, length, units;
    CString InstructionString;

    m_Condition.GetWindowText(condition);
    m_Time.GetWindowText(length);
    m_TimeLength.GetWindowText(units);

    switch (GetBranchCondition())
    {
    case bcNudged:
    case bcNotNudged:
        InstructionString = "If " + condition + " within " + length + " " + units + " then do step " +
                            IntToString(m_Loop->m_End->GetIndex() + 1);
        break;

    case bcInputHigh:
        InstructionString.Format("If %s %s within %s %s then do step %d", cstrIfInput, cstrIfInputHigh, length, units,
                                 m_Loop->m_End->GetIndex() + 1);
        break;

    case bcInputLow:
        InstructionString.Format("If %s %s within %s %s then do step %d", cstrIfInput, cstrIfInputLow, length, units,
                                 m_Loop->m_End->GetIndex() + 1);
        break;

    case bcInputChanges:
        InstructionString.Format("If %s %s within %s %s then do step %d", cstrIfInput, cstrIfInputChanges, length, units,
                                 m_Loop->m_End->GetIndex() + 1);
        break;

    case bcInputGreater:
        InstructionString.Format("If %s %s %3.1f V within %s %s then do step %d", cstrIfInput, cstrIfInputGreater, m_TriggerLevel, length, units,
                                 m_Loop->m_End->GetIndex() + 1);
        break;

    case bcInputLess:
        InstructionString.Format("If %s %s %3.1f V within %s %s then do step %d", cstrIfInput, cstrIfInputLess, m_TriggerLevel, length, units,
                                 m_Loop->m_End->GetIndex() + 1);
        break;
    }

    return InstructionString;
}

CString CBranch::ToFile()
{
    return ToString() + " " + BoolToString(m_Enabled);
}


void CBranch::MakeCondensed()
{
    if (IsSelected())
    {
        return;    // don't do anything if this task is selected
    }

    m_Expanded = FALSE;


    /*
    ** Hide all of the controls in this instruction
    */
    m_Condition.ShowWindow(SW_HIDE);
    m_TimeLength.ShowWindow(SW_HIDE);
    m_Time.ShowWindow(SW_HIDE);
    m_Image.ShowWindow(SW_HIDE);
    m_DisabledImage.ShowWindow(SW_HIDE);
    m_Text1.ShowWindow(SW_HIDE);
    m_Text2.ShowWindow(SW_HIDE);
    m_Text3.ShowWindow(SW_HIDE);
    m_edtTriggerUnits.ShowWindow(SW_HIDE);
    m_edtTriggerLevel.ShowWindow(SW_HIDE);
    m_comWaitInput.ShowWindow(SW_HIDE);


    /*
    ** Resize the window and the borders to the shrunken height
    */
    CRect windRect;
    GetWindowRect(&windRect);
    MoveWindow(windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(), dSMALL_INSTRUCTION_SIZE, TRUE);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), dSMALL_INSTRUCTION_SIZE, TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), dSMALL_INSTRUCTION_SIZE, TRUE);
}

void CBranch::MakeExpanded()
{
    m_Expanded = TRUE;
    m_Condition.ShowWindow(SW_SHOW);
    m_TimeLength.ShowWindow(SW_SHOW);
    m_Time.ShowWindow(SW_SHOW);

    if (!m_Enabled)
    {
        m_DisabledImage.ShowWindow(SW_SHOW);
    }
    else
    {
        m_Image.ShowWindow(SW_SHOW);
    }

    m_Text1.ShowWindow(SW_SHOW);
    m_Text2.ShowWindow(SW_SHOW);
    m_Text3.ShowWindow(SW_SHOW);

    AdjustDisplay(GetBranchCondition());

    CRect windRect;
    GetWindowRect(&windRect);
    MoveWindow(windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(), m_ExpandedHeight, TRUE);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), m_ExpandedHeight, TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), m_ExpandedHeight, TRUE);
}

void CBranch::OnLButtonUp(UINT nFlags, CPoint point)
{
    CInstruction :: MouseButtonUp(nFlags, point);
    CDialog::OnLButtonUp(nFlags, point);
}

void CBranch::ShowBorder(BOOL raised)
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



void CBranch::Configure(TBranchCondition BranchCondition, CString timeLength, CString timeFrame, int ptr, float TriggerLevel)
{

    ptr --; // Decrement the pointer because it was incremented by 1 to adust from 0 ..n
    // to 1 .. n+1 for condensed view and writing to file.  Now, we need to go back to
    // a 0 based system.

    switch (BranchCondition)
    {
    case bcNudged:
        m_Condition.SelectString(-1, cstrIfNudged);
        break;

    case bcNotNudged:
        m_Condition.SelectString(-1, cstrIfNotNudged);
        break;

    case bcInputHigh:
        m_Condition.SelectString(-1, cstrIfInput);
        m_comWaitInput.SelectString(-1, cstrIfInputHigh);
        break;

    case bcInputLow:
        m_Condition.SelectString(-1, cstrIfInput);
        m_comWaitInput.SelectString(-1, cstrIfInputLow);
        break;

    case bcInputChanges:
        m_Condition.SelectString(-1, cstrIfInput);
        m_comWaitInput.SelectString(-1, cstrIfInputChanges);
        break;

    case bcInputGreater:
        m_Condition.SelectString(-1, cstrIfInput);
        m_comWaitInput.SelectString(-1, cstrIfInputGreater);
        break;

    case bcInputLess:
        m_Condition.SelectString(-1, cstrIfInput);
        m_comWaitInput.SelectString(-1, cstrIfInputLess);
        break;

    default:
        ASSERT(FALSE);
        break;
    }

    m_TimeLength.SelectString(-1, timeFrame);
    m_Time.SetWindowText(timeLength);
    m_tmpPtr = ptr;
    m_TriggerLevel = TriggerLevel;
    UpdateData(FALSE);
}

void CBranch::Enable(BOOL enabled)
{

    m_Enabled = enabled;
    m_Condition.EnableWindow(enabled);
    m_TimeLength.EnableWindow(enabled);
    m_Time.EnableWindow(enabled);
    m_Text1.EnableWindow(enabled);
    m_Text2.EnableWindow(enabled);
    m_Text3.EnableWindow(enabled);
    m_Text4.EnableWindow(enabled);
    m_edtTriggerUnits.EnableWindow(enabled);
    m_edtTriggerLevel.EnableWindow(enabled);
    m_comWaitInput.EnableWindow(enabled);

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




CInstruction* CBranch::GetLoopDestination()
{
    /*
    ** returns this object if for some reason the loop points to NULL
    */
    if (m_Loop == NULL)
    {
        return this;
    }

    return m_Loop->m_End;
}



void CBranch::OnSelchangeCondition()
{
    /*
    ** Mark that a change has been made in the FlowZap file
    */

    AdjustDisplay(GetBranchCondition());
    CFlowZap *fc = (CFlowZap*)m_fc;
    fc->ChangeMadeInFile();

}



void CBranch::OnChangeTime()
{
    /*
    ** Mark that a change has been made in the FlowZap file
    */
    if (m_Time.GetModify())
    {
        CFlowZap *fc = (CFlowZap*)m_fc;
        fc->ChangeMadeInFile();
    }

}



void CBranch::OnSelchangeTimeLength()
{
    /*
    ** Mark that a change has been made in the FlowZap file
    */
    CFlowZap *fc = (CFlowZap*)m_fc;
    fc->ChangeMadeInFile();

}



#ifdef ROBOT_WORLD_PROJECT
TTaskResult CBranch::Execute(CRobot * Robot, CExecutableRWGraphicObjectList * ExecutableObjectList, bool & result)
{
    TTaskResult TaskResult;

    CString condition;
    CString length;
    CString units;

    m_Condition.GetWindowText(condition);
    m_Time.GetWindowText(length);
    m_TimeLength.GetWindowText(units);

    unsigned long WaitFor;

    if (units == "min.")
    {
        WaitFor = atoi(length) * 1000 * 60;
    }
    else if (units == "sec.")
    {
        WaitFor = atoi(length) * 1000;
    }
    else if (units == "hr.")
    {
        WaitFor = atoi(length) * 1000 * 60 * 60;
    }
    else
    {
        ASSERT(FALSE);
        WaitFor = 0;
    }

    TBranchCondition BranchCondition = GetBranchCondition();

    switch (BranchCondition)
    {
    case bcNudged:
    case bcNotNudged:
        Robot->AddWaitForNudgeToInstructions(WaitFor);
        break;

    case bcInputHigh:
        Robot->AddWaitForInputHigh(WaitFor);
        break;

    case bcInputLow:
        Robot->AddWaitForInputLow(WaitFor);
        break;

    case bcInputChanges:
        Robot->AddWaitForInputChange(WaitFor);
        break;

    case bcInputGreater:
        Robot->AddWaitForInputGreater(m_TriggerLevel, WaitFor);
        break;

    case bcInputLess:
        Robot->AddWaitForInputLess(m_TriggerLevel, WaitFor);
        break;

    default:
        ASSERT(FALSE);
        break;
    }

    TInstructionResult InstructionResult;
    TaskResult = TR_Error;
    bool Error = false;
    InstructionResult = Robot->FollowInstructions(TRUE, TRUE);
    Error = false;

    switch (InstructionResult)
    {
    case IR_InstructionComplete:
        TaskResult = TR_TaskComplete;
        break;

    case IR_TimedOut:
        TaskResult = TR_Error;
        break;

    case IR_EmergencyStop:
        Error = true;
        TaskResult = TR_EmergencyStop;
        break;

    default:
        Error = true;
        TaskResult = TR_Error;
        break;
    }

    if (!Error)
    {
        /*If robot was nudged, determine result*/
        if (TaskResult == TR_TaskComplete)
        {
            switch (BranchCondition)
            {
            case bcNudged:
                result = true;
                break;

            case bcNotNudged:
                result = false;
                break;

            case bcInputHigh:
            case bcInputLow:
            case bcInputChanges:
            case bcInputGreater:
            case bcInputLess:
                result = true;
                break;

            default:
                ASSERT(FALSE);
                break;
            }
        }
        else
        {
            switch (BranchCondition)
            {
            case bcNudged:
                result = false;
                break;

            case bcNotNudged:
                result = true;
                break;

            case bcInputHigh:
            case bcInputLow:
            case bcInputChanges:
            case bcInputGreater:
            case bcInputLess:
                result = false;
                break;

            default:
                ASSERT(FALSE);
                break;
            }
        }

        TaskResult = TR_TaskComplete;
    }
    else
    {
        TaskResult = TR_Error;
    }

    return TaskResult;
}
#endif

TBranchCondition CBranch::GetBranchCondition(void)
{
    CString Condition;
    TBranchCondition BranchCondition = bcNone;
    m_Condition.GetWindowText(Condition);

    if (Condition == cstrIfNudged)
    {
        BranchCondition = bcNudged;
    }
    else if (Condition == cstrIfNotNudged)
    {
        BranchCondition = bcNotNudged;
    }
    else if (Condition == cstrIfInput)
    {
        m_comWaitInput.GetWindowText(Condition);

        if (Condition == cstrIfInputHigh)
        {
            BranchCondition = bcInputHigh;
        }
        else if (Condition == cstrIfInputLow)
        {
            BranchCondition = bcInputLow;
        }
        else if (Condition == cstrIfInputChanges)
        {
            BranchCondition = bcInputChanges;
        }
        else if (Condition == cstrIfInputGreater)
        {
            BranchCondition = bcInputGreater;
        }
        else if (Condition == cstrIfInputLess)
        {
            BranchCondition = bcInputLess;
        }
        else
        {
            ASSERT(FALSE);
        }
    }
    else
    {
        ASSERT(FALSE);
    }

    return BranchCondition;
}

void CBranch::AdjustDisplay(TBranchCondition BranchCondition)
{
    switch (BranchCondition)
    {
    case bcNudged:
    case bcNotNudged:
        m_edtTriggerUnits.ShowWindow(SW_HIDE);
        m_edtTriggerLevel.ShowWindow(SW_HIDE);
        m_comWaitInput.ShowWindow(SW_HIDE);
        break;

    case bcInputHigh:
    case bcInputLow:
    case bcInputChanges:
        m_edtTriggerUnits.ShowWindow(SW_HIDE);
        m_edtTriggerLevel.ShowWindow(SW_HIDE);
        m_comWaitInput.ShowWindow(SW_SHOW);
        break;

    case bcInputGreater:
    case bcInputLess:
        m_edtTriggerUnits.ShowWindow(SW_SHOW);
        m_edtTriggerLevel.ShowWindow(SW_SHOW);
        m_comWaitInput.ShowWindow(SW_SHOW);
        break;

    default:
        ASSERT(FALSE);
        break;
    }
}

void CBranch::OnSelchangeWaitForInput()
{
    AdjustDisplay(GetBranchCondition());
    CFlowZap *fc = (CFlowZap*)m_fc;
    fc->ChangeMadeInFile();
}

void CBranch::OnChangeWaitTriggerLevel()
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    CFlowZap *fc = (CFlowZap*)m_fc;
    fc->ChangeMadeInFile();
    UpdateData(TRUE);
}
