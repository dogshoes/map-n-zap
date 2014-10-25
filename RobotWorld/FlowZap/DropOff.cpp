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

// DropOff.cpp: implementation of the CDropOff class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FlowZapResource.h"
#include "DropOff.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDropOff::CDropOff(CDialog *parent)
{
    m_Loc.top  = 60;
    m_Loc.left = 75;
    m_Loc.bottom = 400;
    m_Loc.right = 276;

    m_Parent = parent;

}

CDropOff::~CDropOff()
{

}

void CDropOff::OnClick(POINT position)
{

}
void CDropOff::OnLButtonDown(POINT position)
{

}
void CDropOff::OnLButtonUp(POINT position)
{
    if (m_Loc.PtInRect(position))
    {
        ::SetCursor(::LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_STOP)));
    }
}
void CDropOff::OnMouseMove(UINT nFlags, POINT point)
{
}

