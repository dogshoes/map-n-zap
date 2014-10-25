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
// DlgCreateCalibrationWall1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateCalibrationWall1 dialog
#ifndef DLGCRATECALIBRATIONWALL1adfj34098udfkgjh230rudflgj
#define DLGCRATECALIBRATIONWALL1adfj34098udfkgjh230rudflgj
class CRobotWorldView;
//class CDlgCreateCalibrationWall2;

class CDlgCreateCalibrationWall1 : public CDialog
{
        // Construction
    public:
        float GetCornerSize(void);
        void SetCornerSize(float CornerSize);
        CRect m_OriginalRect;
        void SetRobotBitmap(HBITMAP RobotBitmap);
        void SetRobotHeading(double Heading);
        CDlgCreateCalibrationWall1(CRobotWorldView* pView);
        ~CDlgCreateCalibrationWall1();
        // Dialog Data
        //{{AFX_DATA(CDlgCreateCalibrationWall1)
        enum { IDD = IDD_CREATECALIBRATIONWALL1 };
        CScrollBar	m_scrCornerSize;
        BOOL	m_chkCornerCalibration;
        int		m_radLeft;
        int		m_radFloorType;
        CString	m_edtName;
        CString	m_edtCornerSize;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDlgCreateCalibrationWall1)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CDlgCreateCalibrationWall1)
        virtual void OnOK();
        afx_msg void OnPaint();
        virtual BOOL OnInitDialog();
        afx_msg void OnChkCornerCalibration();
        afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnChangeEdtCornerSize();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
        void UpdateHeading(double Heading);
    private:
        UINT CornerSizeToScrollPos(float CornerSize);
        float ScrollPosToCornerSize(UINT nPos);
        float m_CornerSize;
        CDC m_dcDisplayMemory;
        double m_RobotHeading;
        HBITMAP m_hbmRobotBitmap;
        CRobotWorldView* m_View;
        //	CDlgCreateCalibrationWall2* m_DlgCreateCalibrationWall2;
};
#endif
