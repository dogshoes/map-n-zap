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
// RightLoopPanel.h: interface for the CRightLoopPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIGHTLOOPPANEL_H__F62F04E2_786D_11D1_AC8C_0040F6B8AE22__INCLUDED_)
#define AFX_RIGHTLOOPPANEL_H__F62F04E2_786D_11D1_AC8C_0040F6B8AE22__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "LoopPanel.h"

class CRightLoopPanel : public CLoopPanel
{
    public:
        void UpdateLoopLevels();
        void OnLButtonUp(POINT p);
        void OnLButtonDown(POINT p);
        void OnClick(POINT p);
        void OnMouseMove(UINT nFlags, POINT p);
        CLoop * AddLoop(CInstruction* start, CInstruction* end);
        CRightLoopPanel(CDialog *parent);
        virtual ~CRightLoopPanel();

};

#endif // !defined(AFX_RIGHTLOOPPANEL_H__F62F04E2_786D_11D1_AC8C_0040F6B8AE22__INCLUDED_)
