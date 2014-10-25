#if !defined(AFX_DLGPOSITIONERROR_H__90D49181_F9F8_11D0_B231_FAE66F55053E__INCLUDED_)
#define AFX_DLGPOSITIONERROR_H__90D49181_F9F8_11D0_B231_FAE66F55053E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgPositionError.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPositionError dialog

#include "Robot.h"
class CDlgPositionError : public CDialog
{
        // Construction
    public:
        void Update(robPOINT dPosition, double Heading, double dHeading);
        void Create(UINT idRobotBitmap, UINT idDialogID, CView* pView, CRobot* Robot);
        CDlgPositionError(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CDlgPositionError)
        enum { IDD = IDD_POSITIONERROR };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDlgPositionError)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CDlgPositionError)
        afx_msg void OnPaint();
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnDestroy();
        afx_msg void OnSize(UINT nType, int cx, int cy);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    private:
        CView* m_pView;
        CRobot* m_pRobot;
        void SketchError(CDC* dc);
        double m_DHeading;
        double m_Heading;
        robPOINT m_DPosition;
        CDC m_dcDisplayMemory;
        CBitmap* m_pRobotBitmap;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPOSITIONERROR_H__90D49181_F9F8_11D0_B231_FAE66F55053E__INCLUDED_)
