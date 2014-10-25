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
#if !defined(AFX_SUB_H__286E71A2_8699_11D1_AC8C_0040F6B8AE22__INCLUDED_)
#define AFX_SUB_H__286E71A2_8699_11D1_AC8C_0040F6B8AE22__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Sub.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSub dialog
#include "Instruction.h"

class CSub : public CInstruction
{
        DECLARE_DYNAMIC(CSub)
        // Construction
    public:
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        void Configure(CString filename);
        void Enable(BOOL enabled);
        CString ToFile();
        CString m_NameOfFile;
        void MakeCondensed();
        void MakeExpanded();
        CString ToString();
        void ShowBorder(BOOL raised);
        CSub(CWnd* pParent = NULL, int index = 0); // standard constructor

        // Dialog Data
        //{{AFX_DATA(CSub)
        enum { IDD = IDD_SUB };
        CStatic	m_Text;
        CStatic	m_DisabledImage;
        CButton	m_ChooseProgram;
        CStatic	m_Image;
        CStatic	m_EtchedBorder;
        CStatic	m_BlackBorder;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CSub)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        CToolTipCtrl m_tooltip;

        // Generated message map functions
        //{{AFX_MSG(CSub)
        virtual void OnOK();
        virtual void OnCancel();
        virtual BOOL OnInitDialog();
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnPaint();
        afx_msg void OnButtonChooseProgram();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    private:
        CString GetFileNameFromPath(CString path);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUB_H__286E71A2_8699_11D1_AC8C_0040F6B8AE22__INCLUDED_)
