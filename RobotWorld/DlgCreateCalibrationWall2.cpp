// DlgCreateCalibrationWall2.cpp : implementation file
//

#include <math.h>
#include "stdafx.h"
#include "RobotWorld.h"
#include "DlgCreateCalibrationWall2.h"
#include "DlgCreateCalibrationWall4.h"
#include "RobotWorldView.h"
#include "Calibration Zone.h"
#include "Probability Grid.h"
#include "robot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateCalibrationWall2 dialog

/*******************************************************************************
//Created: 12/03/96 S.R.
//Last Revision: 12/03/96 S.R.
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

CDlgCreateCalibrationWall2::CDlgCreateCalibrationWall2(CRobotWorldView* pView)
{
	//{{AFX_DATA_INIT(CDlgCreateCalibrationWall2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	Create(IDD_CREATECALIBRATIONWALL2, pView);
	m_DlgCreateCalibrationWall4 = new CDlgCreateCalibrationWall4(pView);
	m_View = pView;
}


void CDlgCreateCalibrationWall2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCreateCalibrationWall2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCreateCalibrationWall2, CDialog)
	//{{AFX_MSG_MAP(CDlgCreateCalibrationWall2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateCalibrationWall2 message handlers

void CDlgCreateCalibrationWall2::OnOK() 
/*******************************************************************************
//Created: 12/03/96 S.R.
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
	CCalibrationZone* CalZone;
	CDialog::OnOK();
/*Show the EStop dialog window*/
	BOOL EStopWasVisible = m_View->m_Robot->GetEStopVisible();
	m_View->m_Robot->SetEStopVisible(true);
//Run into wall
	m_View->m_Robot->AddVelocitiesToInstructions(cObstacleDetectionSpeed, cObstacleDetectionSpeed, 10000);
	if (m_View->m_Robot->FollowInstructions(TRUE, TRUE) != IR_HitObstacle) {
	/*Hide the EStop*/
		if (!EStopWasVisible) {
			m_View->m_Robot->SetEStopVisible(false);
		}
		return;
	}
//Square to the wall
	if (m_View->m_Robot->SquareToWall(m_View->m_Robot->m_MotorObstacle.MotorObstacle) != IR_InstructionComplete) {
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
/*Make sure the headings are within a certain range of each other.*/
	if (AbsAngularSeparation(m_H0, m_View->m_Robot->m_Heading) > 0.1) {
		AfxMessageBox("Something went wrong.  Try again."); 
		return;
	}
	CDialog DlgCreateCalibrationWall3(IDD_CREATECALIBRATIONWALL3);
	if (DlgCreateCalibrationWall3.DoModal() == IDOK) {
		CreateCornerWall();
//		m_DlgCreateCalibrationWall4->m_P0 = m_P0;
//		m_DlgCreateCalibrationWall4->m_H0 = m_H0;
//		m_DlgCreateCalibrationWall4->m_P1 = m_View->m_Robot->m_Position;
//		m_DlgCreateCalibrationWall4->m_H1 = m_View->m_Robot->m_Heading;
//		m_DlgCreateCalibrationWall4->ShowWindow(SW_SHOW);
	} else {
		m_View->m_Robot->SetEStopVisible(true);
//		CalZone = new CWall(m_P0, m_View->m_Robot->m_Position, m_H0, m_View->m_Robot->m_Heading);
		m_View->m_ProbabilityGrid->m_ExecutableObjectList->Add(CalZone);
		m_View->m_ProbabilityGrid->m_RWGraphicObjectList->Add(CalZone);
		CalZone->SetCaption("CheckPoint");
//		CalZone = m_CalibrationZoneHandler->ConstructCalibrationZone(m_P0, m_H0,
//			m_View->m_Robot->m_Position, m_View->m_Robot->m_Heading, czWall);
	/*Hide the EStop*/
		if (!EStopWasVisible) {
			m_View->m_Robot->SetEStopVisible(false);
		}
		m_P0.y += 36*cTPI;
		if (CalZone != NULL) {
//			m_LabelList->AddCalibrationZoneLabel("Wall", m_P0, CalZone, m_View);
//			CCalibrationZoneLabel* NewLabel = new CCalibrationZoneLabel("Wall", m_P0, m_View, m_LabelList, CalZone);
//			m_LabelList->Add(NewLabel);
		} else {
		}
	}
	m_View->RedrawWindow();
}

void CDlgCreateCalibrationWall2::CreateCornerWall(void)
{

#ifdef IMPLEMENTED
	robPOINT P1 = m_View->m_Robot->m_Position;
	double H1 = m_View->m_Robot->m_Heading;
	unsigned char FeelingMotor;
	TTaskResult TaskResult;
/*Back up, Turn towards the reference point*/
	double phi = atan2(m_View->m_Robot->m_Position.y - m_P0.y, m_View->m_Robot->m_Position.x - m_P0.x);
	if ( AbsAngularSeparation(phi, m_View->m_Robot->m_Heading - pi/2) < AbsAngularSeparation(phi, m_View->m_Robot->m_Heading + pi/2) ) {
		phi = m_View->m_Robot->m_Heading - pi/2;
		FeelingMotor = 1;
	} else {
		phi = m_View->m_Robot->m_Heading + pi/2;
		FeelingMotor = 0;
	}
	robPOINT P;
	P.x = m_View->m_Robot->m_Position.x - (cRRWidth/2 + CCornerWall::CornerBackupDistance + 10 - cRRNoseLength) * cos(m_View->m_Robot->m_Heading);
	P.y = m_View->m_Robot->m_Position.y - (cRRWidth/2 + CCornerWall::CornerBackupDistance + 10 - cRRNoseLength) * sin(m_View->m_Robot->m_Heading);
	unsigned char OldHandleLength = m_View->m_Robot->GetHandleLength();
	TObstacleSensitivity OldSensitivity = m_View->m_Robot->GetObstacleSensitivity();
	m_View->m_Robot->SetHandleLength(10);
	m_View->m_Robot->SetObstacleSensitivity(osMediumSensitivity);
	m_View->m_Robot->AddPositionVelocityToInstructions(P, NULL, 10, cComputerControlSpeed, TRUE, 10000);
	m_View->m_Robot->AddHeadingToInstructions(phi, NULL, 0, cComputerControlHeadingSpeed, 10000);
	if ( (TaskResult = m_View->m_Robot->FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
		goto Error;
	}
/*run into reference point*/
//	m_View->m_Robot->SetObstacleSensitivity(osHighSensitivity);
	m_View->m_Robot->AddStraightVelocityToInstructions(phi, cObstacleDetectionSpeed, NULL, 10000);
	if ( (TaskResult = m_View->m_Robot->FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
		goto Error;
	}
/*push correct wheel up to the reference*/
	if (FeelingMotor == 0) { 
		m_View->m_Robot->AddVelocitiesToInstructions(cObstacleDetectionSpeed, 0, 10000);	
		if ( (TaskResult = m_View->m_Robot->FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
			goto Error;
		}
	} else {
		m_View->m_Robot->AddVelocitiesToInstructions(0, cObstacleDetectionSpeed, 10000);	
		if ( (TaskResult = m_View->m_Robot->FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
			goto Error;
		}
	}
	CCornerWall* CalZone;
//	CalZone = new CCornerWall(m_P0, P1, m_View->m_Robot->m_Position, m_H0, H1, m_View->m_Robot->m_Heading);
	m_View->m_ProbabilityGrid->m_ExecutableObjectList->Add(CalZone);
	m_View->m_ProbabilityGrid->m_RWGraphicObjectList->Add(CalZone);
	CalZone->SetCaption("CheckPoint");
//	m_P0.y += 36*cTPI;
	if (CalZone != NULL) {
//		m_LabelList->AddCalibrationZoneLabel("Corner Wall", m_P0, NewZone, m_View);
//		CCalibrationZoneLabel* NewLabel = new CCalibrationZoneLabel("Corner Wall", m_P0, m_View, m_LabelList, NewZone);
//		m_LabelList->Add(NewLabel);
	}
Error:
	m_View->m_Robot->SetHandleLength(OldHandleLength);
	m_View->m_Robot->SetObstacleSensitivity(OldSensitivity);
	m_View->m_Robot->SetMaxSpeed(cMaxSpeed);
#endif
}

CDlgCreateCalibrationWall2::~CDlgCreateCalibrationWall2()
{
	delete m_DlgCreateCalibrationWall4;
}
