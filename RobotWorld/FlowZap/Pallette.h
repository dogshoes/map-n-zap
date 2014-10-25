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
/////////////////////////////////////////////////////////////////////////////
// CPallette dialog
#ifndef Pallette_aldfaslkdjfoiqwejf
#define Pallette_aldfaslkdjfoiqwejf

#include "PalletteItem.h"

class CPallette : public CDialog
{
        // Construction
    public:
        void OnPalletteItemClicked(UINT nFlags, CPalletteItem* PalletteItem);
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        CWnd* m_Parent;
        CPallette(CWnd* pParent = NULL);   // standard constructor
        CPalletteItem m_butHotSpot;
        CPalletteItem m_butExternalOutput;
        CPalletteItem m_butWait;
        CPalletteItem m_butVacuum;
        CPalletteItem m_butSubroutine;
        CPalletteItem m_butStop;
        CPalletteItem m_butRepeat;
        CPalletteItem m_butPath;
        CPalletteItem m_butMotion;
        CPalletteItem m_butCalibrate;
        CPalletteItem m_butBranch;
        CPalletteItem m_butBeep;
        // Dialog Data
        //{{AFX_DATA(CPallette)
        enum { IDD = IDD_PALLETTE };
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CPallette)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        CToolTipCtrl m_tooltip;

        // Generated message map functions
        //{{AFX_MSG(CPallette)
        virtual void OnOK();
        virtual void OnCancel();
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        virtual BOOL OnInitDialog();
        afx_msg void OnPaint();
        afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
        afx_msg BOOL OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    private:
        CWnd * m_PrevIcon;
        int m_LeftBuffer;
        int m_IconSize;
};


#endif
