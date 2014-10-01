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
// Module:     RRRDispID.h
//
// Abstract:   Dispatch ID definitions file.
//
// Author:     J.C.H	
//
// Revisions:
//    $History: RRRDispID.h $
// 
// *****************  Version 11  *****************
// User: Harnden      Date: 10/25/99   Time: 10:55p
// Updated in $/RRR/RRRCom
// Added MicrophoneVolume event. Implemented SetRobotAddress. Removed all
// references to RobotAdress setting in CyeSrv.ini file.
// 
// *****************  Version 10  *****************
// User: Harnden      Date: 10/12/99   Time: 12:25a
// Updated in $/RRR/RRRCom
// Added multi robot support.
// 
// *****************  Version 9  *****************
// User: Harnden      Date: 3/07/99    Time: 12:14p
// Updated in $/RRR/RRRCom
// Added all functionality in Stephan's document.
// 
// *****************  Version 8  *****************
// User: Harnden      Date: 3/06/99    Time: 11:58a
// Updated in $/RRR/RRRCom
// Last file set sent to Henry.
// 
// *****************  Version 7  *****************
// User: Harnden      Date: 2/14/99    Time: 9:53p
// Updated in $/RRR/RRRCom
// Updated to VC 6.0 and renamed all existing methods to prepare for
// release.
// 
// *****************  Version 6  *****************
// User: Harnden      Date: 11/16/98   Time: 12:42a
// Updated in $/RRR/RRRCom
// Added dynamic serial port change logic.
// 
// *****************  Version 5  *****************
// User: Harnden      Date: 9/09/98    Time: 7:13a
// Updated in $/RRR/RRRCom
// Added RetryLimit property.
// 
// *****************  Version 4  *****************
// User: Harnden      Date: 8/03/98    Time: 10:58p
// Updated in $/RRR/RRRCom
// Added OnHomeBase parameter. Added OnBatteryChargeStateChange. Added
// PowerDown command. Added send and receive of raw data. Added
// CommStateChange event. Purge outgoing message queue when communications
// link is lost.
// 
// *****************  Version 3  *****************
// User: Harnden      Date: 6/22/98    Time: 9:11p
// Updated in $/RRR/RRRCom
// Added three new messages and one event.
// 
// *****************  Version 2  *****************
// User: Harnden      Date: 5/26/98    Time: 10:51p
// Updated in $/RRR/RRRCom
// First radio interface release sent to Cycle Time Corp.
// 
// *****************  Version 1  *****************
// User: Harnden      Date: 5/25/98    Time: 3:54p
// Created in $/RRR/RRRCom
//
// Copyright 1997 Cycle Time Corp
//
///////////////////////////////////////////////////////////////////////////////

// IRRRCom constants
#define RRRCTL_VERSION 100
#define RRRCTL_X 101
#define RRRCTL_Y 102
#define RRRCTL_HEADING 103
#define RRRCTL_ROBOTADDRESS 104
#define RRRCTL_VELOCITY 105
#define RRRCTL_RADIOLEVEL 106
#define RRRCTL_BATTERYLEVEL 107
#define RRRCTL_RESPONSETIMEOUT 108
#define RRRCTL_SETWHEELVELOCITIES 109
#define RRRCTL_SETHEADINGCORRECTIONFACTOR 110
#define RRRCTL_SETFLIPHANDLE 111
#define RRRCTL_SETMAXPWM 112
#define RRRCTL_TURNTO 113
#define RRRCTL_STOPMOTORS 114
#define RRRCTL_SETPWM 115
#define RRRCTL_STOP 119
#define RRRCTL_PAUSE 120
#define RRRCTL_RESUME 121
#define RRRCTL_SENDP1 124
#define RRRCTL_SENDP2 125
#define RRRCTL_SENDP3 126
#define RRRCTL_SENDP4 127
#define RRRCTL_SETBUZZERFREQUENCY 128
#define RRRCTL_SETBUZZERON 129
#define RRRCTL_FOLLOWPATH 130
#define RRRCTL_SETHANDLELENGTH 131
#define RRRCTL_SETMOTORCONTROLCONSTANTS 132
#define RRRCTL_MOVECURVE 133
#define RRRCTL_MOVESTRAIGHT 134
#define RRRCTL_SETMAXSPEED 135
//#define RRRCTL_SENDHEADING 136
//#define RRRCTL_SENDMOTORVELOCITIES 137
#define RRRCTL_PURGERINGBUFFER 138
#define RRRCTL_SETOBSTACLESENSITIVITY 139
#define RRRCTL_MOVETO 140
#define RRRCTL_SETPOSITION 141
#define RRRCTL_SENDPOSITIONDESTINATION 142
#define RRRCTL_LOADPATHSEGMENT 143
#define RRRCTL_SENDP5 144
#define RRRCTL_SENDP6 145
#define RRRCTL_SENDP7 146
#define RRRCTL_SENDP8 147
#define RRRCTL_MOVEDIRECTION 148
#define RRRCTL_SENDINFORMATIONTOTRANSMIT 149
#define RRRCTL_REQUESTSTATE 150
#define RRRCTL_POWERDOWN 151
#define RRRCTL_SENDMESSAGE 152
#define RRRCTL_RETRYLIMIT 153
#define RRRCTL_GETCOMMPORT 154
#define RRRCTL_SETCOMMPORT 155
#define RRRCTL_COMMSTATE 156
#define RRRCTL_RADIOLEVELCHANGE 157
#define RRRCTL_BATTERYLOW 158
#define RRRCTL_POSITIONCHANGE 159
#define RRRCTL_OBSTACLEDETECTED 160
#define RRRCTL_NUDGEDETECTED 161
#define RRRCTL_VELOCITYCHANGE 162
#define RRRCTL_ACCELERATIONCHANGE 163
#define RRRCTL_ENCODERUPDATE 164
#define RRRCTL_ANALOGUPDATE 165
#define RRRCTL_PWMUPDATE 166
#define RRRCTL_RINGBUFFERDUMP 167
#define RRRCTL_NEWPATHSEGMENTREQUEST 168
#define RRRCTL_ROBOTERROR 169
#define RRRCTL_ONHOMEBASE 170
#define RRRCTL_ROBOTSTATUSCHANGE 171
#define RRRCTL_BATTERYCHARGINGSTATECHANGE 172
#define RRRCTL_UNKNOWNMESSAGE 173
#define RRRCTL_COMMSTATECHANGE 174
#define RRRCTL_RIGHTCURRENT 175
#define RRRCTL_LEFTCURRENT 176
#define RRRCTL_LEFTVELOCITY 177
#define RRRCTL_RIGHTVELOCITY 178
#define RRRCTL_TASKRESULT 179
#define RRRCTL_SOFTWAREREVNUMBER 180
#define RRRCTL_ROBOTON 181
#define RRRCTL_ROBOTONTIME 182
#define RRRCTL_EXTERNALINPUT 183
#define RRRCTL_SQUARETOWALL 184
#define RRRCTL_SETFEELINGMOTOR 185
#define RRRCTL_CORNERCALIBRATE 186
#define RRRCTL_FINDHOMEBASE 187
#define RRRCTL_SETHEADINGDESTINATION 188
#define RRRCTL_SETENABLEOBSTACLEBOUNCE 189
#define RRRCTL_SETRAMDATA 190
#define RRRCTL_SETROBOTONSECONDS 191
#define RRRCTL_SETINTERMITTENTBEEP 192
#define RRRCTL_ADDTOPOSITION 193
#define RRRCTL_ADDTOHEADING 194
#define RRRCTL_SETEXTERNALOUTPUT 195
#define RRRCTL_REQUESTEXTERNALINPUT 196
#define RRRCTL_MICROPHONEVOLUME 197

// IRRRPath constants
#define RRRPTH_NUMPOINTS 100
#define RRRPTH_X 101
#define RRRPTH_Y 102


