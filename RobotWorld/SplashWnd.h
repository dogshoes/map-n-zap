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
#if !defined(AFX_SPLASHWND_H__FB715C27_86A2_11D2_B231_0040F6B87672__INCLUDED_)
#define AFX_SPLASHWND_H__FB715C27_86A2_11D2_B231_0040F6B87672__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplashWnd dialog

class CSplashWnd : public CDialog
{
        // Construction
    public:
        CSplashWnd(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CSplashWnd)
        enum { IDD = IDD_SPLASH };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CSplashWnd)
    public:
        virtual BOOL DestroyWindow();
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        CBitmap m_bmpBackground;
        CDC m_dcDisplayMemory;
        // Generated message map functions
        //{{AFX_MSG(CSplashWnd)
        afx_msg void OnPaint();
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASHWND_H__FB715C27_86A2_11D2_B231_0040F6B87672__INCLUDED_)
