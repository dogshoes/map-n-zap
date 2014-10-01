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
 // RobotStatus.h: interface for the CRobotStatus class.
 //
 //////////////////////////////////////////////////////////////////////
 
 #if !defined(AFX_ROBOTSTATUS_H__91AE7F63_BF0C_11D1_B231_0040F6B87672__INCLUDED_)
 #define AFX_ROBOTSTATUS_H__91AE7F63_BF0C_11D1_B231_0040F6B87672__INCLUDED_
 
 #if _MSC_VER >= 1000
 #pragma once
 #endif // _MSC_VER >= 1000
 
 class CDbrRobotStatus : public CDialogBar
 {
 public:
 	virtual BOOL PreTranslateMessage(MSG* pMsg);
 	void SetRadioLevel(float RadioLevel);
 	short SpeedballLocationToMaxSpeed(int SpeedballLocation);
 	CRect m_MaxSpeedRect;
 	void PaintBackground(CDC* pDC);
 	void OnSetBatteryLevel(float BatteryLevel);
 	void OnBatteryChargeStateChange(short BatteryChargeState);
 //	CStatic* GetBatteryChargeState(void) {return (CStatic*)GetDlgItem(IDC_BMP_BATTERY_CHARGE_STATE);};
 //	CButton* GetChkEnableLearnRoom(void) {return (CButton*)GetDlgItem(IDC_CHK_ENABLE_LEARN_ROOM);};
 //	CButton* GetChkAutoCalibrate(void) {return (CButton*)GetDlgItem(IDC_CHK_AUTOCALIBRATE);};
 //	CButton* GetChkVacuumAttached(void) {return (CButton*)GetDlgItem(IDC_CHK_VACUUM_ATTACHED);};
 	void OnSetMaxSpeed(short MaxSpeed);
 	CDbrRobotStatus();
 	virtual ~CDbrRobotStatus();
 protected:
 	//{{AFX_MSG(CDbrRobotStatus)
 	afx_msg void OnPaint();
 	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
 	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
 	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
 	afx_msg void OnSize(UINT nType, int cx, int cy);
 	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 private:
 	CToolTipCtrl m_tooltip;
 	CEdit* GetEdtRadioLevel(void);
 //	CScrollBar* GetScrMaxSpeed(void);
 	int ClientToSpeedballPoint(CPoint point);
 	int m_SpeedballLocation;
 	int m_DraggingOffset;
 	bool m_DraggingMaxSpeed;
 	CDC m_dcDisplayMemory;
 	CBitmap m_bmpBatteryChargeState;
 	CBitmap m_bmpSpeedball;
 	int MaxSpeedToSpeedballLocation(short MaxSpeed);
 };
 
 #endif // !defined(AFX_ROBOTSTATUS_H__91AE7F63_BF0C_11D1_B231_0040F6B87672__INCLUDED_)
