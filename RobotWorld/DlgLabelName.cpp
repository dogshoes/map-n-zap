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

// DlgLabelName.cpp : implementation file
//

#include "stdafx.h"
#include "RobotWorld.h"
#include "DlgLabelName.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgLabelName dialog


CDlgLabelName::CDlgLabelName(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgLabelName::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgLabelName)
    m_edtLabelName = _T("");
    //}}AFX_DATA_INIT
}


void CDlgLabelName::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgLabelName)
    DDX_Text(pDX, IDC_EDTLABELNAME, m_edtLabelName);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLabelName, CDialog)
    //{{AFX_MSG_MAP(CDlgLabelName)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLabelName message handlers
