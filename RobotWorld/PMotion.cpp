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

// PMotion.cpp : implementation file
//
//created by Marc Bohlen
//December 1999
//Last Revision: December 1999
//under construction...
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "robotworld.h"
#include "PMotion.h"
#include "Robot.h"
#include "CyePersona.h"
#include <mmsystem.h>

/////////////////////////////////////////////////////////////////////////////
// CPMotion dialog

void CPMotion::SetRobot(CRobot *Robot)
{
    m_Robot = Robot;
}

CPMotion::CPMotion(CWnd* pParent /*=NULL*/)
    : CDialog(CPMotion::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPMotion)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CPMotion::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPMotion)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPMotion, CDialog)
    //{{AFX_MSG_MAP(CPMotion)
    //	ON_BN_CLICKED(IDC_Test, OnTest)
    ON_BN_CLICKED(IDC_Snake, OnSnake)
    ON_BN_CLICKED(IDC_Meander, OnMeander)
    ON_BN_CLICKED(IDC_LeftRight, OnLeftRight)
    ON_BN_CLICKED(IDC_ZigZag, OnZigZag)
    ON_BN_CLICKED(IDC_Abrupt, OnAbrupt)
    ON_BN_CLICKED(IDC_Ramped, OnRamped)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


void CPMotion::Calibrate(double Heading, long X, long Y)
{
    //OK

    robPOINT PresentPoint;
    TTaskResult TaskResult;
    TFloorType FloorType;
    PresentPoint.x = X;
    PresentPoint.y = Y;
    FloorType = m_Robot->GetFloorType();

    m_Robot->AddSquareToWallToInstructions(Heading, FloorType == ftWood);

    if ((TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete)
    {
        return;
    }

    m_Robot->SetHeading(Heading);
    m_Robot->SetPosition(PresentPoint);
    m_Robot->SetObstacleSensitivity(osMediumSensitivity);	//medium Sensitivity
}


void CPMotion::Smooth(short Velocity, short Duration)
{
    //OK
    RampUp(Velocity);
    SWait(Duration);
    RampDown(Velocity);
}


void CPMotion::Staccato(short Velocity, short Duration, short Iterations)
{
    //do only 3 interations to demo
    for (int i = 1; i < Iterations; i++)
    {
        RampUp(Velocity);
        RampDown(Velocity);
        SWait(Duration);
    }
}


void CPMotion::Snake(short V1, short V2, double Angle1, double Angle2, short Iterations)
{
    //OK

    double OriginalHeading = m_Robot->GetHeading();
    TwoWheelSpinn(V1, V2, Angle1, 1, 0, 0);

    for (int i = 1; i < Iterations; i++)
    {
        TwoWheelSpinn(V2, V1, -Angle2, 1, 0, 0);
        TwoWheelSpinn(V1, V2, Angle2, 1, 0, 0);
    }

    TwoWheelSpinn(V2, V1, -Angle1, 1, 0, 1);

    m_Robot->SetHeadingDestination(OriginalHeading, V1, 2);
}


void CPMotion::Meander(short Velocity, short Duration, short Iterations)
{
    //OK
    double OriginalHeading = m_Robot->GetHeading();
    MoveForward(Velocity, Duration);
    TurnLeft(Velocity);

    for (int i = 1; i < Iterations; i++)
    {
        MoveForward(Velocity, Duration);
        TurnRight(Velocity);
        MoveForward(Velocity, Duration);
        TurnRight(Velocity);
        MoveForward(Velocity, Duration);
        TurnLeft(Velocity);
        MoveForward(Velocity, Duration);
        TurnLeft(Velocity);
    }

    m_Robot->SetHeadingDestination(OriginalHeading, Velocity, 2);
}

void CPMotion::RampUp(short Velocity)
{
    //OK
    int StartV = 15;
    double Multiplier = 1.2;
    int DelayT = 50;
    m_Robot->SetMaxSpeed(360);

    while (Velocity > StartV)
    {
        m_Robot->SetVelocities(StartV, StartV);
        StartV = Multiplier * StartV;
        DelayT = int(DelayT / Multiplier);
        Sleep(DelayT);
    }

    m_Robot->SetVelocities(Velocity, Velocity);

}

void CPMotion::RampDown(short Velocity)
{
    //OK
    int StartV = 15;
    double Multiplier = 1.2;
    int DelayT = 50;
    m_Robot->SetMaxSpeed(360);

    while (Velocity >= (2 * StartV))
    {
        m_Robot->SetVelocities(Velocity, Velocity);
        Sleep(DelayT);
        Velocity = int(Velocity / Multiplier);
        DelayT = DelayT * Multiplier;
    }

    m_Robot->Stop();
}


void CPMotion::MoveForward(short Velocity, short Duration)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    m_Robot->AddVelocitiesToInstructions(Velocity, Velocity, (Duration * 1000));
    m_Robot->FollowInstructions(true, true);

}

void CPMotion::MoveForwardNS(short Velocity, short Duration)
{
    //?
    /*m_Robot->SetMaxSpeed(360);
    m_Robot->SetVelocities(Velocity, Velocity);
    SWait(Duration);
    */
    m_Robot->SetMaxSpeed(360);
    m_Robot->AddVelocitiesToInstructions(Velocity, Velocity, (Duration * 1000));
    m_Robot->FollowInstructions(true, false);
}

void CPMotion::ContForward(short Velocity)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    m_Robot->AddVelocitiesToInstructions(Velocity, Velocity, 0);
    m_Robot->FollowInstructions(true, true);
}


void CPMotion::ContReverse(short Velocity)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    m_Robot->AddVelocitiesToInstructions(-Velocity, -Velocity, 0);
    m_Robot->FollowInstructions(true, true);
}


void CPMotion::MoveBackwards(short Velocity, short Duration)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    m_Robot->AddVelocitiesToInstructions(-Velocity, -Velocity, (Duration * 1000));
    m_Robot->FollowInstructions(true, true);

}

void CPMotion::LeftRight(short Velocity, short Delay, short Iterations)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    double OriginalHeading = m_Robot->GetHeading();

    for (int i = 1; i < Iterations; i++)
    {
        m_Robot->AddVelocitiesToInstructions(Velocity, 0, 1000);
        m_Robot->AddVelocitiesToInstructions(0, Velocity, 1000);

    }

    m_Robot->FollowInstructions();

    m_Robot->SetHeadingDestination(OriginalHeading, Velocity, 2);
    SWait(Delay);
    m_Robot->Stop();

}

void CPMotion::ZigZag(short Velocity, short Delay, short Iterations)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    double OriginalHeading = m_Robot->GetHeading();
    double Angle1 =  pi / 12;
    double Angle2 = -pi / 6;
    double Angle3 = pi / 6;

    TurnToAngle(Velocity, OriginalHeading, Angle1);
    SWait(1);

    for (int i = 1; i < Iterations; i++)
    {
        MoveForwardNS(Velocity, 2);
        TurnToAngle(Velocity, OriginalHeading, Angle2);
        MoveForwardNS(Velocity, 2);
        TurnToAngle(Velocity, OriginalHeading, Angle3);
    }

    MoveForward(Velocity, 1);
    m_Robot->SetHeadingDestination(OriginalHeading, Velocity, 2);
    SWait(Delay);
    m_Robot->Stop();

}

void CPMotion::TurnLeft(short Velocity)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    double FoundHeading = m_Robot->GetHeading();
    double NewHeading = FoundHeading + (pi / 2);

    if (NewHeading > 2 * pi)
    {
        NewHeading = NewHeading - 2 * pi;
    }

    if (NewHeading < 0)
    {
        NewHeading = NewHeading + 2 * pi;
    }

    m_Robot->SetHeadingDestination(NewHeading, Velocity, 2);

    while (fabs(FoundHeading - NewHeading) > 0.1)
    {
        m_Robot->DoWindowMessages();
        FoundHeading = m_Robot->GetHeading();
    }

    m_Robot->SetHeadingDestination(NewHeading, Velocity, 2);

}

void CPMotion::TurnRight(short Velocity)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    double FoundHeading = m_Robot->GetHeading();
    double NewHeading = FoundHeading - (pi / 2);

    if (NewHeading > 2 * pi)
    {
        NewHeading = NewHeading - 2 * pi;
    }

    if (NewHeading < 0)
    {
        NewHeading = NewHeading + 2 * pi;
    }

    m_Robot->SetHeadingDestination(NewHeading, Velocity, 2);

    while (fabs(FoundHeading - NewHeading) > 0.1)
    {
        m_Robot->DoWindowMessages();
        FoundHeading = m_Robot->GetHeading();
    }

    m_Robot->SetHeadingDestination(NewHeading, Velocity, 2);
}


void CPMotion::TurnAround(short Velocity)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    double FoundHeading = m_Robot->GetHeading();
    double NewHeading = FoundHeading + pi;

    if (NewHeading > 2 * pi)
    {
        NewHeading = NewHeading - 2 * pi;
    }

    m_Robot->SetHeadingDestination(NewHeading, Velocity, 2);

    while (fabs(FoundHeading - NewHeading) > 0.2)			//was 0.1
    {
        m_Robot->DoWindowMessages();
        FoundHeading = m_Robot->GetHeading();
    }

    m_Robot->SetHeadingDestination(NewHeading, Velocity, 2);
}


void CPMotion::TinyTweek(short Velocity, bool Direction)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    double NewHeading;
    double FoundHeading = m_Robot->GetHeading();

    if (Direction)
    {
        NewHeading = FoundHeading + (pi / 16);
    }
    else
    {
        NewHeading = FoundHeading - (pi / 16);
    }

    m_Robot->SetHeadingDestination(NewHeading, Velocity, 0);


    while (fabs(FoundHeading - NewHeading) > 0.1)
    {
        m_Robot->DoWindowMessages();
        FoundHeading = m_Robot->GetHeading();
    }
}


void CPMotion::TweekLeft(short Velocity)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    double FoundHeading = m_Robot->GetHeading();
    double NewHeading = FoundHeading + (pi / 8);

    m_Robot->SetHeadingDestination(NewHeading, Velocity, 0);

    while (fabs(FoundHeading - NewHeading) > 0.1)
    {
        m_Robot->DoWindowMessages();
        FoundHeading = m_Robot->GetHeading();
    }
}


void CPMotion::TweekRight(short Velocity)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    double FoundHeading = m_Robot->GetHeading();
    double NewHeading = FoundHeading - (pi / 8);

    m_Robot->SetHeadingDestination(NewHeading, Velocity, 1);

    while (fabs(FoundHeading - NewHeading) > 0.1)
    {
        m_Robot->DoWindowMessages();
        FoundHeading = m_Robot->GetHeading();
    }
}


void CPMotion::TurnToAngle(short Velocity, double RefAngle, double SetHeading)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    double FoundHeading = m_Robot->GetHeading();
    double NewHeading = RefAngle + SetHeading;

    if (NewHeading < 0)
    {
        NewHeading = NewHeading + 2 * pi;
    }

    m_Robot->SetHeadingDestination(NewHeading, Velocity, 2);

    while (fabs(FoundHeading - NewHeading) > 0.1)
    {
        m_Robot->DoWindowMessages();
        FoundHeading = m_Robot->GetHeading();
    }

    m_Robot->SetHeadingDestination(NewHeading, Velocity, 2);
}


void CPMotion::SlowTurns(short Side, double Angle, short Velocity)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    double FoundHeading = m_Robot->GetHeading();
    double NewHeading = FoundHeading + Angle;

    if (NewHeading > 2 * pi)
    {
        NewHeading = NewHeading - 2 * pi;
    }

    m_Robot->SetHeadingDestination(NewHeading, Velocity, Side);

    while (fabs(FoundHeading - NewHeading) > 0.2)		//was 0.1
    {
        m_Robot->DoWindowMessages();
        FoundHeading = m_Robot->GetHeading();
    }
}


void CPMotion::MoveToWall(short Velocity, double OriginalHeading)
{
    //move straight untill cye hits an obstacle
    //OK
    m_Robot->SetMaxSpeed(360);
    m_Robot->SetObstacleSensitivity(osMediumSensitivity);
    ContForward(Velocity);
}


void CPMotion::OneWheelSpinn(short Velocity, short Wheel, short Duration, bool Direction, bool Stop)
{
    //OK for now
    m_Robot->SetMaxSpeed(360);
    double FoundHeading = m_Robot->GetHeading();
    double NewHeading = FoundHeading;

    if (Wheel == 1)			//R
    {
        if (Direction == 1)
        {
            m_Robot->SetVelocities(Velocity, 0);
        }
        else
        {
            m_Robot->SetVelocities(-Velocity, 0);
        }
    }
    else
    {
        if (Direction == 1)
        {
            m_Robot->SetVelocities(0, Velocity);
        }
        else
        {
            m_Robot->SetVelocities(0, -Velocity);
        }
    }

    SWait(Duration);
    FoundHeading = m_Robot->GetHeading();

    while (fabs(FoundHeading - NewHeading) > 0.25)			//was 0.2
    {
        m_Robot->DoWindowMessages();
        FoundHeading = m_Robot->GetHeading();
    }

    if (Stop)
    {
        m_Robot->Stop();
    }
}


void CPMotion::TwoWheelSpinn(short RVelocity, short LVelocity, double TurnAngle, bool Direction, bool Opposite, bool Stop)
{
    //OK for now
    m_Robot->SetMaxSpeed(360);
    double FoundHeading = m_Robot->GetHeading();
    double NewHeading = FoundHeading + TurnAngle;

    if (NewHeading > 2 * pi)
    {
        NewHeading = NewHeading - 2 * pi;
    }

    if (Direction == 1)			//R
    {
        if (Opposite == 1)
        {
            m_Robot->SetVelocities(RVelocity, -LVelocity);
        }

        else
        {
            m_Robot->SetVelocities(RVelocity, LVelocity);
        }

    }
    else
    {
        if (Opposite == 1)
        {
            m_Robot->SetVelocities(-RVelocity, LVelocity);
        }

        else
        {
            m_Robot->SetVelocities(RVelocity, LVelocity);
        }

    }

    //m_Robot->FollowInstructions(true, false);			//do not stop here

    while ((fabs(FoundHeading - NewHeading)) > 0.2)		//0.1
    {
        m_Robot->DoWindowMessages();
        FoundHeading = m_Robot->GetHeading();
    }

    if (Stop)
    {
        m_Robot->Stop();
    }
}


void CPMotion::Spinn360()
{
    //for testing purposes only
    m_Robot->SetMaxSpeed(360);
    double FoundHeading = m_Robot->GetHeading();
    double NewHeading = FoundHeading;

    m_Robot->SetVelocities(50, -50);	//R

    SWait(4);
    FoundHeading = m_Robot->GetHeading();

    while (fabs(FoundHeading - NewHeading) > 0.2)
    {
        m_Robot->DoWindowMessages();
        FoundHeading = m_Robot->GetHeading();
    }

    m_Robot->Stop();

}

void CPMotion::Spinn(short Velocity, short Duration, bool Direction)
{
    //overshoots on fast spinns; use correction...later...
    m_Robot->SetMaxSpeed(360);
    double limit;
    double FoundHeading = m_Robot->GetHeading();
    double Correction;

    if (Velocity  <= 100)
    {
        Correction = pi / 4;
        limit = 0.1;
    }

    else if (Velocity  = 200)
    {
        Correction = pi / 3;
        limit = 0.3;
    }

    else if (Velocity  > 200)
    {
        Correction = pi / 3;
        limit = 0.4;
    }

    double NewHeading = FoundHeading - Correction;

    if (Direction == 1)
    {
        m_Robot->SetVelocities(Velocity, -Velocity);    //R
    }
    else
    {
        m_Robot->SetVelocities(-Velocity, Velocity);    //L
    }

    SWait(Duration);
    FoundHeading = m_Robot->GetHeading();

    while (fabs(FoundHeading - NewHeading) > limit)
    {
        m_Robot->DoWindowMessages();
        FoundHeading = m_Robot->GetHeading();
    }

    m_Robot->Stop();

}

void CPMotion::AbsurdMotion(short Ki, short Kd)
{
    //OK
    m_Robot->SetMaxSpeed(360);
    //set unstable values:
    //Ki = 100             'strong jitter !
    //Kd = 80              'wildly erratic !

    double OriginalHeading = m_Robot->GetHeading();
    robPOINT StartPoint, Destination;
    StartPoint = m_Robot->GetPosition();
    Destination = StartPoint;			//going nowhere
    short Kp, Kb, Kpwm;
    int Velocity = 300;

    //default parameters for MotorControlConstants:
    Kp = 3;
    Kb = 8;
    Kpwm = 0;
    //Kd = 5 ;							//disabled to create absurd motion
    //Ki = 6;

    m_Robot->SetMotorControlConstants(Kp, Ki, Kd, Kb, Kpwm);
    m_Robot->SetMotorControlConstants(Kp, Ki, Kd, Kb, Kpwm);
    m_Robot->AddPositionVelocityToInstructions(Destination, NULL, cLrgPositionZone, 300);
    m_Robot->FollowInstructions(true, false);

    SWait(5);

    //reset:
    m_Robot->Stop();
    ResetMotorConstants();

}

void CPMotion::RandMotion()
{
    int limit = m_cyepersona->GetRandomNumber(3);

    for (int i = 1; i <= limit; i++)
    {
        TurnLeft(200);
    }

    limit = m_cyepersona->GetRandomNumber(3);

    for (int i = 1; i <= limit; i++)
    {
        TurnRight(200);
    }
}

void CPMotion::ResetMotorConstants()
{
    //OK
    short Kp, Ki, Kd, Kb, Kpwm;
    //default parameters for MotorControlConstants:
    Kp = 3;
    Kb = 8;
    Kpwm = 0;
    Kd = 5;
    Ki = 6;
    m_Robot->SetMotorControlConstants(Kp, Ki, Kd, Kb, Kpwm);
}



void CPMotion::SWait(short Seconds)				//waits n seconds
{
    //OK
    CTime WaitUntilTime = CTime::GetCurrentTime();
    WaitUntilTime += CTimeSpan(0, 0, 0, Seconds);
    bool done = false;

    while (!done)
    {
        m_Robot->DoWindowMessages();
        CTime StartTime = CTime::GetCurrentTime();
        CTimeSpan TimeDiff = WaitUntilTime - StartTime;

        if ((TimeDiff.GetHours() == 0) && (TimeDiff.GetMinutes() == 0) &&
                (TimeDiff.GetSeconds() <= 0))
        {
            done = true;
        }
    }
}

bool CPMotion::RWait(short Seconds)				//waits n seconds or exit on nudge
{
    //OK
    CTime WaitUntilTime = CTime::GetCurrentTime();
    WaitUntilTime += CTimeSpan(0, 0, 0, Seconds);
    bool done = false;

    while ((!done) && (!(m_Robot->m_Nudged)))
    {
        m_Robot->DoWindowMessages();
        CTime StartTime = CTime::GetCurrentTime();
        CTimeSpan TimeDiff = WaitUntilTime - StartTime;

        if ((TimeDiff.GetHours() == 0) && (TimeDiff.GetMinutes() == 0) &&
                (TimeDiff.GetSeconds() <= 0))
        {
            done = true;
        }
    }

    return (done || (m_Robot->m_Nudged));	// ||boolean
}


void CPMotion::Pause(clock_t wait)
{
    //SWait for n milliseconds
    clock_t goal;
    goal = wait + clock();

    while (goal > clock())
        ;
}

void CPMotion::IPause(int wait)
{
    int Start, End;

    Start = int(timeGetTime());
    End =	Start + wait;

    while (End > int(timeGetTime()))
        ;

    //AfxMessageBox("ISWait done");

}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
// CPMotion message handlers

void CPMotion::OnTweekLeft()
{
    short Velocity = 100;
    TweekLeft(Velocity);
}

void CPMotion::OnTweekRight()
{
    short Velocity = 100;
    TweekRight(Velocity);
}

void CPMotion::OnForwardNS()
{
    short Velocity = 100;
    ContForward(Velocity);
}

void CPMotion::OnReverseNS()
{
    short Velocity = 100;
    ContReverse(Velocity);
}

void CPMotion::OnTurnLeft()
{
    short Velocity = 100;
    TurnLeft(Velocity);
}

void CPMotion::OnTurnRight()
{
    short Velocity = 100;
    TurnRight(Velocity);
}

void CPMotion::OnTurnAround()
{
    short Velocity = 100;
    TurnAround(Velocity);
}

void CPMotion::OnSnake()
{
    short V1 = 150;
    short V2 = 30;
    double Angle1 = pi / 8;
    double Angle2 = pi / 4;
    short Iterations = 3;
    Snake(V1, V2, Angle1, Angle2, Iterations);
}

void CPMotion::OnMeander()
{
    short Velocity = 100;
    short Duration = 2;			//seconds
    short Iterations = 3;
    Meander(Velocity, Duration, Iterations);
}

void CPMotion::OnLeftRight()
{
    short Velocity = 100;
    short Duration = 1;			//seconds
    short Iterations = 8;
    LeftRight(Velocity, Duration, Iterations);
}

void CPMotion::OnZigZag()
{
    short Velocity = 100;
    short Duration = 1;			//seconds
    short Iterations = 3;
    ZigZag(Velocity, Duration, Iterations);

}

void CPMotion::OnRamped()
{
    short Velocity = 300;
    short Duration = 4;			//seconds
    Smooth(Velocity, Duration);
}

void CPMotion::OnAbrupt()
{
    short Velocity = 300;
    short Duration = 4;			//seconds
    MoveForward(Velocity, Duration);

}

void CPMotion::OnCalibrate()
{
    //sets robot at 0,0 pointing north (90deg)
    double Heading = pi / 2;
    long X = 0;
    long Y = 0;
    Calibrate(Heading, X, Y);
}

void CPMotion::OnTest()
{
    //works
    /*
    m_Robot->SetVelocities(200,200);
    SWait(1000);		//wait 1 sec
    m_Robot->Stop();
    */

    //test SWaits: SWait more precise than SWait

    /*for (int i = 1; i<11; i++)
    {
    	TurnLeft(100);
    	//SWait(1);
    	SWait(1000);
    	TurnRight(100);
    	//SWait(1);
    	SWait(1000);
    }*/
    //works
    //MoveBackwards(100,3);
}
///////////////////////////////////////////////////////////////
