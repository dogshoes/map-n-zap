
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

// Cue.cpp: implementation of the CCue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cue.h"
#include "flowzap.h"
#include "Instruction.h"
#include "..\RobotWorld.h"
#include "util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCue::CCue(CDialog *parent)
    : m_Parent(parent)
    , m_dragging(0)
{
    m_Loc.top  = 75;
    m_Loc.left = 5;
    m_Loc.bottom = 1000;
    m_Loc.right = 25;
    ptr = NULL;
    //m_dragging = 0;

}

CCue::~CCue()
{
    oldPtr = NULL;
    ptr = NULL;
    m_Parent = NULL;
}

void CCue::Init(CInstruction* in)
{
    ptr = in;
    oldPtr = ptr;
}



void CCue::PaintPointer(CPaintDC* dc, int l)
{
    COLORREF blue = RGB(0, 103, 223);
    CFlowZap *fc = ((CFlowZap*)m_Parent);

    try
    {
        CPen *oldPen;
        CBrush *oldBrush;
        CPen cp;
        CBrush cb;

        // if the user is dragging : red
        if (m_dragging)
        {
            cp.CreatePen(PS_SOLID, 1, COLORREF(0x0000ff));
            oldPen = dc->SelectObject(&cp);

            cb.CreateSolidBrush(COLORREF(0x0000ff));
            oldBrush = dc->SelectObject(&cb);
        }

        // if the user is running a program: green
        else if (fc->m_ProgramMode == pmExecute)
        {
            cp.CreatePen(PS_SOLID, 1, COLORREF(0x00ff00));
            oldPen = dc->SelectObject(&cp);

            cb.CreateSolidBrush(COLORREF(0x00ff00));
            oldBrush = dc->SelectObject(&cb);

        }

        // otherwise blue
        else
        {
            cp.CreatePen(PS_SOLID, 1, blue);
            oldPen = dc->SelectObject(&cp);

            cb.CreateSolidBrush(blue);
            oldBrush = dc->SelectObject(&cb);
        }


        vertices[0].x = 7;
        vertices[0].y = l - 5;
        vertices[1].x = 7;
        vertices[1].y = l + 5;
        vertices[2].x = 17;
        vertices[2].y = l + 5;
        vertices[3].x = 23;
        vertices[3].y = l;
        vertices[4].x = 17;
        vertices[4].y = l - 5;
        dc->Polygon(vertices, 5);


        CPen cp2(PS_SOLID, 1, COLORREF(0xffffff));
        dc->SelectObject(&cp2);
        dc->MoveTo(7, l - 4);
        dc->LineTo(17, l - 4);
        dc->LineTo(22, l - 1);

        CPen cp3(PS_SOLID, 1, COLORREF(0x000000));
        dc->SelectObject(&cp3);
        dc->MoveTo(7, l + 4);
        dc->LineTo(17, l + 4);
        dc->LineTo(22, l + 1);


        dc->SelectObject(oldPen);
        dc->SelectObject(oldBrush);
        cp.DeleteObject();
        cb.DeleteObject();
        cp2.DeleteObject();
        cp3.DeleteObject();

    }

    catch (CResourceException e)
    {
        int i = 0;
    }

}

void CCue::Paint(CPaintDC * dc)
{

    /*
    Get a handle to the parent app
    */
    CFlowZap *fc = ((CFlowZap *)m_Parent);




    /*
    If there are no instructions, return
    */
    if (fc->GetInstructionsSize() == 0)
    {
        return;
    }



    /*
    Find the Boundaries of the spine
    */
    int top = fc->GetInstructionAtIndex(0)->GetCenter() - 1;
    int bot = fc->GetInstructionAtIndex(fc->GetInstructionsSize() - 1)->GetCenter() - 1;


    /*
    Set up the brushes and pens
    */
    CBrush cb(COLORREF(0x000000));
    CBrush *oldBrush = dc->SelectObject(&cb);
    CRect r(12, top, 14, bot);
    dc->FillRect(r, &cb);


    CPen cp(PS_SOLID, 1, COLORREF(0xffffff));
    CPen *oldPen = dc->SelectObject(&cp);
    dc->MoveTo(14, bot);
    dc->LineTo(14, top);





    CInstruction *in;

    for (int i = 0; i < fc->GetInstructionsSize(); i ++)
    {
        in = fc->GetInstructionAtIndex(i);
        int pos = in->GetCenter() - 1;

        CRect r2(8, pos - 1, 18, pos + 1);
        dc->FillRect(r2, &cb);
        dc->MoveTo(9, pos + 1);
        dc->LineTo(12, pos + 1);
        dc->MoveTo(14, pos + 1);
        dc->LineTo(17, pos + 1);
    }




    /*
    Find the location of where to paint the pointer.
    If the ptr is invalid, then reset it to the head of the list
    paint the pointer
    */
    if (ptr != NULL)
    {
        if (ptr->GetSafeHwnd() != NULL)
        {
            m_location = ptr->GetCenter() - 1;
        }
        else
        {
            m_location = fc->GetInstructionAtIndex(0)->GetCenter() - 1;
        }
    }
    else
    {
        m_location = fc->GetInstructionAtIndex(0)->GetCenter() - 1;
    }

    PaintPointer(dc, m_location);

    dc->SelectObject(oldPen);
    dc->SelectObject(oldBrush);

}

void CCue::OnClick(POINT position)
{

}

void CCue::OnLButtonDown(POINT position)
{
    if (position.y > m_location - 5 && position.y < m_location + 5)
    {
        m_dragging = TRUE;
        ((CFlowZap *)m_Parent)->m_MouseMode = mmFZMoveCue;
    }
}

void CCue::OnLButtonUp(POINT position)
{
    m_dragging = FALSE;
    oldPtr = ptr;
}

void CCue::OnMouseMove(UINT nFlags, POINT point)
{
    CFlowZap *fc = ((CFlowZap *)m_Parent);
    ptr = fc->GetInstructionAtPixel(point);

    if (ptr == NULL)
    {
        ptr = oldPtr;
    }

    if (oldPtr != ptr)
    {
        // DETERMINE THE RECTANGLE TO REDRAW
        CRect r(0, GetMin(ptr->GetCenter(), oldPtr->GetCenter()) - 7 , 25, GetMax(ptr->GetCenter(), oldPtr->GetCenter()) + 7);
        CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
        SoundPath += "/sounds/choice.wav";

        if (fc->IsSoundOn())
        {
            PlaySound(SoundPath, NULL, SND_FILENAME | SND_ASYNC);
        }

        fc->RedrawWindow(r, NULL, RDW_INVALIDATE | RDW_ERASE);
    }

    oldPtr = ptr;
}



void CCue::OnLButtonDbClk(CPoint point)
{
    CFlowZap *fc = ((CFlowZap *)m_Parent);
    ptr = fc->GetInstructionAtPixel(point);
    CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
    SoundPath += "/sounds/choice.wav";

    if (fc->IsSoundOn())
    {
        PlaySound(SoundPath, NULL, SND_FILENAME | SND_ASYNC);
    }

    oldPtr = ptr;
}



void CCue::SetCueToInstruction(CInstruction *in)
{
    CFlowZap *fc = ((CFlowZap *)m_Parent);

    if (in == NULL)
    {
        in = fc->GetInstructionAtIndex(0);
    }

    ptr = in;
    oldPtr = ptr;
    CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
    SoundPath += "/sounds/choice.wav";

    if (fc->IsSoundOn())
    {
        PlaySound(SoundPath, NULL, SND_FILENAME | SND_ASYNC);
    }


}

CString CCue::ToString()
{
    int i = ptr->GetIndex();
    int     decimal,   sign, precision;

    if (i > 10)
    {
        precision = 2;
    }
    else
    {
        precision = 1;
    }

    CString index = _ecvt(i, precision, &decimal, &sign);
    return index + " **LOCATION OF THE POINTER**";
}


void CCue::Reset()
{
    ptr = NULL;
    oldPtr = NULL;
    m_dragging = FALSE;
    m_location = 80;

}



CInstruction* CCue::GetInstructionAtPointer()
{
    if (ptr != NULL)
    {
        return ptr;
    }
    else
    {
        return NULL;
    }
}

void CCue::HandleRemoveInstruction(CInstruction * in)
{
    if (ptr != in)
    {
        return;
    }

    CFlowZap *fc = ((CFlowZap *)m_Parent);
    SetCueToInstruction(fc->GetNextInstructionInList(in));

}
