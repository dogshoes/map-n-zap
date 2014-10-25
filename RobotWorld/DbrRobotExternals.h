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
#if !defined(AFX_DBRROBOTEXTERNALS_H__7479BC01_91CB_11D3_A5DF_00105A98F145__INCLUDED_)
#define AFX_DBRROBOTEXTERNALS_H__7479BC01_91CB_11D3_A5DF_00105A98F145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DbrRobotExternals.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDbrRobotExternals dialog
class CMainFrame;

const int cNumberOfStates = 2;
const int cNumberHotRects = 2;
class CDbrRobotExternals : public CDialogBar
{
        // Construction
    public:
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        void SetExternalInput(float Volts);
        void SetExternalOutput(bool On);
        void SetFrameWnd(CMainFrame* Frame);
        void OnHotRect(int HotRectNumber);
        void SetState(int State);
        CDbrRobotExternals();   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CDbrRobotExternals)
        enum { IDD = IDW_ROBOT_EXTERNALS };
        CString	m_edtExternalInput;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDbrRobotExternals)
    public:
        virtual BOOL Create(CWnd *pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        CToolTipCtrl m_tooltip;

        // Generated message map functions
        //{{AFX_MSG(CDbrRobotExternals)
        afx_msg void OnPaint();
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    private:
        bool m_NeedToSetUpToolTips;
        CMainFrame* m_wndFrame;
        void SetActiveHotRect(int Active);
        void PaintBackground(CDC *pDC);
        CBitmap m_bmpState[cNumberOfStates];
        UINT m_StateBitmapResources[cNumberOfStates];
        CRect m_HotPointRects[cNumberOfStates];
        CDC m_dcDisplayMemory;
        int m_State;
        int m_ActiveHotRect;
        CPoint m_BitmapOrigin;
        int m_OldState;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBRROBOTEXTERNALS_H__7479BC01_91CB_11D3_A5DF_00105A98F145__INCLUDED_)
