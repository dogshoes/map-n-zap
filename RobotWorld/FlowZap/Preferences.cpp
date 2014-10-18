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
  // Preferences.cpp : implementation file
  //
  
  #include "stdafx.h"
  //#include "FlowZap.h"
  #include "FlowZapResource.h" 
  #include "Preferences.h"
  #include "util.h"

  /////////////////////////////////////////////////////////////////////////////
  // CPreferences dialog
  
  
  CPreferences::CPreferences(CWnd* pParent /*=NULL*/)
  	: CDialog(CPreferences::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CPreferences)
  	//}}AFX_DATA_INIT
  }
  
  
  void CPreferences::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CPreferences)
  	DDX_Control(pDX, IDC_CHECK4, m_SoundOn);
  	DDX_Control(pDX, IDC_CHECK2, m_CondensedView);
  	DDX_Control(pDX, IDC_CHECK1, m_lastProject);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CPreferences, CDialog)
  	//{{AFX_MSG_MAP(CPreferences)
  	ON_WM_SHOWWINDOW()
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CPreferences message handlers
  
  void CPreferences::OnOK() 
  {
  
  	if (m_CondensedView.GetCheck () == 0)	m_TestCondensedView = FALSE;
  	else m_TestCondensedView = TRUE;
  
  	if (m_lastProject.GetCheck() == 0)		m_TestLastProject = FALSE;
  	else m_TestLastProject = TRUE;
  
  	if (m_SoundOn.GetCheck()== 0)		m_TestSoundOn = FALSE;
  	else m_TestSoundOn = TRUE;
  
  
  	WritePrefs();
  	
  	CDialog::OnOK();
  }
  
  
  void CPreferences::WritePrefsToFile(CStdioFile * prefs)
  {
  	prefs->WriteString ("FLOWZAP Version 1.0 Preferences\n");
  	prefs->WriteString (BoolToString(m_TestLastProject)   + "\n");
  	prefs->WriteString (m_PreviousFileName + "\n");
  	prefs->WriteString (BoolToString(m_TestCondensedView) + "\n");
  	prefs->WriteString (BoolToString(m_TestSoundOn) + "\n");
  
  }
  
  
  void CPreferences::ReadPrefsFromFile(CStdioFile *prefs)
  {
  	CString version, last, condensed, sounds;
  	prefs->ReadString (version);
  	prefs->ReadString (last);				m_TestLastProject =		StringToBool(last);
  	prefs->ReadString (m_PreviousFileName);		
  	prefs->ReadString (condensed);			m_TestCondensedView =	StringToBool(condensed);
  	prefs->ReadString (sounds);				m_TestSoundOn =		StringToBool (sounds);
  		
  	
  	
  
  }
  
  
  void CPreferences::ReadPrefs()
  {
  /*	CStdioFile *prefs = OpenFileForReading("FlowZapPrefs.txt");
  	if (prefs != NULL) {
  		ReadPrefsFromFile (prefs);
  		prefs->Close();
  		delete prefs;
  		prefs = NULL;
  	}
  
  	else {
  		SetDefaultPrefs ();
  	}
  */
  	CWinApp* MainApp = AfxGetApp();
  	m_TestLastProject = MainApp->GetProfileInt("FlowZap", "TestLastProject", 0);
  	m_PreviousFileName = MainApp->GetProfileString("FlowZap", "PreviousFileName", "");
  	m_TestCondensedView = MainApp->GetProfileInt("FlowZap", "TestCondensedView", 0);
  	m_TestSoundOn = MainApp->GetProfileInt("FlowZap", "TestSoundOn", 1);
  }
  
  
  void CPreferences::SetDefaultPrefs(){
  /************************************************************************************************
  * DATE CREATED: 
  *	24 Feb 98 12:56 PM EST
  * LAST MODIFIED:
  *	24 Feb 98 12:56 PM EST
  * PARAMETERS:
  *	none
  * RETURN
  *	void
  ************************************************************************************************
  * DESCRIPTION
  *	if the preferences file can not be found, create a new Preferences File and set the defaults
  * METHOD:
  *
  ************************************************************************************************/
  
  	m_TestLastProject   = FALSE;
  	m_PreviousFileName  = "sample.zap";		
  	m_TestCondensedView = FALSE;
  	m_TestSoundOn       = FALSE;
  	WritePrefs();
  
  }
  
  
  
  void CPreferences::OnShowWindow(BOOL bShow, UINT nStatus) 
  {
  	CDialog::OnShowWindow(bShow, nStatus);
  //	ReadPrefs();
  	if (m_TestLastProject)		m_lastProject.SetCheck(1);
  	else						m_lastProject.SetCheck(0);
  
  	if (m_TestCondensedView)	m_CondensedView.SetCheck(1);
  	else						m_CondensedView.SetCheck(0);
  
  	
  	if (m_TestSoundOn)			m_SoundOn.SetCheck(1);
  	else						m_SoundOn.SetCheck(0);
  	
  }
  
  void CPreferences::WritePrefs()
  {
  /*	CStdioFile *prefs = OpenFileForWriting(DefaultDirectory + "\FlowZapPrefs.txt");
  	if (prefs != NULL) {
  		WritePrefsToFile (prefs);
  	}
  	prefs->Close();
  	delete prefs;
  	prefs = NULL;
  */
  	CWinApp* MainApp = AfxGetApp();
  	MainApp->WriteProfileInt("FlowZap", "TestLastProject", m_TestLastProject);
  	MainApp->WriteProfileString("FlowZap", "PreviousFileName", m_PreviousFileName);
  	MainApp->WriteProfileInt("FlowZap", "TestCondensedView", m_TestCondensedView);
  	MainApp->WriteProfileInt("FlowZap", "TestSoundOn", m_TestSoundOn);
  }
  
  
