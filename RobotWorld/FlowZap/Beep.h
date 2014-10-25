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
#if !defined(AFX_BEEP_H__E554FFA2_6FE2_11D1_AC8C_0040F6B8AE22__INCLUDED_)
#define AFX_BEEP_H__E554FFA2_6FE2_11D1_AC8C_0040F6B8AE22__INCLUDED_

#include "instruction.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Beep.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBeep dialog


class CBeep : public CInstruction
{
        DECLARE_DYNAMIC(CBeep)
        // Construction
    public:
#ifdef ROBOT_WORLD_PROJECT
        virtual TTaskResult Execute(CRobot* Robot, CExecutableRWGraphicObjectList* ExecutableObjectList, bool& result);
#endif
        unsigned int GetDuration();
        unsigned int GetFrequency();
        void Enable(BOOL enabled);
        void Configure(int index, CString time);
        void Configure(int index);
        int m_ToneIndex;
        CString ToFile();
        void ShowBorder(BOOL b);
        void MakeExpanded();
        void MakeCondensed();
        CString ToString();
        CBeep(CWnd* pParent = NULL, int index = 1);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CBeep)
        enum { IDD = IDD_BEEP };
        CStatic	m_DisabledImage;
        CStatic	m_BlackBorder;
        CStatic	m_EtchedBorder;
        CStatic	m_Text1;
        CStatic	m_Image;
        CStatic	m_Frequency;
        CSliderCtrl	m_ToneSlider;
        CStatic	m_BeepTone;
        CComboBox	m_BeepLength;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CBeep)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        afx_msg LRESULT OnBeepFrequencyChanged(WPARAM, LPARAM frequency);

        // Generated message map functions
        //{{AFX_MSG(CBeep)
        virtual void OnOK();
        virtual void OnCancel();
        afx_msg void OnPaint();
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        virtual BOOL OnInitDialog();
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnSelchangeBeepLength();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEEP_H__E554FFA2_6FE2_11D1_AC8C_0040F6B8AE22__INCLUDED_)
