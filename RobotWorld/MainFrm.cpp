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
  // MainFrm.cpp : implementation of the CMainFrame class
  //
  
  /*******************************************************************************
  //Created: 02/04/97 S.R.
  //Last Revision: 02/04/97 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  
  #include "stdafx.h"
  #include "RobotWorld Resource.h"
  #include "MainFrm.h"
  
  #include "RobotWorld.h"
  #include "RobotWorldDoc.h"
  #include "RobotWorldView.h"
  #include "Probability Grid.h"
  #include "FlowZap.h"
  #include "Red Zone.h"
  #include "RRDim.h"
  #include "RothUtil.h"
  #include "Laygo.h"
  
  //#include "Label.h"

  #define MAIN_FRAME_SECTION "MAIN_FRAME_DIM"
  /////////////////////////////////////////////////////////////////////////////
  // CMainFrame
  
  IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
  
  BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
  	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TIME, OnUpdateTime)
  	//{{AFX_MSG_MAP(CMainFrame)
  	ON_WM_CREATE()
  	ON_COMMAND(ID_VIEW_ROBOTCONTROLS, OnViewRobotControls)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_ROBOTCONTROLS, OnUpdateViewRobotControls)
  	ON_COMMAND(ID_VIEW_ROBOTSTATUS, OnViewRobotStatus)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_ROBOTSTATUS, OnUpdateViewRobotStatus)
  	ON_COMMAND(ID_PLACELABEL, OnPlaceLabel)
  	ON_COMMAND(ID_VIEW_MOTORCONTROLS, OnViewMotorControls)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_MOTORCONTROLS, OnUpdateViewMotorControls)
  	ON_WM_CLOSE()
  	ON_COMMAND(ID_ZOOM100, OnZoom100)
  	ON_UPDATE_COMMAND_UI(ID_ZOOM100, OnUpdateZoom100)
  	ON_COMMAND(ID_ZOOM200, OnZoom200)
  	ON_UPDATE_COMMAND_UI(ID_ZOOM200, OnUpdateZoom200)
  	ON_COMMAND(ID_ZOOM400, OnZoom400)
  	ON_UPDATE_COMMAND_UI(ID_ZOOM400, OnUpdateZoom400)
  	ON_COMMAND(ID_VIEW_LABELS, OnViewLabels)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_LABELS, OnUpdateViewLabels)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_REDZONES, OnUpdateViewRedZones)
  	ON_COMMAND(ID_CENTERROBOT, OnCenterRobot)
  	ON_COMMAND(ID_VIEW_REDZONES, OnViewRedZones)
  	ON_WM_PALETTECHANGED()
  	ON_WM_QUERYNEWPALETTE()
  	ON_COMMAND(ID_VIEW_VACUUM_PATCHES, OnViewVacuumPatches)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_VACUUM_PATCHES, OnUpdateViewVacuumPatches)
  	ON_COMMAND(ID_MM_BLOCK_GRID, OnMMBlockGrid)
  	ON_COMMAND(ID_MM_CLEAR_GRID, OnMMClearGrid)
  	ON_COMMAND(ID_MM_UNKNOWN_GRID, OnMMUnknownGrid)
  	ON_COMMAND(ID_MMDEFAULT, OnMMDefault)
  	ON_COMMAND(ID_MMERASE, OnMMErase)
  	ON_COMMAND(ID_MMMOVE, OnMMMove)
  	ON_COMMAND(ID_BLOCKAREA, OnBlockArea)
  	ON_COMMAND(ID_CALIBRATESURFACE, OnCalibrateSurface)
  	ON_UPDATE_COMMAND_UI(ID_CALIBRATESURFACE, OnUpdateRobotControls)
  	ON_WM_HSCROLL()
  	ON_COMMAND(ID_VIEW_TEST_ROBOT, OnViewTestRobot)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_TEST_ROBOT, OnUpdateViewTestRobot)
  	ON_UPDATE_COMMAND_UI(ID_BLOCKAREA, OnUpdateBlockArea)
  	ON_UPDATE_COMMAND_UI(ID_MM_BLOCK_GRID, OnUpdateMMBlockGrid)
  	ON_UPDATE_COMMAND_UI(ID_MM_CLEAR_GRID, OnUpdateMMClearGrid)
  	ON_UPDATE_COMMAND_UI(ID_MM_UNKNOWN_GRID, OnUpdateMMUnknownGrid)
  	ON_UPDATE_COMMAND_UI(ID_MMDEFAULT, OnUpdateMMDefault)
  	ON_UPDATE_COMMAND_UI(ID_MMERASE, OnUpdateMMErase)
  	ON_UPDATE_COMMAND_UI(ID_MMMOVE, OnUpdateMMMove)
  	ON_UPDATE_COMMAND_UI(ID_PLACELABEL, OnUpdatePlaceLabel)
  	ON_COMMAND(ID_VACUUM, OnVacuum)
  	ON_UPDATE_COMMAND_UI(ID_VACUUM, OnUpdateVacuum)
  	ON_COMMAND(ID_VIEW_POSITION_SPOTS, OnViewPositionSpots)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_POSITION_SPOTS, OnUpdateViewPositionSpots)
  	ON_COMMAND(ID_VIEW_DRAWING_TOOLS, OnViewDrawingTools)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_DRAWING_TOOLS, OnUpdateViewDrawingTools)
  	ON_COMMAND(ID_MM_PLACE_FINISH_LINE, OnMmPlaceFinishLine)
  	ON_UPDATE_COMMAND_UI(ID_MM_PLACE_FINISH_LINE, OnUpdateMmPlaceFinishLine)
  	ON_COMMAND(ID_VIEW_SETTINGS, OnViewSettings)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_SETTINGS, OnUpdateViewSettings)
  	ON_COMMAND(ID_HELP_RESETLAYGO, OnHelpResetlaygo)
  	ON_COMMAND(ID_HELP_DISABLELAYGO, OnHelpDisablelaygo)
  	ON_UPDATE_COMMAND_UI(ID_HELP_DISABLELAYGO, OnUpdateHelpDisablelaygo)
  	ON_COMMAND(ID_LINE_PATH, OnLinePath)
  	ON_UPDATE_COMMAND_UI(ID_LINE_PATH, OnUpdateLinePath)
  	ON_UPDATE_COMMAND_UI(ID_POWER_OFF, OnUpdatePowerOff)
  	ON_UPDATE_COMMAND_UI(ID_SEARCHAREA, OnUpdateSearchArea)
  	ON_CBN_DROPDOWN(ID_COM_FLOWZAP_FILE, OnFzcDropdownComFlowzapFile)
  	ON_CBN_SELCHANGE(ID_COM_FLOWZAP_FILE, OnFzcSelchangeComFlowzapFile)
  	ON_COMMAND(ID_LOAD_TOOLBARS, OnLoadToolbars)
  	ON_COMMAND(ID_SAVE_TOOLBARS, OnSaveToolbars)
  	ON_COMMAND(ID_DRAW_LINE, OnDrawLine)
  	ON_COMMAND(ID_DRAW_RECTANGLE, OnDrawRectangle)
  	ON_BN_CLICKED(ID_DRAW_PEN_SIZE, OnDrawPenSize)
  	ON_BN_CLICKED(ID_DRAW_PEN_STYLE, OnDrawPenStyle)
  	ON_COMMAND(ID_DRAW_FREEHAND, OnDrawFreehand)
  	ON_UPDATE_COMMAND_UI(ID_DRAW_FREEHAND, OnUpdateDrawFreehand)
  	ON_UPDATE_COMMAND_UI(ID_DRAW_LINE, OnUpdateDrawLine)
  	ON_UPDATE_COMMAND_UI(ID_DRAW_RECTANGLE, OnUpdateDrawRectangle)
  	ON_COMMAND(ID_MMMOVE_ROBOT, OnMMMoveRobot)
  	ON_UPDATE_COMMAND_UI(ID_MMMOVE_ROBOT, OnUpdateMMMoveRobot)
  	ON_WM_PAINT()
  	ON_COMMAND(ID_VIEW_MAP_OBJECTS_TOOLBAR, OnViewMapObjectsToolbar)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_OBJECTS_TOOLBAR, OnUpdateViewMapObjectsToolbar)
  	ON_COMMAND(ID_VIEW_ZAP_CONTROLS, OnViewZapControls)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_ZAP_CONTROLS, OnUpdateViewZapControls)
  	ON_COMMAND(ID_VIEW_FLOWZAP, OnViewFlowZap)
  	ON_COMMAND(ID_VIEW_INSTRUMENT_PANEL, OnViewInstrumentPanel)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_INSTRUMENT_PANEL, OnUpdateViewInstrumentPanel)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_WELCOME_TIP, OnHelpLaygoShowWelcomeTip)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_ROBOT_COLOR_TIP, OnHelpLaygoShowRobotColorTip)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_COM_PORT_SEARCH_TIP, OnHelpLaygoShowComPortSearchTip)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_CREATE_HOMEBASE_TIP, OnHelpLaygoShowCreateHomeBaseTip)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_NAVIGATE_TIP, OnHelpLaygoShowNavigateTip)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_OCCUPANCY_GRID_TIP, OnHelpLaygoShowOccupancyGridTip)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_POSITION_UNCERTAINTY_TIP, OnHelpLaygoShowPositionUncertaintyTip)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_CHECKPOINT_TIP, OnHelpLaygoShowCheckPointTip)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_ADDITIONAL_CHECKPOINT_TIP, OnHelpLaygoShowAdditionalCheckPointTip)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_HOTPOINT_TIP, OnHelpLaygoShowHotPointTip)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_VACUUMPATHS_TIP, OnHelpLaygoShowVacuumPathsTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_WELCOME_TIP, OnUpdateHelpLaygoShowWelcomeTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_ROBOT_COLOR_TIP, OnUpdateHelpLaygoShowRobotColorTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_COM_PORT_SEARCH_TIP, OnUpdateHelpLaygoShowComPortSearchTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_CREATE_HOMEBASE_TIP, OnUpdateHelpLaygoShowCreateHomeBaseTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_NAVIGATE_TIP, OnUpdateHelpLaygoShowNavigateTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_OCCUPANCY_GRID_TIP, OnUpdateHelpLaygoShowOccupancyGridTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_POSITION_UNCERTAINTY_TIP, OnUpdateHelpLaygoShowPositionUncertaintyTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_CHECKPOINT_TIP, OnUpdateHelpLaygoShowCheckPointTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_ADDITIONAL_CHECKPOINT_TIP, OnUpdateHelpLaygoShowAdditionalCheckPointTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_HOTPOINT_TIP, OnUpdateHelpLaygoShowHotPointTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_VACUUMPATHS_TIP, OnUpdateHelpLaygoShowVacuumPathsTip)
  	ON_COMMAND(ID_VIEW_LINE_PATHS, OnViewLinePaths)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_LINE_PATHS, OnUpdateViewLinePaths)
  	ON_COMMAND(ID_HELP_SAMPLES_CYECLOCK, OnHelpSamplesCyeclock)
  	ON_COMMAND(ID_HELP_SAMPLES_CYEGUIDE, OnHelpSamplesCyeguide)
  	ON_COMMAND(ID_HELP_SAMPLES_CYEPATROL, OnHelpSamplesCyepatrol)
  	ON_COMMAND(ID_HELP_SAMPLES_CYEPOST, OnHelpSamplesCyepost)
  	ON_COMMAND(ID_HELP_SAMPLES_CYEPUP, OnHelpSamplesCyepup)
  	ON_COMMAND(ID_HELP_SAMPLES_CYESERVE, OnHelpSamplesCyeserve)
  	ON_COMMAND(ID_HELP_SAMPLES_CYETRUCK, OnHelpSamplesCyetruck)
  	ON_UPDATE_COMMAND_UI(ID_HELP_SAMPLES_CYECLOCK, OnUpdateHelpSamplesCyeclock)
  	ON_UPDATE_COMMAND_UI(ID_HELP_SAMPLES_CYESERVE, OnUpdateHelpSamplesCyeserve)
  	ON_UPDATE_COMMAND_UI(ID_HELP_SAMPLES_CYEPATROL, OnUpdateHelpSamplesCyepatrol)
  	ON_WM_DESTROY()
  	ON_COMMAND(ID_VIEW_RULER_GRID, OnViewRulerGrid)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER_GRID, OnUpdateViewRulerGrid)
  	ON_COMMAND(ID_MM_PLACE_START_LINE, OnMmPlaceStartLine)
  	ON_UPDATE_COMMAND_UI(ID_MM_PLACE_START_LINE, OnUpdateMmPlaceStartLine)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_DANGER_ZONES_TIP, OnHelpLaygoShowDangerZonesTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_DANGER_ZONES_TIP, OnUpdateHelpLaygoShowDangerZonesTip)
  	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
  	ON_UPDATE_COMMAND_UI(ID_EDIT_PROPERTIES, OnUpdateEditProperties)
  	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
  	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
  	ON_COMMAND(ID_DRAW_PEN_CLEAR, OnDrawPenClear)
  	ON_UPDATE_COMMAND_UI(ID_DRAW_PEN_CLEAR, OnUpdateDrawPenClear)
  	ON_COMMAND(ID_DRAW_PEN_OBSTACLE, OnDrawPenObstacle)
  	ON_UPDATE_COMMAND_UI(ID_DRAW_PEN_OBSTACLE, OnUpdateDrawPenObstacle)
  	ON_COMMAND(ID_DRAW_PEN_UNKNOWN, OnDrawPenUnknown)
  	ON_UPDATE_COMMAND_UI(ID_DRAW_PEN_UNKNOWN, OnUpdateDrawPenUnknown)
  	ON_COMMAND(ID_DRAW_PEN_WIDTH1, OnDrawPenWidth1)
  	ON_UPDATE_COMMAND_UI(ID_DRAW_PEN_WIDTH1, OnUpdateDrawPenWidth1)
  	ON_COMMAND(ID_DRAW_PEN_WIDTH2, OnDrawPenWidth2)
  	ON_UPDATE_COMMAND_UI(ID_DRAW_PEN_WIDTH2, OnUpdateDrawPenWidth2)
  	ON_COMMAND(ID_DRAW_PEN_WIDTH3, OnDrawPenWidth3)
  	ON_UPDATE_COMMAND_UI(ID_DRAW_PEN_WIDTH3, OnUpdateDrawPenWidth3)
  	ON_COMMAND(ID_DRAW_PEN_WIDTH4, OnDrawPenWidth4)
  	ON_UPDATE_COMMAND_UI(ID_DRAW_PEN_WIDTH4, OnUpdateDrawPenWidth4)
  	ON_COMMAND(ID_ADJUST_CHECKPOINT, OnAdjustCheckPoint)
  	ON_UPDATE_COMMAND_UI(ID_ADJUST_CHECKPOINT, OnUpdateAdjustCheckPoint)
  	ON_COMMAND(ID_HELP_LAYGO_SHOW_FLOWZAP_TIP, OnHelpLaygoShowFlowzapTip)
  	ON_UPDATE_COMMAND_UI(ID_HELP_LAYGO_SHOW_FLOWZAP_TIP, OnUpdateHelpLaygoShowFlowzapTip)
  	ON_COMMAND(ID_EXTERNAL_OUTPUT_ON, OnExternalOutputOn)
  	ON_UPDATE_COMMAND_UI(ID_EXTERNAL_OUTPUT_ON, OnUpdateExternalOutputOn)
  	ON_COMMAND(ID_EXTERNAL_OUTPUT_OFF, OnExternalOutputOff)
  	ON_UPDATE_COMMAND_UI(ID_EXTERNAL_OUTPUT_OFF, OnUpdateExternalOutputOff)
  	ON_COMMAND(ID_VIEW_ADD_ROBOT, OnViewAddRobot)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_ADD_ROBOT, OnUpdateViewAddRobot)
  	ON_COMMAND(ID_VIEW_ADD_HOMEBASE, OnViewAddHomebase)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_ADD_HOMEBASE, OnUpdateViewAddHomebase)
  	ON_UPDATE_COMMAND_UI(ID_CREATECALIBRATIONDOORWAY, OnUpdateRobotControls)
  	ON_UPDATE_COMMAND_UI(ID_CREATECALIBRATIONWALL, OnUpdateRobotControls)
  	ON_UPDATE_COMMAND_UI(ID_GOHOME, OnUpdateRobotControls)
  	ON_COMMAND(ID_SHOW_VIDEO_BUTTON, OnShowVideoButton)
  	//}}AFX_MSG_MAP
  	// Global help commands
  	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
  	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
  	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
  	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
  	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_NEW, ID_FILE_SEND_MAIL, OnUpdateFileMenu)
  	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, OnUpdateFileMRUMenu)
  END_MESSAGE_MAP()
  
  static UINT indicators[] =
  {
  	ID_SEPARATOR,           // status line indicator
  	ID_INDICATOR_CAPS,
  	ID_INDICATOR_NUM,
  	ID_INDICATOR_SCRL,
  };
  
  /////////////////////////////////////////////////////////////////////////////
  // CMainFrame construction/destruction
  
  CMainFrame::CMainFrame()
  {
  	m_DlgCalibrateSurface = NULL;
  	m_FirstShowing = true;
  }
  
  CMainFrame::~CMainFrame()
  {
  	delete m_DlgCalibrateSurface;
  }
  
  int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
  /*******************************************************************************
  //Created: 07/28/96 S.R.
  //Last Revision: 07/30/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
  		return -1;
  //	RECT FrameRect;
  //	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
  //	int ScreenY = GetSystemMetrics(SM_CYSCREEN);
  //	GetWindowRect(&FrameRect);
  //	SetWindowPos( NULL, ScreenX - (FrameRect.right - FrameRect.left), FrameRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
  
  /*Show the splash screen*/
  /*	CRobotWorldApp* App = (CRobotWorldApp*)AfxGetApp();
  	App->m_WndSplash.Create(CSplashWnd::IDD, this);
  	App->m_WndSplash.ShowWindow(SW_SHOW);
  	App->m_dwSplashTime = ::GetCurrentTime();
  */
  /*Do all the tool bar, status bar stuff*/
  /*Create the generic file/edit/etc. tool bar*/
  	if (!m_wndToolBar.Create(this) ||
  		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
  	{
  		TRACE0("Failed to create toolbar\n");
  		return -1;      // fail to create
  	}
  	m_wndToolBar.SetWindowText("Standard Tool Bar");
  	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
  		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
  /*Create the status bar*/
  	if (!m_wndStatusBar.Create(this) ||
  		!m_wndStatusBar.SetIndicators(indicators,
  		  sizeof(indicators)/sizeof(UINT)))
  	{
  		TRACE0("Failed to create status bar\n");
  		return -1;      // fail to create
  	}
  /*Create Robot Status toolbar from the dialog bar*/
  	if (!m_wndRobotStatus.Create(this, IDW_ROBOTSTATUS, CBRS_TOP, IDW_ROBOTSTATUS) ) {
  		TRACE0("Failed to create robot tool bar\n");
  		return -1;		//fail to create
  	}
  	m_wndRobotStatus.SetWindowText("Robot Status");
  	m_wndRobotStatus.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
  	m_wndRobotStatus.SetBarStyle(m_wndRobotStatus.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
  /*Create the Robot Externals Toolbar*/
  	if ( !m_wndRobotExternals.Create(this, IDW_ROBOT_EXTERNALS, CBRS_TOP, IDW_ROBOT_EXTERNALS)) {
  		TRACE0("Failed to create robot externals rool bar\n");
  		return -1;
  	}
  	m_wndRobotExternals.SetWindowText("Robot Externals");
  	m_wndRobotExternals.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
  	m_wndRobotExternals.SetBarStyle(m_wndRobotExternals.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
  	m_wndRobotExternals.SetFrameWnd(this);
  	
  /*Create the Robot Controls Tool Bar*/
  /*	if ( !m_wndRobotControls.Create(this) ) {
  		TRACE0("Failed to create roboot controls tool bar\n");
  		return -1;
  	}
  	if ( !m_wndRobotControls.LoadToolBar(IDR_ROBOTCONTROL) ) {
  		TRACE0("Failed to create roboot controls tool bar\n");
  		return -1;
  	}
  	m_wndRobotControls.SetWindowText("Robot Controls");
  	m_wndRobotControls.EnableDocking(CBRS_ALIGN_ANY);
  	m_wndRobotControls.SetBarStyle(m_wndRobotControls.GetBarStyle() |
  		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
  */
  /*Create the Map Tools Tool Bar*/
  	if ( !m_wndMapTools.Create(this) ) {
  		TRACE0("Failed to create Map tools tool bar\n");
  		return -1;
  	}
  	if ( !m_wndMapTools.LoadToolBar(IDR_MAP_TOOLS) ) {
  		TRACE0("Failed to create Map Tools tool bar\n");
  		return -1;
  	}
  	m_wndMapTools.SetWindowText("Map Tools");
  	m_wndMapTools.EnableDocking(CBRS_ALIGN_ANY);
  	m_wndMapTools.SetBarStyle(m_wndMapTools.GetBarStyle() |
  		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
  	
  /*Create FlowZap Controls toolbar from the dialog bar*/
  	if (!m_wndFlowZapControl.Create(this, IDW_FLOWZAP_CONTROL, CBRS_TOP, IDW_FLOWZAP_CONTROL) ) {
  		TRACE0("Failed to create FlowZap control tool bar\n");
  		return -1;		//fail to create
  	}
  /*Need to fill the Flow Zap File list otherwise things go wrong first time you open a flowzap file*/
  //	OnFzcDropdownComFlowzapFile();
  	m_wndFlowZapControl.SetWindowText("Zap! Control");
  	m_wndFlowZapControl.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
  	m_wndFlowZapControl.SetBarStyle(m_wndFlowZapControl.GetBarStyle() |
  		CBRS_TOOLTIPS | CBRS_FLYBY);
  
  /*Create Drawing Tools toolbar from the dialog bar*/
  	if (!m_wndDrawingTools.Create(this)) {
  		TRACE0("Failed to create Drawing Tools tool bar\n");
  		return -1;		//fail to create
  	}
  	m_wndDrawingTools.LoadToolBar(IDR_DRAWING_TOOLS);
  	m_wndDrawingTools.SetWindowText("Drawing Tools");
  	m_wndDrawingTools.EnableDocking(CBRS_ALIGN_ANY);
  	m_wndDrawingTools.SetBarStyle(m_wndDrawingTools.GetBarStyle() |
  		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
  
  /*	CButton* Button = (CButton*)m_wndDrawingTools.GetDlgItem(ID_DRAW_LINE);
  	HBITMAP bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DRAW_LINE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
  	Button->SetBitmap(bitmap);
  
  	Button = (CButton*)m_wndDrawingTools.GetDlgItem(ID_DRAW_FREEHAND);
  	bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DRAW_FREEHAND), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
  	Button->SetBitmap(bitmap);
  
  	Button = (CButton*)m_wndDrawingTools.GetDlgItem(ID_DRAW_RECTANGLE);
  	bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DRAW_RECTANGLE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
  	Button->SetBitmap(bitmap);
  	
  	Button = (CButton*)m_wndDrawingTools.GetDlgItem(ID_DRAW_PEN_STYLE);
  	bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DRAW_PENS_INACTIVE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
  	Button->SetBitmap(bitmap);
  
  	Button = (CButton*)m_wndDrawingTools.GetDlgItem(ID_DRAW_PEN_SIZE);
  	bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DRAW_WIDTHS_INACTIVE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
  	Button->SetBitmap(bitmap);
  */
  /*Create the Robot Tools tool bar*/
  	if ( !m_wndRobotTools.Create(this) ) {
  		TRACE0("Failed to create roboot tools tool bar\n");
  		return -1;
  	}
  //	CSize ButtonSize;
  //	ButtonSize.cx = 20;
  //	ButtonSize.cy = 20;
  //	m_wndRobotControls.GetToolBarCtrl().SetButtonSize(ButtonSize);
  	if ( !m_wndRobotTools.LoadToolBar(IDR_ROBOT_TOOLS) ) {
  		TRACE0("Failed to create roboot tools tool bar\n");
  		return -1;
  	}
  
  	m_wndRobotTools.SetWindowText("Robot Tools");
  	m_wndRobotTools.EnableDocking(CBRS_ALIGN_ANY);
  	m_wndRobotTools.SetBarStyle(m_wndRobotTools.GetBarStyle() |
  		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
  /*Create Map Bar toolbar from the dialog bar*/
  	if (!m_wndMapBar.Create(this, IDD_MAP_BAR, CBRS_TOP, IDD_MAP_BAR) ) {
  		TRACE0("Failed to create map bar tool bar\n");
  		return -1;		//fail to create
  	}
  	m_wndMapBar.SetWindowText("Map Bar");
  	m_wndMapBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
  
  	EnableDocking(CBRS_ALIGN_ANY);
  /*Dock all the toolbars in their places*/
  	CRect ClientRect;
  	GetClientRect(&ClientRect);
  	CPoint Point = ClientRect.TopLeft();
  	ClientToScreen(&Point);
  	Point.y--;
  	CRect ToolbarRect;
  	CRect WindowRect;
  	m_wndRobotTools.GetWindowRect(&WindowRect);
  	ToolbarRect.left = Point.x;
  	ToolbarRect.top = Point.y;
  	ToolbarRect.right = ToolbarRect.left + 200;
  	ToolbarRect.bottom = ToolbarRect.top + 20;
  	DockControlBar(&m_wndRobotTools,AFX_IDW_DOCKBAR_TOP,&ToolbarRect);
  	m_wndRobotTools.GetWindowRect(&WindowRect);
  
  	m_wndRobotStatus.GetWindowRect(&WindowRect);
  	ToolbarRect.left = ToolbarRect.right + 1;
  	ToolbarRect.right = ToolbarRect.left + WindowRect.Width();
  	ToolbarRect.bottom = ToolbarRect.top + WindowRect.Height();
  	DockControlBar(&m_wndRobotStatus, AFX_IDW_DOCKBAR_TOP, &ToolbarRect);
  
  	ToolbarRect.top = ToolbarRect.bottom + 1;
  	ToolbarRect.left = Point.x;
  
  	m_wndMapTools.GetWindowRect(&WindowRect);
  	ToolbarRect.right = ToolbarRect.left + WindowRect.Width();
  	ToolbarRect.bottom = ToolbarRect.top + WindowRect.Height();
  	DockControlBar(&m_wndMapTools, AFX_IDW_DOCKBAR_TOP, &ToolbarRect);
  
  	m_wndMapTools.GetWindowRect(&WindowRect);
  
  	DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP,NULL);
  //	DockControlBar(&m_wndRobotControls,AFX_IDW_DOCKBAR_TOP,NULL);
  	DockControlBar(&m_wndFlowZapControl,AFX_IDW_DOCKBAR_TOP,NULL);
  	DockControlBar(&m_wndDrawingTools, AFX_IDW_DOCKBAR_TOP, NULL);
  	DockControlBar(&m_wndMapBar, AFX_IDW_DOCKBAR_TOP, NULL);
  	DockControlBar(&m_wndRobotExternals, AFX_IDW_DOCKBAR_TOP, NULL);
  //	LoadBarState(AfxGetApp()->m_pszProfileName);
  
  /*Restore the state of the toolbars*/
  /*	m_wndToolBar.GetToolBarCtrl().RestoreState(HKEY_CLASSES_ROOT, "MapNZap.Toolbars", "ToolBar");
  //	m_wndRobotStatus.GetToolBarCtrl().RestoreState(HKEY_CLASSES_ROOT, "MapNZap.Toolbars", "RobotStatus");
  //	m_wndStatusBar.GetToolBarCtrl().RestoreState(HKEY_CLASSES_ROOT, "MapNZap.Toolbars", "StatusBar");
  	m_wndRobotControls.GetToolBarCtrl().RestoreState(HKEY_CLASSES_ROOT, "MapNZap.Toolbars", "RobotControls");
  //	m_wndFlowZapControl.GetToolBarCtrl().RestoreState(HKEY_CLASSES_ROOT, "MapNZap.Toolbars", "FlowZapControl");
  	m_wndRobotTools.GetToolBarCtrl().RestoreState(HKEY_CLASSES_ROOT, "MapNZap.Toolbars", "RobotTools");
  */
  //	LoadBarState(AfxGetApp()->m_pszProfileName);
  	// CG: The following block was inserted by 'Status Bar' component.
  	{
  		// Find out the size of the static variable 'indicators' defined
  		// by AppWizard and copy it
  		int nOrigSize = sizeof(indicators) / sizeof(UINT);
  
  		UINT* pIndicators = new UINT[nOrigSize + 2];
  		memcpy(pIndicators, indicators, sizeof(indicators));
  
  		// Call the Status Bar Component's status bar creation function
  		if (!InitStatusBar(pIndicators, nOrigSize, 60))
  		{
  			TRACE0("Failed to initialize Status Bar\n");
  			return -1;
  		}
  		delete[] pIndicators;
  	}
  
  	CPoint DesktopSize = GetDesktopSize();
  	CRect MainFrmRect;
  	MainFrmRect.top = AfxGetApp()->GetProfileInt(MAIN_FRAME_SECTION, "TOP", -1);
  	MainFrmRect.left = AfxGetApp()->GetProfileInt(MAIN_FRAME_SECTION, "LEFT", -1);
  	MainFrmRect.bottom = AfxGetApp()->GetProfileInt(MAIN_FRAME_SECTION, "BOTTOM", -1);
  	MainFrmRect.right = AfxGetApp()->GetProfileInt(MAIN_FRAME_SECTION, "RIGHT", -1);
  	if ( (MainFrmRect.top != -1) && (MainFrmRect.left != -1) && (MainFrmRect.bottom != -1) && (MainFrmRect.right != -1) ) {
  	/*Make sure the old MainFrmRect is inside the desktop area*/
  		if ( (MainFrmRect.top < DesktopSize.y) && (MainFrmRect.left < DesktopSize.x) ) {
  			MoveWindow(MainFrmRect);
  		}
  	}
  
  
  	return 0;
  
  }
  
  BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
  {
  	// TODO: Modify the Window class or styles here by modifying
  	//  the CREATESTRUCT cs
  
  	return CFrameWnd::PreCreateWindow(cs);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // CMainFrame diagnostics
  
  #ifdef _DEBUG
  void CMainFrame::AssertValid() const
  {
  	CFrameWnd::AssertValid();
  }
  
  void CMainFrame::Dump(CDumpContext& dc) const
  {
  	CFrameWnd::Dump(dc);
  }
  
  #endif //_DEBUG
  
  /////////////////////////////////////////////////////////////////////////////
  // CMainFrame message handlers
  
  
  void CMainFrame::OnViewRobotControls() 
  /*******************************************************************************
  //Created: 08/01/96 S.R.
  //Last Revision: 08/01/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	BOOL bVisible = ((m_wndRobotTools.GetStyle() & WS_VISIBLE) != 0);
  
  	ShowControlBar(&m_wndRobotTools, !bVisible, FALSE);
  	RecalcLayout();
  }
  
  void CMainFrame::OnUpdateViewRobotControls(CCmdUI* pCmdUI) 
  /*******************************************************************************
  //Created: 08/01/96 S.R.
  //Last Revision: 08/01/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	BOOL bVisible = ((m_wndRobotTools.GetStyle() & WS_VISIBLE) != 0);
  	pCmdUI->SetCheck(bVisible);
  }
  
  void CMainFrame::OnViewRobotStatus() 
  /*******************************************************************************
  //Created: 08/01/96 S.R.
  //Last Revision: 08/01/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	BOOL bVisible = ((m_wndRobotStatus.GetStyle() & WS_VISIBLE) != 0);
  
  	ShowControlBar(&m_wndRobotStatus, !bVisible, FALSE);
  	RecalcLayout();
  }
  
  void CMainFrame::OnUpdateViewRobotStatus(CCmdUI* pCmdUI) 
  /*******************************************************************************
  //Created: 08/01/96 S.R.
  //Last Revision: 08/01/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	BOOL bVisible = ((m_wndRobotStatus.GetStyle() & WS_VISIBLE) != 0);
  	pCmdUI->SetCheck(bVisible);
  }
  
  void CMainFrame::OnPlaceLabel() 
  /*******************************************************************************
  //Created: 08/01/96 S.R.
  //Last Revision: 02/04/97 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	CRobotWorldView* WorldView = GetRobotWorldView();
  	if (WorldView != NULL) {
  		WorldView->SetMouseMode(mmPlaceLabel);;
  	}
  }
  
  void CMainFrame::OnViewMotorControls() 
  /*******************************************************************************
  //Created: 02/04/97 S.R.
  //Last Revision: 02/04/97 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	if (GetActiveView() != NULL) {
  		CDlgMotorControls* MotorControls = &(GetRobotWorldView()->m_DlgMotorControls);
  		if (MotorControls != NULL) {
  			if (MotorControls->IsWindowVisible()) {
  				MotorControls->ShowWindow(SW_HIDE);
  			} else {
  				MotorControls->ShowWindow(SW_SHOW);
  			}
  		}
  	}
  }
  
  void CMainFrame::OnUpdateViewMotorControls(CCmdUI* pCmdUI) 
  /*******************************************************************************
  //Created: 02/04/97 S.R.
  //Last Revision: 02/04/97 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	if (GetActiveView() != NULL) {
  		CDlgMotorControls* MotorControls = &GetRobotWorldView()->m_DlgMotorControls;
  		if (MotorControls!= NULL) {
  			if (MotorControls->IsWindowVisible()) {
  				pCmdUI->SetCheck(TRUE);
  			} else {
  				pCmdUI->SetCheck(FALSE);
  			}
  		}
  	}
  }
  
  void CMainFrame::OnUpdateTime(CCmdUI* pCmdUI)
  {
  	// CG: This function was inserted by 'Status Bar' component.
  
  	// Get current date and format it
  	CTime time = CTime::GetCurrentTime();
  	CString strTime = time.Format(_T("%X"));
  
  	// BLOCK: compute the width of the date string
  	CSize size;
  	{
  		HGDIOBJ hOldFont = NULL;
  		HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
  		CClientDC dc(NULL);
  		if (hFont != NULL) 
  			hOldFont = dc.SelectObject(hFont);
  		size = dc.GetTextExtent(strTime);
  		if (hOldFont != NULL) 
  			dc.SelectObject(hOldFont);
  	}
  
  	// Update the pane to reflect the current time
  	UINT nID, nStyle;
  	int nWidth;
  	m_wndStatusBar.GetPaneInfo(m_nTimePaneNo, nID, nStyle, nWidth);
  	m_wndStatusBar.SetPaneInfo(m_nTimePaneNo, nID, nStyle, size.cx);
  	pCmdUI->SetText(strTime);
  	pCmdUI->Enable(TRUE);
  
  }
  
  BOOL CMainFrame::InitStatusBar(UINT *pIndicators, int nSize, int nSeconds)
  {
  	// CG: This function was inserted by 'Status Bar' component.
  
  	// Create an index for the TIME pane
  	m_nTimePaneNo = nSize++;
  	nSeconds = 1;
  	pIndicators[m_nTimePaneNo] = ID_INDICATOR_TIME;
  
  	// TODO: Select an appropriate time interval for updating
  	// the status bar when idling.
  	m_wndStatusBar.SetTimer(0x1000, nSeconds * 1000, NULL);
  
  	return m_wndStatusBar.SetIndicators(pIndicators, nSize);
  
  }
  
  void CMainFrame::OnClose() 
  {
  /*	m_wndToolBar.GetToolBarCtrl().SaveState(HKEY_CLASSES_ROOT, "MapNZap.Toolbars", "ToolBar");
  //		m_wndRobotStatus.GetToolBarCtrl().SaveState(HKEY_CLASSES_ROOT, "MapNZap.Toolbars", "RobotStatus");
  //		m_wndStatusBar.GetToolBarCtrl().SaveState(HKEY_CLASSES_ROOT, "MapNZap.Toolbars", "StatusBar");
  	m_wndRobotControls.GetToolBarCtrl().SaveState(HKEY_CLASSES_ROOT, "MapNZap.Toolbars", "RobotControls");
  //		m_wndFlowZapControl.GetToolBarCtrl().SaveState(HKEY_CLASSES_ROOT, "MapNZap.Toolbars", "FlowZapControl");
  	m_wndRobotTools.GetToolBarCtrl().SaveState(HKEY_CLASSES_ROOT, "MapNZap.Toolbars", "RobotTools");
  */
  //	SaveBarState(AfxGetApp()->m_pszProfileName);
  	CRobotWorldView* pView = GetRobotWorldView();
  	if (!RobotWaiting()) {
  		bool SafeShutDown = pView->SafeAppShutdown();
  		ASSERT(SafeShutDown);
  	} else {
  		if (pView->m_DlgFlowZap == NULL) {
  			CFrameWnd::OnClose();
  		} else {
  			if (pView->m_DlgFlowZap->CloseFile() != IDCANCEL) {
  				pView->m_DlgFlowZap->CloseFlowZap();
  				CFrameWnd::OnClose();
  			}
  		}
  	}
  }
  
  void CMainFrame::OnZoom100() 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	View->m_ProbabilityGrid->m_ZoomLevel = 1.0;
  	View->SetScrollBars();
  	View->OnZoomView();
  }
  
  void CMainFrame::OnUpdateZoom100(CCmdUI* pCmdUI) 
  {
  	if (GetRobotWorldView()->m_ProbabilityGrid->m_ZoomLevel == 1.0) {
  		pCmdUI->SetCheck(TRUE);
  	} else {
  		pCmdUI->SetCheck(FALSE);
  	}
  }
  
  void CMainFrame::OnZoom200() 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	View->m_ProbabilityGrid->m_ZoomLevel = 2.0;
  	View->SetScrollBars();
  	View->OnZoomView();
  }
  
  void CMainFrame::OnUpdateZoom200(CCmdUI* pCmdUI) 
  {
  	if (GetRobotWorldView()->m_ProbabilityGrid->m_ZoomLevel == 2.0) {
  		pCmdUI->SetCheck(TRUE);
  	} else {
  		pCmdUI->SetCheck(FALSE);
  	}
  }
  
  void CMainFrame::OnZoom400() 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	View->m_ProbabilityGrid->m_ZoomLevel = 4.0;
  	View->SetScrollBars();
  	View->OnZoomView();
  }
  
  void CMainFrame::OnUpdateZoom400(CCmdUI* pCmdUI) 
  {
  	if (GetRobotWorldView()->m_ProbabilityGrid->m_ZoomLevel == 4.0) {
  		pCmdUI->SetCheck(TRUE);
  	} else {
  		pCmdUI->SetCheck(FALSE);
  	}
  }
  
  void CMainFrame::OnViewLabels() 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	View->m_ProbabilityGrid->m_ExecutableObjectList->ShowCaptions(!View->m_ProbabilityGrid->m_ExecutableObjectList->GetShowCaptions());
  	View->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  }
  
  void CMainFrame::OnUpdateViewLabels(CCmdUI* pCmdUI) 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	pCmdUI->SetCheck(View->m_ProbabilityGrid->m_ExecutableObjectList->GetShowCaptions());
  }
  
  void CMainFrame::OnViewRedZones() 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	View->m_ProbabilityGrid->m_RWGraphicObjectList->ShowObjects(goRedZone, !View->m_ProbabilityGrid->m_RWGraphicObjectList->GetShowRedZones());
  	View->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  }
  
  void CMainFrame::OnUpdateViewRedZones(CCmdUI* pCmdUI) 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	pCmdUI->SetCheck(View->m_ProbabilityGrid->m_RWGraphicObjectList->GetShowRedZones());
  }
  
  void CMainFrame::OnCenterRobot() 
  {
  	GetRobotWorldView()->CenterRobot();
  }
  
  void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd) 
  {
  	CFrameWnd::OnPaletteChanged(pFocusWnd);
  	
  	CRobotWorldView* pView = GetRobotWorldView();
  	if (pView != NULL) {
  		pView->m_ProbabilityGrid->RestorePalette();
  	}
  }
  
  BOOL CMainFrame::OnQueryNewPalette() 
  {
  	CRobotWorldView* pView = GetRobotWorldView();
  	if (pView != NULL) {
  		pView->m_ProbabilityGrid->RestorePalette();
  	}
  	
  	return CFrameWnd::OnQueryNewPalette();
  }
  
  void CMainFrame::OnViewVacuumPatches()
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	View->m_ProbabilityGrid->m_RWGraphicObjectList->ShowObjects(goVacuumPatch, !View->m_ProbabilityGrid->m_RWGraphicObjectList->GetShowVacuumPatches());
  	View->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  }
  
  void CMainFrame::OnUpdateViewVacuumPatches(CCmdUI* pCmdUI) 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	pCmdUI->SetCheck(View->m_ProbabilityGrid->m_RWGraphicObjectList->GetShowVacuumPatches());
  }
  
  void CMainFrame::OnMMBlockGrid() 
  {
  //	GetRobotWorldView()->SetMouseMode(mmBlockGrid);
  }
  
  void CMainFrame::OnMMClearGrid() 
  {
  //	GetRobotWorldView()->SetMouseMode(mmClearGrid);
  }
  
  void CMainFrame::OnMMUnknownGrid() 
  {
  //	GetRobotWorldView()->SetMouseMode(mmUnknownGrid);
  }
  
  void CMainFrame::OnMMDefault() 
  {
  	GetRobotWorldView()->SetMouseMode(mmDefault);
  }
  
  void CMainFrame::OnMMErase() 
  {
  	GetRobotWorldView()->SetMouseMode(mmErase);
  }
  
  void CMainFrame::OnMMMove() 
  {
  	GetRobotWorldView()->SetMouseMode(mmEdit);
  }
  
  void CMainFrame::OnBlockArea() 
  {
  	GetRobotWorldView()->SetMouseMode(mmPlaceRedZone);	
  }
  
  void CMainFrame::OnCalibrateSurface() 
  /*******************************************************************************
  //Created: 12/03/96 S.R.
  //Last Revision: 12/03/96 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	CRobotWorldView* RobotWorldView = GetRobotWorldView();
  	CRobot* Robot = RobotWorldView->GetActiveRobot();
  	if (Robot != NULL) {
  		if (m_DlgCalibrateSurface == NULL) {
  			m_DlgCalibrateSurface = new CDlgCalibrateSurface;
  			m_DlgCalibrateSurface->m_Robot = Robot;
  			m_DlgCalibrateSurface->Create(IDD_CALIBRATESURFACE, NULL);
  			m_DlgCalibrateSurface->ShowWindow(SW_SHOW);
  		} else {
  			m_DlgCalibrateSurface->m_Robot = Robot;
  			m_DlgCalibrateSurface->ShowWindow(SW_SHOW);
  		}
  	}
  }
  
  void CMainFrame::OnUpdateFileMenu(CCmdUI* pCmdUI)
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  //	m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_SAVE, enable);
  //	m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, enable);
  //	m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, enable);
  //	m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_PRINT, enable);
  }
  
  void CMainFrame::OnEnableRobotPCControl(bool Enable)
  {
  	Enable = !Enable;
  	m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_SAVE, Enable);
  	m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, Enable);
  	m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, Enable);
  	m_wndToolBar.GetToolBarCtrl().EnableButton(ID_FILE_PRINT, Enable);
  
  /*	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_GOHOME, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_VACUUM, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_BLOCKAREA, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_PLACELABEL, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_RUNPROGRAM, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_CREATECALIBRATIONWALL, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_CREATECALIBRATIONDOORWAY, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_CALIBRATESURFACE, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_MMERASE, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_MMDEFAULT, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_MMMOVE, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_MM_BLOCK_GRID, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_MM_CLEAR_GRID, Enable);
  	m_wndRobotControls.GetToolBarCtrl().EnableButton(ID_MM_UNKNOWN_GRID, Enable);
  */
  	m_wndDrawingTools.GetToolBarCtrl().EnableButton(ID_DRAW_LINE, Enable);
  	m_wndDrawingTools.GetToolBarCtrl().EnableButton(ID_DRAW_FREEHAND, Enable);
  	m_wndDrawingTools.GetToolBarCtrl().EnableButton(ID_DRAW_RECTANGLE, Enable);
  	m_wndDrawingTools.GetToolBarCtrl().EnableButton(ID_DRAW_PEN_OBSTACLE, Enable);
  	m_wndDrawingTools.GetToolBarCtrl().EnableButton(ID_DRAW_PEN_FREE, Enable);
  	m_wndDrawingTools.GetToolBarCtrl().EnableButton(ID_DRAW_PEN_UNKNOWN, Enable);
  	m_wndDrawingTools.GetToolBarCtrl().EnableButton(ID_DRAW_PEN_WIDTH1, Enable);
  	m_wndDrawingTools.GetToolBarCtrl().EnableButton(ID_DRAW_PEN_WIDTH2, Enable);
  	m_wndDrawingTools.GetToolBarCtrl().EnableButton(ID_DRAW_PEN_WIDTH3, Enable);
  	m_wndDrawingTools.GetToolBarCtrl().EnableButton(ID_DRAW_PEN_WIDTH4, Enable);
  }
  
  void CMainFrame::OnUpdateFileMRUMenu(CCmdUI* pCmdUI)
  {
  	bool enable = RobotWaiting();
  //	bool enable = RobotWaiting();
  	if (enable) pCmdUI->ContinueRouting();
  	else pCmdUI->Enable(enable);
  }
  
  void CMainFrame::OnUpdateRobotControls(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  //	bool enable = RobotWaiting();
  	if (enable) pCmdUI->ContinueRouting();
  	else pCmdUI->Enable(enable);
  }
  
  void CMainFrame::OnSetEnableLearnRoom(bool Enable)
  {
  //	m_wndRobotStatus.GetChkEnableLearnRoom()->SetCheck(Enable);
  }
  
  void CMainFrame::OnSetMaxSpeed(short MaxSpeed)
  {
  	m_wndRobotStatus.OnSetMaxSpeed(MaxSpeed);
  }
  
  void CMainFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
  {
  	CFrameWnd::OnHScroll(nSBCode, nPos, pScrollBar);
  }
  
  void CMainFrame::OnSetAutoCalibrate(bool AutoCalibrate)
  {
  //	m_wndRobotStatus.GetChkAutoCalibrate()->SetCheck(AutoCalibrate);
  }
  
  void CMainFrame::OnViewTestRobot() 
  {
  	if (GetActiveView() != NULL) {
  		GetRobotWorldView()->SetTestRobotDialogVisible(!GetRobotWorldView()->GetTestRobotDialogVisible());
  	}
  }
  
  void CMainFrame::OnUpdateViewTestRobot(CCmdUI* pCmdUI) 
  {
  	if (GetActiveView() != NULL) {
  		if (GetRobotWorldView()->GetTestRobotDialogVisible()) {
  			pCmdUI->SetCheck(TRUE);
  		} else {
  			pCmdUI->SetCheck(FALSE);
  		}
  	}
  }
  
  void CMainFrame::OnUpdateMMDefault(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  //	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmDefault);
  }
  
  void CMainFrame::OnUpdateBlockArea(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  //	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmPlaceRedZone);
  }
  
  void CMainFrame::OnUpdateMMBlockGrid(CCmdUI* pCmdUI) 
  {
  //	bool enable = RobotWaiting();
  //	pCmdUI->Enable(enable);
  //	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmBlockGrid);
  }
  
  void CMainFrame::OnUpdateMMClearGrid(CCmdUI* pCmdUI) 
  {
  //	bool enable = RobotWaiting();
  //	pCmdUI->Enable(enable);
  //	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmClearGrid);
  }
  
  void CMainFrame::OnUpdateMMUnknownGrid(CCmdUI* pCmdUI) 
  {
  //	bool enable = RobotWaiting();
  //	pCmdUI->Enable(enable);
  //	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmUnknownGrid);
  }
  
  void CMainFrame::OnUpdateMMErase(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  //	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmErase);
  }
  
  void CMainFrame::OnUpdateMMMove(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmEdit);
  }
  
  void CMainFrame::OnUpdatePlaceLabel(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmPlaceLabel);
  }
  
  void CMainFrame::OnVacuum() 
  {
  	GetRobotWorldView()->SetMouseMode(mmPlaceVacuumPatch);
  }
  
  void CMainFrame::OnUpdateVacuum(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmPlaceVacuumPatch);
  }
  
  //void CMainFrame::OnViewMouseControls() 
  //{
  //	GetRobotWorldView()->SetMouseControlsDialogVisible(!GetRobotWorldView()->GetMouseControlsDialogVisible());
  //}
  
  //void CMainFrame::OnUpdateViewMouseControls(CCmdUI* pCmdUI) 
  //{
  //	if (GetActiveView() != NULL) {
  //		if (GetRobotWorldView()->GetMouseControlsDialogVisible()) {
  //			pCmdUI->SetCheck(TRUE);
  //		} else {
  //			pCmdUI->SetCheck(FALSE);
  //		}
  //	}
  //}
  
  void CMainFrame::OnViewPositionSpots() 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  //	View->m_ProbabilityGrid->m_RWGraphicObjectList->ShowPositionLabels(!View->m_ProbabilityGrid->m_ExecutableObjectList->GetPositionLabelsVisible());
  //	View->m_LabelList->m_ShowLabels = !View->m_LabelList->m_ShowLabels;
  //	View->m_LabelList->ShowLabels(View->m_LabelList->m_ShowLabels);	
  	View->m_ProbabilityGrid->m_RWGraphicObjectList->ShowObjects(goPosition, !View->m_ProbabilityGrid->m_RWGraphicObjectList->GetShowPositionLabels());
  	View->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  }
  
  void CMainFrame::OnUpdateViewPositionSpots(CCmdUI* pCmdUI) 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  //	pCmdUI->SetCheck(View->m_ProbabilityGrid->m_ExecutableObjectList->GetPositionLabelsVisible());
  	pCmdUI->SetCheck(View->m_ProbabilityGrid->m_RWGraphicObjectList->GetShowPositionLabels());
  }
  
  void CMainFrame::OnViewDrawingTools() 
  {
  	BOOL bVisible = ((m_wndMapTools.GetStyle() & WS_VISIBLE) != 0);
  
  	ShowControlBar(&m_wndMapTools, !bVisible, FALSE);
  	RecalcLayout();
  }
  
  void CMainFrame::OnUpdateViewDrawingTools(CCmdUI* pCmdUI) 
  {
  	BOOL bVisible = ((m_wndDrawingTools.GetStyle() & WS_VISIBLE) != 0);
  	pCmdUI->SetCheck(bVisible);
  }
  
  void CMainFrame::OnMmPlaceFinishLine() 
  {
  	CRobotWorldView* WorldView = GetRobotWorldView();
  	if (WorldView != NULL) {
  		WorldView->SetMouseMode(mmPlaceFinishLine);
  	}
  }
  
  void CMainFrame::OnUpdateMmPlaceFinishLine(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmPlaceFinishLine);
  }
  
  void CMainFrame::OnViewSettings() 
  {
  	GetRobotWorldView()->DoModalPropertySheet();
  }
  
  void CMainFrame::OnUpdateViewSettings(CCmdUI* pCmdUI) 
  {
  }
  
  void CMainFrame::OnHelpResetlaygo() 
  {
  	GetRobotWorldView()->ResetLaygo();	
  }
  
  void CMainFrame::OnHelpDisablelaygo() 
  {
  	GetRobotWorldView()->SetEnableLaygo(!GetRobotWorldView()->GetEnableLaygo());
  }
  
  void CMainFrame::OnUpdateHelpDisablelaygo(CCmdUI* pCmdUI) 
  {
  	bool enable = 	!GetRobotWorldView()->GetEnableLaygo();
  	pCmdUI->SetCheck(enable);
  }
  
  void CMainFrame::OnLinePath() 
  {
  	CRobotWorldView* WorldView = GetRobotWorldView();
  	if (WorldView != NULL) {
  		WorldView->SetMouseMode(mmPlaceLinePath);
  	}
  }
  
  void CMainFrame::OnUpdateLinePath(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmPlaceLinePath);
  }
  
  void CMainFrame::OnBatteryChargeStateChange(short BatteryChargeState)
  {
  	m_wndRobotStatus.OnBatteryChargeStateChange(BatteryChargeState);
  }
  
  void CMainFrame::OnSetBatteryLevel(float BatteryLevel)
  {
  	m_wndRobotStatus.OnSetBatteryLevel(BatteryLevel);
  }
  
  void CMainFrame::OnUpdatePowerOff(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  }
  
  void CMainFrame::OnSetVacuumAttached(bool Attached)
  {
  //	m_wndRobotStatus.GetChkVacuumAttached()->SetCheck(Attached);
  //	m_wndRobotStatus.GetChkWagonAttached()->SetCheck(!Attached);
  }
  
  void CMainFrame::SetRadioLevel(float RadioLevel)
  {
  	m_wndRobotStatus.SetRadioLevel(RadioLevel);
  }
  
  void CMainFrame::OnUpdateSearchArea(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmSearchArea);
  }
  
  void CMainFrame::SetFlowZapControlFileList(CStringList FileList)
  {
  //	m_wndFlowZapControl.SetFileList(FileList);
  }
  
  void CMainFrame::OnFzcButPlay() 
  {
  //	m_wndFlowZapControl.GetPlay()->EnableWindow(FALSE);
  //	m_wndFlowZapControl.GetPause()->EnableWindow(TRUE);
  //	m_wndFlowZapControl.GetStop()->EnableWindow(TRUE);
  //	m_wndFlowZapControl.GetStop()->SetFocus();
  	GetRobotWorldView()->FlowZapPlay();
  //	m_wndFlowZapControl.GetPlay()->EnableWindow(TRUE);
  //	m_wndFlowZapControl.GetPause()->EnableWindow(FALSE);
  //	m_wndFlowZapControl.GetStop()->EnableWindow(FALSE);
  }
  
  void CMainFrame::OnFzcButStop() 
  {
  //	m_wndFlowZapControl.GetPlay()->EnableWindow(TRUE);
  //	m_wndFlowZapControl.GetPause()->EnableWindow(FALSE);
  //	m_wndFlowZapControl.GetStop()->EnableWindow(FALSE);
  	GetRobotWorldView()->FlowZapStop();
  }
  
  void CMainFrame::OnFzcButPause() 
  {
  //	m_wndFlowZapControl.GetPlay()->EnableWindow(TRUE);
  //	m_wndFlowZapControl.GetPause()->EnableWindow(FALSE);
  //	m_wndFlowZapControl.GetStop()->EnableWindow(TRUE);
  	GetRobotWorldView()->FlowZapPause();
  }
  
  void CMainFrame::OnFzcShowFZ()
  {
  	GetRobotWorldView()->OnViewFlowZap();
  }
  
  void CMainFrame::OnFzcDropdownComFlowzapFile() 
  {
  	CString FlowZapFilesPath = ((CRobotWorldApp*)(AfxGetApp()))->m_DefaultPath + (cFlowZapFilesDirectory + 1);
  	FlowZapFilesPath += "\\*.zap";
  	LPCTSTR FileFilter = FlowZapFilesPath;
  
  	
  	WIN32_FIND_DATA FindFileData;
  	HANDLE FileHandle = FindFirstFile(FlowZapFilesPath, &FindFileData);
  	bool Done = false;
  
  	CComboBox* comFileList = m_wndFlowZapControl.GetComFileList();
  	CString OldSelection;
  	comFileList->GetWindowText(OldSelection);
  	comFileList->ResetContent();
  	
  //	CStringList FilesList;
  	while ( (FileHandle != INVALID_HANDLE_VALUE) && (!Done) ) {
  //		FilesList.AddHead(FindFileData.cFileName);
  		comFileList->AddString(FindFileData.cFileName);
  		Done = !FindNextFile(FileHandle, &FindFileData);
  	}
  	int OldSelIndex = comFileList->FindString(0, OldSelection);
  	if (OldSelIndex >= 0) {
  		comFileList->SetCurSel(OldSelIndex);
  	}
  	
  	
  //	m_wndFlowZapControl.DlgDirListComboBox((char*)FileFilter, ID_COM_FLOWZAP_FILE, 0, 0);
  /*	CComboBox* comFileList = m_wndFlowZapControl.GetComFileList();
  	comFileList->ResetContent();
  	
  	int Index = comFileList->Dir(DDL_DIRECTORY, FileFilter);
  	if (Index == CB_ERR) {
  		Index = 0;
  	} else if (Index == CB_ERRSPACE) {
  		Index = 0;
  	}
  */
  }
  
  void CMainFrame::OnFzcSelchangeComFlowzapFile() 
  {
  	CString DefaultPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  	CString FileSelected;
  	CComboBox* m_comFileList = m_wndFlowZapControl.GetComFileList();
  	int Index = m_comFileList->GetCurSel();
  	if (Index != CB_ERR) {
  		m_comFileList->GetLBText(Index, FileSelected);
  		CString FilePath = DefaultPath + (cFlowZapFilesDirectory + 1);
  		FilePath += "\\" + FileSelected;
  		if (!GetRobotWorldView()->SetFlowZapFile(FilePath)) {
  		//If can't open new file, keep old file selected
  			m_comFileList->SelectString(0, m_OldFzcFile);
  		} else {
  		//Need this line because when FlowZap is opened for first time, it will
  		//load the last file opened, and set the combo box to that file.
  			m_comFileList->SelectString(0, FileSelected);
  			m_OldFzcFile = FileSelected;
  		}
  	}
  }
  
  void CMainFrame::OnFzcChangeFileOpen(CString FileOpen)
  {
  	CComboBox* comFileList = m_wndFlowZapControl.GetComFileList();
  	m_OldFzcFile = FileOpen;
  	if (FileOpen == "") {
  		comFileList->SetCurSel(-1);
  	} else {
  		int Index = comFileList->SelectString(0, FileOpen);
  		if (Index < 0) {
  			Index = comFileList->AddString(FileOpen);
  			comFileList->SetCurSel(Index);
  		}
  	}
  }
  
  void CMainFrame::OnLoadToolbars() 
  {
  	ArrangeToolbarsDefault();
  }
  
  void CMainFrame::OnSaveToolbars() 
  {
  //	SaveBarState(AfxGetApp()->m_pszProfileName);
  }
  
  void CMainFrame::OnSetWagonAttached(bool Attached)
  {
  //	m_wndRobotStatus.GetChkVacuumAttached()->SetCheck(!Attached);
  //	m_wndRobotStatus.GetChkWagonAttached()->SetCheck(Attached);
  }
  
  void CMainFrame::OnDrawFreehand() 
  {
  	GetRobotWorldView()->SetMouseMode(mmDrawFreehand);	
  }
  
  void CMainFrame::OnDrawLine() 
  {
  	GetRobotWorldView()->SetMouseMode(mmDrawLine);	
  }
  
  void CMainFrame::OnDrawRectangle() 
  {
  	GetRobotWorldView()->SetMouseMode(mmDrawRectangle);	
  }
  
  void CMainFrame::OnDrawPenSize() 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	switch (View->GetPenSize()) {
  	case CRobotWorldView::psSmall:
  		View->SetPenSize(CRobotWorldView::psMedium);
  		break;
  	case CRobotWorldView::psMedium:
  		View->SetPenSize(CRobotWorldView::psLarge);
  		break;
  	case CRobotWorldView::psLarge:
  		View->SetPenSize(CRobotWorldView::psXLarge);
  		break;
  	case CRobotWorldView::psXLarge:
  		View->SetPenSize(CRobotWorldView::psSmall);
  		break;
  	}
  }
  
  void CMainFrame::OnDrawPenStyle() 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	switch (View->GetPenMode()) {
  	case pmObstacle:
  		View->SetPenMode(pmUnknown);
  		break;
  	case pmUnknown:
  		View->SetPenMode(pmClear);
  		break;
  	case pmClear:
  		View->SetPenMode(pmObstacle);
  		break;
  	}
  //	OnSetDrawingPenMode(View->GetPenMode());
  }
  
  void CMainFrame::OnSetDrawingPenMode(short Mode)
  {
  /*	CButton* Button = (CButton*)m_wndDrawingTools.GetDlgItem(ID_DRAW_PEN_STYLE);
  	TPenMode PenMode = (TPenMode)Mode;
  	HBITMAP bitmap;
  	switch (PenMode) {
  	case pmClear:
  		bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DRAW_PEN_CLEAR_ACTIVE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
  		break;
  	case pmObstacle:
  		bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DRAW_PEN_OBSTACLE_ACTIVE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
  		break;
  	case pmUnknown:
  		bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DRAW_PEN_UNKNOWN_ACTIVE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
  		break;
  	}
  	Button->SetBitmap(bitmap);
  */
  }
  
  void CMainFrame::OnSetDrawingPenSize(short Size)
  {
  /*	CButton* Button = (CButton*)m_wndDrawingTools.GetDlgItem(ID_DRAW_PEN_SIZE);
  	CRobotWorldView::TPenSize PenSize = (CRobotWorldView::TPenSize)Size;
  	HBITMAP bitmap;
  	switch (PenSize) {
  	case CRobotWorldView::psSmall:
  		bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DRAW_WIDTH_SMALL_ACTIVE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
  		break;
  	case CRobotWorldView::psMedium:
  		bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DRAW_WIDTH_MEDIUM_ACTIVE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
  		break;
  	case CRobotWorldView::psLarge:
  		bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DRAW_WIDTH_LARGE_ACTIVE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
  		break;
  	case CRobotWorldView::psXLarge:
  		bitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DRAW_WIDTH_XLARGE_ACTIVE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
  		break;
  	}
  	Button->SetBitmap(bitmap);
  */
  }
  
  void CMainFrame::OnUpdateDrawFreehand(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmDrawFreehand);
  }
  
  void CMainFrame::OnUpdateDrawLine(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmDrawLine);
  }
  
  void CMainFrame::OnUpdateDrawRectangle(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmDrawRectangle);
  }
  
  void CMainFrame::OnMMMoveRobot() 
  {
  	GetRobotWorldView()->SetMouseMode(mmMoveRobot);	
  }
  
  void CMainFrame::OnUpdateMMMoveRobot(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmMoveRobot);
  }
  
  void CMainFrame::ArrangeToolbarsDefault()
  {
  /*Dock all the toolbars in their places*/
  	CRect ClientRect;
  	GetClientRect(&ClientRect);
  	CPoint Point = ClientRect.TopLeft();
  	ClientToScreen(&Point);
  	Point.y--;
  	CRect ToolbarRect;
  	CRect WindowRect;
  	m_wndRobotTools.GetWindowRect(&WindowRect);
  	ToolbarRect.left = Point.x;
  	ToolbarRect.top = Point.y;
  	ToolbarRect.right = ToolbarRect.left + WindowRect.Width();
  	ToolbarRect.bottom = ToolbarRect.top + WindowRect.Height();
  	DockControlBar(&m_wndRobotTools,AFX_IDW_DOCKBAR_TOP,&ToolbarRect);
  	m_wndRobotTools.GetWindowRect(&WindowRect);
  
  	m_wndRobotStatus.GetWindowRect(&WindowRect);
  	ToolbarRect.left = ToolbarRect.right;
  	ToolbarRect.right = ToolbarRect.left + WindowRect.Width();
  	ToolbarRect.bottom = ToolbarRect.top + WindowRect.Height();
  	DockControlBar(&m_wndRobotStatus, AFX_IDW_DOCKBAR_TOP, &ToolbarRect);
  
  	m_wndFlowZapControl.GetWindowRect(&WindowRect);
  	ToolbarRect.left = ToolbarRect.right;
  	ToolbarRect.right = ToolbarRect.left + WindowRect.Width();
  	ToolbarRect.bottom = ToolbarRect.top + WindowRect.Height();
  	DockControlBar(&m_wndFlowZapControl,AFX_IDW_DOCKBAR_TOP,&ToolbarRect);
  
  	ToolbarRect.top = ToolbarRect.bottom + 1;
  	ToolbarRect.left = Point.x;
  
  	m_wndMapTools.GetWindowRect(&WindowRect);
  	ToolbarRect.right = ToolbarRect.left + WindowRect.Width();
  	ToolbarRect.bottom = ToolbarRect.top + WindowRect.Height();
  	DockControlBar(&m_wndMapTools, AFX_IDW_DOCKBAR_TOP, &ToolbarRect);
  
  	m_wndDrawingTools.GetWindowRect(&WindowRect);
  	ToolbarRect.left = ToolbarRect.right;
  	ToolbarRect.right = ToolbarRect.left + WindowRect.Width();
  	ToolbarRect.bottom = ToolbarRect.top + WindowRect.Height();
  	DockControlBar(&m_wndDrawingTools, AFX_IDW_DOCKBAR_TOP, &ToolbarRect);
  
  	m_wndMapBar.GetWindowRect(&WindowRect);
  	ToolbarRect.left = ToolbarRect.right;
  	ToolbarRect.right = ToolbarRect.left + WindowRect.Width();
  	ToolbarRect.bottom = ToolbarRect.top + WindowRect.Height();
  	DockControlBar(&m_wndMapBar, AFX_IDW_DOCKBAR_TOP, &ToolbarRect);
  
  	ToolbarRect.top = ToolbarRect.bottom + 1;
  	ToolbarRect.left = Point.x;
  
  	m_wndRobotExternals.GetWindowRect(&WindowRect);
  	ToolbarRect.right = ToolbarRect.left + WindowRect.Width();
  	ToolbarRect.bottom = ToolbarRect.top + WindowRect.Height();
  	DockControlBar(&m_wndRobotExternals, AFX_IDW_DOCKBAR_TOP, &ToolbarRect);
  //	m_wndMapTools.GetWindowRect(&WindowRect);
  
  	m_wndToolBar.GetWindowRect(&WindowRect);
  	ToolbarRect.left = ToolbarRect.right;
  	ToolbarRect.right = ToolbarRect.left + WindowRect.Width();
  	ToolbarRect.bottom = ToolbarRect.top + WindowRect.Height();
  	DockControlBar(&m_wndToolBar, AFX_IDW_DOCKBAR_TOP, &ToolbarRect);
  //	DockControlBar(&m_wndRobotControls,AFX_IDW_DOCKBAR_TOP,NULL);
  }
  
  void CMainFrame::OnPaint() 
  {
  	CPaintDC dc(this); // device context for painting
  	
  	if (m_FirstShowing) {
  		ArrangeToolbarsDefault();
  //		LoadBarState(AfxGetApp()->m_pszProfileName);
  		m_FirstShowing = false;
  	}
  	
  	// Do not call CFrameWnd::OnPaint() for painting messages
  }
  
  void CMainFrame::OnViewMapObjectsToolbar() 
  {
  	BOOL bVisible = ((m_wndMapTools.GetStyle() & WS_VISIBLE) != 0);
  
  	ShowControlBar(&m_wndMapTools, !bVisible, FALSE);
  	RecalcLayout();
  }
  
  void CMainFrame::OnUpdateViewMapObjectsToolbar(CCmdUI* pCmdUI) 
  {
  	BOOL bVisible = ((m_wndMapTools.GetStyle() & WS_VISIBLE) != 0);
  	pCmdUI->SetCheck(bVisible);
  }
  
  void CMainFrame::OnViewZapControls() 
  {
  	BOOL bVisible = ((m_wndFlowZapControl.GetStyle() & WS_VISIBLE) != 0);
  
  	ShowControlBar(&m_wndFlowZapControl, !bVisible, FALSE);
  	RecalcLayout();
  }
  
  void CMainFrame::OnUpdateViewZapControls(CCmdUI* pCmdUI) 
  {
  	BOOL bVisible = ((m_wndFlowZapControl.GetStyle() & WS_VISIBLE) != 0);
  	pCmdUI->SetCheck(bVisible);
  }
  
  
  void CMainFrame::OnViewFlowZap() 
  {
  	GetRobotWorldView()->OnViewFlowZap();
  }
  
  void CMainFrame::OnViewInstrumentPanel() 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	bool bVisible = View->GetShowVelocityTorqueDisplay();
  	View->SetShowVelocityTorqueDisplay(!bVisible);
  }
  
  void CMainFrame::OnUpdateViewInstrumentPanel(CCmdUI* pCmdUI) 
  {
  	pCmdUI->SetCheck(GetRobotWorldView()->GetShowVelocityTorqueDisplay());	
  }
  
  void CMainFrame::OnHelpLaygoShowWelcomeTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoWelcome, false);
  }
  
  void CMainFrame::OnHelpLaygoShowRobotColorTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoChooseRobotColor, false);
  }
  
  void CMainFrame::OnHelpLaygoShowComPortSearchTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoComPortSearch, false);
  }
  
  void CMainFrame::OnHelpLaygoShowCreateHomeBaseTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoHomeBaseCreateState, false);
  }
  
  void CMainFrame::OnHelpLaygoShowNavigateTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoNavigating, false);
  }
  
  void CMainFrame::OnHelpLaygoShowOccupancyGridTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoOccupancyGrid, false);
  }
  
  void CMainFrame::OnHelpLaygoShowPositionUncertaintyTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoPositionUncertainty, false);
  }
  
  void CMainFrame::OnHelpLaygoShowCheckPointTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoCheckPoint, false);
  }
  
  void CMainFrame::OnHelpLaygoShowAdditionalCheckPointTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoAdditionalCheckPoints, false);
  }
  
  void CMainFrame::OnHelpLaygoShowHotPointTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoHotPoints, false);
  }
  
  void CMainFrame::OnHelpLaygoShowVacuumPathsTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoVacuumPaths, false);
  }
  
  void CMainFrame::OnHelpLaygoShowDangerZonesTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoDangerZones, false);
  }
  
  void CMainFrame::OnHelpLaygoShowFlowzapTip() 
  {
  	GetRobotWorldView()->ShowLaygoTip(CLaygo::lgoFlowZap, false);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowWelcomeTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoWelcome);
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowRobotColorTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoChooseRobotColor);
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowComPortSearchTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoComPortSearch);//2);
  //	ASSERT(CLaygo::lgoComPortSearch == 2);
  	if (!GetRobotWorldView()->GetCommunicationsEstablished()) {
  		pCmdUI->Enable(TRUE);
  	} else {
  		pCmdUI->Enable(FALSE);
  	}
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowCreateHomeBaseTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoHomeBaseCreateState);
  //	ASSERT(CLaygo::lgoHomeBaseCreateState == 3);
  	if (GetRobotWorldView()->m_HomeBase == NULL) {
  		pCmdUI->Enable(TRUE);
  	} else {
  		pCmdUI->Enable(FALSE);
  	}
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowNavigateTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoNavigating);
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowOccupancyGridTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoOccupancyGrid);
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowPositionUncertaintyTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoPositionUncertainty);
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowCheckPointTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoCheckPoint);
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowAdditionalCheckPointTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoAdditionalCheckPoints);
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowHotPointTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoHotPoints);
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowVacuumPathsTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoVacuumPaths);
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowDangerZonesTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoDangerZones);
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnUpdateHelpLaygoShowFlowzapTip(CCmdUI* pCmdUI) 
  {
  	bool check = (GetRobotWorldView()->GetLaygoTip() == CLaygo::lgoFlowZap);
  	pCmdUI->SetCheck(check);
  }
  
  void CMainFrame::OnViewLinePaths() 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	View->m_ProbabilityGrid->m_RWGraphicObjectList->ShowObjects(goLinePath, !View->m_ProbabilityGrid->m_RWGraphicObjectList->GetShowLinePaths());
  	View->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  }
  
  void CMainFrame::OnUpdateViewLinePaths(CCmdUI* pCmdUI) 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	pCmdUI->SetCheck(View->m_ProbabilityGrid->m_RWGraphicObjectList->GetShowLinePaths());
  }
  
  void CMainFrame::OnHelpSamplesCyeclock() 
  {
  	GetRobotWorldView()->LoadCyeSample(CRobotWorldView::csCyeClock);
  }
  
  void CMainFrame::OnHelpSamplesCyeguide() 
  {
  	GetRobotWorldView()->LoadCyeSample(CRobotWorldView::csCyeGuide);
  }
  
  void CMainFrame::OnHelpSamplesCyepatrol() 
  {
  	GetRobotWorldView()->LoadCyeSample(CRobotWorldView::csCyePatrol);
  }
  
  void CMainFrame::OnHelpSamplesCyepost() 
  {
  	GetRobotWorldView()->LoadCyeSample(CRobotWorldView::csCyePost);
  }
  
  void CMainFrame::OnHelpSamplesCyepup() 
  {
  	GetRobotWorldView()->LoadCyeSample(CRobotWorldView::csCyePup);
  }
  
  void CMainFrame::OnHelpSamplesCyeserve() 
  {
  	GetRobotWorldView()->LoadCyeSample(CRobotWorldView::csCyeServe);
  }
  
  void CMainFrame::OnHelpSamplesCyetruck() 
  {
  	GetRobotWorldView()->LoadCyeSample(CRobotWorldView::csCyeTruck);
  }
  
  void CMainFrame::OnUpdateHelpSamplesCyeclock(CCmdUI* pCmdUI) 
  {
  	pCmdUI->Enable(FALSE);
  }
  
  void CMainFrame::OnUpdateHelpSamplesCyeserve(CCmdUI* pCmdUI) 
  {
  }
  
  void CMainFrame::OnUpdateHelpSamplesCyepatrol(CCmdUI* pCmdUI) 
  {
  	pCmdUI->Enable(FALSE);
  }
  
  void CMainFrame::OnDestroy() 
  {
  	CFrameWnd::OnDestroy();
  	
  	CRect MainFrmRect;
  	GetWindowRect(MainFrmRect);
  	AfxGetApp()->WriteProfileInt(MAIN_FRAME_SECTION, "TOP", MainFrmRect.top);
  	AfxGetApp()->WriteProfileInt(MAIN_FRAME_SECTION, "LEFT", MainFrmRect.left);
  	AfxGetApp()->WriteProfileInt(MAIN_FRAME_SECTION, "BOTTOM", MainFrmRect.bottom);
  	AfxGetApp()->WriteProfileInt(MAIN_FRAME_SECTION, "RIGHT", MainFrmRect.right);
  	
  }
  
  void CMainFrame::OnViewRulerGrid() 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	View->SetShowRulerGrid(!View->GetShowRulerGrid());
  	View->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  }
  
  void CMainFrame::OnUpdateViewRulerGrid(CCmdUI* pCmdUI) 
  {
  	CRobotWorldView* View = GetRobotWorldView();
  	pCmdUI->SetCheck(View->GetShowRulerGrid());
  }
  
  void CMainFrame::OnMmPlaceStartLine() 
  {
  	CRobotWorldView* WorldView = GetRobotWorldView();
  	if (WorldView != NULL) {
  		WorldView->SetMouseMode(mmPlaceStartLine);
  	}
  }
  
  void CMainFrame::OnUpdateMmPlaceStartLine(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetMouseMode() == mmPlaceStartLine);
  }
  
  
  void CMainFrame::OnEditProperties() 
  {
  	GetRobotWorldView()->EditSelectedGraphicObject();
  }
  
  void CMainFrame::OnUpdateEditProperties(CCmdUI* pCmdUI) 
  {
  	BOOL bEnable = GetRobotWorldView()->GetSelectedGraphicObject() != NULL;
  	pCmdUI->Enable(bEnable);
  }
  
  void CMainFrame::OnEditDelete() 
  {
  	GetRobotWorldView()->DeleteSelectedGraphicObject();
  }
  
  void CMainFrame::OnUpdateEditDelete(CCmdUI* pCmdUI) 
  {
  	BOOL bEnable = GetRobotWorldView()->GetSelectedGraphicObject() != NULL;
  	pCmdUI->Enable(bEnable);
  }
  
  void CMainFrame::OnDrawPenClear() 
  {
  	GetRobotWorldView()->SetPenMode(pmClear);
  }
  
  void CMainFrame::OnUpdateDrawPenClear(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetPenMode() == pmClear);
  }
  
  void CMainFrame::OnDrawPenObstacle() 
  {
  	GetRobotWorldView()->SetPenMode(pmObstacle);
  }
  
  void CMainFrame::OnUpdateDrawPenObstacle(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetPenMode() == pmObstacle);
  }
  
  void CMainFrame::OnDrawPenUnknown() 
  {
  	GetRobotWorldView()->SetPenMode(pmUnknown);
  }
  
  void CMainFrame::OnUpdateDrawPenUnknown(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetPenMode() == pmUnknown);
  }
  
  void CMainFrame::OnDrawPenWidth1() 
  {
  	GetRobotWorldView()->SetPenSize(CRobotWorldView::psSmall);
  }
  
  void CMainFrame::OnUpdateDrawPenWidth1(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetPenSize() == CRobotWorldView::psSmall);
  }
  
  void CMainFrame::OnDrawPenWidth2() 
  {
  	GetRobotWorldView()->SetPenSize(CRobotWorldView::psMedium);
  }
  
  void CMainFrame::OnUpdateDrawPenWidth2(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetPenSize() == CRobotWorldView::psMedium);
  }
  
  void CMainFrame::OnDrawPenWidth3() 
  {
  	GetRobotWorldView()->SetPenSize(CRobotWorldView::psLarge);
  }
  
  void CMainFrame::OnUpdateDrawPenWidth3(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetPenSize() == CRobotWorldView::psLarge);
  }
  
  void CMainFrame::OnDrawPenWidth4() 
  {
  	GetRobotWorldView()->SetPenSize(CRobotWorldView::psXLarge);
  }
  
  void CMainFrame::OnUpdateDrawPenWidth4(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetPenSize() == CRobotWorldView::psXLarge);
  }
  
  bool CMainFrame::GetPause()
  {
  	return GetRobotWorldView()->GetPause();
  }
  
  bool CMainFrame::GetPlay()
  {
  	return GetRobotWorldView()->GetPlay();
  }
  
  void CMainFrame::OnAdjustCheckPoint() 
  {
  	GetRobotWorldView()->AdjustSelectedCalibrationZone();
  }
  
  void CMainFrame::OnUpdateAdjustCheckPoint(CCmdUI* pCmdUI) 
  {
  	BOOL bEnable = GetRobotWorldView()->GetSelectedCalibrationZone() != NULL;
  	pCmdUI->Enable(bEnable);
  }
  
  void CMainFrame::OnExternalOutputOn() 
  {
  	GetRobotWorldView()->SetRobotExternalOutput(true);
  }
  
  void CMainFrame::OnUpdateExternalOutputOn(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(GetRobotWorldView()->GetRobotExternalOutput());
  }
  
  void CMainFrame::OnExternalOutputOff() 
  {
  	GetRobotWorldView()->SetRobotExternalOutput(false);
  }
  
  void CMainFrame::OnUpdateExternalOutputOff(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  	pCmdUI->SetRadio(!(GetRobotWorldView()->GetRobotExternalOutput()));
  }
  
  void CMainFrame::OnExternalOutputChange(bool On)
  {
  	m_wndRobotExternals.SetExternalOutput(On);
  }
  
  void CMainFrame::SetExternalOutput(bool On)
  {
  	GetRobotWorldView()->SetRobotExternalOutput(On);
  }
  
  void CMainFrame::OnExternalInputUpdate(float Volts)
  {
  	m_wndRobotExternals.SetExternalInput(Volts);
  }
  
  void CMainFrame::OnViewAddRobot() 
  {
  	GetRobotWorldView()->AddRobot();
  	
  }
  
  void CMainFrame::OnUpdateViewAddRobot(CCmdUI* pCmdUI) 
  {
  	// TODO: Add your command update UI handler code here
  	
  }
  
  bool CMainFrame::RobotWaiting()
  {
  	bool Waiting = false;
  	CRobot* Robot = GetRobotWorldView()->GetActiveRobot();
  	if (Robot != NULL) {
  		Waiting = (Robot->GetState() == stWaiting);
  	}
  	return Waiting;
  }
  
  void CMainFrame::OnViewAddHomebase() 
  {
  	GetRobotWorldView()->AddHomeBase();	
  }
  
  void CMainFrame::OnUpdateViewAddHomebase(CCmdUI* pCmdUI) 
  {
  	bool enable = RobotWaiting();
  	pCmdUI->Enable(enable);
  }
  
  void CMainFrame::OnShowVideoButton() 
  {
  	// TODO: Add your command handler code here
  	if (m_video.GetSafeHwnd() == NULL)
  	{
  		m_video.Create(IDD_VIDEO, this);
  	}
  	m_video.ShowWindow(SW_SHOW);
  }
