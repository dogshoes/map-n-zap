// DlgCreateCalibrationDoorway1.cpp : implementation file
//

#include "stdafx.h"
#include "RobotWorld.h"
#include "DlgCreateCalibrationDoorway1.h"
#include <math.h>
#include "Calibration Zone.h"
#include "robot.h"
#include "RobotWorldView.h"

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
// CDlgCreateCalibrationDoorway1 dialog


CDlgCreateCalibrationDoorway1::CDlgCreateCalibrationDoorway1(CRobotWorldView* View)
{
    Create(IDD_CREATECALIBRATIONDOORWAY1, View);
    m_View = View;
    m_DlgCreateCalibrationDoorway2 = new CDlgCreateCalibrationDoorway2(View);
    //{{AFX_DATA_INIT(CDlgCreateCalibrationDoorway1)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgCreateCalibrationDoorway1::DoDataExchange(CDataExchange* pDX)
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
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgCreateCalibrationDoorway1)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCreateCalibrationDoorway1, CDialog)
    //{{AFX_MSG_MAP(CDlgCreateCalibrationDoorway1)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateCalibrationDoorway1 message handlers

void CDlgCreateCalibrationDoorway1::OnOK()
/*******************************************************************************
//Created: 01/10/97 S.R.
//Last Revision: 11/13/97 S.R.
//Parameters:
//	none.
//Return Value:
//	none.
********************************************************************************
//Description:
//		This function creates a calibration doorway.  Once the user has dragged the
//	robot to one corner of the calibration doorway, the function has the robot
//	find the corner.  Then a dialog box saying to drag the robot to the other
//	corner is displayed.  After the user does that and presses o.k., the second
//	corner is found.
//Method:
//
*******************************************************************************/
{
    CDialog::OnOK();
    /*Show the EStop dialog window*/
    BOOL EStopWasVisible = m_View->m_Robot->GetEStopVisible();
    m_View->m_Robot->SetEStopVisible(true);

    /*Find the corner*/
    if (m_View->m_Robot->FindCorner(&(m_DlgCreateCalibrationDoorway2->m_P0),
                                    &(m_DlgCreateCalibrationDoorway2->m_H0)) != IR_InstructionComplete)
    {
        if (!EStopWasVisible)
        {
            m_View->m_Robot->SetEStopVisible(false);
        }

        return;
    }

    POINT p;
    p.x = Round(m_DlgCreateCalibrationDoorway2->m_P0.x);
    p.y = Round(m_DlgCreateCalibrationDoorway2->m_P0.y);
    double h = m_DlgCreateCalibrationDoorway2->m_H0;

    //	m_RobotWorldWindow->AddObject(OT_ELLIPSE, p.x - 10, p.y + 10, p.x + 10, p.y - 10);
    //	m_RobotWorldWindow->SetLineColor(0);
    //	m_RobotWorldWindow->SetFillColor(COL_RED);
    //	m_RobotWorldWindow->AddObject(OT_LINE, p.x, p.y, p.x + 40 * cos(h), p.y + 40*sin(h));
    //	m_RobotWorldWindow->SetLineColor(0);
    /*Hide the EStop*/
    if (!EStopWasVisible)
    {
        m_View->m_Robot->SetEStopVisible(false);
    }

    m_DlgCreateCalibrationDoorway2->ShowWindow(SW_SHOW);
}

CDlgCreateCalibrationDoorway1::~CDlgCreateCalibrationDoorway1()
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
{
    delete m_DlgCreateCalibrationDoorway2;
}
