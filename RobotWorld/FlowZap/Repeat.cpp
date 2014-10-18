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
  // Repeat.cpp: implementation of the CRepeat class.
  //
  //////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "FlowZap.h"
  #include "Repeat.h"
  #include "Loop.h"
  #include "util.h"
  #include "LeftLoopPanel.h"

  /////////////////////////////////////////////////////////////////////////////
  // CRepeat dialog
  IMPLEMENT_DYNAMIC( CRepeat, CInstruction)
  
  CRepeat::CRepeat(CWnd* pParent /*=NULL*/, int index, CLeftLoopPanel * llp)
  	: CInstruction(CRepeat::IDD, pParent, index)
  	, m_llp (llp)
  {
  	//{{AFX_DATA_INIT(CRepeat)
  	//}}AFX_DATA_INIT
  }
  
  
  void CRepeat::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CRepeat)
  	DDX_Control(pDX, IDC_IMAGE_DISABLED, m_DisabledImage);
  	DDX_Control(pDX, IDC_BORDER, m_Border);
  	DDX_Control(pDX, IDC_TEXT2, m_Text2);
  	DDX_Control(pDX, IDC_TEXT1, m_Text1);
  	DDX_Control(pDX, IDC_IMAGE, m_Image);
  	DDX_Control(pDX, IDC_STEPS, m_Steps);
  	DDX_Control(pDX, IDC_NUM_STEPS, m_NumSteps);
  	DDX_Control(pDX, IDC_NUM_REPEATS, m_NumRepeats);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CRepeat, CDialog)
  	//{{AFX_MSG_MAP(CRepeat)
  	ON_WM_LBUTTONDOWN()
  	ON_WM_PAINT()
  	ON_WM_LBUTTONUP()
  	ON_EN_CHANGE(IDC_NUM_REPEATS, OnChangeNumRepeats)
  	ON_EN_KILLFOCUS(IDC_NUM_REPEATS, OnKillfocusNumRepeats)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CRepeat message handlers
  
  void CRepeat::OnOK() 
  {
  	// Comment out following line to keep window from closing when enter is hit.
  	//CDialog::OnOK();
  }
  
  void CRepeat::OnLButtonDown(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseClick (point);
  	
  	CDialog::OnLButtonDown(nFlags, point);
  }
  
  void CRepeat::OnPaint() 
  {
  	CPaintDC dc(this); // device context for painting
  	
  	CInstruction::OnPaint(&dc);
  	
  	// Do not call CDialog::OnPaint() for painting messages
  }
  
  
  
  BOOL CRepeat::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	m_Border.MoveWindow (0,0, windRect.Width(),windRect.Height(), TRUE);
  
  	CInstruction::OnInit();
  	MakeMoveable(FALSE);
  	
  	m_NumRepeats.SetWindowText ("2");
  	m_OrigionalNumRepeats = "2";
  	m_NumRepeats.LimitText(2);
  	SetNumSteps ("0");
  	
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  CString CRepeat::GetNumRepeats()
  {
  	CString s;
  	m_NumRepeats.GetWindowText(s);
  	return s;
  }
  
  void CRepeat::SetNumRepeats(CString s)
  {
  	m_NumRepeats.SetWindowText(s);
  	m_OrigionalNumRepeats = s;
  }
  
  
  void CRepeat::SetNumSteps(CString numSteps)
  {
  	m_NumSteps.SetWindowText (numSteps);
  	if (numSteps == "1") m_Steps.SetWindowText ("task");
  	else m_Steps.SetWindowText ("tasks");
  	m_NumSteps.RedrawWindow(NULL, NULL, RDW_INVALIDATE);
  }
  
  void CRepeat::ConfigureFromFile(CString numBack, CString numRepeats)
  {
  	SetNumSteps (numBack);
  	SetNumRepeats (numRepeats);
  }
  
  
  CString CRepeat::ToString()
  {
  	CString repeats, steps, units;
  	m_NumRepeats.GetWindowText(repeats);
  	m_NumSteps.GetWindowText(steps);
  	m_Steps.GetWindowText(units);
  	return "Repeat last " + steps + " " + units + " " + repeats + " more times ";
  }
  
  CString CRepeat::ToFile()
  {
  	return ToString() + " " + BoolToString (m_Enabled);
  }
  
  
  
  void CRepeat::MakeCondensed()
  {
  	if (IsSelected() ) return;
  	m_Expanded = FALSE;
  	m_NumRepeats.ShowWindow(SW_HIDE);
  	m_NumSteps.ShowWindow(SW_HIDE);
  	m_Steps.ShowWindow(SW_HIDE);
  	m_Image.ShowWindow(SW_HIDE);
  	m_DisabledImage.ShowWindow(SW_HIDE);
  	m_Text1.ShowWindow(SW_HIDE);
  	m_Text2.ShowWindow(SW_HIDE);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),16, TRUE);
  	m_Border.MoveWindow (0, 0, windRect.Width(),16, TRUE);
  }
  
  void CRepeat::MakeExpanded()
  {
  	m_Expanded = TRUE;
  	m_NumRepeats.ShowWindow(SW_SHOW);
  	m_NumSteps.ShowWindow(SW_SHOW);
  	m_Steps.ShowWindow(SW_SHOW);
  	m_Text1.ShowWindow(SW_SHOW);
  	m_Text2.ShowWindow(SW_SHOW);
  
  	if (!m_Enabled) m_DisabledImage.ShowWindow(SW_SHOW);
  	else  m_Image.ShowWindow(SW_SHOW);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_Border.MoveWindow (0, 0, windRect.Width(),m_ExpandedHeight, TRUE);
  }
  
  void CRepeat::OnLButtonUp(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseButtonUp (nFlags, point);
  	CDialog::OnLButtonUp(nFlags, point);
  }
  
  void CRepeat::OnCancel()
  {
  
  }
  
  void CRepeat::ShowBorder(BOOL b)
  {
    // DO NOTHING HERE B/C THE BORDER OF A REPEAT PANEL NEVER CHANGES
  }
  
  
  
  
  
  void CRepeat::Enable(BOOL enabled)
  {
  	m_Enabled = enabled;
  	m_NumRepeats.EnableWindow(enabled);
  	m_NumSteps.EnableWindow(enabled);
  	m_Steps.EnableWindow(enabled);
  	m_Text1.EnableWindow(enabled);
  	m_Text2.EnableWindow(enabled);
  
  	if (m_Expanded) {
  		if (m_Enabled) {
  			m_DisabledImage.ShowWindow(SW_HIDE);
  			m_Image.ShowWindow(SW_SHOW);
  		}
  		else {
  			m_DisabledImage.ShowWindow(SW_SHOW);
  			m_Image.ShowWindow(SW_HIDE);
  		}
  	}
  
  	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
  }
  
  CInstruction * CRepeat::GetLoopDestination()
  {
  	return (m_Loop->m_End);
  }
  
  
  
  void CRepeat::Reset()
  {
  	// m_OrigionalNumRepeats is the number set by the user.
  	m_NumRepeatsLeft = atoi(m_OrigionalNumRepeats);
  	m_NumRepeats.SetWindowText(m_OrigionalNumRepeats);
  }
  
  
  int CRepeat::GetNumRepeatsLeft()
  {
  	// Get the number that is in the window
  	CString strNumRepeatsLeft;
  	m_NumRepeats.GetWindowText(strNumRepeatsLeft);
  
  	/*
  	** Convert to an integer
  	*/
  	int repeats = atoi(strNumRepeatsLeft);
  
  
  	DecrementNumRepeats (repeats);
  
  	
  	return repeats;
  }
  
  
  
  
  
  void CRepeat::DecrementNumRepeats(int num)
  {
  	
  	num--;
  
  
  	/*
  	** reset to origional number if we have reached the end
  	*/
  	if (num == -1) num = atoi(m_OrigionalNumRepeats);
  	m_NumRepeats.SetWindowText(IntToString(num) );
  }
  
  
  
  
  
  
  
  
  
  void CRepeat::UpdateNumSteps()
  {
  	/*
  	** Get the current index of this task
  	*/
  	int index = GetIndex();
  
  
  	/*
  	** Get the current index of the loop task
  	*/
  	int childIndex = m_Loop->m_End->GetIndex();
  
  
  	/*
  	** Set the number of steps as the difference between the two
  	*/
  	SetNumSteps (IntToString (index - childIndex) );
  }
  
  
  void CRepeat::OnChangeNumRepeats() 
  {
  	CFlowZap* fz = (CFlowZap *) m_fc;
  
  
  	
  	CString strNumRepeats;
  	m_NumRepeats.GetWindowText(strNumRepeats);
  
  
  	/*
  	** Change the untits to plural or singular accordingly
  	*/
  	if (strNumRepeats == "1")	m_Text2.SetWindowText("more time");
  	else						m_Text2.SetWindowText("more times");
  
  
  
  
  	fz->RedrawWindow(m_llp->m_Loc, NULL, RDW_INVALIDATE | RDW_ERASE);
  
  
  
  	/*
  	** A change has only been made in the file if the content has changed
  	*/
  	if (m_NumRepeats.GetModify () ) {
  		/*
  		** store the user's input in m_OrigionalNumRepeats only when the user has changed it
  		*/
  		m_NumRepeats.GetWindowText(m_OrigionalNumRepeats);
  		m_NumRepeatsLeft = atoi(m_OrigionalNumRepeats);
  
  
  		fz->ChangeMadeInFile ();
  	}
  	
  }
  
  void CRepeat::OnKillfocusNumRepeats() 
  {
  	m_NumRepeats.GetWindowText(m_OrigionalNumRepeats);
  	if (m_OrigionalNumRepeats == "" ) {
  		m_OrigionalNumRepeats = "1";
  		m_NumRepeats.SetWindowText("1");
  	}
  	
  }
  
