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
  #include "stdafx.h"
  #include "instruction.h"
  #include "flowzap.h"
  #include "util.h"
  #include "RobotWorld.h"
  
  #include "mmsystem.h" 
  
  IMPLEMENT_DYNAMIC( CInstruction, CDialog )
  
  CInstruction::CInstruction(UINT nIDTemplate, CWnd* pParentWnd, int index)
  		: m_fc (pParentWnd)
  		, m_Index (index)
  {
  	CDialog(nIDTemplate, pParentWnd);
  	
  }
  
  
  
  
  
  void CInstruction::OnPaint(CDC * dc){
  /************************************************************************************************
  * DATE CREATED: 
  *	1 Jan 98 11:07 PM EST
  * LAST MODIFIED:
  *	22 Nov 98 6:42 PM EST
  * PARAMETERS:
  *	dc	the drawing context
  * RETURN
  *	void
  ************************************************************************************************
  * DESCRIPTION
  *	does the neccesary drawing routines on each of the instructions
  * METHOD:
  *	
  ************************************************************************************************/
  	
  
  
  	/* 
  	**  Get the rectangle for this object : r
  	*/
  	CRect r;
  	GetClientRect(&r);
  	
  
  	/*
  	** Draw the text if this instruction is condensed
  	*/
  	if (!m_Expanded) {
  		COLORREF oldColor;
  		if (!m_Enabled) { // grey
  			oldColor = dc->SetTextColor (RGB (100, 100, 100) );
  		}
  
  		else { // black
  			oldColor = dc->SetTextColor (RGB (0, 0, 0) );
  		}
  		CFont f;
  		f.CreatePointFont(90, "Arial", NULL);
  		CFont *oldFont = (CFont*) dc->SelectObject (&f);
  		dc->TextOut (30, 0, ToString() );
  		dc->TextOut (10, 0, IntToString (m_Index + 1) + ".");
  		dc->SetTextColor (oldColor);
  		dc->SelectObject (oldFont);
  	}
  
  
  
  	/* 
  	**  Otherwise, Draw the blue background for this object
  	else {
  		// create the pen and brush
  		COLORREF blue = RGB (0, 103, 223);
  		CPen cp (PS_SOLID, 1, blue);
  		CBrush cb;
  		cb.CreateSolidBrush(blue);
  
  		// select the pen and brush
  		CBrush *oldBrush     = dc->SelectObject (&cb);
  		CPen   *oldPen = dc->SelectObject(&cp);
  
  
  		dc->Rectangle (0,0, 50, r.Height() );
  		
  		dc->SelectObject (oldPen);
  		dc->SelectObject (oldBrush);
  	}
  */	
  
  
  
  	/*
  	** Draw the red border if this instruction is selected
  	*/
  	if (m_Selected) {
  		CPen cp (PS_SOLID, 1, COLORREF(0xFF) );
  		CPen *oldPen = dc->SelectObject (&cp);
  		dc->MoveTo (2, 2);
  		dc->LineTo (r.Width() -3, 2);
  		dc->LineTo (r.Width()-3, r.Height()-3 );
  		dc->LineTo (2, r.Height() -3);
  		dc->LineTo (2, 2);
  
  		dc->SelectObject (oldPen);
  		
  	}
  	
  }	
  
  
  
  
  
  
  
  
  void CInstruction::MouseClick(CPoint point){
  /************************************************************************************************
  * DATE CREATED: 
  *	1 Jan 98 11:07 AM EST
  * LAST MODIFIED:
  *	16 Jun 98 10:30 AM EST
  * PARAMETERS:
  *	point	the point of the user click relative to the child window
  * RETURN
  *	void
  ************************************************************************************************
  * DESCRIPTION
  *	does the neccesary drawing routines on each of the instructions
  * METHOD:
  *
  ************************************************************************************************/
  
  	/*
  	** The parent window **
  	*/
  	CFlowZap *fc = ((CFlowZap *)m_fc);
  
  
  
  	/*
  	** DONT ALLOW SELECTION OF LAST STOP!! **
  	*/ 
  	if (this == fc->GetInstructionAtIndex (fc->GetInstructionsSize() -1) ) return;
  
  
  
  	/*
  	** Indicate if this instruction is moveable
  	*/ 
  	if (m_Moveable) fc->m_MouseMode = mmFZSelected;
  	else			fc->m_MouseMode = mmFZIllegalMove;
  
  
  
  	/*
  	** determine if this instruction is not already selected. . . 
  	*/ 
  	if (!m_Selected) { 
  		// play the sound
  		CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  		SoundPath += "/sounds/select.wav";
  		if (fc->IsSoundOn() ) PlaySound(SoundPath, NULL, SND_FILENAME | SND_ASYNC );
  	
  		// deselect the old instruction
  		fc->DeselectInstruction(); 
  		
  		// make this instruction selected
  		MakeSelected(TRUE);
  
  
  		// If we are in condensed view, then expand this window and update the window positions
  		if (!fc->IsExpanded() ) {
  			MakeExpanded(); 
  			fc->UpdateWindowPositions();
  		}
  
  		else RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
  
  
  		// set the selected instruction pointer to this instruction
  		fc->m_SelectedInstruction=this;
  
  		// make the neccessary menu items active now that we have a selected instruction
  		fc->MakeMenuInstructionActive(TRUE);
  		fc->SetEnableDisableMenu(m_Enabled);
  
  	}
  
  	/*
  	** return the mouse capture to the parent window 
  	*/
  	fc->SetCapture();
  	
  }
  
  
  
  
  
  
  
  int CInstruction::GetCenter(){
  /************************************************************************************************
  * DATE CREATED: 
  *	1 Jan 98 11:07 AM EST
  * LAST MODIFIED:
  *	16 Jun 98 10:49 AM EST
  * PARAMETERS:
  *	none
  * RETURN
  *	int		the center of this instruction in the y direction
  ************************************************************************************************
  * DESCRIPTION
  *	returns the y direction of this rectangle relative to the parent window
  * METHOD:
  *
  ************************************************************************************************/
  	CRect r;
  	GetWindowRect(&r);
  	((CFlowZap *)m_fc)->ScreenToClient(&r);
  	return r.TopLeft().y  + (r.Height()/2);
  }
  
  
  
  
  
  
  
  int CInstruction::GetHeight(){
  /************************************************************************************************
  * DATE CREATED: 
  *	1 Jan 98 11:07 AM EST
  * LAST MODIFIED:
  *	16 Jun 98 10:49 AM EST
  * PARAMETERS:
  *	none
  * RETURN
  *	int		the center of this instruction in the y direction
  ************************************************************************************************
  * DESCRIPTION
  *	returns the y height of this rectangle
  * METHOD:
  *
  ************************************************************************************************/
  	CRect r;
  	GetClientRect(&r);
  	return r.Height();
  }
  
  
  
  
  
  
  BOOL CInstruction::Between(CInstruction * a, CInstruction * b){
  /************************************************************************************************
  * DATE CREATED: 
  *	1 Jan 98 11:07 PM EST
  * LAST MODIFIED:
  *	16 Jan 98 10:30 PM EST
  * PARAMETERS:
  *	a	the first instruction
  	b	the second instruction
  * RETURN
  *	TRUE  if this instruction is between a and b
  *	FALSE if this instruction is not between a and b
  ************************************************************************************************
  * DESCRIPTION
  *	determines if this instruction is between a and b (not inclusive)
  * METHOD:
  *	"betweeness" is checked by indexes of the instructions
  ************************************************************************************************/
  
  
  	if ((m_Index > a->GetIndex() ) && (m_Index < b->GetIndex() )) return TRUE;
  	return FALSE;
  }
  
  
  
  
  
  void CInstruction::MouseButtonUp(UINT nFlags, CPoint point)
  {
  	CFlowZap *fc = ((CFlowZap *)m_fc);
  	fc->UnhighlightArrow();
  	
  	fc->m_MouseMode = mmFZDefault;
  	fc->SetCursor();
  	
  }
  
  void CInstruction::OnInit()
  {
  
  	/*
  	** set the defaults as moveable, not selected and enabled with no pointer
  	*/
  	m_Moveable = TRUE;
  	m_Enabled  = TRUE;
  	m_Selected = FALSE;
  
  
  	/*
  	** if this instruction has an associated loop, m_Loop will point to it
  	*/
  	m_Loop = NULL;
  
  
  	/*
  	** if this instruction is being pointed to by a loop, this pointer will lead to that instruction
  	*/
  	m_LoopStart = NULL;
  
  
  	/*
  	** determine the origional size of the window so that we can go back it in expanded views
  	*/
  	CRect r;
  	GetWindowRect (&r);
  	m_ExpandedHeight = r.Height();
  
  
  	/*
  	** determine what view setting the user has and set this instruction to that setting
  	*/
  	m_Expanded = ((CFlowZap*)m_fc)->IsExpanded();
  
  
  	/*
  	** make this instruction condensed if the view is condensed (we can assume that it is not selected)
  	*/
  	if (!m_Expanded) MakeCondensed();
  
  }
  
  
  
  
  
  
  void CInstruction::MakeMoveable(BOOL moveable)
  {
  	m_Moveable = moveable;
  	ShowBorder (!moveable);
  	// this next line is included to keep the white box from appearing around the text
  	// in the condensed view
  	if (!m_Expanded) RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
  }
  
  
  
  
  
  /*************************************************** 
  ***** START PRIVATE MEMBER FUNCTION ACCESS *********
  ****************************************************/
  BOOL CInstruction::IsSelected(){return m_Selected;}
  
  BOOL CInstruction::IsMoveable(){return m_Moveable;}
  
  void CInstruction::SetIndex(int index){m_Index = index;}
  
  void CInstruction::MakeSelected(BOOL s){m_Selected = s;}
  
  int CInstruction::GetIndex(){return m_Index;}
  /*************************************************  
  ***** END PRIVATE MEMBER FUNCTION ACCESS *********
  **************************************************/
  
  
  
  #ifdef ROBOT_WORLD_PROJECT
  TTaskResult CInstruction::Execute(CRobot * Robot, CExecutableRWGraphicObjectList* ExecutableObjectList, bool & result)
  {
  	result = true;
  	return TR_TaskComplete;
  }
  #endif