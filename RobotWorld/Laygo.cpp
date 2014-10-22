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
  // Laygo.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "Laygo.h"
  #include "RobotWorldView.h"
  #include "Util.h"
  //#include "hlp\RobotWorld.hm"

  #define PATH_NAME "/Laygo/"
  
  
  /////////////////////////////////////////////////////////////////////////////
  // CLaygo dialog
  
  
  CLaygo::CLaygo(CWnd* pParent /*=NULL*/)
  	: CDialog(CLaygo::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CLaygo)
  		// NOTE: the ClassWizard will add member initialization here
  	//}}AFX_DATA_INIT
  
  	m_ShowTip = TRUE;
  	m_ShownJoystick = false;
  	m_ShownFlowZap = false;
  	m_EnableLaygo = true;
  	m_CurrentState = 0;
  	m_RobotWorldView = NULL;
  	m_hbmRobot = NULL;
  	m_dlgChooseRobotColor = NULL;
  }
  
  
  void CLaygo::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CLaygo)
  	DDX_Control(pDX, IDC_BMP_LAYGO_IMAGE, m_bmpLaygoImage);
  	DDX_Control(pDX, IDOK, m_OKButton);
  	DDX_Control(pDX, IDCANCEL, m_CancelButton);
  	DDX_Control(pDX, IDC_DONTSHOW, m_DontShow);
  	DDX_Control(pDX, IDC_INFO_VIEWER, m_InfoViewerCtrl);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CLaygo, CDialog)
  	//{{AFX_MSG_MAP(CLaygo)
  	ON_BN_CLICKED(IDC_DONTSHOW, OnDontshow)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CLaygo message handlers
  
  BOOL CLaygo::OnInitDialog() 
  {
  //	m_CurrentState = 0;
  	CDialog::OnInitDialog();
  	CRect CancelLocation;
  	m_OKButton.GetWindowRect(&CancelLocation);
  	ScreenToClient(&CancelLocation);
  	CRect editLocation(10,10,CancelLocation.BottomRight().x ,CancelLocation.TopLeft().y - 5 );
  
  	if (m_CurrentState == lgoHomeBaseCreateState) Configure(m_CurrentState);
  
  //	ShowWindow(SW_HIDE);
  
  	
  	// CG: The following block was added by the ToolTips component.
  	{
  		// Create the ToolTip control.
  		m_tooltip.Create(this);
  		m_tooltip.Activate(TRUE);
  
  		// TODO: Use one of the following forms to add controls:
  		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
  		m_tooltip.AddTool(GetDlgItem(IDCANCEL), "Click here when you are done to close this window");
  	}
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  #include <atlbase.h>
  //key represents which tip to show
  void CLaygo::Configure(int key, bool SaveState)
  {
  	CRect Rect;
  	m_InfoViewerCtrl.GetClientRect(&Rect);
  	m_InfoViewerCtrl.SetRightMargin(Rect.Width() - 15.0f);
  //	if (IsWindowVisible()) return;
  	if (key < lgoWelcome) key = lgoWelcome;
  	if (key <= cNonlinearLaygoes) {
  		if (SaveState) m_CurrentState = key;
  	} else {
  	}
  	m_CurrentShowing = key;
  
  	// Load the correct tip file (includes images)
  	CString fileName;
  	CString bitmapfilename;
  	GetTipFileNames(key, fileName, bitmapfilename);
  	HBITMAP hLaygoBitmap = LoadBitmap(bitmapfilename);
  	LoadTipText(fileName);
  
  	if (m_dlgOrientHomebase.m_hWnd == NULL) {
  		m_dlgOrientHomebase.Create(m_dlgOrientHomebase.IDD, this);
  		m_dlgHomebaseCorner.Create(m_dlgHomebaseCorner.IDD, this);
  		m_dlgHomebaseFloorType.Create(m_dlgHomebaseFloorType.IDD, this);
  	}
  
  	if (m_dlgChooseRobotColor != NULL) {
  		m_dlgChooseRobotColor->ShowWindow(SW_HIDE);
  	}
  	m_dlgOrientHomebase.ShowWindow(SW_HIDE);
  	m_dlgHomebaseCorner.ShowWindow(SW_HIDE);
  	m_dlgHomebaseFloorType.ShowWindow(SW_HIDE);
  	m_OKButton.ShowWindow(SW_HIDE);
  	m_CancelButton.ShowWindow(SW_SHOW);
  	m_CancelButton.EnableWindow(TRUE);
  	m_CancelButton.SetWindowText("OK, I'm Done");
  
  	m_bmpLaygoImage.ShowWindow(SW_SHOW);
  
  	CRect WindowRect, WindowRect1;
  	HBITMAP OldBitmap = NULL;
  	switch (key) {
  		case lgoWelcome: 
  //			m_OKButton.SetWindowText("Getting Started");
  //			m_OKButton.ShowWindow(SW_SHOW);
  //			m_CancelButton.ShowWindow(SW_HIDE);
  			m_CancelButton.SetWindowText("Getting Started");
  //			m_tooltip.AddTool(GetDlgItem(IDOK), "Click here to get started");
  			SetWindowText ("Learn as you go [Welcome to RobotWorld]");
  			OldBitmap = m_bmpLaygoImage.SetBitmap(hLaygoBitmap);
  			break;
  		case lgoChooseRobotColor:
  			m_CancelButton.SetWindowText("Next");
  			SetWindowText ("Learn as you go [Choosing Cye's Color]");
  			if (m_dlgChooseRobotColor == NULL) {
  				m_dlgChooseRobotColor = new CPropRobotColor();
  				ASSERT(m_dlgChooseRobotColor != NULL);
  				m_dlgChooseRobotColor->Create(IDD_PROP_ROBOT_COLOR, this);
  				m_dlgChooseRobotColor->ModifyStyle(WS_BORDER, 0, 0);
  			}
  			m_bmpLaygoImage.GetWindowRect(&WindowRect);
  			m_bmpLaygoImage.ShowWindow(SW_HIDE);
  			ScreenToClient(&WindowRect);
  			m_dlgChooseRobotColor->MoveWindow(WindowRect);
  			m_dlgChooseRobotColor->ShowWindow(SW_SHOW);
  			m_dlgChooseRobotColor->SetRobotColor(m_RobotWorldView->GetRobotColor());
  			m_dlgChooseRobotColor->EnableWindow(TRUE);
  
  			break;
  		case lgoComPortSearch:
  			m_CancelButton.SetWindowText("Next");
  			SetWindowText ("Learn as you go [Searching for the Radio Base]");
  			OldBitmap = m_bmpLaygoImage.SetBitmap(hLaygoBitmap);
  			if (m_RobotWorldView->GetCommunicationsEstablished()) {
  				Configure(++key, SaveState);
  //				MessageBox("Established communications with robot.", "Communications Established", MB_ICONINFORMATION | MB_OK);
  			} else {
  				m_RobotWorldView->FindComPort();
  			}
  			break;
  		case lgoHomeBaseCreateState:
  		case lgoHomeBaseCreate2State: 
  //			if (m_RobotWorldView->m_HomeBase == NULL) {
  //				m_CancelButton.SetWindowText("Create Home Base");
  //			} else {
  //				m_CancelButton.SetWindowText("OK, I'm Done");
  //			}
  //			m_CancelButton.EnableWindow(FALSE);
  			if (m_RobotWorldView->m_HomeBase != NULL) {
  				Configure(++key, SaveState);
  			} else {
  				m_CancelButton.ShowWindow(SW_HIDE);
  				m_OKButton.SetWindowText("Show me more");
  				SetWindowText ("Learn as you go [Set up the Home Base]");
  				m_bmpLaygoImage.GetWindowRect(&WindowRect);
  				ScreenToClient(&WindowRect);
  				m_dlgOrientHomebase.GetWindowRect(&WindowRect1);
  				ScreenToClient(&WindowRect1);
  				WindowRect.right = WindowRect.left + WindowRect1.Width();
  				WindowRect.bottom = WindowRect.top + WindowRect1.Height();
  				m_dlgOrientHomebase.m_Laygo = this;
  				m_dlgHomebaseCorner.m_Laygo = this;
  				m_dlgHomebaseFloorType.m_Laygo = this;
  				m_dlgOrientHomebase.SetRobotBitmap(m_RobotWorldView->GetLargeRobotBitmap());
  				m_dlgHomebaseCorner.SetRobotBitmap(m_RobotWorldView->GetLargeRobotBitmap());
  				m_dlgOrientHomebase.m_wndNext = &m_dlgHomebaseCorner;
  				m_dlgOrientHomebase.MoveWindow(WindowRect);
  				m_dlgOrientHomebase.ShowWindow(SW_SHOW);
  				m_dlgHomebaseCorner.m_wndPrevious = &m_dlgOrientHomebase;
  				m_dlgHomebaseCorner.m_wndNext = &m_dlgHomebaseFloorType;
  				m_dlgHomebaseCorner.MoveWindow(WindowRect);
  				m_dlgHomebaseCorner.ShowWindow(SW_HIDE);
  				m_dlgHomebaseFloorType.m_wndPrevious = &m_dlgHomebaseCorner;
  				m_dlgHomebaseFloorType.MoveWindow(WindowRect);
  				m_dlgHomebaseFloorType.ShowWindow(SW_HIDE);
  
  				m_bmpLaygoImage.ShowWindow(SW_HIDE);
  			}
  			break;
  		case lgoNavigating: 
  			m_tooltip.AddTool (GetDlgItem(IDOK), "Click here to bring up the help file on Navigation");
  			m_OKButton.SetWindowText("Show me more");
  			SetWindowText ("Learn as you go [Navigating the Robot]");
  			OldBitmap = m_bmpLaygoImage.SetBitmap(hLaygoBitmap);
  			break;
  		case lgoOccupancyGrid: 
  			m_tooltip.AddTool (GetDlgItem(IDOK), "Click here to bring up the help file on the Occupancy Grid");
  			m_OKButton.SetWindowText("Show me more");
  			SetWindowText ("Learn as you go [Occupancy Grid]");
  			OldBitmap = m_bmpLaygoImage.SetBitmap(hLaygoBitmap);
  			break;
  		case lgoPositionUncertainty: 
  			m_tooltip.AddTool (GetDlgItem(IDOK), "Click here to bring up the help file on Calibration");
  			m_OKButton.SetWindowText("Show me more");
  			SetWindowText ("Learn as you go [Robot Position Uncertainty]");
  			OldBitmap = m_bmpLaygoImage.SetBitmap(hLaygoBitmap);
  			break;
  		case lgoCheckPoint: 
  			m_tooltip.AddTool (GetDlgItem(IDOK), "Click here to bring up the help file on Check Points");
  			m_OKButton.SetWindowText("Show me more");
  			SetWindowText ("Learn as you go [Robot Check Point]");
  			OldBitmap = m_bmpLaygoImage.SetBitmap(hLaygoBitmap);
  			break;
  		case lgoAdditionalCheckPoints:
  			m_tooltip.AddTool (GetDlgItem(IDOK), "Click here to bring up the help file on Setting up Check Points");
  			m_OKButton.SetWindowText("Show me more");
  			SetWindowText ("Learn as you go [Creating Additional Check Points]");
  			OldBitmap = m_bmpLaygoImage.SetBitmap(hLaygoBitmap);
  			break;
  		case lgoHotPoints: 
  			m_tooltip.AddTool (GetDlgItem(IDOK), "Click here to bring up the help file on HotPoints");
  			m_OKButton.SetWindowText("Show me more");
  			SetWindowText ("Learn as you go [HotPoints]");
  			OldBitmap = m_bmpLaygoImage.SetBitmap(hLaygoBitmap);
  			break;
  		case lgoVacuumPaths: 
  			m_tooltip.AddTool (GetDlgItem(IDOK), "Click here to bring up the help file on Vacuum Paths");
  			m_OKButton.SetWindowText("Show me more");
  			SetWindowText ("Learn as you go [Vacuum Paths]");
  			OldBitmap = m_bmpLaygoImage.SetBitmap(hLaygoBitmap);
  			break;
  		case lgoDangerZones:
  			m_tooltip.AddTool (GetDlgItem(IDOK), "Click here to bring up the help file on Danger Zones");
  			m_OKButton.SetWindowText("Show me more");
  			SetWindowText ("Learn as you go [Danger Zones]");
  			OldBitmap = m_bmpLaygoImage.SetBitmap(hLaygoBitmap);
  			break;
  		case lgoFlowZap:
  			m_ShownFlowZap = true;
  			m_tooltip.AddTool (GetDlgItem(IDOK), "Click here to bring up the help file on Zap!");
  			m_OKButton.SetWindowText("Show me more on Zap!");
  			SetWindowText ("Learn as you go [FlowZap Tool]");
  			OldBitmap = m_bmpLaygoImage.SetBitmap(hLaygoBitmap);
  			break;
  		case lgoJoystick: 
  			m_ShownJoystick = true;
  			m_tooltip.AddTool(GetDlgItem(IDOK), "Click here to bring up the help file on Coordinate Systems");
  			m_OKButton.SetWindowText("Show me more");
  			m_OKButton.ShowWindow(SW_HIDE);
  			SetWindowText ("Learn as you go [Joystick Coordinate Systems]");
  			OldBitmap = m_bmpLaygoImage.SetBitmap(hLaygoBitmap);
  			break;
  	}
  	if (OldBitmap != NULL) {
  		if (!DeleteObject(OldBitmap)) {
  			AfxMessageBox("AAAH!!! CLaygo::Configure Delete Object failed");
  		}
  	}
  
  
  }
  
  void CLaygo::OnCancel() 
  {
  	switch (m_CurrentShowing) {
  	case lgoWelcome:
  		Configure (lgoWelcome+1);
  		break;
  	case lgoChooseRobotColor:
  		m_RobotWorldView->SetRobotColor(m_dlgChooseRobotColor->GetRobotColor());
  		Configure(lgoChooseRobotColor+1);
  		ASSERT(m_dlgChooseRobotColor != NULL);
  		m_dlgChooseRobotColor->ShowWindow(SW_HIDE);
  		break;
  	case lgoComPortSearch:
  		Configure(lgoComPortSearch+1);
  		break;
  	case lgoHomeBaseCreateState:
  	case lgoHomeBaseCreate2State:
  		CreateHomeBase();
  		break;
  	default:
  		CDialog::OnCancel();
  		break;
  	}
  }
  
  void CLaygo::OnOK() 
  {
  	switch (m_CurrentShowing) {
  	case lgoWelcome:
  		Configure(lgoWelcome+1);
  		break;
  	case lgoComPortSearch:
  		Configure(lgoComPortSearch+1);
  		break;
  	default:
  		CWinApp* cwnapp = (CWinApp*)AfxGetApp();
  		cwnapp->WinHelp(0x18020, HELP_CONTEXT);
  		break;
  	}
  	
  //	CDialog::OnOK();
  }
  
  void CLaygo::ResetLaygoTips()
  {
  	m_CurrentState = 0;
  	m_ShownJoystick = false;
  	m_ShownFlowZap = false;
  }
  
  int CLaygo::GetCurrentState()
  {
  	return m_CurrentState;
  }
  
  
  
  
  
  BOOL CLaygo::ShowTip()
  {
  	return m_ShowTip;
  }
  
  void CLaygo::OnDontshow() 
  {
  	m_ShowTip = !m_ShowTip;
  	
  }
  
  BOOL CLaygo::PreTranslateMessage(MSG* pMsg)
  {
  	// CG: The following block was added by the ToolTips component.
  	{
  		// Let the ToolTip process this message.
  		m_tooltip.RelayEvent(pMsg);
  	}
  	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
  }
  
  void CLaygo::SetCurrentState(int State)
  {
  	m_CurrentState = State;
  }
  
  void CLaygo::SetRobotBitmap(HBITMAP hbmRobot)
  {
  	m_hbmRobot = hbmRobot;
  	m_dlgOrientHomebase.SetRobotBitmap(m_hbmRobot);
  	m_dlgHomebaseCorner.SetRobotBitmap(m_hbmRobot);
  }
  
  void CLaygo::SetRobotWorldView(CRobotWorldView * RobotWorldView)
  {
  	m_RobotWorldView = RobotWorldView;
  }
  
  #define LAYGO_SECTION "LAYGO_SECTION"
  #define ENABLE_LAYGO "ENABLE_LAYGO"
  #define LAYGO_STEP "LAYGO_STEP"
  #define LAYGO_SHOWN_JOYSTICK "SHOWN_JOYSTICK"
  #define LAYGO_SHOWN_FLOWZAP "SHOWN_FLOWZAP"
  #define LAYGO_VERSION "VERSION"
  
  void CLaygo::LoadState()
  {
  	CString ProfileString;
  	CWinApp* App = AfxGetApp();
  	int LaygoVersion = App->GetProfileInt(LAYGO_SECTION, LAYGO_VERSION, 1);
  	ProfileString = App->GetProfileString(LAYGO_SECTION, ENABLE_LAYGO, "TRUE");
  	m_EnableLaygo = (ProfileString == "TRUE");
  	ProfileString = App->GetProfileString(LAYGO_SECTION, LAYGO_SHOWN_JOYSTICK, "FALSE");
  	SetShownJoystick(ProfileString == "TRUE");
  	ProfileString = App->GetProfileString(LAYGO_SECTION, LAYGO_SHOWN_FLOWZAP, "FALSE");
  	SetShownFlowZap(ProfileString == "TRUE");
  	ProfileString = App->GetProfileString(LAYGO_SECTION, LAYGO_STEP, "0");
  	int LaygoStep = atoi(ProfileString);
  	if (LaygoStep == lgoComPortSearch) LaygoStep--;
  	if (LaygoStep > cNonlinearLaygoes) LaygoStep = cNonlinearLaygoes;
  	else if (LaygoStep < 0) LaygoStep = 0;
  	SetCurrentState(LaygoStep);
  }
  
  void CLaygo::SaveState()
  {
  	CString ProfileString;
  	CWinApp* App = AfxGetApp();
  	App->WriteProfileInt(LAYGO_SECTION, LAYGO_VERSION, 1);
  	if (m_EnableLaygo) {
  		App->WriteProfileString(LAYGO_SECTION, ENABLE_LAYGO, "TRUE");
  	} else {
  		App->WriteProfileString(LAYGO_SECTION, ENABLE_LAYGO, "FALSE");
  	}
  	if (GetShownJoystick()) {
  		App->WriteProfileString(LAYGO_SECTION, LAYGO_SHOWN_JOYSTICK, "TRUE");
  	} else {
  		App->WriteProfileString(LAYGO_SECTION, LAYGO_SHOWN_JOYSTICK, "FALSE");
  	}
  	if (GetShownFlowZap()) {
  		App->WriteProfileString(LAYGO_SECTION, LAYGO_SHOWN_FLOWZAP, "TRUE");
  	} else {
  		App->WriteProfileString(LAYGO_SECTION, LAYGO_SHOWN_FLOWZAP, "FALSE");
  	}
  	char tempString[10];
  	itoa(GetCurrentState(), tempString, 10);
  	App->WriteProfileString(LAYGO_SECTION, LAYGO_STEP, tempString);
  }
  
  int CLaygo::GetHomeBaseCreateFloorType() {
  	TFloorType FloorType = ftThinCarpet;
  	switch (m_dlgHomebaseFloorType.m_radFloorType) {
  	case 0:
  		FloorType = ftWood;
  		break;
  	case 1:
  		FloorType = ftThinCarpet;
  		break;
  	case 2:
  		FloorType = ftThickCarpet;
  		break;
  	default:
  		ASSERT(FALSE);
  		break;
  	}
  	return FloorType;
  }
  
  void CLaygo::CreateHomeBase()
  {
  	m_CancelButton.EnableWindow(FALSE);
  	TFloorType FloorType;
  	m_dlgHomebaseFloorType.UpdateData(TRUE);
  	FloorType = (TFloorType)GetHomeBaseCreateFloorType();
  	bool CreateResult;
  	CreateResult = m_RobotWorldView->CreateHomebase(m_dlgHomebaseCorner.GetHomebaseHeading(), m_dlgHomebaseCorner.GetCornerToLeft(), FloorType);
  	m_CancelButton.EnableWindow(TRUE);
  	if (CreateResult) {
  		m_bmpLaygoImage.ShowWindow(SW_SHOW);
  		m_dlgOrientHomebase.ShowWindow(SW_HIDE);
  		m_dlgHomebaseCorner.ShowWindow(SW_HIDE);
  		m_dlgHomebaseFloorType.ShowWindow(SW_HIDE);
  		if (m_CurrentShowing == lgoHomeBaseCreateState) {
  			Configure(lgoHomeBaseCreateState+1);
  			ShowWindow(SW_SHOW);
  		} else {
  			CDialog::OnCancel();
  		}
  	} else {
  		Configure(m_CurrentShowing);
  		ShowWindow(SW_SHOW);
  	}
  }
  
  void CLaygo::GetTipFileNames(int key, CString& TipFileName, CString& BitmapFileName)
  {
  	CString BaseName = GetTipBaseName(key);
  	TipFileName = BaseName + ".rtf";
  	BitmapFileName = BaseName + ".bmp";
  }
  
  CString CLaygo::GetTipBaseName(int key)
  {
  	CString BaseName;
  	switch (key) {
  	case lgoWelcome:
  		BaseName = "WelcomeTip";
  		break;
  	case lgoChooseRobotColor:
  		BaseName = "ChooseRobotColorTip";
  		break;
  	case lgoComPortSearch:
  		BaseName = "ComPortSearchTip";
  		break;
  	case lgoHomeBaseCreateState:
  		BaseName = "HomeBaseTip1";
  //		BaseName = "tip3";
  		break;
  	case lgoNavigating:
  		BaseName = "NavigatingTip";
  		break;
  	case lgoOccupancyGrid:
  		BaseName = "OccupancyGridTip";
  		break;
  	case lgoPositionUncertainty:
  		BaseName = "PositionUncertaintyTip";
  		break;
  	case lgoCheckPoint:
  		BaseName = "CheckPointTip";
  		break;
  	case lgoAdditionalCheckPoints:
  		BaseName = "AdditionCheckPointsTip";
  		break;
  	case lgoHotPoints:
  		BaseName = "HotPointsTip";
  		break;
  	case lgoVacuumPaths:
  		BaseName = "VacuumPathsTip";
  		break;
  	case lgoDangerZones:
  		BaseName = "DangerZonesTip";
  		break;
  	case lgoFlowZap:
  		BaseName = "FlowZapTip";
  		break;
  	case lgoJoystick:
  		BaseName = "JoystickTip";
  		break;
  	case lgoHomeBaseCreate2State:
  		BaseName = "HomeBaseTip1";
  //		BaseName = "tip98";
  		break;
  	default: 
  		ASSERT(FALSE);
  		break;
  	}
  	return BaseName;
  }
  
  void CLaygo::LoadTipText(CString FileName)
  {
  	CString AppPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  	AppPath += (CString)(PATH_NAME) + FileName;
  	CComVariant temp(0);
  	m_InfoViewerCtrl.LoadFile(AppPath, temp);
  }
  
  
  bool CLaygo::HomeBaseCreateFloorTypeAlreadyCalibrated()
  {
  	return m_RobotWorldView->GetAlreadyCalibratedFloorType((TFloorType)GetHomeBaseCreateFloorType());
  }
  
  HBITMAP CLaygo::LoadBitmap(CString BitmapFilename)
  {
  	CString AppPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  	AppPath += (CString)(PATH_NAME) + BitmapFilename;
  
  	HBITMAP hLaygoBitmap = (HBITMAP)LoadImage(NULL, AppPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
  	return hLaygoBitmap;
  }
  
  CLaygo::~CLaygo()
  {
  	if (m_dlgChooseRobotColor != NULL) delete m_dlgChooseRobotColor;
  }
