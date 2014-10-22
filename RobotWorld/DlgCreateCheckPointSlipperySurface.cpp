
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
 
  // DlgCreateCheckPointSlipperySurface.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "robotworld.h"
  #include "DlgCreateCheckPointSlipperySurface.h"

  /////////////////////////////////////////////////////////////////////////////
  // CDlgCreateCheckPointSlipperySurface dialog
  
  
  CDlgCreateCheckPointSlipperySurface::CDlgCreateCheckPointSlipperySurface(CWnd* pParent /*=NULL*/)
  	: CDialog(CDlgCreateCheckPointSlipperySurface::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CDlgCreateCheckPointSlipperySurface)
  		// NOTE: the ClassWizard will add member initialization here
  	//}}AFX_DATA_INIT
  }
  
  
  void CDlgCreateCheckPointSlipperySurface::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgCreateCheckPointSlipperySurface)
  		// NOTE: the ClassWizard will add DDX and DDV calls here
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgCreateCheckPointSlipperySurface, CDialog)
  	//{{AFX_MSG_MAP(CDlgCreateCheckPointSlipperySurface)
  		// NOTE: the ClassWizard will add message map macros here
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgCreateCheckPointSlipperySurface message handlers
