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
 #if !defined(AFX_WAIT_H__5AD7A6A3_715B_11D1_AC8C_0040F6B8AE22__INCLUDED_)
 #define AFX_WAIT_H__5AD7A6A3_715B_11D1_AC8C_0040F6B8AE22__INCLUDED_
 
 #if _MSC_VER >= 1000
 #pragma once
 #endif // _MSC_VER >= 1000
 // Wait.h : header file
 // 
 
 /////////////////////////////////////////////////////////////////////////////
 // CWait dialog
 #include "Instruction.h"
 
 enum TWaitCondition {wcWaitNudge, wcWaitUntil, wcWaitFor, wcWaitUntilInputChanges, wcWaitUntilInputIsHigh,
 	wcWaitUntilInputIsLow, wcWaitUntilInputGreater, wcWaitUntilInputLess};
 
 class CWait : public CInstruction
 {
 	DECLARE_DYNAMIC( CWait )
 // Construction
 public:
 //#define dWAIT_FOR 2
 //#define dWAIT_UNTIL 1
 //#define dWAIT_NUDGE 0
 
 	TWaitCondition GetWaitCondition(void);
 #ifdef ROBOT_WORLD_PROJECT
 	virtual TTaskResult Execute(CRobot * Robot, CExecutableRWGraphicObjectList * ExecutableObjectList, bool & result);
 #endif
 	void Enable (BOOL enabled);
 	void ConfigureFromFile (TWaitCondition WaitCondition, CString first, CString second);
 	CString ToFile();
 	int m_OrigionalChoiceWidth;
 	void ShowBorder (BOOL b);
 	void MakeExpanded();
 	void MakeCondensed();
 	CString ToString();
 	CWait(CWnd* pParent = NULL, int index=1);   // standard constructor
 
 // Dialog Data
 	//{{AFX_DATA(CWait)
 	enum { IDD = IDD_WAIT };
 	CStatic	m_staTriggerUnits;
 	CEdit	m_edtTriggerLevel;
 	CComboBox	m_comWaitUntilInput;
 	CEdit	m_WaitHour;
 	CEdit	m_WaitMin;
 	CSpinButtonCtrl	m_ChangeTime;
 	CComboBox	m_TimeLength;
 	CStatic	m_DisabledImage;
 	CStatic	m_BlackBorder;
 	CStatic	m_EtchedBorder;
 	CStatic	m_Image;
 	CComboBox	m_TimeVar;
 	CComboBox	m_WaitCondition;
 	CComboBox	m_am_pm;
 	float	m_TriggerLevel;
 	//}}AFX_DATA
 
 
 // Overrides
 	// ClassWizard generated virtual function overrides
 	//{{AFX_VIRTUAL(CWait)
 	protected:
 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 	//}}AFX_VIRTUAL
 
 // Implementation
 protected:
 	void AdjustDisplay(TWaitCondition m);
 
 	// Generated message map functions
 	//{{AFX_MSG(CWait)
 	virtual void OnOK();
 	virtual void OnCancel();
 	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
 	virtual BOOL OnInitDialog();
 	afx_msg void OnPaint();
 	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
 	afx_msg void OnSelchangeAmPm();
 	afx_msg void OnSelchangeTimeVar();
 	afx_msg void OnSelchangeTimeLength();
 	afx_msg void OnSelchangeWaitCondition();
 	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
 	afx_msg void OnSetfocusWaitHour();
 	afx_msg void OnSetfocusWaitMin();
 	afx_msg void OnUpdateWaitMin();
 	afx_msg void OnUpdateWaitHour();
 	afx_msg void OnSelchangeWaitInput();
 	afx_msg void OnChangeInputTrigger();
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 private:
 	int m_CurrentEditBox;
 	int m_ChangeTimePos;
 };
 
 //{{AFX_INSERT_LOCATION}}
 // Microsoft Developer Studio will insert additional declarations immediately before the previous line.
 
 #endif // !defined(AFX_WAIT_H__5AD7A6A3_715B_11D1_AC8C_0040F6B8AE22__INCLUDED_)
