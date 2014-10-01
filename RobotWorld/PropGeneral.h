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
 #if !defined(AFX_PROPGENERAL_H__A71CB103_4D55_11D2_B231_0040F6B87672__INCLUDED_)
 #define AFX_PROPGENERAL_H__A71CB103_4D55_11D2_B231_0040F6B87672__INCLUDED_
 
 #if _MSC_VER > 1000
 #pragma once
 #endif // _MSC_VER > 1000
 // PropGeneral.h : header file
 //
 
 /////////////////////////////////////////////////////////////////////////////
 // CPropGeneral dialog
 class CRobot;
 
 class CPropGeneral : public CPropertyPage
 {
 	DECLARE_DYNCREATE(CPropGeneral)
 
 // Construction
 public:
 	bool GetEnableDancing(void);
 	void SetEnableDancing(bool Enable);
 	void SetAnimateRobot(bool Animate);
 	bool GetAnimateRobot(void);
 	CRobot* m_Robot;
 	bool GetLearnRoom(void);
 	void SetLearnRoom(bool On);
 	bool GetFlipHandle(void);
 	void SetFlipHandle(bool FlipHandle);
 	bool GetIntermittentBeep(void);
 	void SetIntermittentBeep(bool On);
 	bool GetShowCalibrationResults(void);
 	void SetShowCalibrationResults(bool value);
 	CPropGeneral();
 	~CPropGeneral();
 
 // Dialog Data
 	//{{AFX_DATA(CPropGeneral)
 	enum { IDD = IDD_PROP_GENERAL };
 	BOOL	m_chkShowCalibrationResults;
 	BOOL	m_chkIntermittentBeep;
 	BOOL	m_chkFlipHandle;
 	BOOL	m_chkShowVelocityTorqueDialog;
 	BOOL	m_chkLearnRoom;
 	BOOL	m_chkAnimateRobot;
 	BOOL	m_chkEnableDancing;
 	//}}AFX_DATA
 
 
 // Overrides
 	// ClassWizard generate virtual function overrides
 	//{{AFX_VIRTUAL(CPropGeneral)
 	protected:
 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 	//}}AFX_VIRTUAL
 
 // Implementation
 protected:
 	// Generated message map functions
 	//{{AFX_MSG(CPropGeneral)
 	afx_msg void OnButPowerOff();
 	afx_msg void OnButRobotTest();
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 
 };
 
 //{{AFX_INSERT_LOCATION}}
 // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
 
 #endif // !defined(AFX_PROPGENERAL_H__A71CB103_4D55_11D2_B231_0040F6B87672__INCLUDED_)
