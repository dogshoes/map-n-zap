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
#include "Rothmath.h"

class CRobot;
class CCTDPath;

enum TInstructionNodeType {inNoInstruction, inPositionBungeeNode, inPositionVelocityNode, inHeadingNode,
                           inVelocityNode, inStraightVelocityNode, inTurnNode,
                           inCTDPathNode, inWaitForNudge, inBeep, inWait, inWaitLong, inWaitUntil,
                           inPwmNode, inSquareToWall, inFeelMotor, inWallCalibration,
                           inCornerCalibration, inFindHomeBase, inHeadingMotorEnableNode,
                           inWaitForInputChange, inWaitForInputHigh, inWaitForInputLow,
                           inWaitForInputGreater, inWaitForInputLess, inTune, inWaitForPing
                          };

class CInstructionListNode
{
    public:
        virtual void SendInstruction(CRobot* Robot);
        virtual bool ReachedEndOfInstruction(CRobot* Robot);
        bool InstructionTimedOut();
        void Pause(void);
        void Resume(void);
        void SetTimeOut(unsigned long Milliseconds)
        {
            TimeOut = Milliseconds;
        };
        virtual bool HasTimeOut(void)
        {
            return TimeOut != 0;
        };
        void SetStartTime(void);
        char cParm0;
        short sParm0;
        double Heading2;
        robPOINT Position2;
        CTime m_WaitUntilTime;
        TInstructionNodeType InstructionNodeType;
        CInstructionListNode* Next;
        CCTDPath* CTDPath;
        robPOINT Position;
        double Heading;
        double Zone;
        short M0Velocity;
        short M1Velocity;
        short MaxSpeed;
        short TurnDiameter;
        double Parm0;
        double Parm1;
        bool bParm0;
        CInstructionListNode();
        ~CInstructionListNode();
    private:
        bool InPositionZone(CRobot* Robot);
        unsigned long TimeOut;
        unsigned int StartTime;
};

struct TNote
{
    short m_Frequency;
    int m_Duration;
};

typedef CList<TNote, TNote> TNotesList;

class CTuneInstruction: public CInstructionListNode
{
    public:
        void SendNextNotes(CRobot* Robot);
        void SendInstruction(CRobot* Robot);
        bool ReachedEndOfInstruction(CRobot* Robot);
        ~CTuneInstruction();
        CTuneInstruction(short Frequencies[], int Durations[], int NumberOfNotes);
        CTuneInstruction();
    private:
        CList<TNote, TNote> m_Notes;
        POSITION m_CurrentPosition;
};