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
 #if !defined(AFX_VACUUM_H__64747D02_8B2D_11D1_AC8C_0040F6B8AE22__INCLUDED_)
 #define AFX_VACUUM_H__64747D02_8B2D_11D1_AC8C_0040F6B8AE22__INCLUDED_
 
 #if _MSC_VER >= 1000
 #pragma once
 #endif // _MSC_VER >= 1000
 // Vacuum.h : header file
 // 
 
 /////////////////////////////////////////////////////////////////////////////
 // CVacuum dialog
 #include "Instruction.h"
 
 class CVacuum : public CInstruction
 {
 	DECLARE_DYNAMIC( CVacuum )
 // Construction
 public:
 #ifdef ROBOT_WORLD_PROJECT
 	virtual TTaskResult Execute(CRobot * Robot, CExecutableRWGraphicObjectList * ExecutableObjectList, bool & result);
 #endif
 	void ConfigureFromFile (CString Caption);
 	void RemovePatch(CString OldCaption);
 	void ChangeVacuumPatch (CString OldCaption, CString NewCaption);
 	CString GetName() { CString Name; m_PatchList.GetWindowText(Name); return Name;};
 	void InsertPatch(CString name);
 	void Enable (BOOL enabled);
 	void InsertVacuumPatches(CStringList *patches);
 	CString ToFile();
 	CString ToString();
 	void ShowBorder(BOOL raised);
 	void MakeExpanded();
 	void MakeCondensed();
 	CVacuum(CWnd* pParent = NULL, int index=0);   // standard constructor
 
 // Dialog Data
 	//{{AFX_DATA(CVacuum)
 	enum { IDD = IDD_VACUUM };
 	CStatic	m_DisabledImage;
 	CStatic	m_Text;
 	CComboBox	m_PatchList;
 	CStatic	m_Image;
 	CStatic	m_EtchedBorder;
 	CStatic	m_BlackBorder;
 	//}}AFX_DATA
 
 
 // Overrides
 	// ClassWizard generated virtual function overrides
 	//{{AFX_VIRTUAL(CVacuum)
 	protected:
 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 	//}}AFX_VIRTUAL
 
 // Implementation
 protected:
 
 	// Generated message map functions
 	//{{AFX_MSG(CVacuum)
 	virtual void OnOK();
 	virtual void OnCancel();
 	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
 	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
 	virtual BOOL OnInitDialog();
 	afx_msg void OnPaint();
 	afx_msg void OnSelchangePatchlist();
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 };
 
 //{{AFX_INSERT_LOCATION}}
 // Microsoft Developer Studio will insert additional declarations immediately before the previous line.
 
 #endif // !defined(AFX_VACUUM_H__64747D02_8B2D_11D1_AC8C_0040F6B8AE22__INCLUDED_)
