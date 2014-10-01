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
 // RobotWorld.h : main header file for the ROBOTWORLD application
 //
 
 #ifndef __AFXWIN_H__
 	#error include 'stdafx.h' before including this file for PCH
 #endif
 
 #include "RobotWorld Resource.h"       // main symbols
 #include "SplashWnd.h"						//Splash Window class
 
 /////////////////////////////////////////////////////////////////////////////
 // CRobotWorldApp:
 // See RobotWorld.cpp for the implementation of this class
 //
 
 class CRobotWorldApp : public CWinApp
 {
 public:
 	bool GetEnableDancing(void);
 	void SetEnableDancing(bool Enable);
 	bool GetRobotAnimate(void);
 	void SetRobotAnimate(bool Enable);
 	int GetDefaultRobotVersion(void);
 	bool GetColorAssociatedAddresses(void);
 	void SetCalibratedThickCarpet(bool Calibrated);
 	void SetCalibratedThinCarpet(bool Calibrated);
 	bool GetCalibratedThickCarpet(void);
 	bool GetCalibratedThinCarpet(void);
 	int GetWoodHeadingCorrectionFactor();
 	int GetWoodDefaultHeadingCorrectionFactor();
 	int GetThinCarpetDefaultHeadingCorrectionFactor(void);
 	int GetThickCarpetDefaultHeadingCorrectionFactor(void);
 	short GetThickCarpetHeadingCorrectionFactor(void);
 	short GetThinCarpetHeadingCorrectionFactor(void);
 	void SetThickCarpetHeadingCorrectionFactor(short HeadingCorrectionFactor);
 	void SetThinCarpetHeadingCorrectionFactor(short HeadingCorrectionFactor);
 	BOOL OnIdle(LONG lCount);
 	CSplashWnd m_WndSplash;
 	DWORD m_dwSplashTime;
 	long GetRobotOnSeconds(void);
 	float GetRobotSoftwareRevision(void);
 	void SetRobotSoftwareRevision(float SoftwareRevision);
 	void SetRobotOnSeconds(long On);
 	CString m_DefaultPath;
 	CView* GetView(void);
 	void SetCurrentFile(CString CurrentFile);
 	CString GetCurrentFile(void);
 //	SetCurrentFile(CString CurrentFile);
 	CRobotWorldApp();
 
 // Overrides
 	// ClassWizard generated virtual function overrides
 	//{{AFX_VIRTUAL(CRobotWorldApp)
 	public:
 	virtual BOOL InitInstance();
 	virtual int ExitInstance();
 	//}}AFX_VIRTUAL
 
 // Implementation
 	COleTemplateServer m_server;
 		// Server object for document creation
 
 	//{{AFX_MSG(CRobotWorldApp)
 	afx_msg void OnAppAbout();
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 protected:
 	CString m_CurrentFile;
 };
 
 
 /////////////////////////////////////////////////////////////////////////////
