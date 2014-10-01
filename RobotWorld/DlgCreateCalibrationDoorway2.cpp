// DlgCreateCalibrationDoorway2.cpp : implementation file
//

#include <math.h>
#include "stdafx.h"
#include "RobotWorld.h"
//#include "DlgCreateCalibrationDoorway2.h"
#include "RobotWorldView.h"
#include "Calibration Zone.h"
#include "robot.h"
#include "Probability Grid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*******************************************************************************
//Created: 01/10/97 S.R.
//Last Revision: 01/10/97 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateCalibrationDoorway2 dialog


CDlgCreateCalibrationDoorway2::CDlgCreateCalibrationDoorway2(CRobotWorldView* pView)
{
	Create(IDD_CREATECALIBRATIONDOORWAY2, pView);
	m_View = pView;
	//{{AFX_DATA_INIT(CDlgCreateCalibrationDoorway2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCreateCalibrationDoorway2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCreateCalibrationDoorway2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCreateCalibrationDoorway2, CDialog)
	//{{AFX_MSG_MAP(CDlgCreateCalibrationDoorway2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateCalibrationDoorway2 message handlers

void CDlgCreateCalibrationDoorway2::OnOK() 
/*******************************************************************************
//Created: 01/10/97 S.R.
//Last Revision: 11/13/97 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
	CDialog::OnOK();
	robPOINT P1;
	double H1;
/*Show the EStop dialog window*/
	BOOL EStopWasVisible = m_View->m_Robot->GetEStopVisible();
	m_View->m_Robot->SetEStopVisible(true);
/*Find the corner*/
	if ( m_View->m_Robot->FindCorner(&P1, &H1) != IR_InstructionComplete ) {
	/*Hide the EStop*/
		if (!EStopWasVisible) {
			m_View->m_Robot->SetEStopVisible(false);
		}
		return;
	}
/*Hide the EStop*/
	if (!EStopWasVisible) {
		m_View->m_Robot->SetEStopVisible(false);
	}
//	CCalibrationZone* CalZone = m_CalibrationZoneHandler->ConstructCalibrationZone(m_P0, m_H0, P1, H1, czDoorway);
	CDoorway* CalZone = new CDoorway(m_P0, P1, m_H0, H1);
	m_View->m_ProbabilityGrid->m_ExecutableObjectList->Add(CalZone);
	m_View->m_ProbabilityGrid->m_RWGraphicObjectList->Add(CalZone);
	CalZone->SetCaption("CheckPoint");
	m_P0.y += 36*cTPI;
//	m_LabelList->AddCalibrationZoneLabel("Doorway", m_P0, CalZone, m_View);
//	CCalibrationZoneLabel* NewLabel = new CCalibrationZoneLabel("Doorway", m_P0, m_View, m_LabelList, CalZone);
//	m_LabelList->Add(NewLabel);

	m_View->RedrawWindow();
//	m_RobotWorldWindow->AddObject(OT_ELLIPSE, P1.x - 10, P1.y + 10, P1.x + 10, P1.y - 10);
//	m_RobotWorldWindow->SetLineColor(0);
//	m_RobotWorldWindow->SetFillColor(COL_RED);

//	m_RobotWorldWindow->AddObject(OT_LINE, P1.x, P1.y, P1.x + 40 * cos(H1), P1.y + 40*sin(H1));
//	m_RobotWorldWindow->SetLineColor(0);
}
