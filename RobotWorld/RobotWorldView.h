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
 #include "DlgMotorControls.h"
 #include "Robot.h"	// Added by ClassView
 #include "PropGeneral.h"	// Added by ClassView
 //#include "DlgCalibrateSurface.h"
 #include "DlgCreateCalibrationWall1.h"
 //#include "DlgCreateCalibrationDoorway1.h"
 //#include "DlgPositionError.h"
 #include "RothMath.h"	// Added by ClassView
 #include "DlgJoystickControl.h"
 #include "DlgCreateVacuumPatch.h"
 #include "DlgMouseControls.h"
 #include "DlgPropFloor.h"
 
 //#include "DlgDrawingTools.h"
 #include "PropRobotColor.h"	// Added by ClassView
 
 // RobotWorldView.h : interface of the CRobotWorldView class
 //
 /////////////////////////////////////////////////////////////////////////////
 typedef enum {mmDefault, mmPlaceLabel, mmPlaceRedZone, mmErase, mmEdit,
 	mmPlaceVacuumPatch, mmDrawLine, mmDrawFreehand, mmDrawRectangle,
 	mmPlaceFinishLine, mmPlaceStartLine, mmPlaceLinePath, mmSearchArea, mmMoveRobot} TMouseMode;
 
 typedef enum {pmClear, pmObstacle, pmUnknown} TPenMode;
 
 class CLaygo;
 class CFlowZap;
 class CRedZoneList;
 class CRobotWorldDoc;
 class CDlgTestRobot;
 class CCornerWall;
 class CHomeBase;
 class CLinePath;
 class CInstructionListNode;
 class CDlgVelocityTorqueDisplay;
 class CDlgCyeSample;
 class CRWGraphicObject;
 class CExecutableRWGraphicObject;
 class CDlgAddHomeBase;
 
 class CRobotWorldView : public CView
 {
 protected: // create from serialization only
 	CRobotWorldView();
 //	DECLARE_DYNCREATE(CRobotWorldView)
 	DECLARE_SERIAL(CRobotWorldView)
 // Attributes
 public:
 	typedef enum {csCyeServe, csCyeTruck, csCyePup, csCyeClock, csCyePost, csCyeGuide, csCyePatrol} TCyeSample;
 	void LoadCyeSample(TCyeSample Sample);
 	TPenMode GetPenMode(void) {return m_PenMode;};
 	void SetPenMode(TPenMode Mode);
 	enum TPenSize {psSmall, psMedium, psLarge, psXLarge};
 	CRobotWorldDoc* GetDocument();
 // Operations
    // RRRCom class event callback functions
 public:
 	void OnBatteryLow();
 	CFlowZap* m_DlgFlowZap;
 	void SetScrollBars(void);
 	CDlgMotorControls m_DlgMotorControls;
 	void OnZoomView(void);
 	CPoint RobotToClientCoords(robPOINT point);
 	robPOINT ClientToRobotCoords(CPoint point);
 // Overrides
 	// ClassWizard generated virtual function overrides
 	//{{AFX_VIRTUAL(CRobotWorldView)
 	public:
 	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
 	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
 	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
 	virtual BOOL DestroyWindow();
 	virtual void OnInitialUpdate();
 	protected:
 	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
 	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
 	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
 	//}}AFX_VIRTUAL
 
 // Implementation
 public:
 	void SetRobotExternalInput(float Voltage);
 	void SetAddingHomeBase(bool Adding);
 	void AddHomeBase();
 	void SetLearnRoom(bool Learn);
 	void SetFlipHandle(bool FlipHandle);
 	void SetMaxSpeed(short Speed);
 	void OnBatteryVoltageUpdate(float BatteryVoltage, CRobot* Robot);
 	void OnBatteryChargingStateChange(int State, CRobot* Robot);
 	void OnRadioLevelChange(float RadioLevel, CRobot* Robot);
 	void SetRobotColor(int RobotColor);
 	CRobot* GetRobot(short Index);
 	void AddRobot();
 	void OnMotorVelocityUpdate(short RightVelocity, short LeftVelocity, CRobot* Robot);
 	void OnMotorCurrentUpdate(float RightMotor, float LeftMotor, CRobot* Robot);
 	void OnCommunicationsEstablished();
 	CRect CalculateUpdateRect(robPOINT OldRobotPosition, robPOINT OldRobotError, robPOINT NewRobotPosition, robPOINT NewRobotError);
 	void SetActiveRobot(CRobot* Robot);
 	void SetRobotExternalOutput(bool On);
 	bool GetRobotExternalOutput(void);
 	void OnMicrophoneVolume(short Volume);
 	int GetRobotColor(void);
 //	void SetRobotColor(int RobotColor);
 	void OnRobotCommClaps(int NumberOfClaps);
 	bool GetAlreadyCalibratedFloorType(TFloorType FloorType);
 	CCalibrationZone* GetSelectedCalibrationZone(void);
 	void AdjustSelectedCalibrationZone(void);
 	bool SafeAppShutdown(void);
 	bool GetPlay(void);
 	bool GetPause(void);
 	CRobot* GetActiveRobot(void);
 	void DeleteSelectedGraphicObject(void);
 	void EditSelectedGraphicObject(void);
 	CRWGraphicObject* GetSelectedGraphicObject(void);
 	void RedrawRobot(robPOINT Position, double Heading);
 	void OnRobotCommExternalInput(float Voltage);
 	void FindComPort(void);
 	bool GetCommunicationsEstablished(void);
 	void SetCurrentRWGraphicObject(CRWGraphicObject* CurrentGraphicObject) {m_CurrentRWGraphicObject = CurrentGraphicObject;};
 	CRWGraphicObject* GetCurrentRWGraphicObject(void) {return m_CurrentRWGraphicObject;};
 	void SetShowRulerGrid(bool View);
 	bool GetShowRulerGrid(void);
 	int GetLaygoTip(void);
 	void ShowLaygoTip(int State, bool SaveState);
 	void OnRobotCommunicationEstablished(void);
 	bool GetShowVelocityTorqueDisplay(void);
 	void SetShowVelocityTorqueDisplay(bool Show);
 	void SetStatusBarText(CString Text);
 	void OnFlowZapChangeFileOpen(CString OpenFile);
 	void FlowZapStop(void);
 	void FlowZapPause(void);
 	void FlowZapPlay(void);
 	bool SetFlowZapFile(CString FileSelected);
 	void SetVacuumAttached(bool attached);
 	bool GetVacuumAttached(void);
 	void OnViewFlowZap(void);
 	double m_JoystickOrientation;
 	float m_JoystickTurnGain;
 	float m_JoystickResponsiveness;
 	TJoystickControlType m_JoystickControlType;
 	void OnNewDocument(void);
 	CHomeBase* m_HomeBase;
 	bool CreateHomebase(double Heading, bool CornerLeft, TFloorType FloorType);
 	HBITMAP GetLargeRobotBitmap(void);
 	bool GetEnableLaygo(void);
 	void SetEnableLaygo(bool Enable);
 	void ResetLaygo(void);
 	void DoModalPropertySheet(void);
 	bool GetDrawingToolsVisible(void);
 	void SetPenSize(TPenSize Size);
 	TPenSize GetPenSize(void) {return m_PenSize;};
 	void SetDrawingToolsVisible(bool Visible);
 	CLaygo * m_DlgLaygo;
 	bool GetTestRobotDialogVisible(void);
 	void SetTestRobotDialogVisible(bool Visible);
 	void OnObjectAdded(CString Caption, TRWGraphicObjectType ObjectType);
 	void OnObjectErased(CString Caption, TRWGraphicObjectType ObjectType);
 	void OnObjectCaptionChanged(CString OldCaption, CString NewCaption, TRWGraphicObjectType ObjectType);
 	BOOL m_ViewVacuumPatches;
 	void ContextMenu(CWnd* pwnd, CPoint point);
 	CDlgJoystickControl m_DlgJoystickControl;
 	RECT RobotToClientCoords(robRECT rect);
 	robRECT ClientToRobotCoords(CRect rect);
 	void SetMouseMode(TMouseMode MouseMode);
 	TMouseMode GetMouseMode(void);
 	void Serialize(CArchive& ar);
 	void CenterRobot(void);
 	CRedZoneList* m_RedZoneList;
 	CProbabilityGrid* m_ProbabilityGrid;
 	virtual ~CRobotWorldView();
 	void DrawFoundPath(CDC* pdc, Path* path, bool ShowRobotBounds = false);
 	void DrawCTDPath(CDC* pdc, CCTDPath * path, bool ShowRobotBounds = false);
 #ifdef _DEBUG
 	virtual void AssertValid() const;
 	virtual void Dump(CDumpContext& dc) const;
 #endif
 
 protected:
 
 // Generated message map functions
 protected:
 	CRWGraphicObject* m_CurrentRWGraphicObject;
 	void New(void);
 	bool m_DragLine;
 	short GetPenWidth(void);
 	bool m_MouseOverObject;
 	afx_msg LRESULT OnRobotFindPathFinished(WPARAM, LPARAM output);
 	afx_msg LRESULT OnRobotShowFindPath(WPARAM, LPARAM path);
 	afx_msg LRESULT OnRobotFindPathStage(WPARAM state, LPARAM blah);
 	afx_msg LRESULT OnRobotChangeState(WPARAM State, LPARAM);
 	afx_msg LRESULT OnRobotFindPathError(WPARAM ErrorNumber, LPARAM);
 	afx_msg void OnContextMenu(CWnd*, CPoint point);
 	//{{AFX_MSG(CRobotWorldView)
 	afx_msg void OnGoHome();
 	afx_msg void OnTimer(UINT nIDEvent);
 	afx_msg void OnSize(UINT nType, int cx, int cy);
 	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
 	afx_msg void OnStop();
 	afx_msg void OnSearchArea();
 	afx_msg void OnCreateCalibrationWall();
 	afx_msg void OnCreateCalibrationDoorway();
 	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
 	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
 	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
 	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
 	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
 	afx_msg void OnDestroy();
 	afx_msg void OnPowerOff();
 	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
 	afx_msg void OnViewCyePersona();
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 	DECLARE_EVENTSINK_MAP()
 private:
 	void SetEnableDancing(bool Enable);
 	CDlgAddHomeBase *m_DlgAddHomeBase;
 	CTime m_LastClapTime;
 	bool m_FollowingPath;
 	int m_MusicPlaying;
 	int m_NumberOfClaps;
 	robPOINT m_RobotDestination;
 	CDialog* m_DlgLowRadioLevel;
 	void DisplayLowRadioLevelDialog(void);
 	void AlignPointsWithProbabilityGridSquares(CRect& Rect);
 
 	CExecutableRWGraphicObject* m_LinePathFrom;
 	CExecutableRWGraphicObject* m_LinePathTo;
 	robPOINT m_OldBungeeDestination;
 	robPOINT m_OldBungeeOrigin;
 	double m_OldBungeeHeading;
 	bool m_OldBungeeOn;
 	bool m_ReceivedNewPosition;
 //	robPOINT m_DrawRobotPosition;
 //	double m_DrawRobotHeading;
 	bool m_GoingHome;
 	CDlgCyeSample* m_DlgCyeSample;
 	int m_LaygoNumberClicks;
 	CDlgVelocityTorqueDisplay* m_DlgVelocityTorqueDisplay;
 	TPenMode m_PenMode;
 	bool m_OnSlipperyFloor;
 	CDialog m_DlgBatteryLow;
 	CCTDPath* m_CurrentRobotCTDPath;
 	CCTDPath* m_OldRobotCTDPath;
 	void CreateFlowZap(void);
 	bool m_LMouseButtonDown;
 	CPropGeneral m_DlgPropGeneral;
 	CDlgPropFloor m_DlgPropFloor;
 	bool m_CurrentLinePathEnded;
 	CLinePath* m_CurrentLinePath;
 	void SketchRobotError(robPOINT* Corners, robPOINT DPosition, CDC* dc);
 	CPropRobotColor m_DlgPropRobotColor;
 	CPropertySheet m_DlgPropertySheet;
 	TPenSize m_PenSize;
 	BOOL ShowTip(int state);
 	CDlgTestRobot* m_DlgTestRobot;
 	CDlgMouseControls m_DlgMouseControls;
 	robPOINT m_PreDragRobotPosition;
 	Path* m_FindPathShown;
 	CDlgCreateVacuumPatch m_DlgCreateVacuumPatch;
 	TFrameSide m_DragFrameSide;
 	CRWGraphicObject* m_SelectedGraphicObject;
 	CRect m_OldDragRect;
 	bool m_DraggingRect;
 	int m_DragLineWidth;
 	CRect m_DragRect;
 	POINT m_DragPoint2;
 	POINT m_DragPoint1;
 	BOOL OverRobot(robPOINT WorldClick, POINT ScreenClick);
 	UINT m_OldVScrollPos;
 	UINT m_OldHScrollPos;
 	CDlgCreateCalibrationWall1* m_DlgCreateCalibrationWall;
 	void OrderSearchAreaCorners(long* SearchAreaCorners);
 	void MakeYSearchPath(long* SearchAreaCorners, unsigned char NearCorner, long XSearchLevel);
 	void MakeXSearchPath(long* SearchAreaCorners, unsigned char NearCorner, long YSearchLevel);
 	void StartSearchArea(void);
 	void ShrinkSearchAreaToZero(void);
 	void ObstacleHandler(short RobotNumber, short M0, short M1);
 	void PlaceNewLabel(POINT WorldPoint);
 	TMouseMode m_MouseMode;
 };
 
 #ifndef _DEBUG  // debug version in RobotWorldView.cpp
 inline CRobotWorldDoc* CRobotWorldView::GetDocument()
    { return (CRobotWorldDoc*)m_pDocument; }
 #endif
 
 /////////////////////////////////////////////////////////////////////////////
