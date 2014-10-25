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
// HomeBase.cpp: implementation of the CHomeBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "global.h"
#include "HomeBase.h"
#include "CTDPath.h"
#include "RobotWorld Resource.h"
#include "RobotWorldView.h"
#include "DlgErrorCheckingWoodWithVacuum.h"
#include "RothMath.h"

IMPLEMENT_SERIAL(CHomeBase, CCornerWall, VERSIONABLE_SCHEMA | VERSION_NUMBER)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHomeBase::CHomeBase() : CCornerWall()
{
    m_Caption = "Home Base";
    m_Docking = false;
}

CHomeBase::~CHomeBase()
{

}

TTaskResult CHomeBase::Calibrate(CRobot * Robot, robPOINT* DPosition, double* DHeading)
/*Home base calibration first does a wall calibration, and then calibrates against
the corner.  If the robot touches the home base while calibrating against the wall,
the robot's position is automatically set to its proper value, and the corner
calibration is skipped.*/
{
    //	fprintf(stderr, "!!!!!!!!!!HomeBase Calibration!!!!!!!!!!!!\n");
    if ((m_FloorType == ftWood) && (Robot->GetAttachmentType() == atVacuum))
    {
        CDlgErrorCheckingWoodWithVacuum Warning;
        Warning.DoModal();
        return TR_Error;
    }

    Robot->SetFloorType(m_FloorType);

    Robot->SetTouchedHomeBase(false);
    TTaskResult TaskResult = CWall::Calibrate(Robot, DPosition, DHeading);
    Robot->ShowCalibrationSuccessfulDialog(*DPosition, *DHeading);

    if (TaskResult != TR_TaskComplete)
    {
        return TaskResult;
    }

    if (Robot->GetTouchedHomeBase())
    {
        robPOINT P;
        P.x = (m_P0.x + m_P1.x) / 2 - cRRNoseLength * cos(m_Heading);
        P.y = (m_P0.y + m_P1.y) / 2 - cRRNoseLength * sin(m_Heading);

        DPosition->x += P.x - Robot->m_Position.x;
        DPosition->y += P.y - Robot->m_Position.y;
        Robot->SetPosition(P);
        //		Robot->SetFlipHandle(true);
    }
    else
    {
        TaskResult = CalibrateCorner(Robot, DPosition, DHeading);
        Robot->ShowCalibrationSuccessfulDialog(*DPosition, *DHeading);
        Robot->SetTouchedHomeBase(false);

        if ((!m_Docking) && (TaskResult == TR_TaskComplete))
        {
            /*To reduce calibration error, square to main wall again*/
            /*Find the best point to backup to (need to keep the angulator on the bumper from screwing up homebase tabs)*/
            robPOINT P = Robot->GetPosition();
            P.x -= cRRWidth * cos(m_Heading2);
            P.y -= cRRWidth * sin(m_Heading2);
            robPOINT P2;
            P2.x = (m_P0.x + m_P1.x) / 2;
            P2.y = (m_P0.y + m_P1.y) / 2;

            if (Distance(P, P2) <= sqrt(SQR(cCornerWallBackupDistance + cRRNoseLength) + SQR(cRRWidth / 2 + 40)))
            {
                P2 = Robot->GetHandlePosition(-cFollowCTDPathHandleLength);
                P2.x -= cRRWidth / 2 * cos(m_Heading2);
                P2.y -= cRRWidth / 2 * sin(m_Heading2);
                P.x = (m_P0.x + m_P1.x) / 2 - (cCornerWallBackupDistance + cRRNoseLength + 30) * cos(m_Heading);
                P.x -= cFollowCTDPathHandleLength * cos(m_Heading2);
                P.y = (m_P0.y + m_P1.y) / 2 - (cCornerWallBackupDistance + cRRNoseLength + 30) * sin(m_Heading);
                P.y -= cFollowCTDPathHandleLength * sin(m_Heading2);
                CCTDPath* Path = new CCTDPath(cComputerControlSpeed);
                Path->AddPoint(Robot->GetHandlePosition(-cFollowCTDPathHandleLength));
                Path->AddPoint(P2);
                Path->AddPoint(P);
                Robot->AddCTDPathToInstructions(Path, cMinPositionZone, true);
                TaskResult = Robot->FollowInstructions(true, true, IR_InstructionComplete);

                if (TaskResult != TR_TaskComplete)
                {
                    return TaskResult;
                }
            }
            else
            {
                TaskResult = Robot->BackupFromWall();

                if (TaskResult != TR_TaskComplete)
                {
                    return TaskResult;
                }
            }

            TaskResult = CWall::Calibrate(Robot, DPosition, DHeading);
            Robot->ShowCalibrationSuccessfulDialog(*DPosition, *DHeading);
            //			fprintf(stderr, "HomeBase Calibration, %5.2f, %5.2f, %8.5f\n", DPosition->x, DPosition->y, *DHeading);
            //			fprintf(stderr, "HomeBase Calibration, %5.2f, %5.2f, %8.5f\n", DPosition->x, DPosition->y, *DHeading);
            //			fprintf(stderr, "HomeBase Calibration, %5.2f, %5.2f, %8.5f\n", DPosition->x, DPosition->y, *DHeading);
            //			fprintf(stderr, "HomeBase Calibration, %5.2f, %5.2f, %8.5f\n", DPosition->x, DPosition->y, *DHeading);
        }

        Robot->SetTouchedHomeBase(false);
    }

    return TaskResult;
}

TTaskResult CHomeBase::SpecialFunction(CRobot * Robot, short Speed)
{
    /*If we're not docking, just do the regular calibration routine*/
    if (!m_Docking)
    {
        return CCornerWall::SpecialFunction(Robot, Speed);
    }

    /*If we're docking, do the following*/
    /*Turn off Probability Grid Updating*/
    TRobotInfo OldRobotInfo = Robot->GetRobotInfo();
    Robot->SetLearnRoom(false);
    /*Show the EStop dialog window*/
    Robot->SetEStopVisible(true);
    Robot->SetState(stCalibrating);
    /*Calibrate*/
    Robot->PurgeInstructions();
    TTaskResult CalibrationResult;
    unsigned char Retries = 0;
    const unsigned char cMaxRetries = 2;
    bool KeepTrying = true;
    robPOINT DPosition;
    double DHeading = 0;

    do
    {
        Robot->SetEnableRepositionOnUnknownObstacle(false);
        CalibrationResult = Calibrate(Robot, &DPosition, &DHeading);
        Robot->SetEnableRepositionOnUnknownObstacle(true);

        if (CalibrationResult != TR_TaskComplete)
        {
            goto EXECUTION_ERROR;
        }

        Robot->Stop();
        Robot->ShowCalibrationSuccessfulDialog(DPosition, DHeading);
        /*Find the home base*/
        robPOINT HomeBaseLocation;
        HomeBaseLocation.x = (m_P0.x + m_P1.x) / 2;
        HomeBaseLocation.y = (m_P0.y + m_P1.y) / 2;

        //	Robot->SendFindHomeBase(m_Heading2, m_Heading, HomeBaseLocation);
        if (!Robot->GetOnHomeBase())
        {
            Robot->AddFindHomeBaseToInstructions(m_Heading2, m_Heading, HomeBaseLocation, !Robot->GetTouchedHomeBase());
            CalibrationResult = Robot->FollowInstructions(true, true, IR_InstructionComplete, false);

            if ((CalibrationResult != TR_TaskComplete) && (CalibrationResult != TR_EmergencyStop) && (Retries < cMaxRetries))
            {
                Robot->SetHandleLength(cFollowCTDPathHandleLength);
                Robot->AddPositionVelocityToInstructions(GetEntryPoint(), NULL, cModPositionZone, cComputerControlSpeed, true);
                CalibrationResult = Robot->FollowInstructions(true, true, IR_InstructionComplete);

                if (CalibrationResult != TR_TaskComplete)
                {
                    KeepTrying = false;
                }
            }
            else
            {
                if (CalibrationResult == TR_TaskComplete)
                {
                    /*When you've found the home base, wait a while for the robot to stop bouncing on and off the charger.  Otherwise
                    the robot may try to home itself again*/
                    Robot->AddWaitToInstructions(3000);
                    CalibrationResult = Robot->FollowInstructions(true, true, IR_InstructionComplete);
                }

                KeepTrying = false;
            }
        }
        else
        {
            KeepTrying = false;
        }

        Retries++;
    }
    while (KeepTrying);

EXECUTION_ERROR:
    Robot->SetRobotInfo(OldRobotInfo);

    if (CalibrationResult != TR_TaskComplete)
    {
        if (CalibrationResult != TR_EmergencyStop)
        {
            Robot->DisplayErrorText("Docking Unsuccessful");
        }
    }

    return CalibrationResult;
}

TTaskResult CHomeBase::Dock(CRobot* Robot, short Speed)
{
    m_Docking = true;
    TTaskResult TaskResult = CCornerWall::Execute(Robot, Speed);
    m_Docking = false;
    return TaskResult;
}

void CHomeBase::OnDefaultClick(robPOINT point, float WorldPointsPerPixel, CRobot* Robot)
{
    if (InsideExecuteZone(point, WorldPointsPerPixel))
    {
        TRobotInfo OldRobotInfo = Robot->GetRobotInfo();
        Robot->SetEStopVisible(true);
        Dock(Robot, cComputerControlSpeed);
        Robot->SetRobotInfo(OldRobotInfo);
    }
}

TTaskResult CHomeBase::Adjust(CRobot *Robot)
{
    ASSERT(Robot != NULL);

    if (Robot == NULL)
    {
        return TR_Error;
    }

    TTaskResult TaskResult = GoToEntrance(Robot);

    if (TaskResult == TR_TaskComplete)
    {
        TaskResult = Dock(Robot);

        if (TaskResult == TR_TaskComplete)
        {
            double phi;
            robPOINT P0 = Robot->GetTaskResultPosition();
            TLine WallLine;
            FindLineFrom2Points(m_P0, m_P1, &WallLine);
            /*Find the location of the home base along the primary wall without moving the primary wall*/
            P0 = FindPerpendicularIntersection(WallLine, P0);

            phi = m_Heading + pi / 2;
            m_P0 = P0;
            m_P1 = P0;
            m_P1.x += cRRWidth / 2 * cos(phi);
            m_P1.y += cRRWidth / 2 * sin(phi);
            m_P0.x += -cRRWidth / 2 * cos(phi);
            m_P0.y += -cRRWidth / 2 * sin(phi);
            m_ExecutableObjectList->RemoveObjectFromStoredPaths(this);
            CalculateBounds();
        }
    }

    return TaskResult;
}

bool CHomeBase::Create(CRobot * Robot, bool WallToLeft, double Heading, bool SnapBaseToHeading)
/*******************************************************************************
Robot: Pointer to the robot to use when creating the home base
WallToLeft:	true if the corner wall is to the left of the robot.  False if it's to the right
Heading:	Heading of the main wall.  Only used if SnapBaseToOrigin == true.
SnapBaseToOrigin: if true, the home base will be located at the origin, and lined up with the X-Y axes.
*******************************************************************************/
{
    if ((Robot->GetFloorType() == ftWood) && (Robot->GetAttachmentType() == atVacuum))
    {
        CDlgErrorCheckingWoodWithVacuum Warning;
        Warning.DoModal();
        return false;
    }

    TRobotInfo OldRobotInfo = Robot->GetRobotInfo();
    Robot->SetEnableRepositionOnUnknownObstacle(false);
    Robot->SetEStopVisible(true);
    m_Caption = "Home Base";
    m_FloorType = Robot->GetFloorType();
    bool OldLearnRoom = Robot->GetLearnRoom();
    /*Don't have learn room on during CreateMainWall because the robot will be repositioned.*/
    Robot->SetLearnRoom(false);
    bool result = false;

    if (CreateMainWall(Robot, Heading, SnapBaseToHeading))
    {
        Robot->SetLearnRoom(OldLearnRoom);
        result = CreateCorner(Robot, WallToLeft, cDefaultWallBuffer);

        if (result)
        {
            if (!Robot->GetAlreadyCalibratedFloorType(m_FloorType))
            {
                int HCF;
                CCTDPath* Path = new CCTDPath(cComputerControlSpeed);
                Path->AddPoint(Robot->GetHandlePosition(-cFollowCTDPathHandleLength));
                robPOINT P = Robot->GetPosition();
                P.x -= 5 * cTPI * cos(Robot->GetHeading());
                P.y -= 5 * cTPI * sin(Robot->GetHeading());
                Path->AddPoint(P);
                Path->AddPoint(GetEntryPoint());
                Robot->AddCTDPathToInstructions(Path, cModPositionZone, TRUE);
                Robot->AddHeadingToInstructions(m_Heading, NULL, cMinHeadingZone);
                Robot->AddSquareToWallToInstructions(m_Heading, m_FloorType == ftWood);

                if (Robot->FollowInstructions() == IR_InstructionComplete)
                {
                    if (Robot->CalibrateFloorSurface(&HCF) == TR_TaskComplete)
                    {
                        Robot->SetHeadingCorrectionFactor(m_FloorType, HCF);
                        robPOINT DPosition;
                        double DHeading;

                        if (Calibrate(Robot, &DPosition, &DHeading) == TR_TaskComplete)
                        {
                            Path = new CCTDPath(cComputerControlSpeed);
                            Path->AddPoint(Robot->GetHandlePosition(-cFollowCTDPathHandleLength));
                            P = Robot->GetPosition();
                            P.x -= 5 * cTPI * cos(m_Heading);
                            P.y -= 5 * cTPI * sin(m_Heading);
                            Path->AddPoint(P);
                            Path->AddPoint(GetEntryPoint());
                            Robot->AddCTDPathToInstructions(Path, cModPositionZone, TRUE);
                            Robot->FollowInstructions();
                        }
                    }
                }
            }
            else
            {
                /*Get back in front of the home base*/
                CCTDPath* Path = new CCTDPath(cComputerControlSpeed);
                Path->AddPoint(Robot->GetHandlePosition(-cFollowCTDPathHandleLength));
                robPOINT P = Robot->GetPosition();
                P.x -= 5 * cTPI * cos(Robot->GetHeading());
                P.y -= 5 * cTPI * sin(Robot->GetHeading());
                Path->AddPoint(P);
                Path->AddPoint(GetEntryPoint());
                Robot->AddCTDPathToInstructions(Path, cModPositionZone, TRUE);
                Robot->FollowInstructions();
            }
        }
    }
    else
    {
        result = false;
    }

    Robot->SetHeadingError(0);
    robPOINT Zero(0, 0);
    Robot->SetPositionError(Zero);
    Robot->SetRobotInfo(OldRobotInfo);
    return result;
}

bool CHomeBase::CreateMainWall(CRobot *Robot, double Heading, bool SnapBaseToOrigin)
/*******************************************************************************
Robot: Pointer to the robot to use when creating the home base
Heading:	Heading of the main wall.  Only used if SnapBaseToOrigin == true.
SnapBaseToOrigin: if true, the home base will be located at the origin, and lined up with the X-Y axes.
*******************************************************************************/
{
    m_FloorType = Robot->GetFloorType();
    TRobotInfo OldRobotInfo = Robot->GetRobotInfo();
    Robot->SetEStopVisible(true);
    Robot->SetEnableRepositionOnUnknownObstacle(false);

    /*Find the wall*/
    if (Robot->SquareToWall() != TR_TaskComplete)
    {
        Robot->SetRobotInfo(OldRobotInfo);
        return false;
    }

    robPOINT RobotPosition;

    if (SnapBaseToOrigin)
    {
        robPOINT TaskResultPosition = Robot->GetTaskResultPosition();
        TaskResultPosition.x = -TaskResultPosition.x;
        TaskResultPosition.y = -TaskResultPosition.y;
        double TaskResultHeading = Robot->GetTaskResultHeading();
        Robot->AddToHeading(Heading - TaskResultHeading);
        Robot->AddToPosition(TaskResultPosition);
        m_Heading = BringAngleInRange(Heading);
        RobotPosition.x = RobotPosition.y = 0;
    }
    else
    {
        m_Heading = Robot->GetTaskResultHeading();
        RobotPosition = Robot->GetTaskResultPosition();
    }

    double phi;
    /*Find the corners of the calibration wall given the 2 robot positions*/
    phi = m_Heading + pi / 2;
    m_P0 = RobotPosition;
    m_P1 = RobotPosition;
    m_P1.x += cRRWidth / 2 * cos(phi) + cRRNoseLength * cos(m_Heading);
    m_P1.y += cRRWidth / 2 * sin(phi) + cRRNoseLength * sin(m_Heading);
    m_P0.x += -cRRWidth / 2 * cos(phi) + cRRNoseLength * cos(m_Heading);
    m_P0.y += -cRRWidth / 2 * sin(phi) + cRRNoseLength * sin(m_Heading);
    //	m_ZoneType = czWall;
    CalculateBounds();
    Robot->SetRobotInfo(OldRobotInfo);
    return true;
}

robPOINT CHomeBase::GetDockedPosition()
/*This function returns the position the robot is in when it is docked at this home base.*/
{
    robPOINT RobotDockedPosition;
    RobotDockedPosition.x = (m_P0.x + m_P1.x) / 2 - cRRNoseLength * cos(GetHeading());
    RobotDockedPosition.y = (m_P0.y + m_P1.y) / 2 - cRRNoseLength * sin(GetHeading());
    return RobotDockedPosition;
}
