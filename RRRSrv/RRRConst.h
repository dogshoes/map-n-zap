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
// Module:     RRRConst.h
//
// Abstract:   Common RRR constants.
//
// Author:     J.C.H	
//
// Revisions:
//    $History: RRRConst.h $
// 
// *****************  Version 15  *****************
// User: Harnden      Date: 10/25/99   Time: 10:56p
// Updated in $/RRR/RRRSrv
// Added MicrophoneVolume event. Implemented SetRobotAddress. Removed all
// references to RobotAdress setting in CyeSrv.ini file.
// 
// *****************  Version 14  *****************
// User: Harnden      Date: 10/17/99   Time: 3:02p
// Updated in $/RRR/RRRSrv
// Rewrote message timeout and delivery logic to avoid duplicate messages.
// 
// *****************  Version 13  *****************
// User: Harnden      Date: 10/12/99   Time: 12:23a
// Updated in $/RRR/RRRSrv
// Added multi robot support.
// 
// *****************  Version 12  *****************
// User: Harnden      Date: 3/07/99    Time: 12:12p
// Updated in $/RRR/RRRSrv
// Added all functionality in Stephan's document. Converted name to
// CyeSrv.exe.
// 
// *****************  Version 11  *****************
// User: Harnden      Date: 12/19/98   Time: 3:04p
// Updated in $/RRR/RRRSrv
// Added message priority support.
// 
// *****************  Version 10  *****************
// User: Harnden      Date: 12/03/98   Time: 12:07p
// Updated in $/RRR/RRRSrv
// Added RadioLevel support and fixed about box displays.
// 
// *****************  Version 9  *****************
// User: Harnden      Date: 8/03/98    Time: 10:57p
// Updated in $/RRR/RRRSrv
// Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
// PowerDown command. Added send and receive of raw data. Added
// CommStateChange event. Purge outgoing message queue when communications
// link is lost.
// 
// *****************  Version 8  *****************
// User: Harnden      Date: 7/06/98    Time: 10:28p
// Updated in $/RRR/RRRSrv
// Added Heading parameter to RobotStatusUpdate event.
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
// User: Harnden      Date: 3/22/98    Time: 7:38p
// Updated in $/RRRSrv
// Updated status to include work at CTC.
// 
// *****************  Version 3  *****************
// User: Harnden      Date: 1/12/98    Time: 12:45a
// Updated in $/RRRSrv
// First delivery of ActiveX interface.
// 
// *****************  Version 2  *****************
// User: Harnden      Date: 1/03/98    Time: 5:58p
// Updated in $/RRRSrv
// Completion of all ActiveX method interfaces for RobotWorld.
// 
// *****************  Version 1  *****************
// User: Harnden      Date: 12/29/97   Time: 6:48p
// Created in $/RRRSrv
//
// Copyright 1997 Cycle Time Corp
//
///////////////////////////////////////////////////////////////////////////////

#ifndef RRRCONST
#define RRRCONST

//
// Message codes for message from PC to Robot
enum PC_XMIT_CODES
{
   SET_HEADING_DEST_COMMAND_CHAR='H',				//Command character to set the robot's heading destination
   SET_TURN_COMMAND_CHAR='T',						//Command character to turn the robot
   SET_MOTORS_PWM_COMMAND_CHAR='D',				//Command character to set the robot's PWM for both motors
   SET_MAXPWM_COMMAND_CHAR='M',						//Command character to set the robot's maximum PWM
   SET_MAXSPEED_COMMAND_CHAR='s',					//Command character to set the roobt's maximum speed
   SET_FLIPHANDLE_COMMAND_CHAR='F',				//Command character to set the robot's flip handle parameter
   SET_MOTORS_VELOCITY_COMMAND_CHAR='V',			//Command character to set the robot's motor speeds
   SET_HEADCORRECTFACT_COMMAND_CHAR='c',			//Command character to set the robot's heading correction factor
   SET_POSITIONVELOCITY_DESTINATION_COMMAND_CHAR='v',
   SET_BUNGEE_DEST_COMMAND_CHAR='b',
   STOP_MOTORS_COMMAND_CHAR='S',					//Command character to stop the robot
   REQUEST_ROBOT_POSITION_COMMAND_CHAR='P',		//Command character to request the robot's current position
   SET_HEADING_COMMAND_CHAR='h',					//Command character to set the robot's heading
   SET_POSITION_COMMAND_CHAR='p',					//Command character to set the robot's position.
   SET_GO_STRAIGHT_VELOCITY='a',					//Command character to go straight at a certain velocity
   SET_SENSITIVE_OBSTACLE_DETECTION='o',			//Command character to set the sensitive obstacle detection
   PURGE_RING_BUFFER_COMMAND_CHAR='R',			//Command character to send the ring buffer
   SET_MOTION_CONTROL_CONSTANTS_COMMAND_CHAR='K',
   SET_HANDLE_LENGTH_COMMAND_CHAR='L',
   SET_FOLLOW_PATH_COMMAND_CHAR='P',
   SET_PATH_SEGMENT_COMMAND_CHAR='A',
   SET_BUZZER_ON_COMMAND_CHAR='B',
   SET_BUZZER_FREQUENCY_COMMAND_CHAR='f',
   SET_P1_COMMAND_CHAR='1',
   SET_P2_COMMAND_CHAR='2',
   SET_P3_COMMAND_CHAR='3',
   SET_P4_COMMAND_CHAR='4',
   SET_P5_COMMAND_CHAR='5',
   SET_P6_COMMAND_CHAR='6',
   SET_P7_COMMAND_CHAR='7',
   SET_P8_COMMAND_CHAR='8',
   SET_VELOCITY_DIRECTION_COMMAND_CHAR='d',
   SET_INFORMATION_TO_TRANSMIT_COMMAND_CHAR='i',
   REQUEST_STATE_COMMAND_CHAR='r',
   REQUEST_POWER_OFF_CHAR='O',

	SET_RAM_DATA_COMMAND_CHAR = 'n',
	SET_ENABLE_OBSTACLE_BOUNCE_COMMAND_CHAR = 'q',
	SET_HEADING_DESTINATION_EXTENDED = 't',
	SEND_WALL_CALIBRATE_COMMAND_CHAR = 'w',
	SEND_SQUARE_TO_WALL_COMMAND_CHAR = 'W',
	SEND_FEELING_MOTOR_COMMAND_CHAR = 'C',
	SET_EX1_COMMAND_CHAR = 'y',
	GET_EX2_COMMAND_CHAR = 'x',
	SEND_CORNER_CALIBRATE_COMMAND_CHAR = 'Z',
	SEND_FIND_HOME_BASE_COMMAND_CHAR = 'z',
	SEND_PAUSE_COMMAND_CHAR = 'u',
	SET_ROBOT_ON_SECONDS_COMMAND_CHAR = 'l',
	SET_INTERMITTENT_BEEP_ON_COMMAND_CHAR = 'k',
	SET_POSITION_DELTA_COMMAND_CHAR = 'j',
	SET_HEADING_DELTA_COMMAND_CHAR = 'J',

   // protocol codes
   POLL_REQUEST=10,
   ISTRT_REQUEST=11,
   ASTRT_REQUEST=12,
   ACK_REQUEST=13,
};

//
// Message codes for messages sent from Robot to PC
const unsigned char cPositionInput = 'P';					//Serial input command character for a position packet
const unsigned char cNudgeInput = 'N';						//Serial input command character for a nudge packet
const unsigned char cObstacleInput = 'O';					//Serial input command character for an obstacle packet
const unsigned char cVelocityInput = 'V';					//Serial input command character for a velocity packet
const unsigned char cAccelerationInput = 'A';			//Serial input command character for an acceleration packet
const unsigned char cEncodersInput = 'E';					//Serial input command character for an encoders packet
const unsigned char cAnalogInput = 'a';					//Serial input command character for an analog packet
const unsigned char cPwmInput = 'p';						//Serial input command character for a pwm packet
const unsigned char cRingBufferInput = 'R';				//Serial input command character for a ring buffer dump
const unsigned char cSendNewPathSegment = 's';			//Serial input command character for sending a new robot path segment
const unsigned char cInvalidInputString = 0;				//returned when the command character from a serial string was invalid
const unsigned char cLowBatteryMessage = 'b';		   //Serial input command character for low battery message
const unsigned char cRobotError = 'e';		            //Serial input command character for robot generic error
const unsigned char cRobotAtHome = 'h';		         //Serial input command character for robot at home
const unsigned char cRobotStatusUpdate = 'Q';		   //Serial input command character for robot status update
const unsigned char cBatteryChargeState = 'S';		   //Serial input command character for battery charge state change
const unsigned char cTaskResultInput = 'r';		      //Serial input command character for Task Result
const unsigned char cSoftwareRevNumber = 't';		   //Serial input command character for Software Revision Number
const unsigned char cRobotOn = 'o';		               //Serial input command character for Robot On
const unsigned char cRobotOnSeconds = 'Z';		      //Serial input command character for Robot On Seconds
const unsigned char cSendEX2 = '2';		               //Serial input command character for Send External Input

enum COMM_STATES
{
   ISTRT_STATE=0,                      // initiating startup sequence
   ASTRT_STATE=1,                      // acknowledge start sequence
   IDLE_STATE=2,                       // connected and operational state
   ACK_WAIT_STATE=3,                   // waiting for receipt acknowledge
   UNKNOWN_STATE=-1,                   // unknown comm state, probably not initialized
};

enum MESSAGE_PRIORITIES
{
   LOW_PRIORITY=0,                     // initiating startup sequence
   HIGH_PRIORITY=1,                    // acknowledge start sequence
};

enum WND_MSG_CONSTANTS
{
   RRR_VELOCITY_CHANGE=WM_USER+100,    // velocity changed window message
   RRR_LOCAL_COPYDATA=WM_USER+101,     // local copy data window message
   RRR_TRAY_NOTIFICATIONS=WM_USER+102, // tray icon notification messages
   RRR_SENDING=WM_USER+103,            // sending a message
   RRR_RECEIVING=WM_USER+104,          // receiving a message
   RRR_COMMSTATECHANGE=WM_USER+105,    // protocol interface state change
   RRR_RADIOLEVELCHANGE=WM_USER+106,   // radio level state change
   RRR_MSG_RECV=WM_USER+107,           // message received
   RRR_MSG_TIMEOUT=WM_USER+108,        // message timeout
};

enum ERROR_CONSTANTS
{
   INDEX_OUT_OF_RANGE=1001,            // invalid array index
};

enum RRR_COPYDATA_CONSTANTS
{
   RRR_CHECKSUM_CDMSG=100,
   RRR_RAWDATA_CDMSG=101,
};

struct RRR_CHECKSUM_MSG
{
   short CommandNum;
   long ReturnValue[7];
   float fReturnValue[3];
   double dReturnValue[1];
   unsigned char SoundVolume;
};

#endif

