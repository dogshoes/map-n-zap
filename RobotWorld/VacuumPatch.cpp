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
/*******************************************************************************
//Created: 09/25/97 S.R.
********************************************************************************
//Description:
//
*******************************************************************************/

#include "stdafx.h"
#include <math.h>
#include "VacuumPatch.h"
#include "RRDim.h"
#include "RobotWorld Resource.h"
#include "DlgCreateVacuumPatch.h"
#include "Probability Grid.h"
#include "CTDPath.h"
#include "global.h"

const int cDefaultVacuumSpeed = 100;

IMPLEMENT_SERIAL(CVacuumPatch, CExecutableRWGraphicObject, VERSIONABLE_SCHEMA | VERSION_NUMBER)

/*******************************************************************************
//Created: 10/27/97 S.R.
//Last Revision: 10/27/97 S.R.
//Parameters:
//	none.
//Return Value:
//	none.
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/

CVacuumPatch::CVacuumPatch(robRECT PatchBounds, TEnterSide EnterSide, unsigned short PathWidth, unsigned short TurnDiameter, CExecutableRWGraphicObjectList* ExecutableObjectList): CExecutableRWGraphicObject()
/*******************************************************************************
//Created: 09/25/97 S.R.
//Last Revision: 10/23/97 S.R.
//Parameters:
//	PatchBounds:	The boundaries of the vacuum patch (in robot coordinates).
//	EnterSide:		The side of the vacuum patch the robot should enter from.
//	PathWidth:		The width of a vacuum path (in 0.1" units)
//	TurnDiameter:	The turning diameter for the robot (in 0.1" units)
//Return Value:
//	none.
********************************************************************************
//Description:
//		This function creates a vacuum patch with the given dimensions.  First
//	off the function normalizes PatchBounds.  Then, given which side the robot
//	is entering from, the function makes sure that the width and height are
//	large enough for the robot to get through.  If they're not, the funciton
//	expands the zone by moving the lower right hand corner.  Finally, given
//	the entry side, the function figures out what m_HeadForward and m_HeadNext
//	should be and sets them.
//Method:
//
*******************************************************************************/
{
    m_ExecutableObjectList = ExecutableObjectList;
    m_VacuumingSpeed = cDefaultVacuumSpeed;
    const float cTurnRadius = cRRWheelBase / 2.0f;
    //	m_CaptionDimensions.x = 340;		/*Some big number so the text gets printed the first time things are drawn*/
    //	m_CaptionDimensions.y = 340;		/*Some big number so the text gets printed the first time things are drawn*/
    m_VacuumPath = NULL;
    m_EnterSide = EnterSide;
    m_PathWidth = PathWidth;
    m_TurnDiameter = TurnDiameter;

    if (m_TurnDiameter < m_PathWidth)
    {
        m_TurnDiameter = m_PathWidth;
    }

    m_VacuumLength = 220;
    m_Bounds = PatchBounds;
    m_IsVisible = TRUE;

    /*Normalize the m_Bounds rectangle*/
    double temp;

    if (m_Bounds.left > m_Bounds.right)
    {
        temp = m_Bounds.left;
        m_Bounds.left = m_Bounds.right;
        m_Bounds.right = temp;
    }

    if (m_Bounds.top < m_Bounds.bottom)
    {
        temp = m_Bounds.top;
        m_Bounds.top = m_Bounds.bottom;
        m_Bounds.bottom = temp;
    }

    /*Make sure m_Bounds rectangle is wide enough for robot to go through*/
    BOOL Resized = FALSE;

    if (m_Bounds.top - m_Bounds.bottom < MinHeight())
    {
        m_Bounds.bottom = m_Bounds.top - MinHeight();
        Resized = TRUE;
    }

    if (m_Bounds.right - m_Bounds.left < MinWidth())
    {
        m_Bounds.right = m_Bounds.left + MinWidth();
        Resized = TRUE;
    }

    if (Resized)
    {
        AfxMessageBox("The Vacuum Patch was too narrow, and was resized");
    }

    SetEntryPointAndHeadings();
    CalculateVacuumPaths();
}
#ifdef JUNK
CVacuumPatch::CVacuumPatch(robRECT PatchBounds): CExecutableRWGraphicObject()
/*******************************************************************************
//Created: 09/28/97 S.R.
//Last Revision: 10/28/97 S.R.
//Parameters:
//	PatchBounds:	The boundaries of the vacuum patch (in robot coordinates).
//	Robot:			The robot control class to control the robot.
//Return Value:
//	none.
********************************************************************************
//Description:
//		This function creates a vacuum patch with the given dimensions.  First
//	off the function normalizes PatchBounds.  Then, given which side the robot
//	is entering from, the function makes sure that the width and height are
//	large enough for the robot to get through.  If they're not, the funciton
//	expands the zone by moving the lower right hand corner.  Finally, given
//	the entry side, the function figures out what m_HeadForward and m_HeadNext
//	should be and sets them.
//Method:
//
*******************************************************************************/
{
    const float cTurnRadius = cRRWheelBase / 2.0;
    m_VacuumingSpeed = cDefaultVacuumSpeed;
    m_VacuumPath = NULL;
    m_VacuumLength = 220;
    m_Bounds = PatchBounds;
    m_EnterSide = esTopLft;
    m_PathWidth = 65;
    m_TurnDiameter = 220;
    m_IsVisible = TRUE;
    /*Normalize the PatchBounds rectangle*/
    long temp;

    if (m_Bounds.left > m_Bounds.right)
    {
        temp = m_Bounds.left;
        m_Bounds.left = m_Bounds.right;
        m_Bounds.right = temp;
    }

    if (m_Bounds.top < m_Bounds.bottom)
    {
        temp = m_Bounds.top;
        m_Bounds.top = m_Bounds.bottom;
        m_Bounds.bottom = temp;
    }

    /*Edit the properties of the patch*/
    EditProperties();
}
#endif

void CVacuumPatch::SetEntryPointAndHeadings()
/*******************************************************************************
//Created: 09/25/97 S.R.
//Last Revision: 09/25/97 S.R.
//Parameters:
//	none.
//Return Value:
//	none.
********************************************************************************
//Description:
//		Given the side the robot will enter on, this function finds m_EntryPoint,
//	m_HeadForward and m_HeadNext.
//Method:
//
*******************************************************************************/
{
    robPOINT tEntryPoint, tExitPoint;

    switch (m_EnterSide)
    {
    case esTopLft:
        tEntryPoint.x = m_Bounds.left + cRRWheelBase / 2.0f;
        tEntryPoint.y = m_Bounds.top;
        tExitPoint.x = m_Bounds.right - cRRWheelBase / 2.0f;
        tExitPoint.y = m_Bounds.bottom;
        m_HeadForward = -pi / 2;
        m_HeadNext = 0;
        break;

    case esTopRgt:
        tEntryPoint.x = m_Bounds.right - cRRWheelBase / 2.0f;
        tEntryPoint.y = m_Bounds.top;
        tExitPoint.x = m_Bounds.left + cRRWheelBase / 2.0f;
        tExitPoint.y = m_Bounds.bottom;
        m_HeadForward = -pi / 2;
        m_HeadNext = pi;
        break;

    case esBotLft:
        tEntryPoint.x = m_Bounds.left + cRRWheelBase / 2.0f;
        tEntryPoint.y = m_Bounds.bottom;
        tExitPoint.x = m_Bounds.right - cRRWheelBase / 2.0f;
        tExitPoint.y = m_Bounds.top;
        m_HeadForward = pi / 2;
        m_HeadNext = 0;
        break;

    case esBotRgt:
        tEntryPoint.x = m_Bounds.right - cRRWheelBase / 2.0f;
        tEntryPoint.y = m_Bounds.bottom;
        tExitPoint.x = m_Bounds.left + cRRWheelBase / 2.0f;
        tExitPoint.y = m_Bounds.top;
        m_HeadForward = pi / 2;
        m_HeadNext = pi;
        break;

    case esLftTop:
        tEntryPoint.x = m_Bounds.left;
        tEntryPoint.y = m_Bounds.top - cRRWheelBase / 2.0f;
        tExitPoint.x = m_Bounds.right;
        tExitPoint.y = m_Bounds.bottom + cRRWheelBase / 2.0f;
        m_HeadForward = 0;
        m_HeadNext = -pi / 2;
        break;

    case esLftBot:
        tEntryPoint.x = m_Bounds.left;
        tEntryPoint.y = m_Bounds.bottom + cRRWheelBase / 2.0f;
        tExitPoint.x = m_Bounds.right;
        tExitPoint.y = m_Bounds.top - cRRWheelBase / 2.0f;
        m_HeadForward = 0;
        m_HeadNext = pi / 2;
        break;

    case esRgtTop:
        tEntryPoint.x = m_Bounds.right;
        tEntryPoint.y = m_Bounds.top - cRRWheelBase / 2.0f;
        tExitPoint.x = m_Bounds.left;
        tExitPoint.y = m_Bounds.bottom + cRRWheelBase / 2.0f;
        m_HeadForward = pi;
        m_HeadNext = -pi / 2;
        break;

    case esRgtBot:
        tEntryPoint.x = m_Bounds.right;
        tEntryPoint.y = m_Bounds.bottom + cRRWheelBase / 2.0f;
        tExitPoint.x = m_Bounds.left;
        tExitPoint.y = m_Bounds.top - cRRWheelBase / 2.0f;
        m_HeadForward = pi;
        m_HeadNext = pi / 2;
        break;
    }

    SetEntryPoint(tEntryPoint);
    SetExitPoint(tExitPoint);
}

CVacuumPatch::CVacuumPatch() : CExecutableRWGraphicObject()
{
    m_VacuumingSpeed = cDefaultVacuumSpeed;
    m_VacuumPath = NULL;
    m_IsVisible = TRUE;
}

void CVacuumPatch::Move(robPOINT TopCorner)
/*******************************************************************************
//Created: 09/26/97 S.R.
//Last Revision: 09/26/97 S.R.
//Parameters:
//	TopCorner:	The new top corner for the patch (in robot world coordinates).
//Return Value:
//	none.
********************************************************************************
//Description:
//		This function moves the vacuum patch so TopCorner is the new top left
//	corner for the patch.
//Method:
//
*******************************************************************************/
{
    m_Bounds.right += TopCorner.x - m_Bounds.left;
    m_Bounds.left = TopCorner.x;
    m_Bounds.bottom += TopCorner.y - m_Bounds.top;
    m_Bounds.top = TopCorner.y;
    SetEntryPointAndHeadings();
    CalculateVacuumPaths();
}

void CVacuumPatch::Resize(TFrameSide DragSide, robPOINT point)
/*******************************************************************************
//Created: 09/29/97 S.R.
//Last Revision: 09/29/97 S.R.
//Parameters:
//	DragSide:	The side of the patch rectangle being dragged to resize.
//	point:		The point DragSide is dragged to. (in robot world coordinates)
//Return Value:
//	none.
********************************************************************************
//Description:
//		Given the side of the vacuum patch being dragged, and where it's been
//	dragged to, this function resizes the vacuum patch.
//Method:
//
*******************************************************************************/
{
    switch (DragSide)
    {
    case fsBot:
        m_Bounds.bottom = point.y;

        if (m_Bounds.top - m_Bounds.bottom < MinHeight())
        {
            m_Bounds.bottom = m_Bounds.top - MinHeight();
        }

        break;

    case fsTop:
        m_Bounds.top = point.y;

        if (m_Bounds.top - m_Bounds.bottom < MinHeight())
        {
            m_Bounds.top = m_Bounds.bottom + MinHeight();
        }

        break;

    case fsLft:
        m_Bounds.left = point.x;

        if (m_Bounds.right - m_Bounds.left < MinWidth())
        {
            m_Bounds.left = m_Bounds.right - MinWidth();
        }

        break;

    case fsRgt:
        m_Bounds.right = point.x;

        if (m_Bounds.right - m_Bounds.left < MinWidth())
        {
            m_Bounds.right = m_Bounds.left + MinWidth();
        }

        break;

    case fsTopLft:
        m_Bounds.top = point.y;

        if (m_Bounds.top - m_Bounds.bottom < MinHeight())
        {
            m_Bounds.top = m_Bounds.bottom + MinHeight();
        }

        m_Bounds.left = point.x;

        if (m_Bounds.right - m_Bounds.left < MinWidth())
        {
            m_Bounds.left = m_Bounds.right - MinWidth();
        }

        break;

    case fsTopRgt:
        m_Bounds.top = point.y;

        if (m_Bounds.top - m_Bounds.bottom < MinHeight())
        {
            m_Bounds.top = m_Bounds.bottom + MinHeight();
        }

        m_Bounds.right = point.x;

        if (m_Bounds.right - m_Bounds.left < MinWidth())
        {
            m_Bounds.right = m_Bounds.left + MinWidth();
        }

        break;

    case fsBotLft:
        m_Bounds.bottom = point.y;

        if (m_Bounds.top - m_Bounds.bottom < MinHeight())
        {
            m_Bounds.bottom = m_Bounds.top - MinHeight();
        }

        m_Bounds.left = point.x;

        if (m_Bounds.right - m_Bounds.left < MinWidth())
        {
            m_Bounds.left = m_Bounds.right - MinWidth();
        }

        break;

    case fsBotRgt:
        m_Bounds.bottom = point.y;

        if (m_Bounds.top - m_Bounds.bottom < MinHeight())
        {
            m_Bounds.bottom = m_Bounds.top - MinHeight();
        }

        m_Bounds.right = point.x;

        if (m_Bounds.right - m_Bounds.left < MinWidth())
        {
            m_Bounds.right = m_Bounds.left + MinWidth();
        }

        break;
    }

    SetEntryPointAndHeadings();
    CalculateVacuumPaths();
}

unsigned short CVacuumPatch::MinHeight(void)
/*******************************************************************************
//Created: 09/29/97 S.R.
//Last Revision: 09/29/97 S.R.
//Parameters:
//	none.
//Return Value:
//	The minimum height of the vacuum patch.
********************************************************************************
//Description:
//		This function calculates the minimum height of the vacuum patch.  This
//	minimum height allows the robot to maneuver correctly within the patch
//	without straying outside of the boundaries.
//Method:
//
*******************************************************************************/
{
    switch (m_EnterSide)
    {
    case esTopLft:
    case esTopRgt:
    case esBotLft:
    case esBotRgt:
        return (2 * m_TurnDiameter + (unsigned short)cRRWheelBase);
        break;

    default:
        return (unsigned short)cRRWheelBase;
        break;
    }
}

unsigned short CVacuumPatch::MinWidth(void)
/*******************************************************************************
//Created: 09/29/97 S.R.
//Last Revision: 09/29/97 S.R.
//Parameters:
//	none.
//Return Value:
//	The minimum width of the vacuum patch.
********************************************************************************
//Description:
//		This function calculates the minimum width of the vacuum patch.  This
//	minimum width allows the robot to maneuver correctly within the patch
//	without straying outside of the boundaries.
//Method:
//
*******************************************************************************/
{
    switch (m_EnterSide)
    {
    case esTopLft:
    case esTopRgt:
    case esBotLft:
    case esBotRgt:
        return (unsigned short)cRRWheelBase;
        break;

    default:
        return (2 * m_TurnDiameter + (unsigned short)cRRWheelBase);
        break;
    }
}

TTaskResult CVacuumPatch::SpecialFunction(CRobot* Robot, short Speed)
/*******************************************************************************
//Created: 09/30/97 S.R.
//Last Revision: 10/29/97 S.R.
//Parameters:
//	Robot:	pointer to the robot control class.
//Return Value:
//	Result of vacuuming the patch (any errors and stuff).
********************************************************************************
//Description:
//		Sends the robot through the vacuum patch.
//Method:
//
*******************************************************************************/
{
    Speed = m_VacuumingSpeed;
    robPOINT PathPoint;
    TTaskResult TaskResult;


    if (Robot == NULL)
    {
        return TR_Error;
    }

    TState OldRobotState = Robot->GetState();
    Robot->SetState(stExecuting);
    /*Show the EStop dialog window*/
    BOOL EStopWasVisible = Robot->GetEStopVisible();
    Robot->SetEStopVisible(true);
    bool OldShowCurrentPath = Robot->SetShowCurrentPath(false);
    /*Do the vacuum patch*/
    PathPoint = m_EntryPoint;
    //	PathPoint = Robot->GetRobotToPoint(PathPoint, NULL);
    //	Robot->AddPositionVelocityToInstructions(PathPoint, NULL, cLrgPositionZone, m_VacuumSpeed, FALSE, 0);
    //	TaskResult = Robot->FollowInstructions(TRUE, FALSE, IR_InstructionComplete);
    //	if (TaskResult != TR_TaskComplete) goto Error;
    double HeadForward;
    HeadForward = m_HeadForward;

    unsigned short i;
    i = 0;
    CCTDPath* RobotPath = new CCTDPath(Speed);
    bool Sign = true;
    Robot->SetExternalOutput(true);

    while (i < m_NumberPaths)
    {
        //		CCTDPath* RobotPath = new CCTDPath(Speed);

        /*Straight Stretch*/
        if (i == 0)
        {
            AddStraightLineToPath(m_EntryPoint, m_VacuumPath[i].Arc1s, &RobotPath, Robot);
            //			RobotPath->AddStraightLine(m_EntryPoint, m_VacuumPath[i].Arc1s);
        }
        else if (i < m_NumberPaths - 1)
        {
            AddStraightLineToPath(m_VacuumPath[i - 1].Arc2e, m_VacuumPath[i].Arc1s, &RobotPath, Robot);
            //			RobotPath->AddStraightLine(m_VacuumPath[i-1].Arc2e, m_VacuumPath[i].Arc1s);
        }
        else
        {
            AddStraightLineToPath(m_VacuumPath[i - 1].Arc2e, m_ExitPoint, &RobotPath, Robot);
            //			RobotPath->AddStraightLine(m_VacuumPath[i-1].Arc2e, m_ExitPoint);
        }

        if (i < m_NumberPaths - 1)
        {
            /*Turn around*/
            AddArcToPath(m_VacuumPath[i].Arc1s, m_VacuumPath[i].Arc1e, m_VacuumPath[i].Arc1m, &RobotPath, Robot);
            AddArcToPath(m_VacuumPath[i].Arc2s, m_VacuumPath[i].Arc2e, m_VacuumPath[i].Arc2m, &RobotPath, Robot);
            //			RobotPath->AddArc(m_VacuumPath[i].Arc1s, m_VacuumPath[i].Arc1e, m_VacuumPath[i].Arc1m);
            //			RobotPath->AddArc(m_VacuumPath[i].Arc2s, m_VacuumPath[i].Arc2e, m_VacuumPath[i].Arc2m);
        }

        //		Robot->AddCTDPathToInstructions(RobotPath, cLrgPositionZone, FALSE, 0);
        //		TaskResult = Robot->FollowInstructions(TRUE, FALSE, IR_InstructionComplete);
        //		if (TaskResult != TR_TaskComplete) goto Error;
        if (Sign)
        {
            HeadForward += pi;
            Sign = false;
        }
        else
        {
            HeadForward -= pi;
            Sign = true;
        }

        i++;
    }

    Robot->AddCTDPathToInstructions(RobotPath, cLrgPositionZone, false, 0);
    //	TaskResult = Robot->FollowInstructions(TRUE, FALSE, IR_InstructionComplete);
    TaskResult = Robot->FollowPathWithObstacleAvoidance(Speed, m_ExecutableObjectList->m_ProbabilityGrid, true);
    Robot->SetExternalOutput(false);

    Robot->Stop();

    /*Hide the EStop*/
    if (!EStopWasVisible)
    {
        Robot->SetEStopVisible(false);
    }

    Robot->SetState(OldRobotState);
    Robot->SetShowCurrentPath(OldShowCurrentPath);
    return TaskResult;

}

void CVacuumPatch::CalculateVacuumPaths()
/*******************************************************************************
//Created: 10/02/97 S.R.
//Last Revision: 10/29/97 S.R.
//Parameters:
//	none.
//Return Value:
//	none.
********************************************************************************
//Description:
//		This function calculates all the positions and turns the robot has to do
//	to complete the vacuum patch.
//Method:
//	See note "Vacuum Patches: Generating Paths" 10/23/97.
*******************************************************************************/
{
    const unsigned short cVacuumSpeed = 200;
    robPOINT PathPoint;
    BOOL KeepDoingPaths = TRUE;

    unsigned short nPatches;

    if (m_PathWidth == 0)
    {
        nPatches = 1;
    }
    else if (Round(sin(m_HeadNext)) != 0)
    {
        nPatches = (unsigned short)(m_Bounds.top - m_Bounds.bottom) / m_PathWidth + 1;
    }
    else
    {
        nPatches = (unsigned short)(m_Bounds.right - m_Bounds.left) / m_PathWidth + 1;
    }

    if (m_VacuumPath == NULL)
    {
        m_VacuumPath = new TVacPath[nPatches];
    }
    else
    {
        delete m_VacuumPath;
        m_VacuumPath = new TVacPath[nPatches];
    }

    robPOINT EntryPoint = m_EntryPoint;
    double HeadForward = m_HeadForward;
    unsigned short i = 0;
    robPOINT p1, p2;		/*two points on straight away part of path*/
    robPOINT p3;			/*point at middle of next path*/
    short CosFor;
    short SinFor;
    short CosNext = 0;
    short SinNext = 0;
    short TurnDiameter = m_TurnDiameter;
    short PathWidth = 0;

    while (KeepDoingPaths && i < nPatches)
    {
        CosFor = (short)Round(cos(HeadForward));
        SinFor = (short)Round(sin(HeadForward));
        CosNext = (short)Round(cos(m_HeadNext));
        SinNext = (short)Round(sin(m_HeadNext));

        p1.x = EntryPoint.x + CosFor * (TurnDiameter / 2.0 + cRRWheelBase / 2.0);
        p3.x = EntryPoint.x + CosNext * m_PathWidth + CosFor * ((m_Bounds.right - m_Bounds.left) / 2.0);

        p1.y = EntryPoint.y + SinFor * (TurnDiameter / 2.0 + cRRWheelBase / 2.0);
        p3.y = EntryPoint.y + SinNext * m_PathWidth + SinFor * ((m_Bounds.top - m_Bounds.bottom) / 2.0);

        m_VacuumPath[i].p1.x = (long)p1.x;
        m_VacuumPath[i].p1.y = (long)p1.y;

        /*Calculate the appropriate path width and turning diameter*/
        /*Calculate PathWidth*/
        if (SinNext == 1)
        {
            PathWidth = (short)min(m_PathWidth, m_Bounds.top - p1.y - cRRWheelBase / 2);
        }
        else if (SinNext == -1)
        {
            PathWidth = (short)min(m_PathWidth, p1.y - m_Bounds.bottom - cRRWheelBase / 2);
        }
        else if (CosNext == 1)
        {
            PathWidth = (short)min(m_PathWidth, m_Bounds.right - p1.x - cRRWheelBase / 2);
        }
        else if (CosNext == -1)
        {
            PathWidth = (short)min(m_PathWidth, p1.x - m_Bounds.left - cRRWheelBase / 2);
        }

        /*Forget about paths < 1 inch wide*/
        if (PathWidth <= cTPI)
        {
            KeepDoingPaths = FALSE;
        }

        /*Calculate TurnDiameter*/
        PathPoint = p1;
        PathPoint.x += CosNext * (m_TurnDiameter + cRRWheelBase / 2);
        PathPoint.y += SinNext * (m_TurnDiameter + cRRWheelBase / 2);

        /*If next vacuum path inside the patch, turn to it*/
        /*Take the widest possible turn*/
        if (m_Bounds.Inside(PathPoint))
        {
            TurnDiameter = m_TurnDiameter;
        }
        else
        {
            TurnDiameter = -1;

            if (SinNext == 1)
            {
                TurnDiameter = short(m_Bounds.top - p1.y - cRRWheelBase / 2);
            }
            else if (SinNext == -1)
            {
                TurnDiameter = short(p1.y - m_Bounds.bottom - cRRWheelBase / 2);
            }
            else if (CosNext == 1)
            {
                TurnDiameter = short(m_Bounds.right - p1.x - cRRWheelBase / 2);
            }
            else if (CosNext == -1)
            {
                TurnDiameter = short(p1.x - m_Bounds.left - cRRWheelBase / 2);
            }

            if (TurnDiameter < 0)
            {
                KeepDoingPaths = FALSE;
            }
        }

        /*Calculate correct spot for p2 given the turning radius*/
        p2.x = EntryPoint.x + CosFor * (m_Bounds.right - m_Bounds.left - (TurnDiameter + cRRWheelBase) / 2);
        p2.y = EntryPoint.y + SinFor * (m_Bounds.top - m_Bounds.bottom - (TurnDiameter + cRRWheelBase) / 2);
        m_VacuumPath[i].p2.x = long(p2.x);
        m_VacuumPath[i].p2.y = long(p2.y);

        if (KeepDoingPaths)
        {
            /*Turn*/
            if ((SinFor * CosNext == 1) || (CosFor * SinNext == -1))
            {
                /*Turn counterclockwise*/
                m_VacuumPath[i].TurnDiameter = -TurnDiameter;
            }
            else
            {
                /*Turn clockwise*/
                m_VacuumPath[i].TurnDiameter = TurnDiameter;
            }

            /*Get PathPoint to get the robot back on the next path segment*/
            PathPoint.x = p2.x + (PathWidth) * CosNext -
                          (TurnDiameter / 2.0f * sin(acos((TurnDiameter - 2.0f * PathWidth) / TurnDiameter))) * CosFor;
            PathPoint.y = p2.y + SinNext * PathWidth -
                          (TurnDiameter / 2.0f * sin(acos((TurnDiameter - 2.0f * PathWidth) / TurnDiameter))) * SinFor;
            m_VacuumPath[i].p3.x = long(PathPoint.x);
            m_VacuumPath[i].p3.y = long(PathPoint.y);


            /*Create Turning arcs*/
            m_VacuumPath[i].Arc1s.x = p2.x;
            m_VacuumPath[i].Arc1s.y = p2.y;
            m_VacuumPath[i].Arc1e.x = p2.x - CosFor * sqrt(SQR(TurnDiameter / 2.0) - SQR(PathWidth) / 4.0) + CosNext * (TurnDiameter / 2.0 + PathWidth / 2.0);
            m_VacuumPath[i].Arc1e.y = p2.y + SinNext * (TurnDiameter / 2.0 + PathWidth / 2.0) - SinFor * sqrt(SQR(TurnDiameter / 2.0) - SQR(PathWidth) / 4.0);
            m_VacuumPath[i].Arc1m.x = p2.x + (CosFor + CosNext) * TurnDiameter / 2.0;
            m_VacuumPath[i].Arc1m.y = p2.y + (SinFor + SinNext) * TurnDiameter / 2.0;
            m_VacuumPath[i].Arc2s.x = m_VacuumPath[i].Arc1e.x;
            m_VacuumPath[i].Arc2s.y = m_VacuumPath[i].Arc1e.y;

            if (TurnDiameter == PathWidth)
            {
                /*Don't do a second arc if TurnDiameter == PathWidth*/
                m_VacuumPath[i].Arc2s.x = m_VacuumPath[i].Arc1e.x;
                m_VacuumPath[i].Arc2s.y = m_VacuumPath[i].Arc1e.y;
                m_VacuumPath[i].Arc2e.x = m_VacuumPath[i].Arc1e.x;
                m_VacuumPath[i].Arc2e.y = m_VacuumPath[i].Arc1e.y;
                m_VacuumPath[i].Arc2m.x = m_VacuumPath[i].Arc1e.x;
                m_VacuumPath[i].Arc2m.y = m_VacuumPath[i].Arc1e.y;
                m_VacuumPath[i].Arc2r = 0;
                m_VacuumPath[i].Arc2c.x = m_VacuumPath[i].Arc1e.x;
                m_VacuumPath[i].Arc2c.y = m_VacuumPath[i].Arc1e.y;
            }
            else if (CosFor != 0)
            {
                /*horizontal paths*/
                double xe, ye, m, b, xs, ys;
                robPOINT center1;
                TLine Line;

                ye = p2.y + SinNext * PathWidth;
                xs = m_VacuumPath[i].Arc2s.x;
                ys = m_VacuumPath[i].Arc2s.y;
                center1.x = p2.x;
                center1.y = p2.y + SinNext * TurnDiameter / 2.0;
                FindLineFrom2Points(center1, m_VacuumPath[i].Arc2s, &Line);
                m = Line.m;
                b = Line.b;
                /*Calculate xe*/
                double temp;
                temp = sqrt(ye * (ye * (1 + SQR(m)) - 2 * b - 2 * m * xs - 2 * SQR(m) * ys) + ys * (ys * (-1 + SQR(m)) + 2 * b + 2 * m * xs));
                /*choose right square root (positive or negative)*/
                robPOINT Ps;
                Ps.x = xs;
                Ps.y = ys;
                double xe1, xe2;
                xe1 = temp + xs + m * (ys - ye);
                xe2 = -temp + xs + m * (ys - ye);
                /*find center of second arc*/
                robPOINT center2;
                center2.x = xe1;
                center2.y = m * xe1 + b;

                /*if the center of the arc for xe1 is between the center of the
                first arc, and the second arc's starting point then use xe2.  Otherwise use xe1*/
                if (PointIsBetween(center2, center1, Ps))
                {
                    xe = xe2;
                }
                else
                {
                    xe = xe1;
                }

                /*fill in the data members*/
                m_VacuumPath[i].Arc2e.x = xe;
                m_VacuumPath[i].Arc2e.y = ye;
                m_VacuumPath[i].Arc2r = abs(int(m * xe + b - ye));
                m_VacuumPath[i].Arc2c.x = xe;
                m_VacuumPath[i].Arc2c.y = ye + SinNext * m_VacuumPath[i].Arc2r;
                m_VacuumPath[i].Arc2m.x = (m_VacuumPath[i].Arc2s.x + m_VacuumPath[i].Arc2e.x) / 2;
                m_VacuumPath[i].Arc2m.y = m_VacuumPath[i].Arc2c.y + sqrt(SQR(m_VacuumPath[i].Arc2r) - SQR(m_VacuumPath[i].Arc2m.x - m_VacuumPath[i].Arc2c.x));

                if (!PointIsBetween(m_VacuumPath[i].Arc2m, m_VacuumPath[i].Arc2s, m_VacuumPath[i].Arc2e))
                {
                    m_VacuumPath[i].Arc2m.y = m_VacuumPath[i].Arc2c.y - sqrt(SQR(m_VacuumPath[i].Arc2r) - SQR(m_VacuumPath[i].Arc2m.x - m_VacuumPath[i].Arc2c.x));
                }
            }
            else
            {
                /*Vertical paths*/
                double xe, ye, m, b, xs, ys;
                robPOINT center1;
                TLine Line;

                xe = p2.x + CosNext * PathWidth;
                xs = m_VacuumPath[i].Arc2s.x;
                ys = m_VacuumPath[i].Arc2s.y;
                center1.x = p2.x + CosNext * TurnDiameter / 2.0;
                center1.y = p2.y;
                FindLineFrom2Points(center1, m_VacuumPath[i].Arc2s, &Line);
                m = Line.m;
                b = Line.b;
                /*Calculate ye*/
                double temp;
                temp = sqrt(xe * (xe * (SQR(m) + 1) + 2 * b * m - 2 * xs - 2 * m * ys) + xs * (xs * (-SQR(m) + 1) - 2 * b * m + 2 * m * ys));
                /*choose right square root (positive or negative)*/
                robPOINT Ps;
                Ps.x = xs;
                Ps.y = ys;
                double ye1, ye2;
                ye1 = (temp + xs + m * ys - xe) / m;
                ye2 = (-temp + xs + m * ys - xe) / m;
                /*find center of second arc*/
                robPOINT center2;
                center2.x = (ye1 - b) / m;
                center2.y = ye1;

                /*if the center of the arc for ye1 is between the center of the
                first arc, and the second arc's starting point then use ye2.  Otherwise use ye1*/
                if (PointIsBetween(center2, center1, Ps))
                {
                    ye = ye2;
                }
                else
                {
                    ye = ye1;
                }

                /*fill in the data members*/
                m_VacuumPath[i].Arc2e.x = xe;
                m_VacuumPath[i].Arc2e.y = ye;
                m_VacuumPath[i].Arc2r = abs(int((ye - b) / m - xe));
                m_VacuumPath[i].Arc2c.x = xe + CosNext * m_VacuumPath[i].Arc2r;
                m_VacuumPath[i].Arc2c.y = ye;
                m_VacuumPath[i].Arc2m.y = (m_VacuumPath[i].Arc2s.y + m_VacuumPath[i].Arc2e.y) / 2;
                m_VacuumPath[i].Arc2m.x = m_VacuumPath[i].Arc2c.x + sqrt(SQR(m_VacuumPath[i].Arc2r) - SQR(m_VacuumPath[i].Arc2m.y - m_VacuumPath[i].Arc2c.y));

                if (!PointIsBetween(m_VacuumPath[i].Arc2m, m_VacuumPath[i].Arc2s, m_VacuumPath[i].Arc2e))
                {
                    m_VacuumPath[i].Arc2m.x = m_VacuumPath[i].Arc2c.x - sqrt(SQR(m_VacuumPath[i].Arc2r) - SQR(m_VacuumPath[i].Arc2m.y - m_VacuumPath[i].Arc2c.y));
                }
            }
        }
        else
        {
            m_VacuumPath[i].Arc1s.x = p2.x;
            m_VacuumPath[i].Arc1s.y = p2.y;
            m_VacuumPath[i].Arc1e.x = p2.x;
            m_VacuumPath[i].Arc1e.y = p2.y;
            m_VacuumPath[i].Arc2s.x = p2.x;
            m_VacuumPath[i].Arc2s.y = p2.y;
        }

        /*Onto next path*/
        EntryPoint.x += PathWidth * CosNext +
                        CosFor * (m_Bounds.right - m_Bounds.left);
        EntryPoint.y += PathWidth * SinNext +
                        SinFor * (m_Bounds.top - m_Bounds.bottom);

        /*See if it is inside the patch*/
        if (!m_Bounds.Inside(EntryPoint))
        {
            KeepDoingPaths = FALSE;
        }

        HeadForward += pi;
        i++;
    }

    m_NumberPaths = i;
    m_VacuumPath[i].TurnDiameter = 0;
    m_VacuumPath[0].p1.x = (long)m_EntryPoint.x;
    m_VacuumPath[0].p1.y = (long)m_EntryPoint.y;
    /*Calculate ExitPoint*/
    EntryPoint.x -= PathWidth * CosNext;
    EntryPoint.y -= PathWidth * SinNext;
    SetExitPoint(EntryPoint);
}

void CVacuumPatch::Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbabilityGrid)
/*******************************************************************************
//Created: 09/24/97 S.R.
//Last Revision: 10/23/97 S.R.
//Parameters:
//	VacuumPatch:	the vacuum patch to draw.
//	ProbGridBmpUpdateRect:	the rectangle with
//	pDC:				pointer to a device context for the DD surface back buffer
//Return Value:
//	none
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    CExecutableRWGraphicObject::Draw(ProbGridBmpUpdateRect, pDC, ProbabilityGrid);

    if (m_IsVisible)
    {
        /*Check if bounds are inside the update rectangle*/
        CRect BmpBounds;
        BmpBounds.left = ProbabilityGrid->RobotToBitmapX(m_Bounds.left);
        BmpBounds.right = ProbabilityGrid->RobotToBitmapX(MAX(m_Bounds.right, m_Bounds.left));
        BmpBounds.top = ProbabilityGrid->RobotToBitmapY(m_Bounds.top);
        BmpBounds.bottom = ProbabilityGrid->RobotToBitmapY(m_Bounds.bottom);
        BmpBounds.InflateRect((int)ProbabilityGrid->m_ZoomLevel, (int)ProbabilityGrid->m_ZoomLevel);
        BmpBounds.NormalizeRect();

        if (BmpBounds.IntersectRect(&BmpBounds, &ProbGridBmpUpdateRect))
        {
            CBrush ArrowBrush(COLORREF(0xB9B9B9));
            //			CBrush BorderBrush(COLORREF(0x0));
            CPen PathPen(PS_SOLID, (int)ProbabilityGrid->m_ZoomLevel, COLORREF(0xB9B9B9));
            CPen* OldPen = pDC->SelectObject(&PathPen);
            /*Draw stuff*/
            RECT OuterRect;
            robPOINT RobotPoint;
            POINT point;
            CPoint SurfaceOffset = ProbGridBmpUpdateRect.TopLeft();
            RobotPoint.x = m_Bounds.left, RobotPoint.y = m_Bounds.top;
            point = ProbabilityGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
            OuterRect.left = point.x;
            OuterRect.top = point.y;
            RobotPoint.x = m_Bounds.right, RobotPoint.y = m_Bounds.bottom;
            point = ProbabilityGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
            OuterRect.right = point.x;
            OuterRect.bottom = point.y;

            //			pDC->FrameRect(&OuterRect, &BorderBrush);
            point.x = OuterRect.left;
            point.y = OuterRect.top;
            pDC->MoveTo(point);
            point.x = OuterRect.right;
            pDC->LineTo(point);
            point.y = OuterRect.bottom;
            pDC->LineTo(point);
            point.x = OuterRect.left;
            pDC->LineTo(point);
            point.y = OuterRect.top;
            pDC->LineTo(point);

            unsigned short i = 0;
            double HeadForward = m_HeadForward;

            while (i < m_NumberPaths)
            {
                short CosFor = (short)Round(cos(HeadForward));
                short SinFor = (short)Round(sin(HeadForward));
                short CosNext = (short)Round(cos(m_HeadNext));
                short SinNext = (short)Round(sin(m_HeadNext));
                /*Draw line*/
                POINT BmpPoint[4];

                if (i != 0)
                {
                    BmpPoint[0] = ProbabilityGrid->RobotToDDSurface(m_VacuumPath[i - 1].Arc2e, SurfaceOffset);
                }
                else
                {
                    BmpPoint[0] = ProbabilityGrid->RobotToDDSurface(m_EntryPoint, SurfaceOffset);
                }

                pDC->MoveTo(BmpPoint[0]);
                BmpPoint[0] = ProbabilityGrid->RobotToDDSurface(m_VacuumPath[i].Arc1s, SurfaceOffset);
                pDC->LineTo(BmpPoint[0]);
                /*Draw arrow head*/
                double x1, y1, x2, y2;
                x1 = m_VacuumPath[i].Arc1s.x - 30 * CosFor;
                y1 = m_VacuumPath[i].Arc1s.y - 30 * SinFor;
                RobotPoint.x = x1, RobotPoint.y = y1;
                BmpPoint[0] = ProbabilityGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
                x2 = x1 - CosFor * 30 - CosNext * 30;
                y2 = y1 - SinFor * 30 - SinNext * 30;
                RobotPoint.x = x2, RobotPoint.y = y2;
                BmpPoint[1] = ProbabilityGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
                x2 = x1 - CosFor * 30 + CosNext * 30;
                y2 = y1 - SinFor * 30 + SinNext * 30;
                RobotPoint.x = x2, RobotPoint.y = y2;
                BmpPoint[2] = ProbabilityGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
                BmpPoint[3] = BmpPoint[0];
                CRgn ArrowRegion;
                ArrowRegion.CreatePolygonRgn(BmpPoint, 4, ALTERNATE);
                pDC->FillRgn(&ArrowRegion, &ArrowBrush);

                /*If next vacuum path inside the patch, draw an arc to it*/
                if (i < m_NumberPaths - 1)
                {
                    /*Draw 2 arcs*/
                    /*Arc 1*/
                    CRect ArcRect;
                    short TurnDiameter = ABS(m_VacuumPath[i].TurnDiameter);
                    x1 = m_VacuumPath[i].Arc1s.x + CosFor * TurnDiameter / 2;
                    y1 = m_VacuumPath[i].Arc1s.y + SinFor * TurnDiameter / 2;
                    x2 = m_VacuumPath[i].Arc1s.x - CosFor * TurnDiameter / 2 + CosNext * TurnDiameter;
                    y2 = m_VacuumPath[i].Arc1s.y - SinFor * TurnDiameter / 2 + SinNext * TurnDiameter;
                    RobotPoint.x = x1, RobotPoint.y = y1;
                    point = ProbabilityGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
                    ArcRect.left = point.x, ArcRect.top = point.y;
                    RobotPoint.x = x2, RobotPoint.y = y2;
                    point = ProbabilityGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
                    ArcRect.right = point.x, ArcRect.bottom = point.y;
                    ArcRect.NormalizeRect();

                    if ((SinFor * CosNext == 1) || (CosFor * SinNext == -1))
                    {
                        BmpPoint[0] = ProbabilityGrid->RobotToDDSurface(m_VacuumPath[i].Arc1e, SurfaceOffset);
                        BmpPoint[1] = ProbabilityGrid->RobotToDDSurface(m_VacuumPath[i].Arc1s, SurfaceOffset);
                    }
                    else
                    {
                        BmpPoint[0] = ProbabilityGrid->RobotToDDSurface(m_VacuumPath[i].Arc1s, SurfaceOffset);
                        BmpPoint[1] = ProbabilityGrid->RobotToDDSurface(m_VacuumPath[i].Arc1e, SurfaceOffset);
                    }

                    pDC->Arc(ArcRect, BmpPoint[0], BmpPoint[1]);
                    /*Arc 2*/
                    x1 = m_VacuumPath[i].Arc2c.x + m_VacuumPath[i].Arc2r;
                    y1 = m_VacuumPath[i].Arc2c.y + m_VacuumPath[i].Arc2r;
                    RobotPoint.x = x1, RobotPoint.y = y1;
                    point = ProbabilityGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
                    ArcRect.left = point.x, ArcRect.top = point.y;
                    x1 = m_VacuumPath[i].Arc2c.x - m_VacuumPath[i].Arc2r;
                    y1 = m_VacuumPath[i].Arc2c.y - m_VacuumPath[i].Arc2r;
                    RobotPoint.x = x1, RobotPoint.y = y1;
                    point = ProbabilityGrid->RobotToDDSurface(RobotPoint, SurfaceOffset);
                    ArcRect.right = point.x, ArcRect.bottom = point.y;
                    ArcRect.NormalizeRect();

                    if ((SinFor * CosNext == 1) || (CosFor * SinNext == -1))
                    {
                        BmpPoint[0] = ProbabilityGrid->RobotToDDSurface(m_VacuumPath[i].Arc2s, SurfaceOffset);
                        BmpPoint[1] = ProbabilityGrid->RobotToDDSurface(m_VacuumPath[i].Arc2e, SurfaceOffset);
                    }
                    else
                    {
                        BmpPoint[0] = ProbabilityGrid->RobotToDDSurface(m_VacuumPath[i].Arc2e, SurfaceOffset);
                        BmpPoint[1] = ProbabilityGrid->RobotToDDSurface(m_VacuumPath[i].Arc2s, SurfaceOffset);
                    }

                    pDC->Arc(ArcRect, BmpPoint[0], BmpPoint[1]);
                }

                /*Onto next patch*/
                HeadForward += pi;
                i++;
            }

            pDC->SelectObject(OldPen);
            PathPen.DeleteObject();
            //			BorderBrush.DeleteObject();
            ArrowBrush.DeleteObject();
        }
    }
}

void CVacuumPatch::SetBounds(robRECT PatchBounds)
/*******************************************************************************
//Created: 10/27/97 S.R.
//Last Revision: 10/27/97 S.R.
//Parameters:
//	none.
//Return Value:
//	none.
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    CExecutableRWGraphicObject::SetBounds(PatchBounds);
    SetEntryPointAndHeadings();
    CalculateVacuumPaths();
}

void CVacuumPatch::EditProperties()
/*******************************************************************************
//Created: 10/28/97 S.R.
//Last Revision: 10/28/97 S.R.
//Parameters:
//	none.
//Return Value:
//	none.
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    CDlgCreateVacuumPatch DlgCreateVacuumPatch;
    DlgCreateVacuumPatch.m_EnterSide = m_EnterSide;
    DlgCreateVacuumPatch.SetPathWidth(m_PathWidth);
    DlgCreateVacuumPatch.SetTurnDiameter(m_TurnDiameter);
    DlgCreateVacuumPatch.m_edtName = GetCaption();
    DlgCreateVacuumPatch.m_edtVacuumingSpeed = GetVacuumingSpeed() / 10.0f;

    if (DlgCreateVacuumPatch.DoModal() == IDOK)
    {
        m_EnterSide = DlgCreateVacuumPatch.m_EnterSide;
        m_PathWidth = DlgCreateVacuumPatch.GetPathWidth();
        m_TurnDiameter = DlgCreateVacuumPatch.GetTurnDiameter();
        SetCaption(DlgCreateVacuumPatch.m_edtName);
        SetVacuumingSpeed(short(DlgCreateVacuumPatch.m_edtVacuumingSpeed * 10));
    }

    /*Make sure PatchBounds rectangle is wide enough for robot to go through*/
    BOOL Resized = FALSE;

    if (m_Bounds.top - m_Bounds.bottom < MinHeight())
    {
        m_Bounds.bottom = m_Bounds.top - MinHeight();
        Resized = TRUE;
    }

    if (m_Bounds.right - m_Bounds.left < MinWidth())
    {
        m_Bounds.right = m_Bounds.left + MinWidth();
        Resized = TRUE;
    }

    if (Resized)
    {
        AfxMessageBox("The Vacuum Patch was too narrow, and was resized");
    }

    SetEntryPointAndHeadings();
    CalculateVacuumPaths();
}

CVacuumPatch::~CVacuumPatch()
{
    if (m_VacuumPath != NULL)
    {
        delete m_VacuumPath;
    }
}

void CVacuumPatch::Serialize(CArchive & archive)
{
    CExecutableRWGraphicObject::Serialize(archive);

    if (archive.IsStoring())
    {
        archive << (DWORD)m_EnterSide;
        archive << m_PathWidth;
        archive << m_TurnDiameter;
        archive << m_VacuumLength;
        archive << 	m_VacuumingSpeed;
    }
    else
    {
        DWORD tDword;
        archive >> tDword, m_EnterSide = TEnterSide(tDword);
        archive >> m_PathWidth;
        archive >> m_TurnDiameter;
        archive >> m_VacuumLength;

        if (m_ArchiveVersionNumber >= 6)
        {
            archive >> m_VacuumingSpeed;
        }
        else
        {
            m_VacuumingSpeed = cDefaultVacuumSpeed;
        }

        SetEntryPointAndHeadings();
        CalculateVacuumPaths();
    }
}

void CVacuumPatch::AddStraightLineToPath(robPOINT P1, robPOINT P2, CCTDPath **RobotPath, CRobot* Robot)
{
    if (!(*RobotPath)->AddStraightLine(P1, P2))
    {
        Robot->AddCTDPathToInstructions(*RobotPath, cLrgPositionZone, FALSE, 0);
        *RobotPath = new CCTDPath((*RobotPath)->GetSpeed());
        (*RobotPath)->AddStraightLine(P1, P2);
    }
}

void CVacuumPatch::AddArcToPath(robPOINT P1, robPOINT P2, robPOINT P3, CCTDPath **RobotPath, CRobot *Robot)
{
    if (!(*RobotPath)->AddArc(P1, P2, P3))
    {
        Robot->AddCTDPathToInstructions(*RobotPath, cLrgPositionZone, FALSE, 0);
        *RobotPath = new CCTDPath((*RobotPath)->GetSpeed());
        (*RobotPath)->AddArc(P1, P2, P3);
    }

}

void CVacuumPatch::SetVacuumingSpeed(short Speed)
{
    if (Speed < 50)
    {
        Speed = 50;
    }

    if (Speed > cMaxSpeed)
    {
        Speed = cMaxSpeed;
    }

    m_VacuumingSpeed = Speed;
}