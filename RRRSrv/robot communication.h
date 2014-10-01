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
   //
   // Module:     Robot Communication.h
   //
   // Abstract:   CRobotComm class header file.
   //
   // Author:     S.R.	
   //
   // Revisions:
   //    $History: robot communication.h $
   // 
   // *****************  Version 13  *****************
   // User: Harnden      Date: 10/12/99   Time: 12:23a
   // Updated in $/RRR/RRRSrv
   // Added multi robot support.
   // 
   // *****************  Version 12  *****************
   // User: Harnden      Date: 8/09/99    Time: 10:03p
   // Updated in $/RRR/RRRSrv
   // Merged 1P5 changes into mainstream. Included changes from Stephan in
   // RRRSrvCom.cpp.
   // 
   // *****************  Version 13  *****************
   // User: Harnden      Date: 4/27/99    Time: 2:07a
   // Updated in $/RRR1P5 Branch/RRRSrv
   // Incorporated fixes made to 1.04 software to make work on Pony.
   // 
   // *****************  Version 11  *****************
   // User: Harnden      Date: 3/07/99    Time: 12:12p
   // Updated in $/RRR/RRRSrv
   // Added all functionality in Stephan's document. Converted name to
   // CyeSrv.exe.
   // 
   // *****************  Version 10  *****************
   // User: Harnden      Date: 12/19/98   Time: 3:04p
   // Updated in $/RRR/RRRSrv
   // Added message priority support.
   // 
   // *****************  Version 9  *****************
   // User: Harnden      Date: 11/16/98   Time: 12:41a
   // Updated in $/RRR/RRRSrv
   // Added dynamic serial port change logic.
   // 
   // *****************  Version 8  *****************
   // User: Harnden      Date: 8/03/98    Time: 10:57p
   // Updated in $/RRR/RRRSrv
   // Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
   // PowerDown command. Added send and receive of raw data. Added
   // CommStateChange event. Purge outgoing message queue when communications
   // link is lost.
   // 
   // *****************  Version 7  *****************
   // User: Harnden      Date: 7/06/98    Time: 12:44a
   // Updated in $/RRR/RRRSrv
   // Added new RobotStatusUpdate event and changed SendFollowPath from BOOL
   // to short.
   // 
   // *****************  Version 6  *****************
   // User: Harnden      Date: 6/22/98    Time: 9:11p
   // Updated in $/RRR/RRRSrv
   // Added three new messages and one event.
   // 
   // *****************  Version 5  *****************
   // User: Harnden      Date: 5/25/98    Time: 10:19a
   // Updated in $/RRRSrv
   // Working radio interface.
   // 
   // *****************  Version 4  *****************
   // User: Harnden      Date: 3/22/98    Time: 10:01p
   // Updated in $/RRRSrv
   // Added new functions for Stephan.
   // 
   // *****************  Version 3  *****************
   // User: Harnden      Date: 1/12/98    Time: 12:45a
   // Updated in $/RRRSrv
   // First delivery of ActiveX interface.
   // 
   // *****************  Version 2  *****************
   // User: Harnden      Date: 1/05/98    Time: 12:50a
   // Updated in $/RRRSrv
   // Full send message protocol support (except ExecutePath).
   // 
   // *****************  Version 1  *****************
   // User: Harnden      Date: 1/05/98    Time: 12:16a
   // Created in $/RRRSrv
   //
   // Copyright 1997 Cycle Time Corp
   //
   ///////////////////////////////////////////////////////////////////////////////
   
   #ifndef ROBOTCOMMUNICATION230495lsdfjg0934ldfjb039w4ldfj
   
   #define ROBOTCOMMUNICATION230495lsdfjg0934ldfjb039w4ldfj
   
   #include "RothMath.h"
   #include "rrdim.h"
   #include "RRRConst.h"
   
   class RRRSrvCom;                       // forward declaration for serial interface class
   
   #define cCornerWallBackupDistance cRRWidth/2 + 55 + (10.0f/cRobotEncoderTicksPerInch*cTPI) - cRRNoseLength // dummy value!!!! constant is missing
   
   class CRobotComm : public CObject {
   public:
      CRobotComm();
   
      void ResetInterface(RRRSrvCom *newrcom) {rcom = newrcom;}
   
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
   
   #endif