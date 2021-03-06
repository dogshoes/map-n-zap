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
#if !defined(AFX_PALLETTEITEM_H__51A2BB05_18DB_11D3_A5DF_00105A98F145__INCLUDED_)
#define AFX_PALLETTEITEM_H__51A2BB05_18DB_11D3_A5DF_00105A98F145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PalletteItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPalletteItem window

class CPalletteItem : public CBitmapButton
{
        // Construction
    public:
        CPalletteItem();

        // Attributes
    public:

        // Operations
    public:

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CPalletteItem)
        //}}AFX_VIRTUAL

        // Implementation
    public:
        virtual ~CPalletteItem();

        // Generated message map functions
    protected:
        //{{AFX_MSG(CPalletteItem)
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALLETTEITEM_H__51A2BB05_18DB_11D3_A5DF_00105A98F145__INCLUDED_)
