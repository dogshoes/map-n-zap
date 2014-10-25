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
// RightLoopPanel.cpp: implementation of the CRightLoopPanel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FlowZap.h"
#include "RightLoopPanel.h"
#include "BranchLoop.h"
#include "mmsystem.h"
#include "Util.h"
#include "Instruction.h"
#include "..\RobotWorld.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRightLoopPanel::CRightLoopPanel(CDialog *parent)
{
    m_Loc.top  = 70;
    m_Loc.left = 375;
    m_Loc.bottom = 1000;
    m_Loc.right = 425;
    m_Parent = parent;

    m_ptr = NULL;
    m_oldPtr = NULL;
    m_LoopPtr = NULL;
}

CRightLoopPanel::~CRightLoopPanel()
{

}

CLoop * CRightLoopPanel::AddLoop(CInstruction* start, CInstruction* end)
{
    CBranch *bp = (CBranch*) start;
    int i = m_Loops.GetCount();
    COLORREF color = RGB(0, GetMin(i * 40, 255),  GetMin(255 - i * 40, 255));
    CBranchLoop *l = new CBranchLoop(bp, end, color, i);
    m_Loops.AddHead(l);
    return l;
}


void CRightLoopPanel::OnLButtonDown(POINT p)
{
    CLoop* l;

    for (int i = 0; i < m_Loops.GetCount(); i++)
    {
        l = (CLoop *)m_Loops.GetAt(m_Loops.FindIndex(i));

        if (l->contains(p))
        {
            ((CFlowZap *)m_Parent)->m_MouseMode = mmFZMoveBranchLoop;
            l->m_Start->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
            l->m_End->m_LoopStart = NULL;
            l->m_Selected = TRUE;
            m_LoopPtr = l;
            m_ptr = l->m_End;
            m_oldPtr = m_ptr;
            return;
        }
    }

    return;
}


void CRightLoopPanel::OnMouseMove(UINT nFlags, POINT p)
{
    if (m_LoopPtr == NULL)
    {
        return;
    }

    CFlowZap* fc = (CFlowZap*)m_Parent;

    if (m_ptr != m_oldPtr)
    {
        CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
        SoundPath += "/sounds/choice.wav";

        if (fc->IsSoundOn())
        {
            PlaySound(SoundPath, NULL, SND_FILENAME | SND_ASYNC);
        }

        fc->RedrawWindow(m_Loc, NULL, RDW_INVALIDATE | RDW_ERASE);
    }

    m_oldPtr = m_ptr;
    CBranchLoop* l = (CBranchLoop*) m_LoopPtr;
    CInstruction *in = (fc->GetInstructionAtPixel(p));

    if (in == NULL)
    {
        in = fc->GetInstructionAtIndex(fc->GetInstructionsSize() - 1);
    }

    m_ptr = in;
    l->m_End = in;

    return;
}






void CRightLoopPanel::OnLButtonUp(POINT p)
{
    CFlowZap* fc = (CFlowZap*)m_Parent;

    if (m_LoopPtr != NULL)
    {
        CFlowZap* fc = (CFlowZap*)m_Parent;
        CBranchLoop *l = (CBranchLoop*) m_LoopPtr;
        l->m_Start->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
        l->m_Selected = FALSE;
        CInstruction *in = (fc->GetInstructionAtPixel(p));

        if (in == NULL)
        {
            in = fc->GetInstructionAtIndex(fc->GetInstructionsSize() - 1);
        }

        l->m_End = in;
        l->m_End->m_LoopStart = l->m_Start;
    }

    m_ptr = NULL;
    m_oldPtr = NULL;
    m_LoopPtr = NULL;
    fc->RedrawWindow(m_Loc, NULL, RDW_INVALIDATE | RDW_ERASE);
}



void CRightLoopPanel::OnClick(POINT p)
{

}





void CRightLoopPanel::UpdateLoopLevels()
{

}
