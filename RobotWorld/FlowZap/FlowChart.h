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
// FlowChart.h : main header file for the FLOWCHART application
//

#if !defined(AFX_FLOWCHART_H__C4DFEA59_88FC_11D1_AC8C_0040F6B8AE22__INCLUDED_)
#define AFX_FLOWCHART_H__C4DFEA59_88FC_11D1_AC8C_0040F6B8AE22__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "FlowChartResource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFlowChartApp:
// See FlowChart.cpp for the implementation of this class
//

class CFlowChartApp : public CWinApp
{
    public:
        CFlowChartApp();

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CFlowChartApp)
    public:
        virtual BOOL InitInstance();
        //}}AFX_VIRTUAL

        // Implementation

        //{{AFX_MSG(CFlowChartApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOWCHART_H__C4DFEA59_88FC_11D1_AC8C_0040F6B8AE22__INCLUDED_)
