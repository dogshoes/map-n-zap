
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

// DlgCyeSample.cpp : implementation file
//

#include "stdafx.h"
#include "robotworld.h"
#include "DlgCyeSample.h"
#include <atlbase.h>

/////////////////////////////////////////////////////////////////////////////
// CDlgCyeSample dialog


CDlgCyeSample::CDlgCyeSample(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgCyeSample::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgCyeSample)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgCyeSample::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgCyeSample)
    DDX_Control(pDX, IDC_RTX_CYE_SAMPLE_TEXT, m_rtxSampleFile);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCyeSample, CDialog)
    //{{AFX_MSG_MAP(CDlgCyeSample)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCyeSample message handlers

bool CDlgCyeSample::LoadSampleFile(CString Filename)
{
    CComVariant temp(0);
    m_rtxSampleFile.LoadFile(Filename, temp);
    return true;
}
