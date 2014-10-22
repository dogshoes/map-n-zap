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
  // RobotWorld.cpp : Defines the class behaviors for the application.
  //
  
  #include "stdafx.h"
  
  #include "RobotWorld.h"
  
  #include "MainFrm.h"
  #include "RobotWorldDoc.h"
  #include "RobotWorldView.h"

  #define GENERAL_SECTION "GENERAL"
  #define CURRENT_FILE "CURRENT_FILE"
  #define ROBOT_SOFTWARE "ROBOT SOFTWARE"
  #define ROBOT_ON_TIME "ROBOT ON"
  #define ROBOT_THIN_CARPET_HCF "ROBOT THIN CARPET HCF"
  #define ROBOT_THICK_CARPET_HCF "ROBOT THICK CARPET HCF"
  #define ROBOT_THICK_CARPET_DEFAULT_HCF "ROBOT THICK CARPET DEFAULT HCF"
  #define ROBOT_THIN_CARPET_DEFAULT_HCF "ROBOT THIN CARPET DEFAULT HCF"
  #define ROBOT_WOOD_DEFAULT_HCF "ROBOT WOOD DEFAULT HCF"
  #define ROBOT_WOOD_HCF "ROBOT WOOD HCF"
  #define ROBOT_THIN_CALIBRATED "ROBOT THIN CARPET CALIBRATED"
  #define ROBOT_THICK_CALIBRATED "ROBOT THICK CARPET CALIBRATED"
  #define ROBOT_COLOR_ASSOCIATED_ADDRESSES "ROBOT COLOR ASSOCIATED ADDRESSES"
  #define ROBOT_DEFAULT_VERSION "ROBOT DEFAULT VERSION"
  #define ROBOT_ANIMATE "ROBOT ANIMATE"
  #define ROBOT_DANCING_ENABLE "ROBOT DANCING ENABLE"
  
  /////////////////////////////////////////////////////////////////////////////
  // CRobotWorldApp
  
  BEGIN_MESSAGE_MAP(CRobotWorldApp, CWinApp)
  	//{{AFX_MSG_MAP(CRobotWorldApp)
  	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  	//}}AFX_MSG_MAP
  	// Standard file based document commands
  	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
  	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
  	// Standard print setup command
  	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CRobotWorldApp construction
  #include <fstream>
  #include <iostream>
  #include <cstdio>

  CRobotWorldApp::CRobotWorldApp()
  {
  	freopen("stderr.err","w",stderr);
	freopen("stdout.txt","w",stdout);

	std::cout.sync_with_stdio(false);
	std::cerr.sync_with_stdio(false);

  	// TODO: add construction code here,
  	// Place all significant initialization in InitInstance
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // The one and only CRobotWorldApp object
  
  CRobotWorldApp theApp;
  
  // This identifier was generated to be statistically unique for your app.
  // You may change it if you prefer to choose a specific identifier.
  
  // {DDC1CEE0-E92D-11CF-A7FE-FF3FAF3C334B}
  static const CLSID clsid =
  { 0xddc1cee0, 0xe92d, 0x11cf, { 0xa7, 0xfe, 0xff, 0x3f, 0xaf, 0x3c, 0x33, 0x4b } };
  
  /////////////////////////////////////////////////////////////////////////////
  // CRobotWorldApp initialization
  
  BOOL CRobotWorldApp::InitInstance()
  {
  /*Find the default path*/
  	HKEY hKey;
  	CString SubKey = "CLSID\\{DDC1CEE0-E92D-11CF-A7FE-FF3FAF3C334B}\\InprocServer32";
  //	CString SubKey = "CLSID\\{DDC1CEE0-E92D-11CF-A7FE-FF3FAF3C334B}";
  	bool Success = false;
  	long Result = ERROR_SUCCESS;
  	if ((Result = RegOpenKeyEx(HKEY_CLASSES_ROOT, SubKey, 0, KEY_READ, &hKey)) == ERROR_SUCCESS) {
  		BYTE Data[256];
  		DWORD DataSize = 256;
  		DWORD DataType = REG_SZ;
  		if ((Result = RegQueryValueEx(hKey, NULL, NULL, &DataType, Data, &DataSize)) == ERROR_SUCCESS) {
  			CString Path;
  			for (DWORD i = 0; i < DataSize - 14; i++) {
  				Path += Data[i];
  			}
  			Path += "";
  			m_DefaultPath = Path;
  			m_DefaultPath.GetLength();
  			Success = true;
  		} else {
  			char Buffer[256];
  			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, Result, 0, Buffer, 256, NULL);
  		}
  		RegCloseKey(hKey);
  	} else {
  		char Buffer[256];
  		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, Result, 0, Buffer, 256, NULL);
  	}
  	if (!Success) {
  		char* absPath = _fullpath( NULL, ".", 0);
  		m_DefaultPath = absPath;
  		delete absPath;
  	}
  
  //Handle when I'm running on the development pc
    if (m_DefaultPath.Right(6).CompareNoCase("\\Debug") == 0) {
  		m_DefaultPath = m_DefaultPath.Left(m_DefaultPath.GetLength() - 6);
  	} else if (m_DefaultPath.Right(8).CompareNoCase("\\Release") == 0) {
  		m_DefaultPath = m_DefaultPath.Left(m_DefaultPath.GetLength() - 8);
  	}
  /*
  	m_DefaultPath = m_pszHelpFilePath;
  	CString HelpName = m_pszExeName;
  	HelpName += ".hlp";
  	m_DefaultPath = m_DefaultPath.Left(m_DefaultPath.GetLength() - HelpName.GetLength() - 1);
  
    if (m_DefaultPath.Right(6).CompareNoCase("\\Debug") == 0) {
  		m_DefaultPath = m_DefaultPath.Left(m_DefaultPath.GetLength() - 6);
  	} else if (m_DefaultPath.Right(8).CompareNoCase("\\Release") == 0) {
  		m_DefaultPath = m_DefaultPath.Left(m_DefaultPath.GetLength() - 8);
  	}
  
  */
  /*
  	m_DefaultPath = m_pszHelpFilePath;
  	CString HelpName = m_pszExeName;
  	HelpName += ".hlp";
  	m_DefaultPath = m_DefaultPath.Left(m_DefaultPath.GetLength() - HelpName.GetLength() - 1);
  	
  	WIN32_FIND_DATA FindFileData;
  	HANDLE FileHandle = FindFirstFile(m_DefaultPath + "\\rover.dat", &FindFileData);
  	if (FileHandle == INVALID_HANDLE_VALUE) {
  //	if (!) {
  		if (m_DefaultPath.Right(6).CompareNoCase("\\Debug") == 0) {
  			m_DefaultPath = m_DefaultPath.Left(m_DefaultPath.GetLength() - 6);
  		} else if (m_DefaultPath.Right(8).CompareNoCase("\\Release") == 0) {
  			m_DefaultPath = m_DefaultPath.Left(m_DefaultPath.GetLength() - 8);
  		}
  		FileHandle = FindFirstFile(m_DefaultPath + "\\rover.dat", &FindFileData);
  		ASSERT(FileHandle != INVALID_HANDLE_VALUE);
  	}
  */
  
  
  
  	// Initialize OLE libraries
  	if (!AfxOleInit())
  	{
  		AfxMessageBox(IDP_OLE_INIT_FAILED);
  		return FALSE;
  	}
  
  	AfxEnableControlContainer();
  
  	// Standard initialization
  	// If you are not using these features and wish to reduce the size
  	//  of your final executable, you should remove from the following
  	//  the specific initialization routines you do not need.
  
  #ifdef _AFXDLL
  	Enable3dControls();			// Call this when using MFC in a shared DLL
  #else
  	Enable3dControlsStatic();	// Call this when linking to MFC statically
  #endif
  
  	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
  
  	// Register the application's document templates.  Document templates
  	//  serve as the connection between documents, frame windows and views.
  
  	CSingleDocTemplate* pDocTemplate;
  	pDocTemplate = new CSingleDocTemplate(
  		IDR_MAINFRAME,
  		RUNTIME_CLASS(CRobotWorldDoc),
  		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
  		RUNTIME_CLASS(CRobotWorldView));
  	AddDocTemplate(pDocTemplate);
  
  	// Connect the COleTemplateServer to the document template.
  	//  The COleTemplateServer creates new documents on behalf
  	//  of requesting OLE containers by using information
  	//  specified in the document template.
  	m_server.ConnectTemplate(clsid, pDocTemplate, TRUE);
  		// Note: SDI applications register server objects only if /Embedding
  		//   or /Automation is present on the command line.
  
  	// Enable DDE Execute open		Added 10/14/98 S.R.
  	EnableShellOpen();
  	RegisterShellFileTypes(TRUE);
  
  	// Parse command line for standard shell commands, DDE, file open
  	CCommandLineInfo cmdInfo;
  	ParseCommandLine(cmdInfo);
  
  	// Check to see if launched as OLE server
  	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
  	{
  		// Register all OLE server (factories) as running.  This enables the
  		//  OLE libraries to create objects from other applications.
  		COleTemplateServer::RegisterAll();
  
  		// Application was run with /Embedding or /Automation.  Don't show the
  		//  main window in this case.
  		return TRUE;
  	}
  
  /*Show splash screen*/
  	if ( (!cmdInfo.m_bRunEmbedded) && (cmdInfo.m_bShowSplash) ) {
  		m_WndSplash.Create(CSplashWnd::IDD, NULL);
  		m_WndSplash.ShowWindow(SW_SHOW);
  		m_dwSplashTime = ::GetCurrentTime();
  	}
  
  	// When a server application is launched stand-alone, it is a good idea
  	//  to update the system registry in case it has been damaged.
  	m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
  	COleObjectFactory::UpdateRegistryAll();
  
  	// Dispatch commands specified on the command line
  	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew) {
  	/*Load last file used*/
  		m_CurrentFile = GetProfileString(GENERAL_SECTION, CURRENT_FILE, "");
  		if (m_CurrentFile != "") {
  			CFile CurrentFile;
  			if (CurrentFile.Open(m_CurrentFile, CFile::modeRead | CFile::shareDenyWrite)) {
  				CurrentFile.Close();
  				cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
  				cmdInfo.m_strFileName = m_CurrentFile;
  			}
  		}
  	}
  
  	if (!ProcessShellCommand(cmdInfo))
  		return FALSE;
  
  #ifdef JUNK
  /*Load last file used*/
  	m_CurrentFile = GetProfileString(GENERAL_SECTION, CURRENT_FILE, "");
  	if (m_CurrentFile != "") {
  		CFile CurrentFile;
  		if (!CurrentFile.Open(m_CurrentFile, CFile::modeRead | CFile::shareDenyWrite)) {
  			SetCurrentFile("");
  		} else {
  			CArchive ar(&CurrentFile, CArchive::load);
  			POSITION FirstDoc = pDocTemplate->GetFirstDocPosition();
  			CDocument* document = pDocTemplate->GetNextDoc(FirstDoc);
  			ar.m_pDocument = document;
  			document->SetPathName(m_CurrentFile, FALSE);
  			document->Serialize(ar);
  		}
  	}
  #endif	
  	// Enable drag/drop open	Added 10/14/98 S.R.
  	m_pMainWnd->DragAcceptFiles();
  
  	return TRUE;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // CAboutDlg dialog used for App About
  
  class CAboutDlg : public CDialog
  {
  public:
  	CAboutDlg();
  
  // Dialog Data
  	//{{AFX_DATA(CAboutDlg)
  	enum { IDD = IDD_ABOUTBOX };
  	//}}AFX_DATA
  
  	// ClassWizard generated virtual function overrides
  	//{{AFX_VIRTUAL(CAboutDlg)
  	protected:
  	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  	//}}AFX_VIRTUAL
  
  // Implementation
  protected:
  	//{{AFX_MSG(CAboutDlg)
  	virtual BOOL OnInitDialog();
  	//}}AFX_MSG
  	DECLARE_MESSAGE_MAP()
  };
  
  CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
  {
  	//{{AFX_DATA_INIT(CAboutDlg)
  	//}}AFX_DATA_INIT
  }
  
  void CAboutDlg::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CAboutDlg)
  	//}}AFX_DATA_MAP
  }
  
  BOOL CAboutDlg::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	
  	CString Text = "Robot Software Rev Number ";
  	Text.Format("Robot Software Rev Number: %5.2f", ((CRobotWorldApp*) AfxGetApp())->GetRobotSoftwareRevision());
  	GetDlgItem(IDC_EDT_ROBOT_SOFTWARE_REV)->SetWindowText(Text);
  	Text.Format("Robot Run Time: %d Hours", ((CRobotWorldApp*) AfxGetApp())->GetRobotOnSeconds()/3600);
  	GetDlgItem(IDC_EDT_ROBOT_RUN_TIME)->SetWindowText(Text);
  	
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  	//{{AFX_MSG_MAP(CAboutDlg)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  // App command to run the dialog
  void CRobotWorldApp::OnAppAbout()
  {
  	CAboutDlg aboutDlg;
  	aboutDlg.DoModal();
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // CRobotWorldApp commands
  
  CString CRobotWorldApp::GetCurrentFile()
  {
  	return m_CurrentFile;
  }
  
  void CRobotWorldApp::SetCurrentFile(CString CurrentFile)
  {
  	m_CurrentFile = CurrentFile;
  	WriteProfileString(GENERAL_SECTION, CURRENT_FILE, LPCTSTR(m_CurrentFile));
  	return;
  }
  
  CView* CRobotWorldApp::GetView(void)
  {
  	POSITION Position = GetFirstDocTemplatePosition();
  	CDocTemplate* pFirstDocTemplate = GetNextDocTemplate(Position);
  
  	Position = pFirstDocTemplate->GetFirstDocPosition();
  	CDocument* pDocument = pFirstDocTemplate->GetNextDoc(Position);
  
  	Position = pDocument->GetFirstViewPosition();
  	CView* pView = pDocument->GetNextView(Position);
  	return pView;
  }
  
  int CRobotWorldApp::ExitInstance() 
  {
  	return CWinApp::ExitInstance();
  }
  
  BOOL CRobotWorldApp::OnIdle(LONG lCount)
  {
  	BOOL bResult = CWinApp::OnIdle(lCount);
  
  	if (m_WndSplash.m_hWnd != NULL)
  	{
  		if (::GetCurrentTime() - m_dwSplashTime > 5500)
  		{
  			// timeout expired, destroy the splash window
  //			CWnd* MainFrame = m_WndSplash.GetParent();
  			m_WndSplash.DestroyWindow();
  //			MainFrame->UpdateWindow();
  
  			// NOTE: don't set bResult to FALSE,
  			//  CWinApp::OnIdle may have returned TRUE
  		}
  		else
  		{
  			// check again later...
  			bResult = TRUE;
  		}
  	}
  	return bResult;
  }
  
  void CRobotWorldApp::SetThinCarpetHeadingCorrectionFactor(short HeadingCorrectionFactor)
  {
  	WriteProfileInt(GENERAL_SECTION, ROBOT_THIN_CARPET_HCF, HeadingCorrectionFactor);
  	SetCalibratedThinCarpet(true);
  }
  
  void CRobotWorldApp::SetThickCarpetHeadingCorrectionFactor(short HeadingCorrectionFactor)
  {
  	WriteProfileInt(GENERAL_SECTION, ROBOT_THICK_CARPET_HCF, HeadingCorrectionFactor);
  	SetCalibratedThickCarpet(true);
  }
  
  long CRobotWorldApp::GetRobotOnSeconds(void)
  {
  	return GetProfileInt(GENERAL_SECTION, ROBOT_ON_TIME, 0);
  }
  
  float CRobotWorldApp::GetRobotSoftwareRevision(void)
  {
  	CString Rev = GetProfileString(GENERAL_SECTION, ROBOT_SOFTWARE, "0");
  	return (float)atof(Rev);
  }
  
  void CRobotWorldApp::SetRobotSoftwareRevision(float SoftwareRevision)
  {
  	CString Rev;
  	Rev.Format("%f", SoftwareRevision);
  	WriteProfileString(GENERAL_SECTION, ROBOT_SOFTWARE, Rev);
  }
  
  void CRobotWorldApp::SetRobotOnSeconds(long On)
  {
  	WriteProfileInt(GENERAL_SECTION, ROBOT_ON_TIME, On);
  }
  
  const short cThickCarpetDefaultHeadingCorrectionFactor = -14;
  const short cThinCarpetDefaultHeadingCorrectionFactor = -39;
  const short cWoodDefaultHeadingCorrectionFactor = -39;
  
  short CRobotWorldApp::GetThinCarpetHeadingCorrectionFactor()
  {
  	return GetProfileInt(GENERAL_SECTION, ROBOT_THIN_CARPET_HCF, cThinCarpetDefaultHeadingCorrectionFactor);
  }
  
  short CRobotWorldApp::GetThickCarpetHeadingCorrectionFactor()
  {
  	return GetProfileInt(GENERAL_SECTION, ROBOT_THICK_CARPET_HCF, cThickCarpetDefaultHeadingCorrectionFactor);
  }
  
  int CRobotWorldApp::GetThickCarpetDefaultHeadingCorrectionFactor()
  {
  	return GetProfileInt(GENERAL_SECTION, ROBOT_THICK_CARPET_DEFAULT_HCF, cThickCarpetDefaultHeadingCorrectionFactor);
  }
  
  int CRobotWorldApp::GetThinCarpetDefaultHeadingCorrectionFactor()
  {
  	return GetProfileInt(GENERAL_SECTION, ROBOT_THIN_CARPET_DEFAULT_HCF, cThinCarpetDefaultHeadingCorrectionFactor);
  }
  
  int CRobotWorldApp::GetWoodDefaultHeadingCorrectionFactor()
  {
  	return GetProfileInt(GENERAL_SECTION, ROBOT_WOOD_DEFAULT_HCF, cThinCarpetDefaultHeadingCorrectionFactor);
  }
  
  int CRobotWorldApp::GetWoodHeadingCorrectionFactor()
  {
  	return GetProfileInt(GENERAL_SECTION, ROBOT_WOOD_HCF, cThinCarpetDefaultHeadingCorrectionFactor);
  }
  
  bool CRobotWorldApp::GetCalibratedThinCarpet()
  {
  	CString String = GetProfileString(GENERAL_SECTION, ROBOT_THIN_CALIBRATED, "FALSE");
  	return String == "TRUE";
  }
  
  bool CRobotWorldApp::GetCalibratedThickCarpet()
  {
  	CString String = GetProfileString(GENERAL_SECTION, ROBOT_THICK_CALIBRATED, "FALSE");
  	return String == "TRUE";
  }
  
  void CRobotWorldApp::SetCalibratedThinCarpet(bool Calibrated)
  {
  	CString Value("FALSE");
  	if (Calibrated) Value = "TRUE";
  	WriteProfileString(GENERAL_SECTION, ROBOT_THIN_CALIBRATED, Value);
  }
  
  void CRobotWorldApp::SetCalibratedThickCarpet(bool Calibrated)
  {
  	CString Value("FALSE");
  	if (Calibrated) Value = "TRUE";
  	WriteProfileString(GENERAL_SECTION, ROBOT_THICK_CALIBRATED, Value);
  }
  
  bool CRobotWorldApp::GetColorAssociatedAddresses()
  {
  	CString String = GetProfileString(GENERAL_SECTION, ROBOT_COLOR_ASSOCIATED_ADDRESSES, "FALSE");
  	return String == "TRUE";
  }
  
  int CRobotWorldApp::GetDefaultRobotVersion()
  {
  	return GetProfileInt(GENERAL_SECTION, ROBOT_DEFAULT_VERSION, 2);
  }
  
  void CRobotWorldApp::SetRobotAnimate(bool Enable)
  {
  	CString Value("FALSE");
  	if (Enable) Value = "TRUE";
  	WriteProfileString(GENERAL_SECTION, ROBOT_ANIMATE, Value);
  }
  
  bool CRobotWorldApp::GetRobotAnimate()
  {
  	CString String = GetProfileString(GENERAL_SECTION, ROBOT_ANIMATE, "TRUE");
  	return String == "TRUE";
  }
  
  void CRobotWorldApp::SetEnableDancing(bool Enable)
  {
  	CString Value("FALSE");
  	if (Enable) Value = "TRUE";
  	WriteProfileString(GENERAL_SECTION, ROBOT_DANCING_ENABLE, Value);
  }
  
  bool CRobotWorldApp::GetEnableDancing()
  {
  	CString String = GetProfileString(GENERAL_SECTION, ROBOT_DANCING_ENABLE, "FALSE");
  	return String == "TRUE";
  }
