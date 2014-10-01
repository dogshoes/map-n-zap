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
 #if !defined(AFX_CYEPERSONA_H__3021C5C5_ACC9_11D3_A5DF_00105A98F145__INCLUDED_)
 #define AFX_CYEPERSONA_H__3021C5C5_ACC9_11D3_A5DF_00105A98F145__INCLUDED_
 
 #if _MSC_VER > 1000
 #pragma once
 #endif // _MSC_VER > 1000
 // CyePersona.h : header file
 //
 
 #include "PMotion.h"
 #include "PSong.h"
 #include "PBehavior.h"
 
 /////////////////////////////////////////////////////////////////////////////
 // CCyePersona dialog
 class CRobot;
 
 class CCyePersona : public CDialog
 {
 // Construction
 public:
 	void SetOnHomeBase(bool OnHome);
 	void SetRobot(CRobot* Robot);
 	CCyePersona(CWnd* pParent = NULL);   // standard constructor
 
 	//to access other dialogs
 	CPSong m_PSongDialog;
 	CPMotion m_PMotionDialog;
 	CPBehavior m_PBehaviorDialog;
 
 	//to access other dialog functions
 	CPMotion* m_cpmotion;
 	CPSong* m_cpsong;
 	
 	//helper functions
 	CString GetQualityTime();
 	bool IsOddNumber(short Number);
 	short GetRandomNumber(short MaxNumber);
 
 // Dialog Data
 	//{{AFX_DATA(CCyePersona)
 	enum { IDD = IDD_CYE_PERSONA };
 		// NOTE: the ClassWizard will add data members here
 	//}}AFX_DATA
 
 
 // Overrides
 	// ClassWizard generated virtual function overrides
 	//{{AFX_VIRTUAL(CCyePersona)
 	protected:
 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 	//}}AFX_VIRTUAL
 
 // Implementation
 protected:
 	CRobot* m_Robot;
 	// Generated message map functions
 	//{{AFX_MSG(CCyePersona)
 	afx_msg void OnShowSong();
 	afx_msg void OnShowMotion();
 	afx_msg void OnShowBehavior();
 	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 };
 
 //{{AFX_INSERT_LOCATION}}
 // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
 
 #endif // !defined(AFX_CYEPERSONA_H__3021C5C5_ACC9_11D3_A5DF_00105A98F145__INCLUDED_)
