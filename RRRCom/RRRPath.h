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
///////////////////////////////////////////////////////////////////////////////
// Module:     RRRPath.cpp
// Abstract:   Robot path OLE object header file.
// Author:     J.C.H
// Copyright 1997 Cycle Time Corp
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RRRPATH_H__723BB6A8_8075_11D1_BCC1_444553540000__INCLUDED_)
#define AFX_RRRPATH_H__723BB6A8_8075_11D1_BCC1_444553540000__INCLUDED_

#pragma once

/////////////////////////////////////////////////////////////////////////////
// RRRPath command target

class RRRPath : public CCmdTarget
{
        DECLARE_DYNCREATE(RRRPath)

        RRRPath();           // protected constructor used by dynamic creation

        // Attributes
    public:

        // Operations
    public:

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(RRRPath)
    public:
        virtual void OnFinalRelease();
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        virtual ~RRRPath();

        CArray<double, double> m_x;
        CArray<double, double> m_y;
        short m_NumPoints;

        BOOL ValidIndex(short Index);

        // Generated message map functions
        //{{AFX_MSG(RRRPath)
        // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
        DECLARE_OLECREATE(RRRPath)

        // Generated OLE dispatch map functions
        //{{AFX_DISPATCH(RRRPath)
        //}}AFX_DISPATCH
        afx_msg short GetNumPoints();
        afx_msg void SetNumPoints(short nNewValue);
        afx_msg double GetX(short Index);
        afx_msg void SetX(short Index, double newValue);
        afx_msg double GetY(short Index);
        afx_msg void SetY(short Index, double newValue);
        DECLARE_DISPATCH_MAP()
        DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RRRPATH_H__723BB6A8_8075_11D1_BCC1_444553540000__INCLUDED_)