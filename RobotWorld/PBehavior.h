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
#if !defined(AFX_PBEHAVIOR_H__E3137A44_AD80_11D3_AF79_E03650C10000__INCLUDED_)
#define AFX_PBEHAVIOR_H__E3137A44_AD80_11D3_AF79_E03650C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PBehavior.h : header file
//
extern short Vn;
extern short Vs;
extern short Vf;
extern short Vm;
extern short Vc;
/////////////////////////////////////////////////////////////////////////////
// CPBehavior dialog

class CCyePersona;
class CRobot;
class CPSong;
class CPMotion;

class CPBehavior : public CDialog
{
        // Construction
    public:
        CPBehavior(CWnd* pParent = NULL);   // standard constructor
        void SetRobot(CRobot*);


        CCyePersona* m_cyepersona;
        CPMotion* m_cpmotion;
        CPSong* m_cpsong;

        void SetSong(CPSong*);
        void SetMotion(CPMotion*);

        //Behaviors
        void Impatient1();
        void Impatient2();
        void Impatient3();
        void Content1();
        void Content2();
        void Content3();
        void Content4();
        void Agressive1();
        void Agressive2();
        void Mad1();
        void Jittery1();
        void Jittery2();
        void Quirky1();
        void Quirky();
        void SpinnSway();
        void Wiggle();
        void HotSpinn();
        void Spiral(short EndVelocity, double TurnAngle);
        void Square();
        void Eight();
        void Cross();
        void Nudge();
        void Test();


        // Dialog Data
        //{{AFX_DATA(CPBehavior)
        enum { IDD = IDD_CYE_PERSONA_BEHAVIOR };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CPBehavior)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        CRobot* m_Robot;
        // Generated message map functions
        //{{AFX_MSG(CPBehavior)
        afx_msg void OnTest();
        afx_msg void OnAgressive1();
        afx_msg void OnAgressive2();
        afx_msg void OnBackHome();
        afx_msg void OnQuirky1();
        afx_msg void OnQuirky2();
        afx_msg void OnWiggle();
        afx_msg void OnJittery1();
        afx_msg void OnJittery2();
        afx_msg void OnMad1();
        afx_msg void OnImpatient1();
        afx_msg void OnImpatient2();
        afx_msg void OnImpatient3();
        afx_msg void OnContent1();
        afx_msg void OnContent2();
        afx_msg void OnContent3();
        afx_msg void OnContent4();
        afx_msg void OnSpinnSway();
        afx_msg void OnCross();
        afx_msg void OnEight();
        afx_msg void OnSpinn();
        afx_msg void OnSpiral();
        afx_msg void OnSquare();
        afx_msg void OnNudge();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PBEHAVIOR_H__E3137A44_AD80_11D3_AF79_E03650C10000__INCLUDED_)
