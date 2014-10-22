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
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, 
    USA.
  
    Probotics, Inc
    http://www.personalrobots.com
    700 River Ave #223
    Pittsburgh, PA 15212
    USA
  **********************************************************************/
  
  // Beep.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "FlowZap.h"
  #include "Beep.h"
  #include "util.h"
  
  #define ID_BEEP_FREQUENCY_CHANGED WM_USER + 200
  /////////////////////////////////////////////////////////////////////////////
  // CBeep dialog
  
  IMPLEMENT_DYNAMIC( CBeep, CInstruction)
  
  CBeep::CBeep(CWnd* pParent /*=NULL*/, int index)
  	: CInstruction(CBeep::IDD, pParent, index)
  {
  	//{{AFX_DATA_INIT(CBeep)
  	//}}AFX_DATA_INIT
  	
  }
  
  
  void CBeep::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CBeep)
  	DDX_Control(pDX, IDC_IMAGE_DISABLED, m_DisabledImage);
  	DDX_Control(pDX, IDC_BBORDER, m_BlackBorder);
  	DDX_Control(pDX, IDC_EBORDER, m_EtchedBorder);
  	DDX_Control(pDX, IDC_TEXT1, m_Text1);
  	DDX_Control(pDX, IDC_IMAGE, m_Image);
  	DDX_Control(pDX, IDC_FREQUENCY, m_Frequency);
  	DDX_Control(pDX, IDC_TONE_SLIDER, m_ToneSlider);
  	DDX_Control(pDX, IDC_BEEP_TONE, m_BeepTone);
  	DDX_Control(pDX, IDC_BEEP_LENGTH, m_BeepLength);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CBeep, CDialog)
  	//{{AFX_MSG_MAP(CBeep)
  	ON_WM_PAINT()
  	ON_WM_CREATE()
  	ON_WM_LBUTTONDOWN()
  	ON_WM_HSCROLL()
  	ON_WM_LBUTTONUP()
  	ON_CBN_SELCHANGE(IDC_BEEP_LENGTH, OnSelchangeBeepLength)
  	//}}AFX_MSG_MAP
  	ON_MESSAGE(ID_BEEP_FREQUENCY_CHANGED, OnBeepFrequencyChanged)
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CBeep message handlers
  
  void CBeep::OnPaint() 
  {
  	CPaintDC dc(this); // device context for painting
  	CInstruction::OnPaint(&dc);
  }
  
  int CBeep::OnCreate(LPCREATESTRUCT lpCreateStruct) 
  {
  	if (CDialog::OnCreate(lpCreateStruct) == -1)
  		return -1;
  	
  	return 0;
  }
  
  
  void CBeep::OnOK() 
  {
  	// Comment out following line to keep window from closing when enter is hit.
  	//CDialog::OnOK();
  }
  
  void CBeep::OnCancel() 
  {
  	// Comment out following line to keep window from closing when enter is hit.
  	//CDialog::OnOK();
  }
  
  
  BOOL CBeep::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	
  
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),windRect.Height(), TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),windRect.Height(), TRUE);
  
  	CInstruction::OnInit();
  	
  	m_BeepLength.AddString("1/16 sec.");
  	m_BeepLength.AddString("1/8 sec.");
  	m_BeepLength.AddString("1/4 sec.");
  	m_BeepLength.AddString("1/2 sec.");
  	m_BeepLength.AddString("1 sec.");
  	m_BeepLength.AddString("2 sec.");
  	m_BeepLength.SelectString (-1, "1/2 sec.");
  
  
  	m_ToneSlider.SetRange(0, 35);
  
  	m_ToneIndex = 0;
  
  	
  	
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  
  CString CBeep::ToString()
  {
  	CString tone("temp");
  	CString length("temp");
  	m_BeepTone.GetWindowText(tone);
  	m_BeepLength.GetWindowText(length);
  	return "Beep " + tone + " " + length;
  }
  
  CString CBeep::ToFile()
  {
  	CString length("temp");
  	m_BeepLength.GetWindowText(length);
  	return "Sing Tone " + IntToString(m_ToneIndex) + " for " + length + " " + BoolToString (m_Enabled);
  }
  
  void CBeep::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
  {
  //	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK) {
  //		m_ToneIndex = (int)nPos;
  //		Configure (m_ToneIndex);	
  //	}
  	
  
  	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
  	m_ToneIndex = m_ToneSlider.GetPos();
  	Configure(m_ToneIndex);
  
  	if (nSBCode == SB_ENDSCROLL) {
  		// user has lifted up on the mouse after dragging.
  		CFlowZap *fc = (CFlowZap*)m_fc;
  		double hz = fc->m_HzConversion[m_ToneSlider.GetPos()];
  		PostMessage(ID_BEEP_FREQUENCY_CHANGED, 0, (LPARAM)((float)hz));
  //		fc->MakeRobotBeep (hz);
  		fc->ChangeMadeInFile();
  
  	}
  }
  
  void CBeep::Configure(int index){
  /************************************************************************************************
  * DATE CREATED: 
  *	1 Jan 98 11:07 PM EST
  * LAST MODIFIED:
  *	24 Mar 98 1:18 PM EST
  * PARAMETERS:
  *	index	the index into the array of frequencies and notes (also the position of the slider
  * RETURN
  *	void
  ************************************************************************************************
  * DESCRIPTION
  *	this is the configure from a file method
  * METHOD:
  *
  ************************************************************************************************/
  	CFlowZap *fc = (CFlowZap*)m_fc;
  	double hz = fc->m_HzConversion[index];
  
  	CString frequency  = DoubleToString (hz, 1);
  				
  	m_BeepTone.SetWindowText(fc->m_NoteConversion[index]);
  	m_Frequency.SetWindowText(frequency + " hz");
  	m_ToneSlider.SetPos(index); 
  }
  
  void CBeep::Configure(int index, CString time)
  {
  	Configure (index);
  	m_ToneIndex = index;
  	m_BeepLength.SelectString (-1, time);
  
  }
  
  
  
  
  
  void CBeep::MakeCondensed()
  {
  	// don't condense this instruction if it is selected
  	if (IsSelected() ) return;
  
  	
  	m_Expanded = FALSE;
  	
  	// hide all the components
  	m_BeepLength.ShowWindow(SW_HIDE);	m_BeepTone.ShowWindow(SW_HIDE);		m_Frequency.ShowWindow(SW_HIDE);
  	m_ToneSlider.ShowWindow(SW_HIDE);	m_Image.ShowWindow(SW_HIDE);		m_DisabledImage.ShowWindow(SW_HIDE);
  	m_Text1.ShowWindow(SW_HIDE);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),16, TRUE);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),16, TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),16, TRUE);
  
  }
  
  void CBeep::MakeExpanded()
  {
  	m_Expanded = TRUE;
  	m_BeepLength.ShowWindow(SW_SHOW);
  	m_BeepTone.ShowWindow(SW_SHOW);
  	m_Frequency.ShowWindow(SW_SHOW);
  	m_ToneSlider.ShowWindow(SW_SHOW);
  	if (!m_Enabled) m_DisabledImage.ShowWindow(SW_SHOW);
  	else  m_Image.ShowWindow(SW_SHOW);
  	m_Text1.ShowWindow(SW_SHOW);
  
  	CRect windRect;
  	GetWindowRect (&windRect);
  	MoveWindow (windRect.TopLeft().x, windRect.TopLeft().y, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_EtchedBorder.MoveWindow (0,0, windRect.Width(),m_ExpandedHeight, TRUE);
  	m_BlackBorder.MoveWindow  (0,0, windRect.Width(),m_ExpandedHeight, TRUE);
  }
  
  void CBeep::Enable(BOOL enabled)
  {
  	m_Enabled = enabled;
  	m_BeepLength.EnableWindow(enabled);
  	m_BeepTone.EnableWindow(enabled);
  	m_Frequency.EnableWindow(enabled);
  	m_ToneSlider.EnableWindow(enabled);
  	m_Text1.EnableWindow(enabled);
  
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
  
  void CBeep::OnLButtonUp(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseButtonUp (nFlags, point);
  	CDialog::OnLButtonUp(nFlags, point);
  }
  
  void CBeep::OnLButtonDown(UINT nFlags, CPoint point) 
  {
  	CInstruction :: MouseClick (point);
  	CDialog::OnLButtonDown(nFlags, point);
  }
  
  void CBeep::ShowBorder(BOOL raised)
  {
  	if (raised) {
  		m_BlackBorder.ShowWindow (SW_HIDE);
  		m_EtchedBorder.ShowWindow (SW_SHOW);
  	}
  	else        {
  		m_BlackBorder.ShowWindow (SW_SHOW);
  		m_EtchedBorder.ShowWindow (SW_HIDE);
  	}
  }
  
  unsigned int CBeep::GetDuration()
  {
  	CString BeepLengthString;
  	m_BeepLength.GetWindowText(BeepLengthString);
  	if (BeepLengthString == "1/2 sec.") {
  		return 500;
  	} else if (BeepLengthString == "1/4 sec." ) {
  		return 250;
  	} else if (BeepLengthString == "1/8 sec." ) {
  		return 125;
  	} else if (BeepLengthString == "1/16 sec." ) {
  		return 62;
  	} else if (BeepLengthString == "1 sec." ) {
  		return 1000;
  	} else if (BeepLengthString == "2 sec." ) {
  		return 2000;
  	} else {
  		ASSERT(FALSE);
  	}
  	return 0;
  }
  
  
  unsigned int CBeep::GetFrequency()
  {
  	CString FrequencyString;
  	m_Frequency.GetWindowText(FrequencyString);
  	CString Frequency = FrequencyString.SpanExcluding(" hz");
  	return atoi(Frequency);
  }
  
  void CBeep::OnSelchangeBeepLength() 
  {
  	//Set the flag that a change has been made in the file
  	CFlowZap *fc = (CFlowZap*)m_fc;
  	fc->ChangeMadeInFile();
  	
  }
  #ifdef ROBOT_WORLD_PROJECT
  #include "ExecutableRWGraphicObject.h"
  #include "afxtempl.h"
  
  TTaskResult CBeep::Execute(CRobot * Robot, CExecutableRWGraphicObjectList * ExecutableObjectList, bool & result)
  {
  	if (Robot->GetSoftwareRevision() <= 1.06) { /*Do the tune as beeps for robots before version 1.07*/
  		Robot->AddBeepToInstructions(GetFrequency(), GetDuration());
  		CFlowZap *FZ = (CFlowZap*)m_fc;
  		CBeep* LastBeep = this;
  		CInstruction *Next = FZ->GetNextInstructionInList(this);
  	//	Next = FZ->GetNextExecutableInstruction(Next);
  		while ( Next->IsKindOf( RUNTIME_CLASS(CBeep) ) ) {
  			LastBeep = (CBeep*)Next;
  			Robot->AddBeepToInstructions(LastBeep->GetFrequency(), LastBeep->GetDuration());
  			Next = FZ->GetNextInstructionInList(Next);
  	//		Next = FZ->GetNextExecutableInstruction(Next);
  		}
  		FZ->SetCueToInstruction(LastBeep);
  		FZ->RedrawWindow();
  	} else {
  		unsigned int NumberOfNotes = 1;
  		CArray<short, short> FrequencyList;
  		FrequencyList.SetSize(20, 20);
  		CArray<int, int> DurationList;
  		DurationList.SetSize(20, 20);
  		FrequencyList[0] = GetFrequency();
  		DurationList[0] = GetDuration();
  		CFlowZap *FZ = (CFlowZap*)m_fc;
  		CBeep* LastBeep = this;
  		CInstruction *Next = FZ->GetNextInstructionInList(this);
  	//	Next = FZ->GetNextExecutableInstruction(Next);
  		while ( Next->IsKindOf( RUNTIME_CLASS(CBeep) ) ) {
  			LastBeep = (CBeep*)Next;
  			FrequencyList[NumberOfNotes] = LastBeep->GetFrequency();
  			DurationList[NumberOfNotes] = LastBeep->GetDuration();
  			NumberOfNotes++;
  			Next = FZ->GetNextInstructionInList(Next);
  	//		Next = FZ->GetNextExecutableInstruction(Next);
  		}
  		FrequencyList[NumberOfNotes] = -1;
  		DurationList[NumberOfNotes] = 0;
  		NumberOfNotes++;
  		Robot->AddTuneToInstructions(FrequencyList.GetData(), DurationList.GetData(), NumberOfNotes);
  
  		FZ->SetCueToInstruction(LastBeep);
  		FZ->RedrawWindow();
  	}
  
  	TTaskResult TaskResult;
  	TaskResult = Robot->FollowInstructions(true, true, IR_InstructionComplete);
  	return TaskResult;
  }
  #endif
  
  LRESULT CBeep::OnBeepFrequencyChanged(WPARAM, LPARAM frequency)
  {
  	((CFlowZap*)m_fc)->MakeRobotBeep(frequency);
  	return 0;
  }