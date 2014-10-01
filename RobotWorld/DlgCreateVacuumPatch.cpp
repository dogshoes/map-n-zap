
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
 
  // DlgCreateVacuumPatch.cpp : implementation file
  //
  
  #include "stdafx.h"
  #include "RobotWorld.h"
  #include "DlgCreateVacuumPatch.h"
  #include "VacuumPatch.h"
  #include "rothmath.h"
  #include "WorldGridConsts.h"
  #include "global.h"
  #include <assert.h>
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgCreateVacuumPatch dialog
  
  
  CDlgCreateVacuumPatch::CDlgCreateVacuumPatch(CWnd* pParent /*=NULL*/)
  	: CDialog(CDlgCreateVacuumPatch::IDD, pParent)
  {
  	//{{AFX_DATA_INIT(CDlgCreateVacuumPatch)
  	m_edtPathWidth = 6.5f;
  	m_edtTurnDiameter = 22.0f;
  	m_edtName = _T("VacPatch");
  	m_edtVacuumingSpeed = 10.0f;
  	//}}AFX_DATA_INIT
  	m_EnterSide = esTopLft;
  	m_VacuumPath = NULL;
  }
  
  
  void CDlgCreateVacuumPatch::DoDataExchange(CDataExchange* pDX)
  {
  	CDialog::DoDataExchange(pDX);
  	//{{AFX_DATA_MAP(CDlgCreateVacuumPatch)
  	DDX_Control(pDX, IDC_VACUUM_PATCH_BOUNDS, m_staVacuumPatchBounds);
  	DDX_Text(pDX, IDC_EDTPATHWIDTH, m_edtPathWidth);
  	DDV_MinMaxFloat(pDX, m_edtPathWidth, 0.f, 20.f);
  	DDX_Text(pDX, IDC_EDTTURNDIAMETER, m_edtTurnDiameter);
  	DDV_MinMaxFloat(pDX, m_edtTurnDiameter, 0.f, 44.f);
  	DDX_Text(pDX, IDC_EDT_NAME, m_edtName);
  	DDX_Text(pDX, IDC_EDT_VACUUMING_SPEED, m_edtVacuumingSpeed);
  	DDV_MinMaxFloat(pDX, m_edtVacuumingSpeed, 5.f, 36.f);
  	//}}AFX_DATA_MAP
  }
  
  
  BEGIN_MESSAGE_MAP(CDlgCreateVacuumPatch, CDialog)
  	//{{AFX_MSG_MAP(CDlgCreateVacuumPatch)
  	ON_BN_CLICKED(IDC_RADBOTLFT, OnRadBotLft)
  	ON_BN_CLICKED(IDC_RADBOTRGT, OnRadBotRgt)
  	ON_BN_CLICKED(IDC_RADLFTBOT, OnRadLftBot)
  	ON_BN_CLICKED(IDC_RADLFTTOP, OnRadLftTop)
  	ON_BN_CLICKED(IDC_RADRGTTOP, OnRadRgtTop)
  	ON_BN_CLICKED(IDC_RADRGTBOT, OnRadRgtBot)
  	ON_BN_CLICKED(IDC_RADTOPLFT, OnRadTopLft)
  	ON_BN_CLICKED(IDC_RADTOPRGT, OnRadTopRgt)
  	ON_WM_PAINT()
  	ON_WM_VSCROLL()
  	ON_WM_SIZE()
  	ON_WM_HSCROLL()
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CDlgCreateVacuumPatch message handlers
  
  void CDlgCreateVacuumPatch::OnRadBotLft() 
  {
  	m_EnterSide = esBotLft;
  	UpdateVacuumPatch();
  }
  
  void CDlgCreateVacuumPatch::OnRadBotRgt() 
  {
  	m_EnterSide = esBotRgt;
  	UpdateVacuumPatch();
  }
  
  void CDlgCreateVacuumPatch::OnRadLftBot() 
  {
  	m_EnterSide = esLftBot;
  	UpdateVacuumPatch();
  }
  
  void CDlgCreateVacuumPatch::OnRadLftTop() 
  {
  	m_EnterSide = esLftTop;
  	UpdateVacuumPatch();
  }
  
  void CDlgCreateVacuumPatch::OnRadRgtTop() 
  {
  	m_EnterSide = esRgtTop;
  	UpdateVacuumPatch();
  }
  
  void CDlgCreateVacuumPatch::OnRadRgtBot() 
  {
  	m_EnterSide = esRgtBot;
  	UpdateVacuumPatch();
  }
  
  void CDlgCreateVacuumPatch::OnRadTopLft() 
  {
  	m_EnterSide = esTopLft;
  	UpdateVacuumPatch();
  }
  
  void CDlgCreateVacuumPatch::OnRadTopRgt() 
  {
  	m_EnterSide = esTopRgt;
  	UpdateVacuumPatch();
  }
  
  BOOL CDlgCreateVacuumPatch::OnInitDialog() 
  {
  	CDialog::OnInitDialog();
  
  /*Initial radio button checked is top left*/
  	switch (m_EnterSide) {
  	case esTopLft:
  		CheckDlgButton(IDC_RADTOPLFT, 1);
  		break;
  	case esTopRgt:
  		CheckDlgButton(IDC_RADTOPRGT, 1);
  		break;
  	case esRgtTop:
  		CheckDlgButton(IDC_RADRGTTOP, 1);
  		break;
  	case esRgtBot:
  		CheckDlgButton(IDC_RADRGTBOT, 1);
  		break;
  	case esBotRgt:
  		CheckDlgButton(IDC_RADBOTRGT, 1);
  		break;
  	case esBotLft:
  		CheckDlgButton(IDC_RADBOTLFT, 1);
  		break;
  	case esLftBot:
  		CheckDlgButton(IDC_RADLFTBOT, 1);
  		break;
  	case esLftTop:
  		CheckDlgButton(IDC_RADLFTTOP, 1);
  		break;
  	}
  	UpdateData(FALSE);
  	UpdateVacuumPatch();
  
  	return TRUE;  // return TRUE unless you set the focus to a control
  	              // EXCEPTION: OCX Property Pages should return FALSE
  }
  
  void CDlgCreateVacuumPatch::OnOK() 
  {
  	UpdateData(TRUE);
  	if (m_edtTurnDiameter < m_edtPathWidth) {
  		AfxMessageBox("The Turn Diameter must be equal to or larger than the Path Width");
  	} else {
  		CDialog::OnOK();
  	}
  }
  
  
  void CDlgCreateVacuumPatch::SetTurnDiameter(unsigned short TurnDiameter)
  {
  	m_edtTurnDiameter = TurnDiameter/cTPI;
  }
  
  unsigned short CDlgCreateVacuumPatch::GetTurnDiameter()
  {
  	return (unsigned short)Round(m_edtTurnDiameter*cTPI);
  }
  
  void CDlgCreateVacuumPatch::SetPathWidth(unsigned short PathWidth)
  {
  	m_edtPathWidth = PathWidth/cTPI;
  }
  
  unsigned short CDlgCreateVacuumPatch::GetPathWidth()
  {
  	return (unsigned short)Round(m_edtPathWidth*cTPI);
  }
  
  void CDlgCreateVacuumPatch::SerializeV2(CArchive & archive)
  /*Corrects an error in the serializing.  The archive isn't
  saving the verison information*/
  {
  	if (archive.IsStoring()) {
  		archive << m_edtPathWidth;
  		archive << m_edtTurnDiameter;
  		archive << (DWORD)m_EnterSide;
  	} else {
  		DWORD temp;
  		archive >> m_edtPathWidth;
  		archive >> m_edtTurnDiameter;
  		archive >> temp, m_EnterSide = TEnterSide(temp);
  	}
  }
  
  void CDlgCreateVacuumPatch::Serialize(CArchive & archive)
  {
  	UINT nVersion;
  	if (archive.IsStoring()) {
  		nVersion = VERSION_NUMBER;
  		archive << nVersion;
  		archive << m_edtPathWidth;
  		archive << m_edtTurnDiameter;
  		archive << (DWORD)m_EnterSide;
  	} else {
  		DWORD temp;
  		archive >> nVersion;
  		archive >> m_edtPathWidth;
  		archive >> m_edtTurnDiameter;
  		archive >> temp, m_EnterSide = TEnterSide(temp);
  	}
  }
  
  const float m_ZoomLevel = 4.0f;
  
  POINT CDlgCreateVacuumPatch::RobotToDDSurface(robPOINT RobotPoint, CRect SurfaceOffset)
  {
  	CRect ClientRect;
  	GetClientRect(&ClientRect);
  	int cWorldGridXSize = ClientRect.Width();
  	int cWorldGridYSize = ClientRect.Height();
  
  	POINT SurfacePoint;
  //	SurfacePoint.x = 	Round(((RobotPoint.x) / cWorldGridPointSize + cWorldGridXSize / 2.0 - SurfaceOffset.x)*m_ZoomLevel);
  //	SurfacePoint.y = 	Round(((-RobotPoint.y) / cWorldGridPointSize + cWorldGridYSize / 2.0 - SurfaceOffset.y)*m_ZoomLevel);
  	SurfacePoint.x = 	Round( RobotPoint.x / cWorldGridPointSize * m_ZoomLevel + (SurfaceOffset.left + SurfaceOffset.right)/2 );
  	SurfacePoint.y = 	Round( -RobotPoint.y / cWorldGridPointSize * m_ZoomLevel + (SurfaceOffset.top + SurfaceOffset.bottom)/2 );
  	return SurfacePoint;
  }
  
  void CDlgCreateVacuumPatch::DrawVacuumPatch()
  {
  	if (m_VacuumPath != NULL) {
  		CDC* pDC = GetDC();
  
  		CBrush ArrowBrush(COLORREF(0));
  //		CBrush BorderBrush(COLORREF(0x0));
  	/*Draw stuff*/
  		RECT OuterRect;
  		robPOINT RobotPoint;
  		POINT point;
  
  		CRect ProbGridBmpUpdateRect;
  		m_staVacuumPatchBounds.GetWindowRect(&ProbGridBmpUpdateRect);
  		ScreenToClient(&ProbGridBmpUpdateRect);
  
  //		CPoint SurfaceOffset = ProbGridBmpUpdateRect.TopLeft();
  		CRect SurfaceOffset = ProbGridBmpUpdateRect;
  
  		RobotPoint.x = m_Bounds.left, RobotPoint.y = m_Bounds.top;
  		point = RobotToDDSurface(RobotPoint, SurfaceOffset);
  		OuterRect.left = point.x;
  		OuterRect.top = point.y;
  		RobotPoint.x = m_Bounds.right, RobotPoint.y = m_Bounds.bottom;
  		point = RobotToDDSurface(RobotPoint, SurfaceOffset);
  		OuterRect.right = point.x;
  		OuterRect.bottom = point.y;
  
  //		pDC->FrameRect(&OuterRect, &BorderBrush);
  		unsigned short i = 0;
  		double HeadForward = m_HeadForward;
  		while (i < m_NumberPaths) {
  			short CosFor = (short)Round(cos(HeadForward));
  			short SinFor = (short)Round(sin(HeadForward));
  			short CosNext = (short)Round(cos(m_HeadNext));
  			short SinNext = (short)Round(sin(m_HeadNext));
  		/*Draw line*/
  			POINT BmpPoint[4];
  			if (i != 0) {
  				BmpPoint[0] = RobotToDDSurface(m_VacuumPath[i-1].Arc2e, SurfaceOffset);
  			} else {
  				BmpPoint[0] = RobotToDDSurface(m_EntryPoint, SurfaceOffset);
  			}
  			pDC->MoveTo(BmpPoint[0]);
  			BmpPoint[0] = RobotToDDSurface(m_VacuumPath[i].Arc1s, SurfaceOffset);
  			pDC->LineTo(BmpPoint[0]);
  		/*Draw arrow head*/
  			double x1, y1, x2, y2;
  			x1 = m_VacuumPath[i].Arc1s.x - 30*CosFor;
  			y1 = m_VacuumPath[i].Arc1s.y - 30*SinFor;
  			RobotPoint.x = x1, RobotPoint.y = y1;
  			BmpPoint[0] = RobotToDDSurface(RobotPoint, SurfaceOffset);
  			x2 = x1 - CosFor * 30 - CosNext * 30;
  			y2 = y1 - SinFor * 30 - SinNext * 30;
  			RobotPoint.x = x2, RobotPoint.y = y2;
  			BmpPoint[1] = RobotToDDSurface(RobotPoint, SurfaceOffset);
  			x2 = x1 - CosFor * 30 + CosNext * 30;
  			y2 = y1 - SinFor * 30 + SinNext * 30;
  			RobotPoint.x = x2, RobotPoint.y = y2;
  			BmpPoint[2] = RobotToDDSurface(RobotPoint, SurfaceOffset);
  			BmpPoint[3] = BmpPoint[0];
  			CRgn ArrowRegion;
  			ArrowRegion.CreatePolygonRgn(BmpPoint, 4, ALTERNATE);
  			pDC->FillRgn(&ArrowRegion, &ArrowBrush);
  		/*If next vacuum path inside the patch, draw an arc to it*/
  			if (i < m_NumberPaths - 1) {
  			/*Draw 2 arcs*/
  				/*Arc 1*/
  				CRect ArcRect;
  				short TurnDiameter = ABS(m_VacuumPath[i].TurnDiameter);
  				x1 = m_VacuumPath[i].Arc1s.x + CosFor * TurnDiameter/2;
  				y1 = m_VacuumPath[i].Arc1s.y + SinFor * TurnDiameter/2;
  				x2 = m_VacuumPath[i].Arc1s.x - CosFor * TurnDiameter/2 + CosNext * TurnDiameter;
  				y2 = m_VacuumPath[i].Arc1s.y - SinFor * TurnDiameter/2 + SinNext * TurnDiameter;
  				RobotPoint.x = x1, RobotPoint.y = y1;
  				point = RobotToDDSurface(RobotPoint, SurfaceOffset);
  				ArcRect.left = point.x, ArcRect.top = point.y;
  				RobotPoint.x = x2, RobotPoint.y = y2;
  				point = RobotToDDSurface(RobotPoint, SurfaceOffset);
  				ArcRect.right = point.x, ArcRect.bottom = point.y;
  				ArcRect.NormalizeRect();
  				if ( (SinFor * CosNext == 1) || (CosFor * SinNext == -1) ) {
  					BmpPoint[0] = RobotToDDSurface(m_VacuumPath[i].Arc1e, SurfaceOffset);
  					BmpPoint[1] = RobotToDDSurface(m_VacuumPath[i].Arc1s, SurfaceOffset);
  				} else {
  					BmpPoint[0] = RobotToDDSurface(m_VacuumPath[i].Arc1s, SurfaceOffset);
  					BmpPoint[1] = RobotToDDSurface(m_VacuumPath[i].Arc1e, SurfaceOffset);
  				}
  				pDC->Arc(ArcRect, BmpPoint[0], BmpPoint[1]);
  				/*Arc 2*/
  				x1 = m_VacuumPath[i].Arc2c.x + m_VacuumPath[i].Arc2r;
  				y1 = m_VacuumPath[i].Arc2c.y + m_VacuumPath[i].Arc2r;
  				RobotPoint.x = x1, RobotPoint.y = y1;
  				point = RobotToDDSurface(RobotPoint, SurfaceOffset);
  				ArcRect.left = point.x, ArcRect.top = point.y;
  				x1 = m_VacuumPath[i].Arc2c.x - m_VacuumPath[i].Arc2r;
  				y1 = m_VacuumPath[i].Arc2c.y - m_VacuumPath[i].Arc2r;
  				RobotPoint.x = x1, RobotPoint.y = y1;
  				point = RobotToDDSurface(RobotPoint, SurfaceOffset);
  				ArcRect.right = point.x, ArcRect.bottom = point.y;
  				ArcRect.NormalizeRect();
  				if ( (SinFor * CosNext == 1) || (CosFor * SinNext == -1) ) {
  					BmpPoint[0] = RobotToDDSurface(m_VacuumPath[i].Arc2s, SurfaceOffset);
  					BmpPoint[1] = RobotToDDSurface(m_VacuumPath[i].Arc2e, SurfaceOffset);
  				} else {
  					BmpPoint[0] = RobotToDDSurface(m_VacuumPath[i].Arc2e, SurfaceOffset);
  					BmpPoint[1] = RobotToDDSurface(m_VacuumPath[i].Arc2s, SurfaceOffset);
  				}
  				pDC->Arc(ArcRect, BmpPoint[0], BmpPoint[1]);
  			}
  		/*Onto next patch*/
  			HeadForward += pi;
  			i++;
  		}
  //		BorderBrush.DeleteObject();
  //		ArrowBrush.DeleteObject();
  	}
  }
  
  void CDlgCreateVacuumPatch::CalculateVacuumPaths()
  /*******************************************************************************
  //Created: 10/02/97 S.R.
  //Last Revision: 10/29/97 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function calculates all the positions and turns the robot has to do
  //	to complete the vacuum patch.
  //Method:
  //	See note "Vacuum Patches: Generating Paths" 10/23/97.
  *******************************************************************************/
  {
  	const unsigned short cVacuumSpeed = 200;
  	robPOINT PathPoint;
  	BOOL KeepDoingPaths = TRUE;
  
  	unsigned short nPatches;
  
  	if (m_PathWidth == 0) nPatches = 1;
  	else if (Round(sin(m_HeadNext)) != 0) {
  		nPatches = unsigned short((m_Bounds.top - m_Bounds.bottom)/m_PathWidth + 1);
  	} else {
  		nPatches = unsigned short((m_Bounds.right - m_Bounds.left)/m_PathWidth + 1);
  	}
  	if (m_VacuumPath == NULL) {
  		m_VacuumPath = new TVacPath[nPatches];
  		m_SizeVacuumPath = nPatches;
  	} else {
  		if (m_SizeVacuumPath < nPatches) {
  			delete m_VacuumPath;
  			m_VacuumPath = new TVacPath[nPatches];
  			assert(m_VacuumPath);
  			m_SizeVacuumPath = nPatches;
  		}
  	}
  	robPOINT EntryPoint = m_EntryPoint;
  	double HeadForward= m_HeadForward;
  	unsigned short i = 0;
  	robPOINT p1, p2;		/*two points on straight away part of path*/
  	robPOINT p3;			/*point at middle of next path*/
  	short CosFor;
  	short SinFor;
  	short CosNext;
  	short SinNext;
  	short TurnDiameter = m_TurnDiameter;
  	short PathWidth;
  
  	while (KeepDoingPaths && i<nPatches) {
  		CosFor = (short)Round(cos(HeadForward));
  		SinFor = (short)Round(sin(HeadForward));
  		CosNext = (short)Round(cos(m_HeadNext));
  		SinNext = (short)Round(sin(m_HeadNext));
  
  		p1.x = EntryPoint.x + CosFor * ( TurnDiameter/2.0 + cRRWheelBase/2.0 );
  		p3.x = EntryPoint.x + CosNext * m_PathWidth + CosFor * ( (m_Bounds.right - m_Bounds.left)/2.0 );
  
  		p1.y = EntryPoint.y + SinFor * ( TurnDiameter/2.0 + cRRWheelBase/2.0 );
  		p3.y = EntryPoint.y + SinNext * m_PathWidth + SinFor * ( (m_Bounds.top - m_Bounds.bottom)/2.0 );
  		
  		m_VacuumPath[i].p1.x = (long)p1.x;
  		m_VacuumPath[i].p1.y = (long)p1.y;
  	/*Calculate the appropriate path width and turning diameter*/
  	/*Calculate PathWidth*/
  		if (SinNext == 1) {
  			PathWidth = (short)min(m_PathWidth, m_Bounds.top - p1.y - cRRWheelBase/2); 
  		} else if (SinNext == -1) {
  			PathWidth = (short)min(m_PathWidth, p1.y - m_Bounds.bottom - cRRWheelBase/2);
  		} else if (CosNext == 1) {
  			PathWidth = (short)min(m_PathWidth, m_Bounds.right - p1.x - cRRWheelBase/2);
  		} else if (CosNext == -1) {
  			PathWidth = (short)min(m_PathWidth, p1.x - m_Bounds.left - cRRWheelBase/2);
  		} else {
  			ASSERT(FALSE);
  			PathWidth = (short)min(m_PathWidth, m_Bounds.top - p1.y - cRRWheelBase/2); 
  		}
  	/*Forget about paths < 1 inch wide*/
  		if (PathWidth <= cTPI) {
  			KeepDoingPaths = FALSE;
  		}
  
  	/*Calculate TurnDiameter*/
  		PathPoint = p1;
  		PathPoint.x += CosNext * (m_TurnDiameter + cRRWheelBase/2);
  		PathPoint.y += SinNext * (m_TurnDiameter + cRRWheelBase/2);
  	/*If next vacuum path inside the patch, turn to it*/
  	/*Take the widest possible turn*/
  		if (m_Bounds.Inside(PathPoint)) {
  			TurnDiameter = m_TurnDiameter;
  		} else {
  			TurnDiameter = -1;
  			if (SinNext == 1) {
  				TurnDiameter = short(m_Bounds.top - p1.y - cRRWheelBase/2);
  			} else if (SinNext == -1) {
  				TurnDiameter = short(p1.y - m_Bounds.bottom - cRRWheelBase/2);
  			} else if (CosNext == 1) {
  				TurnDiameter = short(m_Bounds.right - p1.x - cRRWheelBase/2);
  			} else if (CosNext == -1) {
  				TurnDiameter = short(p1.x - m_Bounds.left - cRRWheelBase/2);
  			}
  			if (TurnDiameter < 0) {
  				KeepDoingPaths = FALSE;
  			}
  		}
  	/*Calculate correct spot for p2 given the turning radius*/
  		p2.x = EntryPoint.x + CosFor * (m_Bounds.right - m_Bounds.left - (TurnDiameter + cRRWheelBase)/2);
  		p2.y = EntryPoint.y + SinFor * (m_Bounds.top - m_Bounds.bottom - (TurnDiameter + cRRWheelBase)/2);
  		m_VacuumPath[i].p2.x = (long)p2.x;
  		m_VacuumPath[i].p2.y = (long)p2.y;
  
  		if (KeepDoingPaths) {
  		/*Turn*/
  			if ( (SinFor * CosNext == 1) || (CosFor * SinNext == -1) ) {
  			/*Turn counterclockwise*/
  				m_VacuumPath[i].TurnDiameter = -TurnDiameter;
  			} else {
  			/*Turn clockwise*/
  				m_VacuumPath[i].TurnDiameter = TurnDiameter;
  			}
  		/*Get PathPoint to get the robot back on the next path segment*/
  			PathPoint.x = p2.x + (PathWidth) * CosNext - 
  				(TurnDiameter/2.0f*sin(acos((TurnDiameter - 2.0f*PathWidth)/TurnDiameter))) * CosFor;
  			PathPoint.y = p2.y + SinNext * PathWidth - 
  				(TurnDiameter/2.0f*sin(acos((TurnDiameter - 2.0f*PathWidth)/TurnDiameter))) * SinFor;
  			m_VacuumPath[i].p3.x = (long)PathPoint.x;
  			m_VacuumPath[i].p3.y = (long)PathPoint.y;
  
  
  		/*Create Turning arcs*/
  			m_VacuumPath[i].Arc1s.x = p2.x;
  			m_VacuumPath[i].Arc1s.y = p2.y;
  			m_VacuumPath[i].Arc1e.x = p2.x - CosFor * sqrt(SQR(TurnDiameter/2.0) - SQR(PathWidth)/4.0) + CosNext * (TurnDiameter/2.0 + PathWidth/2.0);
  			m_VacuumPath[i].Arc1e.y = p2.y + SinNext * (TurnDiameter/2.0 + PathWidth/2.0) - SinFor * sqrt(SQR(TurnDiameter/2.0) - SQR(PathWidth)/4.0);
  			m_VacuumPath[i].Arc1m.x = p2.x + (CosFor + CosNext) * TurnDiameter/2.0;
  			m_VacuumPath[i].Arc1m.y = p2.y + (SinFor + SinNext) * TurnDiameter/2.0;
  			m_VacuumPath[i].Arc2s.x = m_VacuumPath[i].Arc1e.x;
  			m_VacuumPath[i].Arc2s.y = m_VacuumPath[i].Arc1e.y;
  			if (TurnDiameter == PathWidth) {
  			/*Don't do a second arc if TurnDiameter == PathWidth*/
  				m_VacuumPath[i].Arc2s.x = m_VacuumPath[i].Arc1e.x;
  				m_VacuumPath[i].Arc2s.y = m_VacuumPath[i].Arc1e.y;
  				m_VacuumPath[i].Arc2e.x = m_VacuumPath[i].Arc1e.x;
  				m_VacuumPath[i].Arc2e.y = m_VacuumPath[i].Arc1e.y;
  				m_VacuumPath[i].Arc2m.x = m_VacuumPath[i].Arc1e.x;
  				m_VacuumPath[i].Arc2m.y = m_VacuumPath[i].Arc1e.y;
  				m_VacuumPath[i].Arc2r = 0;
  				m_VacuumPath[i].Arc2c.x = m_VacuumPath[i].Arc1e.x;
  				m_VacuumPath[i].Arc2c.y = m_VacuumPath[i].Arc1e.y;
  			} else if (CosFor != 0) {
  			/*horizontal paths*/
  				double xe, ye, m, b, xs, ys;
  				robPOINT center1;
  				TLine Line;
  
  				ye = p2.y + SinNext * PathWidth;
  				xs = m_VacuumPath[i].Arc2s.x;
  				ys = m_VacuumPath[i].Arc2s.y;
  				center1.x = p2.x;
  				center1.y = p2.y + SinNext * TurnDiameter/2.0;
  				FindLineFrom2Points(center1, m_VacuumPath[i].Arc2s, &Line);
  				m = Line.m;
  				b = Line.b;
  			/*Calculate xe*/
  				double temp;
  				temp = sqrt(ye*(ye*(1 + SQR(m)) - 2*b - 2*m*xs - 2*SQR(m)*ys) + ys * (ys * (-1 + SQR(m)) + 2*b + 2*m*xs));
  			/*choose right square root (positive or negative)*/
  				robPOINT Ps;
  				Ps.x = xs;
  				Ps.y = ys;
  				double xe1, xe2;
  				xe1 = temp + xs + m * (ys - ye);
  				xe2 = -temp + xs + m * (ys - ye);
  				/*find center of second arc*/
  				robPOINT center2;
  				center2.x = xe1;
  				center2.y = m*xe1+b;
  				/*if the center of the arc for xe1 is between the center of the
  				first arc, and the second arc's starting point then use xe2.  Otherwise use xe1*/
  				if (PointIsBetween(center2,center1,Ps)) {
  					xe = xe2;
  				} else {
  					xe = xe1;
  				}
  			/*fill in the data members*/
  				m_VacuumPath[i].Arc2e.x = xe;
  				m_VacuumPath[i].Arc2e.y = ye;
  				m_VacuumPath[i].Arc2r = abs(int(m*xe + b - ye));
  				m_VacuumPath[i].Arc2c.x = xe;
  				m_VacuumPath[i].Arc2c.y = ye + SinNext * m_VacuumPath[i].Arc2r;
  				m_VacuumPath[i].Arc2m.x = (m_VacuumPath[i].Arc2s.x + m_VacuumPath[i].Arc2e.x) / 2;
  				m_VacuumPath[i].Arc2m.y = m_VacuumPath[i].Arc2c.y + sqrt(SQR(m_VacuumPath[i].Arc2r) - SQR(m_VacuumPath[i].Arc2m.x - m_VacuumPath[i].Arc2c.x));
  				if (!PointIsBetween(m_VacuumPath[i].Arc2m, m_VacuumPath[i].Arc2s, m_VacuumPath[i].Arc2e)) {
  					m_VacuumPath[i].Arc2m.y = m_VacuumPath[i].Arc2c.y - sqrt(SQR(m_VacuumPath[i].Arc2r) - SQR(m_VacuumPath[i].Arc2m.x - m_VacuumPath[i].Arc2c.x));
  				}
  			} else {
  			/*Vertical paths*/
  				double xe, ye, m, b, xs, ys;
  				robPOINT center1;
  				TLine Line;
  
  				xe = p2.x + CosNext * PathWidth;
  				xs = m_VacuumPath[i].Arc2s.x;
  				ys = m_VacuumPath[i].Arc2s.y;
  				center1.x = p2.x + CosNext * TurnDiameter/2.0;
  				center1.y = p2.y;
  				FindLineFrom2Points(center1, m_VacuumPath[i].Arc2s, &Line);
  				m = Line.m;
  				b = Line.b;
  			/*Calculate ye*/	
  				double temp;
  				temp = sqrt(xe*(xe*(SQR(m) + 1) + 2*b*m - 2*xs - 2*m*ys) + xs*(xs*(-SQR(m) + 1) - 2*b*m + 2*m*ys));
  			/*choose right square root (positive or negative)*/
  				robPOINT Ps;
  				Ps.x = xs;
  				Ps.y = ys;
  				double ye1, ye2;
  				ye1 = (temp + xs + m*ys - xe)/m;
  				ye2 = (-temp + xs + m*ys - xe)/m;
  				/*find center of second arc*/
  				robPOINT center2;
  				center2.x = (ye1 - b)/m;
  				center2.y = ye1;
  				/*if the center of the arc for ye1 is between the center of the
  				first arc, and the second arc's starting point then use ye2.  Otherwise use ye1*/
  				if (PointIsBetween(center2, center1, Ps)) {
  					ye = ye2;
  				} else {
  					ye = ye1;
  				}
  			/*fill in the data members*/
  				m_VacuumPath[i].Arc2e.x = xe;
  				m_VacuumPath[i].Arc2e.y = ye;
  				m_VacuumPath[i].Arc2r = abs( int((ye - b)/m - xe) );
  				m_VacuumPath[i].Arc2c.x = xe + CosNext * m_VacuumPath[i].Arc2r;
  				m_VacuumPath[i].Arc2c.y = ye;
  				m_VacuumPath[i].Arc2m.y = (m_VacuumPath[i].Arc2s.y + m_VacuumPath[i].Arc2e.y) / 2;
  				m_VacuumPath[i].Arc2m.x = m_VacuumPath[i].Arc2c.x + sqrt(SQR(m_VacuumPath[i].Arc2r) - SQR(m_VacuumPath[i].Arc2m.y - m_VacuumPath[i].Arc2c.y));
  				if (!PointIsBetween(m_VacuumPath[i].Arc2m, m_VacuumPath[i].Arc2s, m_VacuumPath[i].Arc2e)) {
  					m_VacuumPath[i].Arc2m.x = m_VacuumPath[i].Arc2c.x - sqrt(SQR(m_VacuumPath[i].Arc2r) - SQR(m_VacuumPath[i].Arc2m.y - m_VacuumPath[i].Arc2c.y));
  				}
  			}
  		} else {
  			m_VacuumPath[i].Arc1s.x = p2.x;
  			m_VacuumPath[i].Arc1s.y = p2.y;
  			m_VacuumPath[i].Arc1e.x = p2.x;
  			m_VacuumPath[i].Arc1e.y = p2.y;
  			m_VacuumPath[i].Arc2s.x = p2.x;
  			m_VacuumPath[i].Arc2s.y = p2.y;
  		}
  	/*Onto next path*/
  		EntryPoint.x += PathWidth * CosNext +
  			CosFor * (m_Bounds.right - m_Bounds.left);
  		EntryPoint.y += PathWidth * SinNext +
  			SinFor * (m_Bounds.top - m_Bounds.bottom);
  	/*See if it is inside the patch*/
  		if ( !m_Bounds.Inside(EntryPoint) ) {
  			KeepDoingPaths = FALSE;
  		}
  		HeadForward += pi;
  		i++;
  	}
  
  	m_NumberPaths = i;
  	m_VacuumPath[i].TurnDiameter = 0;
  	m_VacuumPath[0].p1.x = (long)m_EntryPoint.x;
  	m_VacuumPath[0].p1.y = (long)m_EntryPoint.y;
  /*Calculate ExitPoint*/
  //	EntryPoint.x -= PathWidth * CosNext;
  //	EntryPoint.y -= PathWidth * SinNext;
  }
  
  void CDlgCreateVacuumPatch::PreCreateVacuumPatch(robRECT PatchBounds, TEnterSide EnterSide, unsigned short PathWidth, unsigned short TurnDiameter)
  /*******************************************************************************
  //Created: 09/25/97 S.R.
  //Last Revision: 10/23/97 S.R.
  //Parameters:
  //	PatchBounds:	The boundaries of the vacuum patch (in robot coordinates).
  //	EnterSide:		The side of the vacuum patch the robot should enter from.
  //	PathWidth:		The width of a vacuum path (in 0.1" units)
  //	TurnDiameter:	The turning diameter for the robot (in 0.1" units)
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function creates a vacuum patch with the given dimensions.  First
  //	off the function normalizes PatchBounds.  Then, given which side the robot
  //	is entering from, the function makes sure that the width and height are
  //	large enough for the robot to get through.  If they're not, the funciton
  //	expands the zone by moving the lower right hand corner.  Finally, given
  //	the entry side, the function figures out what m_HeadForward and m_HeadNext
  //	should be and sets them.
  //Method:
  //
  *******************************************************************************/
  {
  	const float cTurnRadius = cRRWheelBase/2.0f;
  	m_EnterSide = EnterSide;
  	m_PathWidth = PathWidth;
  	m_TurnDiameter = TurnDiameter;
  	if (m_TurnDiameter < m_PathWidth) m_TurnDiameter = m_PathWidth;
  	m_Bounds = PatchBounds;
  
  /*Normalize the m_Bounds rectangle*/
  	m_Bounds.Normalize();
  	SetEntryPointAndHeadings();
  	CalculateVacuumPaths();
  }
  
  void CDlgCreateVacuumPatch::SetEntryPointAndHeadings()
  /*******************************************************************************
  //Created: 09/25/97 S.R.
  //Last Revision: 09/25/97 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		Given the side the robot will enter on, this function finds m_EntryPoint,
  //	m_HeadForward and m_HeadNext.
  //Method:
  //
  *******************************************************************************/
  {
  	robPOINT tEntryPoint, tExitPoint;
  	switch (m_EnterSide) {
  	case esTopLft:
  		tEntryPoint.x = m_Bounds.left + cRRWheelBase/2.0f;
  		tEntryPoint.y = m_Bounds.top;
  		tExitPoint.x = m_Bounds.right - cRRWheelBase/2.0f;
  		tExitPoint.y = m_Bounds.bottom;
  		m_HeadForward = -pi/2;
  		m_HeadNext = 0;
  		break;
  	case esTopRgt:
  		tEntryPoint.x = m_Bounds.right - cRRWheelBase/2.0f;
  		tEntryPoint.y = m_Bounds.top;
  		tExitPoint.x = m_Bounds.left + cRRWheelBase/2.0f;
  		tExitPoint.y = m_Bounds.bottom;
  		m_HeadForward = -pi/2;
  		m_HeadNext = pi;
  		break;
  	case esBotLft:
  		tEntryPoint.x = m_Bounds.left + cRRWheelBase/2.0f;
  		tEntryPoint.y = m_Bounds.bottom;
  		tExitPoint.x = m_Bounds.right - cRRWheelBase/2.0f;
  		tExitPoint.y = m_Bounds.top;
  		m_HeadForward = pi/2;
  		m_HeadNext = 0;
  		break;
  	case esBotRgt:
  		tEntryPoint.x = m_Bounds.right - cRRWheelBase/2.0f;
  		tEntryPoint.y = m_Bounds.bottom;
  		tExitPoint.x = m_Bounds.left + cRRWheelBase/2.0f;
  		tExitPoint.y = m_Bounds.top;
  		m_HeadForward = pi/2;
  		m_HeadNext = pi;
  		break;
  	case esLftTop:
  		tEntryPoint.x = m_Bounds.left;
  		tEntryPoint.y = m_Bounds.top - cRRWheelBase/2.0f;
  		tExitPoint.x = m_Bounds.right;
  		tExitPoint.y = m_Bounds.bottom + cRRWheelBase/2.0f;
  		m_HeadForward = 0;
  		m_HeadNext = -pi/2;
  		break;
  	case esLftBot:
  		tEntryPoint.x = m_Bounds.left;
  		tEntryPoint.y = m_Bounds.bottom + cRRWheelBase/2.0f;
  		tExitPoint.x = m_Bounds.right;
  		tExitPoint.y = m_Bounds.top - cRRWheelBase/2.0f;
  		m_HeadForward = 0;
  		m_HeadNext = pi/2;
  		break;
  	case esRgtTop:
  		tEntryPoint.x = m_Bounds.right;
  		tEntryPoint.y = m_Bounds.top - cRRWheelBase/2.0f;
  		tExitPoint.x = m_Bounds.left;
  		tExitPoint.y = m_Bounds.bottom + cRRWheelBase/2.0f;
  		m_HeadForward = pi;
  		m_HeadNext = -pi/2;
  		break;
  	case esRgtBot:
  		tEntryPoint.x = m_Bounds.right;
  		tEntryPoint.y = m_Bounds.bottom + cRRWheelBase/2.0f;
  		tExitPoint.x = m_Bounds.left;
  		tExitPoint.y = m_Bounds.top - cRRWheelBase/2.0f;
  		m_HeadForward = pi;
  		m_HeadNext = pi/2;
  		break;
  	}
  	m_EntryPoint = tEntryPoint;
  }
  
  void CDlgCreateVacuumPatch::UpdateVacuumPatch()
  {
  	UpdateData(TRUE);
  	CRect Rect;
  	m_staVacuumPatchBounds.GetClientRect(&Rect);
  	m_Bounds.left = (-Rect.Width()/2)/m_ZoomLevel * cWorldGridPointSize;
  	m_Bounds.right = (Rect.Width()/2)/m_ZoomLevel * cWorldGridPointSize;
  	m_Bounds.top = (Rect.Height()/2)/m_ZoomLevel * cWorldGridPointSize;
  	m_Bounds.bottom = (-Rect.Height()/2)/m_ZoomLevel * cWorldGridPointSize;
  	m_PathWidth = GetPathWidth();
  	m_TurnDiameter = GetTurnDiameter();
  	PreCreateVacuumPatch(m_Bounds, m_EnterSide, m_PathWidth, m_TurnDiameter);
  //	CalculateVacuumPaths();
  	RedrawWindow();
  }
  
  void CDlgCreateVacuumPatch::OnPaint() 
  {
  	CPaintDC dc(this); // device context for painting
  	
  	DrawVacuumPatch();	
  	// Do not call CDialog::OnPaint() for painting messages
  }
  
  void CDlgCreateVacuumPatch::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
  {
  	const int cLineInc = 10;
  	CRect ViewRect;
  	GetClientRect(ViewRect);
  	int cPageInc = 20;
  //	static UINT OldPos = 0;
  	int Min, Max;
  
  	int OldScrollPos = GetScrollPos(SB_VERT);
  
  	switch (nSBCode) {
  	case SB_LEFT:
  		GetScrollRange(SB_VERT, &Min, &Max);
  		nPos = Min;
  		break;
  	case SB_ENDSCROLL:
  		nPos = OldScrollPos;
  		break;
  	case SB_LINELEFT:
  		nPos = GetScrollPos(SB_VERT);
  		if (nPos <= cLineInc) nPos = 0;
  		else nPos -= cLineInc;
  		GetScrollRange(SB_VERT, &Min, &Max);
  		if (nPos < (UINT)Min) nPos = Min;
  		break;
  	case SB_LINERIGHT:
  		nPos = GetScrollPos(SB_VERT);
  		nPos += cLineInc;
  		GetScrollRange(SB_VERT, &Min, &Max);
  		if (nPos > (UINT)Max) nPos = Max;
  		break;
  	case SB_PAGELEFT:
  		nPos = GetScrollPos(SB_VERT);
  		nPos -= cPageInc;
  		GetScrollRange(SB_VERT, &Min, &Max);
  		if (nPos < (UINT)Min) nPos = Min;
  		break;
  	case SB_PAGERIGHT:
  		nPos = GetScrollPos(SB_VERT);
  		nPos += cPageInc;
  		GetScrollRange(SB_VERT, &Min, &Max);
  		if (nPos > (UINT)Max) nPos = Max;
  		break;
  	case SB_RIGHT:
  		GetScrollRange(SB_VERT, &Min, &Max);
  		nPos = Max;
  		break;
  	case SB_THUMBPOSITION:
  		break;
  	case SB_THUMBTRACK:
  		break;
  	}
  	ScrollWindow(0, (OldScrollPos - nPos), NULL, NULL);
  	SetScrollPos(SB_VERT, nPos, TRUE);
  	UpdateWindow();
  	
  	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
  }
  
  void CDlgCreateVacuumPatch::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
  {
  	const int cLineInc = 10;
  	CRect ViewRect;
  	GetClientRect(ViewRect);
  	int cPageInc = 20;
  //	static UINT OldPos = 0;
  	int Min, Max;
  
  	int OldHScrollPos = GetScrollPos(SB_HORZ);
  
  	switch (nSBCode) {
  	case SB_LEFT:
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		nPos = Min;
  		break;
  	case SB_ENDSCROLL:
  		nPos = OldHScrollPos;
  		break;
  	case SB_LINELEFT:
  		nPos = GetScrollPos(SB_HORZ);
  		if (nPos <= cLineInc) nPos = 0;
  		else nPos -= cLineInc;
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		if (nPos < (UINT)Min) nPos = Min;
  		break;
  	case SB_LINERIGHT:
  		nPos = GetScrollPos(SB_HORZ);
  		nPos += cLineInc;
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		if (nPos > (UINT)Max) nPos = Max;
  		break;
  	case SB_PAGELEFT:
  		nPos = GetScrollPos(SB_HORZ);
  		nPos -= cPageInc;
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		if (nPos < (UINT)Min) nPos = Min;
  		break;
  	case SB_PAGERIGHT:
  		nPos = GetScrollPos(SB_HORZ);
  		nPos += cPageInc;
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		if (nPos > (UINT)Max) nPos = Max;
  		break;
  	case SB_RIGHT:
  		GetScrollRange(SB_HORZ, &Min, &Max);
  		nPos = Max;
  		break;
  	case SB_THUMBPOSITION:
  		break;
  	case SB_THUMBTRACK:
  		break;
  	}
  	ScrollWindow((OldHScrollPos - nPos), 0, NULL, NULL);
  	SetScrollPos(SB_HORZ, nPos, TRUE);
  	UpdateWindow();
  	
  	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
  }
  
  void CDlgCreateVacuumPatch::OnSize(UINT nType, int cx, int cy) 
  {
  	CDialog::OnSize(nType, cx, cy);
  
  	int DialogWidth;
  	CWnd* CntrlWnd = GetDlgItem(IDC_RADRGTBOT);
  	if (CntrlWnd != NULL) {
  		CRect CntrlRect;
  		CntrlWnd->GetWindowRect(&CntrlRect);
  		ScreenToClient(&CntrlRect);
  		DialogWidth = CntrlRect.right + GetScrollPos(SB_HORZ);
  
  		int DialogHeight;
  		CntrlWnd = GetDlgItem(IDOK);
  		if (CntrlWnd != NULL) {
  			CntrlWnd->GetWindowRect(&CntrlRect);
  			ScreenToClient(&CntrlRect);
  			DialogHeight = CntrlRect.bottom + CntrlRect.Height() + GetScrollPos(SB_VERT);
  
  			GetClientRect(&CntrlRect);
  		/*Do Width Stuff*/
  			if (CntrlRect.Width() + GetScrollPos(SB_HORZ) > DialogWidth) {
  				if (CntrlRect.Width() >= DialogWidth) {
  					ScrollWindow(GetScrollPos(SB_HORZ), 0);
  					SetScrollPos(SB_HORZ, 0);
  					ShowScrollBar(SB_HORZ, FALSE);
  				} else {
  					ScrollWindow((CntrlRect.Width() + GetScrollPos(SB_HORZ)) - DialogWidth, 0);
  					SetScrollPos(SB_HORZ, DialogWidth - CntrlRect.Width());
  				}
  			}
  			if (CntrlRect.Width() < DialogWidth) {
  				ShowScrollBar(SB_HORZ, TRUE);
  				SetScrollRange(SB_HORZ, 0, DialogWidth - CntrlRect.Width(), TRUE);
  			} else {
  				ShowScrollBar(SB_HORZ, FALSE);
  			}
  		/*Do Height Stuff*/
  			if (CntrlRect.Height() + GetScrollPos(SB_VERT) > DialogHeight) {
  				if (CntrlRect.Height() >= DialogHeight) {
  					ScrollWindow(0, GetScrollPos(SB_VERT));
  					SetScrollPos(SB_VERT, 0);
  					ShowScrollBar(SB_VERT, FALSE);
  				} else {
  					ScrollWindow(0, (CntrlRect.Height() + GetScrollPos(SB_VERT)) - DialogHeight);
  					SetScrollPos(SB_VERT, DialogHeight - CntrlRect.Height());
  				}
  			}
  			if (CntrlRect.Height() < DialogHeight) {
  				ShowScrollBar(SB_VERT, TRUE);
  				SetScrollRange(SB_VERT, 0, DialogHeight - CntrlRect.Height(), TRUE);
  			} else {
  				ShowScrollBar(SB_VERT, FALSE);
  			}
  		}
  	}
  }
