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
// LoopPanel.cpp: implementation of the CLoopPanel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Instruction.h"
#include "LoopPanel.h"
#include "Loop.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoopPanel::CLoopPanel()
{

}

CLoopPanel::~CLoopPanel()
{
    EmptyList();
}



void CLoopPanel::RemoveLoop(CInstruction * in)
{
    CLoop * toRemove = 	in->m_Loop;
    m_Loops.RemoveAt(m_Loops.Find(toRemove));
    delete toRemove;
    toRemove = NULL;
    in->m_Loop = NULL;

}



void CLoopPanel::Paint(CPaintDC * dc)
{
    int width = m_Loc.Width();

    if (m_Loops.IsEmpty())
    {
        return;
    }

    /* TEST SIZE OF PANEL HERE
    CBrush *oldBrush;
    CBrush brush;
    brush.CreateSolidBrush (COLORREF(0x000000) );
    oldBrush = dc->SelectObject (&brush);
    dc->FillRect (m_Loc, &brush);
    dc->SelectObject (oldBrush);
    //delete brush;
    //brush = NULL;
    */

    CLoop* l;

    for (int i = 0; i < m_Loops.GetCount(); i++)
    {
        l = (CLoop *)m_Loops.GetAt(m_Loops.FindIndex(i));
        UpdateLoopLevels();
        l->CalculateLocation(m_Parent, i, m_Loops.GetCount(), width);
        l->Draw(dc);
    }

}




void CLoopPanel::EmptyList()
{
    CLoop* l;

    for (int i = 0; i < m_Loops.GetCount(); i++)
    {
        l = (CLoop *)m_Loops.GetAt(m_Loops.FindIndex(i));
        delete l;
    }

    m_Loops.RemoveAll();
}

