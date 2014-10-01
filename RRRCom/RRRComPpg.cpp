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
// RRRComPpg.cpp : Implementation of the CRRRComPropPage property page class.

#include "stdafx.h"
#include "RRRCom.h"
#include "RRRComPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRRRComPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CRRRComPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CRRRComPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CRRRComPropPage, "RRRCOM.RRRComPropPage.1",
	0x1bb9c514, 0x657f, 0x11d0, 0x84, 0x65, 0xca, 0x28, 0x1d, 0, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// CRRRComPropPage::CRRRComPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CRRRComPropPage

BOOL CRRRComPropPage::CRRRComPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_RRRCOM_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CRRRComPropPage::CRRRComPropPage - Constructor

CRRRComPropPage::CRRRComPropPage() :
	COlePropertyPage(IDD, IDS_RRRCOM_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CRRRComPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CRRRComPropPage::DoDataExchange - Moves data between page and properties

void CRRRComPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CRRRComPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CRRRComPropPage message handlers
