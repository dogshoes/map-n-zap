// DlgCreateCalibrationDoorway2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateCalibrationDoorway2 dialog
#ifndef DLGCREATECALIBRATIONDOORWAY2asdfjlk3j4r09345tldjg
#define DLGCREATECALIBRATIONDOORWAY2asdfjlk3j4r09345tldjg
#include "rothmath.h"

class CRobotWorldView;

class CDlgCreateCalibrationDoorway2 : public CDialog
{
        // Construction
    public:
        double m_H0;
        robPOINT m_P0;
        CDlgCreateCalibrationDoorway2(CRobotWorldView* pView);

        // Dialog Data
        //{{AFX_DATA(CDlgCreateCalibrationDoorway2)
        enum { IDD = IDD_CREATECALIBRATIONDOORWAY1 };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDlgCreateCalibrationDoorway2)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CDlgCreateCalibrationDoorway2)
        virtual void OnOK();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    private:
        CRobotWorldView* m_View;
};
#endif