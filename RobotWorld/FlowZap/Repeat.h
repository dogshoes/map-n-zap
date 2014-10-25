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
#if !defined(AFX_REPEAT_H__A1BC3EA5_709D_11D1_AC8C_0040F6B8AE22__INCLUDED_)
#define AFX_REPEAT_H__A1BC3EA5_709D_11D1_AC8C_0040F6B8AE22__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Repeat.h : header file

#include "Instruction.h"
class CLeftLoopPanel;
//
/////////////////////////////////////////////////////////////////////////////
// CRepeat dialog

class CRepeat : public CInstruction
{
        DECLARE_DYNAMIC(CRepeat)
        // Construction
    public:
        void DecrementNumRepeats(int num);
        CLeftLoopPanel * m_llp;
        void UpdateNumSteps();
        int GetNumRepeatsLeft();
        void Reset();
        int m_NumRepeatsLeft;
        CString m_OrigionalNumRepeats;
        CInstruction * GetLoopDestination();
        void Enable(BOOL enabled);
        void ConfigureFromFile(CString numBack, CString numRepeats);
        void SetNumRepeats(CString s);
        CString ToFile();
        void ShowBorder(BOOL b);
        void OnCancel();
        void MakeExpanded();
        void MakeCondensed();
        void SetNumSteps(CString numSteps);
        CString ToString();
        CString GetNumRepeats();
        CRepeat(CWnd* pParent = NULL, int index = 1, CLeftLoopPanel * llp = NULL); // standard constructor

        // Dialog Data
        //{{AFX_DATA(CRepeat)
        enum { IDD = IDD_REPEAT };
        CStatic	m_DisabledImage;
        CStatic	m_Border;
        CStatic	m_Text2;
        CStatic	m_Text1;
        CStatic	m_Image;
        CStatic	m_Steps;
        CStatic	m_NumSteps;
        CEdit	m_NumRepeats;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CRepeat)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CRepeat)
        virtual void OnOK();
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnPaint();
        virtual BOOL OnInitDialog();
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnChangeNumRepeats();
        afx_msg void OnKillfocusNumRepeats();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPEAT_H__A1BC3EA5_709D_11D1_AC8C_0040F6B8AE22__INCLUDED_)
