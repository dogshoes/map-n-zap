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
#if !defined(AFX_DLGADDHOMEBASE_H__FEB9E0E1_B852_11D3_A5DF_00105A98F145__INCLUDED_)
#define AFX_DLGADDHOMEBASE_H__FEB9E0E1_B852_11D3_A5DF_00105A98F145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddHomebase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddHomeBase dialog
class CRobotWorldView;

class CDlgAddHomeBase : public CDialog
{
        // Construction
    public:
        void SetRobotBitmap(HBITMAP RobotBitmap);
        void SetRobotHeading(double Heading);
        void SetRobotWorldView(CRobotWorldView* View)
        {
            m_View = View;
        };
        CDlgAddHomeBase(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CDlgAddHomeBase)
        enum { IDD = IDD_ADD_HOMEBASE };
        int		m_radFloorType;
        int		m_radLeft;
        CString	m_edtAddHomeBaseInstructions;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDlgAddHomeBase)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CDlgAddHomeBase)
        virtual void OnOK();
        virtual BOOL OnInitDialog();
        afx_msg void OnPaint();
        virtual void OnCancel();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    private:
        void DrawRobot(double Heading);
        CDC m_dcDisplayMemory;
        double m_RobotHeading;
        HBITMAP m_hbmRobotBitmap;
        CRobotWorldView* m_View;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDHOMEBASE_H__FEB9E0E1_B852_11D3_A5DF_00105A98F145__INCLUDED_)
