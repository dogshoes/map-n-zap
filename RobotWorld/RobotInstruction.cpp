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
 
  #include "stdafx.h"
  #include "RobotInstruction.h"
  #include "RothTime.h"
  #include "CTDPath.h"
  #include "Robot.h"
  
  CInstructionListNode::CInstructionListNode()
  /*******************************************************************************
  //Created: 10/21/97 S.R.
  //Last Revision: 10/21/97 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	CTDPath = NULL;
  	Next = NULL;
  }
  
  CInstructionListNode::~CInstructionListNode()
  /*******************************************************************************
  //Created: 10/21/97 S.R.
  //Last Revision: 10/21/97 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	if (CTDPath != NULL) { 
  		delete CTDPath;
  	}
  }
  
  void CInstructionListNode::SetStartTime()
  {
  	if (TimeOut != 0) {
  		if (TimeOut < 50000) {
  			StartTime = Time_CurrentTime();
  		} else {
  			unsigned long TotalSeconds = TimeOut / 1000;
  			CTime CurrentTime = CTime::GetCurrentTime();
  			unsigned int Days, Hours, Minutes;
  			Days = TotalSeconds / 86400;
  			TotalSeconds %= 86400;
  			Hours = TotalSeconds / 3600;
  			TotalSeconds %= 3600;
  			Minutes = TotalSeconds / 60;
  			TotalSeconds %= 60;
  
  			CTimeSpan DT(Days, Hours,Minutes,TotalSeconds);
  			CurrentTime.GetHour();
  			CurrentTime.GetMinute();
  			CurrentTime.GetSecond();
  			DT.GetHours();
  			DT.GetMinutes();
  			DT.GetSeconds();
  			CTime WaitTime = CurrentTime + DT;
  
  			m_WaitUntilTime = WaitTime;
  			m_WaitUntilTime.GetHour();
  			m_WaitUntilTime.GetMinute();
  			m_WaitUntilTime.GetSecond(); 
  		}
  	}
  }
  
  bool CInstructionListNode::InstructionTimedOut()
  {
  	bool TimedOut = false;
  	if (TimeOut != 0) {
  		if (TimeOut < 50000) {
  			if (Time_DT(StartTime,Time_CurrentTime()) > TimeOut) {
  				TimedOut = true;
  			}
  		} else {
  			CTimeSpan TimeDiff = m_WaitUntilTime - CTime::GetCurrentTime();
  			if ( (TimeDiff.GetDays() <= 0) && (TimeDiff.GetHours() <= 0) && 
  				(TimeDiff.GetMinutes() <= 0) && (TimeDiff.GetSeconds() <= 0) ) return true;
  
  		}
  	}
  	return TimedOut;
  }
  
  void CInstructionListNode::Pause()
  {
  	if (HasTimeOut()) {
  		if (TimeOut < 50000) {
  			unsigned int CurrentTime = Time_CurrentTime();
  			if (Time_DT(StartTime, CurrentTime) >= TimeOut) {
  				TimeOut = 1;
  			} else {
  				TimeOut -= Time_DT(StartTime,CurrentTime);
  			}
  		} else {
  			CTimeSpan TimeLeft = m_WaitUntilTime - CTime::GetCurrentTime();
  			TimeOut = 1000*(TimeLeft.GetSeconds() + 60*(TimeLeft.GetMinutes() + 60*(TimeLeft.GetHours() + 24*(TimeLeft.GetDays()))));
  			if (TimeOut == 0) TimeOut = 1;
  		}
  //		SetStartTime();
  	}
  }
  
  void CInstructionListNode::Resume()
  {
  	if (HasTimeOut()) {
  		SetStartTime();
  	}
  }
  
  bool CInstructionListNode::InPositionZone(CRobot* Robot)
  /*******************************************************************************
  //Created: 09/05/96 S.R.
  //Last Revision: 09/05/96 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	TRUE:		The robot's handle is within the desired position zone.
  //	FALSE:	It isn't.
  ********************************************************************************
  //Description:
  //		This function determines if the robot's handle is within a circular zone
  //	of a point in the robot world.  The zone and end point are taken from the
  //	robot's path list.
  //Method:
  //		First the function calculates the distance between the desired position and
  //	the end of the robot's handle.  This has to be done separately for the two
  //	cases where FlipHandle is true and false.  If this distance is within the
  //	zone then TRUE is returned.  Otherwise, FALSE is returned.
  *******************************************************************************/
  {
  	short HandleLength;
  	if (InstructionNodeType == inCTDPathNode) {
  		HandleLength = cFollowCTDPathHandleLength;
  	} else {
  		HandleLength = Robot->GetHandleLength();
  	}
  
  	double distance;
  	if (!Robot->GetFlipHandle()) {
  		distance = SQR(Robot->m_Position.x + HandleLength*cos(Robot->m_Heading) - Position.x);
  		distance += SQR(Robot->m_Position.y + HandleLength*sin(Robot->m_Heading) - Position.y);
  		if ( distance <= SQR(Zone) ) return TRUE;
  	} else {
  		double V = (Position.x - Robot->m_Position.x) * cos(Robot->m_Heading);
  		V += (Position.y - Robot->m_Position.y) * sin(Robot->m_Heading);
  		if (V >= 0) {
  			distance = SQR(Robot->m_Position.x + HandleLength*cos(Robot->m_Heading) - Position.x);
  			distance += SQR(Robot->m_Position.y + HandleLength*sin(Robot->m_Heading) - Position.y);
  			if ( distance <= SQR(Zone) ) return TRUE;
  		} else {
  			distance = SQR(Robot->m_Position.x - HandleLength*cos(Robot->m_Heading) - Position.x);
  			distance += SQR(Robot->m_Position.y - HandleLength*sin(Robot->m_Heading) - Position.y);
  			if ( distance <= SQR(Zone) ) return TRUE;
  		}
  	}
  	return FALSE;
  }
  
  bool CInstructionListNode::ReachedEndOfInstruction(CRobot* Robot)
  {
  	CTimeSpan TimeDiff;
  	switch (InstructionNodeType) {
  	case inPositionBungeeNode:
  	case inPositionVelocityNode:
  		if ( InPositionZone(Robot) ) {
  			return true;
  		} else return false;
  		break;
  	case inCTDPathNode:
  		if (CTDPath->m_PathPosition >= CTDPath->m_Size - 20) {
  			if ( InPositionZone(Robot) ) {
  				return true;
  			} else return false;
  		} else {
  			return false;
  		}
  		break;
  	case inHeadingNode:
  	case inHeadingMotorEnableNode:
  	case inTurnNode:
  		if (AbsAngularSeparation(Robot->GetHeading(), Heading) < Zone) {
  			return true;
  		} else return false;
  		break;
  	case inVelocityNode:
  	case inStraightVelocityNode:
  	case inPwmNode:
  	case inFeelMotor:
  		break;
  	case inWaitForNudge:
  		if (Robot->m_Nudged) {
  			Robot->m_Nudged = NDG_None;
  			return true;
  		}
  		break;
  	case inBeep:
  	case inWait:
  		break;
  	case inWaitUntil:
  //		case inWaitLong:
  		TimeDiff = m_WaitUntilTime - CTime::GetCurrentTime();
  		if ( (TimeDiff.GetDays() <= 0) && (TimeDiff.GetHours() <= 0) && 
  			(TimeDiff.GetMinutes() <= 0) && (TimeDiff.GetSeconds() <= 0) ) return true;
  		break;
  	case inSquareToWall:
  	case inWallCalibration:
  	case inCornerCalibration:
  	case inFindHomeBase:
  	case inWaitForInputChange:
  	case inWaitForInputHigh:
  	case inWaitForInputLow:
  	case inWaitForInputGreater:
  	case inWaitForInputLess:
  	case inWaitForPing:
  		if (Robot->GetInstructionResultValid()) {
  			Robot->SetInstructionResultValid(false);
  			return true;
  		}
  		break;
  	default:
  		ASSERT(false);
  		break;
  	}
  	return false;
  }
  
  void CInstructionListNode::SendInstruction(CRobot* Robot)
  {
  	Robot->SetExecutingRobotTask(false);
  	Robot->SetInstructionResultValid(false);
  	if (InstructionNodeType == inCTDPathNode) {
  		if (Robot->GetShowCurrentPath()) {
  			Robot->SetFollowingPath(CTDPath);
  		}
  	} else {
  		Robot->SetFollowingPath(NULL);
  	}
  	switch (InstructionNodeType) {
  	case inCTDPathNode:
  		Robot->SetFlipHandle(bParm0);
  		Robot->StartFollowingCTDPath(CTDPath);
  		break;
  	case inPositionBungeeNode:
  		Robot->SetFlipHandle(bParm0);
  		Robot->SetPositionPwmDestination(Position);
  		break;
  	case inPositionVelocityNode:
  		Robot->SetFlipHandle(bParm0);
  		Robot->SetPositionVelocityDestination(Position, MaxSpeed);
  		break;
  	case inHeadingNode:
  		Robot->SetHeadingDestination(Heading, MaxSpeed);
  //			Robot->m_RobotComm->SendHeadingDestination(Head->Heading);
  		break;
  	case inHeadingMotorEnableNode:
  		Robot->SetHeadingDestination(Heading, MaxSpeed, sParm0);
  		break;
  	case inVelocityNode:
  		Robot->SetVelocities(M0Velocity, M1Velocity);
  		break;
  	case inPwmNode:
  		Robot->SetPwm(M0Velocity, M1Velocity);
  		break;
  	case inStraightVelocityNode:
  		Robot->SetStraightVelocity(MaxSpeed, Heading);
  		break;
  	case inTurnNode:
  		Robot->SetTurn(Heading, MaxSpeed, TurnDiameter);
  //			Robot->m_RobotComm->SendTurn(Heading, MaxSpeed, TurnDiameter);
  		break;
  	case inWaitForNudge:
  	case inWaitUntil:
  //		case inWaitLong:
  	case inWait:
  		Robot->m_Nudged = NDG_None;
  		Robot->Stop();
  //			Robot->m_RobotComm->SendStopMotors();
  		break;
  	case inBeep:
  		Robot->Stop();
  //			Robot->m_RobotComm->SendStopMotors();
  		Robot->SetBuzzerFrequency(sParm0);
  		break;
  	case inSquareToWall:
  		Robot->SetExecutingRobotTask(true);
  		Robot->SendSquareToWall(Heading, bParm0);
  		break;
  /*		case inWallCalibration:
  		Robot->m_ExecutingRobotTask = true;
  		SendWallCalibrate(Position, Heading, Position2, Heading2, bParm0, cParm0);
  		break;
  */		
  	case inCornerCalibration:
  		Robot->SetExecutingRobotTask(true);
  		Robot->SendCornerCalibrate(Position, Heading, cParm0, bParm0, sParm0);
  		break;
  	case inFindHomeBase:
  		Robot->SetExecutingRobotTask(true);
  		Robot->SendFindHomeBase(Heading, Heading2, Position, bParm0);
  		break;
  	case inFeelMotor:
  		Robot->SetFeelingMotor(cParm0);
  		break;
  	case inWaitForInputChange:
  		Robot->StartWaitingForExternalInputChange();
  	case inWaitForInputHigh:
  	case inWaitForInputLow:
  	case inWaitForInputGreater:
  	case inWaitForInputLess:
  		Robot->SetExecutingRobotTask(true);
  		Robot->RequestExternalInput();
  		break;
  	case inWaitForPing:
  		Robot->SendPing();
  		break;
  	default:
  		ASSERT(FALSE);
  		break;
  	}
  }
  
  CTuneInstruction::CTuneInstruction()
  {
  	InstructionNodeType = inTune;
  }
  
  CTuneInstruction::CTuneInstruction(short Frequencies[], int Durations[], int NumberOfNotes)
  {
  	InstructionNodeType = inTune;
  	ASSERT(NumberOfNotes != 0);
  	ASSERT(Frequencies != NULL);
  	ASSERT(Durations != NULL);
  	if (NumberOfNotes == 0) return;
  	if (Frequencies == NULL) return;
  	if (Durations == NULL) return;
  
  	TNote Note;
  	for (int i = 0; i < NumberOfNotes; i++) {
  		Note.m_Frequency = Frequencies[i];
  		Note.m_Duration = Durations[i];
  		m_Notes.AddTail(Note);
  	}
  }
  
  CTuneInstruction::~CTuneInstruction()
  {
  }
  
  bool CTuneInstruction::ReachedEndOfInstruction(CRobot* Robot)
  {
  	bool ReachedEnd = false;
  	if (Robot->GetInstructionResultValid()) {
  		Robot->SetInstructionResultValid(false);
  		ReachedEnd = true;
  	}
  	return ReachedEnd;
  }
  
  void CTuneInstruction::SendInstruction(CRobot* Robot)
  {
    int i;
  	short Frequency[5];
  	int Duration[5];
  	int NumberOfNotesSent = m_Notes.GetCount();
  	if (NumberOfNotesSent > 5) NumberOfNotesSent = 5;
  	m_CurrentPosition = m_Notes.GetHeadPosition();
  	for (i = 0; i < NumberOfNotesSent; i++) {
  		TNote Note = m_Notes.GetNext(m_CurrentPosition);
  		Frequency[i] = Note.m_Frequency;
  		Duration[i] = Note.m_Duration;
  	}
  	Robot->SetExecutingRobotTask(true);
  	Robot->SendTune(Frequency, Duration, NumberOfNotesSent, true, false);
  	SendNextNotes(Robot);
  	for (i = 0; (i < 5) && (m_CurrentPosition != NULL); i++) {
  		TNote Note = m_Notes.GetNext(m_CurrentPosition);
  		Frequency[i] = Note.m_Frequency;
  		Duration[i] = Note.m_Duration;
  	}
  	Robot->SendTune(Frequency, Duration, i, false, true);
  }
  
  void CTuneInstruction::SendNextNotes(CRobot* Robot)
  {
  	short Frequency[5];
  	int Duration[5];
  	int NumberOfNotesSent;
  	for (NumberOfNotesSent = 0; ( (m_CurrentPosition != NULL) && (NumberOfNotesSent < 5) ); NumberOfNotesSent++) {
  		TNote Note = m_Notes.GetNext(m_CurrentPosition);
  		Frequency[NumberOfNotesSent] = Note.m_Frequency;
  		Duration[NumberOfNotesSent] = Note.m_Duration;
  	}
  	if (NumberOfNotesSent != 0) {
  		Robot->SendTune(Frequency, Duration, NumberOfNotesSent);
  	}
  }
