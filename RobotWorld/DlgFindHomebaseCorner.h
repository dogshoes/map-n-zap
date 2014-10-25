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
#if !defined(AFX_DLGFINDHOMEBASECORNER_H__52A308A2_1640_11D2_B231_0040F6B87672__INCLUDED_)
#define AFX_DLGFINDHOMEBASECORNER_H__52A308A2_1640_11D2_B231_0040F6B87672__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgFindHomebaseCorner.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFindHomebaseCorner dialog
class CLaygo;

class CDlgFindHomebaseCorner : public CDialog
{
        // Construction
    public:
        CLaygo* m_Laygo;
        double GetHomebaseHeading(void);
        CWnd* m_wndPrevious;
        CWnd* m_wndNext;
        bool GetCornerToLeft(void);
        void SetRobotBitmap(HBITMAP Bitmap)
        {
            m_hbmRobotBitmap = Bitmap;
        };
        void SetHomebaseHeading(double Heading);
        CDlgFindHomebaseCorner(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CDlgFindHomebaseCorner)
        enum { IDD = IDD_HOMEBASE_CORNER };
        CButton	m_rad0;
        int		m_radDirection;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDlgFindHomebaseCorner)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CDlgFindHomebaseCorner)
        virtual BOOL OnInitDialog();
        afx_msg void OnPaint();
        afx_msg void OnButBack();
        afx_msg void OnButNext();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
        HBITMAP m_hbmRobotBitmap;
        CDC m_dcDisplayMemory;
        double m_RobotHeading;
    private:
        void UpdateHeading(double Heading);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFINDHOMEBASECORNER_H__52A308A2_1640_11D2_B231_0040F6B87672__INCLUDED_)
