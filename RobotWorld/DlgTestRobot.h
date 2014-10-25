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
#if !defined(AFX_DLGTESTROBOT_H__943D99C3_CA04_11D1_B231_0040F6B87672__INCLUDED_)
#define AFX_DLGTESTROBOT_H__943D99C3_CA04_11D1_B231_0040F6B87672__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgTestRobot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTestRobot dialog
class CRobot;

class CDlgTestRobot : public CDialog
{
        // Construction
    public:
        bool RotationTest(void);
        enum {nFreeWheelTestResults = 5, nFreeWheelTestMin = 0, nFreeWheelTestMax = 1, nFreeWheelTestDir = 2, nFreeWheelTestStd = 3, nFreeWheelTestZeroV = 4};
        CDlgTestRobot(CWnd* pParent = NULL);   // standard constructor
        bool FreeWheelCheck(bool Forward, double* Velocities, int nMeasurements, bool TestResults[nFreeWheelTestResults], FILE* out);
        bool FreeWheelTest(void);
        CRobot* m_Robot;
        // Dialog Data
        //{{AFX_DATA(CDlgTestRobot)
        enum { IDD = IDD_TEST_ROBOT };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDlgTestRobot)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        bool CheckBackAndForth(unsigned char Motor);

        // Generated message map functions
        //{{AFX_MSG(CDlgTestRobot)
        afx_msg void OnButFreeWheelTest();
        afx_msg void OnButM1Backward();
        afx_msg void OnButM1Forward();
        afx_msg void OnButStopMotors();
        afx_msg void OnButM0Forward();
        afx_msg void OnButM0Backward();
        afx_msg void OnButAgainstWallTest();
        afx_msg void OnButRotateTest();
        afx_msg void OnTestMicrophone();
        afx_msg void OnTestInput();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    private:
        bool AgainstWallTest(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTESTROBOT_H__943D99C3_CA04_11D1_B231_0040F6B87672__INCLUDED_)
