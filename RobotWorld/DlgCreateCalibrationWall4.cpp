// DlgCreateCalibrationWall4.cpp : implementation file
//

#include "stdafx.h"
#include "RobotWorld.h"
#include "DlgCreateCalibrationWall4.h"
//#include "CalibrationZoneLabel.h"
#include "RobotWorldView.h"
#include "Probability Grid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateCalibrationWall4 dialog


CDlgCreateCalibrationWall4::CDlgCreateCalibrationWall4(CRobotWorldView* pView)
{
    //{{AFX_DATA_INIT(CDlgCreateCalibrationWall4)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    Create(CDlgCreateCalibrationWall4::IDD, pView);
    m_View = pView;
}


void CDlgCreateCalibrationWall4::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgCreateCalibrationWall4)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCreateCalibrationWall4, CDialog)
    //{{AFX_MSG_MAP(CDlgCreateCalibrationWall4)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateCalibrationWall4 message handlers

void CDlgCreateCalibrationWall4::OnOK()
{
    CCalibrationZone* CalZone;
    CDialog::OnOK();
    /*Show the EStop dialog window*/
    BOOL EStopWasVisible = m_View->m_Robot->GetEStopVisible();
    m_View->m_Robot->SetEStopVisible(true);
    //Run into wall
    m_View->m_Robot->AddVelocitiesToInstructions(cObstacleDetectionSpeed, cObstacleDetectionSpeed, 10000);

    if (m_View->m_Robot->FollowInstructions(TRUE, TRUE) != IR_HitObstacle)
    {
        /*Hide the EStop*/
        if (!EStopWasVisible)
        {
            m_View->m_Robot->SetEStopVisible(false);
        }

        return;
    }

    //Square to the wall
    if (m_View->m_Robot->SquareToWall(m_View->m_Robot->m_MotorObstacle.MotorObstacle) != IR_InstructionComplete)
    {
        /*Hide the EStop*/
        if (!EStopWasVisible)
        {
            m_View->m_Robot->SetEStopVisible(false);
        }

        return;
    }

    /*Hide the EStop*/
    if (!EStopWasVisible)
    {
        m_View->m_Robot->SetEStopVisible(false);
    }

    //	CalZone = new CCornerWall(m_P0, m_P1, m_View->m_Robot->m_Position, m_H0, m_H1, m_View->m_Robot->m_Heading);
    m_View->m_ProbabilityGrid->m_ExecutableObjectList->Add(CalZone);
    m_View->m_ProbabilityGrid->m_RWGraphicObjectList->Add(CalZone);
    CalZone->SetCaption("CheckPoint");

    if (CalZone != NULL)
    {
        //		m_LabelList->AddCalibrationZoneLabel("Corner Wall", m_P0, CalZone, m_View);
        //		CCalibrationZoneLabel* NewLabel = new CCalibrationZoneLabel("Corner Wall", m_P0, m_View, m_LabelList, CalZone);
        //		m_LabelList->Add(NewLabel);
    }
}
