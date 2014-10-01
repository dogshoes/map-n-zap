
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
  
  // DlgCreateCalibrationWall1.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "RobotWorld.h"
  #include "DlgCreateCalibrationWall1.h"
  #include "DlgErrorCheckingWoodWithVacuum.h"
  //#include "DlgCreateCalibrationWall2.h"
  #include "RobotWorldView.h"
  #include "Probability Grid.h"
  #include "CTDPath.h"
  #include "Robot.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  const unsigned long cBmpFrameWidth = 40;
  const unsigned long cBmpFrameHeight = 40;
  const unsigned long cBmpFramePerLine = 5;
  const unsigned long cTotalFrames = 40;
  const int cMaxScrollCornerSize = 1000;
  const float cMaxCornerSize = 7.0f;
  const float cMinCornerSize = 2.0f;
  const int cMaxCornerSizeChars = 4;
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgCreateCalibrationWall1 dialog
  
  /*******************************************************************************
  //Created: 01/10/97 S.R.
  //Last Revision: 01/10/97 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  
  CDlgCreateCalibrationWall1::CDlgCreateCalibrationWall1(CRobotWorldView* pView)
  {
  	//{{AFX_DATA_INIT(CDlgCreateCalibrationWall1)
  	m_chkCornerCalibration = FALSE;
  	m_radLeft = 0;
  	m_radFloorType = 1;
  	m_edtName = _T("Check Point");
  	m_edtCornerSize = _T("");
  	//}}AFX_DATA_INIT
  	m_CornerSize = 5.0f;
  	Create(IDD_CREATECALIBRATIONWALL1, pView);
  	m_View = pView;
  //	m_DlgCreateCalibrationWall2 = new CDlgCreateCalibrationWall2(pView);
  }
  
  CDlgCreateCalibrationWall1::~CDlgCreateCalibrationWall1()
  {
  //	delete m_DlgCreateCalibrationWall2;
  }
  
  
  
  void CDlgCreateCalibrationWall1::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgCreateCalibrationWall1)
  	DDX_Control(pDX, IDC_SCR_CORNER_SIZE, m_scrCornerSize);
  	DDX_Check(pDX, IDC_CHK_CORNER_CALIBRATION, m_chkCornerCalibration);
  	DDX_Radio(pDX, IDC_RAD_LEFT, m_radLeft);
  	DDX_Radio(pDX, IDC_WOOD_FLOOR, m_radFloorType);
  	DDX_Text(pDX, IDC_NAME, m_edtName);
  	DDX_Text(pDX, IDC_EDT_CORNER_SIZE, m_edtCornerSize);
  	DDV_MaxChars(pDX, m_edtCornerSize, 3);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgCreateCalibrationWall1, CDialog)
  	//{{AFX_MSG_MAP(CDlgCreateCalibrationWall1)
  	ON_WM_PAINT()
  	ON_BN_CLICKED(IDC_CHK_CORNER_CALIBRATION, OnChkCornerCalibration)
  	ON_WM_HSCROLL()
  	ON_EN_CHANGE(IDC_EDT_CORNER_SIZE, OnChangeEdtCornerSize)
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgCreateCalibrationWall1 message handlers
  
  void CDlgCreateCalibrationWall1::OnOK() 
  /*******************************************************************************
  //Created: 12/03/96 S.R.
  //Last Revision: 11/13/97 S.R.
  //Parameters:
  //
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	CDialog::OnOK();
  	CRobot* Robot = m_View->GetActiveRobot();
  	if (Robot != NULL) {
  		TRobotInfo RobotInfo = Robot->GetRobotInfo();
  		switch (m_radFloorType) {
  		case 0:
  			Robot->SetFloorType(ftWood);
  			break;
  		case 1:
  			Robot->SetFloorType(ftThinCarpet);
  			break;
  		case 2:
  			Robot->SetFloorType(ftThickCarpet);
  			break;
  		}
  		if ( (Robot->GetFloorType() == ftWood) && (Robot->GetAttachmentType() == atVacuum) ) {
  			CDlgErrorCheckingWoodWithVacuum Warning;
  			Warning.DoModal();
  			return;
  		}
  		if (!m_chkCornerCalibration) {
  			CWall* CalZone = new CWall;
  			if (CalZone->Create(Robot)) {
  				m_View->m_ProbabilityGrid->m_ExecutableObjectList->Add(CalZone);
  				m_View->m_ProbabilityGrid->m_RWGraphicObjectList->Add(CalZone);
  				CalZone->SetCaption(m_edtName);
  	//			CalZone->EditProperties();
  
  			} else {
  				delete CalZone;
  			}
  		} else {
  			CCornerWall* CalZone = new CCornerWall;
  			if (CalZone->Create(Robot, m_radLeft == 0, int(GetCornerSize() * cTPI))) {
  				m_View->m_ProbabilityGrid->m_ExecutableObjectList->Add(CalZone);
  				m_View->m_ProbabilityGrid->m_RWGraphicObjectList->Add(CalZone);
  				CalZone->SetCaption(m_edtName);
  	//			CalZone->EditProperties();
  			} else {
  				delete CalZone;
  			}
  		}
  		Robot->SetRobotInfo(RobotInfo);
  	}
  	m_View->RedrawWindow();
  }
  
  void CDlgCreateCalibrationWall1::SetRobotHeading(double Heading)
  {
  	m_RobotHeading = Heading;
  
  	CRect ClientRect;
  	CWnd* pwnd = GetDlgItem(IDC_FRM_BITMAP);
  	pwnd->GetWindowRect(&ClientRect);
  	ScreenToClient(&ClientRect);
  	int xCenter = (ClientRect.left + ClientRect.right) / 2;
  	int yCenter = (ClientRect.top + ClientRect.bottom) / 2;
  	
  	pwnd = GetDlgItem(IDC_RAD_LEFT);
  	pwnd->GetClientRect(&ClientRect);
  	int y;
  	y = int(yCenter - cBmpFrameHeight * sin(Heading + pi/2) + ClientRect.Height()/2);
  	pwnd->SetWindowPos(NULL, int(xCenter + cBmpFrameWidth * cos(Heading + pi/2) - ClientRect.Width()/2), int(yCenter - cBmpFrameHeight * sin(Heading + pi/2) - ClientRect.Height()/2), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  	pwnd = GetDlgItem(IDC_RAD_RIGHT);
  	pwnd->GetClientRect(&ClientRect);
  	y = int(yCenter - cBmpFrameHeight * sin(Heading - pi/2) + ClientRect.Height()/2);
  	pwnd->SetWindowPos(NULL, int(xCenter + cBmpFrameWidth * cos(Heading - pi/2) - ClientRect.Width()/2), int(yCenter - cBmpFrameHeight * sin(Heading - pi/2) - ClientRect.Height()/2), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  
  }
  
  void CDlgCreateCalibrationWall1::SetRobotBitmap(HBITMAP RobotBitmap)
  {
  	m_hbmRobotBitmap = RobotBitmap;
  }
  
  void CDlgCreateCalibrationWall1::OnPaint() 
  {
  	CPaintDC dc(this); // device context for painting
  	
  	UpdateHeading(m_RobotHeading);
  	// TODO: Add your message handler code here
  	
  	// Do not call CDialog::OnPaint() for painting messages
  }
  
  void CDlgCreateCalibrationWall1::UpdateHeading(double Heading)
  /*******************************************************************************
  ********************************************************************************
  //Description:
  //		Updates the robot bitmap to reflect the correct heading.
  //Method:
  //
  *******************************************************************************/
  {
  	UpdateData(TRUE);
  //	if (!m_chkCornerCalibration) return;
  	CDC* dc = GetDC(); // device context for painting
  //	if (Heading == 360) Heading = 0;
  //	Heading = DegreeToRadian(Heading);
  //	Heading = BringAngleInRange(Heading);
  
  	CRect rcRect;
  	int Frame = RobotHeadingToBitmapFrame(Heading, cTotalFrames);
  	rcRect.left   = Frame%cBmpFramePerLine*cBmpFrameWidth;
  	rcRect.top    = Frame/cBmpFramePerLine*cBmpFrameHeight;
  	rcRect.right  = rcRect.left + cBmpFrameWidth;
  	rcRect.bottom = rcRect.top + cBmpFrameHeight;
  
  /*Blit Robot*/
  	CRect ClientRect;
  	CWnd* pwnd = GetDlgItem(IDC_FRM_BITMAP);
  	pwnd->GetWindowRect(&ClientRect);
  	ScreenToClient(&ClientRect);
  	int xCenter = (ClientRect.left + ClientRect.right) / 2 - cBmpFrameWidth / 2;
  	int yCenter = (ClientRect.top + ClientRect.bottom) / 2 - cBmpFrameHeight / 2;
  //	CRect RadioClientRect;
  //	m_rad0.GetWindowRect(&RadioClientRect);
  //	ScreenToClient(&RadioClientRect);
  //	ClientRect.top = (RadioClientRect.top + RadioClientRect.bottom) / 2 - cBmpFrameHeight / 2;
  //	ClientRect.top = ClientRect.Height()/2 - cBmpFrameHeight / 2;
  
  	CBitmap* temp = m_dcDisplayMemory.SelectObject(CBitmap::FromHandle(m_hbmRobotBitmap));
  	if (temp == NULL) {
  		AfxMessageBox("FindHomebaseCorner SelectObject Error");
  	}
  	
  	dc->BitBlt(xCenter, yCenter, rcRect.Width(), rcRect.Height(), &m_dcDisplayMemory, rcRect.left, rcRect.top, SRCCOPY );
  //	dc->BitBlt(0, 0, ClientRect.Width(), ClientRect.Height(), &m_dcDisplayMemory, 0, 0, SRCCOPY );
  	m_dcDisplayMemory.SelectObject(temp);
  }
  
  BOOL CDlgCreateCalibrationWall1::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  	m_scrCornerSize.SetScrollRange(0, cMaxScrollCornerSize);
  	SetCornerSize(m_CornerSize);
  	if (m_dcDisplayMemory.GetSafeHdc() == NULL) {
  		if (!m_dcDisplayMemory.CreateCompatibleDC(GetDC())) {
  			AfxMessageBox("CreateCalibrationWall1 CreateCompatibleDC failed");
  		}
  	}
  	GetWindowRect(&m_OriginalRect);
  	
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CDlgCreateCalibrationWall1::OnChkCornerCalibration() 
  {
  	UpdateData(TRUE);
  	CWnd* pwnd = GetDlgItem(IDC_RAD_LEFT);
  	pwnd->EnableWindow(m_chkCornerCalibration);
  /*	if (m_chkCornerCalibration) {
  		SetWindowPos(NULL, 0, 0, m_OriginalRect.Width(), m_OriginalRect.Height(), SWP_NOZORDER | SWP_NOMOVE);
  	} else {
  		pwnd = GetDlgItem(IDC_CHK_CORNER_CALIBRATION);
  		CRect WindowRect;
  		pwnd->GetWindowRect(&WindowRect);
  		ScreenToClient(&WindowRect);
  		WindowRect.bottom += 20;
  		SetWindowPos(NULL, 0, 0, m_OriginalRect.Width(), WindowRect.bottom, SWP_NOZORDER | SWP_NOMOVE);
  	}
  */
  	GetDlgItem(IDC_RAD_RIGHT)->EnableWindow(m_chkCornerCalibration);
  	GetDlgItem(IDC_SCR_CORNER_SIZE)->EnableWindow(m_chkCornerCalibration);
  	GetDlgItem(IDC_EDT_CORNER_SIZE)->EnableWindow(m_chkCornerCalibration);
  	m_radLeft = 0;
  	SetRobotHeading(m_View->GetActiveRobot()->GetHeading());
  	RedrawWindow();
  	UpdateData(FALSE);
  }
  
  void CDlgCreateCalibrationWall1::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
  {
  	const int cLineInc = 10;
  	const int cPageInc = 10;
  //	static UINT OldPos = 0;
  	int Min, Max;
  
  	switch (nSBCode) {
  	case SB_LEFT:
  		m_scrCornerSize.GetScrollRange(&Min, &Max);
  		nPos = Min;
  		break;
  	case SB_ENDSCROLL:
  		nPos = m_scrCornerSize.GetScrollPos();
  		break;
  	case SB_LINELEFT:
  		nPos = m_scrCornerSize.GetScrollPos();
  		if (nPos <= cLineInc) nPos = 0;
  		else nPos -= cLineInc;
  		m_scrCornerSize.GetScrollRange(&Min, &Max);
  		if (nPos < (UINT)Min) nPos = (UINT)Min;
  		break;
  	case SB_LINERIGHT:
  		nPos = m_scrCornerSize.GetScrollPos();
  		nPos += cLineInc;
  		m_scrCornerSize.GetScrollRange(&Min, &Max);
  		if (nPos > (UINT)Max) nPos = (UINT)Max;
  		break;
  	case SB_PAGELEFT:
  		nPos = m_scrCornerSize.GetScrollPos();
  		nPos -= cPageInc;
  		m_scrCornerSize.GetScrollRange(&Min, &Max);
  		if (nPos < (UINT)Min) nPos = (UINT)Min;
  		break;
  	case SB_PAGERIGHT:
  		nPos = m_scrCornerSize.GetScrollPos();
  		nPos += cPageInc;
  		m_scrCornerSize.GetScrollRange(&Min, &Max);
  		if (nPos > (UINT)Max) nPos = (UINT)Max;
  		break;
  	case SB_RIGHT:
  		m_scrCornerSize.GetScrollRange(&Min, &Max);
  		nPos = Max;
  		break;
  	case SB_THUMBPOSITION:
  		break;
  	case SB_THUMBTRACK:
  		break;
  	}
  	m_scrCornerSize.SetScrollPos(nPos, TRUE);
  	char buffer[cMaxCornerSizeChars];
  	m_CornerSize = ScrollPosToCornerSize(nPos);
  	m_edtCornerSize = gcvt(m_CornerSize, 2, buffer);
  	UpdateData(FALSE);
  
  	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
  }
  
  float CDlgCreateCalibrationWall1::ScrollPosToCornerSize(UINT nPos)
  {
  	return cMinCornerSize + (cMaxCornerSize - cMinCornerSize) / cMaxScrollCornerSize * nPos;
  }
  
  UINT CDlgCreateCalibrationWall1::CornerSizeToScrollPos(float CornerSize)
  {
  	return UINT((CornerSize - cMinCornerSize) * cMaxScrollCornerSize / (cMaxCornerSize - cMinCornerSize));
  }
  
  void CDlgCreateCalibrationWall1::SetCornerSize(float CornerSize)
  {
  	if (CornerSize < cMinCornerSize) CornerSize = cMinCornerSize;
  	if (CornerSize > cMaxCornerSize) CornerSize = cMaxCornerSize;
  	m_CornerSize = CornerSize;
  	if (GetSafeHwnd() != NULL) {
  		m_scrCornerSize.SetScrollPos(CornerSizeToScrollPos(m_CornerSize), TRUE);
  		char buffer[cMaxCornerSizeChars];
  		m_edtCornerSize = gcvt(m_CornerSize, 2, buffer);
  		UpdateData(FALSE);
  	}
  }
  
  float CDlgCreateCalibrationWall1::GetCornerSize()
  {
  	return m_CornerSize;
  }
  
  void CDlgCreateCalibrationWall1::OnChangeEdtCornerSize() 
  {
  	// TODO: If this is a RICHEDIT control, the control will not
  	// send this notification unless you override the CDialog::OnInitDialog()
  	// function and call CRichEditCtrl().SetEventMask()
  	// with the ENM_CHANGE flag ORed into the mask.
  	UpdateData(TRUE);
  	m_CornerSize = (float)atof(m_edtCornerSize);
  	if (m_CornerSize < cMinCornerSize) {
  		m_CornerSize = cMinCornerSize;
  		char buffer[cMaxCornerSizeChars];
  		m_edtCornerSize = gcvt(m_CornerSize, 2, buffer);
  		UpdateData(FALSE);
  	}
  	if (m_CornerSize > cMaxCornerSize) {
  		m_CornerSize = cMaxCornerSize;
  		char buffer[cMaxCornerSizeChars];
  		m_edtCornerSize = gcvt(m_CornerSize, 2, buffer);
  		UpdateData(FALSE);
  	}
  	m_scrCornerSize.SetScrollPos(CornerSizeToScrollPos(m_CornerSize), TRUE);
  }
