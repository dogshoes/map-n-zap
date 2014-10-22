
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
  // CyePersona.cpp : implementation file
  //
  //created by Marc Bohlen
  //December 1999
  //Last Revision: December 1999
  //under construction...
  ////////////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "CyePersona.h"
  #include "Robot.h"

  /////////////////////////////////////////////////////////////////////////////
  // CCyePersona dialog
  
  
  CCyePersona::CCyePersona(CWnd* pParent /*=NULL*/)
  	: CDialog(CCyePersona::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CCyePersona)
  		// NOTE: the ClassWizard will add member initialization here
  	//}}AFX_DATA_INIT
  }
  
  
  void CCyePersona::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CCyePersona)
  		// NOTE: the ClassWizard will add DDX and DDV calls here
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CCyePersona, CDialog)
  	//{{AFX_MSG_MAP(CCyePersona)	
  	ON_BN_CLICKED(IDC_ShowSong, OnShowSong)
  	ON_BN_CLICKED(IDC_ShowMotion, OnShowMotion)
  	ON_BN_CLICKED(IDC_ShowBehavior, OnShowBehavior)	
  	ON_WM_CREATE()
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CCyePersona message handlers
  
  void CCyePersona::SetRobot(CRobot *Robot)
  {
  	m_Robot = Robot;
  }
  
  int CCyePersona::OnCreate(LPCREATESTRUCT lpCreateStruct) 
  {
  	if (CDialog::OnCreate(lpCreateStruct) == -1)
  		return -1;
  
  	if(m_PSongDialog.GetSafeHwnd() == NULL)
  	{
  		m_PSongDialog.Create(IDD_CYE_PERSONA_SONG,this);
  		m_PSongDialog.SetRobot(m_Robot);
  	}	
  
  	if(m_PMotionDialog.GetSafeHwnd() == NULL)
  	{
  		m_PMotionDialog.Create(IDD_CYE_PERSONA_MOTION,this);
  		m_PMotionDialog.SetRobot(m_Robot);
  	}
  
  
  	if(m_PBehaviorDialog.GetSafeHwnd() == NULL)
  	{
  		m_PBehaviorDialog.Create(IDD_CYE_PERSONA_BEHAVIOR,this);
  		m_PBehaviorDialog.SetRobot(m_Robot);
  		m_PBehaviorDialog.SetMotion(&m_PMotionDialog);
  		m_PBehaviorDialog.SetSong(&m_PSongDialog);
  	}
  	
  	return 0;
  }
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  
  void CCyePersona::SetOnHomeBase(bool OnHome)
  {
  
  }
  
  void CCyePersona::OnShowSong() 
  {
  	
  	m_PSongDialog.ShowWindow(SW_SHOW);	
  }
  
  void CCyePersona::OnShowMotion() 
  {
  
  	m_PMotionDialog.ShowWindow(SW_SHOW);
  }
  
  void CCyePersona::OnShowBehavior() 
  {
  	
  	m_PBehaviorDialog.ShowWindow(SW_SHOW);
  }
  
  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////
   
  CString CCyePersona::GetQualityTime()
  {
  	CString TimeOfDay;
  	CTime PresentTime = CTime::GetCurrentTime();
  	CTime PresentHour = PresentTime.GetHour();
  	 
  	//can not use n <= PresentHour <= m !!
  
  	if (PresentHour >= 22)				//22,23,24			
  		{
  		AfxMessageBox("LateNight");
  		TimeOfDay = "LateNight";
  		}
  	else if(PresentHour >=20)			//20,21
  		{
  		AfxMessageBox("MidEvening");
  		TimeOfDay = "MidEvening";
  		}	
  	else if(PresentHour >= 18)			//18,19
  		{
  		AfxMessageBox("EarlyEvening");
  		TimeOfDay = "EarlyEvening";
  		}
  	else if(PresentHour >= 15)			//15,16,17
  		{
  		AfxMessageBox("LateAfterNoon");
  		TimeOfDay = "LateAfterNoon";
  		}
  	else if(PresentHour >= 12)			//12,13,14 
  		{
  		AfxMessageBox("EarlyAfterNoon ");
  		TimeOfDay = "EarlyAfterNoon";
  		}
  	else if(PresentHour >= 10)			//10,11
  		{
  		AfxMessageBox("LateMorning");
  		TimeOfDay = "LateMorning";
  		}
  	else if(PresentHour >= 7)			//7,8,9
  		{
  		AfxMessageBox("MidMorning");
  		TimeOfDay = "MidMorning";
  		}
  	else if(PresentHour >= 4)			//4,5,6
  		{
  		AfxMessageBox("EarlyMorning");
  		TimeOfDay = "EarlyMorning";
  		}
  	else
  		{
  		AfxMessageBox("Between 12 midnight and 4 am");
  		TimeOfDay = "OffHours";
  		}
  	return TimeOfDay;	
  }
  
  bool CCyePersona::IsOddNumber(short Number) 
  {
  //returns 1 if number is odd, else 0
  	bool IsOdd;
  
  	if ((Number % 2) == 1) 
  		IsOdd = 1;
  	else
  		IsOdd = 0;
  
  	return  IsOdd;
  }
  
  short CCyePersona::GetRandomNumber(short MyBound)
  {
  //returns a random number between 1 and MyBound
  	short RandNum;	 	
  	srand( (unsigned int)time( (time_t *)NULL ) ); 
  	RandNum = (rand() * MyBound / (RAND_MAX + 1)) +  1; 
  	return RandNum;
  }
  
