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
// Module:     RobotComm.h
// Abstract:   CRobotComm class header file.
// Author:     S.R.
// Copyright 1997 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RothMath.h"
#include "rrdim.h"
#include "RRRConst.h"

class RRRSrvCom;                       // forward declaration for serial interface class

#define cCornerWallBackupDistance cRRWidth/2 + 55 + (10.0f/cRobotEncoderTicksPerInch*cTPI) - cRRNoseLength // dummy value!!!! constant is missing

class CRobotComm : public CObject
{
    public:
        CRobotComm();
        ~CRobotComm();

        /// <summary>
        /// Reset and provide a new communication implementation to talk to the robot over.  Used when changing the COM port the radio pod is connected to.
        /// </summary>
        /// <param name="newrcom">The new communication implementation.</param>
        void ResetInterface(RRRSrvCom *newrcom);

        /// <summary>
        /// This function sends a connect-the-dots path segment to the robot.
        /// </summary>
        /// <remarks>
        /// The function transforms the points from world to robot coordinates.  It sends the path segment down as a segment origin point then the distance between successive points.  This saves download time.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="PurgePath">TRUE if the robot should purge its previous path.</param>
        /// <param name="Points">Points to add to the robot's connect-the-dots path.</param>
        /// <param name="nPoints">Number of points in Points array.</param>
        void SendPathSegment(short RobotAddress, BOOL PurgePath, robPOINT* Points, short nPoints);

        /// <summary>
        /// This function sends a command to the robot to start or stop following its stored connect-the-dots path.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="FollowPath">A value (1 or 0) indicating whether the robot should follow a path.</param>
        void SendFollowPath(short RobotAddress, short FollowPath);

        /// <summary>
        /// This function transforms HandleLength into robot units then sends it to the robot.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="HandleLength">The new robot's handle length in world units.</param>
        void SendHandleLength(short RobotAddress, short HandleLength);

        /// <summary>
        /// This function sends the maximum speed the robot can use.
        /// </summary>
        /// <remarks>
        /// The function creates a string with the SET_MAXSPEED_COMMAND_CHAR and MaxSpeed.  The string is sent through the COM port by the rcom->SendMsg function.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="MaxSpeed">The max speed the robot can use.</param>
        void SendMaxSpeed(short RobotAddress, short MaxSpeed);

        /// <summary>
        /// This function sends the robot's correct heading.
        /// </summary>
        /// <remarks>
        /// The function creates a string with the SET_HEADING_COMMAND_CHAR and Heading.  The string is sent through the COM port by the rcom->SendMsg function.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Heading">The robot's correct heading</param>
        void SendHeading(short RobotAddress, double Heading);

        /// <summary>
        /// This function sends the robot's correct position.
        /// </summary>
        /// <remarks>
        /// The function creates a string with the SET_POSITION_COMMAND_CHAR and Position.  Position is transformed from world to robot coordinates.  The string is sent through the COM port by the rcom->SendMsg function.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Position">The robot's correct position.</param>
        void SendPosition(short RobotAddress, robPOINT Position);

        /// <summary>
        /// This function sets the velocities the robot's motors should go.
        /// </summary>
        /// <remarks>
        /// The function creates a string with the SET_MOTORS_VELOCITY_COMMAND_CHAR and M0Velocity and M1Velocity.  The string is sent through the COM port by the rcom->SendMsg function.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="M0Velocity">The velocities the motor 0 should go.</param>
        /// <param name="M1Velocity">The velocities the motor 1 should go.</param>
        /// <param name="Priority">The priority of this request.  LOW_PRIORITY or HIGH_PRIORITY.
        void SendMotorVelocities(short RobotAddress, int M0Velocity, int M1Velocity, short Priority);

        /// <summary>
        /// The function sends the heading correction factor to the robot.
        /// </summary>
        /// <remarks>
        /// The function creates a string with the SET_HEADCORRECTFACT_COMMAND_CHAR and Correction.  The string is sent through the COM port by the rcom->SendMsg function.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Correction">The heading correction factor.</param>
        void SendHeadingCorrectionFactor(short RobotAddress, int Correction);

        /// <summary>
        /// This function sends the flip handle value to the robot.
        /// </summary>
        /// <remarks>
        /// The function creates a string with the SET_FLIPHANDLE_COMMAND_CHAR and value.  The string is sent through the COM port by the rcom->SendMsg function.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="value">The value to set FlipHandle to.</param>
        void SendFlipHandle(short RobotAddress, BOOL value);

        /// <summary>
        /// This function sends the maximum pwm the robot can use.
        /// </summary>
        /// <remarks>
        /// The function creates a string with the SET_MAXPWM_COMMAND_CHAR and MaxPWM.  The string is sent through the COM port by the rcom->SendMsg function.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="MaxPWM">The max pwm the robot can use.</param>
        void SendMaxPWM(short RobotAddress, unsigned char MaxPWM);

        /// <summary>
        /// This function sends a heading for the robot to turn to.
        /// </summary>
        /// <remarks>
        /// The function creates a string with the SET_HEADING_DEST_COMMAND_CHAR and Heading.  The string is sent through the COM port by the rcom->SendMsg function.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Heading">The heading to send to the robot.</param>
        /// <param name="Velocity">The velocity to send to the robot.</param>
        void SendHeadingDestination(short RobotAddress, double Heading, short Velocity);

        /// <summary>
        /// This function sends a command to the RugRover to stop the motors.
        /// </summary>
        /// <remarks>
        /// The function sends a string with the command character to stop the motors.  This string is sent through the COM port by the rcom->SendMsg function.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        void SendStopMotors(short RobotAddress);

        /// <summary>
        /// This function sends the Robot a command to change the pwm for the two motors.
        /// </summary>
        /// <remarks>
        /// The function forms a string with the command character to set the motors' PWM at the front followed by motor0's pwm and motor1's pwm.  This string is sent through the COM port by the rcom->SendMsg function.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="M0PWM">The amount of pwm to send to motor 0.</param>
        /// <param name="M1PWM">The amount of pwm to send to motor 1.</param>
        void SendMotorsPWM(short RobotAddress, int M0PWM, int M1PWM);

        /// <summary>
        /// This function sends the robot destination to the RugRover through the serial link.
        /// </summary>
        /// <remarks>
        /// The function uses the LongToBytes function to break the long int X and Y coordinates into their individual bytes.  Then the string to output is formed and sent out using the rcom->SendMsg function.  This is done individually for both the X and Y coordinate.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Destination">The destination coordinate for the RugRover.</param>
        void SendPositionDestination(short RobotAddress, robPOINT Destination);

#ifdef IMPLEMENTED
        /// <summary>
        /// This function decodes the serial input from the robot.  The function returns a character signifying what command was recieved and a long integer array that has the numbers associated with that command (if any.)
        /// </summary>
        /// <remarks>
        /// The input string from the robot is made up of two parts.  The first part is the command code stored as a character at the beginning of the input string.  The second part is a set of optional numbers associated with that command.  The function strips the InputCode command from the beginning of the input string.  Then the function forms the return value.  How this is formed depends on the command code (the length of the number depends on the command character.)
        /// </remarks>
        /// <param name="InputString">The string received from the serial port that is to be decoded.</param>
        /// <param name="InputCode">A character signifying what command was received from the RugRover.</param>
        /// <returns>The long integer part of the command received.  Do not delete the returned pointer.</returns>
        long int* DecodeSerialInput(short RobotAddress, CString* InputString, unsigned char* InputCode);
#endif

        /// <remarks>
        /// This command can change the value of FlipHandle and MaxSpeed in the robot!
        /// </remarks>
        void SendStraightVelocity(short RobotAddress, int Velocity, double Heading);

        void SendP1(short RobotAddress, long P1);
        void SendP2(short RobotAddress, long P2);
        void SendP3(short RobotAddress, long P3);
        void SendP4(short RobotAddress, long P4);
        void SendP5(short RobotAddress, long P5);
        void SendP6(short RobotAddress, long P6);
        void SendP7(short RobotAddress, long P7);
        void SendP8(short RobotAddress, long P8);

        void SendPurgeRingBuffer(short RobotAddress);
        void SendVelocityDirection(short RobotAddress, int Velocity, double Heading, short Priority);
        void SendBuzzerFrequency(short RobotAddress, short BuzzerFrequency);
        void SendBuzzerOn(short RobotAddress, BOOL On);
        void SendMotorControlConstants(short RobotAddress, char Kp, char Ki, char Kd, char Kb, char KPwm);
        void SendTurn(short RobotAddress, double Heading, short Velocity, short Diameter);
        void SendSensitiveObstacleDetection(short RobotAddress, short Sensitive);
        void SendPositionVelocityDestination(short RobotAddress, robPOINT Destination, short Velocity, short Priority);

        void SendInformationToTransmit(short RobotAddress, long mask);
        void SendRequestState(short RobotAddress);
        void SendPowerOff(short RobotAddress);
        void SendRawData(short RobotAddress, RRRMsg &Msg);

        /* new stuff 3/6/99 */
        void SquareToWall(short RobotAddress, double Heading, float TravelTime, BOOL SlipperySurface);
        void SetFeelingMotor(short RobotAddress, unsigned char Motor);
        void CornerCalibrate(short RobotAddress, long X, long Y, double H0, unsigned char FeelWheel, float TravelTime, BOOL SlipperySurface);
        void FindHomeBase(short RobotAddress, double CornerHeading, double WallHeading, long HomeBaseLocationX, long HomeBaseLocationY, BOOL NeedToGetInFrontOfHomeBase);
        void Pause(short RobotAddress);
        void Resume(short RobotAddress);
        void SetHeadingDestination(short RobotAddress, double Heading, short Speed, short MotorEnable);
        void SetEnableObstacleBounce(short RobotAddress, BOOL Enable);
        void SetRAMData(short RobotAddress, unsigned short Address, unsigned char NumberBytes, unsigned char *Data);
        void SetRobotOnSeconds(short RobotAddress, long Seconds);
        void SetIntermittentBeep(short RobotAddress, BOOL On);
        void AddToPosition(short RobotAddress, double DeltaX, double DeltaY);
        void AddToHeading(short RobotAddress, double Delta);
        void SetExternalOutput(short RobotAddress, BOOL On);
        void RequestEX2Value(short RobotAddress);

    protected:
        RRRSrvCom *rcom;                    // common serial interface

    private:
        long WorldToRobotUnits(double WorldUnit);
        double RobotToWorldUnits(long RobotUnit);
        long WorldToRobotUnits(long WorldUnit);
        int RadianToEncoderHeading(double Heading);

        /// <summary>
        /// This function takes a long integer and breaks it up into its individual bytes.
        /// </summary>
        /// <remarks>
        /// This function uses the bit shift operations to strip out the individual bytes of the long integer.  It then returns an array of characters containing the individual characters.
        /// </remarks>
        /// <param name="X">The long integer to break up into individual bytes.</param>
        /// <returns>An array of characters containing the individual bytes of X.  Do not delte the resulting pointer!</returns>
        char* LongToBytes(long int X);

        /// <summary>
        /// This function takes two individual bytes and turns them into an integer.
        /// </summary>
        /// <remarks>
        /// The function uses the or and bit shift operations to glue the two bytes together.
        /// </remarks>
        /// <param name="Byte0">The low byte of the number.</param>
        /// <param name="Byte1">The high byte of the number.</param>
        /// <returns>The ingeger made up of the individual bytes in Byte0 and Byte 1.</returns>
        int BytesToInt(unsigned char Byte0, unsigned char Byte1);

        /// <summary>
        /// This function takes four individual bytes and turns them into a long integer.
        /// </summary>
        /// <remarks>
        /// The function uses the or and bit shift operations to glue the four bytes together.
        /// </remarks>
        /// <param name="Byte0">The low order byte.</param>
        /// <param name="Byte1">The next-lowest order byte.</param>
        /// <param name="Byte2">The previous-highest order byte.</param>
        /// <param name="Byte3">The high order byte.</param>
        /// <returns>The long integer made up of the individual bytes in Byte0 to Byte3.</returns>
        long int BytesToLong(unsigned char Byte0, unsigned char Byte1, unsigned char Byte2, unsigned char Byte3);
};