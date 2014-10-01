// DlgCreateCalibrationWall2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateCalibrationWall2 dialog

#include "rothmath.h"

class CRobotWorldView;
class CDlgCreateCalibrationWall4;

class CDlgCreateCalibrationWall2 : public CDialog
{
// Construction
public:
	 ~CDlgCreateCalibrationWall2();
	CDlgCreateCalibrationWall4* m_DlgCreateCalibrationWall4;
	double m_H0;
	robPOINT m_P0;
	CDlgCreateCalibrationWall2(CRobotWorldView* pView);

// Dialog Data
	//{{AFX_DATA(CDlgCreateCalibrationWall2)
	enum { IDD = IDD_CREATECALIBRATIONWALL2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCreateCalibrationWall2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCreateCalibrationWall2)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CreateCornerWall(void);
	CRobotWorldView* m_View;
};
