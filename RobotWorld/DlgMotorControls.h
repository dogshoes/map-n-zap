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
 // DlgMotorControls.h : header file
 //
 #ifndef DLGMOTORCONTROLS29383946ukkhdfglksjfn09238kjdf982q35kdfv
 #define DLGMOTORCONTROLS29383946ukkhdfglksjfn09238kjdf982q35kdfv
 /////////////////////////////////////////////////////////////////////////////
 // CDlgMotorControls dialog
 
 #include "Calibration Zone.h"
 #include "robot.h"
 #include "rothmath.h"
 class CDlgMotorControls : public CDialog
 {
 // Construction
 public:
 	UINT m_OldVScrollPos;
 	CView* m_ParentView;
 	 ~CDlgMotorControls();
 	long m_M1Encoder;
 	long m_M0Encoder;
 	BOOL GetFlipHandle(void);
 	CExecutableRWGraphicObjectList* m_CalibrationZoneHandler;
 	long m_CurrentHeading;
 	CRobot* m_Robot;
 	CDlgMotorControls(CWnd* pParent = NULL);   // standard constructor
 
 // Dialog Data
 	//{{AFX_DATA(CDlgMotorControls)
 	enum { IDD = IDD_MOTORCONTROLS };
 	CString	m_edtTotalEncoderTicks;
 	int	m_edtM0VelMeas;
 	int	m_edtM1Acceleration;
 	int	m_edtM1VelMeas;
 	CString	m_edtObstacle;
 	BOOL	m_chkRecordInformation;
 	int	m_edtM1PwmMeas;
 	long	m_edtH;
 	long	m_edtX;
 	long	m_edtY;
 	long	m_edtM1Encoder;
 	long	m_edtM0Encoder;
 	double	m_edtCurrentHeading;
 	int		m_edtM0Acceleration;
 	int		m_edtM0PwmMeas;
 	double	m_edtKHeadErr;
 	CString	m_edtM0Pwm;
 	CString	m_edtM0Speed;
 	CString	m_edtMaxSpeed;
 	CString	m_edtM1Pwm;
 	CString	m_edtM1Speed;
 	CString	m_edtHeading;
 	CString	m_edtStraightHeading;
 	BYTE	m_edtKb;
 	BYTE	m_edtKd;
 	BYTE	m_edtKi;
 	BYTE	m_edtKp;
 	BYTE	m_edtKPwm;
 	short	m_edtHandleLength;
 	UINT	m_BuzzerFrequency;
 	long	m_edtP1;
 	long	m_edtP2;
 	long	m_edtP3;
 	long	m_edtP4;
 	CString	m_edtCorrection;
 	CString	m_edtRepeat;
 	CString	m_edtStartX;
 	CString	m_edtStartY;
 	CString	m_edtEndX;
 	CString	m_edtEndY;
 	CString	m_edtIterations;
 	CString	m_edtEndDist;
 	CString	m_edtFreeDist;
 	CString	m_edtMinFound;
 	CString	m_edtStartDist;
 	BOOL	m_chkStopPath;
 	CString	m_edtAvgFound;
 	CString	m_edtFindCalZoneX;
 	CString	m_edtFindCalZoneY;
 	double	m_edtBatteryLevel;
 	CString	m_edtVelocityDirectionHeading;
 	CString	m_edtVelocityDirectionVelocity;
 	BOOL	m_chkFlipHandle;
 	BOOL	m_chkSendAD;
 	BOOL	m_chkSendPosition;
 	BOOL	m_chkSendVelocity;
 	BOOL	m_chkSendAccel;
 	long	m_edtP8;
 	long	m_edtP5;
 	long	m_edtP6;
 	long	m_edtP7;
 	float	m_edtM0AD;
 	float	m_edtM1AD;
 	int		m_edtPathBuffer;
 	CString	m_edtM0Obstacle;
 	CString	m_edtM1Obstacle;
 	float	m_edtAnalogInput;
 	BOOL	m_chkRobotAnimation;
 	CString	m_edtOutput;
 	int		m_edtMicrophoneVolume;
 	//}}AFX_DATA
 
 
 // Overrides
 	// ClassWizard generated virtual function overrides
 	//{{AFX_VIRTUAL(CDlgMotorControls)
 	protected:
 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 	//}}AFX_VIRTUAL
 
 // Implementation
 protected:
 
 	// Generated message map functions
 	//{{AFX_MSG(CDlgMotorControls)
 	afx_msg void OnButSetPWM();
 	afx_msg void OnButSetHeading();
 	afx_msg void OnButStopMotors();
 	afx_msg void OnChkFlipHandle();
 	afx_msg void OnButSetSpeed();
 	afx_msg void OnButShowHeading();
 	afx_msg void OnButton1();
 	afx_msg void OnButCalibrateHeading();
 	afx_msg void OnButDo360();
 	afx_msg void OnButResetPosition();
 	afx_msg void OnButCreateDoorway();
 	afx_msg void OnButcreateWall();
 	afx_msg void OnButCalibrate();
 	afx_msg void OnButSendDefaultCorrection();
 	afx_msg void OnButSetMaxSpeed();
 	afx_msg void OnButSetStraightHeading();
 	afx_msg void OnButPurgeRingBuffer();
 	afx_msg void OnObstacleHighSensitivity();
 	afx_msg void OnObstacleLowSensitivity();
 	afx_msg void OnObstacleMediumSensitivity();
 	afx_msg void OnObstacleNone();
 	afx_msg void OnButShowPosition();
 	afx_msg void OnButNoCorrection();
 	afx_msg void OnButSendMotorControlConstants();
 	afx_msg void OnButSendMotoConDefaults();
 	afx_msg void OnButFollowPath();
 	afx_msg void OnChkRecordInfo();
 	afx_msg void OnButSendBuzzer();
 	afx_msg void OnButSet();
 	afx_msg void OnButParmDefault();
 	afx_msg void OnButSendCorrection();
 	afx_msg void OnButWriteProbGrid();
 	afx_msg void OnButFindPath();
 	afx_msg void OnButFindCalZone();
 	virtual void OnOK();
 	afx_msg void OnButSetVelocityDirection();
 	afx_msg void OnButCreateCalWall();
 	afx_msg void OnButSendValues();
 	afx_msg void OnButTestMotors();
 	afx_msg void OnButOn();
 	afx_msg void OnButSetPathBuffer();
 	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
 	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
 	virtual BOOL OnInitDialog();
 	afx_msg void OnButRequestAnalogInput();
 	afx_msg void OnChkRobotAnimation();
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 private:
 	CPoint m_OriginalSize;
 	UINT m_OldHScrollPos;
 	unsigned char m_PathPosition;
 	robPOINT* m_Path;
 	void DoMessages(void);
 };
 
 #endif
