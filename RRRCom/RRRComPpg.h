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
// RRRComPpg.h : Declaration of the CRRRComPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CRRRComPropPage : See RRRComPpg.cpp.cpp for implementation.

class CRRRComPropPage : public COlePropertyPage
{
        DECLARE_DYNCREATE(CRRRComPropPage)
        DECLARE_OLECREATE_EX(CRRRComPropPage)

        // Constructor
    public:
        CRRRComPropPage();

        // Dialog Data
        //{{AFX_DATA(CRRRComPropPage)
        enum { IDD = IDD_PROPPAGE_RRRCOM };
        // NOTE - ClassWizard will add data members here.
        //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_DATA

        // Implementation
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

        // Message maps
    protected:
        //{{AFX_MSG(CRRRComPropPage)
        // NOTE - ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

};
