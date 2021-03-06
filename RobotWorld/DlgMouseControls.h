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
#if !defined(AFX_DLGMOUSECONTROLS_H__E06568A3_CAC7_11D1_B231_0040F6B87672__INCLUDED_)
#define AFX_DLGMOUSECONTROLS_H__E06568A3_CAC7_11D1_B231_0040F6B87672__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgMouseControls.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMouseControls dialog

class CDlgMouseControls : public CPropertyPage
{
        DECLARE_DYNCREATE(CDlgMouseControls)
        // Construction
    public:
        void GetMotorControlConstants(char& Kp, char& Ki, char& Kd, char& Kb, char& Kpwm);
        short GetHandleLength(void);
        bool GetFlipHandle(void);
        void SetHandleLength(short HandleLength);
        void SetFlipHandle(bool FlipHandle);
        void SetMotorControlConstants(char Kp, char Ki, char Kd, char Kb, char Kpwm);
        CDlgMouseControls();   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CDlgMouseControls)
        enum { IDD = IDD_MOUSE_CONTROLS };
        short	m_edtKb;
        short	m_edtKd;
        short	m_edtKi;
        short	m_edtKp;
        short	m_edtKPwm;
        short	m_edtHandleLength;
        BOOL	m_chkFlipHandle;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDlgMouseControls)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CDlgMouseControls)
        afx_msg void OnButSendMotoConDefaults();
        afx_msg void OnChkFlipHandle();
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMOUSECONTROLS_H__E06568A3_CAC7_11D1_B231_0040F6B87672__INCLUDED_)
