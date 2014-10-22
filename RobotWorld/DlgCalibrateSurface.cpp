
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
  
  // DlgCalibrateSurface.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "RobotWorld.h"
  #include "DlgCalibrateSurface.h"
  #include "Robot.h"

  /////////////////////////////////////////////////////////////////////////////
  // CDlgCalibrateSurface dialog
  
  
  CDlgCalibrateSurface::CDlgCalibrateSurface(CWnd* pParent /*=NULL*/)
  	: CDialog(CDlgCalibrateSurface::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CDlgCalibrateSurface)
  		// NOTE: the ClassWizard will add member initialization here
  	//}}AFX_DATA_INIT
  }
  
  
  void CDlgCalibrateSurface::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgCalibrateSurface)
  		// NOTE: the ClassWizard will add DDX and DDV calls here
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgCalibrateSurface, CDialog)
  	//{{AFX_MSG_MAP(CDlgCalibrateSurface)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgCalibrateSurface message handlers
  
  void CDlgCalibrateSurface::OnOK() 
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
  	CDialog::OnOK();
  	int result;
  	m_Robot->CalibrateFloorSurface(&result);
  }
