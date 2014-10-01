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
 //{{AFX_INCLUDES()
 #include "richtext.h"
 #include "DlgFindHomebaseCorner.h"	// Added by ClassView
 #include "DlgOrientHomebase.h"	// Added by ClassView
 #include "DlgHomeBaseFloorType.h"
 //}}AFX_INCLUDES
 #if !defined(AFX_LAYGO_H__BBED0064_CE1D_11D1_AC8D_0040F6B8AE22__INCLUDED_)
 #define AFX_LAYGO_H__BBED0064_CE1D_11D1_AC8D_0040F6B8AE22__INCLUDED_
 
 #if _MSC_VER >= 1000
 #pragma once
 #endif // _MSC_VER >= 1000
 // Laygo.h : header file
 //
 
 /////////////////////////////////////////////////////////////////////////////
 // CLaygo dialog
 class CRobotWorldView;
 class CPropRobotColor;
 class CLaygo : public CDialog
 {
 // Construction
 public:
 	~CLaygo();
 	HBITMAP LoadBitmap(CString BitmapFilename);
 	bool HomeBaseCreateFloorTypeAlreadyCalibrated(void);
 	CString GetTipBaseName(int key);
 	void LoadTipText(CString FileName);
 	void CreateHomeBase(void);
 	void SaveState(void);
 	void LoadState(void);
 	void SetRobotWorldView(CRobotWorldView* RobotWorldView);
 	void SetRobotBitmap(HBITMAP hbmRobot);
 	void SetCurrentState(int State);
 	bool m_EnableLaygo;
 	bool GetShownJoystick(void) {return m_ShownJoystick;};
 	void SetShownJoystick(bool Shown) {m_ShownJoystick = Shown;};
 	bool GetShownFlowZap(void) {return m_ShownFlowZap;};
 	void SetShownFlowZap(bool Shown) {m_ShownFlowZap = Shown;};
 	enum {lgoWelcome = 1, lgoChooseRobotColor, lgoComPortSearch, lgoHomeBaseCreateState, lgoNavigating, lgoOccupancyGrid,
 		lgoPositionUncertainty, lgoCheckPoint, lgoAdditionalCheckPoints,
 		lgoHotPoints, lgoVacuumPaths, lgoDangerZones, lgoFlowZap,
 		cNonlinearLaygoes, lgoJoystick = 99, lgoHomeBaseCreate2State = 98};
 	virtual BOOL PreTranslateMessage(MSG* pMsg);
 	BOOL ShowTip();
 	int GetCurrentState();
 	void ResetLaygoTips();
 	void Configure (int key, bool SaveState = true);
 //	CRichEditCtrl * m_InfoViewerCtrl;
 	CLaygo(CWnd* pParent = NULL);   // standard constructor
 
 // Dialog Data
 	//{{AFX_DATA(CLaygo)
 	enum { IDD = IDD_LAYGO };
 	CStatic	m_bmpLaygoImage;
 	CButton	m_OKButton;
 	CButton	m_CancelButton;
 	CButton	m_DontShow;
 	CRichText	m_InfoViewerCtrl;
 	//}}AFX_DATA
 
 
 // Overrides
 	// ClassWizard generated virtual function overrides
 	//{{AFX_VIRTUAL(CLaygo)
 	protected:
 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 	//}}AFX_VIRTUAL
 
 // Implementation
 protected:
 	CToolTipCtrl m_tooltip;
 
 	// Generated message map functions
 	//{{AFX_MSG(CLaygo)
 	virtual BOOL OnInitDialog();
 	virtual void OnCancel();
 	virtual void OnOK();
 	afx_msg void OnDontshow();
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 private:
 	CPropRobotColor* m_dlgChooseRobotColor;
 	void GetTipFileNames(int key, CString& TipFileName, CString& BitmapFileName);
 	int m_CurrentShowing;
 	CRobotWorldView* m_RobotWorldView;
 	CDlgOrientHomebase m_dlgOrientHomebase;
 	HBITMAP m_hbmRobot;
 	CDlgFindHomebaseCorner m_dlgHomebaseCorner;
 	CDlgHomeBaseFloorType m_dlgHomebaseFloorType;
 	bool m_ShownJoystick;
 	bool m_ShownFlowZap;
 	BOOL m_ShowTip;
 	int m_CurrentState;
 	int GetHomeBaseCreateFloorType();
 };
 
 //{{AFX_INSERT_LOCATION}}
 // Microsoft Developer Studio will insert additional declarations immediately before the previous line.
 
 #endif // !defined(AFX_LAYGO_H__BBED0064_CE1D_11D1_AC8D_0040F6B8AE22__INCLUDED_)
