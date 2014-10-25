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
// SplashWnd.cpp : implementation file
//

#include "stdafx.h"
#include "robotworld.h"
#include "SplashWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CSplashWnd dialog


CSplashWnd::CSplashWnd(CWnd* pParent /*=NULL*/)
    : CDialog(CSplashWnd::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSplashWnd)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CSplashWnd::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSplashWnd)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplashWnd, CDialog)
    //{{AFX_MSG_MAP(CSplashWnd)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashWnd message handlers

void CSplashWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // Do not call CDialog::OnPaint() for painting messages
}

BOOL CSplashWnd::OnInitDialog()
{
    CDialog::OnInitDialog();

    CBitmap Bitmap;
    BITMAP BitmapInfo;
    Bitmap.LoadBitmap(IDB_SPLASH);
    Bitmap.GetBitmap(&BitmapInfo);
    CSize BitmapSize;
    BitmapSize.cx = BitmapInfo.bmWidth;
    BitmapSize.cy = BitmapInfo.bmHeight;
    CRect WindowRect;
    GetWindowRect(WindowRect);

    if ((WindowRect.Width() != BitmapSize.cx) || (WindowRect.Height() != BitmapSize.cy))
    {
        SetWindowPos(NULL, 0, 0, BitmapSize.cx, BitmapSize.cy, SWP_NOMOVE);
    }

    /*	if (m_dcDisplayMemory.GetSafeHdc() == NULL) {
    		if (!m_bmpBackground.LoadBitmap(IDB_SPLASH)) {
    			AfxMessageBox("SplashWnd  Load Stop Bitmap failed");
    		}
    		if (!m_dcDisplayMemory.CreateCompatibleDC(GetDC())) {
    			AfxMessageBox("SplashWnd CreateCompatibleDC failed");
    		}
    	}
    */
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSplashWnd::DestroyWindow()
{
    m_dcDisplayMemory.DeleteDC();
    return CDialog::DestroyWindow();
}
