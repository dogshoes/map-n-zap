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

  /*******************************************************************
  COPYRIGHT 1999 Probotics, Inc.
      All title and copyrights in and to the Map-N-Zap program (including
  but not limited to any images, animations, audio, music, text, and
  dynamic link libraries) incorporated into the Map-N-Zap program, the
  accompanying printed materials, and any copies of the Map-N-Zap program
  are owned by Probotics, Inc. The Map-N-Zap program is protected by
  copyright laws. Therefore, you must treat the Map-N-Zap program and it's
  includes like any other copyrighted material.
  ********************************************************************/
  // DlgErrorCheckingWoodWithVacuum.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "DlgErrorCheckingWoodWithVacuum.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgErrorCheckingWoodWithVacuum dialog
  
  
  CDlgErrorCheckingWoodWithVacuum::CDlgErrorCheckingWoodWithVacuum(CWnd* pParent /*=NULL*/)
  	: CDialog(CDlgErrorCheckingWoodWithVacuum::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CDlgErrorCheckingWoodWithVacuum)
  		// NOTE: the ClassWizard will add member initialization here
  	//}}AFX_DATA_INIT
  }
  
  
  void CDlgErrorCheckingWoodWithVacuum::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgErrorCheckingWoodWithVacuum)
  		// NOTE: the ClassWizard will add DDX and DDV calls here
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgErrorCheckingWoodWithVacuum, CDialog)
  	//{{AFX_MSG_MAP(CDlgErrorCheckingWoodWithVacuum)
  		// NOTE: the ClassWizard will add message map macros here
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgErrorCheckingWoodWithVacuum message handlers
