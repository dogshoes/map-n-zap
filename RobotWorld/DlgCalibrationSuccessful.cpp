
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

// DlgCalibrationSuccessful.cpp : implementation file
//

#include "stdafx.h"
#include "robotworld.h"
#include "DlgCalibrationSuccessful.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibrationSuccessful dialog


CDlgCalibrationSuccessful::CDlgCalibrationSuccessful(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgCalibrationSuccessful::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgCalibrationSuccessful)
    m_edtDH = 0.0f;
    m_edtDX = 0.0f;
    m_edtDY = 0.0f;
    m_chkDontShowAgain = FALSE;
    //}}AFX_DATA_INIT
}


void CDlgCalibrationSuccessful::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgCalibrationSuccessful)
    DDX_Text(pDX, IDC_EDT_DH, m_edtDH);
    DDX_Text(pDX, IDC_EDT_DX, m_edtDX);
    DDX_Text(pDX, IDC_EDT_DY, m_edtDY);
    DDX_Check(pDX, IDC_CHK_DONT_SHOW_AGAIN, m_chkDontShowAgain);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCalibrationSuccessful, CDialog)
    //{{AFX_MSG_MAP(CDlgCalibrationSuccessful)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibrationSuccessful message handlers
