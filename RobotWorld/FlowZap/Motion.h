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
 #if !defined(AFX_MOTION_H__4B0BFC62_B438_11D1_AC8C_0040F6B8AE22__INCLUDED_)
 #define AFX_MOTION_H__4B0BFC62_B438_11D1_AC8C_0040F6B8AE22__INCLUDED_
 
 #if _MSC_VER >= 1000
 #pragma once
 #endif // _MSC_VER >= 1000
 // Motion.h : header file 
 //
 
 /////////////////////////////////////////////////////////////////////////////
 // CMotion dialog
 #include "Instruction.h"
 
 class CMotion : public CInstruction
 {
 	DECLARE_DYNAMIC (CMotion)
 // Construction
 public:
 #ifdef ROBOT_WORLD_PROJECT
 	virtual TTaskResult Execute(CRobot* Robot, CExecutableRWGraphicObjectList* ExecutableObjectList, bool& result);
 #endif
 	void ConfigureFromFile ( BOOL, BOOL, CString, CString, CString, CString);
 	CString ToFile();
 	CString ToString();
 	void ShowBorder(BOOL b);
 	void MakeExpanded();
 	void MakeCondensed();
 	void Enable(BOOL enabled);
 	CMotion(CWnd* pParent = NULL, int index = 0);   // standard constructor
 
 // Dialog Data
 	//{{AFX_DATA(CMotion)
 	enum { IDD = IDD_MOTION };
 	CStatic	m_TextRotate;
 	CStatic	m_TextOClock;
 	CStatic	m_TextGo;
 	CComboBox	m_Direction;
 	CComboBox	m_Units;
 	CButton	m_Rotate;
 	CButton	m_Motion;
 	CStatic	m_Image;
 	CStatic	m_DisabledImage;
 	CStatic	m_EtchedBorder;
 	CEdit	m_Distance;
 	CComboBox	m_Degrees;
 	CStatic	m_BlackBorder;
 	//}}AFX_DATA
 
 
 // Overrides
 	// ClassWizard generated virtual function overrides
 	//{{AFX_VIRTUAL(CMotion)
 	protected:
 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 	//}}AFX_VIRTUAL
 
 // Implementation
 protected:
 
 	// Generated message map functions
 	//{{AFX_MSG(CMotion)
 	virtual void OnOK();
 	virtual void OnCancel();
 	virtual BOOL OnInitDialog();
 	afx_msg void OnPaint();
 	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
 	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
 	afx_msg void OnClickMotion();
 	afx_msg void OnClickRotate();
 	afx_msg void OnSelchangeDegrees();
 	afx_msg void OnSelchangeDirection();
 	afx_msg void OnChangeDistance();
 	afx_msg void OnSelchangeUnits();
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 private:
 	void EnableMotionControls(BOOL enabled);
 	void EnableRotationControls(BOOL enabled);
 #ifdef ROBOT_WORLD_PROJECT
 	double GetTravelDistance(void);
 	double GetRotationAngle(void);
 #endif
 };
 
 //{{AFX_INSERT_LOCATION}}
 // Microsoft Developer Studio will insert additional declarations immediately before the previous line.
 
 #endif // !defined(AFX_MOTION_H__4B0BFC62_B438_11D1_AC8C_0040F6B8AE22__INCLUDED_)
