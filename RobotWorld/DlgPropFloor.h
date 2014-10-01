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
 #if !defined(AFX_DLGPROPFLOOR_H__B6ECAD21_89D1_11D2_B231_0040F6B87672__INCLUDED_)
 #define AFX_DLGPROPFLOOR_H__B6ECAD21_89D1_11D2_B231_0040F6B87672__INCLUDED_
 
 #if _MSC_VER > 1000
 #pragma once
 #endif // _MSC_VER > 1000
 // DlgPropFloor.h : header file
 //
 
 /////////////////////////////////////////////////////////////////////////////
 // CDlgPropFloor dialog
 
 class CRobot;
 
 class CDlgPropFloor : public CPropertyPage
 {
 // Construction
 public:
 	void SetRobot(CRobot* Robot);
 	int GetFloorType(void);
 	void SetFloorType(int Floor);
 	float GetThickSlipPercent(void);
 	float GetNormalSlipPercent(void);
 	void SetThickSlipPercent(float Slip);
 	void SetNormalSlipPercent(float Slip);
 	CDlgPropFloor();   // standard constructor
 
 // Dialog Data
 	//{{AFX_DATA(CDlgPropFloor)
 	enum { IDD = IDD_PROP_FLOOR };
 	CScrollBar	m_scrThickSlipPercent;
 	CScrollBar	m_scrNormalSlipPercent;
 	CString	m_edtNormalSlipPercent;
 	CString	m_edtThickSlipPercent;
 	int		m_FloorType;
 	CString	m_edtApproximateSlipCompensation;
 	//}}AFX_DATA
 
 
 // Overrides
 	// ClassWizard generated virtual function overrides
 	//{{AFX_VIRTUAL(CDlgPropFloor)
 	protected:
 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 	//}}AFX_VIRTUAL
 
 // Implementation
 protected:
 
 	// Generated message map functions
 	//{{AFX_MSG(CDlgPropFloor)
 	afx_msg void OnButSetDefaults();
 	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
 	virtual BOOL OnInitDialog();
 	afx_msg void OnChangeEdtNormalSlipPercent();
 	afx_msg void OnChangeEdtThickSlipPercent();
 	afx_msg void OnButFindSlipCompensation();
 	afx_msg void OnEStop();
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 private:
 	CBitmapButton m_butEStop;
 	int SlipPercentToScrollPos(float Slip);
 	float ScrollPosToSlipPercent(int Pos);
 	CRobot* m_Robot;
 };
 
 //{{AFX_INSERT_LOCATION}}
 // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
 
 #endif // !defined(AFX_DLGPROPFLOOR_H__B6ECAD21_89D1_11D2_B231_0040F6B87672__INCLUDED_)
