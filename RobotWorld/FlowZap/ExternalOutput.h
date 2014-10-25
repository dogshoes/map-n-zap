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
#if !defined(AFX_EXTERNALOUTPUT_H__19205A81_7F8E_11D2_B231_0040F6B87672__INCLUDED_)
#define AFX_EXTERNALOUTPUT_H__19205A81_7F8E_11D2_B231_0040F6B87672__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Instruction.h"

#ifdef ROBOT_WORLD_PROJECT
#include "Robot.h"
#endif

class CExecutableRWGraphicObjectList;

// ExternalOutput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExternalOutput dialog

class CExternalOutput : public CInstruction
{
        // Construction
    public:
#ifdef ROBOT_WORLD_PROJECT
        TTaskResult Execute(CRobot * Robot, CExecutableRWGraphicObjectList* ExecutableObjectList, bool & result);
#endif
        void SetOutputOn(bool On);
        void Enable(BOOL enabled);
        CString ToString(void);
        CString ToFile(void);
        void ShowBorder(BOOL b);
        void MakeExpanded();
        void MakeCondensed();
        CExternalOutput(CWnd* pParent = NULL, int index = 1);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CExternalOutput)
        enum { IDD = IDD_EXTERNAL_OUTPUT };
        CStatic	m_BlackBorder;
        CStatic	m_EtchedBorder;
        CStatic	m_Text;
        CStatic	m_DisabledImage;
        CStatic	m_Image;
        CComboBox	m_comExternalOutput;
        CString	m_strExternalOutput;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CExternalOutput)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CExternalOutput)
        virtual void OnOK();
        virtual void OnCancel();
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnPaint();
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTERNALOUTPUT_H__19205A81_7F8E_11D2_B231_0040F6B87672__INCLUDED_)
