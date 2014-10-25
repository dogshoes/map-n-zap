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
// Wait.cpp : implementation file
//

#include "stdafx.h"
#include "FlowZap.h"
#include "Wait.h"
#include "util.h"

#define dEDIT_HOUR 0
#define dEDIT_MIN 1
/////////////////////////////////////////////////////////////////////////////
// CWait dialog

const char cstrWaitUntilInputIsGreater[] = "is >";
const char cstrWaitUntilInputIsLess[] = "is <";
const char cstrWaitUntilInputIsHigh[] = "is high";
const char cstrWaitUntilInputIsLow[] = "is low";
const char cstrWaitUntilInputChanges[] = "toggles";
const char cstrWaitUntilInput[] = "Wait until input";
const char cstrWaitUntilNudged[] = "Wait until nudged";
const char cstrWaitUntil[] = "Wait until";
const char cstrWaitFor[] = "Wait for";

IMPLEMENT_DYNAMIC(CWait, CInstruction)
CWait::CWait(CWnd* pParent /*=NULL*/, int index)
    : CInstruction(CWait::IDD, pParent, index)
{
    m_CurrentEditBox = dEDIT_HOUR;
    //{{AFX_DATA_INIT(CWait)
    m_TriggerLevel = 2.5f;
    //}}AFX_DATA_INIT
}


void CWait::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CWait)
    DDX_Control(pDX, IDC_TRIGGER_UNITS, m_staTriggerUnits);
    DDX_Control(pDX, IDC_INPUT_TRIGGER, m_edtTriggerLevel);
    DDX_Control(pDX, IDC_WAIT_INPUT, m_comWaitUntilInput);
    DDX_Control(pDX, IDC_WAIT_HOUR, m_WaitHour);
    DDX_Control(pDX, IDC_WAIT_MIN, m_WaitMin);
    DDX_Control(pDX, IDC_CHANGE_TIME, m_ChangeTime);
    DDX_Control(pDX, IDC_TIME_LENGTH, m_TimeLength);
    DDX_Control(pDX, IDC_IMAGE_DISABLED, m_DisabledImage);
    DDX_Control(pDX, IDC_BBORDER, m_BlackBorder);
    DDX_Control(pDX, IDC_EBORDER, m_EtchedBorder);
    DDX_Control(pDX, IDC_IMAGE, m_Image);
    DDX_Control(pDX, IDC_TIME_VAR, m_TimeVar);
    DDX_Control(pDX, IDC_WAIT_CONDITION, m_WaitCondition);
    DDX_Control(pDX, IDC_AM_PM, m_am_pm);
    DDX_Text(pDX, IDC_INPUT_TRIGGER, m_TriggerLevel);
    DDV_MinMaxFloat(pDX, m_TriggerLevel, 0.f, 5.f);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWait, CDialog)
    //{{AFX_MSG_MAP(CWait)
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    ON_WM_LBUTTONUP()
    ON_CBN_SELCHANGE(IDC_AM_PM, OnSelchangeAmPm)
    ON_CBN_SELCHANGE(IDC_TIME_VAR, OnSelchangeTimeVar)
    ON_CBN_SELCHANGE(IDC_TIME_LENGTH, OnSelchangeTimeLength)
    ON_CBN_SELCHANGE(IDC_WAIT_CONDITION, OnSelchangeWaitCondition)
    ON_WM_VSCROLL()
    ON_EN_SETFOCUS(IDC_WAIT_HOUR, OnSetfocusWaitHour)
    ON_EN_SETFOCUS(IDC_WAIT_MIN, OnSetfocusWaitMin)
    ON_EN_UPDATE(IDC_WAIT_MIN, OnUpdateWaitMin)
    ON_EN_UPDATE(IDC_WAIT_HOUR, OnUpdateWaitHour)
    ON_CBN_SELCHANGE(IDC_WAIT_INPUT, OnSelchangeWaitInput)
    ON_EN_CHANGE(IDC_INPUT_TRIGGER, OnChangeInputTrigger)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWait message handlers

void CWait::OnLButtonDown(UINT nFlags, CPoint point)
{
    CInstruction :: MouseClick(point);

    CDialog::OnLButtonDown(nFlags, point);
}

BOOL CWait::OnInitDialog()
{
    CDialog::OnInitDialog();


    m_ChangeTime.SetRange(1, 3);
    m_ChangeTime.SetPos(2);


    CRect windRect;
    GetWindowRect(&windRect);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), windRect.Height(), TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), windRect.Height(), TRUE);

    CInstruction::OnInit();
    m_WaitCondition.AddString(cstrWaitUntilNudged);
    m_WaitCondition.AddString(cstrWaitUntil);
    m_WaitCondition.AddString(cstrWaitFor);
    m_WaitCondition.AddString(cstrWaitUntilInput);
    m_WaitCondition.SelectString(0, cstrWaitUntilNudged);
    m_WaitCondition.SetDroppedWidth(200);

    m_comWaitUntilInput.AddString(cstrWaitUntilInputIsHigh);
    m_comWaitUntilInput.AddString(cstrWaitUntilInputIsLow);
    m_comWaitUntilInput.AddString(cstrWaitUntilInputChanges);
    m_comWaitUntilInput.AddString(cstrWaitUntilInputIsGreater);
    m_comWaitUntilInput.AddString(cstrWaitUntilInputIsLess);
    m_comWaitUntilInput.SelectString(0, cstrWaitUntilInputIsHigh);

    CRect r;
    m_WaitCondition.GetWindowRect(&r);
    m_OrigionalChoiceWidth = r.Width();

    m_WaitHour.SetWindowText("7");
    m_WaitMin.SetWindowText("00");


    m_TimeVar.AddString("Min.");
    m_TimeVar.AddString("Sec.");
    m_TimeVar.SelectString(0, "Sec");

    m_am_pm.AddString("AM");
    m_am_pm.AddString("PM");
    m_am_pm.SelectString(0, "AM");

    m_TimeLength.AddString("1/4");
    m_TimeLength.AddString("1/2");
    m_TimeLength.AddString("3/4");

    for (int i = 1; i <= 60; i++)
    {
        m_TimeLength.AddString(IntToString(i));
    }

    m_TimeLength.SelectString(-1, "1");



    m_TimeLength.ShowWindow(SW_HIDE);
    m_TimeVar.ShowWindow(SW_HIDE);

    AdjustDisplay(wcWaitNudge);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}



void CWait::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CInstruction::OnPaint(&dc);
    // Do not call CDialog::OnPaint() for painting messages
}


void CWait::OnOK()
{
    // Comment out following line to keep window from closing when enter is hit.
    //CDialog::OnOK();
}

void CWait::OnCancel()
{
    // Comment out following line to keep window from closing when esc is hit.
    //CDialog::OnCancel();
}

CString CWait::ToString()
{
    float InputTrigger;
    CString WaitMin, WaitHour;
    CString units;
    CString Selection;
    m_WaitCondition.GetWindowText(Selection);

    switch (GetWaitCondition())
    {
    case wcWaitUntil:
        //	int index = m_WaitCondition.GetCurSel();
        //	if (index == 1) {
        m_WaitHour.GetWindowText(WaitHour);
        m_WaitMin.GetWindowText(WaitMin);
        m_am_pm.GetWindowText(units);
        Selection = "Wait until " + WaitHour + ":" + WaitMin + " " + units;
        break;

    case wcWaitFor:
        //	else if (index == 2) {
        m_TimeLength.GetWindowText(WaitMin);
        m_TimeVar.GetWindowText(units);
        Selection = "Wait for " + WaitMin + " " + units;
        break;

    case wcWaitNudge:
        Selection = "Wait until nudged";
        break;

    case wcWaitUntilInputChanges:
        Selection.Format("%s %s", cstrWaitUntilInput, cstrWaitUntilInputChanges);
        break;

    case wcWaitUntilInputIsHigh:
        Selection.Format("%s %s", cstrWaitUntilInput, cstrWaitUntilInputIsHigh);
        break;

    case wcWaitUntilInputIsLow:
        Selection.Format("%s %s", cstrWaitUntilInput, cstrWaitUntilInputIsLow);
        break;

    case wcWaitUntilInputGreater:
        InputTrigger = m_TriggerLevel;
        Selection.Format("%s %s %3.1f Volts", cstrWaitUntilInput, cstrWaitUntilInputIsGreater, InputTrigger);
        break;

    case wcWaitUntilInputLess:
        InputTrigger = m_TriggerLevel;
        Selection.Format("%s %s %3.1f Volts", cstrWaitUntilInput, cstrWaitUntilInputIsLess, InputTrigger);
        break;

    default:
        ASSERT(FALSE);
        Selection = "Wait until nudged";
        break;
    }

    return Selection;
}

void CWait::MakeCondensed()
{
    if (IsSelected())
    {
        return;
    }

    m_Expanded = FALSE;
    m_am_pm.ShowWindow(SW_HIDE);
    m_Image.ShowWindow(SW_HIDE);
    m_DisabledImage.ShowWindow(SW_HIDE);
    m_WaitHour.ShowWindow(SW_HIDE);
    m_WaitMin.ShowWindow(SW_HIDE);
    m_WaitCondition.ShowWindow(SW_HIDE);
    m_TimeLength.ShowWindow(SW_HIDE);
    m_TimeVar.ShowWindow(SW_HIDE);
    m_ChangeTime.ShowWindow(SW_HIDE);
    m_edtTriggerLevel.ShowWindow(SW_HIDE);
    m_comWaitUntilInput.ShowWindow(SW_HIDE);
    CRect windRect;
    GetWindowRect(&windRect);
    MoveWindow(windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(), 16, TRUE);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), 16, TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), 16, TRUE);
}

void CWait::MakeExpanded()
{
    m_Expanded = TRUE;

    if (!m_Enabled)
    {
        m_DisabledImage.ShowWindow(SW_SHOW);
    }
    else
    {
        m_Image.ShowWindow(SW_SHOW);
    }

    m_WaitCondition.ShowWindow(SW_SHOW);
    AdjustDisplay(GetWaitCondition());

    CRect windRect;
    GetWindowRect(&windRect);
    MoveWindow(windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(), m_ExpandedHeight, TRUE);
    m_EtchedBorder.MoveWindow(0, 0, windRect.Width(), m_ExpandedHeight, TRUE);
    m_BlackBorder.MoveWindow(0, 0, windRect.Width(), m_ExpandedHeight, TRUE);
}

void CWait::AdjustDisplay(TWaitCondition m)
{
    CRect r;
    m_WaitCondition.GetWindowRect(&r);

    switch (m)
    {
    case wcWaitNudge:
        m_WaitHour.ShowWindow(SW_HIDE);
        m_WaitMin.ShowWindow(SW_HIDE);
        m_am_pm.ShowWindow(SW_HIDE);
        m_ChangeTime.ShowWindow(SW_HIDE);

        m_TimeLength.ShowWindow(SW_HIDE);
        m_TimeVar.ShowWindow(SW_HIDE);
        m_comWaitUntilInput.ShowWindow(SW_HIDE);
        m_edtTriggerLevel.ShowWindow(SW_HIDE);
        m_staTriggerUnits.ShowWindow(SW_HIDE);
        m_WaitCondition.MoveWindow(50, 11, (int)(m_OrigionalChoiceWidth * 2), r.Height(), TRUE);
        break;

    case wcWaitUntil:
        m_WaitHour.ShowWindow(SW_SHOW);
        m_WaitMin.ShowWindow(SW_SHOW);
        m_am_pm.ShowWindow(SW_SHOW);
        m_ChangeTime.ShowWindow(SW_SHOW);

        m_TimeLength.ShowWindow(SW_HIDE);
        m_TimeVar.ShowWindow(SW_HIDE);
        m_comWaitUntilInput.ShowWindow(SW_HIDE);
        m_edtTriggerLevel.ShowWindow(SW_HIDE);
        m_staTriggerUnits.ShowWindow(SW_HIDE);
        m_WaitCondition.MoveWindow(50, 11, m_OrigionalChoiceWidth, r.Height(), TRUE);
        break;

    case wcWaitFor:
        m_WaitHour.ShowWindow(SW_HIDE);
        m_WaitMin.ShowWindow(SW_HIDE);
        m_am_pm.ShowWindow(SW_HIDE);
        m_ChangeTime.ShowWindow(SW_HIDE);

        m_TimeLength.ShowWindow(SW_SHOW);
        m_TimeVar.ShowWindow(SW_SHOW);
        m_comWaitUntilInput.ShowWindow(SW_HIDE);
        m_edtTriggerLevel.ShowWindow(SW_HIDE);
        m_staTriggerUnits.ShowWindow(SW_HIDE);
        m_WaitCondition.MoveWindow(50, 11, m_OrigionalChoiceWidth, r.Height(), TRUE);
        break;

    case wcWaitUntilInputChanges:
    case wcWaitUntilInputIsHigh:
    case wcWaitUntilInputIsLow:
        m_WaitHour.ShowWindow(SW_HIDE);
        m_WaitMin.ShowWindow(SW_HIDE);
        m_am_pm.ShowWindow(SW_HIDE);
        m_ChangeTime.ShowWindow(SW_HIDE);

        m_TimeLength.ShowWindow(SW_HIDE);
        m_TimeVar.ShowWindow(SW_HIDE);
        m_comWaitUntilInput.ShowWindow(SW_SHOW);
        m_edtTriggerLevel.ShowWindow(SW_HIDE);
        m_staTriggerUnits.ShowWindow(SW_HIDE);
        m_WaitCondition.MoveWindow(50, 11, (int)(m_OrigionalChoiceWidth * 1.5), r.Height(), TRUE);
        break;

    case wcWaitUntilInputGreater:
    case wcWaitUntilInputLess:
        m_WaitHour.ShowWindow(SW_HIDE);
        m_WaitMin.ShowWindow(SW_HIDE);
        m_am_pm.ShowWindow(SW_HIDE);
        m_ChangeTime.ShowWindow(SW_HIDE);

        m_TimeLength.ShowWindow(SW_HIDE);
        m_TimeVar.ShowWindow(SW_HIDE);
        m_comWaitUntilInput.ShowWindow(SW_SHOW);
        m_edtTriggerLevel.ShowWindow(SW_SHOW);
        m_staTriggerUnits.ShowWindow(SW_SHOW);
        m_WaitCondition.MoveWindow(50, 11, (int)(m_OrigionalChoiceWidth * 1.5), r.Height(), TRUE);
        break;

    default:
        ASSERT(FALSE);
        break;
    }
}

void CWait::OnLButtonUp(UINT nFlags, CPoint point)
{
    CInstruction :: MouseButtonUp(nFlags, point);

    CDialog::OnLButtonUp(nFlags, point);
}

void CWait::ShowBorder(BOOL raised)
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

CString CWait::ToFile()
{
    return ToString() + " " + BoolToString(m_Enabled);
}

void CWait::ConfigureFromFile(TWaitCondition WaitCondition, CString first, CString second)
{
    CString hours;
    CString minutes;
    CStringList time;

    switch (WaitCondition)
    {
    case wcWaitNudge:
        m_WaitCondition.SelectString(-1, cstrWaitUntilNudged);
        break;

    case wcWaitUntil:
        m_WaitCondition.SetCurSel(m_WaitCondition.FindStringExact(-1, cstrWaitUntil));
        //		m_WaitCondition.SelectString (-1, "Wait until");
        Split(":", first, time);
        hours =   time.GetHead();
        minutes = time.GetTail();
        m_WaitHour.SetWindowText(hours);
        m_WaitMin.SetWindowText(minutes);
        m_am_pm.SelectString(-1, second);
        break;

    case wcWaitFor:
        m_WaitCondition.SelectString(-1, cstrWaitFor);
        m_TimeLength.SelectString(-1, first);
        m_TimeVar.SelectString(-1, second);
        break;

    case wcWaitUntilInputChanges:
        m_WaitCondition.SelectString(-1, cstrWaitUntilInput);
        m_comWaitUntilInput.SelectString(-1, cstrWaitUntilInputChanges);
        break;

    case wcWaitUntilInputIsHigh:
        m_WaitCondition.SelectString(-1, cstrWaitUntilInput);
        m_comWaitUntilInput.SelectString(-1, cstrWaitUntilInputIsHigh);
        break;

    case wcWaitUntilInputIsLow:
        m_WaitCondition.SelectString(-1, cstrWaitUntilInput);
        m_comWaitUntilInput.SelectString(-1, cstrWaitUntilInputIsLow);
        break;

    case wcWaitUntilInputGreater:
        m_WaitCondition.SelectString(-1, cstrWaitUntilInput);
        m_comWaitUntilInput.SelectString(-1, cstrWaitUntilInputIsGreater);
        m_edtTriggerLevel.SetWindowText(first);
        UpdateData(TRUE);
        break;

    case wcWaitUntilInputLess:
        m_WaitCondition.SelectString(-1, cstrWaitUntilInput);
        m_comWaitUntilInput.SelectString(-1, cstrWaitUntilInputIsLess);
        m_edtTriggerLevel.SetWindowText(first);
        UpdateData(TRUE);
        break;

    default:
        ASSERT(FALSE);
        break;
    }

    AdjustDisplay(WaitCondition);
}


void CWait::Enable(BOOL enabled)
{
    m_Enabled = enabled;
    m_am_pm.EnableWindow(enabled);
    m_WaitHour.EnableWindow(enabled);
    m_WaitMin.EnableWindow(enabled);
    m_WaitCondition.EnableWindow(enabled);
    m_TimeLength.EnableWindow(enabled);
    m_TimeVar.EnableWindow(enabled);
    m_ChangeTime.EnableWindow(enabled);

    m_comWaitUntilInput.EnableWindow(enabled);
    m_edtTriggerLevel.EnableWindow(enabled);
    m_staTriggerUnits.EnableWindow(enabled);

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

void CWait::OnSelchangeAmPm()
{
    CFlowZap *fc = (CFlowZap*)m_fc;
    fc->ChangeMadeInFile();
}

void CWait::OnSelchangeTimeLength()
{
    CFlowZap *fc = (CFlowZap*)m_fc;
    fc->ChangeMadeInFile();
}

void CWait::OnSelchangeTimeVar()
{
    CFlowZap *fc = (CFlowZap*)m_fc;
    fc->ChangeMadeInFile();
}

void CWait::OnSelchangeWaitCondition()
{
    AdjustDisplay(GetWaitCondition());

    CFlowZap *fc = (CFlowZap*)m_fc;
    fc->ChangeMadeInFile();
}

void CWait::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if ((int)nSBCode == 8)
    {
        return;
    }

    //record that a change has been made in the file
    CFlowZap *fc = (CFlowZap*)m_fc;
    fc->ChangeMadeInFile();

    //determine which edit box is active, and set the corresponding variables
    CString sEditBoxVal;
    CEdit * currentEditBox = NULL;
    int maxValue, minValue;
    int i = m_CurrentEditBox;
    int j = i;

    if (m_CurrentEditBox == dEDIT_HOUR)
    {
        m_WaitHour.GetWindowText(sEditBoxVal);
        maxValue = 12;
        minValue = 1;
        currentEditBox = &m_WaitHour;

    }

    else
    {
        m_WaitMin.GetWindowText(sEditBoxVal);
        maxValue = 59;
        minValue = 0;
        currentEditBox = &m_WaitMin;
    }


    int EditBoxNum = atoi(sEditBoxVal);


    if (m_ChangeTimePos > (int)nPos)
    {
        //clicked down arrow
        if (EditBoxNum <= minValue)
        {
            EditBoxNum = maxValue;
        }
        else
        {
            EditBoxNum --;
        }

    }
    else
    {
        //clicked up arrow
        if (EditBoxNum >= maxValue)
        {
            EditBoxNum = minValue;
        }
        else
        {
            EditBoxNum ++;
        }
    }

    sEditBoxVal = IntToString(EditBoxNum);

    //adjust for numbers between 0-9
    if (m_CurrentEditBox == dEDIT_MIN)
    {
        if (EditBoxNum < 10)
        {
            sEditBoxVal = "0" + sEditBoxVal;
        }
    }

    currentEditBox->SetWindowText(sEditBoxVal);
    currentEditBox->RedrawWindow(NULL, NULL);

    m_ChangeTime.SetPos(2);
    m_ChangeTimePos = 2;
    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CWait::OnSetfocusWaitHour()
{
    m_CurrentEditBox = dEDIT_HOUR;

}

void CWait::OnSetfocusWaitMin()
{
    m_CurrentEditBox = 1;

}

void CWait::OnUpdateWaitMin()
{

    CString val;
    m_WaitMin.GetWindowText(val);
    int min = atoi(val);

    if (min > 59)
    {
        m_WaitMin.SetWindowText("59");
    }

    if (min < 0)
    {
        m_WaitMin.SetWindowText("00");
    }

}

void CWait::OnUpdateWaitHour()
{

    CString val;
    m_WaitHour.GetWindowText(val);
    int hour = atoi(val);

    if (hour > 12)
    {
        m_WaitHour.SetWindowText("12");
    }

    if (hour < 1)
    {
        m_WaitHour.SetWindowText("1");
    }

}


TWaitCondition CWait::GetWaitCondition()
{
    UpdateData(TRUE);
    CString WaitCondition;
    m_WaitCondition.GetWindowText(WaitCondition);
    TWaitCondition ReturnValue = wcWaitNudge;

    if (WaitCondition == cstrWaitUntilNudged)
    {
        ReturnValue = wcWaitNudge;
    }
    else if (WaitCondition == cstrWaitUntil)
    {
        ReturnValue = wcWaitUntil;
    }
    else if (WaitCondition == cstrWaitFor)
    {
        ReturnValue = wcWaitFor;
    }
    else if (WaitCondition == cstrWaitUntilInput)
    {
        //		m_comWaitUntilInput.GetWindowText(WaitCondition);
        m_comWaitUntilInput.GetLBText(m_comWaitUntilInput.GetCurSel(), WaitCondition);

        if (WaitCondition == cstrWaitUntilInputIsHigh)
        {
            ReturnValue = wcWaitUntilInputIsHigh;
        }
        else if (WaitCondition == cstrWaitUntilInputIsLow)
        {
            ReturnValue = wcWaitUntilInputIsLow;
        }
        else if (WaitCondition == cstrWaitUntilInputChanges)
        {
            ReturnValue = wcWaitUntilInputChanges;
        }
        else if (WaitCondition == cstrWaitUntilInputIsGreater)
        {
            ReturnValue = wcWaitUntilInputGreater;
        }
        else if (WaitCondition == cstrWaitUntilInputIsLess)
        {
            ReturnValue = wcWaitUntilInputLess;
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

    return ReturnValue;
}

#ifdef ROBOT_WORLD_PROJECT
TTaskResult CWait::Execute(CRobot * Robot, CExecutableRWGraphicObjectList * ExecutableObjectList, bool & result)
{
    UpdateData(TRUE);
    CString strTimeLength;
    TTaskResult TaskResult;
    /*Do Waiting stuff*/
    CTime CurrentTime = CTime::GetCurrentTime();

    //	int index = m_WaitCondition.GetCurSel();
    //	switch (index) {
    switch (GetWaitCondition())
    {
    case wcWaitNudge:
        Robot->AddWaitForNudgeToInstructions(0);
        break;

    case wcWaitUntil:
    {
        CString strHours, strMins;
        m_WaitHour.GetWindowText(strHours);
        m_WaitMin.GetWindowText(strMins);

        /*Make sure strWaitTime is an ok string*/
        //if (strWaitTime.GetLength() > 5) return TR_Error;
        //CString ValidCharsString = strWaitTime.SpanIncluding("0123456789:");
        //if ( strWaitTime.GetLength() != ValidCharsString.GetLength()) return TR_Error;
        //unsigned char NumColons = 0;
        //for (int i = 0; i < strWaitTime.GetLength(); i++) {
        //	if (strWaitTime[i] == ':') NumColons++;
        //}
        //if (NumColons != 1) return TR_Error;
        /*String seems to be OK, get the hours and minutes of the time*/

        int dHours = atoi(strHours);
        int dMinutes = atoi(strMins);
        CString Minutes;

        /*Make sure hour and minutes are valid*/
        if ((dHours > 12) || (dMinutes >= 60) || (dHours <= 0) || (dMinutes < 0))
        {
            return TR_Error;
        }

        if (m_am_pm.GetCurSel() == 0)
        {
            if (dHours == 12)
            {
                dHours = 0;
            }
        }
        else
        {
            if (dHours != 12)
            {
                dHours += 12;
            }
        }

        CTime WaitTime(CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), dHours, dMinutes, 0);

        if (WaitTime < CTime::GetCurrentTime())
        {
            WaitTime += CTimeSpan(1, 0, 0, 0);
        }

        Robot->AddWaitUntilToInstructions(WaitTime);
    }
    break;

    case wcWaitFor:

        m_TimeLength.GetWindowText(strTimeLength);

        if (m_TimeVar.GetCurSel() == 0)
        {
            /*Minutes*/
            float minutes;

            if (strTimeLength.GetAt(1) == '/')
            {
                minutes = (strTimeLength.GetAt(0) - 48.0f) / (strTimeLength.GetAt(2) - 48);
            }
            else
            {
                minutes = (float)atoi(strTimeLength);
            }

            /*Minutes must be < 1440 (minutes in 1 day) or else the wait until instruction screws up*/
            ASSERT(minutes <= 1439.0f);

            if (minutes > 1439.0f)
            {
                return TR_Error;
            }

            //			CTimeSpan DT(0,0,minutes,0);
            //			CTime WaitTime = CurrentTime + DT;
            //			Robot->AddWaitUntilToInstructions(WaitTime);
            if (minutes >= 1.0f)
            {
                Robot->AddWaitLongToInstructions(0, (int)minutes, 0);
            }
            else
            {
                Robot->AddWaitLongToInstructions(0, 0, int(60 * minutes));
            }
        }
        else
        {
            /*Seconds*/
            float seconds;

            if (strTimeLength.GetLength() > 1)
            {
                if (strTimeLength.GetAt(1) == '/')
                {
                    seconds = (strTimeLength.GetAt(0) - 48.0f) / (strTimeLength.GetAt(2) - 48);
                }
                else
                {
                    seconds = (float)atoi(strTimeLength);
                }
            }
            else
            {
                seconds = (float)atoi(strTimeLength);
            }

            /*Seconds must be < number of seconds in a day or else the wait until instruction screws up*/
            //			if (seconds > 86340) return TR_Error;
            if (seconds >= 50.0f)
            {
                //				CTimeSpan DT(0,0,0,seconds);
                //				CTime WaitTime = CurrentTime + DT;
                //				Robot->AddWaitUntilToInstructions(WaitTime);
                Robot->AddWaitLongToInstructions(0, 0, (int)seconds);
            }
            else
            {
                Robot->AddWaitToInstructions(unsigned int(seconds * 1000));
            }
        }

        break;

    case wcWaitUntilInputChanges:
        Robot->AddWaitForInputChange(0);
        break;

    case wcWaitUntilInputIsHigh:
        Robot->AddWaitForInputHigh(0);
        break;

    case wcWaitUntilInputIsLow:
        Robot->AddWaitForInputLow(0);
        break;

    case wcWaitUntilInputGreater:
        Robot->AddWaitForInputGreater(m_TriggerLevel, 0);
        break;

    case wcWaitUntilInputLess:
        Robot->AddWaitForInputLess(m_TriggerLevel, 0);
        break;

    default:
        ASSERT(FALSE);
        break;
    }

    TaskResult = Robot->FollowInstructions(true, true, IR_InstructionComplete);
    return TaskResult;
}
#endif

void CWait::OnSelchangeWaitInput()
{
    AdjustDisplay(GetWaitCondition());
    CFlowZap *fc = (CFlowZap*)m_fc;
    fc->ChangeMadeInFile();
}

void CWait::OnChangeInputTrigger()
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    UpdateData(TRUE);
    CFlowZap *fc = (CFlowZap*)m_fc;
    fc->ChangeMadeInFile();
}
