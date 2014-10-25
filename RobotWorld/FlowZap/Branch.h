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
#if !defined(AFX_BRANCH_H__5AD7A6A4_715B_11D1_AC8C_0040F6B8AE22__INCLUDED_)
#define AFX_BRANCH_H__5AD7A6A4_715B_11D1_AC8C_0040F6B8AE22__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Branch.h : header file
//
#include "Instruction.h"
/////////////////////////////////////////////////////////////////////////////
// CBranch dialog

enum TBranchCondition {bcNone, bcNudged, bcNotNudged, bcInputHigh, bcInputLow, bcInputChanges, bcInputGreater, bcInputLess};
class CBranch : public CInstruction
{
        DECLARE_DYNAMIC(CBranch)
        // Construction
    public:
        void AdjustDisplay(TBranchCondition BranchCondition);
        TBranchCondition GetBranchCondition(void);
#ifdef ROBOT_WORLD_PROJECT
        virtual TTaskResult  Execute(CRobot * Robot, CExecutableRWGraphicObjectList * ExecutableObjectList, bool & result);
#endif
        CInstruction* GetLoopDestination();
        void Enable(BOOL enabled);
        int m_tmpPtr;
        void Configure(TBranchCondition BranchCondition, CString timeLength, CString timeFrame, int ptr, float TriggerLevel = 0);
        CString ToFile();
        void ShowBorder(BOOL b);
        void MakeExpanded();
        void MakeCondensed();
        CString ToString();
        CBranch(CWnd* pParent = NULL, int index = 1);  // standard constructor

        // Dialog Data
        //{{AFX_DATA(CBranch)
        enum { IDD = IDD_BRANCH };
        CEdit	m_edtTriggerLevel;
        CStatic	m_edtTriggerUnits;
        CComboBox	m_comWaitInput;
        CStatic	m_Text4;
        CStatic	m_DisabledImage;
        CStatic	m_EtchedBorder;
        CStatic	m_BlackBorder;
        CStatic	m_Text3;
        CStatic	m_Text2;
        CStatic	m_Text1;
        CStatic	m_Image;
        CEdit	m_Time;
        CComboBox	m_TimeLength;
        CComboBox	m_Condition;
        float	m_TriggerLevel;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CBranch)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CBranch)
        virtual void OnOK();
        virtual void OnCancel();
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        virtual BOOL OnInitDialog();
        afx_msg void OnPaint();
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnSelchangeCondition();
        afx_msg void OnChangeTime();
        afx_msg void OnSelchangeTimeLength();
        afx_msg void OnSelchangeWaitForInput();
        afx_msg void OnChangeWaitTriggerLevel();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRANCH_H__5AD7A6A4_715B_11D1_AC8C_0040F6B8AE22__INCLUDED_)
