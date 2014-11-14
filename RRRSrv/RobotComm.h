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

/// <summary>
/// This class will build and send instructions to the robot.
/// </summary>
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
        /// <param name="Priority">The priority of this request.  LOW_PRIORITY or HIGH_PRIORITY.</param>
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
        /// This is called Stop in the published API.
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

        /// <summary>
        /// This instruction makes the robot move in a straight line with a given speed.  The direction to move is given by Heading.  The robot first rotates to Heading then moves in a straight line in that direction.
        /// </summary>
        /// <remarks>
        /// This command can change the value of FlipHandle and MaxSpeed in the robot!
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Velocity">The speed to move forward in 0.1" / s.</param>
        /// <param name="Heading">The heading to move straight in radians.</param>
        void SendStraightVelocity(short RobotAddress, int Velocity, double Heading);

        /// <summary>
        /// Set the robot's parameter 1.
        /// </summary>
        /// <remarks>
        /// I have no idea what this does.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="P1">The value to set parameter 1 to.  The default is 1500.</param>
        void SendP1(short RobotAddress, long P1);

        /// <summary>
        /// Set the robot's parameter 2.
        /// </summary>
        /// <remarks>
        /// I have no idea what this does.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="P2">The value to set parameter 2 to.  The default is 100.</param>
        void SendP2(short RobotAddress, long P2);

        /// <summary>
        /// Set the robot's parameter 3.
        /// </summary>
        /// <remarks>
        /// I have no idea what this does.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="P3">The value to set parameter 3 to.  The default is 90.</param>
        void SendP3(short RobotAddress, long P3);

        /// <summary>
        /// Set the robot's parameter 4.
        /// </summary>
        /// <remarks>
        /// I have no idea what this does.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="P4">The value to set parameter 4 to.  The default is 500.</param>
        void SendP4(short RobotAddress, long P4);

        /// <summary>
        /// Set the robot's parameter 5.
        /// </summary>
        /// <remarks>
        /// I have no idea what this does.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="P5">The value to set parameter 5 to.  The default is 800.</param>
        void SendP5(short RobotAddress, long P5);

        /// <summary>
        /// Set the robot's parameter 6.
        /// </summary>
        /// <remarks>
        /// I have no idea what this does.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="P6">The value to set parameter 6 to.  The default is 51.</param>
        void SendP6(short RobotAddress, long P6);

        /// <summary>
        /// Set the robot's parameter 7.
        /// </summary>
        /// <remarks>
        /// I have no idea what this does.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="P7">The value to set parameter 7 to.  The default is 250.</param>
        void SendP7(short RobotAddress, long P7);

        /// <summary>
        /// Set the robot's parameter 8.
        /// </summary>
        /// <remarks>
        /// I have no idea what this does.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="P8">The value to set parameter 8 to.  The default is 0.</param>
        void SendP8(short RobotAddress, long P8);

        /// <summary>
        /// This method instructs the robot to purge its ring buffer.
        /// </summary>
        /// <remarks>
        /// I'm unsure of the outcome of this call.  It appears to be used by the manufacturer when testing things.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        void SendPurgeRingBuffer(short RobotAddress);

        /// <summary>
        /// This method makes the robot move in a given direction with a given speed at a given priority.
        /// </summary>
        /// <remarks>
        /// The MoveDirection call in the published API uses this with a HIGH_PRIORITY urgency.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Velocity">The the speed to move with in 0.1" / s units.</param>
        /// <param name="Heading">The the direction to move (in radians).</param>
        /// <param name="Priority">The priority of this request.  LOW_PRIORITY or HIGH_PRIORITY.</param>
        void SendVelocityDirection(short RobotAddress, int Velocity, double Heading, short Priority);

        /// <summary>
        /// This method makes the robot's buzzer buzz at the given frequency.  The range is about 0 – 2.5 KHz.  At higher frequencies, the resolution is very coarse.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="BuzzerFrequency">The buzzer's frequency in Hz.</param>
        void SendBuzzerFrequency(short RobotAddress, short BuzzerFrequency);

        /// <summary>
        /// This method simply turns the buzzer on or off.  When on, the buzzer's frequency is set to 2 KHz.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="On">A value indicating whether to turn the buzzer on (true) or off (false).</param>
        void SendBuzzerOn(short RobotAddress, BOOL On);

        /// <summary>
        /// This method sets the values the robot uses to control the PWM to the wheels when moving.
        /// </summary>
        /// <remarks>
        /// Whenever the robot is moving to a destination point or heading, it controls the velocities at each wheel to reach the destination.  The terms Kp, Ki, Kd and KPwm effect how the velocity is controlled at each motor.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Kp">The proportional term.</param>
        /// <param name="Ki">The integral term.</param>
        /// <param name="Kd">The differential term.</param>
        /// <param name="Kb">The strength of the bungee.</param>
        /// <param name="KPwm">The averaging term.</param>
        void SendMotorControlConstants(short RobotAddress, char Kp, char Ki, char Kd, char Kb, char KPwm);

        /// <summary>
        /// This method makes the robot move in a circular path until it reaches the given heading.  The diameter of the circular path is given by Diameter, and the travel speed is given by Speed.
        /// </summary>
        /// <remarks>
        /// This is called MoveCurve in the published API.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Heading">The heading to turn to in radians.</param>
        /// <param name="Velocity">The speed to turn with in 0.1" / s.</param>
        /// <param name="Diameter">The diameter of the curve in 0.1" units.</param>
        void SendTurn(short RobotAddress, double Heading, short Velocity, short Diameter);

        /// <summary>
        /// This method sets how sensitive the robot is to obstacles.  Lower sensitivity means the robot has to hit an obstacle harder before detecting it.  Setting the robot's obstacle sensitivity to high can cause the robot to detect imaginary obstacles.  If the sensitivity is set to 0 (None), the only time the robot will detect an obstacle is if there is a dangerous amount of current going to either motor.
        /// </summary>
        /// <remarks>
        /// This is called SetObstacleSensitivity in the published API.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Sensitive">The obstacle detection sensitivity.</param>
        void SendSensitiveObstacleDetection(short RobotAddress, short Sensitive);

        /// <summary>
        /// This method makes the robot move to the (X, Y) position at the given speed and priority.
        /// </summary>
        /// <remarks>
        /// The MoveTo call in the published API uses this with a HIGH_PRIORITY urgency.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Destination">The destination position tuple to move to in 0.1" units.</param>
        /// <param name="Velocity">The speed to move with in 0.1" / s units.</param>
        /// <param name="Priority">The priority of this request.  LOW_PRIORITY or HIGH_PRIORITY.</param>
        void SendPositionVelocityDestination(short RobotAddress, robPOINT Destination, short Velocity, short Priority);

        /// <summary>
        /// This method sends the robot an information transmission mask.
        /// </summary>
        /// <remarks>
        /// I'm unsure of the outcome of this call.  It appears to be used by the manufacturer when testing things.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="mask">A bit mask of Velocity || Acceleration || AD || Position.</param>
        void SendInformationToTransmit(short RobotAddress, long mask);

        /// <summary>
        /// This method instructs the robot to send its current state.  The robot responds with messages reporting its position, heading, wheel velocities, motor currents, battery level, charging state, software revision, the amount of time It has been on and if it is on or off the home base.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        void SendRequestState(short RobotAddress);

        /// <summary>
        /// This method instructs the robot to turn itself off.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        void SendPowerOff(short RobotAddress);

        /// <summary>
        /// Send a raw message to the robot.
        /// </summary>
        /// <remarks>
        /// This is called SendMessage in the published API.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Msg">The message to send to the robot.</param>
        void SendRawData(short RobotAddress, RRRMsg &Msg);

        /// <summary>
        /// This method instructs the robot to square up to a wall.
        /// </summary>
        /// <remarks>
        /// The robot turns to the supplied heading then goes straight until it hits an obstacle.  If it finds one, it pushes its bumper against the wall.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Heading">The direction to the wall in radians.</param>
        /// <param name="TravelTime">The time to move forward trying to find the wall in seconds.</param>
        /// <param name="SlipperySurface">A value indicating whether the robot is on a slippery surface (true) or not (false).</param>
        void SquareToWall(short RobotAddress, double Heading, float TravelTime, BOOL SlipperySurface);

        /// <summary>
        /// This method instructs the robot to move forward with a small PWM on the motors.  This can be used to lightly feel obstacles.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Motor">The motor to feel with.  0 for the right wheel, 1 for the left, 2 for both.</param>
        void SetFeelingMotor(short RobotAddress, unsigned char Motor);

        /// <summary>
        /// This method instructs the robot to calibrate against a corner.  The robot should square up to the primary wall before calibrating on the corner.
        /// </summary>
        /// <remarks>
        /// Which direction the robot turns to find the corner is determined by the FeelWheel parameter.  If FeelWheel is 0, the robot turns to the left and uses the right wheel to feel the corner.  If FeelWheel is 1, the robot turns to the right and uses the left wheel to feel the corner.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="X">The X coordinate of the main wall.</param>
        /// <param name="Y">The Y coordinate of the main wall.</param>
        /// <param name="H0">The heading of the main wall (in radians).</param>
        /// <param name="FeelWheel">The wheel to use to feel the corner.</param>
        /// <param name="TravelTime">The time to travel towards the corner wall (in seconds).</param>
        /// <param name="SlipperySurface">A value indicating whether the corner is on a slippery surface (true) or not (false).</param>
        void CornerCalibrate(short RobotAddress, long X, long Y, double H0, unsigned char FeelWheel, float TravelTime, BOOL SlipperySurface);

        /// <summary>
        /// This method instructs the robot to find the home base.  The robot should calibrate at the home base's calibration walls before trying to find the home base.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="CornerHeading">The heading to the corner wall (in radians).</param>
        /// <param name="WallHeading">The heading to the main wall (in radians).</param>
        /// <param name="HomeBaseLocationX">The X coordinate of the home base position (in 0.1" units).</param>
        /// <param name="HomeBaseLocationY">The Y coordinate of the home base position (in 0.1" units).</param>
        /// <param name="NeedToGetInFrontOfHomeBase">A value indicating whether the robot needs to get in front of the home base (true) or not (false).</param>
        void FindHomeBase(short RobotAddress, double CornerHeading, double WallHeading, long HomeBaseLocationX, long HomeBaseLocationY, BOOL NeedToGetInFrontOfHomeBase);
        
        /// <summary>
        /// This method pauses the robot.  If the robot was executing an instruction, it will pause until given a Resume command or told to Stop.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        void Pause(short RobotAddress);
        
        /// <summary>
        /// If the robot was paused, this command makes the robot resume execution of its last instruction.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        void Resume(short RobotAddress);

        /// <summary>
        /// Use this method to turn the robot.  When given this instruction, the robot moves one wheel forward and one backward until it faces the correct heading.  To control how fast the robot rotates, use the Speed parameter.  If you want the robot to use only one wheel when rotating, set the MotorEnable parameter so the correct wheel turns.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Heading">The heading to turn to (in radians).</param>
        /// <param name="Speed">The speed to turn with (in 0.1" / s units).</param>
        /// <param name="MotorEnable">Which motors to enable while turning.</param>
        void SetHeadingDestination(short RobotAddress, double Heading, short Speed, short MotorEnable);

        /// <summary>
        /// When obstacle bounce is enabled on the robot, the robot will reverse the motors for a short period whenever it finds an obstacle.  This is meant to reduce the amount of wheel slipping that occurs when the robot runs into something
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Enable">A value indicating whether the robot should reverse motors when it hits an obstacle (true) or not (false).</param>
        void SetEnableObstacleBounce(short RobotAddress, BOOL Enable);

        /// <summary>
        /// Set data directly in the robot's RAM.
        /// </summary>
        /// <remarks>
        /// Don't use this.  It's for the manufacturer to use when working with the firmware on the robot.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Address">RAM address of the first memory location to change.</param>
        /// <param name="NumberBytes">The number of bytes to be written.</param>
        /// <param name="Data">The data to be stored in RAM.</param>
        void SetRAMData(short RobotAddress, unsigned short Address, unsigned char NumberBytes, unsigned char *Data);
        
        /// <summary>
        /// The robot stores how many seconds it has been on.  Use this method to set that value.  When this method is used, the robot will update its stored value if the new value is greater than the value stored in the robot.  So if the robot thinks it has been on for 100 seconds and you try to set it to 50 seconds, the robot will keep the old 100 seconds value.  If the robot thinks it has been on for 100 seconds and you try to set it to 200 seconds, the robot will update its stored value.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Seconds">The number of seconds the robot has been on.</param>
        void SetRobotOnSeconds(short RobotAddress, long Seconds);

        /// <summary>
        /// Using this method, you can make the robot beep intermittently.  When this feature is enabled, the robot will beep at 1000Hz for 1½ seconds then be quiet for 1½ seconds.  This is similar to the beeping one hears when a large truck is backing up.  It can be used to warn people when the robot is near.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="On">A value indicating whether the robot should beep intermittently (true) or not (false).</param>
        void SetIntermittentBeep(short RobotAddress, BOOL On);

        /// <summary>
        /// This method is used to correct the robot’s position.  When the robot receives this instruction, it will add DeltaX and DeltaY to its X and Y position.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="DeltaX">The amount to add to the robot's X position in 0.1" units.</param>
        /// <param name="DeltaY">The amount to add to the robot's Y position in 0.1" units.</param>
        void AddToPosition(short RobotAddress, double DeltaX, double DeltaY);

        /// <summary>
        /// This method is used to correct the robot’s heading.  When the robot receives this instruction, it will add Delta to its heading.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="Delta">The amount to add to the robot's heading in radians.</param>
        void AddToHeading(short RobotAddress, double Delta);

        /// <summary>
        /// Use this method to turn the robot’s external output on or off.  The robot’s external output is pin 7 on the robot header.  It can sink a maximum of 0.75A, and the pin can have a maximum of +12V on it.
        /// </summary>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        /// <param name="On">A value indicating whether to turn the external output on (true) or off (false).</param>
        void SetExternalOutput(short RobotAddress, BOOL On);

        /// <summary>
        /// Use this method to request the voltage level at the external input pin on the robot header.  When the robot sends the voltage level, the application will receive an ExternalInput Event containing the voltage at the external input.
        /// </summary>
        /// <remarks>
        /// This is called RequestExternalInput in the published API.
        /// </remarks>
        /// <param name="RobotAddress">The address of the robot to send the message to.</param>
        void RequestEX2Value(short RobotAddress);

    protected:
        /// <summary>
        /// Common serial interface.
        /// </summary>
        RRRSrvCom *rcom;

    private:
        /// <summary>
        /// Convert real-world length units (in 0.1" steps) to units the robot will understand.
        /// </summary>
        /// <param name="WorldUnit">Real-world units in 0.1" steps to convert.</param>
        /// <returns>A converted value for the robot.</returns>
        long WorldToRobotUnits(double WorldUnit);

        /// <summary>
        /// Convert real-world length units (in 0.1" steps) to units the robot will understand.
        /// </summary>
        /// <param name="WorldUnit">Real-world units in 0.1" steps to convert.</param>
        /// <returns>A converted value for the robot.</returns>
        long WorldToRobotUnits(long WorldUnit);

        /// <summary>
        /// Convert a length value from the robot into real-world units (in 0.1" steps).
        /// </summary>
        /// <param name="RobotUnit">A length value from the robot in robot units.</param>
        /// <returns>A converted value in 0.1" units.</returns>
        double RobotToWorldUnits(long RobotUnit);

        /// <summary>
        /// Convert radians used for headings into a value the robot can understand.
        /// </summary>
        /// <param name="Heading">The heading in radians.</param>
        /// <returns>Encoded heading for the robot.</returns>
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