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
#if !defined(AFX_DLGADDROBOT_H__751445E1_A896_11D3_A5DF_00105A98F145__INCLUDED_)
#define AFX_DLGADDROBOT_H__751445E1_A896_11D3_A5DF_00105A98F145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddRobot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRobot dialog

class CDlgAddRobot : public CDialog
{
        // Construction
    public:
        int GetRobotVersion(void);
        short GetRobotColor(void);
        CDlgAddRobot(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CDlgAddRobot)
        enum { IDD = IDD_ADD_ROBOT };
        CComboBox	m_comRobotStyle;
        CStatic	m_BmpYellow;
        CStatic	m_BmpOrange;
        CStatic	m_BmpChrome;
        CStatic	m_BmpBlack;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDlgAddRobot)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CDlgAddRobot)
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnPaint();
        virtual BOOL OnInitDialog();
        afx_msg void OnSelchangeComRobotStyle();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    private:
        void HighlightBitmap(CDC* pDC, CStatic * Bitmap);
        enum {cNumRobotColors = 4};
        CStatic* m_BmpRobotColors[cNumRobotColors];
        CStatic* m_HighlightedBmp;
        int m_RobotStyle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDROBOT_H__751445E1_A896_11D3_A5DF_00105A98F145__INCLUDED_)
