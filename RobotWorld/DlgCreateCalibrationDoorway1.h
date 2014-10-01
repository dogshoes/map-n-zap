// DlgCreateCalibrationDoorway1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateCalibrationDoorway1 dialog

#ifndef DLGCREATECALIBRATIONDOORWAY2asdjfkladsf87adsfkhwe98u234lkjsdf
#define DLGCREATECALIBRATIONDOORWAY2asdjfkladsf87adsfkhwe98u234lkjsdf
#include "DlgCreateCalibrationDoorway2.h"

class CRobotWorldView;

class CDlgCreateCalibrationDoorway1 : public CDialog
{
// Construction
public:
	~CDlgCreateCalibrationDoorway1();
	CDlgCreateCalibrationDoorway1(CRobotWorldView* View);
// Dialog Data
	//{{AFX_DATA(CDlgCreateCalibrationDoorway1)
	enum { IDD = IDD_CREATECALIBRATIONDOORWAY1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCreateCalibrationDoorway1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCreateCalibrationDoorway1)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CRobotWorldView* m_View;
	CDlgCreateCalibrationDoorway2* m_DlgCreateCalibrationDoorway2;
};
#endif