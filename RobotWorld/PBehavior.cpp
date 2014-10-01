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

  // PBehavior.cpp : implementation file
  //
  //created by Marc Bohlen
  //December 1999
  //Last Revision: December 1999
  //under construction...
  ////////////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "PBehavior.h"
  #include "Robot.h"
  #include "PMotion.h"
  #include "PSong.h"
  #include "CyePersona.h"
  
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  short Vf = 300;		 
  short Vn = 200;
  short Vns = 150;
  short Vm = 100;
  short Vc = 80;
  short Vs = 50;
  /////////////////////////////////////////////////////////////////////////////
  // CPBehavior dialog
  
  
  void CPBehavior::SetRobot(CRobot *Robot)
  {
  	m_Robot = Robot;
  }
  
  CPBehavior::CPBehavior(CWnd* pParent /*=NULL*/)
  	: CDialog(CPBehavior::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CPBehavior)
  		// NOTE: the ClassWizard will add member initialization here
  	//}}AFX_DATA_INIT
  }
  
  
  void CPBehavior::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CPBehavior)
  		// NOTE: the ClassWizard will add DDX and DDV calls here
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CPBehavior, CDialog)
  	//{{AFX_MSG_MAP(CPBehavior)
  //	ON_BN_CLICKED(IDC_Test, OnTest)
  	ON_BN_CLICKED(IDC_Agressive1, OnAgressive1)
  	ON_BN_CLICKED(IDC_Agressive2, OnAgressive2)
  //	ON_BN_CLICKED(IDC_BackHome, OnBackHome)
  	ON_BN_CLICKED(IDC_Quirky1, OnQuirky1)
  	ON_BN_CLICKED(IDC_Quirky2, OnQuirky2)
  	ON_BN_CLICKED(IDC_Wiggle, OnWiggle)
  	ON_BN_CLICKED(IDC_Jittery1, OnJittery1)
  	ON_BN_CLICKED(IDC_Jittery2, OnJittery2)
  	ON_BN_CLICKED(IDC_Mad1, OnMad1)
  	ON_BN_CLICKED(IDC_Impatient1, OnImpatient1)
  	ON_BN_CLICKED(IDC_Impatient2, OnImpatient2)
  	ON_BN_CLICKED(IDC_Impatient3, OnImpatient3)
  	ON_BN_CLICKED(IDC_Content1, OnContent1)
  	ON_BN_CLICKED(IDC_Content2, OnContent2)
  	ON_BN_CLICKED(IDC_Content3, OnContent3)
  	ON_BN_CLICKED(IDC_Content4, OnContent4)
  	ON_BN_CLICKED(IDC_SpinnSway, OnSpinnSway)
  	ON_BN_CLICKED(IDC_Cross, OnCross)
  	ON_BN_CLICKED(IDC_Eight, OnEight)
  	ON_BN_CLICKED(IDC_Spinn, OnSpinn)
  	ON_BN_CLICKED(IDC_Spiral, OnSpiral)
  	ON_BN_CLICKED(IDC_Square, OnSquare)
  	ON_BN_CLICKED(IDC_Nudge, OnNudge)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  ////////////////////////////////////////////////////////////
  // CPBehavior message handlers
  
  void CPBehavior::SetSong(CPSong* cpsong)
  {
  	m_cpsong = cpsong;
  }
  
  void CPBehavior::SetMotion(CPMotion* cpmotion)
  {
  	m_cpmotion = cpmotion;
  }
  
  ////////////////////////////////////////////////////////////
  //hotspot activities
  ////////////////////////////////////////////////////////////
  
  
  void CPBehavior::Spiral(short EndVelocity, double TurnAngle)
  {
  //OK
  	m_cpmotion->TwoWheelSpinn(EndVelocity, 5,	TurnAngle, 1, 0, 0);
  	m_cpmotion->TwoWheelSpinn(EndVelocity, 20,	TurnAngle, 1, 0, 0);
  	m_cpmotion->TwoWheelSpinn(EndVelocity, 35,	TurnAngle, 1, 0, 0);
  	m_cpmotion->TwoWheelSpinn(EndVelocity, 55,	TurnAngle, 1, 0, 0);
  	m_cpmotion->TwoWheelSpinn(EndVelocity, 75,	TurnAngle, 1, 0, 0);
  	m_cpmotion->TwoWheelSpinn(EndVelocity, 95,	TurnAngle, 1, 0, 1);
  
  	/*
  	m_cpmotion->TwoWheelSpinn(EndVelocity, 65, TurnAngle, 1, 0, 0);
  	m_cpmotion->TwoWheelSpinn(EndVelocity, 45,  TurnAngle, 1, 0, 0);
  	m_cpmotion->TwoWheelSpinn(EndVelocity, 25,  TurnAngle, 1, 0, 0);
  	m_cpmotion->TwoWheelSpinn(EndVelocity, 5,   TurnAngle, 1, 0, 1);
  	
  	while(!(m_cpmotion->RWait(10)))
  		;
  	if(m_Robot->m_Nudged)						//acknowledge
  	{
  		m_cpsong->AcknowledgeTune();
  		m_cpmotion->SWait(1);
  		m_cpmotion->Spinn(Vn,2,1);
  	}
  	*/
  }
   
  
  void CPBehavior::SpinnSway()
  {
  //spinn, respond to nudge with an other spinn
  //OK
  	double OriginalHeading = m_Robot->GetHeading();
  
  	m_cpmotion->Spinn(Vs,4,1);		        //Vc (on carpet in CA) lets Cye sway while circling
  
  	m_Robot->SetHeadingDestination(OriginalHeading, Vm, 2);
  }
  
  
  void CPBehavior::HotSpinn()
  {
  //OK
  	double OriginalHeading = m_Robot->GetHeading();  
      
      m_cpmotion->Spinn(Vn, 2, true);
      m_Robot->SetHeadingDestination(OriginalHeading, Vm, 2);
  	
  }
   
  void CPBehavior::Square()
  {
  //do this with a predefined path later....
  	double OriginalHeading = m_Robot->GetHeading();
  	double Heading;
  
  	//Left
  	//m_Robot->AddTurnToInstructions(OriginalHeading+pi/2, 0, NULL, 0, 200, 0);	
  	//Right  
  	//m_Robot->AddTurnToInstructions(OriginalHeading-pi/2, -1, NULL, 0, 200, 0);
  
  	/*m_Robot->AddVelocitiesToInstructions(Vc,Vc,2000);
  	m_Robot->AddTurnToInstructions(OriginalHeading-pi/2, -1, NULL, 0, 100, 0);
  	m_Robot->AddVelocitiesToInstructions(Vc,Vc,2000);
  	Heading = m_Robot->GetHeading();
  	m_Robot->AddHeadingToInstructions((Heading-pi/2),NULL,cMinHeadingZone,Vc,0);
  	m_Robot->FollowInstructions(true, true);
  	*/
  
  	m_cpmotion->MoveForward(80,2);
  	m_cpmotion->TurnRight(100);
  	m_cpmotion->MoveForward(80,2);
  	m_cpmotion->TurnRight(100); 
  	
  //#ifdef garbage								//use this to exclude code with comments
  
  	for (int i = 1; i<4; i++)
  	{
  		m_cpmotion->MoveForward(80,4);
  		m_cpmotion->TurnRight(100);	
  	}
  
  	m_cpmotion->MoveForward(80,2);
  	m_cpmotion->TurnRight(100);
  	m_cpmotion->MoveForward(80,2);
  	m_cpmotion->TurnAround(200);
  	m_Robot->SetHeadingDestination(OriginalHeading, Vc, 2);
  	
  	/*
  	while(!(m_cpmotion->RWait(10)))
  		;
  	if(m_Robot->m_Nudged)						//acknowledge
  	{
  		m_cpsong->AcknowledgeTune();
  		m_cpmotion->SWait(1);
  		m_cpmotion->Spinn(Vn,2000,1);
  	}
  	*/
  //#endif
  	
  }
  
  
  void CPBehavior::Cross()
  {
  //do with paths...
  	double OriginalHeading = m_Robot->GetHeading();
  
  	m_cpmotion->MoveForward(80,2);
  	m_cpmotion->TurnAround(200);
  	m_cpmotion->MoveForward(80,4);
  	m_cpmotion->TurnAround(200);
  	m_cpmotion->MoveForward(80,2);
  	m_cpmotion->TurnLeft(100);
  	m_cpmotion->MoveForward(80,2);
  	m_cpmotion->TurnAround(200);
  	m_cpmotion->MoveForward(80,4);
  	m_cpmotion->TurnAround(200);
  	m_cpmotion->MoveForward(80,2);
  	m_cpmotion->TurnRight(100);
  
      m_Robot->SetHeadingDestination(OriginalHeading, (Vn / 2), 2);
  }
  
  void CPBehavior::Eight()
  {
  //OK
  	double OriginalHeading = m_Robot->GetHeading();
  	
  	m_cpmotion->OneWheelSpinn(Vm,1,3,1,0);		//4
  	m_cpmotion->OneWheelSpinn(Vm,0,3,1,1);		//4 
  
  	m_cpmotion->SWait(2);
      m_Robot->SetHeadingDestination(OriginalHeading, (Vn / 2), 2);
  	
  }
  /////////////////////////////////////////////////////////////
  //home base behaviors
  
  void CPBehavior::Impatient1()
  {
  //move into and out of home base a few times
  //OK
  
  	short i, limit;
  	TTaskResult TaskResult;
  	TFloorType FloorType;
  	double OriginalHeading = m_Robot->GetHeading();	
  	//limit = m_Robot->GetRandomNumber(6);
  	limit = 4;
     
      m_cpsong->BoredTune1();
  	
  	m_Robot->AddVelocitiesToInstructions(-Vs,-Vs,3000);
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->FollowInstructions(true, true);
    
      for (i = 1; i<limit; i++)
  	{
  		m_Robot->AddVelocitiesToInstructions(40,40,2000);
  		m_Robot->AddWaitToInstructions(1000);
  		m_Robot->AddVelocitiesToInstructions(-40,-40,2000);
  		m_Robot->AddWaitToInstructions(1000);
  		m_Robot->FollowInstructions(true,true);
  		m_Robot->DoWindowMessages();
  	}
      
      m_Robot->SetHeadingDestination(OriginalHeading, (Vs), 2);
  
      //Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  
  	m_cpmotion->SWait(1);
      m_cpsong->BoredTune1();
  }
  
  
  void CPBehavior::Impatient2()
  {
  //pace a random number of times infront of homebase and return home
  //OK
  	short i, limit;
  	TTaskResult TaskResult;
  	TFloorType FloorType;
  	double Heading;
  	double OriginalHeading = m_Robot->GetHeading();
  	limit = m_cyepersona->GetRandomNumber(6);
  	//if (limit <2)
  	//		limit = 2;									//1 does not look good here
  	limit = 4;
      m_cpsong->BoredTune2();
  
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vs,-Vs,4000);
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->FollowInstructions(true,true);
  	
  	m_cpmotion->TurnLeft(Vn);
  
  	m_Robot->AddHeadingToInstructions(OriginalHeading+pi/2,NULL,cLrgHeadingZone,Vn,0);
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(Vs, Vs, 3000);
  	m_Robot->FollowInstructions(true, true);
  
  	m_cpmotion->TurnAround(Vn);
  	m_cpmotion->SWait(2);
      
      for (i = 1; i< limit; i++)
  	{
          m_cpmotion->MoveForward(Vs, 4);
          m_cpmotion->TurnAround(Vn);
  		m_cpmotion->SWait(2);
          m_Robot->DoWindowMessages();
      }
  
      m_cpmotion->MoveForward(Vs, 2);
   
      if (m_cyepersona->IsOddNumber(limit) == 1)
          m_cpmotion->TurnLeft(Vs);			//changed
      else
          m_cpmotion->TurnRight(Vs);
      
      
      m_Robot->SetHeadingDestination(OriginalHeading, (Vc), 2);              
      
     //Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  
      m_cpmotion->SWait(1);
      m_cpsong->BoredTune2();
  }
  
  void CPBehavior::Impatient3()
  {
  //OK
  	short i, limit;
  	TTaskResult TaskResult;
  	TFloorType FloorType;
  	double OriginalHeading = m_Robot->GetHeading();
  	//limit = m_Robot->GetRandomNumber(6);
  	limit = 6;
     
      m_cpsong->BoredTune3();
  
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vc,-Vc,2000);
  	
  	m_Robot->FollowInstructions(true,true);
  	m_cpmotion->TurnAround(Vm);
  	m_cpmotion->SWait(1);
  
  	m_cpmotion->TwoWheelSpinn(50,50,0,0,1,1);		//left
  
      for (i = 1; i<limit; i++)
  	{
  		m_cpmotion->TwoWheelSpinn(100,100,0,1,1,1);	//right
  		m_cpmotion->TwoWheelSpinn(100,100,0,0,1,1);	//left
          m_Robot->DoWindowMessages();
  	}
  
  	m_cpmotion->TurnAround(Vm);
      m_Robot->SetHeadingDestination(OriginalHeading, (Vs), 2);
  
      //Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
    
  	m_cpmotion->SWait(1);
      m_cpsong->BoredTune3();
  
  }
   
  void CPBehavior::Content1()
  {
  //reverse, diamondzag and return home
  //OK
  	TTaskResult TaskResult;
  	TFloorType FloorType;
  	double TurnAngle1;
      double OriginalHeading = m_Robot->GetHeading();
      double Correction = pi / 20;
      double TurnAngle = pi / 2;
      TurnAngle1 = TurnAngle + Correction;
  	 
      m_cpsong->ContentTune1();
  
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vs,-Vs,3000);
  	m_Robot->FollowInstructions(true, true);
  
      m_cpmotion->SlowTurns(1, TurnAngle1, Vf);	//Vn before
      m_cpmotion->SlowTurns(0, TurnAngle1, Vf);	
      m_cpmotion->SlowTurns(1, TurnAngle1, Vf);
      m_cpmotion->SlowTurns(0, TurnAngle1, Vf);
  
  	m_cpmotion->SWait(1);
      m_Robot->SetHeadingDestination(OriginalHeading, (Vn / 2), 2);
      
  	//Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  
  	m_cpmotion->SWait(1);
      m_cpsong->ContentTune1();    
     
  }
  
  void CPBehavior::Content2()
  {
  //reverse, do zigzag motion and return home
  //OK
  	TTaskResult TaskResult;
  	TFloorType FloorType;
      double OriginalHeading = m_Robot->GetHeading();
      double Correction = pi / 20;
      double TurnAngle = pi / 8;
      double TurnAngle1 = TurnAngle + Correction;
  
  	m_cpsong->ContentTune2();
  
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vc,-Vc,2000);
  	m_Robot->FollowInstructions(true, true);
  	
  	m_cpmotion->SlowTurns(1, TurnAngle1, Vn);		//all Vs before
      m_cpmotion->SlowTurns(0, -TurnAngle1, Vn);
  	m_cpmotion->SlowTurns(1, TurnAngle1, Vn);
      m_cpmotion->SlowTurns(0, -TurnAngle1, -Vn);
  
  	
  	m_cpmotion->SlowTurns(0, TurnAngle1, Vn);
  	m_cpmotion->SlowTurns(1, -TurnAngle1, -Vn);
  	m_cpmotion->SlowTurns(0, TurnAngle1, Vn);
  	m_cpmotion->SlowTurns(1, -TurnAngle1, -Vn);
  	m_Robot->Stop();
  
  	//m_cpmotion->SWait(1);
      m_Robot->SetHeadingDestination(OriginalHeading, (Vn / 2), 2);
      
     //Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  
  	m_cpmotion->SWait(1);
      m_cpsong->ContentTune2();    
  
  }
  
  void CPBehavior::Content3()
  {
  	TTaskResult TaskResult;
  	TFloorType FloorType;
  	double OriginalHeading = m_Robot->GetHeading();
  	m_cpsong->ContentTune3();
  
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vc,-Vc,4000);
  	m_Robot->FollowInstructions(true, true);
  	
  	m_cpmotion->OneWheelSpinn(Vn,1,3,1,1);		// Vf
  	m_cpmotion->OneWheelSpinn(Vn,0,3,1,1);		//
  
  	m_cpmotion->SWait(1);
      m_Robot->SetHeadingDestination(OriginalHeading, (Vn / 2), 2);
  	m_cpmotion->SWait(1);
      
     //Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  
  	m_cpmotion->SWait(1);
      m_cpsong->ContentTune3();    
  }
  
  void CPBehavior::Content4()
  {
  //bone's mix of Content2 and 3
  	TTaskResult TaskResult;
  	TFloorType FloorType;	
  	double OriginalHeading = m_Robot->GetHeading();
      double Correction = pi / 20;
      double TurnAngle = pi / 8;
      double TurnAngle1 = TurnAngle + Correction;
  
  	m_cpsong->SmoothTune();
  
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vc,-Vc,3000);
  	m_Robot->FollowInstructions(true, true);
  
  	//m_cpmotion->SWait(1);
      //m_cpmotion->MoveBackwards(Vc, 3);
  
  	//content2
  	m_cpmotion->SlowTurns(1, TurnAngle1, Vn);		
      m_cpmotion->SlowTurns(0, -TurnAngle1, Vn);
  	m_cpmotion->SlowTurns(1, TurnAngle1, Vn);
      m_cpmotion->SlowTurns(0, -TurnAngle1, -Vn);
  
  	//content3
  	m_cpmotion->OneWheelSpinn(Vns,1,3,1,1);		// Vf
  	m_cpmotion->OneWheelSpinn(Vns,0,3,1,1);		//
      m_Robot->SetHeadingDestination(OriginalHeading, (Vn / 2), 2);
  
  	//content2
  	m_cpmotion->SlowTurns(0, TurnAngle1, Vn);
  	m_cpmotion->SlowTurns(1, -TurnAngle1, -Vn);
  	m_cpmotion->SlowTurns(0, TurnAngle1, Vn);
  	m_cpmotion->SlowTurns(1, -TurnAngle1, -Vn);
  	m_Robot->Stop();
  
  
      m_Robot->SetHeadingDestination(OriginalHeading, (Vn / 2), 2);
      
      //Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  
  	m_cpmotion->SWait(1);
      m_cpsong->SmoothTune();    
  
  }
  void CPBehavior::Agressive1()
  {
  //reverse and turnaround to face user
  	TTaskResult TaskResult;
  	TFloorType FloorType;
  	double OriginalHeading = m_Robot->GetHeading();
  
      m_cpsong->AgressiveTune1();
  
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vf, -Vf, 1000);
  	m_Robot->FollowInstructions(true, true);
  
      m_cpmotion->TurnAround(Vf);
    
  	m_cpmotion->SWait(1);
      m_cpsong->AgressiveTune1();
  
  	AfxMessageBox("Press OK to send angry Cye back home.");
  	m_cpmotion->TurnAround(Vf);
  
  	//Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  }
  
  void CPBehavior::Agressive2()
  {
  //reverse, face user and defend territory
  //OK
  	TTaskResult TaskResult;
  	TFloorType FloorType;
      double OriginalHeading = m_Robot->GetHeading();
  
      m_cpsong->AgressiveTune2();
  	
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vf, -Vf, 1000);
  	m_Robot->FollowInstructions(true, true);
      
      m_cpmotion->TurnAround(Vf);
  	m_cpmotion->SWait(1);					//waits are ok
      m_cpmotion->TurnLeft(Vf);
  	m_cpmotion->SWait(1);;
      m_cpmotion->TurnAround(Vf);
  	m_cpmotion->SWait(1);
      m_cpmotion->TurnLeft(Vf);
  	m_cpmotion->SWait(1);
      m_Robot->SetHeadingDestination(OriginalHeading + pi, Vm, 2);
  
  	m_cpmotion->SWait(1);
      m_cpsong->AgressiveTune2();
  
  	AfxMessageBox("Press OK to send aggressive Cye back home.");
  	m_cpmotion->TurnAround(Vf);
  
  	//Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  
  }
  
  void CPBehavior::Jittery1()
  {
  //reverse,jitter and end facing user  
  	TTaskResult TaskResult;
  	TFloorType FloorType;
      m_cpmotion->ResetMotorConstants();             
      double OriginalHeading;
  
  	OriginalHeading = m_Robot->GetHeading();
  
  	m_cpsong->JitteryTune1();
  
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vm,-Vm,2000);
  	m_Robot->FollowInstructions(true, true);
  
  	//m_cpmotion->SWait(1);
      //m_cpmotion->MoveBackwards(Vm, 2);
  
      m_cpmotion->AbsurdMotion(Vm, 5);       //use speed as Ki parameter !!
  	
      //Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  
  	m_cpmotion->SWait(1);
  	m_cpsong->JitteryTune1();
  }
  
  void CPBehavior::Jittery2()
  {
  //reverse,turnaround, jitter and end facing user  
  	TTaskResult TaskResult;
  	TFloorType FloorType;
      m_cpmotion->ResetMotorConstants();             
      double OriginalHeading;
  
  	OriginalHeading = m_Robot->GetHeading();
  	m_cpsong->JitteryTune2();
  
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vc,-Vc,4000);
  	m_Robot->FollowInstructions(true, true);
  
  	//m_cpmotion->SWait(1);
      //m_cpmotion->MoveBackwards(Vc, 4);
  
      m_cpmotion->TurnAround(Vf);
      m_cpmotion->AbsurdMotion(Vm, 5);               
      
  	m_cpmotion->TurnAround(Vm);
  	m_Robot->SetHeadingDestination(OriginalHeading, (Vm), 2);
      
  	//Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  
  	m_cpmotion->SWait(1);
  	m_cpsong->JitteryTune2();
  	
  }
  
  void CPBehavior::Quirky1()
  {
  //same as Quirky, but spinn multiple times
  	TTaskResult TaskResult;
  	TFloorType FloorType;
      double OriginalHeading = m_Robot->GetHeading();  
      
  	m_cpsong->QuirkyTune2();
  
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vm,-Vm,2000);
  	m_Robot->FollowInstructions(true, true);
  
  	//m_cpmotion->SWait(1);
  	//m_cpmotion->MoveBackwards(Vm, 2);
  
      m_cpmotion->Spinn(Vf, 4, true);		//3
  	m_cpmotion->SWait(1);
  	m_cpsong->DizzyTune();
  	m_cpmotion->SWait(2);				//as if dizzy...
  	m_Robot->SetHeadingDestination(OriginalHeading, Vn, 2);
      m_cpmotion->SWait(1);
      
     //Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  	
  	m_cpmotion->SWait(1);
  	m_cpsong->QuirkyTune2();
  
  }
  
  void CPBehavior::Quirky()
  {
  //revers, spinn and return home
  	TTaskResult TaskResult;
  	TFloorType FloorType;
      double OriginalHeading = m_Robot->GetHeading();  
   
  	m_cpsong->QuirkyTune1();
  
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vm,-Vm,2000);
  	m_Robot->FollowInstructions(true, true);
  
  	//m_cpmotion->SWait(1);
      //m_cpmotion->MoveBackwards(Vm, 2);
  
      m_cpmotion->Spinn(Vn, 2, true);
  	m_cpmotion->SWait(1);
  	m_Robot->SetHeadingDestination(OriginalHeading, (Vm), 2);
       
      //Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  
  	m_cpmotion->SWait(1);;
  	m_cpsong->QuirkyTune1();
  }
  
  void CPBehavior::Mad1()
  {
   //reverse, do some mad moves and return home
  	TTaskResult TaskResult;
  	TFloorType FloorType;
  	double OriginalHeading = m_Robot->GetHeading();
      m_cpmotion->ResetMotorConstants();                
     
  	m_Robot->AddVelocitiesToInstructions(-Vf,-Vf,1000);
  	m_Robot->FollowInstructions(true, true);
  
      m_cpmotion->AbsurdMotion(6, Vm);                //use speed as Kd parameter !
  	m_Robot->SetHeadingDestination(OriginalHeading, Vm, 2);
  
      //Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}  
  }
  
  void CPBehavior::Wiggle()
  {
  	TTaskResult TaskResult;
  	TFloorType FloorType;
  	short Iterations = 7;
  	short Delay = 1;
  	double OriginalHeading = m_Robot->GetHeading();
  	double SecondHeading = OriginalHeading + pi;
  	double Angle1 =  pi/32;
  	double Angle2 = -pi/16;
  	double Angle3 = pi/16;
  
  	m_cpsong->SadTune();
  
  	m_Robot->AddWaitToInstructions(1000);
  	m_Robot->AddVelocitiesToInstructions(-Vc,-Vc,3000);
  	m_Robot->FollowInstructions(true, true);
  
  	//m_cpmotion->SWait(1);
  	//m_cpmotion->MoveBackwards(Vc, 3);
  	
      m_cpmotion->TurnAround(Vm);
      m_cpmotion->SWait(1);           
  
  	for (int i = 1; i< Iterations; i++)
  	{	
  		m_cpmotion->TurnToAngle(Vn, SecondHeading,Angle2);
  		m_cpmotion->TurnToAngle(Vn, SecondHeading,Angle3);
  	}
  	 
  	m_cpmotion->TurnAround(Vm);
  	m_Robot->SetHeadingDestination(OriginalHeading, Vm,2);
    
      //Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, OriginalHeading);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(OriginalHeading, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  
  	m_cpmotion->SWait(1);
  	m_cpsong->SadTune();
  
  }
  
  void CPBehavior::Nudge()
  {
  	AfxMessageBox("Press OK and I will wait 10 seconds for you nudge my bumper");
  	m_Robot->m_Nudged = NDG_None;
  	while(!(m_cpmotion->RWait(10)))
  		;
  	if(m_Robot->m_Nudged)						//acknowledge
  	{
  		m_cpsong->AcknowledgeTune();
  		m_cpmotion->SWait(2);
  		m_cpmotion->RandMotion();
  		m_cpsong->AcknowledgeTune();
  	}
  	m_Robot->m_Nudged = NDG_None;
  
  }
  
  void CPBehavior::Test()
  {
  //as agressive, but using "addinstructions.."
  	double OriginalHeading = m_Robot->GetHeading();
  	/*works
  	m_Robot->AddVelocitiesToInstructions(-Vf,-Vf,1000);
  	m_Robot->FollowInstructions(true, true);
  	*/
  
  	/*works
  	//Left
  	m_Robot->AddTurnToInstructions(OriginalHeading+pi/2, 0, NULL, 0, 200, 0);
  	
  	//Right  
  	m_Robot->AddTurnToInstructions(OriginalHeading-pi/2, -1, NULL, 0, 200, 0);
  
  	m_Robot->FollowInstructions(true, true);
  	*/   
     
  }
  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  
  void CPBehavior::OnAgressive1() 
  {
  	Agressive1();
  }
  
  void CPBehavior::OnAgressive2() 
  {
  	Agressive2();
  }
  
  void CPBehavior::OnBackHome() 
  {
  //only good if you calibrate prior to using this code
  
  	TTaskResult TaskResult;
  	TFloorType FloorType;
  	m_cpmotion->TurnAround(Vn);
  	m_cpmotion->SWait(1);
  	m_Robot->SetHeadingDestination((pi/2), Vm);		//home at pi/2
  	m_cpmotion->SWait(1);
  
  	//Move forward till you hit the main wall and square to wall.
      m_cpmotion->MoveToWall(Vm, pi/2);
  	FloorType = m_Robot->GetFloorType();
  	m_Robot->AddSquareToWallToInstructions(pi/2, FloorType == ftWood);
  	if ( (TaskResult = m_Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) 
  	{
  		return;
  	}
  }
  
  void CPBehavior::OnQuirky1() 
  {
  	Quirky();
  }
  
  void CPBehavior::OnQuirky2() 
  {
  	Quirky1();
  }
  
  void CPBehavior::OnWiggle() 
  {
  	Wiggle();
  }
  
  void CPBehavior::OnJittery1() 
  {
  	Jittery1();
  	
  }
  
  void CPBehavior::OnJittery2() 
  {
  	Jittery2();
  	
  }
  
  void CPBehavior::OnMad1() 
  {
  	Mad1();	
  }
  
  void CPBehavior::OnImpatient1() 
  {
  	Impatient1();	
  }
  
  void CPBehavior::OnImpatient2() 
  {
  	Impatient2();	
  }
  
  void CPBehavior::OnImpatient3() 
  {
  	Impatient3();	
  }
  
  void CPBehavior::OnContent1() 
  {
  	Content1();	
  }
  
  void CPBehavior::OnContent2() 
  {
  	Content2();
  }
  
  void CPBehavior::OnContent3() 
  {
  	Content3();
  }
  
  void CPBehavior::OnContent4() 
  {
  	Content4();
  }
  
  void CPBehavior::OnSpinnSway() 
  {
  	SpinnSway();
  }
  
  void CPBehavior::OnCross() 
  {
  	Cross();
  }
  
  void CPBehavior::OnEight() 
  {
  	Eight();
  }
  
  void CPBehavior::OnSpinn() 
  {
  	HotSpinn();
  }
  
  void CPBehavior::OnSpiral() 
  {
  	short EndVelocity = 200;		
  	double TurnAngle = pi/3;		//5 for return spiral
  	Spiral(EndVelocity, TurnAngle);
  }
  
  void CPBehavior::OnSquare() 
  {
  	Square();
  }
  
  void CPBehavior::OnTest() 
  {
  	//m_cpsong->TestTune();
  	Test();
  }
  
  
  void CPBehavior::OnNudge() 
  {
  	
  	Nudge();
  	
  }
