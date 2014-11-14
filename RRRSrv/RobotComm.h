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

        void ResetInterface(RRRSrvCom *newrcom)
        {
            rcom = newrcom;
        }

        void SendP8(short RobotAddress, long P8);
        void SendP7(short RobotAddress, long P7);
        void SendP6(short RobotAddress, long P6);
        void SendP5(short RobotAddress, long P5);
        void SendP4(short RobotAddress, long P4);
        void SendP3(short RobotAddress, long P3);
        void SendP2(short RobotAddress, long P2);
        void SendP1(short RobotAddress, long P1);
        void SendBuzzerFrequency(short RobotAddress, short BuzzerFrequency);
        void SendBuzzerOn(short RobotAddress, BOOL On);
        void SendPathSegment(short RobotAddress, BOOL PurgePath, robPOINT* Points, short nPoints);
        void SendFollowPath(short RobotAddress, short FollowPath);
        void SendHandleLength(short RobotAddress, short HandleLength);
        void SendMotorControlConstants(short RobotAddress, char Kp, char Ki, char Kd, char Kb, char KPwm);
        void SendTurn(short RobotAddress, double Heading, short Velocity, short Diameter);
        ~CRobotComm();
        void SendSensitiveObstacleDetection(short RobotAddress, short Sensitive);
        void SendStraightVelocity(short RobotAddress, int Velocity, double Heading);
        void SendPositionVelocityDestination(short RobotAddress, robPOINT Destination, short Velocity, short Priority);
        void SendMaxSpeed(short RobotAddress, short MaxSpeed);
        void SendHeading(short RobotAddress, double Heading);
        void SendPosition(short RobotAddress, robPOINT Position);
        void SendMotorVelocities(short RobotAddress, int M0Velocity, int M1Velocity, short Priority);
        void SendHeadingCorrectionFactor(short RobotAddress, int Correction);
        void SendFlipHandle(short RobotAddress, BOOL value);
        void SendMaxPWM(short RobotAddress, unsigned char MaxPWM);
        void SendHeadingDestination(short RobotAddress, double Heading, short Velocity);
        void SendStopMotors(short RobotAddress);
        void SendPurgeRingBuffer(short RobotAddress);
        void SendMotorsPWM(short RobotAddress, int M0PWM, int M1PWM);
        void SendPositionDestination(short RobotAddress, robPOINT Destination);
        void SendVelocityDirection(short RobotAddress, int Velocity, double Heading, short Priority);
        long int* DecodeSerialInput(short RobotAddress, CString* InputString, unsigned char* InputCode);
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
        char* LongToBytes(long int X);
        int BytesToInt(unsigned char Byte0, unsigned char Byte1);
        long int BytesToLong(unsigned char Byte0, unsigned char Byte1,
                             unsigned char Byte2, unsigned char Byte3);
};