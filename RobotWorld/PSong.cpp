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
  // Song.cpp : implementation file
  //
  //created by Marc Bohlen
  //December 1999
  //Last Revision: December 1999
  //under construction...
  ////////////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "PSong.h"
  #include "Robot.h"
  #include "CyePersona.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  /////////////////////////////////////////////////////////////////////////////
  // CPSong dialog
  
  void CPSong::SetRobot(CRobot *Robot)
  {
  	m_Robot = Robot;
  }
  
  CPSong::CPSong(CWnd* pParent /*=NULL*/)
  	: CDialog(CPSong::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CPSong)
  	//}}AFX_DATA_INIT
  }
  
  
  void CPSong::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CPSong)
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CPSong, CDialog)
  	//{{AFX_MSG_MAP(CPSong)
  	ON_BN_CLICKED(IDC_BUTTON1, OnTestTune)
  	ON_BN_CLICKED(IDC_QuirkyTune1, OnQuirkyTune1)
  	ON_BN_CLICKED(IDC_QuirkyTune2, OnQuirkyTune2)
  	ON_BN_CLICKED(IDC_SadTune, OnSadTune)
  	ON_BN_CLICKED(IDC_SmoothTune, OnSmoothTune)
  	ON_BN_CLICKED(IDC_AgressiveTune1, OnAgressiveTune1)
  	ON_BN_CLICKED(IDC_AgressiveTune2, OnAgressiveTune2)
  	ON_BN_CLICKED(IDC_ContentTune1, OnContentTune1)
  	ON_BN_CLICKED(IDC_ContentTune2, OnContentTune2)
  	ON_BN_CLICKED(IDC_ContentTune3, OnContentTune3)
  	ON_BN_CLICKED(IDC_BoredTune1, OnBoredTune1)
  	ON_BN_CLICKED(IDC_BoredTune2, OnBoredTune2)
  	ON_BN_CLICKED(IDC_BoredTune3, OnBoredTune3)
  	ON_BN_CLICKED(IDC_SwayTune, OnSwayTune)
  	ON_BN_CLICKED(IDC_AcknowledgeTune, OnAcknowledgeTune)
  	ON_BN_CLICKED(IDC_JitteryTune1, OnJitteryTune1)
  	ON_BN_CLICKED(IDC_JitteryTune2, OnJitteryTune2)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CPSong message handlers
  
  void CPSong::OnTestTune() 
  {
  	// TODO: Add your control notification handler code here
  	TestTune();
  	
  }
  
  void CPSong::QuirkyTune1()
  {
  	//short Frequencies1[] = {1000, 900, 800, 700, -1};	//-1 turns cye off ...
  	short Frequencies[] = {1000, 900, 800, 700, 1000, 900, 800, 700, 1000, 900, 800, 700, 1100, 1200, 1300, 1400, -1};
  	int Durations[] = {80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80,80,80,80,0};
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();										
  }
  
  void CPSong::QuirkyTune2()
  {
  	//short Frequencies1[] = {1200, 1100, 1000, 900, -1};		
  	//short Frequencies2[] = {1300, 1400, 1500, 1600, -1};
  
  	short Frequencies[] = {1200, 1100, 1000, 900,1200, 1100, 1000, 900,1200, 1100, 1000, 900,1300, 1400, 1500, 1600,-1};
  	int Durations[] = {80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80,80,80,80,0};
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();										
  }
  
  
  
  void CPSong::SadTune()				
  {
  	short Frequencies[] = {1500,1300,1250,-1};
  	int Durations[] = {500,200,200,0};						
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  }
  
  void CPSong::SmoothTune()				//needs work
  {
  	short Frequencies[] = {900, 950, 1000, 1050, -1};
  	int Durations[] = {200,200,200,200,0};						
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  }
  
  void CPSong::AgressiveTune1()				
  {
  	//short Frequencies[] = {1000,900,800,-1};
  	//int Durations[] = {100,100,100,0};
  	short Frequencies[] = {1000,900,800,1000,900,800,-1};
  	int Durations[] = {100,100,100,100, 100, 100,0};
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  }
  
  void CPSong::AgressiveTune2()				
  {
  	short Frequencies[] = {1300,1500,-1};
  	int Durations[] = {100,100,0};
  	
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  }
  
  void CPSong::ContentTune1()				
  {
  	//short Frequencies[] = {1500,1700,1300,-1};
  	//int Durations[] = {100,200,100,0};
  	short Frequencies[] = {1300,1400,1100,1300,1400,1100,-1};
  	int Durations[] = {100,200,100,100,200,100,0};
  
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  }
  
  void CPSong::ContentTune2()				
  {
  	//short Frequencies[] = {1300,1800,1600,-1};
  	//int Durations[] = {80,80,80,80};
  	short Frequencies[] = {	1100,1600,1400,1100,1600,1400,-1};
  	int Durations[] = {	80,80,80,80,80,80,0};
  	
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  }
  
  void CPSong::ContentTune3()				
  {
  	short Frequencies[] = {1300,1400,1600,-1};
  	int Durations[] = {100,100,200,0};
  
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  
  }
  
  void CPSong::BoredTune1()				
  {
  	short Frequencies[] = {300,150,-1};
  	int Durations[] = {700,900,0};
  
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  
  }
  
  void CPSong::BoredTune2()				
  {
  	short Frequencies[] = {200,100,-1};
  	int Durations[] = {700,900,0};
  
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  	
  }
  
  void CPSong::BoredTune3()				
  {
  	short Frequencies[] = {100,50,-1};
  	int Durations[] = {700,1000,0};
  
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  }
  
  
  void CPSong::SwayTune()				
  {
  	//short Frequencies1[] = {1200, 1250, 1300, -1};
  	//short Frequencies2[] = {1300, 1250, 1200, -1};
  	//int Durations[] = {200,200,200,200,0};
  	short Frequencies[] = {1200, 1250, 1300, 0, 1350, 1250, 1200,-1};
  	int Durations[] = {200,200,200, 300, 200, 200,200,200,200,0};
  
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  }
  
  void CPSong::DizzyTune()				
  {
  	//short Frequencies1[] = {1200, 1250, 1300, -1};
  	//short Frequencies2[] = {1300, 1250, 1200, -1};
  	//int Durations[] = {200,200,200,200,0};
  	short Frequencies[] = {1200,1250, 1300, -1};
  	int Durations[] = {200,200,200,300,0};
  
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  }
  
  void CPSong::AcknowledgeTune()
  {
  	short Frequencies[] = {900,1000,900,1000,-1};
  	int Durations[] = {100, 100, 100, 100, 0};						
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  }
  
  
  void CPSong::JitteryTune1()
  {
  	//short Frequencies[] = {800,1300, -1};
  	//int Durations[] = {100,100,0};
  	short Frequencies[] = {800,1300,800,1300,800,1300,-1};
  	int Durations[] = {100,100,100,100,100,100,0};
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  
  }
  
  void CPSong::JitteryTune2()
  {
  	//short Frequencies[] = {1200,1700, -1};
  	//int Durations[] = {100,100,0};
  	short Frequencies[] = {	1200,1700,1200,1700,1200,1700,-1};
  	int Durations[] = {100,100,100,100,100,100,0};
  
  	m_Robot->AddTuneToInstructions(Frequencies, Durations, sizeof(Frequencies) / sizeof(short));
  	m_Robot->FollowInstructions();
  
  }
  
  /////////////////////////////////////////////////////////////////////////////
  void CPSong::TestTune()
  {
  	short Frequencies1[] = {1000, 900, 800, 700, -1};		
  	short Frequencies2[] = {1100, 1200, 1300, 1400, -1};
  	short Frequencies3[] = {800, 900, 800, 900, -1};
  	short Frequencies4[] = {1500,1700,1300,-1};
  	short Frequencies5[] = {300,150,-1};
  	short Frequencies6[] = {1300,1800,1600,-1};
  	short Frequencies7[] = {1300, 1400,1900,-1};
  	short Frequencies8[] = {1500,1300,1250,-1};
  	short Frequencies9[] = {1300,1500,1700,-1};
  	short Frequencies10[] = {900,950,1000,1050,-1};
  	short Frequencies11[] = {1200,1250,1300,-1};
  	short Frequencies11a[] = {1300,1250,1200,-1};
  	short Frequencies12[] = {1000,800, 1500, -1};
  	short Frequencies13[] = {900,1000,900,1000,-1};
  
  	int Durations1[] = {100, 100, 100, 100, 0};				//no less than 80ms
  	int Durations1a[] = {200,200,200,200,200};
  	int Durations2[] = {80, 80, 80, 80, 80};
  	int Durations3[] = {200, 100, 100,400};
  	int Durations4[] = {200,400,200,0};
  	int Durations5[] = {700,900,0};
  	int Durations6[] = {200,200,200,0};
  	int Durations7[] = {200,200,400,0};
  	int Durations8[] = {500, 200, 200,0};
  	int Durations9[] = {100,100,100,0};
  	int Durations10[] = {200,200,200,200,0};
  							
  	m_Robot->AddTuneToInstructions(Frequencies13, Durations1, sizeof(Frequencies13) / sizeof(short));
  	m_Robot->FollowInstructions();
  	
  }
  /////////////////////////////////////////////////////////////
  void CPSong::OnQuirkyTune1() 
  {
  	QuirkyTune1();	
  }
  
  void CPSong::OnQuirkyTune2() 
  {
  	QuirkyTune2();
  }
  
  void CPSong::OnSadTune() 
  {
  	SadTune();
  }
  
  void CPSong::OnSmoothTune() 
  {
  	SmoothTune();
  }
  
  void CPSong::OnAgressiveTune1() 
  {
  	AgressiveTune1();
  }
  
  void CPSong::OnAgressiveTune2() 
  {
  	AgressiveTune2();
  }
  
  void CPSong::OnContentTune1() 
  {
  	ContentTune1();
  }
  
  void CPSong::OnContentTune2() 
  {
  	ContentTune2();
  }
  
  void CPSong::OnContentTune3() 
  {
  	ContentTune3();
  }
  
  void CPSong::OnBoredTune1() 
  {
  	BoredTune1();
  }
  
  void CPSong::OnBoredTune2() 
  {
  	BoredTune2();
  }
  
  void CPSong::OnBoredTune3() 
  {
  	BoredTune3();
  }
  
  void CPSong::OnSwayTune() 
  {
  	SwayTune();
  }
  
  void CPSong::OnAcknowledgeTune() 
  {
  	AcknowledgeTune();
  }
  
  void CPSong::OnJitteryTune1() 
  {
  	JitteryTune1();
  }
  
  void CPSong::OnJitteryTune2() 
  {
  	JitteryTune2();
  }
