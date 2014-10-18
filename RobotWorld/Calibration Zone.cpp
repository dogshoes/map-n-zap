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
  /*******************************************************************************
  //Label.cpp
  //Created: 12/02/96 S.R.
  ********************************************************************************
  //Description:
  //This file contains the functions for handling calibration zones.
  *******************************************************************************/
  #include "stdafx.h"
  #include <math.h>
  #include "global.h"
  #include "RobotWorld Resource.h"
  #include "RRDim.h"
  #include "RothMath.h"
  #include "Calibration Zone.h"
  #include "Probability Grid.h"
  #include "DlgEditCalibrationZone.h"
  #include "CTDPath.h"
  #include "DlgErrorCheckingWoodWithVacuum.h"
  #include "DlgCreateCheckPointSlipperySurface.h"
  #include "HomeBase.h"
  #include "RobotWorldView.h"
  
  IMPLEMENT_SERIAL(CCalibrationZone, CExecutableRWGraphicObject, VERSIONABLE_SCHEMA | VERSION_NUMBER)
  IMPLEMENT_SERIAL(CDoorway, CCalibrationZone, VERSIONABLE_SCHEMA | VERSION_NUMBER)
  IMPLEMENT_SERIAL(CCornerWall, CCalibrationZone, VERSIONABLE_SCHEMA | VERSION_NUMBER)
  IMPLEMENT_SERIAL(CWall, CCalibrationZone, VERSIONABLE_SCHEMA | VERSION_NUMBER)
  
  const int cZoom3CalZoneCircleBmpWidth = 16;
  const int cZoom3CalZoneCircleBmpHeight = 16;
  const int cZoom2CalZoneCircleBmpWidth = 8;
  const int cZoom2CalZoneCircleBmpHeight = 8;
  const int cZoom1CalZoneCircleBmpWidth = 4;
  const int cZoom1CalZoneCircleBmpHeight = 4;
  
  /*const int cZoom3CalZoneArrowBmpWidth = 40;
  const int cZoom3CalZoneArrowBmpHeight = 40;
  const int cZoom2CalZoneArrowBmpWidth = 20;
  const int cZoom2CalZoneArrowBmpHeight = 20;
  const int cZoom1CalZoneArrowBmpWidth = 10;
  const int cZoom1CalZoneArrowBmpHeight = 10;
  */
  const int cZoom3CalZoneArrowBmpWidth = 31;
  const int cZoom3CalZoneArrowBmpHeight = 31;
  const int cZoom2CalZoneArrowBmpWidth = 15;
  const int cZoom2CalZoneArrowBmpHeight = 15;
  const int cZoom1CalZoneArrowBmpWidth = 7;
  const int cZoom1CalZoneArrowBmpHeight = 7;
  
  const int cArrowDist = -10;	/*How far back the calibration zone's arrow is offset*/
  //const int ArrowBmpFramePerLine = 6;
  //const int ArrowBmpTotalFrames = 36;
  //const unsigned long cTransparencyColor = 0xFF;
  
  const int ArrowBmpFramePerLine = 5;
  const int ArrowBmpTotalFrames = 40;
  const unsigned long cTransparencyColor = 0xFFFFFF;
  
  const int cThirdCircleDistance = 120;
  
  LPDIRECTDRAWSURFACE3 CCalibrationZone::m_lpDDSCalibrationZoneBmp = NULL;
  LPDIRECTDRAWSURFACE3 CCalibrationZone::m_lpDDSArrowBmp = NULL;
  float CCalibrationZone::m_OldZoomLevel = cZoomLevel3;
  float CCornerWall::m_OldGreyArrowZoomLevel = cZoomLevel3;
  LPDIRECTDRAWSURFACE3 CCornerWall::m_lpDDSGreyArrowBmp = NULL;
  /*******************************************************************************
  //Created: 07/17/97 S.R.
  //Last Revision: 07/17/97 S.R.
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
  
  //#pragma warning( once : 4244 )        //Issue warning 4244 only once.
  
  CCalibrationZone::CCalibrationZone(robPOINT P0, robPOINT P1, double Heading, TCalibrationZone ZoneType) : CExecutableRWGraphicObject()
  /*******************************************************************************
  //Created: 12/02/96 S.R.
  //Last Revision: 07/16/97 S.R.
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
  	m_FloorType = ftThinCarpet;
  	m_P0 = P0;
  	m_P1 = P1;
  	m_Heading = BringAngleInRange(Heading);
  //	m_ZoneType = ZoneType;
  
  	CalculateBounds();
  }
  
  CCalibrationZone::~CCalibrationZone()
  /*******************************************************************************
  //Created: 12/02/96 S.R.
  //Last Revision: 12/02/96 S.R.
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
  	if (m_lpDDSCalibrationZoneBmp != NULL) {
  		m_lpDDSCalibrationZoneBmp->Release();
  		m_lpDDSCalibrationZoneBmp = NULL;
  	}
  	if (m_lpDDSArrowBmp != NULL) {
  		m_lpDDSArrowBmp->Release();
  		m_lpDDSArrowBmp = NULL;
  	}
  }
  
  
  
  
  /*
  void CCalibrationZone::Test(void)
  {
  	robPOINT P0;
  	P0.x = 0;
  	P0.y = 0;
  	robPOINT P1;
  	P1.x = 10*cTPI;
  	P1.y = 10*cTPI;
  	double Heading = 3*pi/4;
  	CCalibrationZone* CalZone = new CCalibrationZone(P0, P1, Heading, czWall);
  	CalZone->m_P0.x = 0;
  	CalZone->m_P0.y = 0;
  	CalZone->m_P1.x = 10;
  	CalZone->m_P1.y = 10;
  	CalZone->m_Heading = 3*pi/4;
  	robPOINT Position;
  	Position.x = 2.1213;
  	Position.y = -2.1213;
  //true
  	bool result = CalZone->InCalibrationZone(3, Position);
  	
  	Position.x = -2.1213;
  	Position.y = -2.1213;
  //true
  	result = CalZone->InCalibrationZone(3, Position);
  
  	Position.x = 5;
  	Position.y = 5;
  //true
  	result = CalZone->InCalibrationZone(3, Position);
  
  	Position.x = 5.4;
  	Position.y = 5;
  //true
  	result = CalZone->InCalibrationZone(3, Position);
  	
  	Position.x = 2.2;
  	Position.y = -2.2;
  //false
  	result = CalZone->InCalibrationZone(3, Position);
  
  	Position.x = 2.2;
  	Position.y = -2.12;
  //false
  	result = CalZone->InCalibrationZone(3, Position);
  
  	Position.x = 4.5;
  	Position.y = 5;
  //false
  	result = CalZone->InCalibrationZone(3, Position);
  
  	delete CalZone;
  }
  */
  
  
  CCalibrationZone::CCalibrationZone() : CExecutableRWGraphicObject()
  /*******************************************************************************
  //Created: 07/17/97 S.R.
  //Last Revision: 07/17/97 S.R.
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
  	m_FloorType = ftThinCarpet;
  	m_Heading = 0;
  	m_P0.x = m_P0.y = 0;
  	m_P1.x = m_P1.y = 0;
  //	m_ZoneType = czWall;
  }
  
  void CCalibrationZone::Serialize(CArchive & archive)
  /*******************************************************************************
  //Created: 07/17/97 S.R.
  //Last Revision: 07/17/97 S.R.
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
  	DWORD temp;
  	CExecutableRWGraphicObject::Serialize(archive);
  	if (archive.IsStoring()) {
  		archive << (DWORD)m_FloorType;
  		archive << m_Heading;
  		archive << m_P0.x << m_P0.y;
  		archive << m_P1.x << m_P1.y;
  		archive << (DWORD)1;
  	} else {
  //		UINT VersionNumber = archive.GetObjectSchema();
  		if (GetArchiveVersionNumber() >= 2) {
  			archive >> temp;
  			m_FloorType = (TFloorType)temp;
  		}
  		archive >> m_Heading;
  		archive >> m_P0.x >> m_P0.y;
  		archive >> m_P1.x >> m_P1.y;
  		archive >> temp;//, m_ZoneType = (TCalibrationZone)temp;
  		CalculateBounds();
  	}
  }
  
  CCornerWall::CCornerWall(robPOINT P0, robPOINT P2, double H0, double H2) : CWall(P0, H0)
  /*******************************************************************************
  //Created: 11/25/97 S.R.
  //Last Revision: 11/25/97 S.R.
  //Parameters:
  //	P0, P1, P2:	Position of robot when feeling different parts of wall.
  //	H0, H1, H2:	Headings " ".
  //Return Value:
  //
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	double phi;
  	m_P2.x = P2.x + cRRWidth/2 * cos(m_Heading + pi) + cRRNoseLength * cos(H2);
  	m_P2.y = P2.y + cRRWidth/2 * sin(m_Heading + pi) + cRRNoseLength * sin(H2);
  	m_Heading2 = H2;
  	phi = m_Heading2;
  	if ( AbsAngularSeparation(phi, m_Heading - pi/2) < AbsAngularSeparation(phi, m_Heading + pi/2) ) {
  		phi = m_Heading - pi/2;
  		m_FeelingMotor = 1;
  	} else {
  		phi = m_Heading + pi/2;
  		m_FeelingMotor = 0;
  	}
  	if (m_FeelingMotor == 0) {
  		m_CalPoint3.x = P2.x + cRRWidth/2 * cos(H2 - pi/2) + cRRNoseLength * cos(H2);
  		m_CalPoint3.y = P2.y + cRRWidth/2 * sin(H2 - pi/2) + cRRNoseLength * sin(H2);
  	} else {
  		m_CalPoint3.x = P2.x + cRRWidth/2 * cos(H2 + pi/2) + cRRNoseLength * cos(H2);
  		m_CalPoint3.y = P2.y + cRRWidth/2 * sin(H2 + pi/2) + cRRNoseLength * sin(H2);
  	}
  	m_Heading2 = phi;
  //	m_ZoneType = czCornerWall;
  /*Move points P1 and P2 around to form a nice corner*/
  //	TLine L1;
  //	FindLineFrom2Points(P0,P1,&L1);
  //	robPOINT Intersection = FindPerpendicularIntersection(L1,m_P2);
  //	if ( Distance(Intersection,m_P0) < Distance(Intersection,m_P1) ) {
  //		m_P0 = Intersection;
  //	} else {
  //		m_P1 = Intersection;
  //	}
  	m_WallBuffer = cDefaultWallBuffer;
  	CalculateBounds();
  }
  
  CWall::CWall(robPOINT P0, double H0) : CCalibrationZone()
  /*******************************************************************************
  //Created: 12/03/97 S.R.
  //Last Revision: 12/03/97 S.R.
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
  //	CWall::CWall();
  	double phi;
  /*Calculate the heading for the wall*/
  	m_Heading = BringAngleInRange(H0);
  /*Find the corners of the calibration wall given the 2 robot positions*/
  	phi = m_Heading + pi/2;
  	m_P0 = P0;
  	m_P1 = P0;
  	m_P1.x += cRRWidth/2 * cos(phi) + cRRNoseLength * cos(m_Heading);
  	m_P1.y += cRRWidth/2 * sin(phi) + cRRNoseLength * sin(m_Heading);
  	m_P0.x += -cRRWidth/2 * cos(phi) + cRRNoseLength * cos(m_Heading);
  	m_P0.y += -cRRWidth/2 * sin(phi) + cRRNoseLength * sin(m_Heading);
  //	m_ZoneType = czWall;
  	CalculateBounds();
  }
  
  CDoorway::CDoorway(robPOINT P0, robPOINT P1, double H0, double H1) : CCalibrationZone()
  /*******************************************************************************
  //Created: 12/03/97 S.R.
  //Last Revision: 12/03/97 S.R.
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
  	double H, phi;
  	phi = atan2(P1.y - P0.y, P1.x - P0.x);
  	H = AngularBisector(H0,H1);
  	if ( AbsAngularSeparation(H,phi + pi/2) < AbsAngularSeparation(H, phi-pi/2) ) {
  		H = BringAngleInRange(phi + pi/2);
  	} else {
  		H = BringAngleInRange(phi - pi/2);
  	}
  	m_P0 = P0;
  	m_P1 = P1;
  	m_Heading = H;
  //	m_ZoneType = czDoorway;
  	CalculateBounds();
  }
  
  void CCornerWall::Serialize(CArchive & archive)
  {
  	CWall::Serialize(archive);
  	if (archive.IsStoring()) {
  		archive << m_Heading2;
  		archive << m_P2.x << m_P2.y;
  		archive << m_CalPoint3.x << m_CalPoint3.y;
  		archive << m_FeelingMotor;
  		archive << m_WallBuffer;
  	} else {
  		archive >> m_Heading2;
  		archive >> m_P2.x >> m_P2.y;
  		archive >> m_CalPoint3.x >> m_CalPoint3.y;
  		archive >> m_FeelingMotor;
  		if (m_ArchiveVersionNumber >= 7) {
  			archive >> m_WallBuffer;
  		}
  		CalculateBounds();
  	}
  }
  
  CDoorway::CDoorway() : CCalibrationZone()
  {
  }
  
  CCornerWall::CCornerWall() : CWall()
  {
  	m_Heading2 = pi;
  	m_P2.x = m_P2.y = 0;
  	m_WallBuffer = cDefaultWallBuffer;
  }
  
  CWall::CWall() : CCalibrationZone()
  {
  }
  
  robPOINT CDoorway::GetEntryPoint()
  {
  	robPOINT Start;
  	Start.x = (m_P0.x + m_P1.x) / 2.0;
  	Start.y = (m_P0.y + m_P1.y) / 2.0;
  	return Start;
  }
  
  robPOINT CCalibrationZone::GetEntryPoint()
  {
  	robPOINT Start;
  	Start.x = (m_P0.x + m_P1.x) / 2.0 - 20*cTPI*cos(m_Heading);
  	Start.y = (m_P0.y + m_P1.y) / 2.0 - 20*cTPI*sin(m_Heading);
  //	Start.x = (m_P0.x + m_P1.x) / 2.0 - 2*cRRWidth*cos(m_Heading);
  //	Start.y = (m_P0.y + m_P1.y) / 2.0 - 2*cRRWidth*sin(m_Heading);
  //	Start.x = (m_P0.x + m_P1.x) / 2.0 - cRRWidth*cos(m_Heading);
  //	Start.y = (m_P0.y + m_P1.y) / 2.0 - cRRWidth*sin(m_Heading);
  	return Start;
  }
  
  robPOINT CCalibrationZone::GetExitPoint()
  {
  	robPOINT Start;
  //	Start.x = (m_P0.x + m_P1.x) / 2.0 - 2*cRRWidth*cos(m_Heading);
  //	Start.y = (m_P0.y + m_P1.y) / 2.0 - 2*cRRWidth*sin(m_Heading);
  	Start.x = (m_P0.x + m_P1.x) / 2.0 - cRRWidth*cos(m_Heading);
  	Start.y = (m_P0.y + m_P1.y) / 2.0 - cRRWidth*sin(m_Heading);
  	return Start;
  }
  
  void CCalibrationZone::DrawObject(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid)
  {
  	CDC* pDC = CProbabilityGrid::DDrawGetSurfaceDC(BackBuffer);
  	Draw(ProbabilityGridUpdateRect, pDC, ProbGrid);
  	CProbabilityGrid::DDrawReleaseSurfaceDC(pDC, BackBuffer);
  
  	Draw(ProbabilityGridUpdateRect, BackBuffer, ProbGrid);
  	return;
  }
  
  void CCalibrationZone::Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbGrid)
  {
  	if (!m_IsVisible) return;
  	CExecutableRWGraphicObject::Draw(ProbGridBmpUpdateRect, pDC, ProbGrid);
  
  	CPoint SurfaceOffset = ProbGridBmpUpdateRect.TopLeft();
  
  	int CalZoneCircleBmpWidth, CalZoneCircleBmpHeight;
  	if (ProbGrid->m_ZoomLevel == cZoomLevel1) {
  		CalZoneCircleBmpWidth = cZoom1CalZoneCircleBmpWidth;
  		CalZoneCircleBmpHeight = cZoom1CalZoneCircleBmpHeight;
  	} else if (ProbGrid->m_ZoomLevel == cZoomLevel2) {
  		CalZoneCircleBmpWidth = cZoom2CalZoneCircleBmpWidth;
  		CalZoneCircleBmpHeight = cZoom2CalZoneCircleBmpHeight;
  	} else if (ProbGrid->m_ZoomLevel == cZoomLevel3) {
  		CalZoneCircleBmpWidth = cZoom3CalZoneCircleBmpWidth;
  		CalZoneCircleBmpHeight = cZoom3CalZoneCircleBmpHeight;
  	} else {
  		ASSERT(FALSE);
  		CalZoneCircleBmpWidth = cZoom1CalZoneCircleBmpWidth;
  		CalZoneCircleBmpHeight = cZoom1CalZoneCircleBmpHeight;
  	}
  	robPOINT point;
  	point = m_P0;
  	point.x += ProbGrid->ClientSizeToRobotSize(CalZoneCircleBmpWidth/2) * cos(m_Heading);
  	point.y += ProbGrid->ClientSizeToRobotSize(CalZoneCircleBmpWidth/2) * sin(m_Heading);
  	CPoint P1 = ProbGrid->RobotToDDSurface(point, SurfaceOffset);
  	point = m_P1;
  	point.x += ProbGrid->ClientSizeToRobotSize(CalZoneCircleBmpWidth/2) * cos(m_Heading);
  	point.y += ProbGrid->ClientSizeToRobotSize(CalZoneCircleBmpWidth/2) * sin(m_Heading);
  	CPoint P2 = ProbGrid->RobotToDDSurface(point, SurfaceOffset);
  	double theta = atan2((double)(P2.y - P1.y), (double)(P2.x - P1.x));
  	P1.x -= long(CalZoneCircleBmpWidth * cos(theta));
  	P1.y -= long(CalZoneCircleBmpWidth * sin(theta));
  	P2.x += long(CalZoneCircleBmpWidth * cos(theta));
  	P2.y += long(CalZoneCircleBmpWidth * sin(theta));
  
  	POINT WallP[5];
  	
  	theta += pi/2;
  	int c = Round(CalZoneCircleBmpWidth / 2 * cos(theta));
  	int s = Round(CalZoneCircleBmpWidth / 2 * sin(theta));
  	WallP[0].x = P1.x - c;
  	WallP[0].y = P1.y - s;
  	WallP[1].x = P1.x + c;
  	WallP[1].y = P1.y + s;
  	WallP[2].x = P2.x + c;
  	WallP[2].y = P2.y + s;
  	WallP[3].x = P2.x - c;
  	WallP[3].y = P2.y - s;
  	WallP[4] = WallP[0];
  
  	CGdiObject* OldBrush = pDC->SelectStockObject(BLACK_BRUSH);
  	CGdiObject* OldPen = pDC->SelectStockObject(WHITE_PEN);
  	int OldPolyFillMode = pDC->SetPolyFillMode(ALTERNATE);
  	pDC->Polygon(WallP, 5);
  	pDC->SetPolyFillMode(OldPolyFillMode);
  	pDC->SelectObject(OldPen);
  	pDC->SelectObject(OldBrush);
  
  	return;
  }
  
  void CCalibrationZone::Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid)
  {
  	if (!m_IsVisible) return;
  	CRect rcRect, destRect;
  	int Frame;
  	int CalZoneCircleBmpWidth, CalZoneCircleBmpHeight;
  	HRESULT ddrval;
  
  /*Load Calibration Zone Bitmap*/
  	if (m_lpDDSCalibrationZoneBmp == NULL) {
  		m_lpDDSCalibrationZoneBmp = ProbGrid->DDrawLoadBitmap(IDB_CALIBRATIONZONE);
  		if( m_lpDDSCalibrationZoneBmp == NULL ) {
  			fprintf(stderr, "CalibrationZone Draw Error!!!! DDrawLoadBitmap failed.  Did not load IDB_CALIBRATIONZONE\n");
  		}
  	}
  
  	if (m_lpDDSCalibrationZoneBmp->IsLost() == DDERR_SURFACELOST) {
  		m_lpDDSCalibrationZoneBmp->Restore();
  	}
  
  
  /*Load Largest bitmap initially*/
  	if (m_lpDDSArrowBmp == NULL) {
  		m_lpDDSArrowBmp = ProbGrid->DDrawLoadBitmap(IDB_LARGE_BLACK_ARROW);
  		if( m_lpDDSArrowBmp == NULL ) {
  			fprintf(stderr, "PositionLabel Draw Error!!!! DDrawLoadBitmap failed.  Did not load IDB_LARGE_BLACK_ARROW\n");
  		}
  		m_OldZoomLevel = cZoomLevel3;
  	}
  	if (ProbGrid->m_ZoomLevel != m_OldZoomLevel) {
  		HRESULT reloadresult;
  		if (ProbGrid->m_ZoomLevel == cZoomLevel3) {
  			reloadresult = ProbGrid->DDrawReloadBitmap(m_lpDDSArrowBmp, IDB_LARGE_BLACK_ARROW);
  		} else if (ProbGrid->m_ZoomLevel == cZoomLevel2) {
  			reloadresult = ProbGrid->DDrawReloadBitmap(m_lpDDSArrowBmp, IDB_MEDIUM_BLACK_ARROW);
  		} else {
  			reloadresult = ProbGrid->DDrawReloadBitmap(m_lpDDSArrowBmp, IDB_SMALL_BLACK_ARROW);
  		}
  		if (reloadresult != DD_OK) {
  			fprintf(stderr, "CalibrationZone Draw Error!!!! DDrawReLoadBitmap failed.  Did not load IDB_x_BLACK_ARROWx\n");
  			ProbGrid->DDrawDiagnostic(reloadresult);
  		}
  	}
  	if (m_lpDDSArrowBmp->IsLost() == DDERR_SURFACELOST) {
  		m_lpDDSArrowBmp->Restore();
  	}
  	
  	m_OldZoomLevel = ProbGrid->m_ZoomLevel;
  	
  	if (ProbGrid->m_ZoomLevel == cZoomLevel1) {
  		CalZoneCircleBmpWidth = cZoom1CalZoneCircleBmpWidth;
  		CalZoneCircleBmpHeight = cZoom1CalZoneCircleBmpHeight;
  		rcRect.left = 0;
  		rcRect.top = 0;
  		rcRect.right = rcRect.left + CalZoneCircleBmpWidth;
  		rcRect.bottom = rcRect.top + CalZoneCircleBmpHeight;
  	} else if (ProbGrid->m_ZoomLevel == cZoomLevel2) {
  		CalZoneCircleBmpWidth = cZoom2CalZoneCircleBmpWidth;
  		CalZoneCircleBmpHeight = cZoom2CalZoneCircleBmpHeight;
  		rcRect.left = 4;
  		rcRect.top = 0;
  		rcRect.right = rcRect.left + CalZoneCircleBmpWidth;
  		rcRect.bottom = rcRect.top + CalZoneCircleBmpHeight;
  	} else if (ProbGrid->m_ZoomLevel == cZoomLevel3) {
  		CalZoneCircleBmpWidth = cZoom3CalZoneCircleBmpWidth;
  		CalZoneCircleBmpHeight = cZoom3CalZoneCircleBmpHeight;
  		rcRect.left = 12;
  		rcRect.top = 0;
  		rcRect.right = rcRect.left + CalZoneCircleBmpWidth;
  		rcRect.bottom = rcRect.top + CalZoneCircleBmpHeight;
  	} else {
  		ASSERT(FALSE);
  		CalZoneCircleBmpWidth = cZoom1CalZoneCircleBmpWidth;
  		CalZoneCircleBmpHeight = cZoom1CalZoneCircleBmpHeight;
  	}
  	CRect tRect = rcRect;
  /*Draw first corner of zone*/
  	float temp;
  	temp = ProbGrid->RobotToDDSurfaceX(m_P0.x + ProbGrid->ClientSizeToRobotSize(CalZoneCircleBmpWidth/2) * cos(m_Heading), ProbabilityGridUpdateRect.left);
  	temp -= CalZoneCircleBmpWidth/2.0f;
  	destRect.left = Round(temp);
  //		destRect.left = Round((CurrentNode->CalibrationZone->m_P0.x / cWorldGridPointSize + cWorldGridXSize / 2 - ProbabilityGridUpdateRect.left)*m_ZoomLevel - CalZoneCircleBmpWidth/2.0 );
  	destRect.right = destRect.left + CalZoneCircleBmpWidth;
  	temp = ProbGrid->RobotToDDSurfaceY(m_P0.y + ProbGrid->ClientSizeToRobotSize(CalZoneCircleBmpWidth/2) * sin(m_Heading), ProbabilityGridUpdateRect.top);
  	temp -= CalZoneCircleBmpHeight/2.0f;
  	destRect.top = Round(temp);
  //		destRect.top = Round((-CurrentNode->CalibrationZone->m_P0.y / cWorldGridPointSize + cWorldGridYSize / 2 - ProbabilityGridUpdateRect.top)*m_ZoomLevel - CalZoneCircleBmpHeight/2.0 );
  	destRect.bottom = destRect.top + CalZoneCircleBmpHeight;
  	rcRect = tRect;
  	ProbGrid->RectifyRectangles(&rcRect, &destRect);
  	ProbGrid->DDSetColorKey(m_lpDDSCalibrationZoneBmp, cTransparencyColor);
  	if ( (destRect.Height() != 0) && (destRect.Width() != 0) && (rcRect.Height() != 0) && (rcRect.Width() != 0) ) {
  		while (BackBuffer->GetBltStatus(DDGBS_CANBLT) != DD_OK);
  		ddrval = BackBuffer->BltFast(destRect.left, destRect.top, m_lpDDSCalibrationZoneBmp, rcRect, DDBLTFAST_SRCCOLORKEY);
  		if (ddrval != DD_OK) {
  			fprintf(stderr, "Draw Calibration Zones Failed\n");
  			ProbGrid->DDrawDiagnostic(ddrval);
  			fprintf(stderr, "***DrawCalibrationZone Error***\n");
  			fprintf(stderr, "destRect = %d %d %d %d\n",destRect.left, destRect.top, destRect.right, destRect.bottom);
  			fprintf(stderr, "rcRect = %d %d %d %d\n", rcRect.left,rcRect.top, rcRect.right, rcRect.bottom);
  			fflush(stderr);
  		}
  	}
  /*Draw second corner of zone*/
  	temp = ProbGrid->RobotToDDSurfaceX(m_P1.x + ProbGrid->ClientSizeToRobotSize(CalZoneCircleBmpWidth/2) * cos(m_Heading), ProbabilityGridUpdateRect.left);
  	temp -= CalZoneCircleBmpWidth/2.0f;
  	destRect.left = Round(temp);
  //		destRect.left = Round((CurrentNode->m_CalibrationZone->m_P1.x / cWorldGridPointSize + cWorldGridXSize / 2 - ProbabilityGridUpdateRect.left)*m_ZoomLevel - CalZoneCircleBmpWidth/2.0 );
  	destRect.right = destRect.left + CalZoneCircleBmpWidth;
  	temp = ProbGrid->RobotToDDSurfaceY(m_P1.y + ProbGrid->ClientSizeToRobotSize(CalZoneCircleBmpWidth/2) * sin(m_Heading), ProbabilityGridUpdateRect.top);
  	temp -= CalZoneCircleBmpHeight/2.0f;
  	destRect.top = Round(temp);
  //		destRect.top = Round((-CurrentNode->m_CalibrationZone->m_P1.y / cWorldGridPointSize + cWorldGridYSize / 2 - ProbabilityGridUpdateRect.top)*m_ZoomLevel - CalZoneCircleBmpHeight/2.0 );
  	destRect.bottom = destRect.top + CalZoneCircleBmpHeight;
  	rcRect = tRect;
  	ProbGrid->RectifyRectangles(&rcRect, &destRect);
  	ProbGrid->DDSetColorKey(m_lpDDSCalibrationZoneBmp, cTransparencyColor);
  	if ( (destRect.Height() != 0) && (destRect.Width() != 0) && (rcRect.Height() != 0) && (rcRect.Width() != 0) ) {
  		while (BackBuffer->GetBltStatus(DDGBS_CANBLT) != DD_OK);
  		ddrval = BackBuffer->BltFast(destRect.left, destRect.top, m_lpDDSCalibrationZoneBmp, rcRect, DDBLTFAST_SRCCOLORKEY);
  		if (ddrval != DD_OK) {
  			fprintf(stderr, "Draw Calibration Zones Failed\n");
  			ProbGrid->DDrawDiagnostic(ddrval);
  			fprintf(stderr, "***DrawCalibrationZone Error***\n");
  			fprintf(stderr, "destRect = %d %d %d %d\n",destRect.left, destRect.top, destRect.right, destRect.bottom);
  			fprintf(stderr, "rcRect = %d %d %d %d\n", rcRect.left,rcRect.top, rcRect.right, rcRect.bottom);
  			fflush(stderr);
  		}
  	}
  /*Draw arrow*/
  //	LPDIRECTDRAWSURFACE lpDDSArrowBmp = NULL;
  	int ArrowBmpFrameWidth;
  	int ArrowBmpFrameHeight;
  	if (ProbGrid->m_ZoomLevel == cZoomLevel1) {
  //		lpDDSArrowBmp = ProbGrid->m_lpDDSArrowBmp1;
  		ArrowBmpFrameWidth = cZoom1CalZoneArrowBmpWidth;
  		ArrowBmpFrameHeight = cZoom1CalZoneArrowBmpHeight;
  	} else if (ProbGrid->m_ZoomLevel == cZoomLevel2) {
  //		lpDDSArrowBmp = ProbGrid->m_lpDDSArrowBmp2;
  		ArrowBmpFrameWidth = cZoom2CalZoneArrowBmpWidth;
  		ArrowBmpFrameHeight = cZoom2CalZoneArrowBmpHeight;
  	} else if (ProbGrid->m_ZoomLevel == cZoomLevel3) {
  //		lpDDSArrowBmp = ProbGrid->m_lpDDSArrowBmp3;
  		ArrowBmpFrameWidth = cZoom3CalZoneArrowBmpWidth;
  		ArrowBmpFrameHeight = cZoom3CalZoneArrowBmpHeight;
  	} else {
  		ASSERT(FALSE);
  		ArrowBmpFrameWidth = cZoom1CalZoneArrowBmpWidth;
  		ArrowBmpFrameHeight = cZoom1CalZoneArrowBmpHeight;
  	}
  
  	double ZoneHeading = m_Heading;
  	Frame = ArrowHeadingToArrowBitmapFrame(ZoneHeading, ArrowBmpTotalFrames);
  	rcRect.left   = Frame%ArrowBmpFramePerLine*ArrowBmpFrameWidth;
  	rcRect.top    = Frame/ArrowBmpFramePerLine*ArrowBmpFrameHeight;
  	rcRect.right  = rcRect.left + ArrowBmpFrameWidth;
  	rcRect.bottom = rcRect.top + ArrowBmpFrameHeight;
  	temp = ProbGrid->RobotToDDSurfaceX((m_P1.x + m_P0.x)/2.0+ cArrowDist*cTPI*cos(ZoneHeading), ProbabilityGridUpdateRect.left);
  	temp -= ArrowBmpFrameWidth/2.0f;
  	destRect.left = Round(temp);
  //		destRect.left = Round((((CurrentNode->CalibrationZone->m_P1.x + CurrentNode->CalibrationZone->m_P0.x)/2.0 + cArrowDist*cTPI*cos(ZoneHeading)) / cWorldGridPointSize + cWorldGridXSize / 2 - ProbabilityGridUpdateRect.left)*m_ZoomLevel - ArrowBmpFrameWidth/2.0 );
  	destRect.right = destRect.left + ArrowBmpFrameWidth;
  	temp = ProbGrid->RobotToDDSurfaceY((m_P1.y + m_P0.y)/2.0 + cArrowDist*cTPI*sin(ZoneHeading), ProbabilityGridUpdateRect.top);
  	temp -= ArrowBmpFrameHeight/2.0f;
  	destRect.top = Round(temp);
  //		destRect.top = Round((-((CurrentNode->CalibrationZone->m_P1.y + CurrentNode->CalibrationZone->m_P0.y)/2.0 + cArrowDist*cTPI*sin(ZoneHeading)) / cWorldGridPointSize + cWorldGridYSize / 2 - ProbabilityGridUpdateRect.top)*m_ZoomLevel - ArrowBmpFrameHeight/2.0 );
  	destRect.bottom = destRect.top + ArrowBmpFrameHeight;
  	ProbGrid->RectifyRectangles(&rcRect, &destRect);
  	ProbGrid->DDSetColorKey(m_lpDDSArrowBmp, cTransparencyColor);
  	if ( (destRect.Height() != 0) && (destRect.Width() != 0) && (rcRect.Height() != 0) && (rcRect.Width() != 0) ) {
  		while (BackBuffer->GetBltStatus(DDGBS_CANBLT) != DD_OK);
  		ddrval = BackBuffer->BltFast(destRect.left, destRect.top, m_lpDDSArrowBmp, rcRect, DDBLTFAST_SRCCOLORKEY);
  		if (ddrval != DD_OK) {
  			fprintf(stderr, "Draw Arrow Failed\n");
  			ProbGrid->DDrawDiagnostic(ddrval);
  			fprintf(stderr, "***DrawCalibrationZone Error***\n");
  			fprintf(stderr, "destRect = %d %d %d %d\n",destRect.left, destRect.top, destRect.right, destRect.bottom);
  			fprintf(stderr, "rcRect = %d %d %d %d\n", rcRect.left,rcRect.top, rcRect.right, rcRect.bottom);
  			fflush(stderr);
  		}
  	}
  }
  
  int CCalibrationZone::ArrowHeadingToArrowBitmapFrame(double Heading, int TotalFrames)
  {
  	return RobotHeadingToBitmapFrame(Heading, TotalFrames);
  /*
  	int Frame;
  	Heading -= pi / TotalFrames;
  	
  	if (Heading <= 0) Frame = 0;
  	else Frame = Heading * (TotalFrames-1) / (2*pi);
  
  	if (Frame > TotalFrames - 1) Frame = 0;
  
  	return Frame;
  */
  /*
  	int Frame;
  	double FrameHeading = Heading - 2*pi/TotalFrames/2 + pi/2;
  	FrameHeading = BringAngleInRange(FrameHeading);
  	if (FrameHeading <= 3*pi/2) {
  		Frame = int(-20/pi * FrameHeading + 30);
  	} else {
  		const float X1 = float(3*pi/2 + 2*pi/TotalFrames);
  		const float X2 = float(2*pi - 2*pi/TotalFrames);
  		const float m = 8 / (X1 - X2);
  		Frame = int(m*FrameHeading - m*X1 + TotalFrames - 1);
  	}
  	if (Frame > TotalFrames - 1) Frame = 0;
  	return Frame;
  */
  }
  
  void CCalibrationZone::SetBounds(robRECT bounds)
  {
  	return;
  }
  
  void CCalibrationZone::CalculateBounds()
  {
  	m_Bounds.top = m_P0.y;
  	m_Bounds.left = m_P0.x;
  	m_Bounds.bottom = m_P1.y;
  	m_Bounds.right = m_P1.x;
  	m_Bounds.Normalize();
  	robPOINT point;
  	point.x = cZoom3CalZoneCircleBmpWidth/cZoomLevel3*cWorldGridPointSize;
  	point.y = cZoom3CalZoneCircleBmpHeight/cZoomLevel3*cWorldGridPointSize;
  	m_Bounds.InflateRect(point.x, point.y);
  	double c = cos(m_Heading);
  	double s = sin(m_Heading);
  	point.x = (m_P1.x + m_P0.x)/2.0 + cArrowDist*cTPI*c;
  	if (c >= 0) {
  		point.x -= cZoom3CalZoneArrowBmpWidth/cZoomLevel3*cWorldGridPointSize/2;
  	} else {
  		point.x += cZoom3CalZoneArrowBmpWidth/cZoomLevel3*cWorldGridPointSize/2;
  	}
  	point.y = (m_P1.y + m_P0.y)/2.0 + cArrowDist*cTPI*s;
  	if (s >= 0) {
  		point.y -= cZoom3CalZoneArrowBmpHeight/cZoomLevel3*cWorldGridPointSize/2;
  	} else {
  		point.y += cZoom3CalZoneArrowBmpHeight/cZoomLevel3*cWorldGridPointSize/2;
  	}
  	m_Bounds.Include(point);
  }
  
  void CCalibrationZone::EditProperties()
  {
  	CDlgEditCalibrationZone EditCalZone;
  	EditCalZone.SetAdjustCheckPoint(false);
  	EditCalZone.m_edtCalZoneName = GetCaption();
  	EditCalZone.m_radFloorType = int(GetFloorType());
  	bool HasCorner = false;
  	HasCorner = IsKindOf(RUNTIME_CLASS(CCornerWall))&& !IsKindOf(RUNTIME_CLASS(CHomeBase));
  	EditCalZone.SetHasCorner(HasCorner);
  	if (HasCorner) {
  		EditCalZone.SetCornerSize(((CCornerWall*)this)->GetWallBuffer() / cTPI);
  	}
  	if (EditCalZone.DoModal() == IDOK) {
  		SetCaption(EditCalZone.m_edtCalZoneName);
  		m_FloorType = TFloorType(EditCalZone.m_radFloorType);
  		if (EditCalZone.GetAdjustCheckPoint()) {
  			if (m_ParentView->GetActiveRobot() != NULL) {
  				Adjust(m_ParentView->GetActiveRobot());
  			}
  		}
  		if (HasCorner) {
  			((CCornerWall*)this)->SetWallBuffer(int(EditCalZone.GetCornerSize() * cTPI));
  		}
  	}
  }
  
  TTaskResult CCalibrationZone::Execute(CRobot* Robot, short Speed)
  {
  	const double cRunwayWidth = 40.0;
  	TRobotInfo RobotInfo = Robot->GetRobotInfo();
  //	bool OldEStopVisible = Robot->GetEStopVisible();
  //	TState OldRobotState = Robot->GetState();
  	Robot->SetState(stExecuting);
  	Robot->SetEStopVisible(true);
  	TTaskResult TaskResult = TR_TaskComplete;
  	Robot->SetStatusBarText("");
  	if (Robot->GetOnHomeBase()) {
  		if ( (TaskResult = Robot->BackupFromWall(true, Speed)) != TR_TaskComplete) {
  			goto CLEANUP;
  		}
  	}
  	if (m_Bounds.Inside(Robot->GetPosition())) {
  	//See if the robot is inside the runway area
  		robPOINT Destination;
  		TLine Runway;
  		Destination.x = (m_P0.x + m_P1.x) / 2;
  		Destination.y = (m_P0.y + m_P1.y) / 2;		
  		FindLineFromPointAndSlope(Destination, tan(m_Heading), &Runway);
  		double DistanceFromRunway = PerpendicularDistanceFromLine(Runway, Robot->GetPosition());
  		if (DistanceFromRunway > cRunwayWidth) {
  			TLine Wall;
  			FindLineFrom2Points(m_P0, m_P1, &Wall);
  			double Distance = PerpendicularDistanceFromLine(Wall, Robot->GetPosition());
  			if (Distance <= cRRWidth/2 + 4*cTPI) {
  				Destination.x = (m_P0.x + m_P1.x) / 2 - (cRRWidth/2 + 4*cTPI) * cos(m_Heading);
  				Destination.y = (m_P0.y + m_P1.y) / 2 - (cRRWidth/2 + 4*cTPI) * sin(m_Heading);
  			} else {
  				Destination.x = (m_P0.x + m_P1.x) / 2 - (Distance) * cos(m_Heading);
  				Destination.y = (m_P0.y + m_P1.y) / 2 - (Distance) * sin(m_Heading);
  			}
  			CCTDPath* Path = new CCTDPath(cComputerControlSpeed);
  			Path->AddPoint(Robot->GetHandlePosition(cFollowCTDPathHandleLength));
  			Path->AddPoint(Destination);
  			Robot->AddCTDPathToInstructions(Path, cMinPositionZone, TRUE);
  			TaskResult = Robot->FollowInstructions(TRUE, FALSE, IR_InstructionComplete);
  			if (TaskResult == TR_TaskComplete) {
  				TaskResult = SpecialFunction(Robot, Speed);
  			}
  		} else {
  			TaskResult = SpecialFunction(Robot, Speed);
  		}
  	} else {
  		TaskResult = ExecutePathToEntryPoint(Robot, Speed);
  		if (TaskResult == TR_TaskComplete) {
  			TaskResult = SpecialFunction(Robot, Speed);
  		}
  //		TaskResult = CExecutableRWGraphicObject::Execute(Robot, Speed);
  
  	}
  CLEANUP:
  	Robot->Stop();
  	Robot->SetRobotInfo(RobotInfo);
  //	Robot->SetState(OldRobotState);
  //	Robot->SetEStopVisible(OldEStopVisible);
  	return TaskResult;
  }
  
  TTaskResult CCalibrationZone::SpecialFunction(CRobot * Robot, short Speed)
  {
  /*Turn off Probability Grid Updating*/
  	TRobotInfo OldRobotInfo = Robot->GetRobotInfo();
  
  //	bool OldUpdateProbabilityGrid = Robot->m_UpdateProbabilityGrid;
  	Robot->SetLearnRoom(false);
  //	short OldMaxSpeed = Robot->GetMaxSpeed();
  /*Show the EStop dialog window*/
  //	bool EStopWasVisible = Robot->GetEStopVisible();
  	Robot->SetEStopVisible(true);
  
  	Robot->PurgeInstructions();
  //	TState OldState = Robot->GetState();
  	Robot->SetState(stCalibrating);
  	TTaskResult CalibrationResult;
  //	TObstacleSensitivity OldSensitivity = Robot->GetObstacleSensitivity();
  //	bool OldFlipHandle = Robot->GetFlipHandle();
  //	unsigned char OldHandleLength = Robot->GetHandleLength();
  	Robot->SetFlipHandle(false);
  	robPOINT DPosition;
  	double DHeading = 0;
  	Robot->SetEnableRepositionOnUnknownObstacle(false);
  	CalibrationResult = Calibrate(Robot, &DPosition, &DHeading);
  	Robot->SetEnableRepositionOnUnknownObstacle(true);
  	if (CalibrationResult == TR_TaskComplete) {
  		Robot->Stop();
  		Robot->ShowCalibrationSuccessfulDialog(DPosition, DHeading);
  //		Robot->SetHandleLength(200);
  /*		robPOINT Backup;
  		Backup.x = Robot->m_Position.x - (20*cTPI + Robot->GetHandleLength()) * cos(Robot->m_Heading);
  		Backup.y = Robot->m_Position.y - (20*cTPI + Robot->GetHandleLength()) * sin(Robot->m_Heading);
  		Robot->AddPositionVelocityToInstructions(Backup, NULL, cLrgPositionZone, cComputerControlSpeed, TRUE, 10000);
  		CalibrationResult = Robot->FollowInstructions(TRUE, TRUE, IR_InstructionComplete);
  		if (CalibrationResult == TR_TaskComplete) {
  			Robot->AddHeadingToInstructions(Robot->HeadingToDestination(Robot->m_Position, GetExitPoint()));
  			CCTDPath* Path = new CCTDPath();
  			Path->AddPoint(Robot->m_Position);
  			Path->AddPoint(GetExitPoint());
  			Robot->AddCTDPathToInstructions(Path);
  			CalibrationResult = Robot->FollowInstructions(TRUE, TRUE, IR_InstructionComplete);
  		}
  */
  		CCTDPath* Path = new CCTDPath(cComputerControlSpeed);
  		Path->AddPoint(Robot->GetHandlePosition(-cFollowCTDPathHandleLength - 20));
  		robPOINT Backup;
  		Backup = Robot->GetHandlePosition(-cFollowCTDPathHandleLength);
  		Backup.x -= (cRRWidth/2 + cRRNoseLength + cFollowCTDPathHandleLength)*cos(Robot->GetHeading());
  		Backup.y -= (cRRWidth/2 + cRRNoseLength + cFollowCTDPathHandleLength)*sin(Robot->GetHeading());
  		Path->AddPoint(Backup);
  		Path->AddPoint(GetExitPoint());
  		Robot->AddCTDPathToInstructions(Path, cModPositionZone, TRUE);
  		CalibrationResult = Robot->FollowInstructions(TRUE, TRUE, IR_InstructionComplete);
  	} else {
  		Robot->DisplayErrorText("Checking In Unsuccessful");
  	}
  	Robot->SetRobotInfo(OldRobotInfo);
  //	Robot->SetObstacleSensitivity(OldSensitivity);
  //	Robot->SetHandleLength(OldHandleLength);
  //	Robot->SetFlipHandle(OldFlipHandle);
  //	Robot->SetState(OldState);
  //	Robot->SetMaxSpeed(OldMaxSpeed);
  //	Robot->SetEStopVisible(EStopWasVisible);
  //	Robot->m_UpdateProbabilityGrid = OldUpdateProbabilityGrid;
  	return CalibrationResult;
  }
  
  TTaskResult CCalibrationZone::Calibrate(CRobot * Robot, robPOINT* DPosition, double* DHeading)
  {
  	DPosition->x = DPosition->y = *DHeading = 0;
  	return TR_TaskComplete;
  }
  
  TTaskResult CWall::Calibrate(CRobot * Robot, robPOINT* DPosition, double* DHeading)
  {
  	robPOINT OldError;
  	if ( (m_FloorType == ftWood) && (Robot->GetAttachmentType() == atVacuum) ) {
  		CDlgErrorCheckingWoodWithVacuum Warning;
  		Warning.DoModal();
  		return TR_Error;
  	}
  	Robot->SetFloorType(m_FloorType);
  
  	robPOINT RobotPosition;
  	TTaskResult TaskResult;
  	TCoordSystem CalCoordSys;
  	robPOINT P;
  	double theta;
  /*Create a coordinate system for the calibration doorway*/
  	CalCoordSys.Origin.x = (m_P0.x + m_P1.x) / 2;
  	CalCoordSys.Origin.y = (m_P0.y + m_P1.y) / 2;
  	CalCoordSys.Phi = m_Heading;
  /*Assume we're already in front of the calibration wall, and just run into it.*/
  	theta = Robot->HeadingToDestination(Robot->m_Position, CalCoordSys.Origin);
  	Robot->SetObstacleSensitivity(osMediumSensitivity);
  
  //	Robot->AddSquareToWallToInstructions(theta);
  	Robot->AddSquareToWallToInstructions(m_Heading, m_FloorType == ftWood);
  	if ( (TaskResult = Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) {
  		goto Error;
  	}
  
  /*Update the position and heading*/
  	double RobotHeading;
  	RobotHeading = Robot->GetTaskResultHeading();
  	RobotPosition = Robot->GetTaskResultPosition();
  
  	TLine L;
  	FindLineFrom2Points(m_P0, m_P1, &L);
  	P = RobotPosition;
  //	P.x = Robot->m_Position.x;
  //	P.y = Robot->m_Position.y;
  	P = FindPerpendicularIntersection(L, P);
  
  	P.x = P.x - cRRNoseLength * cos(m_Heading);
  	P.y = P.y - cRRNoseLength * sin(m_Heading);
  
  /*Set how much the robot's position is corrected*/
  	RobotPosition.x = P.x - RobotPosition.x;
  	RobotPosition.y = P.y - RobotPosition.y;
  	RobotHeading = m_Heading - RobotHeading;
  	*DHeading = *DHeading + RobotHeading;
  	DPosition->x += RobotPosition.x;
  	DPosition->y += RobotPosition.y;
  	
  	OldError = Robot->GetPositionError();
  
  	Robot->AddToPosition(RobotPosition);
  	Robot->AddToHeading(RobotHeading);
  //	Robot->SetHeading(m_Heading);
  //	Robot->SetPosition(P);
  //	Robot->Stop();
  /*Update Errors*/
  	Robot->m_PositionError.x = OldError.x * sin(m_Heading);	/*This correction is not quite right, but close enough*/
  	if (Robot->m_PositionError.x < 0) Robot->m_PositionError.x = -Robot->m_PositionError.x;
  	Robot->m_PositionError.y = OldError.y * cos(m_Heading);	/*This correction is not quite right, but close enough*/
  	if (Robot->m_PositionError.y < 0) Robot->m_PositionError.y = -Robot->m_PositionError.y;
  	Robot->m_HeadingError = 0;
  	return TR_TaskComplete;
  Error:
  	return TaskResult;
  }
  
  TTaskResult CCornerWall::Calibrate(CRobot * Robot, robPOINT* DPosition, double* DHeading)
  /*******************************************************************************
  //Created: S.R.
  //Return Value:
  //	The result of the calibration task.
  ********************************************************************************
  //Description:
  //		Given a calibration zone that is a corner calibration wall, the function will
  //	calibrate the robot against it.  This function assumes the robot is already
  //	in front of the calibration wall.
  //Method:
  //
  *******************************************************************************/
  {
  	if ( (m_FloorType == ftWood) && (Robot->GetAttachmentType() == atVacuum) ) {
  		CDlgErrorCheckingWoodWithVacuum Warning;
  		Warning.DoModal();
  		return TR_Error;
  	}
  	Robot->SetFloorType(m_FloorType);
  
  	TTaskResult TaskResult = CWall::Calibrate(Robot, DPosition, DHeading);
  	if (TaskResult != TR_TaskComplete) {
  		return TaskResult;
  	}
  	Robot->ShowCalibrationSuccessfulDialog(*DPosition, *DHeading);
  	TaskResult = CalibrateCorner(Robot, DPosition, DHeading);
  	if (TaskResult != TR_TaskComplete) {
  		return TaskResult;
  	}
  	Robot->ShowCalibrationSuccessfulDialog(*DPosition, *DHeading);
  /*To reduce calibration error, square to main wall again*/
  	TaskResult = Robot->BackupFromWall();
  	if (TaskResult != TR_TaskComplete) {
  		return TaskResult;
  	}
  	TaskResult = CWall::Calibrate(Robot, DPosition, DHeading);
  	Robot->ShowCalibrationSuccessfulDialog(*DPosition, *DHeading);
  
  //	fprintf(stderr, "Corner Calibration: %5.2f, %5.2f, %6.3f\n", DPosition->x, DPosition->y, *DHeading);
  //	fprintf(stderr, "Corner Calibration: %5.2f, %5.2f, %6.3f\n", DPosition->x, DPosition->y, *DHeading);
  //	fprintf(stderr, "Corner Calibration: %5.2f, %5.2f, %6.3f\n", DPosition->x, DPosition->y, *DHeading);
  //	fprintf(stderr, "Corner Calibration: %5.2f, %5.2f, %6.3f\n", DPosition->x, DPosition->y, *DHeading);
  //	if (TaskResult != TR_TaskComplete) {
  //		return TaskResult;
  //	}
  //	TaskResult = Robot->BackupFromWall();
  	return TaskResult;
  }
  
  TTaskResult CCornerWall::CalibrateCorner(CRobot * Robot, robPOINT* DPosition, double* DHeading)
  /*Does the work for calibrating on the corner*/
  {
  	double phi;
  	phi = m_Heading2;
  
  	robPOINT P;
  	double H0;
  	P.x = (m_P0.x + m_P1.x) / 2;
  	P.y = (m_P0.y + m_P1.y) / 2;
  	H0 = m_Heading;
  //	Robot->SetHandleLength(10);
  //	Robot->SetFlipHandle(true);
  
  	Robot->AddCornerCalibrationToInstructions(P, H0, m_FeelingMotor, m_FloorType == ftWood, m_WallBuffer);
  	TTaskResult TaskResult;
  	if ( (TaskResult = Robot->FollowInstructions(true, false, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TR_Error;
  	}
  
  	robPOINT RobotPosition = Robot->GetTaskResultPosition();
  	double RobotHeading = Robot->GetTaskResultHeading();
  	if (m_FeelingMotor == 0) {
  		P.x = RobotPosition.x + cRRWidth/2 * cos(RobotHeading - pi/2) + cRRNoseLength * cos(RobotHeading);
  		P.y = RobotPosition.y + cRRWidth/2 * sin(RobotHeading - pi/2) + cRRNoseLength * sin(RobotHeading);
  	} else {
  		P.x = RobotPosition.x + cRRWidth/2 * cos(RobotHeading + pi/2) + cRRNoseLength * cos(RobotHeading);
  		P.y = RobotPosition.y + cRRWidth/2 * sin(RobotHeading + pi/2) + cRRNoseLength * sin(RobotHeading);
  	}
  	
  	TLine Wall1;
  	FindLineFrom2Points(m_P0, m_P1, &Wall1);
  	TLine CornerWall;
  	FindPerpendicularLine(Wall1, m_CalPoint3, &CornerWall);
  	robPOINT Intersection = FindPerpendicularIntersection(CornerWall, P);
  	phi = atan2(Intersection.y - P.y, Intersection.x - P.x);
  	double distance;
  	distance = Distance(Intersection, P);
  	P.x = distance * cos(phi);
  	P.y = distance * sin(phi);
  	RobotPosition.x += P.x;
  	RobotPosition.y += P.y;
  	DPosition->x += P.x;
  	DPosition->y += P.y;
  	Robot->AddToPosition(P);
  //	Robot->SetPosition(RobotPosition);
  //	Robot->Stop();
  /*Update Errors*/
  	Robot->m_PositionError.x = 0;
  	Robot->m_PositionError.y = 0;
  	Robot->m_HeadingError = 0;
  	return TR_TaskComplete;
  }
  
  TTaskResult CDoorway::Calibrate(CRobot* Robot, robPOINT* DPosition, double* DHeading)
  /*******************************************************************************
  //Created: 02/27/97 S.R.
  //Last Revision: 01/27/98 S.R.
  //Parameters:
  //	CalibrationZone:	the calibration zone to calibrate against.
  //Return Value:
  //	The result of the calibration task.
  ********************************************************************************
  //Description:
  //		Given a calibration zone that is a calibration doorway, the function will
  //	calibrate the robot against it.  This function assumes the robot is already
  //	in the middle of the doorway.
  //Method:
  //
  *******************************************************************************/
  {
  	TCoordSystem CalCoordSys;
  	TTaskResult TaskResult;
  	robPOINT P, Corner;
  /*Create a coordinate system for the calibration doorway*/
  	CalCoordSys.Origin.x = (m_P0.x + m_P1.x) / 2;
  	CalCoordSys.Origin.y = (m_P0.y + m_P1.y) / 2;
  	CalCoordSys.Phi = m_Heading;
  /*Assume we're in the middle of the doorway, and turn to the left*/
  	Robot->AddHeadingToInstructions(pi/2, &CalCoordSys, 0, cComputerControlHeadingSpeed, 0);
  	if ( (TaskResult = Robot->FollowInstructions(TRUE, FALSE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Hit doorway*/
  	Corner.x = 0;
  	Corner.y = Distance(m_P0, m_P1) / 2;
  	P = TransformToDisplacedCoordinates(&CalCoordSys,Robot->m_Position);
  	Robot->AddStraightVelocityToInstructions(Robot->HeadingToDestination(P,Corner), cObstacleDetectionSpeed, &CalCoordSys, 10000);
  //	AddStraightVelocityToInstructions(pi/2, cObstacleDetectionSpeed, &CalCoordSys, 10000);
  	if ( (TaskResult = Robot->FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	TLine L1;
  	Robot->GetFrontLine(Robot->m_Position, Robot->m_Heading, &L1);
  /*Rotate a bit*/
  	Robot->AddVelocitiesToInstructions(-cObstacleDetectionSpeed,0,1000);
  	if ( (TaskResult = Robot->FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Hit the doorway again*/
  	Robot->AddVelocitiesToInstructions(cObstacleDetectionSpeed,cObstacleDetectionSpeed,10000);
  	if ( (TaskResult = Robot->FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	TLine L2;
  	Robot->GetFrontLine(Robot->m_Position, Robot->m_Heading, &L2);
  /*Correct Robot's Position*/
  	robPOINT FirstMeasuredCornerPosition;
  	IntersectionOfLines(L1,L2,&FirstMeasuredCornerPosition);
  	robPOINT FirstActualCornerPosition;
  	FirstActualCornerPosition = TransformToNonDisplacedCoordinates(&CalCoordSys, Corner);
  	Robot->m_Position.x += FirstActualCornerPosition.x - FirstMeasuredCornerPosition.x;
  	Robot->m_Position.y += FirstActualCornerPosition.y - FirstMeasuredCornerPosition.y;
  	DPosition->x = FirstActualCornerPosition.x - FirstMeasuredCornerPosition.x;
  	DPosition->y = FirstActualCornerPosition.y - FirstMeasuredCornerPosition.y;
  	Robot->SetPosition(Robot->m_Position);
  
  /*Get Ready to find second corner*/
  	Robot->SetFlipHandle(true);
  	robPOINT Destination;
  	Destination.x = Robot->m_Position.x - (8*cTPI + Robot->GetHandleLength()) * cos(Robot->m_Heading);
  	Destination.y = Robot->m_Position.y - (8*cTPI + Robot->GetHandleLength()) * sin(Robot->m_Heading); 
  	Robot->AddPositionVelocityToInstructions(Destination, NULL, 3*cTPI, cObstacleDetectionSpeed, TRUE, 9000);
  //	AddVelocitiesToInstructions(-cComputerControlSpeed,-cComputerControlSpeed,1000);
  	if ( (TaskResult = Robot->FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	P.x = 0;
  	P.y = 0;
  	Robot->AddPositionVelocityToInstructions(Robot->GetRobotToPoint(P, &CalCoordSys), &CalCoordSys, 0, cComputerControlSpeed, FALSE, 9000);
  	Robot->AddHeadingToInstructions(-pi/2, &CalCoordSys, 0, cComputerControlHeadingSpeed, 0);
  	if ( (TaskResult = Robot->FollowInstructions(TRUE, FALSE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Hit doorway*/
  	Corner.x = 0;
  	Corner.y = -Distance(m_P0, m_P1) / 2;
  	P = TransformToDisplacedCoordinates(&CalCoordSys,Robot->m_Position);
  	Robot->AddStraightVelocityToInstructions(Robot->HeadingToDestination(P,Corner), cObstacleDetectionSpeed, &CalCoordSys, 10000);
  //	AddStraightVelocityToInstructions(-pi/2, cObstacleDetectionSpeed, &CalCoordSys, 10000);
  	if ( (TaskResult = Robot->FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	Robot->GetFrontLine(Robot->m_Position, Robot->m_Heading, &L1);
  /*Rotate a bit*/
  	Robot->AddVelocitiesToInstructions(0,-cObstacleDetectionSpeed,1000);
  	if ( (TaskResult = Robot->FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Hit the doorway again*/
  	Robot->AddVelocitiesToInstructions(cObstacleDetectionSpeed,cObstacleDetectionSpeed,10000);
  	if ( (TaskResult = Robot->FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	Robot->GetFrontLine(Robot->m_Position, Robot->m_Heading, &L2);
  
  	robPOINT SecondMeasuredCornerPosition;
  	IntersectionOfLines(L1,L2,&SecondMeasuredCornerPosition);
  	robPOINT SecondActualCornerPosition;
  	SecondActualCornerPosition.x = 0;
  	SecondActualCornerPosition.y = -Distance(m_P0, m_P1) / 2;
  	SecondActualCornerPosition = TransformToNonDisplacedCoordinates(&CalCoordSys, SecondActualCornerPosition);
  /*Correct the robot's heading*/
  	double theta, thetaprime;
  	theta = atan2(SecondActualCornerPosition.y - FirstActualCornerPosition.y, SecondActualCornerPosition.x - FirstActualCornerPosition.x);
  	thetaprime = atan2(SecondMeasuredCornerPosition.y - FirstActualCornerPosition.y, SecondMeasuredCornerPosition.x - FirstActualCornerPosition.x);
  	Robot->SetHeading(Robot->m_Heading + theta - thetaprime);
  	*DHeading = theta - thetaprime;
  	Robot->m_HeadingError = 0;
  /*Correct Robot's Position*/
  	Robot->m_Position.x += SecondActualCornerPosition.x - SecondMeasuredCornerPosition.x;
  	Robot->m_Position.y += SecondActualCornerPosition.y - SecondMeasuredCornerPosition.y;
  	DPosition->x += SecondActualCornerPosition.x - SecondMeasuredCornerPosition.x;
  	DPosition->y += SecondActualCornerPosition.y - SecondMeasuredCornerPosition.y;
  	Robot->SetPosition(Robot->m_Position);
  	Robot->m_PositionError.x = 0;
  	Robot->m_PositionError.y = 0;
  	return TR_TaskComplete;
  }
  
  void CCornerWall::Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbGrid)
  {
  	if (!m_IsVisible) return;
  	CExecutableRWGraphicObject::Draw(ProbGridBmpUpdateRect, pDC, ProbGrid);
  
  	CPoint SurfaceOffset = ProbGridBmpUpdateRect.TopLeft();
  
  	int CalZoneCircleBmpWidth, CalZoneCircleBmpHeight;
  	if (ProbGrid->m_ZoomLevel == cZoomLevel1) {
  		CalZoneCircleBmpWidth = cZoom1CalZoneCircleBmpWidth;
  		CalZoneCircleBmpHeight = cZoom1CalZoneCircleBmpHeight;
  	} else if (ProbGrid->m_ZoomLevel == cZoomLevel2) {
  		CalZoneCircleBmpWidth = cZoom2CalZoneCircleBmpWidth;
  		CalZoneCircleBmpHeight = cZoom2CalZoneCircleBmpHeight;
  	} else if (ProbGrid->m_ZoomLevel == cZoomLevel3) {
  		CalZoneCircleBmpWidth = cZoom3CalZoneCircleBmpWidth;
  		CalZoneCircleBmpHeight = cZoom3CalZoneCircleBmpHeight;
  	} else {
  		ASSERT(FALSE);
  		CalZoneCircleBmpWidth = cZoom1CalZoneCircleBmpWidth;
  		CalZoneCircleBmpHeight = cZoom1CalZoneCircleBmpHeight;
  	}
  	robPOINT Polygon[6];
  	TLine WallLine;
  	FindLineFrom2Points(m_P0, m_P1, &WallLine);
  	robPOINT Intersection = FindPerpendicularIntersection(WallLine, m_P2);
  	double CalZoneCircleSize = ProbGrid->ClientSizeToRobotSize(CalZoneCircleBmpWidth);
  	double WallLength;
  	if ( Distance(Intersection, m_P0) > Distance(Intersection, m_P1) ) {
  		Polygon[0].x = m_P0.x - CalZoneCircleSize * cos(m_Heading2);
  		Polygon[0].y = m_P0.y - CalZoneCircleSize * sin(m_Heading2);
  		WallLength = Distance(Intersection, m_P0);
  	} else {
  		Polygon[0].x = m_P1.x - CalZoneCircleSize * cos(m_Heading2);
  		Polygon[0].y = m_P1.y - CalZoneCircleSize * sin(m_Heading2);
  		WallLength = Distance(Intersection, m_P1);
  	}
  	Polygon[1] = Intersection;
  	Polygon[2].x = m_P2.x - CalZoneCircleSize * cos(m_Heading);
  	Polygon[2].y = m_P2.y - CalZoneCircleSize * sin(m_Heading);
  	Polygon[3].x = Polygon[2].x + CalZoneCircleSize * cos(m_Heading2);
  	Polygon[3].y = Polygon[2].y + CalZoneCircleSize * sin(m_Heading2);
  	Polygon[5].x = Polygon[0].x + CalZoneCircleSize * cos(m_Heading);
  	Polygon[5].y = Polygon[0].y + CalZoneCircleSize * sin(m_Heading);
  	Polygon[4].x = Polygon[5].x + (WallLength + 2*CalZoneCircleSize) * cos(m_Heading2);
  	Polygon[4].y = Polygon[5].y + (WallLength + 2*CalZoneCircleSize) * sin(m_Heading2);
  
  	POINT WallP[6];
  	for (int i = 0; i < 6; i++) WallP[i] = ProbGrid->RobotToDDSurface(Polygon[i], SurfaceOffset);
  
  	POINT WallP2[4];
  
  	WallP2[0] = WallP[1];
  	WallP2[1] = WallP[2];
  	WallP2[2] = WallP[3];
  	WallP2[3] = WallP[4];
  	int OldPolyFillMode = pDC->SetPolyFillMode(ALTERNATE);
  	CBrush CornerWallBrush(COLORREF(0x7F7F7F));
  	CGdiObject* OldBrush = pDC->SelectObject(&CornerWallBrush);
  	CGdiObject* OldPen = pDC->SelectStockObject(WHITE_PEN);
  	pDC->Polygon(WallP2, 4);
  
  	WallP2[0] = WallP[0];
  	WallP2[1] = WallP[1];
  	WallP2[2] = WallP[4];
  	WallP2[3] = WallP[5];
  	pDC->SelectStockObject(BLACK_BRUSH);
  //	pDC->SelectStockObject(BLACK_PEN);
  	pDC->Polygon(WallP2, 4);
  
  	pDC->SetPolyFillMode(OldPolyFillMode);
  	pDC->SelectObject(OldBrush);
  	pDC->SelectObject(OldPen);
  
  	return;
  }
  
  void CCornerWall::Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid)
  {
  	if (!m_IsVisible) return;
  	CWall::Draw(ProbabilityGridUpdateRect, BackBuffer, ProbGrid);
  
  	CRect rcRect, destRect;
  	int Frame;
  	HRESULT ddrval;
  
  /*Load Largest bitmap initially*/
  	if (m_lpDDSGreyArrowBmp == NULL) {
  		m_lpDDSGreyArrowBmp = ProbGrid->DDrawLoadBitmap(IDB_LARGE_GREY_ARROW);
  		if( m_lpDDSGreyArrowBmp == NULL ) {
  			fprintf(stderr, "CornerWall Draw Error!!!! DDrawLoadBitmap failed.  Did not load IDB_LARGE_GREY_ARROWx\n");
  		}
  		m_OldGreyArrowZoomLevel = cZoomLevel3;
  	}
  	if (ProbGrid->m_ZoomLevel != m_OldGreyArrowZoomLevel) {
  		HRESULT reloadresult;
  		if (ProbGrid->m_ZoomLevel == cZoomLevel3) {
  			reloadresult = ProbGrid->DDrawReloadBitmap(m_lpDDSGreyArrowBmp, IDB_LARGE_GREY_ARROW);
  		} else if (ProbGrid->m_ZoomLevel == cZoomLevel2) {
  			reloadresult = ProbGrid->DDrawReloadBitmap(m_lpDDSGreyArrowBmp, IDB_MEDIUM_GREY_ARROW);
  		} else {
  			reloadresult = ProbGrid->DDrawReloadBitmap(m_lpDDSGreyArrowBmp, IDB_SMALL_GREY_ARROW);
  		}
  		if (reloadresult != DD_OK) {
  			fprintf(stderr, "CalibrationZone Draw Error!!!! DDrawReLoadBitmap failed.  Did not load IDB_x_GREY_ARROW\n");
  			ProbGrid->DDrawDiagnostic(reloadresult);
  		}
  	}
  	if (m_lpDDSGreyArrowBmp->IsLost() == DDERR_SURFACELOST) {
  		m_lpDDSGreyArrowBmp->Restore();
  	}
  	
  	m_OldGreyArrowZoomLevel = ProbGrid->m_ZoomLevel;
  	
  /*Draw arrow*/
  //	LPDIRECTDRAWSURFACE lpDDSArrowBmp = NULL;
  	int ArrowBmpFrameWidth;
  	int ArrowBmpFrameHeight;
  	if (ProbGrid->m_ZoomLevel == cZoomLevel1) {
  //		lpDDSArrowBmp = ProbGrid->m_lpDDSGreyArrowBmp1;
  		ArrowBmpFrameWidth = cZoom1CalZoneArrowBmpWidth;
  		ArrowBmpFrameHeight = cZoom1CalZoneArrowBmpHeight;
  	} else if (ProbGrid->m_ZoomLevel == cZoomLevel2) {
  //		lpDDSArrowBmp = ProbGrid->m_lpDDSGreyArrowBmp2;
  		ArrowBmpFrameWidth = cZoom2CalZoneArrowBmpWidth;
  		ArrowBmpFrameHeight = cZoom2CalZoneArrowBmpHeight;
  	} else if (ProbGrid->m_ZoomLevel == cZoomLevel3) {
  //		lpDDSArrowBmp = ProbGrid->m_lpDDSGreyArrowBmp3;
  		ArrowBmpFrameWidth = cZoom3CalZoneArrowBmpWidth;
  		ArrowBmpFrameHeight = cZoom3CalZoneArrowBmpHeight;
  	} else {
  		ASSERT(FALSE);
  		ArrowBmpFrameWidth = cZoom1CalZoneArrowBmpWidth;
  		ArrowBmpFrameHeight = cZoom1CalZoneArrowBmpHeight;
  	}
  	
  	Frame = ArrowHeadingToArrowBitmapFrame(m_Heading2, ArrowBmpTotalFrames);
  	rcRect.left   = Frame%ArrowBmpFramePerLine*ArrowBmpFrameWidth;
  	rcRect.top    = Frame/ArrowBmpFramePerLine*ArrowBmpFrameHeight;
  	rcRect.right  = rcRect.left + ArrowBmpFrameWidth;
  	rcRect.bottom = rcRect.top + ArrowBmpFrameHeight;
  
  	TLine WallLine;
  	FindLineFrom2Points(m_P0, m_P1, &WallLine);
  	robPOINT Intersection = FindPerpendicularIntersection(WallLine, m_P2);
  	robPOINT ArrowPoint;
  	double ArrowLength = ProbGrid->ClientSizeToRobotSize(ArrowBmpFrameWidth);
  	ArrowPoint.x = Intersection.x + ((cArrowDist - 4)*cTPI - ArrowLength / 2) * cos(m_Heading);
  	ArrowPoint.x = ArrowPoint.x + cArrowDist * cTPI * cos(m_Heading2);
  	ArrowPoint.y = Intersection.y + ((cArrowDist - 4)*cTPI - ArrowLength / 2) * sin(m_Heading);
  	ArrowPoint.y = ArrowPoint.y + cArrowDist * cTPI * sin(m_Heading2);
  
  	float temp = ProbGrid->RobotToDDSurfaceX(ArrowPoint.x, ProbabilityGridUpdateRect.left);
  	temp -= ArrowBmpFrameWidth/2.0f;
  	destRect.left = Round(temp);
  //		destRect.left = Round((((CurrentNode->CalibrationZone->m_P1.x + CurrentNode->CalibrationZone->m_P0.x)/2.0 + cArrowDist*cTPI*cos(m_Heading2)) / cWorldGridPointSize + cWorldGridXSize / 2 - ProbabilityGridUpdateRect.left)*m_ZoomLevel - ArrowBmpFrameWidth/2.0 );
  	destRect.right = destRect.left + ArrowBmpFrameWidth;
  	temp = ProbGrid->RobotToDDSurfaceY(ArrowPoint.y, ProbabilityGridUpdateRect.top);
  	temp -= ArrowBmpFrameHeight/2.0f;
  	destRect.top = Round(temp);
  //		destRect.top = Round((-((CurrentNode->CalibrationZone->m_P1.y + CurrentNode->CalibrationZone->m_P0.y)/2.0 + cArrowDist*cTPI*sin(m_Heading2)) / cWorldGridPointSize + cWorldGridYSize / 2 - ProbabilityGridUpdateRect.top)*m_ZoomLevel - ArrowBmpFrameHeight/2.0 );
  	destRect.bottom = destRect.top + ArrowBmpFrameHeight;
  	ProbGrid->RectifyRectangles(&rcRect, &destRect);
  	ProbGrid->DDSetColorKey(m_lpDDSGreyArrowBmp, cTransparencyColor);
  	if ( (destRect.Height() != 0) && (destRect.Width() != 0) && (rcRect.Height() != 0) && (rcRect.Width() != 0) ) {
  		while (BackBuffer->GetBltStatus(DDGBS_CANBLT) != DD_OK);
  		ddrval = BackBuffer->BltFast(destRect.left, destRect.top, m_lpDDSGreyArrowBmp, rcRect, DDBLTFAST_SRCCOLORKEY);
  		if (ddrval != DD_OK) {
  			fprintf(stderr, "Draw Arrow Failed\n");
  			ProbGrid->DDrawDiagnostic(ddrval);
  			fprintf(stderr, "***DrawCornerCalibrationZone Error***\n");
  			fprintf(stderr, "destRect = %d %d %d %d\n",destRect.left, destRect.top, destRect.right, destRect.bottom);
  			fprintf(stderr, "rcRect = %d %d %d %d\n", rcRect.left,rcRect.top, rcRect.right, rcRect.bottom);
  			fflush(stderr);
  		}
  	}
  	return;
  }
  
  void CCornerWall::CalculateBounds()
  {
  	CCalibrationZone::CalculateBounds();
  	robPOINT point;
  	point = m_P2;
  /*Include the corner point inside the bounds*/
  	point.x = m_P2.x + cZoom3CalZoneCircleBmpWidth/cZoomLevel3*cWorldGridPointSize;
  	point.y = m_P2.y + cZoom3CalZoneCircleBmpHeight/cZoomLevel3*cWorldGridPointSize;
  	m_Bounds.Include(point);
  
  	point.x = m_P2.x - cZoom3CalZoneCircleBmpWidth/cZoomLevel3*cWorldGridPointSize;
  	point.y = m_P2.y - cZoom3CalZoneCircleBmpHeight/cZoomLevel3*cWorldGridPointSize;
  	m_Bounds.Include(point);
  }
  
  bool CCornerWall::Create(CRobot * Robot, bool WallToLeft, int WallBuffer)
  //WallBuffer is the distance the robot travels from the wall while searching for the corner.
  {
  	if ( (Robot->GetFloorType() == ftWood) && (Robot->GetAttachmentType() == atVacuum) ) {
  		CDlgErrorCheckingWoodWithVacuum Warning;
  		Warning.DoModal();
  		return false;
  	}
  
  	TRobotInfo OldRobotInfo = Robot->GetRobotInfo();
  	Robot->SetEnableRepositionOnUnknownObstacle(false);
  	Robot->SetEStopVisible(true);
  	m_FloorType = Robot->GetFloorType();
  	
  	if (m_FloorType == ftWood) {
  		CDlgCreateCheckPointSlipperySurface Warning;
  		if (Warning.DoModal() != IDOK) return false;
  		Robot->AddWaitLongToInstructions(0,0,10);
  		Robot->AddBeepToInstructions(2000, 3);
  		if (Robot->FollowInstructions(false, false, IR_InstructionComplete, false) != TR_TaskComplete) return false;
  	}
  
  	if (!CreateFrontWall(Robot)) {
  		Robot->SetRobotInfo(OldRobotInfo);
  		return false;
  	}
  
  	bool result = CreateCorner(Robot, WallToLeft, WallBuffer);
  	if (result) {
  	/*Calibrate the floor surface if necessary*/
  		if (!Robot->GetAlreadyCalibratedFloorType(m_FloorType)) {
  			CDialog Warning(IDD_MEASURING_CARPET_DENSITY);
  			if (Warning.DoModal() == IDOK) {
  				int HCF;
  				CCTDPath* Path = new CCTDPath(cComputerControlSpeed);
  				Path->AddPoint(Robot->GetHandlePosition(-cFollowCTDPathHandleLength));
  				robPOINT P = Robot->GetPosition();
  				P.x -= 5*cTPI*cos(Robot->GetHeading());
  				P.y -= 5*cTPI*sin(Robot->GetHeading());
  				Path->AddPoint(P);
  				Path->AddPoint(GetEntryPoint());
  				Robot->AddCTDPathToInstructions(Path, cModPositionZone, TRUE);
  				Robot->AddHeadingToInstructions(m_Heading, NULL, cMinHeadingZone);
  				Robot->AddSquareToWallToInstructions(m_Heading, m_FloorType == ftWood);
  				if (Robot->FollowInstructions() == IR_InstructionComplete) {
  					if (Robot->CalibrateFloorSurface(&HCF) == TR_TaskComplete) {
  						Robot->SetHeadingCorrectionFactor(m_FloorType, HCF);
  						robPOINT DPosition;
  						double DHeading;
  						if (Calibrate(Robot, &DPosition, &DHeading) == TR_TaskComplete) {
  							Path = new CCTDPath(cComputerControlSpeed);
  							Path->AddPoint(Robot->GetHandlePosition(-cFollowCTDPathHandleLength));
  							P = Robot->GetPosition();
  							P.x -= 5*cTPI*cos(m_Heading);
  							P.y -= 5*cTPI*sin(m_Heading);
  							Path->AddPoint(P);
  							Path->AddPoint(GetEntryPoint());
  							Robot->AddCTDPathToInstructions(Path, cModPositionZone, TRUE);
  							Robot->FollowInstructions();
  						}
  					}
  				}
  			} else {
  				CCTDPath* Path = new CCTDPath(cComputerControlSpeed);
  				Path->AddPoint(Robot->GetHandlePosition(-cFollowCTDPathHandleLength));
  				Path->AddPoint(GetExitPoint());
  				robPOINT P;
  				P.x = P.y = 0;
  				Robot->SetPositionError(P);
  				Robot->SetHeadingError(0);
  				Robot->AddCTDPathToInstructions(Path, cModPositionZone, TRUE);
  				Robot->FollowInstructions();
  			}
  		} else {
  			CCTDPath* Path = new CCTDPath(cComputerControlSpeed);
  			Path->AddPoint(Robot->GetHandlePosition(-cFollowCTDPathHandleLength));
  			Path->AddPoint(GetExitPoint());
  			robPOINT P;
  			P.x = P.y = 0;
  			Robot->SetPositionError(P);
  			Robot->SetHeadingError(0);
  			Robot->AddCTDPathToInstructions(Path, cModPositionZone, TRUE);
  			Robot->FollowInstructions();
  		}
  	}
  	Robot->SetRobotInfo(OldRobotInfo);
  	return result;
  }
  
  bool CCornerWall::CreateCorner(CRobot* Robot, bool WallToLeft, int WallBuffer)
  {
  	const double cFindCornerDistance = 60 * cTPI;
  	robPOINT P2;
  	unsigned char FeelingMotor;
  	TTaskResult TaskResult;
  	m_WallBuffer = WallBuffer;
  #ifdef IMPLEMENTED
  /*Back up, Turn towards the reference point*/
  	double phi;
  	if (WallToLeft) {
  		phi = m_Heading + pi/2;
  		FeelingMotor = 0;
  	} else {
  		phi = m_Heading - pi/2;
  		FeelingMotor = 1;
  	}
  	robPOINT P;
  	P.x = (m_P0.x + m_P1.x) / 2;
  	P.y = (m_P0.y + m_P1.y) / 2;
  
  	P.x -= cRRNoseLength * cos(m_Heading);
  	P.y -= cRRNoseLength * sin(m_Heading);
  
  	P.x = P.x - (cCornerWallBackupDistance) * cos(m_Heading);
  	P.y = P.y - (cCornerWallBackupDistance) * sin(m_Heading);
  
  	CCTDPath* Path = new CCTDPath(cComputerControlSpeed);
  	Path->AddPoint(Robot->GetHandlePosition(-cFollowCTDPathHandleLength));
  	Path->AddPoint(P);
  	Robot->SetHandleLength(cFollowCTDPathHandleLength);
  	Robot->SetObstacleSensitivity(osMediumSensitivity);
  	Robot->AddCTDPathToInstructions(Path, cMinPositionZone, TRUE);
  	Robot->AddHeadingToInstructions(phi, NULL, 0, cComputerControlHeadingSpeed, 0);
  	P.x += cFollowCTDPathHandleLength * cos(Robot->m_Heading);
  	P.y += cFollowCTDPathHandleLength * sin(Robot->m_Heading);
  	if ( (TaskResult = Robot->FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		goto Error;
  	}
  /*run into reference point*/
  	CCTDPath* Straight;
  	Straight = new CCTDPath(cObstacleDetectionSpeed);
  	Straight->AddPoint(P);
  	P.x += cFindCornerDistance * cos(phi);
  	P.y += cFindCornerDistance * sin(phi);
  	Straight->AddPoint(P);
  	Robot->AddCTDPathToInstructions(Straight);
  	if ( (TaskResult = Robot->FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		goto Error;
  	}
  /*push correct wheel up to the reference*/
  	if (FeelingMotor == 0) { 
  		Robot->AddFeelMotorToInstructions(0, 3000);
  		Robot->FollowInstructions();
  	} else {
  		Robot->AddFeelMotorToInstructions(1, 3000);
  		Robot->FollowInstructions();
  	}
  #endif
  	robPOINT P;
  	P.x = (m_P0.x + m_P1.x) / 2;
  	P.y = (m_P0.y + m_P1.y) / 2;
  	double phi;
  	if (WallToLeft) {
  		phi = m_Heading + pi/2;
  		FeelingMotor = 0;
  	} else {
  		phi = m_Heading - pi/2;
  		FeelingMotor = 1;
  	}
  	Robot->AddCornerCalibrationToInstructions(P, m_Heading, FeelingMotor, false, m_WallBuffer);
  	TaskResult = Robot->FollowInstructions(TRUE, TRUE, IR_InstructionComplete);
  	if (TaskResult != TR_TaskComplete) {
  		goto Error;
  	}
  	P2 = Robot->GetTaskResultPosition();
  	double H2;
  	H2 = Robot->GetTaskResultHeading();
  	m_FeelingMotor = FeelingMotor;
  	m_Heading2 = phi;
  	if (m_FeelingMotor == 0) {
  		m_CalPoint3.x = P2.x + cRRWidth/2 * cos(H2 - pi/2) + cRRNoseLength * cos(H2);
  		m_CalPoint3.y = P2.y + cRRWidth/2 * sin(H2 - pi/2) + cRRNoseLength * sin(H2);
  	} else {
  		m_CalPoint3.x = P2.x + cRRWidth/2 * cos(H2 + pi/2) + cRRNoseLength * cos(H2);
  		m_CalPoint3.y = P2.y + cRRWidth/2 * sin(H2 + pi/2) + cRRNoseLength * sin(H2);
  	}
  
  	TLine WallLine;
  	FindLineFrom2Points(m_P0, m_P1, &WallLine);
  	m_P2 = FindPerpendicularIntersection(WallLine, m_CalPoint3);
  	m_P2.x -= cThirdCircleDistance * cos(m_Heading);
  	m_P2.y -= cThirdCircleDistance * sin(m_Heading);
  
  //	m_ZoneType = czCornerWall;
  	CalculateBounds();
  	
  	return true;
  Error:
  	return false;
  }
  
  bool CWall::Create(CRobot * Robot)
  {
  	if ( (Robot->GetFloorType() == ftWood) && (Robot->GetAttachmentType() == atVacuum) ) {
  		CDlgErrorCheckingWoodWithVacuum Warning;
  		Warning.DoModal();
  		return false;
  	}
  	m_FloorType = Robot->GetFloorType();
  	if (m_FloorType == ftWood) {
  		if (!IsKindOf(RUNTIME_CLASS(CHomeBase))) {
  			CDlgCreateCheckPointSlipperySurface Warning;
  			if (Warning.DoModal() != IDOK) return false;
  			Robot->AddWaitLongToInstructions(0,0,10);
  			Robot->AddBeepToInstructions(2000, 3);
  			if (Robot->FollowInstructions(false, false, IR_InstructionComplete, false) != TR_TaskComplete) return false;
  		}
  	}
  /*Show the EStop dialog window*/
  	TRobotInfo OldRobotInfo = Robot->GetRobotInfo();
  	Robot->SetEStopVisible(true);
  	Robot->SetEnableRepositionOnUnknownObstacle(false);
  	if (!CreateFrontWall(Robot)) {
  		Robot->SetRobotInfo(OldRobotInfo);
  		return false;
  	}
  //	m_ZoneType = czWall;
  	CalculateBounds();
  
  /*Calibrate the floor surface if necessary*/
  	if (!Robot->GetAlreadyCalibratedFloorType(m_FloorType)) {
  		CDialog Warning(IDD_MEASURING_CARPET_DENSITY);
  		if (Warning.DoModal() == IDOK) {
  			int HCF;
  			if (Robot->CalibrateFloorSurface(&HCF) == TR_TaskComplete) {
  				Robot->SetHeadingCorrectionFactor(m_FloorType, HCF);
  				robPOINT DPosition;
  				double DHeading;
  				Calibrate(Robot, &DPosition, &DHeading);
  			}
  		}
  	}
  /*Back away from wall*/
  	CCTDPath* Path = new CCTDPath(cComputerControlSpeed);
  	Path->AddPoint(Robot->GetHandlePosition(-cFollowCTDPathHandleLength));
  	Path->AddPoint(GetExitPoint());
  	robPOINT P;
  	P.x = P.y = 0;
  	Robot->SetPositionError(P);
  	Robot->SetHeadingError(0);
  	Robot->AddCTDPathToInstructions(Path, cModPositionZone, TRUE);
  	Robot->FollowInstructions();
  
  	Robot->SetRobotInfo(OldRobotInfo);
  	return true;
  }
  
  bool CWall::CreateFrontWall(CRobot *Robot)
  {
  /*Find the wall*/
  	if (Robot->SquareToWall() != TR_TaskComplete) {
  		return false;
  	}
  	
  	double phi;
  	double H0;
  	robPOINT P0;
  	H0 = Robot->GetTaskResultHeading();
  	P0 = Robot->GetTaskResultPosition();
  /*Calculate the heading for the wall*/
  	m_Heading = BringAngleInRange(H0);
  /*Find the corners of the calibration wall given the 2 robot positions*/
  	phi = m_Heading + pi/2;
  	m_P0 = P0;
  	m_P1 = P0;
  	m_P1.x += cRRWidth/2 * cos(phi) + cRRNoseLength * cos(m_Heading);
  	m_P1.y += cRRWidth/2 * sin(phi) + cRRNoseLength * sin(m_Heading);
  	m_P0.x += -cRRWidth/2 * cos(phi) + cRRNoseLength * cos(m_Heading);
  	m_P0.y += -cRRWidth/2 * sin(phi) + cRRNoseLength * sin(m_Heading);
  	return true;
  }
  
  CWall::CWall(CWall& Wall) : CCalibrationZone(Wall)
  {
  	m_P0 = Wall.m_P0;
  	m_P1 = Wall.m_P1;
  	m_FloorType = Wall.m_FloorType;
  	m_Heading = Wall.m_Heading;
  //	m_ZoneType = Wall.m_ZoneType;
  }
  
  CCornerWall::CCornerWall(CCornerWall& Wall) : CWall(Wall)
  {
  	m_P0 = Wall.m_P0;
  	m_P1 = Wall.m_P1;
  	m_FloorType = Wall.m_FloorType;
  	m_Heading = Wall.m_Heading;
  //	m_ZoneType = Wall.m_ZoneType;
  	m_CalPoint3 = Wall.m_CalPoint3;
  	m_FeelingMotor = Wall.m_FeelingMotor;
  	m_P2 = Wall.m_P2;
  	m_Heading2 = Wall.m_Heading2;
  	m_WallBuffer = cDefaultWallBuffer;
  }
  
  void CWall::operator=(CWall Wall)
  {
  //	CWall::CWall(Wall);
  }
  
  void CCornerWall::operator=(CCornerWall Corner)
  {
  //	CCornerWall::CCornerWall(Corner);
  }
  
  void CCalibrationZone::SetCaption(CString Caption)
  {
  	if (Caption == "Home Base") Caption = "Home Base1";
  	CExecutableRWGraphicObject::SetCaption(Caption);
  }
  
  TTaskResult CCalibrationZone::Adjust(CRobot *Robot)
  {
  	return TR_Error;
  }
  
  TTaskResult CCalibrationZone::GoToEntrance(CRobot *Robot, short Speed)
  {
  	ASSERT(Robot != NULL);
  	if (Robot == NULL) return TR_Error;
  
  	TRobotInfo RobotInfo = Robot->GetRobotInfo();
  	Robot->SetState(stExecuting);
  	Robot->SetEStopVisible(true);
  	TTaskResult TaskResult;
  	if (Robot->GetOnHomeBase()) {
  		if ( (TaskResult = Robot->BackupFromWall()) != TR_TaskComplete) {
  			goto CLEANUP;
  		}
  	}
  	if (m_Bounds.Inside(Robot->GetPosition())) {
  		robPOINT Destination;
  		TLine Wall;
  		FindLineFrom2Points(m_P0, m_P1, &Wall);
  		double Distance = PerpendicularDistanceFromLine(Wall, Robot->GetPosition());
  		if (Distance <= cRRWidth/2 + 4*cTPI) {
  			Destination.x = (m_P0.x + m_P1.x) / 2 - (cRRWidth/2 + 4*cTPI) * cos(m_Heading);
  			Destination.y = (m_P0.y + m_P1.y) / 2 - (cRRWidth/2 + 4*cTPI) * sin(m_Heading);
  		} else {
  			Destination.x = (m_P0.x + m_P1.x) / 2 - (Distance) * cos(m_Heading);
  			Destination.y = (m_P0.y + m_P1.y) / 2 - (Distance) * sin(m_Heading);
  		}
  		CCTDPath* Path = new CCTDPath(cComputerControlSpeed);
  		Path->AddPoint(Robot->GetHandlePosition(cFollowCTDPathHandleLength));
  		Path->AddPoint(Destination);
  		Robot->AddCTDPathToInstructions(Path, cMinPositionZone, TRUE);
  		TaskResult = Robot->FollowInstructions(TRUE, FALSE, IR_InstructionComplete);
  	} else {
  		TaskResult = ExecutePathToEntryPoint(Robot, Speed);
  	}
  
  	if (TaskResult == TR_TaskComplete) {
  		Robot->AddHeadingToInstructions(m_Heading, NULL, cMinHeadingZone);
  		TaskResult = Robot->FollowInstructions(true, true ,IR_InstructionComplete);
  	}
  
  CLEANUP:
  	Robot->SetRobotInfo(RobotInfo);
  	return TaskResult;
  }
  
  TTaskResult CCornerWall::Adjust(CRobot *Robot)
  {
  	ASSERT(Robot != NULL);
  	if (Robot == NULL) return TR_Error;
  	TRobotInfo OldRobotInfo = Robot->GetRobotInfo();
  	Robot->SetEStopVisible(true);
  	TTaskResult TaskResult = GoToEntrance(Robot);
  	if (TaskResult == TR_TaskComplete) {
  	/*Store the old calibration data in case adjustment fails*/
  		robPOINT OldP0 = m_P0;
  		robPOINT OldP1 = m_P1;
  		double OldHeading = m_Heading;
  //		CCornerWall OldCorner = *this;
  		Robot->SetFloorType(m_FloorType);
  		bool OldVisible = GetVisible() == TRUE;
  		SetVisible(false);
  		Redraw(true);
  		if (Create(Robot, m_FeelingMotor == 0, m_WallBuffer)) {
  			m_ExecutableObjectList->RemoveObjectFromStoredPaths(this);
  			TaskResult = TR_TaskComplete;
  		} else {
  		/*restore old calibration data on failure (Only need to restore CWall members because others aren't updated until creation is successful)*/
  			m_P0 = OldP0;
  			m_P1 = OldP1;
  			m_Heading = OldHeading;
  //			*this = OldCorner;
  			CalculateBounds();
  			TaskResult = TR_Error;
  		}
  		SetVisible(OldVisible);
  		Redraw(true);
  	}
  	Robot->SetRobotInfo(OldRobotInfo);
  	return TaskResult;
  }
  
  TTaskResult CWall::Adjust(CRobot *Robot)
  {
  	ASSERT(Robot != NULL);
  	if (Robot == NULL) return TR_Error;
  	TRobotInfo OldRobotInfo = Robot->GetRobotInfo();
  	Robot->SetEStopVisible(true);
  	TTaskResult TaskResult = GoToEntrance(Robot);
  	if (TaskResult == TR_TaskComplete) {
  		bool OldVisible = GetVisible() == TRUE;
  		SetVisible(false);
  		Redraw(true);
  		Robot->SetFloorType(m_FloorType);
  		if (Create(Robot)) {
  			m_ExecutableObjectList->RemoveObjectFromStoredPaths(this);
  			TaskResult = TR_TaskComplete;
  		} else {
  			CalculateBounds();
  			TaskResult = TR_Error;
  		}
  		SetVisible(OldVisible);
  		Redraw(true);
  	}
  	Robot->SetRobotInfo(OldRobotInfo);
  	return TaskResult;
  }
  
  int CCornerWall::GetWallBuffer()
  {
  	return m_WallBuffer;
  }
  
  void CCornerWall::SetWallBuffer(int WallBuffer)
  {
  	m_WallBuffer = WallBuffer;
  }
