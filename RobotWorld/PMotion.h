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
#if !defined(AFX_PMOTION_H__E3137A43_AD80_11D3_AF79_E03650C10000__INCLUDED_)
#define AFX_PMOTION_H__E3137A43_AD80_11D3_AF79_E03650C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PMotion.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPMotion dialog
class CRobot;
class CCyePersona;
class CPSong;

class CPMotion : public CDialog
{
        // Construction
    public:
        CPMotion(CWnd* pParent = NULL);   // standard constructor
        void SetRobot(CRobot*);

        //pointer to main dialog with cyecom ocx
        CCyePersona* m_cyepersona;

        //pointer to song dialog
        CPSong* m_cpsong;


        void Smooth(short Velocity, short Duration);
        void RampUp(short Velocity);
        void RampDown(short Velocity);
        void TurnLeft(short Velocity);
        void TurnRight(short Velocity);
        void TweekRight(short Velocity);
        void TweekLeft(short Velocity);
        void TinyTweek(short Velocity, bool Direction);
        void TurnAround(short Velocity);
        void TurnToAngle(short Velocity, double RefAngle, double SetHeading);
        void MoveForward(short Velocity, short Duration);
        void MoveForwardNS(short Velocity, short Duration);
        void ContForward(short Velocity);
        void ContReverse(short Velocity);
        void MoveBackwards(short Velocity, short Duration);
        void MoveBackwardsNS(short Velocity);
        void Spinn(short Velocity, short Duration, bool Direction);
        void Spinn360();
        void OneWheelSpinn(short Velocity, short Wheel, short Duration, bool Direction, bool Stop);
        void TwoWheelSpinn(short RVelocity, short LVelocity, double Angle, bool Direction, bool Opposite, bool Stop);
        void Staccato(short Velocity, short Duration, short Iteration);
        void ZigZag(short Velocity, short Delay, short Iterations);
        void LeftRight(short Velocity, short Delay, short Iterations);
        void SlowTurns(short Side, double Angle, short Velocity);
        void Snake(short V1, short V2, double Angle1, double Angle2, short Iterations);
        void Meander(short Velocity, short Duration, short Iterations);
        void RandMotion();
        void AbsurdMotion(short Ki, short Kd);

        //helper functions
        bool RWait(short Seconds);
        void SWait(short Seconds);
        void Pause(clock_t wait);
        void IPause(int wait);
        void MoveToWall(short Velocity, double OriginalHeading);
        void Calibrate(double Heading, long X, long Y);
        void ResetMotorConstants();

        // Dialog Data
        //{{AFX_DATA(CPMotion)
        enum { IDD = IDD_CYE_PERSONA_MOTION };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA
        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CPMotion)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        CRobot* m_Robot;
        // Generated message map functions
        //{{AFX_MSG(CPMotion)
        afx_msg void OnTest();
        afx_msg void OnTweekLeft();
        afx_msg void OnTweekRight();
        afx_msg void OnForwardNS();
        afx_msg void OnReverseNS();
        afx_msg void OnTurnLeft();
        afx_msg void OnTurnRight();
        afx_msg void OnTurnAround();
        afx_msg void OnSnake();
        afx_msg void OnMeander();
        afx_msg void OnLeftRight();
        afx_msg void OnZigZag();
        afx_msg void OnAbrupt();
        afx_msg void OnCalibrate();
        afx_msg void OnRamped();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMOTION_H__E3137A43_AD80_11D3_AF79_E03650C10000__INCLUDED_)
