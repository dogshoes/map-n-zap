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
  // LeftLoopPanel.cpp: implementation of the CLeftLoopPanel class.
  //
  //////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "stdlib.h"
  #include "FlowZap.h" 
  #include "LeftLoopPanel.h"
  #include "RepeatLoop.h"
  #include "wingdi.h"
  #include "mmsystem.h"
  #include "Util.h"
  #include "Instruction.h"
  #include "Repeat.h"
  #include "RobotWorld.h"
  
  #ifdef _DEBUG
  #undef THIS_FILE
  static char THIS_FILE[]=__FILE__;
  #define new DEBUG_NEW
  #endif
  
  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////
  
  CLeftLoopPanel::CLeftLoopPanel(CDialog *parent)
  {
  	m_Loc.top  = 70;
  	m_Loc.left = 25;
  	m_Loc.bottom = 1000;
  	m_Loc.right = 75;
  	m_Parent = parent;
  
  	m_ptr = NULL;
  	m_oldPtr = NULL;
  	m_LoopPtr = NULL;
  }
  
  CLeftLoopPanel::~CLeftLoopPanel()
  {
  
  }
  
  CLoop* CLeftLoopPanel::AddLoop(CInstruction* start, CInstruction* end)
  {
  	CRepeat *rp = (CRepeat*) start;
  	end->MakeMoveable(FALSE);
  	int i = m_Loops.GetCount();
  	COLORREF color = RGB(0, GetMin(215-i*30, 255),  GetMin(40 + i*30, 255));
  	CRepeatLoop *l = new CRepeatLoop (rp, end, color, i);
  	m_Loops.AddHead(l);
  	return l;
  }
  
  
  
  void CLeftLoopPanel::OnClick(POINT p)
  {
  
  }
  
  void CLeftLoopPanel::OnLButtonDown(POINT p)
  {	
  	CLoop *l;
  	for (int i = 0; i < m_Loops.GetCount(); i++) {
  		l = (CLoop *)m_Loops.GetAt (m_Loops.FindIndex(i) );
  		if (l->contains(p) ) {
  			l->m_Selected = TRUE;
  			((CFlowZap *)m_Parent)->m_MouseMode = mmFZMoveRepeatLoop;
  			m_LoopPtr = l;
  			l->m_End->m_LoopStart = NULL;
  			m_ptr = l->m_End;
  			m_oldPtr = m_ptr;
  			return; // return when one has been found so more than one arrow can not be selected
  		}	
  	}
  }
  
  
  
  void CLeftLoopPanel::OnLButtonUp(POINT p)
  {
  	if (m_LoopPtr != NULL) {
  		CFlowZap* fc = (CFlowZap*)m_Parent;
  		CRepeatLoop *l= (CRepeatLoop*) m_LoopPtr;
  		l->m_Selected = FALSE;
  		if (!l->m_LegalPoint)  l->m_End = l->m_Start;
  		fc->RedrawWindow(m_Loc, NULL, RDW_INVALIDATE | RDW_ERASE);
  		l->m_LegalPoint = TRUE;
  		//l->m_End->MakeMoveable(FALSE);
  		fc->UpdateMovableStatusOfInstructions();
  		l->m_End->m_LoopStart = l->m_Start;
  	}
  			
  	m_ptr = NULL;
  	m_oldPtr = NULL;
  	m_LoopPtr = NULL;
  	
  }
  
  
  void CLeftLoopPanel::OnMouseMove(UINT nFlags, POINT p)
  {
  	
  	if (m_LoopPtr == NULL) return; // If there is no loop selected
  
  	CFlowZap* fc = (CFlowZap*)m_Parent;
  	CRepeatLoop* l = (CRepeatLoop*) m_LoopPtr;
  
  	CInstruction *in = (fc->GetInstructionAtPixel(p) );
  	if (in == NULL) in = fc->GetInstructionAtIndex (fc->GetInstructionsSize() - 1);
  
  	l->m_End = in;
  	l->m_LegalPoint = IsLegalPoint();
  	if (in->IsKindOf( RUNTIME_CLASS( CRepeat ) ) && in != l->m_Start ) l->m_LegalPoint = FALSE;
  	if (in->GetIndex() > l->m_Start->GetIndex() ) in = l->m_Start;
  	l->m_End = in;
  	if (m_ptr != m_oldPtr) {
  		if (l->m_LegalPoint) {
  
  			// the redraw is only needed if the repeat panel is condensed
  			if (!l->m_Start->m_Expanded) l->m_Start->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
  
  			CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  			SoundPath += "/sounds/choice.wav";
  			if (fc->IsSoundOn() ) PlaySound (SoundPath, NULL, SND_FILENAME | SND_ASYNC );
  
  			//Convert the number of steps back from an in to a string
  			int ns = l->m_Start->GetIndex() - l->m_End->GetIndex();
  			CString numsteps = IntToString (ns);
  		
  			((CRepeat*)(l->m_Start))->SetNumSteps(numsteps);
  		}
  		else {
  			((CRepeat*)(l->m_Start))->SetNumSteps("X");
  		}
  		fc->RedrawWindow(m_Loc, NULL, RDW_INVALIDATE | RDW_ERASE);
  	}
  	m_oldPtr = m_ptr;
  			
  	m_ptr  = in;
  	
  	
  }
  
  BOOL CLeftLoopPanel::IsLegalPoint(){
  /************************************************************************************************
  * CREATED: 1 Jan 98 1:19PM  EST 
  *
  * LAST MODIFIED: 23 Mar 98 1:19PM  EST 
  *
  * PARAMETERS
  *	none
  * RETURN
  *	BOOL	the value of the point (legal or illegal)
  ************************************************************************************************
  * DESCRIPTION
  *	determines the current selection for the repeat loop is a legal point
  * METHOD:
  *
  ************************************************************************************************/
  
  
  	// if this is the only loop in the list, then any point is a legal point.
  	if (m_Loops.GetCount() == 1) return TRUE;
  	
  	BOOL legalPoint = FALSE;
  	CRepeatLoop * rl;
  	for (int i = 0; i < m_Loops.GetCount(); i++) {
  		rl = (CRepeatLoop *)m_Loops.GetAt (m_Loops.FindIndex(i) );
  		if (m_LoopPtr == rl) continue;
  		BOOL startBetween = m_LoopPtr->m_Start->Between(rl->m_End, rl->m_Start);
  		BOOL endBetween   = m_LoopPtr->m_End->Between(rl->m_End, rl->m_Start);
  		BOOL pointToSame = (m_LoopPtr->m_End == rl->m_End );
  		// TEST CASES
  		if (!startBetween && !endBetween) legalPoint =  TRUE; 
  		else if ( startBetween &&  endBetween) legalPoint = TRUE; 
  		else if (pointToSame)				  legalPoint = TRUE;
  		else return FALSE;
  	}
  	
  	return legalPoint;
  }
  
  
  
  void CLeftLoopPanel::UpdateLoopLevels()
  {	
  	CInstruction *in;
  	CInstruction *changing;
  	int startIndex, endIndex;
  	CFlowZap* fc = (CFlowZap*)m_Parent;
  	for (int i =0; i < fc->GetInstructionsSize() - 1; i++) {
  		in = fc->GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CRepeat ) ) ) in->m_Loop->m_Level = 0;
  	}
  		
  	for (i =0; i < fc->GetInstructionsSize() - 1; i++) {
  		in = fc->GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CRepeat ) ) ) {
  			//in.moveable = false;
  			//in.pointingTo.moveable = false;
  			startIndex = in->GetIndex();
  			endIndex = in->m_Loop->m_End->GetIndex();
  			for (int j = endIndex + 1; j < startIndex; j++) {
  				changing = fc->GetInstructionAtIndex(j);
  				if (changing->IsKindOf( RUNTIME_CLASS( CRepeat ) ) ) changing->m_Loop->m_Level++;
  			}
  		}
  
  	} 
  	
  
  
  }
