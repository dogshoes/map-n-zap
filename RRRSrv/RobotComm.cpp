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
///////////////////////////////////////////////////////////////////////////////
// Module:     Robot Communication.cpp
// Abstract:   This file includes the member functions for the CRobotComm
//             class.  This object is used to handle the communications with
//             the robot.  It deals with decoding the serial input from the
//             robot, and encoding the data to be sent to the robot.
//
//             This has been modified to work within the RRRSrv structure. The
//             send mechanism was changed to fit the existing architecture
//             without changing the content of the messages.
// Author:     04/16/96 S.R.
// Copyright 1997 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

//#include <afxwin.h>
//#include <math.h>
//#include <assert.h>
//#include <afxdisp.h>
#include "stdafx.h"
#include "rrrsrv.h"
#include "RobotComm.h"

CRobotComm::CRobotComm()
{
    // get address of serial interface class
    CRRRSrvApp *SrvApp = (CRRRSrvApp *)AfxGetApp();
    rcom = SrvApp->m_oRRRCom;
}

CRobotComm::~CRobotComm()
{
}

void CRobotComm::ResetInterface(RRRSrvCom *newrcom)
{
    rcom = newrcom;
}

void CRobotComm::SendPositionDestination(short RobotAddress, robPOINT Destination)
{
    RRRMsg OutputString(RobotAddress, '\000', 9);

    OutputString.SetAt(0, SET_BUNGEE_DEST_COMMAND_CHAR);

    /*Output the X coordinate*/
    char* NumOutput = LongToBytes(WorldToRobotUnits(Destination.x));
    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    /*Output the Y coordinate*/
    NumOutput = LongToBytes(WorldToRobotUnits(Destination.y));
    for (char i = 5; i < 9; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 5]);
    }

    rcom->SendMsg(OutputString);
}

#ifdef IMPLEMENTED
long int* CRobotComm::DecodeSerialInput(CString* InputString, unsigned char* InputCode)
{
    static long int ReturnValue[3];

    if ((InputString != NULL) && (InputString->GetLength() != 0))
    {
        switch (*InputCode = InputString->GetAt(0))
        {
        case cPositionInput:
            if (InputString->GetLength() >= 11)
            {
                ReturnValue[0] = RobotToWorldUnits(BytesToLong(InputString->GetAt(1), InputString->GetAt(2),
                                                   InputString->GetAt(3), InputString->GetAt(4)));
                ReturnValue[1] = RobotToWorldUnits(BytesToLong(InputString->GetAt(5), InputString->GetAt(6),
                                                   InputString->GetAt(7), InputString->GetAt(8)));
                ReturnValue[2] = BytesToInt(InputString->GetAt(9), InputString->GetAt(10));
            }

            break;

        /*One char input*/
        case cNudgeInput:
            ReturnValue[0] = InputString->GetAt(1);
            break;

        /*Two char input*/
        case cObstacleInput:
            ReturnValue[0] = InputString->GetAt(1);
            ReturnValue[1] = InputString->GetAt(2);
            break;

        case cEncodersInput:
            ReturnValue[0] = InputString->GetAt(1);
            ReturnValue[1] = InputString->GetAt(2);
            break;

        /*Two int input*/
        case cVelocityInput:
        case cAccelerationInput:
        case cPwmInput:
            ReturnValue[0] = BytesToInt(InputString->GetAt(1), InputString->GetAt(2));
            ReturnValue[1] = BytesToInt(InputString->GetAt(3), InputString->GetAt(4));
            break;

        /*Three int input*/
        case cAnalogInput:
            ReturnValue[0] = BytesToInt(InputString->GetAt(1), InputString->GetAt(2));
            ReturnValue[1] = BytesToInt(InputString->GetAt(3), InputString->GetAt(4));
            ReturnValue[2] = BytesToInt(InputString->GetAt(5), InputString->GetAt(6));
            break;

        /*One int input*/
        case cRingBufferInput:
            ReturnValue[0] = BytesToInt(InputString->GetAt(1), InputString->GetAt(2));
            break;

        /*No data input*/
        case cSendNewPathSegment:
        case cLowBatteryMessage:
            break;

        default:
            *InputCode = cInvalidInputString;
            break;
        }
    }
    else
    {
        InputCode = cInvalidInputString;
    }

    return ReturnValue;
}
#endif

int CRobotComm::BytesToInt(unsigned char Byte0, unsigned char Byte1)
{
    return (((int)Byte0) | (((int)Byte1) << 8));
}

long int CRobotComm::BytesToLong(unsigned char Byte0, unsigned char Byte1, unsigned char Byte2, unsigned char Byte3)
{
    long int Result = Byte0;
    Result |= ((long int)Byte1) << 8;
    Result |= ((long int)Byte2) << 16;
    Result |= ((long int)Byte3) << 24;
    return Result;
}

char* CRobotComm::LongToBytes(long int X)
{
    static char Result[4];
    Result[0] = (char)X;
    Result[1] = (char)(X >> 8);
    Result[2] = (char)(X >> 16);
    Result[3] = (char)(X >> 24);
    return Result;
}

void CRobotComm::SendMotorsPWM(short RobotAddress, int M0PWM, int M1PWM)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    /*Set the command character*/
    OutputString.SetAt(0, SET_MOTORS_PWM_COMMAND_CHAR);

    /*Set M0's pwm*/
    char* NumOutput = LongToBytes(M0PWM);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);

    /*Set M1's pwm*/
    NumOutput = LongToBytes(M1PWM);
    OutputString.SetAt(3, NumOutput[0]);
    OutputString.SetAt(4, NumOutput[1]);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendStopMotors(short RobotAddress)
{
    RRRMsg OutputString(RobotAddress);

    OutputString.Add(STOP_MOTORS_COMMAND_CHAR);

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendPurgeRingBuffer(short RobotAddress)
{
    RRRMsg OutputString(RobotAddress);

    OutputString.Add(PURGE_RING_BUFFER_COMMAND_CHAR);

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendHeadingDestination(short RobotAddress, double Heading, short Velocity)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    /*Set the command character*/
    OutputString.SetAt(0, SET_HEADING_DEST_COMMAND_CHAR);

    /*Set the heading*/
    char* NumOutput = LongToBytes(RadianToEncoderHeading(Heading));
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);

    NumOutput = LongToBytes(Velocity);
    OutputString.SetAt(3, NumOutput[0]);
    OutputString.SetAt(4, NumOutput[1]);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendMaxPWM(short RobotAddress, unsigned char MaxPWM)
{
    RRRMsg OutputString(RobotAddress, '\000', 2);

    /*Set the command character*/
    OutputString.SetAt(0, SET_MAXPWM_COMMAND_CHAR);

    /*Set M0's pwm*/
    OutputString.SetAt(1, MaxPWM);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendFlipHandle(short RobotAddress, BOOL value)
{
    RRRMsg OutputString(RobotAddress, '\000', 2);

    /*Set the command character*/
    OutputString.SetAt(0, SET_FLIPHANDLE_COMMAND_CHAR);

    /*Set the value*/
    OutputString.SetAt(1, value);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendHeadingCorrectionFactor(short RobotAddress, int Correction)
{
    RRRMsg OutputString(RobotAddress, '\000', 3);

    /*Set the command character*/
    OutputString.SetAt(0, SET_HEADCORRECTFACT_COMMAND_CHAR);

    /*Set the correction bytes*/
    char* NumOutput = LongToBytes(Correction);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendMotorVelocities(short RobotAddress, int M0Velocity, int M1Velocity, short Priority)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    OutputString.SetPriority(Priority);

    /*Set the command character*/
    OutputString.SetAt(0, SET_MOTORS_VELOCITY_COMMAND_CHAR);

    /*Set M0's pwm*/
    char* NumOutput = LongToBytes(M0Velocity);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);

    NumOutput = LongToBytes(M1Velocity);
    OutputString.SetAt(3, NumOutput[0]);
    OutputString.SetAt(4, NumOutput[1]);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendPosition(short RobotAddress, robPOINT Position)
{
    RRRMsg OutputString(RobotAddress, '\000', 9);

    OutputString.SetAt(0, SET_POSITION_COMMAND_CHAR);

    /*Output the X coordinate*/
    char* NumOutput = LongToBytes(WorldToRobotUnits(Position.x));
    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    /*Output the Y coordinate*/
    NumOutput = LongToBytes(WorldToRobotUnits(Position.y));
    for (char i = 5; i < 9; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 5]);
    }

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendHeading(short RobotAddress, double Heading)

{
    RRRMsg OutputString(RobotAddress, '\000', 3);

    /*Set the command character*/
    OutputString.SetAt(0, SET_HEADING_COMMAND_CHAR);

    /*Set the heading*/
    char* NumOutput = LongToBytes(RadianToEncoderHeading(Heading));
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendMaxSpeed(short RobotAddress, short MaxSpeed)
{
    RRRMsg OutputString(RobotAddress, '\000', 3);

    /*Set the command character*/
    OutputString.SetAt(0, SET_MAXSPEED_COMMAND_CHAR);

    /*Set M0's speed*/
    char* NumOutput = LongToBytes(MaxSpeed);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendPositionVelocityDestination(short RobotAddress, robPOINT Destination, short Velocity, short Priority)
{
    RRRMsg OutputString(RobotAddress, '\000', 11);

    OutputString.SetPriority(Priority);
    OutputString.SetAt(0, SET_POSITIONVELOCITY_DESTINATION_COMMAND_CHAR);

    /*Output the X coordinate*/
    char* NumOutput = LongToBytes(WorldToRobotUnits(Destination.x));
    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    /*Output the Y coordinate*/
    NumOutput = LongToBytes(WorldToRobotUnits(Destination.y));
    for (char i = 5; i < 9; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 5]);
    }

    NumOutput = LongToBytes(Velocity);
    OutputString.SetAt(9, NumOutput[0]);
    OutputString.SetAt(10, NumOutput[1]);

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendStraightVelocity(short RobotAddress, int Velocity, double Heading)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    OutputString.SetAt(0, SET_GO_STRAIGHT_VELOCITY);

    char* NumOutput = LongToBytes(Velocity);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);

    NumOutput = LongToBytes(RadianToEncoderHeading(Heading));
    OutputString.SetAt(3, NumOutput[0]);
    OutputString.SetAt(4, NumOutput[1]);

    rcom->SendMsg(OutputString);
}

int CRobotComm::RadianToEncoderHeading(double Heading)
{
    Heading = BringAngleInRange(Heading);
    /*Transform radian heading into a robot heading*/
    Heading = Heading * cNHeadings / (2 * pi);

    if (Heading >= cNHeadings)
    {
        Heading -= cNHeadings;
    }

    if (Heading >= cNHeadings)
    {
        AfxMessageBox("RadianToEncoderHeading:  Heading Out Of Range", MB_OK, 0);
    }

    return Round(Heading);
}

void CRobotComm::SendSensitiveObstacleDetection(short RobotAddress, short Sensitive)
{
    RRRMsg OutputString(RobotAddress, '\000', 2);

    /*Set the command character*/
    OutputString.SetAt(0, SET_SENSITIVE_OBSTACLE_DETECTION);

    /*Set the value*/
    OutputString.SetAt(1, (unsigned char)Sensitive);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendTurn(short RobotAddress, double Heading, short Velocity, short Diameter)
{
    RRRMsg OutputString(RobotAddress, '\000', 7);

    /*Set the command character*/
    OutputString.SetAt(0, SET_TURN_COMMAND_CHAR);

    /*Set the heading*/
    char* NumOutput = LongToBytes(RadianToEncoderHeading(Heading));
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);

    /*Set the velocity*/
    NumOutput = LongToBytes(Velocity);
    OutputString.SetAt(3, NumOutput[0]);
    OutputString.SetAt(4, NumOutput[1]);

    /*Set the diameter*/
    NumOutput = LongToBytes(WorldToRobotUnits((long)Diameter));
    OutputString.SetAt(5, NumOutput[0]);
    OutputString.SetAt(6, NumOutput[1]);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendMotorControlConstants(short RobotAddress, char Kp, char Ki, char Kd, char Kb, char KPwm)
{
    RRRMsg OutputString(RobotAddress, '\000', 6);

    /*Set the command character*/
    OutputString.SetAt(0, SET_MOTION_CONTROL_CONSTANTS_COMMAND_CHAR);

    /*Set the string values*/
    OutputString.SetAt(1, Kp);
    OutputString.SetAt(2, Ki);
    OutputString.SetAt(3, Kd);
    OutputString.SetAt(4, Kb);
    OutputString.SetAt(5, KPwm);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendHandleLength(short RobotAddress, short HandleLength)
{
    RRRMsg OutputString(RobotAddress, '\000', 3);

    /*Set the command character*/
    OutputString.SetAt(0, SET_HANDLE_LENGTH_COMMAND_CHAR);

    /*Set the handle length*/
    HandleLength = (short)WorldToRobotUnits((long)HandleLength);
    char* NumOutput = LongToBytes(HandleLength);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendFollowPath(short RobotAddress, short FollowPath)
{
    RRRMsg OutputString(RobotAddress, '\000', 3);

    /*Set the command character*/
    OutputString.SetAt(0, SET_FOLLOW_PATH_COMMAND_CHAR);

    /*Set the Follow Path*/
    char* NumOutput = LongToBytes(FollowPath);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendPathSegment(short RobotAddress, BOOL PurgePath, robPOINT* Points, short nPoints)
{
    RRRMsg OutputString(RobotAddress, '\000', 8 + 2 * (nPoints - 1) + 3);

    OutputString.SetAt(0, SET_PATH_SEGMENT_COMMAND_CHAR);

    /*Set Purge Path*/
    OutputString.SetAt(1, PurgePath);

    /*Set the number of points*/
    OutputString.SetAt(2, (unsigned char)nPoints);

    if (nPoints != 0)
    {
        /*Output the X0 coordinate*/
        char* NumOutput = LongToBytes(WorldToRobotUnits(Points[0].x));
        for (unsigned char i = 0; i < 4; i++)
        {
            OutputString.SetAt(i + 3, NumOutput[i]);
        }

        /*Output the Y0 coordinate*/
        NumOutput = LongToBytes(WorldToRobotUnits(Points[0].y));
        for (unsigned char i = 0; i < 4; i++)
        {
            OutputString.SetAt(i + 7, NumOutput[i]);
        }

        /*Output the rest of the coordinates*/
        for (unsigned char i = 0; i < nPoints - 1; i++)
        {
            OutputString.SetAt(2 * i + 11, (unsigned char)WorldToRobotUnits(Points[i + 1].x - Points[i].x));
            OutputString.SetAt(2 * i + 1 + 11, (unsigned char)WorldToRobotUnits(Points[i + 1].y - Points[i].y));
        }
    }

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendBuzzerOn(short RobotAddress, BOOL On)
{
    RRRMsg OutputString(RobotAddress, '\000', 2);

    /*Set the command character*/
    OutputString.SetAt(0, SET_BUZZER_ON_COMMAND_CHAR);

    /*Set M0's pwm*/
    OutputString.SetAt(1, On);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

void CRobotComm::SendBuzzerFrequency(short RobotAddress, short BuzzerFrequency)
{
    typedef enum {bf0, bf2000, bf4000, bf6000, bf8000} TBuzzerFrequency;

    RRRMsg OutputString(RobotAddress, '\000', 3);

    /*Set the command character*/
    OutputString.SetAt(0, SET_BUZZER_FREQUENCY_COMMAND_CHAR);

    /*Set Buzzer Frequency*/
    char* NumOutput = LongToBytes(BuzzerFrequency);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);

    /*Send the data*/
    rcom->SendMsg(OutputString);
}

long CRobotComm::WorldToRobotUnits(long WorldUnit)
{
    return long(WorldUnit * cRobotEncoderTicksPerInch / cTPI);
}

long CRobotComm::WorldToRobotUnits(double WorldUnit)
{
    return Round(WorldUnit * cRobotEncoderTicksPerInch / cTPI);
}

double CRobotComm::RobotToWorldUnits(long RobotUnit)
{
    return (RobotUnit * cTPI / cRobotEncoderTicksPerInch);
}

void CRobotComm::SendP1(short RobotAddress, long P1)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    OutputString.SetAt(0, SET_P1_COMMAND_CHAR);

    /*Output P1*/
    char* NumOutput = LongToBytes(P1);
    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendP2(short RobotAddress, long P2)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    OutputString.SetAt(0, SET_P2_COMMAND_CHAR);

    /*Output P2*/
    char* NumOutput = LongToBytes(P2);
    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendP3(short RobotAddress, long P3)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    OutputString.SetAt(0, SET_P3_COMMAND_CHAR);

    /*Output P3*/
    char* NumOutput = LongToBytes(P3);
    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendP4(short RobotAddress, long P4)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    OutputString.SetAt(0, SET_P4_COMMAND_CHAR);

    /*Output P4*/
    char* NumOutput = LongToBytes(P4);
    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendP5(short RobotAddress, long P5)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    OutputString.SetAt(0, SET_P5_COMMAND_CHAR);

    /*Output P5*/
    char* NumOutput = LongToBytes(P5);
    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendP6(short RobotAddress, long P6)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    OutputString.SetAt(0, SET_P6_COMMAND_CHAR);

    /*Output P6*/
    char* NumOutput = LongToBytes(P6);
    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendP7(short RobotAddress, long P7)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    OutputString.SetAt(0, SET_P7_COMMAND_CHAR);

    /*Output P7*/
    char* NumOutput = LongToBytes(P7);
    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendP8(short RobotAddress, long P8)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    OutputString.SetAt(0, SET_P8_COMMAND_CHAR);

    /*Output P8*/
    char* NumOutput = LongToBytes(P8);
    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendVelocityDirection(short RobotAddress, int Velocity, double Heading, short Priority)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    OutputString.SetPriority(Priority);
    OutputString.SetAt(0, SET_VELOCITY_DIRECTION_COMMAND_CHAR);

    char* NumOutput = LongToBytes(Velocity);
    OutputString.SetAt(1, NumOutput[0]);
    OutputString.SetAt(2, NumOutput[1]);

    NumOutput = LongToBytes(RadianToEncoderHeading(Heading));
    OutputString.SetAt(3, NumOutput[0]);
    OutputString.SetAt(4, NumOutput[1]);

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendInformationToTransmit(short RobotAddress, long mask)
{
    RRRMsg OutputString(RobotAddress, '\000', 5);

    OutputString.SetAt(0, SET_INFORMATION_TO_TRANSMIT_COMMAND_CHAR);

    /*Output mask*/
    char* NumOutput = LongToBytes(mask);

    for (char i = 1; i <= 4; i++)
    {
        OutputString.SetAt(i, NumOutput[i - 1]);
    }

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendRequestState(short RobotAddress)
{
    RRRMsg OutputString(RobotAddress, '\000', 1);

    OutputString.SetAt(0, REQUEST_STATE_COMMAND_CHAR);

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendPowerOff(short RobotAddress)
{
    RRRMsg OutputString(RobotAddress, '\000', 1);

    OutputString.SetAt(0, REQUEST_POWER_OFF_CHAR);

    rcom->SendMsg(OutputString);
}

void CRobotComm::SendRawData(short RobotAddress, RRRMsg &Msg)
{
    rcom->SendMsg(Msg);
}

/* New stuff 3/6/99 */

void CRobotComm::SquareToWall(short RobotAddress, double Heading, float TravelTime, BOOL SlipperySurface)
/*Heading = heading to wall*/
/*TravelTime = time to travel forward looking for wall (in seconds)*/
/*SlipperySurface = whether the floor is slippery (wood) or not*/
{
    RRRMsg String(RobotAddress, '\000', 6);

    String.SetAt(0, SEND_SQUARE_TO_WALL_COMMAND_CHAR);

    char* Numbers = LongToBytes(RadianToEncoderHeading(Heading));
    String.SetAt(1, Numbers[0]);
    String.SetAt(2, Numbers[1]);
    String.SetAt(3, SlipperySurface);

    Numbers = LongToBytes(long(TravelTime * 1000 / 50));
    String.SetAt(4, Numbers[0]);
    String.SetAt(5, Numbers[1]);

    rcom->SendMsg(String);
}

void CRobotComm::SetFeelingMotor(short RobotAddress, unsigned char Motor)
{
    RRRMsg String(RobotAddress, 0, 2);

    String.SetAt(0, SEND_FEELING_MOTOR_COMMAND_CHAR);

    String.SetAt(1, Motor);

    rcom->SendMsg(String);
}

void CRobotComm:: CornerCalibrate(short RobotAddress, long X, long Y, double H0, unsigned char FeelWheel, float TravelTime, BOOL SlipperySurface)
/*X,Y = location of center of calibration wall*/
/*H0 = heading of wall*/
/*FeelWheel = wheel to feel corner with*/
/*TravelTime = time to travel parallel to wall to find corner (in seconds)*/
/*SlipperySurface = whether the floor is slippery (wood) or not*/
{
    double P0x, P0y;
    P0x = X - cRRNoseLength * cos(H0);
    P0y = Y - cRRNoseLength * sin(H0);

    P0x = P0x - (cCornerWallBackupDistance) * cos(H0);
    P0y = P0y - (cCornerWallBackupDistance) * sin(H0);

    if (FeelWheel == 0)
    {
        H0 += pi / 2;
    }
    else
    {
        H0 -= pi / 2;
    }

    RRRMsg String(RobotAddress, 0, 15);

    String.SetAt(0, SEND_CORNER_CALIBRATE_COMMAND_CHAR);

    char* Numbers = LongToBytes(WorldToRobotUnits(P0x));
    String.SetAt(1, Numbers[0]);
    String.SetAt(2, Numbers[1]);
    String.SetAt(3, Numbers[2]);
    String.SetAt(4, Numbers[3]);

    Numbers = LongToBytes(WorldToRobotUnits(P0y));
    String.SetAt(5, Numbers[0]);
    String.SetAt(6, Numbers[1]);
    String.SetAt(7, Numbers[2]);
    String.SetAt(8, Numbers[3]);

    Numbers = LongToBytes(RadianToEncoderHeading(H0));
    String.SetAt(9, Numbers[0]);
    String.SetAt(10, Numbers[1]);
    String.SetAt(11, FeelWheel);
    String.SetAt(12, SlipperySurface);

    Numbers = LongToBytes(long(TravelTime * 1000 / 50));
    String.SetAt(13, Numbers[0]);
    String.SetAt(14, Numbers[1]);

    rcom->SendMsg(String);
}

void CRobotComm::FindHomeBase(short RobotAddress, double CornerHeading, double WallHeading, long HomeBaseLocationX, long HomeBaseLocationY, BOOL NeedToGetInFrontOfHomeBase)
{
    double P1x, P1y;
    P1x = HomeBaseLocationX;
    P1y = HomeBaseLocationY;

    if (NeedToGetInFrontOfHomeBase)
    {
        P1x -= (cRRWidth + 6 * cTPI) * cos(WallHeading);
        P1x -= 0.5 * cTPI * cos(CornerHeading);
        P1y -= (cRRWidth + 6 * cTPI) * sin(WallHeading);
        P1y -= 0.5 * cTPI * sin(CornerHeading);
    }
    else
    {
        P1x -= (6 * cTPI) * cos(WallHeading);
        P1y -= (6 * cTPI) * sin(WallHeading);
    }

    RRRMsg String(RobotAddress, 0, 12);

    String.SetAt(0, SEND_FIND_HOME_BASE_COMMAND_CHAR);

    char* Numbers = LongToBytes(WorldToRobotUnits(P1x));
    String.SetAt(1, Numbers[0]);
    String.SetAt(2, Numbers[1]);
    String.SetAt(3, Numbers[2]);
    String.SetAt(4, Numbers[3]);

    Numbers = LongToBytes(WorldToRobotUnits(P1y));
    String.SetAt(5, Numbers[0]);
    String.SetAt(6, Numbers[1]);
    String.SetAt(7, Numbers[2]);
    String.SetAt(8, Numbers[3]);

    Numbers = LongToBytes(RadianToEncoderHeading(WallHeading));
    String.SetAt(9, Numbers[0]);
    String.SetAt(10, Numbers[1]);

    String.SetAt(11, NeedToGetInFrontOfHomeBase);

    rcom->SendMsg(String);
}

void CRobotComm::Pause(short RobotAddress)
{
    RRRMsg String(RobotAddress, 0, 2);

    String.SetAt(0, SEND_PAUSE_COMMAND_CHAR);
    String.SetAt(1, 1);

    rcom->SendMsg(String);
}

void CRobotComm::Resume(short RobotAddress)
{
    RRRMsg String(RobotAddress, 0, 2);

    String.SetAt(0, SEND_PAUSE_COMMAND_CHAR);
    String.SetAt(1, 0);

    rcom->SendMsg(String);
}

void CRobotComm::SetHeadingDestination(short RobotAddress, double Heading, short Speed, short MotorEnable)
{
    RRRMsg String(RobotAddress, 0, 6);

    String.SetAt(0, SET_HEADING_DESTINATION_EXTENDED);

    char* Numbers = LongToBytes(RadianToEncoderHeading(Heading));
    String.SetAt(1, Numbers[0]);
    String.SetAt(2, Numbers[1]);

    if (Speed < 0)
    {
        Speed = -Speed;
    }

    Numbers = LongToBytes(Speed);
    String.SetAt(3, Numbers[0]);
    String.SetAt(4, Numbers[1]);

    String.SetAt(5, (unsigned char) MotorEnable);

    rcom->SendMsg(String);
}

void CRobotComm::SetEnableObstacleBounce(short RobotAddress, BOOL Enable)
{
    RRRMsg String(RobotAddress, 0, 2);

    String.SetAt(0, SET_ENABLE_OBSTACLE_BOUNCE_COMMAND_CHAR);
    String.SetAt(1, Enable);

    rcom->SendMsg(String);
}

void CRobotComm::SetRAMData(short RobotAddress, unsigned short Address, unsigned char NumberBytes, unsigned char *Data)
{
    ASSERT(NumberBytes < 15);

    RRRMsg String(RobotAddress, 0, NumberBytes + 4);

    String.SetAt(0, SET_RAM_DATA_COMMAND_CHAR);
    String.SetAt(1, NumberBytes);

    char* Numbers = LongToBytes(Address);
    String.SetAt(2, Numbers[0]);
    String.SetAt(3, Numbers[1]);

    for (unsigned char i = 0; i < NumberBytes; i++)
    {
        String.SetAt(i + 4, Data[i]);
    }

    rcom->SendMsg(String);
}

void CRobotComm::SetRobotOnSeconds(short RobotAddress, long Seconds)
{
    RRRMsg String(RobotAddress, 0, 5);

    String.SetAt(0, SET_ROBOT_ON_SECONDS_COMMAND_CHAR);

    char* Numbers = LongToBytes(long(Seconds / 0.953f));
    String.SetAt(1, Numbers[0]);
    String.SetAt(2, Numbers[1]);
    String.SetAt(3, Numbers[2]);
    String.SetAt(4, Numbers[3]);

    rcom->SendMsg(String);
}

void CRobotComm::SetIntermittentBeep(short RobotAddress, BOOL On)
{
    RRRMsg String(RobotAddress, 0, 2);

    String.SetAt(0, SET_INTERMITTENT_BEEP_ON_COMMAND_CHAR);
    String.SetAt(1, On);

    rcom->SendMsg(String);
}

void CRobotComm::AddToPosition(short RobotAddress, double DeltaX, double DeltaY)
{
    long X = WorldToRobotUnits(DeltaX);
    long Y = WorldToRobotUnits(DeltaY);

    RRRMsg String(RobotAddress, 0, 9);

    String.SetAt(0, SET_POSITION_DELTA_COMMAND_CHAR);

    char* Numbers = LongToBytes(X);
    String.SetAt(1, Numbers[0]);
    String.SetAt(2, Numbers[1]);
    String.SetAt(3, Numbers[2]);
    String.SetAt(4, Numbers[3]);

    Numbers = LongToBytes(Y);
    String.SetAt(5, Numbers[0]);
    String.SetAt(6, Numbers[1]);
    String.SetAt(7, Numbers[2]);
    String.SetAt(8, Numbers[3]);

    rcom->SendMsg(String);
}

void CRobotComm::AddToHeading(short RobotAddress, double Delta)
{
    int Heading = RadianToEncoderHeading(Delta);
    char* Numbers = LongToBytes(Heading);

    RRRMsg String(RobotAddress, 0, 3);

    String.SetAt(0, SET_HEADING_DELTA_COMMAND_CHAR);
    //	Numbers = LongToBytes(Delta);
    String.SetAt(1, Numbers[0]);
    String.SetAt(2, Numbers[1]);

    rcom->SendMsg(String);
}

void CRobotComm::SetExternalOutput(short RobotAddress, BOOL On)
{
    RRRMsg String(RobotAddress, 0, 2);

    String.SetAt(0, SET_EX1_COMMAND_CHAR);
    String.SetAt(1, On);

    rcom->SendMsg(String);
}

void CRobotComm::RequestEX2Value(short RobotAddress)
{
    RRRMsg String(RobotAddress, 0, 1);

    String.SetAt(0, GET_EX2_COMMAND_CHAR);

    rcom->SendMsg(String);
}