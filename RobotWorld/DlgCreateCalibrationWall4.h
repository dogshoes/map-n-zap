#if !defined(AFX_DLGCREATECALIBRATIONWALL4_H__F71A48A2_657C_11D1_B231_0040F6B87672__INCLUDED_)
#define AFX_DLGCREATECALIBRATIONWALL4_H__F71A48A2_657C_11D1_B231_0040F6B87672__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgCreateCalibrationWall4.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateCalibrationWall4 dialog

#include "rothmath.h"
class CRobotWorldView;
class CDlgCreateCalibrationWall4 : public CDialog
{
        // Construction
    public:
        double m_H0;
        double m_H1;
        robPOINT m_P0;
        robPOINT m_P1;
        CDlgCreateCalibrationWall4(CRobotWorldView* pView);

        // Dialog Data
        //{{AFX_DATA(CDlgCreateCalibrationWall4)
        enum { IDD = IDD_CREATECALIBRATIONWALL4 };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDlgCreateCalibrationWall4)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CDlgCreateCalibrationWall4)
        virtual void OnOK();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    private:
        CRobotWorldView* m_View;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCREATECALIBRATIONWALL4_H__F71A48A2_657C_11D1_B231_0040F6B87672__INCLUDED_)
