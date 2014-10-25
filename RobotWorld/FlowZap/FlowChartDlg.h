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
// FlowChartDlg.h : header file
//

#if !defined(AFX_FLOWCHARTDLG_H__C4DFEA5B_88FC_11D1_AC8C_0040F6B8AE22__INCLUDED_)
#define AFX_FLOWCHARTDLG_H__C4DFEA5B_88FC_11D1_AC8C_0040F6B8AE22__INCLUDED_

#include "FlowZap.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CFlowChartDlg dialog

class CFlowChartDlg : public CDialog
{
        // Construction
    public:
        CFlowZap m_FlowZap;
        CFlowChartDlg(CWnd* pParent = NULL);	// standard constructor

        // Dialog Data
        //{{AFX_DATA(CFlowChartDlg)
        enum { IDD = IDD_FLOWCHART_DIALOG };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA

        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CFlowChartDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
        virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        HICON m_hIcon;

        // Generated message map functions
        //{{AFX_MSG(CFlowChartDlg)
        virtual BOOL OnInitDialog();
        afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
        afx_msg void OnPaint();
        afx_msg HCURSOR OnQueryDragIcon();
        afx_msg void OnOpenflowzap();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOWCHARTDLG_H__C4DFEA5B_88FC_11D1_AC8C_0040F6B8AE22__INCLUDED_)
