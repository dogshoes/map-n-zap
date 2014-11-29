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
#if !defined(AFX_TOOL_H__1A233682_B1B4_11D1_AC8C_0040F6B8AE22__INCLUDED_)
#define AFX_TOOL_H__1A233682_B1B4_11D1_AC8C_0040F6B8AE22__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Tool.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTool dialog

class CTool : public CDialog
{
        // Construction
    public:
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        CWnd * m_Parent;
        CTool(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CTool)
        enum { IDD = IDD_TOOLS };
        CButton	m_ExitFZ;
        CButton	m_OpenFile;
        CButton	m_NewFile;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CTool)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        CToolTipCtrl m_tooltip;

        // Generated message map functions
        //{{AFX_MSG(CTool)
        virtual void OnOK();
        virtual void OnCancel();
        virtual BOOL OnInitDialog();
        afx_msg void OnNewfile();
        afx_msg void OnOpenfile();
        afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
        afx_msg void OnExitflowzap();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOL_H__1A233682_B1B4_11D1_AC8C_0040F6B8AE22__INCLUDED_)