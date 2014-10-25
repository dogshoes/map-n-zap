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
// DropOff.h: interface for the CDropOff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROPOFF_H__E554FFA4_6FE2_11D1_AC8C_0040F6B8AE22__INCLUDED_)
#define AFX_DROPOFF_H__E554FFA4_6FE2_11D1_AC8C_0040F6B8AE22__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Zone.h"

class CDropOff : public CZone
{
    public:
        CDropOff(CDialog *parent = NULL);
        virtual ~CDropOff();

        void OnClick(POINT position);
        void OnLButtonDown(POINT position);
        void OnLButtonUp(POINT position);
        void OnMouseMove(UINT nFlags, POINT point);

};

#endif // !defined(AFX_DROPOFF_H__E554FFA4_6FE2_11D1_AC8C_0040F6B8AE22__INCLUDED_)
