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
#if !defined(AFX_DLGCREATEVACUUMPATCH_H__BCCD3562_364A_11D1_B231_0040F6B87672__INCLUDED_)
#define AFX_DLGCREATEVACUUMPATCH_H__BCCD3562_364A_11D1_B231_0040F6B87672__INCLUDED_

#include "VacuumPatch.h"	// Added by ClassView
#include "RothMath.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgCreateVacuumPatch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateVacuumPatch dialog

class CDlgCreateVacuumPatch : public CDialog
{
        // Construction
    public:
        void Serialize(CArchive& archive);
        void SerializeV2(CArchive& archive);
        unsigned short GetPathWidth(void);
        void SetPathWidth(unsigned short PathWidth);
        unsigned short GetTurnDiameter(void);
        void SetTurnDiameter(unsigned short TurnDiameter);
        TEnterSide m_EnterSide;
        CDlgCreateVacuumPatch(CWnd* pParent = NULL);   // standard constructor
        ~CDlgCreateVacuumPatch()
        {
            delete m_VacuumPath;
        };

        // Dialog Data
        //{{AFX_DATA(CDlgCreateVacuumPatch)
        enum { IDD = IDD_CREATEVACUUMPATCH };
        CStatic	m_staVacuumPatchBounds;
        float	m_edtPathWidth;
        float	m_edtTurnDiameter;
        CString	m_edtName;
        float	m_edtVacuumingSpeed;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDlgCreateVacuumPatch)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CDlgCreateVacuumPatch)
        afx_msg void OnRadBotLft();
        afx_msg void OnRadBotRgt();
        afx_msg void OnRadLftBot();
        afx_msg void OnRadLftTop();
        afx_msg void OnRadRgtTop();
        afx_msg void OnRadRgtBot();
        afx_msg void OnRadTopLft();
        afx_msg void OnRadTopRgt();
        virtual BOOL OnInitDialog();
        virtual void OnOK();
        afx_msg void OnPaint();
        afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    private:
        unsigned short m_SizeVacuumPath;
        void PreCreateVacuumPatch(robRECT PatchBounds, TEnterSide EnterSide, unsigned short PathWidth, unsigned short TurnDiameter);
        void SetEntryPointAndHeadings();
        void UpdateVacuumPatch(void);
        POINT RobotToDDSurface(robPOINT RobotPoint, CRect SurfaceOffset);
        void CalculateVacuumPaths();
        double m_HeadForward;
        double m_HeadNext;
        unsigned short m_PathWidth;
        unsigned short m_TurnDiameter;
        int m_NumberPaths;
        robPOINT m_EntryPoint;
        typedef struct
        {
            POINT p1, p2, p3;
            short TurnDiameter;
            robPOINT Arc1s;		/*Starting point of Arc 1*/
            robPOINT Arc1e;		/*Ending point of Arc 1*/
            robPOINT Arc1m;		/*third point on Arc 1*/
            robPOINT Arc2s;		/*Starting point of Arc 2*/
            robPOINT Arc2e;		/*Ending point of Arc 2*/
            robPOINT Arc2m;		/*third point on Arc 2*/
            robPOINT Arc2c;		/*Center of Arc 2*/
            double Arc2r;		/*Radius of Arc 2*/
        } TVacPath;
        TVacPath* m_VacuumPath;
        robRECT m_Bounds;
        void DrawVacuumPatch(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCREATEVACUUMPATCH_H__BCCD3562_364A_11D1_B231_0040F6B87672__INCLUDED_)
