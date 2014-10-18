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
  // FlowZap.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "FlowZap.h"
  #include "Loop.h"
  
  #include "instruction.h"
  #include "leftlooppanel.h"
  #include "rightlooppanel.h"
  #include "pallette.h"
  #include "cue.h"
  #include "dropoff.h"
  #include "arrow.h"
  #include "util.h"
  #include "execute.h"
  
  
  #include "Beep.h"
  #include "Repeat.h"
  #include "Branch.h"
  #include "Wait.h"
  #include "Goto.h"
  #include "Start.h"
  #include "Sub.h"
  #include "Stop.h"
  #include "Vacuum.h"
  #include "Calibrate.h"
  #include "Motion.h"
  #include "PathFollow.h"
  #include <iostream>
  #include "Preferences.h"
  #include "Tool.h"
  #include "ExternalOutput.h"
  #include "..\RobotWorld.h"
  #include "..\RobotWorldView.h"
  
  #include "..\RothUtil.h"
  
  #include "afx.h"

  #define FLOWZAP_WINDOW_SECTION "FLOWZAP_WINDOW_DIM"
  
  const char* cFlowZapFilesDirectory = ".\\Zap Files";
  
  /////////////////////////////////////////////////////////////////////////////
  // CFlowZap dialog
  
  
  CFlowZap::CFlowZap(CWnd* pParent /*=NULL*/, CExecutableRWGraphicObjectList* ExecutableObjectList)
  	: CDialog(CFlowZap::IDD, pParent)
  	
  {
  	m_ActiveRobot = NULL;
  	m_Pallette = NULL;
  	m_RobotWorldView = (CRobotWorldView*)pParent;
  	//{{AFX_DATA_INIT(CFlowZap)
  	//}}AFX_DATA_INIT
  	m_Pause = false;
  	m_Playing = false;
  	m_SubProgramWindow = NULL;
  	m_IsSubroutineWindow = false;
  
  	m_ExecutableObjectList = ExecutableObjectList;
  
   /*
    * Set the mouse mode to the default *
    */
  	m_MouseMode = mmFZDefault;
  
  
   /*
    * Init all of the working zones *
    */
  	m_Cue      = new CCue(this);
  	m_llp      = new CLeftLoopPanel (this);
  	m_rlp      = new CRightLoopPanel (this);
  	m_DropOff  = new CDropOff(this);
  
  
  
   /*
    * Set the program pointers to NULL *
    */
  	m_SelectedInstruction = NULL; // this is the currently selected instruction
  	m_hArrow = NULL;  // this is highlighted arrow
  
  
   /*
    * Make the initial view mode expanded (this may change with preferences) *
    * this does change when a program is read in from file (prev state is saved) *
    */
  	m_ExpandedView = TRUE;
  
  
   /*
    * Begin Assuming that the program has not been named yet*
    * and that there is a file open
    * and that there has been no change in the file that needs to be saved*
    */
  	m_NeedToSaveAs = TRUE;
  	m_FileIsOpen = TRUE;
  	m_ChangeMadeInFile = FALSE;
  
  
  
   /*
    * Set the program mode to EditMode *
    */
  	SwitchToExecuteMode (FALSE);
  
  
  
  	m_OldVScrollPosition = 0;
  
  
  
  	// this is for executing programs
  	m_ExecuteControl = NULL;
  
  
   /*
    * Read in the preferences file *
    */
  	m_PreferencesControl = new CPreferences(this);
  	m_PreferencesControl->ReadPrefs();
  
  
  
  	m_CalibrationZones = NULL;
  	m_VacuumPatches = NULL;
  	m_LabelNames = NULL;
  	m_LinePaths = NULL;
  	
  	
  
  }
  
  CFlowZap::~CFlowZap()
  {
  	delete m_Cue;
  	delete m_llp;
  	delete m_rlp;
  	delete m_DropOff;
  	delete m_PreferencesControl;
  	delete m_Pallette;
  	delete m_Tool;
  	if (m_ExecuteControl != NULL) delete m_ExecuteControl;
  
  	EmptyArrowList();
  	EmptyInstructionList();
  
  	// Purge the string lists from Robot World
  	PurgeStringList (m_CalibrationZones);
  	PurgeStringList (m_LabelNames);
  	PurgeStringList (m_VacuumPatches);
  	PurgeStringList (m_LinePaths);
  
  	if (m_SubProgramWindow != NULL) {
  		delete m_SubProgramWindow;
  		m_SubProgramWindow = NULL;
  	}
  }
  
  void CFlowZap::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CFlowZap)
  	DDX_Control(pDX, IDC_BORDER, m_Border);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CFlowZap, CDialog)
  	ON_WM_CONTEXTMENU()
  	//{{AFX_MSG_MAP(CFlowZap)
  	ON_WM_SETCURSOR()
  	ON_WM_LBUTTONDOWN()
  	ON_WM_LBUTTONUP()
  	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
  	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
  	ON_WM_MOUSEMOVE()
  	ON_WM_CANCELMODE()
  	ON_WM_PAINT()
  	ON_WM_CAPTURECHANGED()
  	ON_WM_KEYUP()
  	ON_WM_KEYDOWN()
  	ON_WM_CLOSE()
  	ON_COMMAND(ID_VIEW_CONDENSED, OnViewCondensed)
  	ON_COMMAND(ID_INSTRUCTION_DELETE, OnInstructionDelete)
  	ON_WM_LBUTTONDBLCLK()
  	ON_COMMAND(ID_INSTRUCTION_SET_CUE, OnInstructionSetCue)
  	ON_COMMAND(ID_VIEW_EXPANDED, OnViewExpanded)
  	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
  	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
  	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
  	ON_COMMAND(ID_FILE_NEW, OnFileNew)
  	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
  	ON_UPDATE_COMMAND_UI(ID_VIEW_CONDENSED, OnUpdateViewCondensed)
  	ON_WM_SIZING()
  	ON_COMMAND(ID_INSTRUCTION_DISABLE, OnInstructionDisable)
  	ON_COMMAND(ID_INSTRUCTION_ENABLE, OnInstructionEnable)
  	ON_WM_VSCROLL()
  	ON_WM_SETFOCUS()
  	ON_COMMAND(ID_INSERT_CALIBRATE, OnInsertCalibrate)
  	ON_COMMAND(ID_INSERT_EXECUTE, OnInsertExecute)
  	ON_COMMAND(ID_INSERT_GOTO, OnInsertGoto)
  	ON_COMMAND(ID_INSERT_IF, OnInsertIf)
  	ON_COMMAND(ID_INSERT_REPEAT, OnInsertRepeat)
  	ON_COMMAND(ID_INSERT_SING, OnInsertSing)
  	ON_COMMAND(ID_INSERT_STOP, OnInsertStop)
  	ON_COMMAND(ID_INSERT_VACUUM, OnInsertVacuum)
  	ON_COMMAND(ID_INSERT_MOTION, OnInsertMotion)
  	ON_COMMAND(ID_INSERT_WAIT, OnInsertWait)
  	ON_COMMAND(ID_INSERT_FOLLOWPATH, OnInsertFollowPath)
  	ON_COMMAND(ID_INSERT_EXTERNAL_OUTPUT, OnInsertExternalOutput)
  	ON_COMMAND(ID_POPUP_DELETE, OnPopupDelete)
  	ON_COMMAND(ID_POPUP_DISABLE, OnPopupDisable)
  	ON_COMMAND(ID_POPUP_ENABLE, OnPopupEnable)
  	ON_COMMAND(ID_EDIT_PREFERENCES, OnEditPreferences)
  	ON_WM_SHOWWINDOW()
  	ON_WM_NCMOUSEMOVE()
  	ON_WM_SIZE()
  	ON_WM_DESTROY()
  	ON_COMMAND(ID_POPUP_CLOSE, OnFileClose)
  	ON_COMMAND(ID_POPUP_EXIT, OnFileExit)
  	ON_COMMAND(ID_POPUP_NEW, OnFileNew)
  	ON_COMMAND(ID_POPUP_OPEN, OnFileOpen)
  	ON_COMMAND(ID_POPUP_SAVE, OnFileSave)
  	ON_COMMAND(ID_POPUP_SAVEAS, OnFileSaveas)
  	ON_WM_CREATE()
  	//}}AFX_MSG_MAP
  	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CFlowZap message handlers
  
  
  void CFlowZap::SetCursor()
  {
  	OnSetCursor(this, HTCLIENT, WM_SETCURSOR); 	
  }
  
  BOOL CFlowZap::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
  {
  	if (nHitTest == HTCLIENT && nHitTest != HTVSCROLL && nHitTest != HTHSCROLL) {
  		switch (m_MouseMode) {
  		case mmFZStop:		::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_STOP)));		return true;	break;
  		case mmFZBeep:		::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_BEEP)));		return true;	break;
  		case mmFZVac:		::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_VACUUM)));	return true;	break;
  		case mmFZCal:		::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_CAL)));		return true;	break;
  		case mmFZRepeat:	::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_REPEAT)));	return true;	break;
  		case mmFZWait:		::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_WAIT_CURSOR)));		return true;	break;
  		case mmFZGoto:		::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_GOTO)));		return true;	break;
  		case mmFZBranch:	::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_BRANCH)));	return true;	break;
  		case mmFZSub:		::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_SUB)));		return true;	break;
  		case mmFZMotion:	::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MOTION)));		return true;	break;
  		case mmFZExternalOutput:	::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_EXTERNAL_OUTPUT)));		return true;	break;
  
  		case mmFZMove:	::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MOVE)));		return true;	break;
  		case mmFZPath:	::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_PATH)));		return true;
  		case mmFZMoveRepeatLoop:	::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MOVE))); return true;	break;
  		case mmFZMoveBranchLoop:	::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MOVE))); return true;	break;
  		case mmFZMoveCue:			::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_MOVE))); return true;	break;
  
  		case mmFZDefault: ::SetCursor(::LoadCursor(NULL,IDC_ARROW)); return true;	break;
  
  		}
  	}
  	return CDialog::OnSetCursor(pWnd, nHitTest, message);
  }
  
  
  
  
  void CFlowZap::OnLButtonDblClk(UINT nFlags, CPoint point) {
  /************************************************************************************************
  * CREATED: 1/1/98 12:00PM
  *
  * LAST MODIFIED: 1/20/98 10:43PM
  *
  * PARAMETERS
  *	point	the point of the user click
  * RETURN
  *	
  ************************************************************************************************
  * DESCRIPTION
  *	if the user clicks in the cue area, move the cue to the instruction near the click
  * METHOD:
  *
  ************************************************************************************************/
  
   /*
    * Do not allow any editing if the program mode is not in edit mode*
    */
  	if (m_ProgramMode != pmEdit) return;
  
  
  
   /*
    * Check if the click is inside the cue area*
    */
  
  	if (m_Cue->Contains(point) ){
  		m_Cue->OnLButtonDbClk(point);
  		RedrawWindow(m_Cue->m_Loc, NULL, RDW_ERASE | RDW_INVALIDATE);
  	}
  	
  	CDialog::OnLButtonDblClk(nFlags, point);
  }
  
  
  
  
  
  void CFlowZap::OnLButtonDown(UINT nFlags, CPoint point) {
  /************************************************************************************************
  * CREATED: 1/1/98 12:00PM
  *
  * LAST MODIFIED: 1/20/98 10:45PM
  *
  * PARAMETERS
  *	point	the point of the user click
  * RETURN
  *	
  ************************************************************************************************
  * DESCRIPTION
  *	determine where in the window the user has clicked and set the mouse mode accordingly
  * METHOD:
  *
  ************************************************************************************************/
  	SetCapture();
   /*
    * Do not allow any editing if the program mode is not in edit mode*
    */
  	if (m_ProgramMode != pmEdit || !m_FileIsOpen) return;
  
  
  
   /*
    * Check if the user click is in the Cue Area
    */
  	else if (m_Cue->Contains(point) ) {
  		m_MouseMode = mmFZCue;
  		m_Cue->OnLButtonDown(point);
  		RedrawWindow(m_Cue->m_Loc, NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
  	}
  
  
   /*
    * Check if the user click is in the Left Loop Panel
    */
  	else if (m_llp->Contains(point) ) {
  		m_MouseMode = mmFZRepeatLoop;
  		m_llp->OnLButtonDown(point);
  		// !!!*** LEAVE THE RDW_ERASE IN TO HANDLE THE WHITE BOX AROUND THE TEXT ***!!!
  		RedrawWindow(m_llp->m_Loc, NULL, RDW_INVALIDATE | RDW_NOCHILDREN | RDW_ERASE);
  	}
  
  
   /*
    * Check if the user click is in the Right Loop Panel
    */
  	else if (m_rlp->Contains(point) ) {
  		m_MouseMode = mmFZBranchLoop;
  		m_rlp->OnLButtonDown(point);
  		RedrawWindow(m_rlp->m_Loc, NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
  	}
  
  
   /*
    * Set the cursor accordingly
    */
  	SetCursor();
  
  	CDialog::OnLButtonDown(nFlags, point);
  }
  
  
  void CFlowZap::OnNcMouseMove(UINT nHitTest, CPoint point) 
  {
  	/*
    * If there is no button pressed, clear the icons and then do nothing.
    */
  	
  
  	CDialog::OnNcMouseMove(nHitTest, point);
  }
  
  
  
  void CFlowZap::OnMouseMove(UINT nFlags, CPoint point) {
  /************************************************************************************************
  * CREATED: 1/1/98 12:00PM
  *
  * LAST MODIFIED: 1/21/98 5:28PM
  *
  * PARAMETERS
  *	point	the point of the user click
  * RETURN
  *	
  ************************************************************************************************
  * DESCRIPTION
  *	if the user clicks in the cue area, move the cue to the instruction near the click
  * METHOD:
  *
  ************************************************************************************************/
  
   /*
    * If there is no button pressed, clear the icons and then do nothing.
    */
  	if (!(nFlags & MK_LBUTTON) ) {
  		CDialog::OnMouseMove(nFlags, point);
  		return;
  	}
  
  
   /*
    * if we are not in edit mode, then do nothing.
    */
  	else if (m_ProgramMode != pmEdit || !m_FileIsOpen) {
  		CDialog::OnMouseMove(nFlags, point);
  		return;
  	}
  
  	
  	else if (m_MouseMode == mmFZMoveCue)          /*THEN*/ m_Cue->OnMouseMove(nFlags, point);
  	else if (m_MouseMode == mmFZMoveRepeatLoop)   /*THEN*/ m_llp->OnMouseMove(nFlags, point);
  	else if (m_MouseMode == mmFZMoveBranchLoop)   /*THEN*/ m_rlp->OnMouseMove(nFlags, point);
  
  
  	else if (m_MouseMode == mmFZSelected) {
  		if (m_SelectedInstruction != NULL) {
  			CRect r;
  			m_SelectedInstruction->GetWindowRect(&r);
  			ScreenToClient(&r);
  			//if (point.y < r.TopLeft().y || point.y > r.BottomRight().y) 
  			m_MouseMode = mmFZMove;
  
  		}
  	}
  
  	else if (m_MouseMode == mmFZMove) MoveInstruction(point);
  		
  	else if (m_MouseMode >= mmFZVac && m_MouseMode <= mmFZGoto) TrackMouse (point);
  	else {
  		// Do nothing in these cases: mmFZCue, mmFZBranchLoop, mmFZRepeatLoop, mmFZDefault
  
  	}
  	
  	CDialog::OnMouseMove(nFlags, point);
  }
  
  
  
  
  
  void CFlowZap::OnLButtonUp(UINT nFlags, CPoint point) {
  /************************************************************************************************
  * CREATED: 1/1/98 12:00PM
  *
  * LAST MODIFIED: 1/20/98 10:49PM
  *
  * PARAMETERS
  *	point	the point of the user click
  * RETURN
  *	
  ************************************************************************************************
  * DESCRIPTION
  *	depending on the mouse mode, do the correct action, and redraw the correct area of the window
  * METHOD:
  *
  ************************************************************************************************/
  
  
  	ReleaseCapture();
  	m_hArrow = NULL;
  
   /*
    * Do not allow any editing if the program mode is not in edit mode*
    */
  	if (m_ProgramMode != pmEdit || !m_FileIsOpen) {
  		// Do nothing here
  	}
  	
  
  	//DESELECT INSTRUCTION
  	else if (m_MouseMode == mmFZCue || m_MouseMode == mmFZBranchLoop || m_MouseMode == mmFZRepeatLoop) {
  		DeselectInstruction();
  		MakeMenuInstructionActive(FALSE);
  	}
  
  
  	//CUE
  	else if (m_MouseMode == mmFZMoveCue) {
  		m_Cue->OnLButtonUp(point);
  		RedrawWindow(m_Cue->m_Loc, NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
  		ChangeMadeInFile();
  	}
  
  	
  	//MOVE REPEAT LOOP
  	else if (m_MouseMode == mmFZMoveRepeatLoop) {
  		m_llp->OnLButtonUp(point);
  		RedrawWindow(m_llp->m_Loc, NULL, RDW_INVALIDATE | RDW_NOCHILDREN | RDW_ERASE);
  		ChangeMadeInFile();
  	}
  
  	//MOVE BRANCH LOOP
  	else if (m_MouseMode == mmFZMoveBranchLoop) {
  		m_rlp->OnLButtonUp(point);
  		RedrawWindow(m_rlp->m_Loc, NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
  		ChangeMadeInFile();
  	}
  
  	// MOVE INSTRUCTION
  	else if (m_MouseMode == mmFZMove) {
  		//UpdateWindowPositions();
  
  	}
  
  
  	// ADD NEW INSTRUCTION
  	else if (m_MouseMode >= mmFZVac && m_MouseMode <= mmFZGoto) {
  		CInstruction *ptr = GetInstructionAtPixel(point);
  		if (point.y > m_Origional_Origin) AddInstruction (m_MouseMode, ptr);
  		
  		
  	}
  
  
  	if (point.y > m_DropOff->m_Loc.bottom) {
  		DeselectInstruction();
  		MakeMenuInstructionActive(FALSE);
  	}
  	// always reset the mouse mode
  	m_MouseMode = mmFZDefault;
  	//reset the cursor to the pointer
  	SetCursor(); 
  	CDialog::OnLButtonUp(nFlags, point);
  	SetFocus();
  }
  
  
  
  
  
  int CFlowZap::TrackMouse(CPoint point)
  {
  /************************************************************************************************
  * CREATED: 1/21/98 1:09PM
  *
  * LAST MODIFIED: 1/21/98 1:22PM
  *
  * PARAMETERS
  *	point	the point of the location of the mouse
  * RETURN
  *	
  ************************************************************************************************
  * DESCRIPTION
  *	determines which mouse to highlight and if the arrows need to be repainted
  * METHOD:
  *
  ************************************************************************************************/
  	CArrow* ar;
  	CInstruction* in;
  	BOOL needRedraw = FALSE;
  	if (point.y > m_Origional_Origin){
  		for (int i = 0; i < m_arrows.GetCount(); i ++) {	
  			ar = (CArrow *)m_arrows.GetAt (m_arrows.FindIndex(i) );
  			in = GetInstructionAtIndex (i+1);
  			if (point.y < ar->m_loc.y + in->GetHeight() +2) {
  				if (m_hArrow != ar){
  					if (m_hArrow != NULL) 
  						RedrawWindow(m_hArrow->GetBoundingRectangle(), NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
  					m_hArrow = ar;
  					needRedraw = TRUE;
  				}
  				break;
  			}
  		}	
  		if (needRedraw) {
  			RedrawWindow(m_hArrow->GetBoundingRectangle() , NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
  		}
  	}
  	ar = NULL;
  	if (m_hArrow != NULL) return m_hArrow->m_loc.y;
  	else return 0;
  }
  
  
  
  
  void CFlowZap::AddInstruction(TFZMouseMode mode, CInstruction* ptr)
  {
  	CInstruction *in = NULL;
  	int index=0;
  	switch (mode) {
  	
  	case mmFZBeep   : in = makeBeep(index);		break;
  	case mmFZStop   : in = makeStop(index);		break;
  	case mmFZRepeat : in = makeRepeat(index);	in->m_Loop = m_llp->AddLoop(in, in);	break;
  	case mmFZBranch : in = makeBranch(index);	in->m_Loop = m_rlp->AddLoop(in, in);	break;
  	case mmFZGoto   : in = makeGoto(index);		break;	
  	case mmFZWait   : in = makeWait(index);		break;
  	case mmFZSub	: in = makeSub(index);		break;
  	case mmFZVac	: in = makeVac(index);		break;	
  	case mmFZCal	: in = makeCal (index);		break;
  	case mmFZMotion	: in = makeMotion (index);	break;
  	case mmFZExternalOutput	: in = makeExternalOutput(index); break;
  	case mmFZPath	: in = makePathFollow (index);	break;
  	}
  
  	if (in != NULL) {
  		CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  		SoundPath += "/sounds/addInstruction.wav";
  		if (IsSoundOn() )PlaySound (SoundPath, NULL, SND_FILENAME | SND_ASYNC ); 
  		InsertInstructionBefore(ptr, in);
  	}
  	UpdateRepeats(); // updates the numbers on the repeat loops
  	
  }
  
  
  
  
  
  void CFlowZap::InsertInstructionAfter(CInstruction* bench, CInstruction* insert){
  /************************************************************************************************
  * CREATED:			1/1/98 1:25PM
  *
  * LAST MODIFIED:	1/21/98 4:00PM
  *
  * PARAMETERS
  *	bench	the benchmark instruction in the list
  *	insert	the instruction to insert after the bench
  * RETURN
  *	
  ************************************************************************************************
  * DESCRIPTION
  *	inserts an instruction into the list after the bench instruction
  * METHOD:
  *	SETS THE FLAG FOR A CHANGE MADE IN THE FILE
  ************************************************************************************************/
  	if (bench == NULL) bench = GetInstructionAtIndex(GetInstructionsSize() -2);
  	if (bench == GetInstructionAtIndex(GetInstructionsSize() -1 ) ) bench = GetInstructionAtIndex(GetInstructionsSize() -2);
  	POSITION p = m_instructions.Find(bench);
  	
  	m_instructions.InsertAfter (p, insert);
  	insert->ShowWindow(SW_SHOW);
  	AddArrow();
  	UpdateWindowPositions();
  	ChangeMadeInFile();
  }
  
  
  
  
  
  
  void CFlowZap::InsertInstructionBefore(CInstruction * bench, CInstruction * insert){
  /************************************************************************************************
  * CREATED:			1/21/98 3:52PM
  *
  * LAST MODIFIED:	1/21/98 4:00PM
  *
  * PARAMETERS
  *	bench	the benchmark instruction in the list
  *	insert	the instruction to insert after the bench
  * RETURN
  *	
  ************************************************************************************************
  * DESCRIPTION
  *	inserts an instruction into the list before the bench instruction
  * METHOD:
  *	SETS THE FLAG FOR A CHANGE MADE IN THE FILE
  ************************************************************************************************/
  	if (bench == NULL) bench = GetInstructionAtIndex(GetInstructionsSize() -2);
  	if (bench == GetInstructionAtIndex(0) ) bench = GetInstructionAtIndex(1);
  	POSITION p = m_instructions.Find(bench);
  	
  	m_instructions.InsertBefore (p, insert);
  	insert->ShowWindow(SW_SHOW);
  	AddArrow();
  	UpdateWindowPositions();
  	ChangeMadeInFile();
  }
  
  
  
  
  
  
  int CFlowZap::AddArrow(){
  /************************************************************************************************
  * CREATED:			1/1/98 12:00PM
  *
  * LAST MODIFIED:	1/20/98 11:10AM
  *
  * PARAMETERS
  *	point	the point of the user click
  * RETURN
  *	
  ************************************************************************************************
  * DESCRIPTION
  *	adds a new arrow to the list
  * METHOD:
  *
  ************************************************************************************************/
  	m_arrows.AddHead (new CArrow ( CPoint (215, 116), this ) );
  	return 0;
  }
  
  
  
  
  
  void CFlowZap::UnhighlightArrow(){
  /************************************************************************************************
  * CREATED: 1/20/98 11:07AM
  *
  * LAST MODIFIED: 1/20/98 11:10AM
  *
  * PARAMETERS
  *	none
  * RETURN
  *	
  ************************************************************************************************
  * DESCRIPTION
  *	makes the highlighted arrow (m_hArrow) NULL;
  * METHOD:
  *
  ************************************************************************************************/
  	if (m_hArrow != NULL) {
  		m_hArrow = NULL;
  	}
  }
  
  
  
  
  
  
  
  CInstruction* CFlowZap::GetInstructionAtPixel(CPoint p){
  /************************************************************************************************
  * CREATED: 1/20/98 11:07AM
  *
  * LAST MODIFIED: 2/24/98 4:16 PM
  *
  * PARAMETERS
  *	p		the Point in (x,y) of the user click
  * RETURN
  *	CInstruction * in	the instruction the point is associated with
  ************************************************************************************************
  * DESCRIPTION
  *	determines which instruction the point is on or before in the list.
  *	
  * METHOD:
  *	if the click is below the last instruction, a NULL pointer is returnded
  ************************************************************************************************/
  	
  	CInstruction* in;
  	CRect windRect;
  	for (int i = 0; i < m_instructions.GetCount(); i ++) {	
  		in = GetInstructionAtIndex(i);
  		in->GetWindowRect (&windRect);
  		ScreenToClient(&windRect);
  		if (p.y < windRect.BottomRight().y) return in;
  		
  	}
  	return NULL;
  }
  
  
  
  
  
  
  
  CInstruction* CFlowZap::makeRepeat(int index)
  {
  	CRepeat *newRepeat = new CRepeat(this, index, m_llp);
  	newRepeat->Create(IDD_REPEAT, this);
  	return (newRepeat);
  
  }
  
  CInstruction * CFlowZap::makeVac(int index)
  {
  
  	CVacuum *newVac = new CVacuum(this, index);
  	newVac->Create(IDD_VACUUM, this);
  	if (m_VacuumPatches != NULL)  newVac->InsertVacuumPatches (m_VacuumPatches);
  	return (newVac);
  
  }
  
  CInstruction* CFlowZap::makePathFollow(int index)
  {
  	CPathFollow *newPath = new CPathFollow(this, index);
  	newPath->Create(IDD_PATH, this);
  	if (m_LinePaths != NULL) newPath->InsertLinePaths(m_LinePaths);
  	return (newPath);
  }
  
  CInstruction * CFlowZap::makeStart(int index)
  {
  	CStart *newStart = new CStart(this, index);
  	newStart->Create(IDD_START, this);
  	return (newStart);
  }
  
  CInstruction* CFlowZap::makeStop(int index)
  {
  	CStop *newStop = new CStop(this, index);
  	newStop->Create(IDD_STOP, this);
  	return (newStop);
  }
  
  
  CInstruction* CFlowZap::makeBeep(int index)
  {
  	CBeep *newBeep = new CBeep(this, index);
  	newBeep->Create(IDD_BEEP, this);
  	return (newBeep);
  }
  
  CInstruction* CFlowZap::makeGoto(int index)
  {
  	CGoto *newGoto = new CGoto(this, index);
  	newGoto->Create(IDD_GOTO, this);
  	if (m_LabelNames != NULL)  newGoto->InsertLabelNames (m_LabelNames);
  	return (newGoto);
  
  }
  
  CInstruction * CFlowZap::makeCal(int index)
  {
  	CCalibrate *newCalibrate = new CCalibrate(this, index);
  	newCalibrate->Create(IDD_CAL, this);
  	if (m_CalibrationZones != NULL) newCalibrate->InsertCalibrationZones (m_CalibrationZones);
  	return (newCalibrate);
  
  }
  
  CInstruction* CFlowZap::makeBranch(int index)
  {
  	CBranch *newBranch = new CBranch(this, index);
  	newBranch->Create(IDD_BRANCH, this);
  	return (newBranch);
  
  }
  
  CInstruction* CFlowZap::makeWait(int index)
  {
  	CWait *newWait = new CWait(this, index);
  	newWait->Create(IDD_WAIT, this);
  	return (newWait);
  
  }
  
  CInstruction * CFlowZap::makeSub(int index)
  {
  	CSub *newSub = new CSub(this, index);
  	newSub->Create(IDD_SUB, this);
  	return (newSub);
  }
  
  
  CInstruction* CFlowZap::makeMotion(int index)
  {
  	CMotion *newMotion = new CMotion(this, index);
  	newMotion->Create(IDD_MOTION, this);
  	return (newMotion);
  }
  
  CInstruction* CFlowZap::makeExternalOutput(int index)
  {
  	CExternalOutput *newExternalOutput = new CExternalOutput(this, index);
  	newExternalOutput->Create(IDD_EXTERNAL_OUTPUT, this);
  	return newExternalOutput;
  }
  
  
  
  
  
  void CFlowZap::UpdateArrowPositions(int arrowHeight)
  {
  	CArrow *ar;
  	CInstruction* in;
  	CRect windRect;
  	int y = m_Origin; 
  
  	for (int i = 0; i < m_arrows.GetCount(); i ++) {	
  		ar = (CArrow *)m_arrows.GetAt (m_arrows.FindIndex(i) );
  		in = GetInstructionAtIndex(i);
  		in->GetWindowRect (&windRect);
  		
  		y += windRect.Height() + arrowHeight;
  		ar->SetLocation (m_InstructionMidPoint, y-2);
  	}
  }
  
  
  
  
  
  CRect CFlowZap::UpdateInstructionPositions(int arrowHeight){
  /************************************************************************************************
  * CREATED: 2/26/98 10:14AM
  *
  * LAST MODIFIED: 2/26/98 10:14AM
  *
  * PARAMETERS
  *	int		the height of the arrow (15 if expanded, 8 if condensed
  * RETURN
  *	CRect	the rectangle around the instructions	
  ************************************************************************************************
  * DESCRIPTION
  *	determines the location of each of the instructions in the list
  * METHOD:
  *	also initializes the location of the border frame around the list of instructions
  ************************************************************************************************/
  
  	int y = m_Origin;
  	
  	CInstruction * in;
  	CRect windRect, parentRect;
  	GetWindowRect (&parentRect);
  	for (int i = 0; i < m_instructions.GetCount(); i ++) {	
  		in = (CInstruction *)m_instructions.GetAt (m_instructions.FindIndex(i) );
  		in->SetIndex(i); // might as well set the index every time we redraw here.
  		in->GetClientRect (&windRect);
  		in->SetWindowPos (NULL, m_xPosIndent, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER); 
  		y += windRect.Height() + arrowHeight;
  	}
  
  	UpdateArrowPositions(arrowHeight);
  
  	m_rlp->UpdateRectangle      (dZONE_NOCHANGE, m_Origin,  dZONE_NOCHANGE, y - 10); // dZONE_NOCHANGE = -1
  	m_llp->UpdateRectangle      (dZONE_NOCHANGE, m_Origin,  dZONE_NOCHANGE, y - 10);
  	m_Cue->UpdateRectangle      (dZONE_NOCHANGE, m_Origin,  dZONE_NOCHANGE, y - 10);
  	m_DropOff->UpdateRectangle  (dZONE_NOCHANGE, m_Origin,  dZONE_NOCHANGE, y - 6);
  
    /*
     * Handle ScrollBars [determine if the vertical scrollbar should be shown
     */
  	CRect borderRect;
  	m_Border.GetClientRect(&borderRect); // Gets the width and height of the border rectangle
  	GetWindowRect (&parentRect);
  	m_Border.MoveWindow  (3, m_Origin - 5, borderRect.Width()  ,y - (m_Origin - 5), TRUE);
  //	m_Border.MoveWindow  (3, m_Origin - 5, parentRect.Width() - 10  ,y - (m_Origin - 5), TRUE);
  	GetClientRect (&parentRect);
  	m_ListHeight = y - (m_Origin - 5);
  	CRect pRect;
  	m_Pallette->GetClientRect(&pRect);
  	m_ScrollingArea = parentRect.Height() - pRect.Height();
  	AdjustScrollBars(m_ScrollingArea, m_ListHeight);
  	
  
  
    /*
     * Always keep the Pallette on top of the instruciton windows
     */
  	m_Pallette->BringWindowToTop();	
  	CRect r;
  	m_Border.GetWindowRect (&r);
  	ScreenToClient(&r);
  	return r;
  }
  
  
  
  
  
  
  
  void CFlowZap::UpdateWindowPositions()
  {
  	CRect redrawArea = UpdateInstructionPositions( (m_ExpandedView) ? 15 : 8);
  	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE ); // too much flicker
  }
  
  
  
  
  
  
  BOOL CFlowZap::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  
    /*
     * Make the tool window, hide it at first,
     * this will be shown when the user closes a flowzap file w/0 opening a new one
     */
  	m_Tool = new CTool(this);
  	m_Tool->Create(IDD_TOOLS, this);
  	m_Tool->ShowWindow (SW_HIDE);
  
  	CRect toolRect, parentRect;
  	GetClientRect (&parentRect);
  	m_Tool->GetWindowRect (&toolRect);
  	int xPos = (parentRect.Width() - toolRect.Width() )/2;
  	m_Tool->MoveWindow  (xPos, 100, toolRect.Width(), toolRect.Height(), TRUE);
  
  
  
    /*
     * Make the lookup tables for the beep panels
     */	
  	MakeLookupTables();
  	
  
    /*
     * Init ScrollBar [don't show it at first];
     */
  	ShowScrollBar (SB_VERT, FALSE);
  
    /*
     * Init the location of the pallette window
     */
  	InitPallette();
  
    /*
     * Init the menu bar
     */
  	InitMenuBar();
  
  
    /*
     * Determine the correct file to open on startup, and open it
     */
  	BOOL result = OpenFileOnStartup();
  
  
    /*
     * Keep a record of the origional width for use in resizing the window
     */
  	CRect r;
  	GetWindowRect (&r);
  	m_OrigionalWindowWidth = r.Width();
  
  
  	
  	if (m_PreferencesControl->m_TestCondensedView) MakeViewExpanded(FALSE);
  	else MakeViewExpanded(TRUE);
  
  
  
  
  	// CG: The following block was added by the ToolTips component.
  	{
  		// Create the ToolTip control.
  		m_tooltip.Create(this,TTS_ALWAYSTIP);
  		m_tooltip.Activate(TRUE);
  		m_Border.EnableToolTips(TRUE);
  
  		// TODO: Use one of the following forms to add controls:
  		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
  
  		m_tooltip.AddTool(GetDlgItem(IDC_BORDER), "Click and drag into list to insert");
  	}
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  
  
  
  
  
  BOOL CFlowZap::OpenFileOnStartup(){
  /************************************************************************************************
  * CREATED: 3/3/98 3:08 PM EST
  *
  * LAST MODIFIED: 6/15/98 3:08 PM EST
  *
  * PARAMETERS
  *	none
  * RETURN
  *	BOOL, true if successful
  ************************************************************************************************
  * DESCRIPTION
  *	determines a file to open on startup
  * METHOD:
  *	
  ************************************************************************************************/
    int i;
  
  	// IF OPEN PREV FILE SELECTED IN THE PREFERENCES FILE:
  	if (m_PreferencesControl->m_TestLastProject) {
  		if (OpenFile(m_PreferencesControl->m_PreviousFileName)) {
  			CString FileName = m_PreferencesControl->m_PreviousFileName;
  			for (i = FileName.GetLength() - 1; i >= 0; i--) {
  				if (FileName.GetAt(i) == '\\') break;
  			}
  			FileName.Delete(0, i + 1);
  //			CRobotWorldApp* MainApp = (CRobotWorldApp*)AfxGetApp();
  //			CString DefaultPath = MainApp->m_DefaultPath;
  //			DefaultPath += (cFlowZapFilesDirectory + 1) + "\\";
  //			FileName->SpanExcluding(DefaultPath);
  #ifdef ROBOT_WORLD_PROJECT
  			m_RobotWorldView->OnFlowZapChangeFileOpen(FileName);
  #endif
  /*		CStdioFile *file = OpenFileForReading(m_PreferencesControl->m_PreviousFileName);
  		if (file != NULL) {
  			m_RobotWorldView->OnFlowZapChangeFileOpen(m_PreferencesControl->m_PreviousFileName);
  			ParseFile (file);
  			m_CurrentFileName = m_PreferencesControl->m_PreviousFileName;
  			SetWindowText ("Zap - [" + m_CurrentFileName + "]");
  			file->Close();
  			delete file;
  			file = NULL;
  
  			m_NeedToSaveAs     = FALSE;
  			m_ChangeMadeInFile = FALSE;
  			m_FileIsOpen = TRUE;
  */
  		} else {
  		/*Annoying Message
  			CString message = "The previously opened file: " +  m_CurrentFileName  + " \n" +
  				              "could not be found.  An empty file will be opened instead\n"; 
  			AfxMessageBox (message, MB_ICONEXCLAMATION, 0);
  		*/
  			m_InstructionMidPoint = InitInstructionList();
  		}
  
  		/*
  		** Make the last instruction (stop) not movable
  		*/
  		CInstruction * in = (CInstruction *) m_instructions.GetTail();
  		in->MakeMoveable(FALSE);
  		
  
  		
  	}
  
  	// m_InstructionMidPoint is used for placing the arrow
  	else m_InstructionMidPoint = InitInstructionList();
  
  	/*
  	** Determine the placement of the zones and child windows
  	*/
  		
  	UpdateZonePlacements();
  
  //	// Try and remember why this line is here
  //	m_NeedToSaveAs = FALSE;
  
  	return TRUE;
  }
  
  
  
  
  
  
  
  void CFlowZap::UpdateZonePlacements()
  {
  	int cue_width = m_Cue->GetWidth();
  		
  	CRect childRect, parentRect;
  	if (GetInstructionAtIndex(0) != NULL) GetInstructionAtIndex(0)->GetClientRect (&childRect);
  	GetWindowRect (&parentRect);
  	int total_window_width = parentRect.Width();
  	int loop_width = (total_window_width - 25 - childRect.Width() - cue_width) / 2;
  	m_xPosIndent = cue_width + loop_width;
  	m_InstructionMidPoint = m_xPosIndent + (childRect.Width()/2);
  	UpdateArrowPositions ((m_ExpandedView) ? 15 : 8);
  	
  
  	m_llp->UpdateRectangle (dZONE_NOCHANGE, dZONE_NOCHANGE, m_xPosIndent - 1, dZONE_NOCHANGE);
  	m_rlp->UpdateRectangle (m_xPosIndent + childRect.Width(), dZONE_NOCHANGE, 
  							m_xPosIndent + childRect.Width() + loop_width , dZONE_NOCHANGE);
  }
  
  
  
  
  
  
  
  
  
  void CFlowZap::InitPallette(){
  /************************************************************************************************
  * CREATED: 2/26/98 10:14AM
  *
  * LAST MODIFIED: 29 Nov 98 1:00pm
  *
  * PARAMETERS
  *	none
  * RETURN
  *	
  ************************************************************************************************
  * DESCRIPTION
  *	initializes the pallette window and sets its location
  * METHOD:
  *	also initializes the location of the border frame around the list of instructions
  ************************************************************************************************/
  
  	// create and show the pallette
  	m_Pallette = new CPallette(this);
  	m_Pallette->Create(IDD_PALLETTE, this);
  	m_Pallette->ShowWindow (SW_SHOW);
  
  	// determine where to place the pallette
  	CRect pRect, parentRect;
  	GetWindowRect (&parentRect);
  	
  	m_Pallette->GetWindowRect (&pRect);
  	m_Pallette->MoveWindow  ( (parentRect.Width() - pRect.Width() ) / 2, 0, pRect.Width(), pRect.Height(), TRUE);
  
  
  	m_Border.MoveWindow  (3, pRect.Height() + 5, pRect.Width(), pRect.Height(), TRUE);
  
  	// set the oragin of where to start the instructions
  	m_Origin = pRect.Height() + 15;
  	m_Origional_Origin = m_Origin;
  
  }
  
  
  
  
  
  
  int CFlowZap::InitInstructionList(){
  /************************************************************************************************
  * CREATED: 1/1/98 10:14AM
  *
  * LAST MODIFIED: 3/3/98 3:30 PM EST
  *
  * PARAMETERS
  *	none
  * RETURN
  *	
  ************************************************************************************************
  * DESCRIPTION
  *	inits a list of instuctions with just a start and a stop
  * METHOD:
  *
  ************************************************************************************************/
  	
  	// START								// STOP
  	CInstruction *start = makeStart(0);		CInstruction *stop  = makeStop(1);
  	start->ShowWindow(SW_SHOW);				stop->ShowWindow(SW_SHOW);
  	m_instructions.AddHead (start);			m_instructions.AddTail (stop);	
  	start->MakeMoveable (FALSE);			stop->MakeMoveable (FALSE);		
  	
  
  	CArrow *ar = new CArrow(CPoint (-1000, -1000) , this);
  	m_arrows.AddHead (ar);
  
  	// Set the Cue to the head of the list
  	m_Cue->Init (start);
  
  
  	CRect windRect;
  	start->GetClientRect (&windRect);
  	m_InstructionMidPoint = m_xPosIndent + (windRect.Width()/2);
  		
  
  	UpdateWindowPositions();
  
  //	OnFileManipulation (CMD_FILENEW, 0, "Untitled-1");
  
  	m_NeedToSaveAs = TRUE;
  	m_FileIsOpen = TRUE;
  	m_ChangeMadeInFile = FALSE;
  
  	m_CurrentFileName = "Untitled-1";
  	SetWindowText( "Zap - [" + m_CurrentFileName + "]");
  
  	CMenu* menu = GetMenu();
  	menu->EnableMenuItem(ID_FILE_CLOSE,	MF_BYCOMMAND | MF_ENABLED );
  	menu->EnableMenuItem(ID_FILE_SAVE,	MF_BYCOMMAND | MF_GRAYED );
  	menu->EnableMenuItem(ID_FILE_SAVEAS,MF_BYCOMMAND | MF_ENABLED );
  
  //			m_Tool->ShowWindow (SW_HIDE);
  //			m_Border.ShowWindow (SW_SHOW);
  
  
  
  	return m_InstructionMidPoint;
  }
  
  
  
  void CFlowZap::OnCancelMode() 
  {
  	CDialog::OnCancelMode();
  	
  	
  	
  }
  
  
  
  
  void CFlowZap::OnPaint() 
  {
  	try {
  		CPaintDC dc(this); // device context for painting
  		PaintArrows (&dc);
  		m_Cue->Paint(&dc);
  		m_llp->Paint(&dc);
  		m_rlp->Paint(&dc);
  	}
  
  	catch ( CResourceException ){
  		#ifdef _DEBUG
  		afxDump << "Caught a CResourceException in CFlowZap::OnPaint()\n";
  		#endif
  	}
  
  	
  	
  	
  	
  }
  
  void CFlowZap::OnCaptureChanged(CWnd *pWnd) 
  {
  	// TODO: Add your message handler code here
  	
  	CDialog::OnCaptureChanged(pWnd);
  }
  
  void CFlowZap::PaintArrows(CPaintDC *dc)
  {
  	CArrow * ar;
  	for (int i =0; i < m_arrows.GetCount(); i ++) {	
  		ar = (CArrow *)m_arrows.GetAt (m_arrows.FindIndex(i) );
  		if (ar == m_hArrow)	ar->PaintArrow (COLORREF(0xff    ), dc); // red
  		else				ar->PaintArrow (COLORREF(0x000000), dc); // black
  	}
  
  }
  
  
  
  void CFlowZap::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
  {
  	if (m_SelectedInstruction != NULL) RemoveInstruction(m_SelectedInstruction);
  	m_SelectedInstruction = NULL;
  	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
  }
  
  void CFlowZap::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
  {
  	if (m_SelectedInstruction != NULL) RemoveInstruction(m_SelectedInstruction);
  	m_SelectedInstruction = NULL;
  	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
  }
  
  
  
  
  
  
  //MENU COMMANDS//
  
  void CFlowZap::OnFileNew() 
  {
  	int reply = CloseFile(); // Closes the currently open file
  	if (reply == IDCANCEL) return;
  	InitInstructionList(); // contains OnFileManipulation (CMD_FILENEW, 0, "Untitled-1");
  #ifdef ROBOT_WORLD_PROJECT
  	m_RobotWorldView->OnFlowZapChangeFileOpen("");
  #endif
  }
  
  void CFlowZap::OnFileOpen() 
  {
  	CFileDialog fd(TRUE, "zap", NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR , "Zap Files (*.zap)|*.zap||", this);
  
  	CString DefaultPath = GetDefaultZapDirectory();
  	fd.m_ofn.lpstrInitialDir = DefaultPath;
  	int response = fd.DoModal();
  
  	if (response == IDCANCEL) {
  //		OnFileManipulation (CMD_FILEOPEN, IDCANCEL, "");
  	} else {
  //		CloseFile();
  		if (OpenFile(fd.GetPathName())) m_RobotWorldView->OnFlowZapChangeFileOpen(fd.GetFileName());
  //		OnFileManipulation (CMD_FILEOPEN, IDOK, fd.GetFileName());
  	}
  
  //	reply = OpenFile(); // contains OnFileManipulation (CMD_FILEOPEN, reply, m_CurrentFileName);
  	
  }
  
  bool CFlowZap::OpenFile(CString FileName)
  {
  	CStdioFile *siof = OpenFileForReading (FileName);
  	if (siof != NULL) {
  		if (m_FileIsOpen) {
  			int reply = CloseFile();         // if a file is open, close (prompts for save)
  			if (reply == IDCANCEL) {
  				siof->Close();
  				delete siof;
  				return false;
  			}
  		}
  		m_CurrentFileName = FileName;
  		ParseFile (siof);
  		siof->Close();
  		delete siof;
  		siof = NULL;
  
  //		OnFileManipulation (CMD_FILEOPEN, IDOK, m_CurrentFileName);
  
  		CMenu *menu = GetMenu();
  		menu->EnableMenuItem(ID_FILE_SAVE,	MF_BYCOMMAND | MF_GRAYED );
  
  		m_NeedToSaveAs     = FALSE;
  		m_ChangeMadeInFile = FALSE;
  
  		SetWindowText( "Zap - [" + m_CurrentFileName + "]");
  
  		m_FileIsOpen = TRUE;
  
  		m_PreferencesControl->m_PreviousFileName = m_CurrentFileName;
  
  
  		menu->EnableMenuItem(ID_FILE_CLOSE,	MF_BYCOMMAND | MF_ENABLED );
  		menu->EnableMenuItem(ID_FILE_SAVEAS,MF_BYCOMMAND | MF_ENABLED );
  
  		m_Tool->ShowWindow  (SW_HIDE);
  
  
  		UpdateWindowPositions();
  		return true;
  	}
  	return false;	
  }
  
  
  
  void CFlowZap::ParseFile(CStdioFile * f)
  {
  	CString line, pointerLocation, viewType, futureUse;
  	CStringList words;
  	f->ReadString (futureUse);
  	f->ReadString (futureUse);
  	f->ReadString (futureUse);
  	f->ReadString (futureUse);
  	f->ReadString(viewType);
  	f->ReadString(pointerLocation);
  	int pointer = atoi (pointerLocation);	
  	int j = 1;
  	int index = 0;
  	while (f->ReadString(line) ) {
  		Split (" ", line, words);
  		MakeNewInstructionFromFile(&words, index);
  		index ++;
  	}
  	if (!m_arrows.IsEmpty()) {
  		delete (m_arrows.RemoveTail());
  	}
  
  	//Configure all the branch loops
  	CInstruction* in = NULL;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CBranch ) ) ){
  			CBranch* cb = (CBranch*)in;
  			cb->m_Loop = m_rlp->AddLoop(cb, GetInstructionAtIndex (cb->m_tmpPtr) );
  		}
  	}
  	in = NULL;
  	
  	m_Cue->SetCueToInstruction (GetInstructionAtIndex(pointer) );
  	if (viewType == "CONDENSED VIEW") MakeViewExpanded (FALSE);
  	else MakeViewExpanded (TRUE);
  }
  
  CInstruction * CFlowZap::MakeBranchFromFile(CStringList * words){
  
  	/*
  	** FILE LOOKS LIKE
  	** If not nudged within 15 sec. then goto step 1 TRUE
  	** OR
  	** If nudged within 5 min. then goto step 1 TRUE
  	*/
  
  
  	CInstruction * in = makeBranch(0);
  	BOOL enabled = StringToBool (words->GetAt (words->FindIndex(words->GetCount() - 1) ) );
  	in->Enable (enabled);
  
  	CBranch *bp = (CBranch *) in;
  	CString timeLength, timeFrame, where;
  	CString condition = words->GetAt (words->FindIndex(1) );
  	float TriggerLevel;
  	if (condition == "not") {
  		timeLength = words->GetAt (words->FindIndex(4) );
  		timeFrame = words->GetAt (words->FindIndex(5) );
  		where = words->GetAt (words->FindIndex(9) );
  		bp->Configure (bcNotNudged, timeLength, timeFrame, atoi(where) );		
  	} else if (condition == "nudged") {
  		timeLength = words->GetAt (words->FindIndex(3) );
  		timeFrame = words->GetAt (words->FindIndex(4) );
  		where = words->GetAt (words->FindIndex(8) );
  		bp->Configure (bcNudged, timeLength, timeFrame, atoi(where) );
  	} else if (condition == "input") {
  		condition = words->GetAt(words->FindIndex(3));
  		if (condition == "high") {
  			timeLength = words->GetAt (words->FindIndex(5) );
  			timeFrame = words->GetAt (words->FindIndex(6) );
  			where = words->GetAt (words->FindIndex(10) );
  			bp->Configure (bcInputHigh, timeLength, timeFrame, atoi(where) );
  		} else if (condition == "low") {
  			timeLength = words->GetAt (words->FindIndex(5) );
  			timeFrame = words->GetAt (words->FindIndex(6) );
  			where = words->GetAt (words->FindIndex(10) );
  			bp->Configure (bcInputLow, timeLength, timeFrame, atoi(where) );
  		} else if (condition == ">") {
  			TriggerLevel = (float)atof(words->GetAt(words->FindIndex(4)));
  			timeLength = words->GetAt(words->FindIndex(7));
  			timeFrame = words->GetAt(words->FindIndex(8));
  			where = words->GetAt (words->FindIndex(12) );
  			bp->Configure (bcInputGreater, timeLength, timeFrame, atoi(where), TriggerLevel);
  		} else if (condition == "<") {
  			TriggerLevel = (float)atof(words->GetAt(words->FindIndex(4)));
  			timeLength = words->GetAt(words->FindIndex(7));
  			timeFrame = words->GetAt(words->FindIndex(8));
  			where = words->GetAt (words->FindIndex(12) );
  			bp->Configure (bcInputLess, timeLength, timeFrame, atoi(where), TriggerLevel);
  		} else if (words->GetAt(words->FindIndex(2)) == "toggles") {
  			timeLength = words->GetAt (words->FindIndex(4) );
  			timeFrame = words->GetAt (words->FindIndex(5) );
  			where = words->GetAt (words->FindIndex(9) );
  			bp->Configure (bcInputChanges, timeLength, timeFrame, atoi(where) );
  		} else {
  			delete in;
  			in = NULL;
  			ASSERT(FALSE);
  		}
  	}
  	return in;
  }
  
  
  
  CInstruction* CFlowZap::MakeGotoFromFile(CStringList * words)
  {
  	CString labelname = "";
  	CInstruction * in = makeGoto(0);
  
  	BOOL enabled = StringToBool (words->GetAt (words->FindIndex(words->GetCount() - 1) ) );
  	in->Enable (enabled);
  	// extract the name of the label:
  	CGoto* cg = (CGoto*) in;
  	for (int i = 1; i < words->GetCount() -2; i++){
  		labelname += words->GetAt(words->FindIndex(i) ) + " ";
  	}
  	labelname.TrimRight(); // trims the trailing white space from the name
  	CString sp = words->GetAt(words->FindIndex(words->GetCount() - 2) );
  	int speed = atoi(sp);
  	cg->ConfigureFromFile(labelname, speed);
  	return in;
  }
  
  CInstruction* CFlowZap::MakeExternalOutputFromFile(CStringList *words)
  {
  	CExternalOutput * in = (CExternalOutput*)makeExternalOutput(0);
  	BOOL enabled = StringToBool(words->GetAt(words->FindIndex(words->GetCount() - 1) ) );
  	in->Enable(enabled);
  	CString On = words->GetAt(words->FindIndex(words->GetCount() - 2) );
  	in->SetOutputOn(On == "On");
  	return in;
  }
  
  
  CInstruction* CFlowZap::MakeRepeatFromFile(CStringList * words, int index)
  {
  
  	CInstruction * in = makeRepeat(0);
  	BOOL enabled = StringToBool (words->GetAt (words->FindIndex(words->GetCount() - 1) ) );
  	in->Enable (enabled);
  
  	CRepeat *rp = (CRepeat *) in;
  	CString numRepeats = words->GetAt (words->FindIndex(4) ); // This word is the number back to look for
  	CString numBack    = words->GetAt (words->FindIndex(2) );    // This word is the number back to look for
  	rp->ConfigureFromFile (numBack, numRepeats);
  
  	int pointingTo = index - atoi(numBack);
  	CInstruction* ptr = NULL;
  	if (pointingTo < 0) ptr = in;
  	else if (pointingTo == index) ptr = in;
  	else  ptr = GetInstructionAtIndex (pointingTo);
  	rp->m_Loop = m_llp->AddLoop(in, ptr );
  	ptr->m_LoopStart = rp;
  
  	return in;
  }
  
  CInstruction * CFlowZap::MakeWaitFromFile(CStringList * words)
  {
  	CInstruction *in = makeWait(0);
  	BOOL enabled = StringToBool (words->GetAt (words->FindIndex(words->GetCount() - 1) ) );
  	in->Enable (enabled);
  
  	CWait *wp = (CWait *) in;
  	CString condition = words->GetAt (words->FindIndex(2) ); // This word is the condition
  
  	if (condition == "nudged") {
  		wp->ConfigureFromFile (wcWaitNudge, "", "");
  	} else {
  		condition = words->GetAt (words->FindIndex(1) );    // This word is the number back to look for
  		CString first  = words->GetAt (words->FindIndex(2) );
  		CString second = words->GetAt (words->FindIndex(3) );
  		if (condition == "until") {
  			if (first == "input") {
  				if (second == "is") {
  					second = words->GetAt(words->FindIndex(4));
  					if (second == "high") {
  						wp->ConfigureFromFile(wcWaitUntilInputIsHigh, first, second);
  					} else if (second == "low") {
  						wp->ConfigureFromFile(wcWaitUntilInputIsLow, first, second);
  					} else if (second == ">") {
  						first = words->GetAt(words->FindIndex(5));
  						wp->ConfigureFromFile(wcWaitUntilInputGreater, first, second);
  					} else if (second == "<") {
  						first = words->GetAt(words->FindIndex(5));
  						wp->ConfigureFromFile(wcWaitUntilInputLess, first, second);
  					} else {
  						ASSERT(FALSE);
  					}
  				} else if (second == "toggles") {
  					wp->ConfigureFromFile(wcWaitUntilInputChanges, first, second);
  				}
  			} else {
  				wp->ConfigureFromFile (wcWaitUntil, first, second);
  			}
  		} else if (condition == "for") {
  			wp->ConfigureFromFile (wcWaitFor, first, second);
  		}
  	}
  
  	return in;
  }
  
  CInstruction * CFlowZap::MakeVacuumFromFile(CStringList * words)
  {
  	CString patchname = "";
  	CInstruction * in = makeVac(0);
  	BOOL enabled = StringToBool (words->GetAt (words->FindIndex(words->GetCount() - 1) ) );
  	in->Enable(enabled);
  
  	// extract the name of the patch:
  	CVacuum* cv = (CVacuum*) in;
  	for (int i = 1; i < words->GetCount() -1; i++){
  		patchname += words->GetAt(words->FindIndex(i) ) + " ";
  	}
  	patchname.TrimRight(); // trims the trailing white space from the name
  	cv->ConfigureFromFile(patchname);
  
  
  	return in;
  	
  }
  
  CInstruction * CFlowZap::MakeCalibrateFromFile(CStringList * words)
  {
  	CString zonename = "";
  	CInstruction *in = makeCal(0);
  	BOOL enabled = StringToBool (words->GetAt (words->FindIndex(words->GetCount() - 1) ) );
  	in->Enable(enabled);
  
  	// extract the name of the zone:
  	CCalibrate* cc = (CCalibrate*) in;
  	for (int i = 2; i < words->GetCount() -1; i++){
  		zonename += words->GetAt(words->FindIndex(i) ) + " ";
  	}
  	zonename.TrimRight(); // trims the trailing white space from the name
  	cc->ConfigureFromFile(zonename);
  
  	return in;
  }
  
  CInstruction * CFlowZap::MakeMotionFromFile(CStringList * words)
  {
  	CString testRotate, testMove, degrees, direction, distance, units;
  	CInstruction *in = makeMotion(0);
  	BOOL enabled = StringToBool (words->GetAt (words->FindIndex(words->GetCount() - 1) ) );
  	in->Enable(enabled);
  	
  
  
  	CMotion* cm = (CMotion*) in;
  	testRotate = words->GetAt (words->FindIndex(1) );
  	testMove   = words->GetAt (words->FindIndex(2) );
  	degrees   = words->GetAt (words->FindIndex(3) );
  	direction   = words->GetAt (words->FindIndex(4) );
  	distance   = words->GetAt (words->FindIndex(5) );
  	units   = words->GetAt (words->FindIndex(6) );
  
  	cm->ConfigureFromFile ( StringToBool(testRotate), StringToBool(testMove), degrees, direction, distance, units);
  	cm->Enable(enabled);
  	return in;
  
  }
  
  
  
  
  void CFlowZap::MakeNewInstructionFromFile(CStringList * words, int index)
  {
  	BOOL enabled = TRUE;
  	CInstruction* in = NULL;
  	CString head = words->GetHead();
  
  	
  	if (head == "Stop")  {
  		in = makeStop(0);
  		enabled = StringToBool (words->GetAt (words->FindIndex(words->GetCount() - 1) ) );
  		in->Enable (enabled);
  	}
  
  	else if (head == "Start")		in = makeStart(0);
  	else if (head == "Goto")		in = MakeGotoFromFile(words);
  	else if (head == "Repeat")		in = MakeRepeatFromFile(words, index);
  	else if (head == "If")			in = MakeBranchFromFile(words);
  	else if (head == "Wait")		in = MakeWaitFromFile(words);
  	else if (head == "Vacuum")		in = MakeVacuumFromFile(words);
  	else if (head == "Calibrate")	in = MakeCalibrateFromFile(words);
  	else if (head == "Motion:")		in = MakeMotionFromFile(words);
  	else if (head == "External")	in = MakeExternalOutputFromFile(words);
  	else if (head == "Turn") in = MakeExternalOutputFromFile(words);
  
  	else if (head == "Sing") {
  		in = makeBeep(0);
  		enabled = StringToBool (words->GetAt (words->FindIndex(words->GetCount() - 1) ) );
  		in->Enable (enabled);
  		CBeep *bp = (CBeep *) in;
  		CString index = words->GetAt (words->FindIndex(2) ); // This word is the condition
  		CString length1 = words->GetAt (words->FindIndex(4) );
  		CString length2 = words->GetAt (words->FindIndex(5) );
  		bp->Configure (atoi (index), length1 + " " + length2);
  	}
  
  	else if (head == "Beep") {
  		in = makeBeep(0);
  		enabled = StringToBool (words->GetAt (words->FindIndex(words->GetCount() - 1) ) );
  		in->Enable (enabled);
  		CBeep *bp = (CBeep *) in;
  		CString index = words->GetAt (words->FindIndex(1) ); // This word is the condition
  		CString length1 = words->GetAt (words->FindIndex(3) );
  		CString length2 = words->GetAt (words->FindIndex(4) );
  		bp->Configure (atoi (index), length1 + " " + length2);
  	}
  	
  
  	else if (head == "Execute") {	//Version 1,1
  		in = makeSub(0);
  		CSub *cs = (CSub *) in;
  		CString filename = "";
  		for (int i = 2; i < words->GetCount() -1; i++){
  			filename += words->GetAt(words->FindIndex(i) ) + " ";
  		}
  		filename.TrimRight(); // trims the trailing white space from the name of the file
  		cs->Configure (filename);
  		
  
  		enabled = StringToBool (words->GetAt (words->FindIndex(words->GetCount() - 1) ) );
  		in->Enable (enabled);
  
  	}
  
  	else if (head == "Run") {	//Version 1,2 and >
  		in = makeSub(0);
  		CSub *cs = (CSub *) in;
  		CString filename = "";
  		for (int i = 1; i < words->GetCount() -1; i++){
  			filename += words->GetAt(words->FindIndex(i) ) + " ";
  		}
  		filename.TrimRight(); // trims the trailing white space from the name of the file
  		cs->Configure (filename);
  		
  
  		enabled = StringToBool (words->GetAt (words->FindIndex(words->GetCount() - 1) ) );
  		in->Enable (enabled);
  
  	}
  
  	else if (head == "Follow") {
  		in = MakePathFollowFromFile(words);
  	}
  
  	else {
  		in = NULL; // else do nothing, there is something wrong with this line in the file, so just ignore it.
  	}
  
  
  	if (in != NULL) {
  		m_instructions.AddTail (in);
  		in->ShowWindow(SW_SHOW);
  		AddArrow();
  	}
  	
  
  
  	
  
  }
  
  void CFlowZap::OnFileClose() 
  {
  	int reply = CloseFile(); // contains OnFileManipulation(CMD_FILECLOSE, 0, " ");  // " " is new filename
  	if (reply == IDCANCEL) return;
  	m_RobotWorldView->OnFlowZapChangeFileOpen("");
  	m_Origin = m_Origional_Origin;
  
  }
  
  
  
  int CFlowZap::CloseFile()
  {
  
  	int reply = IDNO; // set to default
  
  	// Prompt the user if they would like to save changes in the file if changes have been made.  
  	if (m_ChangeMadeInFile) {
  		
  		reply = AfxMessageBox ("You have made changes to " + m_CurrentFileName +"\nDo you wish to save your changes?", 
  			                    MB_YESNOCANCEL | MB_ICONEXCLAMATION, 0);
  
  		// if user cancels the save, do not close the file
  		if (reply == IDCANCEL)  return reply;
  
  		// if the reply is yes, then save the file
  		if (reply == IDYES) SaveFile();
  
  	}
  	// change the values of the file variables
  //	OnFileManipulation(CMD_FILECLOSE, reply, " "); 
  
  	m_NeedToSaveAs = FALSE;
  	m_FileIsOpen = FALSE;
  	m_ChangeMadeInFile = FALSE;
  	m_CurrentFileName = "";
  	SetWindowText( "Zap - [" + m_CurrentFileName + "]");
  
  
  	CMenu *menu = GetMenu();
  	menu->EnableMenuItem(ID_FILE_CLOSE,	MF_BYCOMMAND | MF_GRAYED );
  	menu->EnableMenuItem(ID_FILE_SAVE,	MF_BYCOMMAND | MF_GRAYED );
  	menu->EnableMenuItem(ID_FILE_SAVEAS,MF_BYCOMMAND | MF_GRAYED );
  //	m_Tool->ShowWindow (SW_SHOW);
  
  
  
  
  	UpdateWindowPositions();
  	
  	EmptyInstructionList();
  	EmptyArrowList();
  	m_llp->EmptyList();
  	m_rlp->EmptyList();
  	m_Cue->Reset();
  	
  	
  	return reply;
  }
  
  void CFlowZap::OnFileSaveas() {
  	SaveAs();
  }
  
  	
  int CFlowZap::SaveAs()
  {
  	/*
  	CFileDialog( BOOL bOpenFileDialog, [TRUE = OPEN, FALSE = SAVEAS]
  				 LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL, 
  				 DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
  				 LPCTSTR lpszFilter = NULL,  CWnd* pParentWnd = NULL );
  
      */
  	CFileDialog fd(FALSE, "zap", m_CurrentFileName, OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT , "Zap Files (*.zap)|*.zap||",this);
  	
  	CString DefaultPath = GetDefaultZapDirectory();
  	fd.m_ofn.lpstrInitialDir = DefaultPath;
  
  	int response = fd.DoModal();
  
  	// clean up
  //	delete fd;
  //	fd = NULL;
  
  //	OnFileManipulation (CMD_FILESAVEAS, response, m_CurrentFileName);
  	if (response == IDOK)	{
  		m_CurrentFileName = fd.GetPathName();
  		m_NeedToSaveAs = FALSE;
  		SaveFile();
  		m_FileIsOpen = TRUE;
  		m_ChangeMadeInFile = FALSE;
  
  //		m_CurrentFileName = filename;
  		SetWindowText( "Zap - [" + m_CurrentFileName + "]");
  		m_PreferencesControl->m_PreviousFileName = m_CurrentFileName;
  		m_RobotWorldView->OnFlowZapChangeFileOpen(fd.GetFileName());
  	}
  //	else				m_NeedToSaveAs = TRUE;
  
  
  	return response;
  }
  
  
  void CFlowZap::OnFileSave() 
  {
  	SaveFile();	
  }
  
  int CFlowZap::SaveFile()
  {	int reply = IDOK;
  	if (m_NeedToSaveAs)   reply = SaveAs();
  
  	if (reply == IDOK) {
  
  		CStdioFile *siof = OpenFileForWriting (m_CurrentFileName);
  		WriteInstructionsToFile (siof);
  		siof->Close();
  		delete siof;
  		siof = NULL;
  
  		m_NeedToSaveAs = FALSE;
  		m_FileIsOpen = TRUE;
  		m_ChangeMadeInFile = FALSE;
  		CMenu* menu = GetMenu();
  		menu->EnableMenuItem(ID_FILE_SAVE,	MF_BYCOMMAND | MF_GRAYED );
  	}
  
  //	OnFileManipulation (CMD_FILESAVE, reply, m_CurrentFileName);
  
  	return reply;
  }
  
  
  void CFlowZap::WriteInstructionsToFile(CStdioFile * f)
  {
  	ASSERT(f != NULL);
  	try {
  		f->WriteString ("FLOWZAP Version 1.0\n");
  		f->WriteString ("Copyright 1998 Probotics Inc.");
  		f->WriteString ("*** SAVE FOR FUTURE USE ***\n");
  		f->WriteString ("*** SAVE FOR FUTURE USE ***\n");
  		f->WriteString ("*** SAVE FOR FUTURE USE ***\n");
  		if (m_ExpandedView) f->WriteString ("EXPANDED VIEW\n");
  		else f->WriteString ("CONDENSED VIEW\n");
  		f->WriteString (m_Cue->ToString() + "\n" );
  	}
  	catch (CFileException fe) {
  	}
  	
  	CInstruction *in;
  	for (int i =0; i < GetInstructionsSize(); i ++ ) {
  		in = GetInstructionAtIndex(i);
  		try {
  			f->WriteString (in->ToFile() + "\n" );
  		}
  		catch (CFileException fe) {
  		}
  	}
  
  }
  
  void CFlowZap::OnClose() 
  {
  	CloseFlowZap();
  	CDialog::OnClose();
  }
  
  
  void CFlowZap::OnFileExit() 
  {
  	CloseFlowZap();	
  }
  
  
  
  void CFlowZap::CloseFlowZap()
  {
  	if (m_FileIsOpen) {
  		int reply = CloseFile(); // contains OnFileManipulation(CMD_FILECLOSE, 0, " ");  // " " is new filename
  		if (reply == IDCANCEL) return;
  	}
  	InitInstructionList(); // contains OnFileManipulation (CMD_FILENEW, 0, "Untitled-1");
  	m_RobotWorldView->OnFlowZapChangeFileOpen("");
  //	m_Origin = m_Origional_Origin;
  
  
  //	m_RobotWorldView->OnFlowZapChangeFileOpen("");
  //	if (m_ExecuteControl != NULL) {
  //		m_ExecuteControl->ShowWindow(SW_HIDE);
  //	}
  //	if (m_FileIsOpen) {
  //		int reply = CloseFile();
  //		if (reply == IDCANCEL) return;
  //	}
  //	OnFileManipulation (CMD_FILEEXIT, 0, " ");
  	m_PreferencesControl->WritePrefs();
  //	m_NeedToSaveAs = FALSE;
  //	m_FileIsOpen = FALSE;
  //	m_ChangeMadeInFile = FALSE;
  
  	ShowWindow (SW_HIDE);
  
  	if (m_Playing) {
  		m_OrigionalPointerLocation = GetInstructionAtIndex(0);
  		m_Cue->SetCueToInstruction(m_OrigionalPointerLocation);
  		OnPlayStop();
  	}
  }
  
  
  
  
  
  
  void CFlowZap::OnEditPreferences() {
  /************************************************************************************************
  * CREATED: 1/22/98 10:54AM
  *
  * LAST MODIFIED: 1/22/98 11:25AM
  *
  * PARAMETERS
  *	point	the point of the user click
  * RETURN
  *	
  ************************************************************************************************
  * DESCRIPTION
  *	opens the preferences window (which is system modal)
  * METHOD:
  *
  ************************************************************************************************/
  	if (m_PreferencesControl->GetSafeHwnd() == NULL) {
  		m_PreferencesControl->Create( IDD_PREFERENCES, this);
  		m_PreferencesControl->ShowWindow(SW_SHOW);
  		m_PreferencesControl->EnableWindow();
  		m_PreferencesControl->SetActiveWindow();
  	}
  	else {
  		m_PreferencesControl->ShowWindow(SW_SHOW);
  	}
  	
  }
  
  
  void CFlowZap::OnViewCondensed() 
  {
  	MakeViewExpanded (FALSE);
  }
  
  
  void CFlowZap::OnUpdateViewCondensed(CCmdUI* pCmdUI) 
  {
  	MakeViewExpanded (FALSE);		
  }
  
  
  void CFlowZap::OnViewExpanded() 
  {
  	MakeViewExpanded (TRUE);	
  }
  
  
  
  
  
  
  
  
  
  
  
  void CFlowZap::MakeViewExpanded(BOOL expanded)
  {
  	CInstruction *in;
  	if (expanded) {
  		CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  		SoundPath += "/sounds/expand.wav";
  		if (!m_ExpandedView && IsSoundOn() ) PlaySound (SoundPath, NULL, SND_FILENAME | SND_ASYNC );
  		for (int i = 0; i < m_instructions.GetCount(); i ++) {	
  			in = GetInstructionAtIndex(i);
  			in->MakeExpanded();
  		}
  	}
  	else {
  		CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  		SoundPath += "/sounds/condense.wav";
  		if (m_ExpandedView && IsSoundOn() ) PlaySound (SoundPath, NULL, SND_FILENAME | SND_ASYNC ); 
  		for (int i = 0; i < m_instructions.GetCount(); i ++) {	
  			in = GetInstructionAtIndex(i);
  			in->MakeCondensed();
  		}
  
  	}
  	m_ExpandedView = expanded;
  	ToggleMenuViewChecked(expanded);
  	m_Origin = m_Origional_Origin; // reset the Y oragin to the origional position
  
  
  	/*
  	**  reset the thumb to the top of the scroll bar
  	*/
  	int min, max;
  	GetScrollRange (SB_VERT, &min, &max);
  	SetScrollPos(SB_VERT, min, TRUE); 
  	m_OldVScrollPosition=min;
  
  
  	UpdateWindowPositions();
  }
  
  
  
  
  
  
  
  
  
  
  
  void CFlowZap::ToggleMenuViewChecked(BOOL b)
  {
  	CMenu *menu = GetMenu();
  	if (b) {
  		menu->CheckMenuItem(ID_VIEW_EXPANDED ,MF_BYCOMMAND | MF_CHECKED );
  		menu->CheckMenuItem(ID_VIEW_CONDENSED ,MF_BYCOMMAND | MF_UNCHECKED );
  	}
  
  	else {
  		menu->CheckMenuItem(ID_VIEW_EXPANDED ,MF_BYCOMMAND | MF_UNCHECKED );
  		menu->CheckMenuItem(ID_VIEW_CONDENSED ,MF_BYCOMMAND | MF_CHECKED );
  	}
  	DrawMenuBar();
  
  }
  
  
  
  
  
  void CFlowZap::OnInstructionDelete() 
  {
  	if (m_SelectedInstruction != NULL) RemoveInstruction(m_SelectedInstruction);
  	MakeMenuInstructionActive(FALSE);
  	m_SelectedInstruction = NULL;
  	
  }
  
  
  
  CInstruction* CFlowZap::RemoveInstruction(CInstruction *in)
  {
  	if (in == NULL) return NULL;
  	CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  	SoundPath += "/sounds/removeInstruction.wav";
  	if (IsSoundOn() ) PlaySound (SoundPath, NULL, SND_FILENAME | SND_ASYNC ); 
  
  	m_Cue->HandleRemoveInstruction(in);
  
  	RemoveInstructionFromList(in);
  	RemoveArrowFromList();
  
  	
  
  	if (in == m_SelectedInstruction) m_SelectedInstruction = NULL;
  
  	if ( in->IsKindOf( RUNTIME_CLASS( CRepeat ) ) ){
  		m_llp->RemoveLoop(in);
  	}
  
  	else if ( in->IsKindOf( RUNTIME_CLASS( CBranch ) ) ){
  		m_rlp->RemoveLoop(in);
  	}
  
  
  	// check each of the repeat  + branch loops, if they point to this item, then point them to themselves.
  	CInstruction * ptr;
  	for (int i = 0; i < GetInstructionsSize(); i++) {
  		ptr = GetInstructionAtIndex(i);
  		if (ptr->IsKindOf( RUNTIME_CLASS( CRepeat ) ) ){
  			if (ptr->m_Loop->m_End == in) ptr->m_Loop->m_End = ptr; //point it to itself
  		}
  		
  		else if (ptr->IsKindOf( RUNTIME_CLASS( CBranch ) ) ){
  			if (ptr->m_Loop->m_End == in) ptr->m_Loop->m_End = ptr; //point it to itself
  		}
  	}
  
  	delete (in);
  	in = NULL;
  	UpdateWindowPositions();
  	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
  	UpdateRepeats();
  	ChangeMadeInFile();
  	return NULL;
  
  }
  
  void CFlowZap::RemoveInstructionFromList(CInstruction* in)
  {
  	POSITION p = m_instructions.Find(in);
  	m_instructions.RemoveAt (p);
  }
  
  
  
  
  
  
  void CFlowZap::RemoveArrowFromList(){
  /************************************************************************************************
  * CREATED: 3/02/98 10:14AM
  *
  * LAST MODIFIED: 3/02/98 10:14 AM EST
  *
  * PARAMETERS
  *	none
  * RETURN
  *	void
  *
  ************************************************************************************************
  * DESCRIPTION
  *	removes the arrow from the head of the list
  * METHOD:
  *	none
  ************************************************************************************************/
  	CArrow * head = (CArrow*) m_arrows.RemoveHead();
  	delete head;
  	head = NULL;
  }
  
  
  
  
  
  CInstruction* CFlowZap::DeselectInstruction()
  {
  	CInstruction* in = NULL;
  	if (m_SelectedInstruction != NULL && m_SelectedInstruction->GetSafeHwnd() != NULL) {
  		in = m_SelectedInstruction;
  		m_SelectedInstruction->MakeSelected(FALSE);
  		if (!m_ExpandedView) m_SelectedInstruction->MakeCondensed();
  		m_SelectedInstruction->RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
  		m_SelectedInstruction = NULL;
  		if (!m_ExpandedView) UpdateWindowPositions();
  	}
  	MakeMenuInstructionActive(FALSE);
  	return in;
  }
  
  
  void CFlowZap::OnInstructionDisable() 
  {
  	if (m_SelectedInstruction != NULL) EnableInstruction(m_SelectedInstruction, FALSE);
  	
  }
  
  
  
  void CFlowZap::OnInstructionEnable() 
  {
  	if (m_SelectedInstruction != NULL) EnableInstruction(m_SelectedInstruction, TRUE);
  	
  }
  
  void CFlowZap::EnableInstruction(CInstruction* in, BOOL enable)
  {
  	if (in != NULL) {
  		ChangeMadeInFile();
  		in->Enable(enable);
  		SetEnableDisableMenu(enable);
  		if (in->IsKindOf( RUNTIME_CLASS( CRepeat ) ) ) RedrawWindow(m_llp->m_Loc, NULL, RDW_INVALIDATE | RDW_NOCHILDREN | RDW_ERASE);
  		if (in->IsKindOf( RUNTIME_CLASS( CBranch ) ) ) RedrawWindow(m_rlp->m_Loc, NULL, RDW_INVALIDATE | RDW_NOCHILDREN | RDW_ERASE); 
  	}
  //	RedrawWindow();
  }
  
  void CFlowZap::SetEnableDisableMenu(BOOL enabled)
  {
  	CMenu *menu = GetMenu();
  
  	if (enabled) {
  		//menu->EnableMenuItem(ID_INSTRUCTION_DISABLE, MF_BYCOMMAND | MF_ENABLED );
  		//menu->EnableMenuItem(ID_INSTRUCTION_ENABLE,  MF_BYCOMMAND | MF_GRAYED );
  		menu->CheckMenuItem (ID_INSTRUCTION_DISABLE, MF_BYCOMMAND | MF_UNCHECKED );
  		menu->CheckMenuItem (ID_INSTRUCTION_ENABLE,  MF_BYCOMMAND | MF_CHECKED );
  	}
  
  	else {
  		menu->CheckMenuItem (ID_INSTRUCTION_DISABLE, MF_BYCOMMAND | MF_CHECKED );
  		menu->CheckMenuItem (ID_INSTRUCTION_ENABLE,  MF_BYCOMMAND | MF_UNCHECKED );
  	}
  	DrawMenuBar();
  }
  
  
  
  
  
  
  
  void CFlowZap::OnInstructionSetCue() 
  {
  	if (m_SelectedInstruction == NULL) return;
  	m_Cue->SetCueToInstruction (m_SelectedInstruction);
  	RedrawWindow(m_Cue->m_Loc, NULL, RDW_ERASE | RDW_INVALIDATE);
  	
  	
  }
  
  
  
  
  
  void CFlowZap::MakeLookupTables()
  {
  	m_NoteConversion [0] = "low C for";		m_NoteConversion [1] = "low C# for";
  	m_NoteConversion [2] = "low D for";		m_NoteConversion [3] = "low D# for";
  	m_NoteConversion [4] = "low E for";		m_NoteConversion [5] = "low F for";
  	m_NoteConversion [6] = "low F# for";	m_NoteConversion [7] = "low G for";
  	m_NoteConversion [8] = "low G# for";	m_NoteConversion [9] = "low A for";
  	m_NoteConversion [10] = "low A# for";	m_NoteConversion [11] = "low B for";
  
  	m_NoteConversion [12] = "mid C for";	m_NoteConversion [13] = "mid C# for";
  	m_NoteConversion [14] = "mid D for";	m_NoteConversion [15] = "mid D# for";
  	m_NoteConversion [16] = "mid E for";	m_NoteConversion [17] = "mid F for";
  	m_NoteConversion [18] = "mid F# for";	m_NoteConversion [19] = "mid G for";
  	m_NoteConversion [20] = "mid G# for";	m_NoteConversion [21] = "mid A for";
  	m_NoteConversion [22] = "mid A# for";	m_NoteConversion [23] = "mid B for";
  
  	m_NoteConversion [24] = "high C for";	m_NoteConversion [25] = "high C# for";
  	m_NoteConversion [26] = "high D for";	m_NoteConversion [27] = "high D# for";
  	m_NoteConversion [28] = "high E for";	m_NoteConversion [29] = "high F for";
  	m_NoteConversion [30] = "high F# for";	m_NoteConversion [31] = "high G for";
  	m_NoteConversion [32] = "high G# for";	m_NoteConversion [33] = "high A for";
  	m_NoteConversion [34] = "high A# for";	m_NoteConversion [35] = "high B for";
  		
  	m_HzConversion [0] = 130.6;
  	for (int i = 1; i < 36 ; i++ ) {
  		m_HzConversion [i] = m_HzConversion[i-1] * 1.0595;
  	}
  }
  
  
  
  /*FUNCTIONS FOR RETURNING INFORMATION */
  
  
  /*
  returns if the current view type is expanded or condensed
  */
  BOOL CFlowZap::IsExpanded(){return m_ExpandedView;}
  
  
  
  /*
  gets the POSITION of an instruction in the list
  */
  POSITION CFlowZap::GetInstructionPosition(CInstruction * in){return m_instructions.Find(in);}
  
  
  
  /*
  returns the size of the list of instructions
  */
  int CFlowZap::GetInstructionsSize() {return m_instructions.GetCount(); }
  
  
  
  /*
  returns an instruction at an index
  */
  CInstruction* CFlowZap::GetInstructionAtIndex(int index)
  {
  	if (m_instructions.IsEmpty() ) return NULL;
  	if (index >= m_instructions.GetCount() ) return NULL;
  	return (CInstruction *)m_instructions.GetAt (m_instructions.FindIndex(index) );
  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  void CFlowZap::MakeMenuInstructionActive(BOOL active)
  {
    /* 
     * get a handle to the menubar, make it active or inactive accordingly, then redraw it
     */
  	if (m_hWnd) {
  		if (!m_IsSubroutineWindow) {
  			CMenu *menu = GetMenu();
  			if (active) menu->EnableMenuItem(3,MF_BYPOSITION | MF_ENABLED );
  			else        menu->EnableMenuItem(3,MF_BYPOSITION | MF_GRAYED );
  			DrawMenuBar();
  		}
  	}
  }
  
  
  
  // attempting to put images in menubar
  void CFlowZap::InitMenuBar()
  {
  	//CMenu *menu = GetMenu();
  	//CBitmap *test;
  	//test->LoadBitmap(MAKEINTRESOURCE(IDI_BRANCH));
  	//menu->SetMenuItemBitmaps(ID_EDIT_UNDO, MF_BYCOMMAND, test, test);
  }
  
  
  
  void CFlowZap::OnHelpAbout() 
  {
  /*	CStringList *words = new CStringList();
  	words->AddHead ("Bedroom");
  	words->AddHead ("Kitchen");
  	words->AddHead ("Dining Room");
  	words->AddHead ("Home");
  	words->AddHead ("Living Room");
  	SetLabelNames (words);
  	//delete words;
  	//words = NULL;
  */	
  }
  
  void CFlowZap::UpdateRepeats()
  {
  	CInstruction* ptr;
  	for (int i = 1; i < GetInstructionsSize(); i ++){
  		ptr = GetInstructionAtIndex(i);
  		if (ptr->IsKindOf( RUNTIME_CLASS( CRepeat ) ) ){
  			CRepeat *rp = (CRepeat*) ptr;
  			rp->UpdateNumSteps ();
  
  		}
  	}
  }
  
  void CFlowZap::MoveInstruction(CPoint point)
  {
     /*
  	* Determine the height of the arrows between the instructions
  	*/
  	int buffer;
  	if (m_ExpandedView) buffer = 25;
  	else buffer = 8;
  
  	SetCursor();
  	if (m_SelectedInstruction != NULL) {
  		CRect r;
  		m_SelectedInstruction->GetWindowRect(&r);
  		ScreenToClient(&r);
  		if (point.y < r.TopLeft().y - buffer) {  
  			CInstruction *in = GetInstructionAtPixel(point);
  			if (in != GetInstructionAtIndex(0) ) {
  				CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  				SoundPath += "/sounds/choice.wav";
  				if (IsSoundOn() )  PlaySound (SoundPath, NULL, SND_FILENAME | SND_ASYNC );
  				RemoveInstructionFromList(m_SelectedInstruction);
  				RemoveArrowFromList();
  				InsertInstructionBefore(in, m_SelectedInstruction);
  				ChangeMadeInFile();
  				UpdateRepeats();
  			}
  		}
  
  		else if (point.y > r.BottomRight().y + buffer) {
  			CInstruction *in = GetInstructionAtPixel(point);
  			if (in != GetInstructionAtIndex(GetInstructionsSize() -1) && in != NULL) {
  				CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  				SoundPath += "/sounds/choice.wav";
  				if (IsSoundOn() )  PlaySound (SoundPath, NULL, SND_FILENAME | SND_ASYNC );
  				RemoveInstructionFromList(m_SelectedInstruction);
  				RemoveArrowFromList();
  				InsertInstructionAfter(in, m_SelectedInstruction);
  				ChangeMadeInFile();
  				UpdateRepeats();
  			}
  		}
  		//RedrawWindow(NULL, NULL);	
  
  	}
  }
  
  
  
  
  
  void CFlowZap::EmptyInstructionList() 
  {
  	DeselectInstruction();
  	CInstruction* in;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {	
  		in = GetInstructionAtIndex (i);
  		if (in != NULL) {
  			delete in;
  			in = NULL;
  		}
  	}
  	m_instructions.RemoveAll();
  
  }
  
  
  
  void CFlowZap::EmptyArrowList() 
  {
  	CArrow *ar = NULL;
  	for (int i = 0; i < m_arrows.GetCount(); i ++) {	
  		ar = (CArrow *)m_arrows.GetAt (m_arrows.FindIndex(i) );
  		delete ar;
  		ar = NULL;
  	}
  	m_arrows.RemoveAll();
  }
  
  
  
  
  
  // HANDLE WHEN THE USER WANTS TO RESIZE THE WINDOW
  
  
  void CFlowZap::OnSizing(UINT fwSide, LPRECT pRect) 
  {
  	if (m_Pallette != NULL) {
  		if (m_Pallette->m_hWnd != NULL) {
  			CRect ClientRect;
  			GetClientRect(&ClientRect);
  			CRect PalletteRect;
  			m_Pallette->GetWindowRect(&PalletteRect);
  			ScreenToClient(&PalletteRect);
  			int margin = (ClientRect.Width() - PalletteRect.Width()) / 2;
  			if (margin < 5) {
  				margin = 5;
  			}
  			m_Pallette->MoveWindow(margin, PalletteRect.top, PalletteRect.Width(), PalletteRect.Height(), TRUE);
  		}
  	}
  	
  	UpdateZonePlacements();
  	UpdateInstructionPositions( (m_ExpandedView) ? 15 : 8);
  	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	//somehow determine if we need to adjust scrollbars
  	CDialog::OnSizing(fwSide, pRect);
  	//CRect r;
  	//GetWindowRect(&r);
  	//MoveWindow (r.TopLeft().x, r.TopLeft().y, m_OrigionalWindowWidth, r.Height() );
  	
  }
  
  
  
  
  BOOL CFlowZap::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
  {   
  	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;    UINT nID =pNMHDR->idFrom;
      if (pTTT->uFlags & TTF_IDISHWND)    {
          // idFrom is actually the HWND of the tool
          nID = ::GetDlgCtrlID((HWND)nID);        if(nID)        {
              pTTT->lpszText = MAKEINTRESOURCE(nID);
              pTTT->hinst = AfxGetResourceHandle();            return(TRUE);
         }    }    
  	return(FALSE);}
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  // THESE FUNCTIONS ARE USED FOR INTERACTION WITH ROBOT WORLD
  
  CInstruction * CFlowZap::Play()
  {
  	SwitchToExecuteMode (TRUE);
  	CInstruction* ptr =  m_Cue->GetInstructionAtPointer();
  	if (ptr != NULL) {
  		/*
  		* Save the location of the pointer before running anything so that we can put it
  		* back when we are done.
  		*/
  		m_OrigionalPointerLocation = ptr;
  		ptr = GetNextExecutableInstruction(ptr);
  		m_Cue->SetCueToInstruction(ptr);
  	}
  	return ptr;
  	
  }
  
  
  
  void CFlowZap::Stop()
  {
  	SwitchToExecuteMode (FALSE);
  	m_Cue->SetCueToInstruction(m_OrigionalPointerLocation);
  	if (m_SubProgramWindow != NULL) {
  		delete m_SubProgramWindow;
  		m_SubProgramWindow = NULL;
  	}
  	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
  }
  
  
  
  void CopyStringList(CStringList* To, CStringList* From) {
  	ASSERT(To != NULL);
  	ASSERT(From != NULL);
  	POSITION Current = From->GetHeadPosition();
  	while (Current != NULL) {
  		To->AddHead(From->GetNext(Current));
  	}
  	return;
  }
  
  
  CFlowZap* CFlowZap::OpenSubProgramWindow(CString filename)
  {
  	CFlowZap* wnd = new CFlowZap(m_RobotWorldView);
  	CStringList* Zones = new CStringList();
  	CopyStringList(Zones, m_CalibrationZones);
  	CStringList* Labels = new CStringList();
  	CopyStringList(Labels, m_LabelNames);
  	CStringList* LinePaths = new CStringList();
  	CopyStringList(LinePaths, m_LinePaths);
  	CStringList* Patches = new CStringList();
  	CopyStringList(Patches, m_VacuumPatches);
  	wnd->SetCalibrationZones(Zones);
  	wnd->SetLabelNames(Labels);
  	wnd->SetLinePaths(LinePaths);
  	wnd->SetVacuumPatches(Patches);
  
  
  	wnd->Create( IDD_FLOWZAP, this);
  	CRect r;
  	GetWindowRect (&r);
  	wnd->SetWindowText("FlowZap [" + filename + "]");
  	wnd->MoveWindow (r.TopLeft().x + 25, r.TopLeft().y + 25, r.Width(), r.Height(), TRUE);
  	
  	
  	wnd->CloseFile();  // Close the default file
  	CStdioFile *file = OpenFileForReading(filename);
  	if (file != NULL) {
  		wnd->ParseFile (file);
  		// file handle cleanup 
  		file->Close();
  		delete file;
  		file = NULL;
  
  		wnd->m_Tool->ShowWindow(SW_HIDE);	//Hide the Open, New, ... window
  		wnd->SwitchToExecuteMode(TRUE);
  		wnd->SetCueToStart();
  		wnd->SetIsSubroutineWindow();
  		wnd->ShowWindow(SW_SHOW);
  	} else {
  		AfxMessageBox("Subroutine file not found");
  		delete wnd;
  		wnd = NULL;
  	}
  	
  	return wnd;
  }
  
  
  
  
  
  
  CInstruction* CFlowZap::GetInstructionToExecute(BOOL test)
  {
  	CInstruction* ptr  =  m_Cue->GetInstructionAtPointer(); //by this time, ptr has already been executed
  	CInstruction *next = NULL;
  
  	if (ptr == NULL) {
  		return NULL;
  	} else if ( test && ptr->IsKindOf( RUNTIME_CLASS ( CBranch ) ) ){
  		CBranch* cb = (CBranch*)ptr;
  		next = cb->GetLoopDestination();
  	} else if (ptr->IsKindOf (RUNTIME_CLASS ( CSub ) ) ) {
  		CSub *cs = ((CSub*) ptr);
  		if (cs->m_NameOfFile != "CHOOSE FILE") {
  			if (m_SubProgramWindow == NULL) {
  				m_SubProgramWindow = OpenSubProgramWindow(cs->m_NameOfFile);
  			}
  			if (m_SubProgramWindow == NULL) return NULL;
  			CInstruction *cin = m_SubProgramWindow->GetInstructionToExecute(test);
  			if (cin == NULL) {
  				m_SubProgramWindow->ShowWindow(SW_HIDE);
  				delete m_SubProgramWindow;
  				m_SubProgramWindow = NULL;
  				return NULL;
  			} else if (cin->IsKindOf (RUNTIME_CLASS ( CStop ) ) ) {
  				next = GetNextInstructionInList(ptr);
  				m_SubProgramWindow->ShowWindow(SW_HIDE);
  				delete m_SubProgramWindow;
  				m_SubProgramWindow = NULL;
  			}
  			else return cin;
  			
  		}
  
  		else { // This is reached when an invalid file has been named or the file can not be found
  			next = GetNextInstructionInList(ptr);
  		}
  
  	}
  
  	else next = GetNextInstructionInList(ptr);
  
  
  	next = GetNextExecutableInstruction(next);
  	
  	
  	
  	m_Cue->SetCueToInstruction (next);
  	
  	return next;
  
  	
  }
  
  
  
  void CFlowZap::SwitchToExecuteMode(BOOL execute)
  {
  	if (execute) m_ProgramMode = pmExecute;
  	else m_ProgramMode = pmEdit;
  }
  
  
  
  /* RETURNS THE NEXT EXECUTABLE INSTRUCTION,  */
  /*note: it could be -in-, if -in- is executalbe*/
  CInstruction* CFlowZap::GetNextExecutableInstruction(CInstruction * in)
  {
  	
  	// if this instruction is disabled, get the next one in the list.
  	if (!in->m_Enabled) {
  		in = GetNextInstructionInList(in);
  		return GetNextExecutableInstruction(in);
  	}
  
  	m_Cue->SetCueToInstruction (in);
  	RedrawWindow(m_Cue->m_Loc, NULL, RDW_INVALIDATE | RDW_ERASE  | RDW_UPDATENOW);
  
  	if (in->IsKindOf( RUNTIME_CLASS ( CRepeat ) ) ){
  		CRepeat *rp = ((CRepeat*) in);
  		int numRepeats = rp->GetNumRepeatsLeft();
  		if (numRepeats > 0) in = rp->GetLoopDestination();
  		else  {
  			rp->Reset();
  			in = GetNextInstructionInList(in);
  			
  		}
  		in = GetNextExecutableInstruction(in);
  		RedrawWindow(m_llp->m_Loc, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_NOCHILDREN | RDW_UPDATENOW);
  	}
  
  
  
    /*
     * Just skip over the START and get the next instruction in the list 
     */
  	else if (in->IsKindOf (RUNTIME_CLASS ( CStart ) ) ){
  		in = GetNextInstructionInList(in);
  		in = GetNextExecutableInstruction(in);
  	}
  
  	return in; // Base step b/c in is not changed
  }
  
  
  
  
  
  
  
  CInstruction* CFlowZap::GetNextInstructionInList(CInstruction * in){
  /************************************************************************************************
  * CREATED: 1/20/98 10:27AM
  *
  * LAST MODIFIED: 2/26/98 10:43 AM EST
  *
  * PARAMETERS
  *	in		an instruction in the list
  * RETURN
  *	CInstruction *	the next instruction in the list
  *
  ************************************************************************************************
  * DESCRIPTION
  *	returns the next (in order) instruction in the list of instructions
  * METHOD:
  *	if instruction is at the end of the list, return self
  ************************************************************************************************/
  	int index = in->GetIndex();
  	if (index >= GetInstructionsSize() -1) in = in;
  	else in =  GetInstructionAtIndex(index + 1);
  	return in;
  }
  
  
  
  
  
  
  
  void CFlowZap::PurgeStringList(CStringList * list)
  {
  	if (list != NULL) list->RemoveAll();
  	delete list;
  	list = NULL;
  }
  
  
  
  
  
  
  
  void CFlowZap::SetLabelNames(CStringList* labels){
  /************************************************************************************************
  * CREATED: 1/20/98 10:27AM
  *
  * LAST MODIFIED: 2/24/98 2:13 PM
  *
  * PARAMETERS
  *	labels		a list of names of labels in the RobotWorld Program
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	adds one label to the list of labels on all goto instructions
  * METHOD:
  *
  ************************************************************************************************/
  	if (labels == NULL) labels = new CStringList;
  	PurgeStringList (m_LabelNames);
  	m_LabelNames = labels;
  	CInstruction *in = NULL;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CGoto ) ) ){
  			CGoto* cg = (CGoto*)in;
  			cg->InsertLabelNames(labels);
  		}
  	}
  }
  
  
  
  
  
  void CFlowZap::SetCalibrationZones(CStringList* zones){
  /************************************************************************************************
  * CREATED: 1/20/98 10:27AM
  *
  * LAST MODIFIED: 2/24/98 2:14 PM
  *
  * PARAMETERS
  *	zones		a list of names of calibration zones in the RobotWorld Program
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	adds one label to the list of labels on all goto instructions
  * METHOD:
  *
  ************************************************************************************************/
  	if (zones == NULL) zones = new CStringList;
  	PurgeStringList (m_CalibrationZones);
  	m_CalibrationZones = zones;
  	CInstruction *in = NULL;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CCalibrate ) ) ){
  			CCalibrate* cc = (CCalibrate*)in;
  			cc->InsertCalibrationZones(zones);
  		}
  	}
  }
  
  
  
  
  
  void CFlowZap::SetVacuumPatches(CStringList* patches){
  /************************************************************************************************
  * CREATED: 1/20/98 10:27AM
  *
  * LAST MODIFIED: 2/24/98 2:15 PM
  *
  * PARAMETERS
  *	zones		a list of vacuum patches in the RobotWorld Program
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	adds one label to the list of labels on all goto instructions
  * METHOD:
  *
  ************************************************************************************************/
  	if (patches == NULL) patches = new CStringList;
  	PurgeStringList (m_VacuumPatches);
  	m_VacuumPatches = patches;
  	CInstruction *in = NULL;
  	for (int i = 1; i < GetInstructionsSize() - 1; i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CVacuum ) ) ){
  			CVacuum* cv = (CVacuum*)in;
  			cv->InsertVacuumPatches(patches);
  		}
  	}
  }
  
  void CFlowZap::SetLinePaths(CStringList* LinePaths){
  /************************************************************************************************
  * CREATED: 9/11/98 10:27AM
  *
  * LAST MODIFIED: 9/11/98 2:15 PM
  *
  * PARAMETERS
  *	zones		a list of line paths in the RobotWorld Program
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *
  * METHOD:
  *
  ************************************************************************************************/
  	if (LinePaths == NULL) LinePaths = new CStringList;
  	PurgeStringList (m_LinePaths);
  	m_LinePaths = LinePaths;
  	CInstruction *in = NULL;
  	for (int i = 1; i < GetInstructionsSize() - 1; i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CPathFollow ) ) ) {
  			CPathFollow* cp = (CPathFollow*)in;
  			cp->InsertLinePaths(LinePaths);
  		}
  	}
  }
  
  
  void CFlowZap::AddLabel (CString NewCaption){
  /************************************************************************************************
  * CREATED: 1/20/98 10:27AM
  *
  * LAST MODIFIED: 2/24/98 1:46 PM
  *
  * PARAMETERS
  *	NewCaption		the name of the patch to add to the vacuum controls
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	adds one label to the list of labels on all goto instructions
  * METHOD:
  *
  ************************************************************************************************/
  	ASSERT(m_LabelNames != NULL);
  	m_LabelNames->AddTail (NewCaption);
  	CInstruction *in = NULL;
  	for (int i = 1; i < GetInstructionsSize() - 1; i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CGoto ) ) ){
  			CGoto* cg = (CGoto*)in;
  			cg->InsertLabel(NewCaption);
  		}
  	}
  
  	
  }
  
  void CFlowZap::AddLinePath (CString NewCaption){
  	ASSERT(m_LinePaths != NULL);
  	m_LinePaths->AddTail (NewCaption);
  	CInstruction *in = NULL;
  	for (int i = 1; i < GetInstructionsSize() - 1; i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CPathFollow ) ) ){
  			CPathFollow* cg = (CPathFollow*)in;
  			cg->InsertLabel(NewCaption);
  		}
  	}
  
  	
  }
  
  
  
  
  
  
  
  void CFlowZap::AddCalibrationZone (CString NewCaption){
  /************************************************************************************************
  * CREATED: 1/20/98 10:27AM
  *
  * LAST MODIFIED: 1/20/98 10:30AM
  *
  * PARAMETERS
  *	NewCaption		the name of the patch to add to the vacuum controls
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	adds one zone to the list of zones on all cal's
  * METHOD:
  *
  ************************************************************************************************/
  	ASSERT(m_CalibrationZones != NULL);
  	m_CalibrationZones->AddTail (NewCaption);
  	CInstruction *in = NULL;
  	for (int i = 1; i < GetInstructionsSize() - 1; i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CCalibrate ) ) ){
  			CCalibrate* cc = (CCalibrate*)in;
  			cc->InsertZone(NewCaption);
  		}
  	}
  	
  	
  }
  
  
  
  
  
  void CFlowZap::AddVacuumPatch (CString NewCaption){
  /************************************************************************************************
  * CREATED: 2/24/98 3:03 PM
  *
  * LAST MODIFIED: 2/24/98 3:03 PM
  *
  * PARAMETERS
  *	NewCaption		the name of the patch to add to the vacuum controls
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	adds one patch to the list of patches on all vac's
  * METHOD:
  *
  ************************************************************************************************/
  	ASSERT(m_VacuumPatches != NULL);
  	m_VacuumPatches->AddTail (NewCaption);
  	CInstruction *in = NULL;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CVacuum ) ) ){
  			CVacuum* cv = (CVacuum*)in;
  			cv->InsertPatch(NewCaption);
  		}
  	}
  	
  }
  
  
  
  
  
  
  
  void CFlowZap::RemoveLabel(CString OldCaption){
  /************************************************************************************************
  * CREATED: 2/24/98 2:53 PM
  *
  * LAST MODIFIED: 2/24/98 2:53 PM
  *
  * PARAMETERS
  *	OldCaption		the name of the label to remove
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	removes a label from the list of labels
  * METHOD:
  *
  ************************************************************************************************/
  	CInstruction *in = NULL;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CGoto ) ) ){
  			CGoto* cg = (CGoto*)in;
  			cg->RemoveLabel(OldCaption);
  		}
  	}
  	POSITION p = m_LabelNames->Find (OldCaption );
  	if (p != NULL) m_LabelNames->RemoveAt (p);
  }
  
  void CFlowZap::RemoveLinePath(CString OldCaption){
  	CInstruction *in = NULL;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CPathFollow ) ) ){
  			CPathFollow* cp = (CPathFollow*)in;
  			cp->RemoveLabel(OldCaption);
  		}
  	}
  	POSITION p = m_LinePaths->Find (OldCaption );
  	if (p != NULL) m_LinePaths->RemoveAt (p);
  }
  
  
  
  
  void CFlowZap::RemoveCalibrationZone(CString OldCaption){
  /************************************************************************************************
  * CREATED: 2/24/98 2:53 PM
  *
  * LAST MODIFIED: 2/24/98 2:53 PM
  *
  * PARAMETERS
  *	name		the name of the patch to add to the vacuum controls
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	adds one patch to the list of patches on all vac's
  * METHOD:
  *
  ************************************************************************************************/
  	CInstruction *in = NULL;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CCalibrate ) ) ){
  			CCalibrate* cc = (CCalibrate*)in;
  			cc->RemoveZone(OldCaption);
  		}
  	}
  	POSITION p = m_CalibrationZones->Find (OldCaption );
  	if (p != NULL) m_CalibrationZones->RemoveAt (p);
  	
  }
  
  
  
  
  
  void CFlowZap::RemoveVacuumPatch(CString OldCaption){
  /************************************************************************************************
  * CREATED: 2/24/98 2:53 PM
  *
  * LAST MODIFIED: 2/24/98 2:53 PM
  *
  * PARAMETERS
  *	name		the name of the patch to add to the vacuum controls
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	adds one patch to the list of patches on all vac's
  * METHOD:
  *
  ************************************************************************************************/
  	CInstruction *in = NULL;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CVacuum ) ) ){
  			CVacuum* cv = (CVacuum*)in;
  			cv->RemovePatch(OldCaption);
  		}
  	}
  	POSITION p = m_VacuumPatches->Find (OldCaption );
  	if (p != NULL) m_VacuumPatches->RemoveAt (p);
  }
  
  
  
  
  
  
  void CFlowZap::ChangeLabel(CString OldCaption, CString NewCaption){
  /************************************************************************************************
  * CREATED: 2/24/98 1:50 PM
  *
  * LAST MODIFIED: 2/24/98 1:50 PM
  *
  * PARAMETERS
  *	OldCaption		the name of the caption to change
  *	NewCaption		the name to change OldCaption to
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	adds one patch to the list of patches on all vac's
  * METHOD:
  *
  ************************************************************************************************/
  	CInstruction *in = NULL;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CGoto ) ) ){
  			CGoto* cg = (CGoto*)in;
  			cg->ChangeLabel(OldCaption, NewCaption);
  		}
  	}
  
  	POSITION p = m_LabelNames->Find (OldCaption );
  	if (p != NULL) m_LabelNames->RemoveAt (p);
  	m_LabelNames->AddTail (NewCaption);
  
  }
  
  
  void CFlowZap::ChangeLinePath(CString OldCaption, CString NewCaption){
  /************************************************************************************************
  * CREATED: 2/24/98 1:50 PM
  *
  * LAST MODIFIED: 2/24/98 1:50 PM
  *
  * PARAMETERS
  *	OldCaption		the name of the caption to change
  *	NewCaption		the name to change OldCaption to
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	adds one patch to the list of patches on all vac's
  * METHOD:
  *
  ************************************************************************************************/
  	CInstruction *in = NULL;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CPathFollow ) ) ){
  			CPathFollow* cg = (CPathFollow*)in;
  			cg->ChangeLabel(OldCaption, NewCaption);
  		}
  	}
  
  	POSITION p = m_LinePaths->Find (OldCaption );
  	if (p != NULL) m_LinePaths->RemoveAt (p);
  	m_LinePaths->AddTail (NewCaption);
  
  }
  
  
  
  
  void CFlowZap::ChangeVacuumPatch(CString OldCaption, CString NewCaption){
  /************************************************************************************************
  * CREATED: 1/20/98 10:27AM
  *
  * LAST MODIFIED: 1/20/98 10:30AM
  *
  * PARAMETERS
  *	OldCaption		the name of the caption to change
  *	NewCaption		the name to change OldCaption to
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	adds one patch to the list of patches on all vac's
  * METHOD:
  *
  ************************************************************************************************/
  	CInstruction *in = NULL;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CVacuum ) ) ){
  			CVacuum* cv = (CVacuum*)in;
  			cv->ChangeVacuumPatch(OldCaption, NewCaption);
  		}
  	}
  
  	POSITION p = m_VacuumPatches->Find (OldCaption );
  	if (p != NULL) m_VacuumPatches->RemoveAt (p);
  	m_VacuumPatches->AddTail (NewCaption);
  
  }
  
  
  
  
  
  void CFlowZap::ChangeCalibrationZone(CString OldCaption, CString NewCaption){
  /************************************************************************************************
  * CREATED: 2/24/98 2:00PM
  *
  * LAST MODIFIED: 2/24/98 2:00PM
  *
  * PARAMETERS
  *	OldCaption		the name of the caption to change
  *	NewCaption		the name to change OldCaption to
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *	adds one patch to the list of patches on all vac's
  * METHOD:
  *
  ************************************************************************************************/
  	CInstruction *in = NULL;
  	for (int i = 0; i < GetInstructionsSize(); i ++) {
  		in = GetInstructionAtIndex(i);
  		if (in->IsKindOf( RUNTIME_CLASS( CCalibrate ) ) ){
  			CCalibrate* cc = (CCalibrate*)in;
  			cc->ChangeCalibrationZone(OldCaption, NewCaption);
  		}
  	}
  
  	POSITION p = m_CalibrationZones->Find (OldCaption );
  	if (p != NULL) m_CalibrationZones->RemoveAt (p);
  	m_CalibrationZones->AddTail (NewCaption);
  
  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  /*SCROLLING FUNCTIONS */
  void CFlowZap::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
  /************************************************************************************************
  * CREATED: 1/16/98 12:00PM
  *
  * LAST MODIFIED: 1/16/98 2:59PM
  *
  * PARAMETERS
  *	nSBCode		the code of the scroll-type
  *	nPos		the position of the thumb
  *	pScrollBar	a pointer the the scrollbar object
  * RETURN
  *	void		null
  *
  ************************************************************************************************
  * DESCRIPTION
  *
  * METHOD:
  *
  ************************************************************************************************/
  	int Min, Max;
  	SetScrollRange (SB_VERT, 0, m_ListHeight - m_ScrollingArea);
  	GetScrollRange (SB_VERT, &Min, &Max);
  
  	const int cLineInc = 40;
  	const int cPageInc = 80;
  	int toScroll = 0;
  //	static UINT OldPos = 0;
  	if ( (nSBCode != SB_THUMBPOSITION) && (nSBCode != SB_THUMBTRACK) ) {
  		nPos = GetScrollPos(SB_VERT);
  	}
  	switch (nSBCode) {
  	
  	case SB_ENDSCROLL:	break;
  	
  	case SB_BOTTOM:		break;
  
  	case SB_LINEDOWN:
  		if (Max - nPos >= cLineInc) {
  			nPos += cLineInc;
  			toScroll = -cLineInc;
  		}
  		else {
  			toScroll = nPos - Max;
  			nPos = Max;	
  		}
  		break;
  
  	case SB_LINEUP:
  		if (nPos - Min >= cLineInc) {
  			nPos -= cLineInc;
  			toScroll = cLineInc;
  		}
  		else {
  			toScroll = nPos - Min;
  			nPos = Min;	
  		}
  		break;
  
  	case SB_PAGEDOWN:
  		if (Max - nPos >= cPageInc) {
  			nPos += cPageInc;
  			toScroll = -cPageInc;
  		}
  		else {
  			toScroll = nPos - Max;
  			nPos = Max;	
  		}
  		break;
  
  	case SB_PAGEUP:
  		if (nPos - Min >= cPageInc) {
  			nPos -= cPageInc;
  			toScroll = cPageInc;
  		}
  		else {
  			toScroll = nPos - Min;
  			nPos = Min;	
  		}
  		break;
  
  	case SB_THUMBPOSITION:
  		toScroll = m_OldVScrollPosition - nPos;
  		break;
  	case SB_THUMBTRACK:
  		toScroll = m_OldVScrollPosition - nPos;
  		break;
  	}
  	m_Origin += toScroll;
  	CRect r;
  	m_Pallette->GetWindowRect(&r);
  	ScreenToClient(&r);
  	ScrollWindow(0, toScroll,  NULL);
  	m_Pallette->MoveWindow (r.left,0,r.Width(), r.Height() );
  	
  	SetScrollPos(SB_VERT, nPos, TRUE);
  	UpdateArrowPositions( (m_ExpandedView) ? 15 : 8);
  	m_OldVScrollPosition = nPos;
  	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
  	m_Pallette->BringWindowToTop();	
  //	m_Pallette->RedrawWindow(NULL, NULL, RDW_INVALIDATE /* | RDW_ERASE*/);
  }
  
  
  
  
  void CFlowZap::AdjustScrollBars(int ScrollAreaHeight , int ListHeight){
  /************************************************************************************************
  * CREATED:			1/16/98 12:00PM
  *
  * LAST MODIFIED:	1/16/98 1:42PM
  *
  * PARAMETERS
  *
  * RETURN
  *
  ************************************************************************************************
  * DESCRIPTION
  *
  * METHOD:
  *
  ************************************************************************************************/
  //	SetScrollRange(SB_VERT, 0, 100, TRUE );
  	if ( (ScrollAreaHeight < ListHeight) && (m_OldVScrollPosition + ScrollAreaHeight > ListHeight) ) {
  		int toScroll = m_OldVScrollPosition + ScrollAreaHeight - ListHeight;
  		m_Origin += toScroll;
  		CRect r;
  		m_Pallette->GetWindowRect(&r);
  		ScreenToClient(&r);
  		ScrollWindow(0, toScroll,  NULL);
  		m_Pallette->MoveWindow(r.left,0,r.Width(), r.Height() );		
  		UpdateArrowPositions( (m_ExpandedView) ? 15 : 8);
  	} else if ( (ScrollAreaHeight >= ListHeight) && (m_Origin != m_Origional_Origin) ) {
  		int toScroll = m_Origional_Origin - m_Origin;
  		m_Origin += toScroll;
  		CRect r;
  		m_Pallette->GetWindowRect(&r);
  		ScreenToClient(&r);
  		ScrollWindow(0, toScroll,  NULL);
  		m_Pallette->MoveWindow(r.left,0,r.Width(), r.Height() );		
  		UpdateArrowPositions( (m_ExpandedView) ? 15 : 8);
  	}
  
  	if (ScrollAreaHeight > ListHeight) {
  		SetScrollPos(SB_VERT, 0, FALSE);
  		m_OldVScrollPosition = 0;
  		ShowScrollBar(SB_VERT, FALSE);
  		return;
  	} else {
  		ShowScrollBar(SB_VERT, TRUE);
  		SetScrollRange(SB_VERT, 0, ListHeight - ScrollAreaHeight);
  		if (ListHeight - ScrollAreaHeight < m_OldVScrollPosition) {
  			m_OldVScrollPosition = ListHeight - ScrollAreaHeight;
  		}
  	}
  	SetScrollPos(SB_VERT, m_OldVScrollPosition, TRUE);
  /*	if (m_Origin <= dOriginalOrigin) {
  	if (ListHeight < ScrollAreaHeight) {
  		ShowScrollBar (SB_VERT, FALSE);
  		return;
  	}
  
  	else {
  		ShowScrollBar (SB_VERT, TRUE);
  
  	}
  
  	int pos = (m_Origional_Origin - m_Origin);
  	SetScrollRange (SB_VERT, 0, m_ListHeight - m_ScrollingArea);
  	SetScrollPos(SB_VERT, m_OldVScrollPosition, TRUE);
  */
  }
  
  
  
  
  
  
  void CFlowZap::OnSetFocus(CWnd* pOldWnd) {
  /************************************************************************************************
  * CREATED:			1/20/98 11:26AM
  *
  * LAST MODIFIED:	1/20/98 11:26AM
  *
  * PARAMETERS
  *	pOldWnd		the window that has lost the mouse focus
  * RETURN
  *
  ************************************************************************************************
  * DESCRIPTION
  *	called when the window regains focus
  * METHOD:
  *	this function is a patch!!! ***DO NOT REMOVE***	
  *	this function repaints the repeat loop area so that there are no white boxes around the numbers
  ************************************************************************************************/
  	CDialog::OnSetFocus(pOldWnd);
  	RedrawWindow(m_llp->m_Loc, NULL, RDW_INVALIDATE | RDW_NOCHILDREN | RDW_ERASE);
  	
  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  //POPUP MENU HANDLERS
  
  void CFlowZap::HandlePopupInstructionMenu(CInstruction * ptr, CMenu* menu){
  /************************************************************************************************
  * CREATED:			1/21/98 11:20AM
  *
  * LAST MODIFIED:	2/24/98 1:41 PM
  *
  * PARAMETERS
  *	ptr		a ptr to the instruction where the user clicked
  *	menu	a handle to the popup menu
  * RETURN
  *
  ************************************************************************************************
  * DESCRIPTION
  *	initializes the popup menu bar depending on where the user clicked
  * METHOD:
  *	this function handles the popup menu function
  ************************************************************************************************/
  //	if (ptr == m_instructions.GetHead() || ptr == m_instructions.GetTail() ) ptr = NULL;
  	CMenu* pPopup = menu->GetSubMenu(0);
  	ASSERT(pPopup != NULL);
  
  	if ( (ptr != m_instructions.GetHead()) && (ptr != m_instructions.GetTail()) ) {
  		pPopup->ModifyMenu( 0,  MF_BYPOSITION, 0, "This Task: " + ptr->ToString() );
  		pPopup->EnableMenuItem( 0,  MF_BYPOSITION | MF_ENABLED);
  		if (ptr->m_Enabled) {
  			menu->EnableMenuItem (ID_POPUP_ENABLE, MF_BYCOMMAND | MF_GRAYED);
  			menu->EnableMenuItem (ID_POPUP_DISABLE, MF_BYCOMMAND | MF_ENABLED);
  		}
  
  		else {
  				
  			menu->EnableMenuItem (ID_POPUP_ENABLE, MF_BYCOMMAND | MF_ENABLED);
  			menu->EnableMenuItem (ID_POPUP_DISABLE, MF_BYCOMMAND | MF_GRAYED);
  		}
  		pPopup->ModifyMenu( 2, MF_BYPOSITION, 0, "Insert New Task After" );
  	}
  	else {
  		pPopup->ModifyMenu( 0,  MF_BYPOSITION, 0, "Can not alter this task" );
  		pPopup->EnableMenuItem( 0,  MF_BYPOSITION | MF_GRAYED);
  		if (ptr == m_instructions.GetTail()) {
  			pPopup->ModifyMenu( 2, MF_BYPOSITION, 0, "Before This Task Insert" );
  		} else {
  			pPopup->ModifyMenu( 2, MF_BYPOSITION, 0, "After This Task Insert" );
  		}
  	}
  }
  
  
  
  
  
  
  void CFlowZap::HandlePopupFileMenu(CMenu* menu){
  /************************************************************************************************
  * CREATED:			1/21/98 11:40AM
  *
  * LAST MODIFIED:	1/21/98 11:40AM
  *
  * PARAMETERS
  *	menu	a handle to the popup menu
  *
  ************************************************************************************************
  * DESCRIPTION
  *	called when the popup window is activated by a right click
  * METHOD:
  *	this function handles the popup menu function
  ************************************************************************************************/
  	/*
  	NEW : no changes need to be made
  	*/
  
  	/*
  	OPEN no changes need to be made
  	*/
  
  	/*
  	CLOSE no changes need to be made
  	*/
  	if (m_CurrentFileName == " ") menu->EnableMenuItem (ID_POPUP_CLOSE, MF_BYCOMMAND | MF_GRAYED);
  	else						  menu->EnableMenuItem (ID_POPUP_CLOSE, MF_BYCOMMAND | MF_ENABLED);
  
  	/*
  	SAVE no changes need to be made
  	*/
  
  	/*
  	SAVEAS no changes need to be made
  	*/
  
  	/*
  	EXIT no changes need to be made
  	*/
  }
  
  
  
  
  void CFlowZap::OnContextMenu(CWnd*, CPoint point){
  /************************************************************************************************
  * CREATED:			1/21/98 11:40AM
  *
  * LAST MODIFIED:	1/21/98 11:40AM
  *
  * PARAMETERS
  *	point		the point of the user click	
  *	wnd			NULL not used
  ************************************************************************************************
  * DESCRIPTION
  *	called when the user right clicks anywhere on the window
  * METHOD:
  *	
  ************************************************************************************************/
  	if (m_IsSubroutineWindow) return;
  	/****
  	Determine if the click is on an instruction, or in a clear area
  	****/
  	CPoint p = point;
  	ScreenToClient(&p);
  	CInstruction* ptr = GetInstructionAtPixel (p);
  	
  	CMenu menu;
  		
  	
  	if (ptr == NULL) {
  		VERIFY(menu.LoadMenu(IDR_POPUP2));
  		HandlePopupFileMenu(&menu);
  	}
  
  	else {
  		VERIFY(menu.LoadMenu(IDR_POPUP));
  		HandlePopupInstructionMenu(ptr, &menu);
  		m_PopUpMenuLocation = p;
  	}
  
  	
  	// CG: This block was added by the Pop-up Menu component
  	{
  		if (point.x == -1 && point.y == -1){
  			//keystroke invocation
  			CRect rect;
  			GetClientRect(rect);
  			ClientToScreen(rect);
  
  			point = rect.TopLeft();
  			point.Offset(5, 5);
  		}
  
  		
  		
  
  		CMenu* pPopup = menu.GetSubMenu(0);
  		ASSERT(pPopup != NULL);
  
  		CWnd* pWndPopupOwner = this;
  
  		while (pWndPopupOwner->GetStyle() & WS_CHILD)
  			pWndPopupOwner = pWndPopupOwner->GetParent();
  
  		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
  	}
  }
  
  
  
  
  
  
  
  //THIS COMMENT BLOCK DEALS WITH THE NEXT BLOCK OF FUNCTIONS
  /************************************************************************************************
  * CREATED:			1/21/98 11:40AM
  *
  * LAST MODIFIED:	1/21/98 11:40AM
  *
  * PARAMETERS
  *	
  *
  ************************************************************************************************
  * DESCRIPTION
  *	called when the user right clicks to insert an instruction
  * METHOD:
  *	
  ************************************************************************************************/
  void CFlowZap::OnInsertCalibrate() 
  {
  	CInstruction *ptr = GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (ptr == m_instructions.GetTail() ) {
  		InsertInstructionBefore(ptr, makeCal (0) );
  	} else {
  		InsertInstructionAfter(ptr, makeCal (0) );
  	}
  }
  
  
  
  void CFlowZap::OnInsertExecute() 
  {
  	CInstruction *ptr = GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (ptr == m_instructions.GetTail() ) {
  		InsertInstructionBefore(ptr, makeSub (0) );
  	} else {
  		InsertInstructionAfter(ptr, makeSub (0) );
  	}
  }
  
  
  void CFlowZap::OnInsertGoto() 
  {
  	CInstruction *ptr = GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (ptr == m_instructions.GetTail() ) {
  		InsertInstructionBefore(ptr, makeGoto (0));	
  	} else {
  		InsertInstructionAfter(ptr, makeGoto (0));	
  	}
  }
  
  void CFlowZap::OnInsertMotion(){
  	CInstruction *ptr = GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (ptr == m_instructions.GetTail() ) {
  		InsertInstructionBefore(ptr, makeMotion (0) );	
  	} else {
  		InsertInstructionAfter(ptr, makeMotion (0) );	
  	}
  }
  
  
  
  void CFlowZap::OnInsertFollowPath() 
  {
  	CInstruction*ptr = GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (ptr == m_instructions.GetTail() ) {
  		InsertInstructionBefore(ptr, makePathFollow(0) );
  	} else {
  		InsertInstructionAfter(ptr, makePathFollow(0) );
  	}
  }
  
  
  void CFlowZap::OnInsertIf() 
  {
  	CInstruction*ptr = GetInstructionAtPixel (m_PopUpMenuLocation);
  	CInstruction *in  = makeBranch (0);
  	in->m_Loop = m_rlp->AddLoop(in, in);
  	if (ptr == m_instructions.GetTail() ) {
  		InsertInstructionBefore(ptr, in);
  	} else {
  		InsertInstructionAfter(ptr, in);
  	}
  }
  
  void CFlowZap::OnInsertRepeat() 
  {
  	CInstruction *ptr =GetInstructionAtPixel (m_PopUpMenuLocation);
  	CInstruction *in  = makeRepeat (0);
  	in->m_Loop = m_llp->AddLoop(in, in);
  	if (ptr == m_instructions.GetTail() ) {
  		InsertInstructionBefore(ptr, in);	
  	} else {
  		InsertInstructionAfter(ptr, in);	
  	}
  }
  
  void CFlowZap::OnInsertSing() 
  {
  	CInstruction *ptr = GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (ptr == m_instructions.GetTail() ) {
  		InsertInstructionBefore(ptr, makeBeep (0));
  	} else {
  		InsertInstructionAfter(ptr, makeBeep (0));
  	}
  }
  
  
  void CFlowZap::OnInsertStop() 
  {
  	CInstruction *ptr =GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (ptr == m_instructions.GetTail() ) {
  		InsertInstructionBefore(ptr, makeStop (0));	
  	} else {
  		InsertInstructionAfter(ptr, makeStop (0));	
  	}
  }
  
  void CFlowZap::OnInsertVacuum() 
  {
  	CInstruction *ptr = GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (ptr == m_instructions.GetTail() ) {
  		InsertInstructionBefore(ptr, makeVac (0));	
  	} else {
  		InsertInstructionAfter(ptr, makeVac (0));	
  	}
  }
  
  void CFlowZap::OnInsertWait() 
  {
  	CInstruction *ptr = GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (ptr == m_instructions.GetTail() ) {
  		InsertInstructionBefore(ptr, makeWait (0) );
  	} else {
  		InsertInstructionAfter(ptr, makeWait (0) );
  	}
  }
  
  void CFlowZap::OnInsertExternalOutput()
  {
  	CInstruction *ptr = GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (ptr == m_instructions.GetTail() ) {
  		InsertInstructionBefore(ptr, makeExternalOutput(0) );
  	} else {
  		InsertInstructionAfter(ptr, makeExternalOutput(0) );
  	}
  }
  
  
  
  
  
  
  void CFlowZap::OnPopupDelete() {
  /************************************************************************************************
  * CREATED:			1/21/98 11:40AM
  *
  * LAST MODIFIED:	1/21/98 11:40AM
  *
  * PARAMETERS
  *	
  *
  ************************************************************************************************
  * DESCRIPTION
  *	called when the user clicks on an instruction to delete it
  * METHOD:
  *	do not allow user to delete the head or the tail of the list
  ************************************************************************************************/
  	CInstruction *in = (CInstruction *) GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (in == m_instructions.GetHead() || in == m_instructions.GetTail() ) return;
  	if (in != NULL) RemoveInstruction(in);
  }
  
  
  
  
  
  
  void CFlowZap::OnPopupDisable() {
  /************************************************************************************************
  * CREATED:			1/21/98 11:40AM
  *
  * LAST MODIFIED:	1/21/98 11:40AM
  *
  * PARAMETERS
  *	
  *
  ************************************************************************************************
  * DESCRIPTION
  *	called when the user clicks on an instruction to disable it
  * METHOD:
  *	
  ************************************************************************************************/
  	CInstruction *in = (CInstruction *) GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (in == m_instructions.GetHead() || in == m_instructions.GetTail() ) return;
  	if (in != NULL) EnableInstruction(in, FALSE);
  	
  }
  
  
  
  
  
  
  
  void CFlowZap::OnPopupEnable() {
  /************************************************************************************************
  * CREATED:			1/21/98 11:40AM
  *
  * LAST MODIFIED:	1/21/98 11:40AM
  *
  * PARAMETERS
  *	
  *
  ************************************************************************************************
  * DESCRIPTION
  *	called when the user clicks on an instruction to enable it
  * METHOD:
  *	
  ************************************************************************************************/
  	CInstruction *in = (CInstruction *) GetInstructionAtPixel (m_PopUpMenuLocation);
  	if (in == m_instructions.GetHead() || in == m_instructions.GetTail() ) return;
  	if (in != NULL) EnableInstruction(in, TRUE);
  	
  }
  
  BOOL CFlowZap::PreTranslateMessage(MSG* pMsg)
  {
  	// CG: The following block was added by the ToolTips component.
  	{
  		// Let the ToolTip process this message.
  		m_tooltip.RelayEvent(pMsg);
  	}
  	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
  }
  
  
  
  
  
  
  BOOL CFlowZap::IsSoundOn(){
  /************************************************************************************************
  * DATE CREATED: 
  *	24 Feb 98 10:30 AM EST
  * LAST MODIFIED:
  *	24 Feb 98 10:30 AM EST
  * PARAMETERS:
  *	none
  * RETURN
  *	BOOL
  ************************************************************************************************
  * DESCRIPTION
  *	tests is the user has chosen to have sounds on or off as set in the preferences file
  * METHOD:
  *
  ************************************************************************************************/
  	return m_PreferencesControl->m_TestSoundOn;
  }
  
  
  
  
  
  
  
  void CFlowZap::SetCueToStart()
  {
  	CInstruction* in = GetInstructionAtIndex(0);
  	if (in != NULL) m_Cue->SetCueToInstruction (in);
  	
  }
  
  
  
  
  
  
  
  
  void CFlowZap::MakeRobotBeep(double hz){
  /************************************************************************************************
  * DATE CREATED: 
  *	2 March 98 1:22 PM EST
  * LAST MODIFIED:
  *	2 March 98 1:22 PM EST
  * PARAMETERS:
  *	double hz	the frequency of the beep
  * RETURN
  *	void
  ************************************************************************************************
  * DESCRIPTION
  *	causes the robot to beep with the given frequency
  * METHOD:
  *	this function is called when the user lets up on the slider control of any beep instruction
  *   it gives the user a chance to hear the note they have selected w/o having to run a program
  ************************************************************************************************/
  
  #ifdef ROBOT_WORLD_PROJECT
  	CRobot* Robot = GetActiveRobot();
  	if (Robot != NULL) {
  		if (Robot->GetState() == stWaiting) {
  			Robot->AddBeepToInstructions((unsigned short)hz, 500);
  			Robot->FollowInstructions();
  		}
  	}
  #endif
  }
  
  
  
  void CFlowZap::OnShowWindow(BOOL bShow, UINT nStatus) 
  {
  
  	RedrawWindow(m_llp->m_Loc, NULL, RDW_INVALIDATE | RDW_NOCHILDREN | RDW_ERASE);
  	CDialog::OnShowWindow(bShow, nStatus);
  	
  	//OpenFileOnStartup();
  	
  }
  
  void CFlowZap::OnFileManipulation(int Command, int Code, CString filename){
  
  	CMenu *menu = GetMenu();
  	
  	switch (Command) {
  
  		case CMD_FILENEW:
  			m_NeedToSaveAs = TRUE;
  			m_FileIsOpen = TRUE;
  			m_ChangeMadeInFile = FALSE;
  
  			m_CurrentFileName = filename;
  				SetWindowText( "Zap - [" + m_CurrentFileName + "]");
  
  
  			menu->EnableMenuItem(ID_FILE_CLOSE,	MF_BYCOMMAND | MF_ENABLED );
  			menu->EnableMenuItem(ID_FILE_SAVE,	MF_BYCOMMAND | MF_GRAYED );
  			menu->EnableMenuItem(ID_FILE_SAVEAS,MF_BYCOMMAND | MF_ENABLED );
  
  			m_Tool->ShowWindow (SW_HIDE);
  //			m_Border.ShowWindow (SW_SHOW);
  			break;
  
  
  		case CMD_FILEOPEN:
  
  			menu->EnableMenuItem(ID_FILE_SAVE,	MF_BYCOMMAND | MF_GRAYED );
  
  			if (Code == IDCANCEL) {
  				m_FileIsOpen       = FALSE;
  				
  				menu->EnableMenuItem(ID_FILE_CLOSE,	MF_BYCOMMAND | MF_GRAYED );
  				menu->EnableMenuItem(ID_FILE_SAVEAS,MF_BYCOMMAND | MF_GRAYED );
  
  //				m_Tool->ShowWindow  (SW_SHOW);
  //				m_Border.ShowWindow (SW_HIDE);
  			}
  			else { // code is IDOK
  				m_NeedToSaveAs     = FALSE;
  				m_ChangeMadeInFile = FALSE;
  
  				m_CurrentFileName = filename;
  				SetWindowText( "Zap - [" + filename + "]");
  
  				m_FileIsOpen = TRUE;
  
  				m_PreferencesControl->m_PreviousFileName = filename;
  
  
  				menu->EnableMenuItem(ID_FILE_CLOSE,	MF_BYCOMMAND | MF_ENABLED );
  				menu->EnableMenuItem(ID_FILE_SAVEAS,MF_BYCOMMAND | MF_ENABLED );
  
  				m_Tool->ShowWindow  (SW_HIDE);
  //				m_Border.ShowWindow (SW_SHOW);
  			}
  			break;
  
  
  		case CMD_FILECLOSE:
  			m_NeedToSaveAs = FALSE;
  			m_FileIsOpen = FALSE;
  			m_ChangeMadeInFile = FALSE;
  			m_CurrentFileName = filename;
  			SetWindowText( "Zap - [" + m_CurrentFileName + "]");
  
  
  			menu->EnableMenuItem(ID_FILE_CLOSE,	MF_BYCOMMAND | MF_GRAYED );
  			menu->EnableMenuItem(ID_FILE_SAVE,	MF_BYCOMMAND | MF_GRAYED );
  			menu->EnableMenuItem(ID_FILE_SAVEAS,MF_BYCOMMAND | MF_GRAYED );
  			m_Tool->ShowWindow (SW_SHOW);
  //			m_Border.ShowWindow (SW_HIDE);
  			break;
  
  
  		case CMD_FILESAVE:
  			m_NeedToSaveAs = FALSE;
  			m_FileIsOpen = TRUE;
  			m_ChangeMadeInFile = FALSE;
  			menu->EnableMenuItem(ID_FILE_SAVE,	MF_BYCOMMAND | MF_GRAYED );
  			break;
  
  
  		case CMD_FILESAVEAS:
  			if (Code == IDOK)	m_NeedToSaveAs = FALSE;
  			else				m_NeedToSaveAs = TRUE;
  
  			m_FileIsOpen = TRUE;
  			m_ChangeMadeInFile = FALSE;
  
  			m_CurrentFileName = filename;
  			SetWindowText( "Zap - [" + filename + "]");
  			m_PreferencesControl->m_PreviousFileName = filename;
  			break;
  
  
  		case CMD_FILEEXIT:
  			m_PreferencesControl->WritePrefs();
  			m_NeedToSaveAs = FALSE;
  			m_FileIsOpen = FALSE;
  			m_ChangeMadeInFile = FALSE;
  			break;
  
  	}
  
  	
  
  }
  
  
  
  void CFlowZap::ChangeMadeInFile()
  {
  	if (! m_ChangeMadeInFile ) {
  		CMenu *menu = GetMenu();
  		m_ChangeMadeInFile = TRUE;
  		menu->EnableMenuItem(ID_FILE_SAVE,	MF_BYCOMMAND | MF_ENABLED );
  		DrawMenuBar();
  	}
  
  }
  
  
  
  
  void CFlowZap::UpdateMovableStatusOfInstructions()
  {
  	CInstruction* in= NULL;
  	for (int i = 1; i < GetInstructionsSize() - 1; i ++) {
  		in = GetInstructionAtIndex(i);
  		in->MakeMoveable(TRUE);
  		if (in->IsKindOf( RUNTIME_CLASS( CRepeat ) ) ){
  			CRepeat* cr = (CRepeat*)in;
  			cr->MakeMoveable(FALSE);
  			cr->m_Loop->m_End->MakeMoveable(FALSE);
  		}
  	}
  }
  
  
  
  
  
  
  
  void CFlowZap::OnSize(UINT nType, int cx, int cy) 
  {
  	CDialog::OnSize(nType, cx, cy);
  /*Center the tool palette*/
  	if (m_Pallette != NULL) {
  		if (m_Pallette->m_hWnd != NULL) {
  			CRect ClientRect;
  			GetClientRect(&ClientRect);
  			CRect PalletteRect;
  			m_Pallette->GetWindowRect(&PalletteRect);
  			ScreenToClient(&PalletteRect);
  			int margin = (ClientRect.Width() - PalletteRect.Width()) / 2;
  			if (margin < 5) {
  				margin = 5;
  			}
  			m_Pallette->MoveWindow(margin, PalletteRect.top, PalletteRect.Width(), PalletteRect.Height(), TRUE);
  		}
  	}
  
  	if (!m_instructions.IsEmpty()) {
  		UpdateZonePlacements();
  		UpdateInstructionPositions( (m_ExpandedView) ? 15 : 8);
  	}
  	RedrawWindow();	
  }
  
  
  CInstruction* CFlowZap::MakePathFollowFromFile(CStringList *words)
  {
  	CPathFollow* cpf = (CPathFollow*)makePathFollow(0);
  	CString pathname = "";
  	CString speed;
  	speed = words->GetAt(words->FindIndex(words->GetCount() -2) );
  	for (int i = 1; i < words->GetCount() -2; i++){
  		pathname += words->GetAt(words->FindIndex(i) ) + " ";
  	}
  	pathname.TrimRight(); // trims the trailing white space from the name of the file
  	cpf->Configure (pathname, atoi(speed) );
  	
  	cpf->Enable(StringToBool(words->GetAt (words->FindIndex(words->GetCount() - 1) ) ));
  	return cpf;
  }
  
  void CFlowZap::OnPlayStart()
  {
  	if (m_Playing) {
  		m_Pause = false;
  #ifdef ROBOT_WORLD_PROJECT
  		if (GetActiveRobot() != NULL) {
  			GetActiveRobot()->Pause(false);
  		}
  #endif
  		return;
  	}
  	if (m_RobotWorldView->GetActiveRobot() == NULL) {
  		return;
  	} else {
  		if (m_RobotWorldView->GetActiveRobot()->GetState() != stWaiting) return;
  	}
  	SetActiveRobot(m_RobotWorldView->GetActiveRobot());
  	m_Playing = true;
  #ifdef ROBOT_WORLD_PROJECT
  //	m_PlayButton.EnableWindow(FALSE);
  //	m_PauseButton.EnableWindow(TRUE);
  //	m_PauseButton.SetFocus();
  //	m_StopButton.EnableWindow(TRUE);
  
  	if (!IsWindowVisible()) {
  		m_Cue->SetCueToInstruction(GetInstructionAtIndex(0));
  	}
  
  	CInstruction* in = Play();
  	if (in != NULL) {
  		bool result;
  		bool stop = false;
  		bool OldEStopVisible = GetActiveRobot()->GetEStopVisible();
  		GetActiveRobot()->SetEStopVisible(true);
  		while ( !stop ) {
  			if (in == NULL) {
  				stop = true;
  			} else if ( in->IsKindOf(RUNTIME_CLASS (CStop)) ) {
  				stop = true;
  			} else {
  		//		if (ExecuteInstruction(in, result) != TR_TaskComplete) {
  				#ifdef _DEBUG
  				afxDump << in->ToString() + "\n";
  				#endif
  				if (in->Execute(GetActiveRobot(), m_ExecutableObjectList, result) != TR_TaskComplete) {
  					stop = true;
  				} else {
  					in = GetInstructionToExecute(result);
  				}
  //				ASSERT(in != NULL);
  			}
  		}
  		OnPlayStop();
  		GetActiveRobot()->SetEStopVisible(OldEStopVisible);
  	}
  	SetActiveRobot(NULL);
  #endif
  	m_Playing = false;
  	return;
  }
  
  void CFlowZap::OnPlayPause()
  {
  //	m_PlayButton.EnableWindow(TRUE);
  //	m_PlayButton.SetFocus();
  //	m_PauseButton.EnableWindow(TRUE);
  //	m_StopButton.EnableWindow(TRUE);
  	if (m_Playing) {
  		m_Pause = !m_Pause;
  #ifdef ROBOT_WORLD_PROJECT
  		if (GetActiveRobot() != NULL) GetActiveRobot()->Pause(m_Pause);
  #endif
  	}
  }
  
  void CFlowZap::OnPlayStop()
  {
  //	m_PlayButton.EnableWindow(TRUE);
  //	m_PlayButton.SetFocus();
  //	m_PauseButton.EnableWindow(FALSE);
  //	m_StopButton.EnableWindow(FALSE);
  	Stop();
  	m_Playing = false;
  	m_Pause = false;
  #ifdef ROBOT_WORLD_PROJECT
  	if (GetActiveRobot() != NULL) {
  		GetActiveRobot()->EStop();
  		GetActiveRobot()->SetBuzzerFrequency(0);
  	}
  #endif
  }
  
  void CFlowZap::OnDestroy() 
  {
  	CDialog::OnDestroy();
  	
  	CRect FlowZapRect;
  	GetWindowRect(FlowZapRect);
  	AfxGetApp()->WriteProfileInt(FLOWZAP_WINDOW_SECTION, "TOP", FlowZapRect.top);
  	AfxGetApp()->WriteProfileInt(FLOWZAP_WINDOW_SECTION, "LEFT", FlowZapRect.left);
  	AfxGetApp()->WriteProfileInt(FLOWZAP_WINDOW_SECTION, "BOTTOM", FlowZapRect.bottom);
  	AfxGetApp()->WriteProfileInt(FLOWZAP_WINDOW_SECTION, "RIGHT", FlowZapRect.right);
  	
  }
  
  int CFlowZap::OnCreate(LPCREATESTRUCT lpCreateStruct) 
  {
  	if (CDialog::OnCreate(lpCreateStruct) == -1)
  		return -1;
  
  	CPoint DesktopSize = GetDesktopSize();
  	CRect FlowZapRect;
  	FlowZapRect.top = AfxGetApp()->GetProfileInt(FLOWZAP_WINDOW_SECTION, "TOP", -1);
  	FlowZapRect.left = AfxGetApp()->GetProfileInt(FLOWZAP_WINDOW_SECTION, "LEFT", -1);
  	FlowZapRect.bottom = AfxGetApp()->GetProfileInt(FLOWZAP_WINDOW_SECTION, "BOTTOM", -1);
  	FlowZapRect.right = AfxGetApp()->GetProfileInt(FLOWZAP_WINDOW_SECTION, "RIGHT", -1);
  	if ( (FlowZapRect.top != -1) && (FlowZapRect.left != -1) && (FlowZapRect.bottom != -1) && (FlowZapRect.right != -1) ) {
  	/*Make sure the old FlowZapRect is inside the desktop area*/
  		if ( (FlowZapRect.top < DesktopSize.y) && (FlowZapRect.left < DesktopSize.x) ) {
  			MoveWindow(FlowZapRect);
  		}
  	}
  	
  	EnableToolTips(TRUE);
  	return 0;
  }
  
  void CFlowZap::SetIsSubroutineWindow()
  {
  	m_IsSubroutineWindow = true;
  	m_Playing = true;
  	ModifyStyle(WS_SYSMENU, 0, 0);
  	CMenu *menu = GetMenu();
  	menu->EnableMenuItem(0,	MF_BYPOSITION | MF_GRAYED );
  	menu->EnableMenuItem(1,	MF_BYPOSITION | MF_GRAYED );
  	menu->EnableMenuItem(3, MF_BYPOSITION | MF_GRAYED );
  	RedrawWindow();
  }
  
  void CFlowZap::SetActiveRWGraphicObject(CRWGraphicObject *ActiveObject)
  {
  	m_RobotWorldView->SetCurrentRWGraphicObject(ActiveObject);
  }
  
  int CFlowZap::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
  {
  	return CWnd::OnToolHitTest( point, pTI );
  }
  
  CString CFlowZap::GetDefaultZapDirectory()
  {
  	CRobotWorldApp* MainApp = (CRobotWorldApp*)AfxGetApp();
  	CString DefaultPath = MainApp->m_DefaultPath;
  	DefaultPath += (cFlowZapFilesDirectory + 1);
  	return DefaultPath;
  }
  
  void CFlowZap::SetCueToInstruction(CInstruction *Instruction)
  {
  	m_Cue->SetCueToInstruction(Instruction);
  }
  
  bool CFlowZap::GetPlaying()
  {
  	return m_Playing;
  }
  
  CRobot* CFlowZap::GetActiveRobot()
  {
  	return m_ActiveRobot;
  }
  
  void CFlowZap::SetActiveRobot(CRobot *Robot)
  {
  	m_ActiveRobot = Robot;
  }
