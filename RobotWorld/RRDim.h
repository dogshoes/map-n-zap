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
 #ifndef RRDIM2309423ojdrfg0394ldgj0934lkdjg0938lkd
 
 #define RRDIM2309423ojdrfg0394ldgj0934lkdjg0938lkd
 /*RugRover dimensions*/
 const float cRobotEncoderTicksPerInch = 12.20187f;		//Number of encoder ticks per inch (as reported by robot)
 const float cTPI = 10.0f;										//Encoder Ticks Per Inch (in robot world coords)
 const float cIPT = 1.0f/cTPI;									//Inches Per Encoder Tick (in robot world coords)
 //const long cNHeadings = 1062;									//Total number of headings in 1 rotation
 const long cNHeadings = 1160;									//Total number of headings in 1 rotation
 
 const float cRRNoseLength = 4.1f*cTPI;							//Length of the body from wheel axle to nose.
 const float cRRWheelBase = 15.5f*cTPI;						//distance between wheels
 const float cRRWidth = 15.5f * cTPI;					//Width of the robot's body
 const short cRRDefaultHandleLength = short(cRRWheelBase);		//Default length of the robot's handle
 const float cThinCarpetKHeadError = 0.00169f;
 const float cThinCarpetKStraightHeadError = 0.001f;
 const float cThickCarpetKHeadError = 0.0039f;
 const float cThickCarpetKStraightHeadError = 0.002f;
 
 //const int cSearchPositionPWM = 220;
 //const int cSearchHeadingPWM = 220;
 
 const short cFollowCTDPathHandleLength = short(10.0f/cRobotEncoderTicksPerInch*cTPI);
 //const float cCornerWallBackupDistance = 55.0f;
 const float cCornerWallBackupDistance = cRRWidth/2 + 55 + cFollowCTDPathHandleLength - cRRNoseLength;
 const short cComputerControlSpeed = 200;					//Speed the robot will go while under computer control.
 const short cComputerControlHeadingSpeed = 200;			//Speed the robot will go while turning under computer control
 const short cObstacleDetectionSpeed = 100;				//Speed the robot will go when trying to detect an obstacle
 const short cMaxSpeed = 330;									//Maximum speed the robot can go
 const short cMinSpeed = 25;									//Minimum speed the robot can go
 const short cMaxPwm = 1000;									//Maximum Pwm for robot
 
 typedef enum {osNone = 0, osLowSensitivity = 1, osMediumSensitivity = 2, osHighSensitivity = 3} TObstacleSensitivity;
 
 #endif