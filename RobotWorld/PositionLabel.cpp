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
  // PositionLabel.cpp: implementation of the CPositionLabel class.
  //
  //////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "RobotWorld.h"
  #include "PositionLabel.h"
  #include "Robot.h"
  #include "WorldGridConsts.h"
  #include "global.h"
  #include "PathList.h"
  #include "ddraw.h"
  #include "ddutil.h"
  #include "Probability Grid.h"
  #include "DlgExecutableObjectGeneralProperties.h"
  
  #ifdef _DEBUG
  #undef THIS_FILE
  static char THIS_FILE[]=__FILE__;
  #define new DEBUG_NEW
  #endif
  
  IMPLEMENT_SERIAL(CPositionLabel, CExecutableRWGraphicObject, VERSIONABLE_SCHEMA | VERSION_NUMBER)
  
  LPDIRECTDRAWSURFACE3 CPositionLabel::m_lpDDSPositionLabelBmp = NULL;
  LPDIRECTDRAWSURFACE3 CPositionLabel::m_lpDDSAnimatedPositionLabelBmp = NULL;
  float CPositionLabel::m_OldZoomLevel = 0;
  float CPositionLabel::m_OldAnimationZoomLevel = 0;
  int CPositionLabel::m_OldAnimationFrame = 0;
  
  const int cZoom3BmpHeight = 34;
  const int cZoom3BmpWidth = 35;
  const int cZoom2BmpHeight = 17;
  const int cZoom2BmpWidth = 18;
  const int cZoom1BmpHeight = 9;
  const int cZoom1BmpWidth = 9;
  
  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////
  
  CPositionLabel::CPositionLabel() : CExecutableRWGraphicObject()
  {
  }
  
  CPositionLabel::~CPositionLabel()
  {
  	if (m_lpDDSPositionLabelBmp != NULL) {
  		m_lpDDSPositionLabelBmp->Release();
  		m_lpDDSPositionLabelBmp = NULL;
  	}
  }
  
  void CPositionLabel::EditProperties()
  {
  	CDlgExecutableObjectGeneralProperties DlgProperties;
  	DlgProperties.SetName(m_Caption);
  	DlgProperties.SetNumberOfClaps(m_NumberOfClaps);
  	DlgProperties.SetEditingObjectType(GetObjectType());
  	if (DlgProperties.DoModal() == IDOK) {
  	/*Change the label name*/
  		SetCaption(DlgProperties.GetName());
  		SetNumberOfClaps(DlgProperties.GetNumberOfClaps());
  	}
  
  //	CDlgLabelName DlgLabelName;
  //	DlgLabelName.m_edtLabelName = m_Caption;
  //	if (DlgLabelName.DoModal() == IDOK) {
  //	/*Change the label name*/
  //		SetCaption(DlgLabelName.m_edtLabelName);
  //	}
  }
  
  void CPositionLabel::Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid * ProbGrid)
  {
  	const unsigned long cTransparencyColor = 0xFFFFFF;
  	
  	if (!m_IsVisible) return;
  	
  	LPDIRECTDRAWSURFACE3	PositionLabelBmp = NULL;
  
  /*Load Largest bitmap initially*/
  	if (m_lpDDSPositionLabelBmp == NULL) {
  		m_lpDDSPositionLabelBmp = ProbGrid->DDrawLoadBitmap(IDB_POSITION_LABEL3);
  		if( m_lpDDSPositionLabelBmp == NULL ) {
  			fprintf(stderr, "PositionLabel Draw Error!!!! DDrawLoadBitmap failed.  Did not load IDB_POSITION_LABELx\n");
  		}
  		m_OldZoomLevel = cZoomLevel3;
  	}
  	if (ProbGrid->m_ZoomLevel != m_OldZoomLevel) {
  		HRESULT reloadresult;
  		if (ProbGrid->m_ZoomLevel == cZoomLevel3) {
  			reloadresult = ProbGrid->DDrawReloadBitmap(m_lpDDSPositionLabelBmp, IDB_POSITION_LABEL3);
  		} else if (ProbGrid->m_ZoomLevel == cZoomLevel2) {
  			reloadresult = ProbGrid->DDrawReloadBitmap(m_lpDDSPositionLabelBmp, IDB_POSITION_LABEL2);
  		} else {
  			reloadresult = ProbGrid->DDrawReloadBitmap(m_lpDDSPositionLabelBmp, IDB_POSITION_LABEL1);
  		}
  		if (reloadresult != DD_OK) {
  			fprintf(stderr, "PositionLabel Draw Error!!!! DDrawReLoadBitmap failed.  Did not load IDB_POSITION_LABELx\n");
  			ProbGrid->DDrawDiagnostic(reloadresult);
  		}
  	}
  	if (m_lpDDSPositionLabelBmp->IsLost() == DDERR_SURFACELOST) {
  		m_lpDDSPositionLabelBmp->Restore();
  	}
  	m_OldZoomLevel = ProbGrid->m_ZoomLevel;
  	PositionLabelBmp = m_lpDDSPositionLabelBmp;
  /*Load animated bitmaps*/
  	if (m_lpDDSAnimatedPositionLabelBmp == NULL) {
  		m_lpDDSAnimatedPositionLabelBmp = ProbGrid->DDrawLoadBitmap(IDB_POSITION_LABEL30);
  		if( m_lpDDSAnimatedPositionLabelBmp == NULL ) {
  			fprintf(stderr, "PositionLabel Draw Error!!!! DDrawLoadBitmap failed.  Did not load IDB_POSITION_LABELx\n");
  		}
  		m_OldAnimationZoomLevel = cZoomLevel3;
  	}
  	if ( GetAnimate() && ((ProbGrid->m_ZoomLevel != m_OldAnimationZoomLevel) || (m_AnimationFrame != m_OldAnimationFrame)) ) {
  		HRESULT reloadresult;
  		reloadresult = ProbGrid->DDrawReloadBitmap(m_lpDDSAnimatedPositionLabelBmp, GetAnimatedBitmapID(ProbGrid->m_ZoomLevel, m_AnimationFrame));
  		if (reloadresult != DD_OK) {
  			fprintf(stderr, "PositionLabel Draw Error!!!! DDrawReLoadBitmap failed.  Did not load Animation Bitmap IDB_POSITION_LABELxx\n");
  			ProbGrid->DDrawDiagnostic(reloadresult);
  		}
  		if (m_lpDDSAnimatedPositionLabelBmp->IsLost() == DDERR_SURFACELOST) {
  			m_lpDDSAnimatedPositionLabelBmp->Restore();
  		}
  		m_OldAnimationZoomLevel = ProbGrid->m_ZoomLevel;
  		m_OldAnimationFrame = m_AnimationFrame;
  	}
  	if (GetAnimate()) PositionLabelBmp = m_lpDDSAnimatedPositionLabelBmp;
  
  	CRect rcRect, destRect;
  	int PositionLabelBmpFrameWidth, PositionLabelBmpFrameHeight;
  	HRESULT ddrval;
  	if (ProbGrid->m_ZoomLevel == cZoomLevel1) {
  		PositionLabelBmpFrameWidth = cZoom1BmpWidth;
  		PositionLabelBmpFrameHeight = cZoom1BmpHeight;
  		rcRect.left = 0;
  		rcRect.top = 0;
  		rcRect.right = rcRect.left + PositionLabelBmpFrameWidth;
  		rcRect.bottom = rcRect.top + PositionLabelBmpFrameHeight;
  	} else if (ProbGrid->m_ZoomLevel == cZoomLevel2) {
  		PositionLabelBmpFrameWidth = cZoom2BmpWidth;
  		PositionLabelBmpFrameHeight = cZoom2BmpHeight;
  		rcRect.left = 0;
  		rcRect.top = 0;
  		rcRect.right = rcRect.left + PositionLabelBmpFrameWidth;
  		rcRect.bottom = rcRect.top + PositionLabelBmpFrameHeight;
  	} else if (ProbGrid->m_ZoomLevel == cZoomLevel3) {
  		PositionLabelBmpFrameWidth = cZoom3BmpWidth;
  		PositionLabelBmpFrameHeight = cZoom3BmpHeight;
  		rcRect.left = 0;
  		rcRect.top = 0;
  		rcRect.right = rcRect.left + cZoom3BmpWidth;
  		rcRect.bottom = rcRect.top + cZoom3BmpHeight;
  	} else {
  		ASSERT(FALSE);
  		PositionLabelBmpFrameWidth = cZoom1BmpWidth;
  		PositionLabelBmpFrameHeight = cZoom1BmpHeight;
  	}
  /*Draw first corner of zone*/
  	float temp;
  	temp = ProbGrid->RobotToDDSurfaceX(m_EntryPoint.x, ProbabilityGridUpdateRect.left);
  	temp -= PositionLabelBmpFrameWidth/2.0f;
  	destRect.left = Round(temp);
  //		destRect.left = Round((CurrentNode->CalibrationZone->m_P0.x / cWorldGridPointSize + cWorldGridXSize / 2 - ProbGridBmpUpdateRect.left)*m_ZoomLevel - PositionLabelBmpFrameWidth/2.0 );
  	destRect.right = destRect.left + PositionLabelBmpFrameWidth;
  	temp = ProbGrid->RobotToDDSurfaceY(m_EntryPoint.y, ProbabilityGridUpdateRect.top);
  	temp -= PositionLabelBmpFrameHeight/2.0f;
  	destRect.top = Round(temp);
  //		destRect.top = Round((-CurrentNode->CalibrationZone->m_P0.y / cWorldGridPointSize + cWorldGridYSize / 2 - ProbGridBmpUpdateRect.top)*m_ZoomLevel - PositionLabelBmpFrameHeight/2.0 );
  	destRect.bottom = destRect.top + PositionLabelBmpFrameHeight;
  	ProbGrid->RectifyRectangles(&rcRect, &destRect);
  	ProbGrid->DDSetColorKey(PositionLabelBmp, cTransparencyColor);
  	if ( (destRect.Height() != 0) && (destRect.Width() != 0) && (rcRect.Height() != 0) && (rcRect.Width() != 0) ) {
  		while (BackBuffer->GetBltStatus(DDGBS_CANBLT) != DD_OK);
  		ddrval = BackBuffer->BltFast(destRect.left, destRect.top, PositionLabelBmp, rcRect, DDBLTFAST_SRCCOLORKEY);
  		if (ddrval != DD_OK) {
  			ProbGrid->DDrawDiagnostic(ddrval);
  			fprintf(stderr, "***Draw Position Label Error***\n");
  			fprintf(stderr, "destRect = %d %d %d %d\n",destRect.left, destRect.top, destRect.right, destRect.bottom);
  			fprintf(stderr, "rcRect = %d %d %d %d\n", rcRect.left,rcRect.top, rcRect.right, rcRect.bottom);
  			fflush(stderr);
  		}
  	}
  }
  
  unsigned int CPositionLabel::GetAnimatedBitmapID(float ZoomLevel, int AnimationFrame)
  {
  	unsigned int IDBPositionLabel = 0;
  	switch (AnimationFrame) {
  	case 0:
  	case 11:
  		if (ZoomLevel == cZoomLevel3) {
  			IDBPositionLabel = IDB_POSITION_LABEL30;
  		} else if (ZoomLevel == cZoomLevel2) {
  			IDBPositionLabel = IDB_POSITION_LABEL20;
  		} else {
  			IDBPositionLabel = IDB_POSITION_LABEL10;
  		}
  		break;
  	case 1:
  	case 10:
  		if (ZoomLevel == cZoomLevel3) {
  			IDBPositionLabel = IDB_POSITION_LABEL31;
  		} else if (ZoomLevel == cZoomLevel2) {
  			IDBPositionLabel = IDB_POSITION_LABEL21;
  		} else {
  			IDBPositionLabel = IDB_POSITION_LABEL11;
  		}
  		break;
  	case 2:
  	case 9:
  		if (ZoomLevel == cZoomLevel3) {
  			IDBPositionLabel = IDB_POSITION_LABEL32;
  		} else if (ZoomLevel == cZoomLevel2) {
  			IDBPositionLabel = IDB_POSITION_LABEL22;
  		} else {
  			IDBPositionLabel = IDB_POSITION_LABEL12;
  		}
  		break;
  	case 3:
  	case 8:
  		if (ZoomLevel == cZoomLevel3) {
  			IDBPositionLabel = IDB_POSITION_LABEL33;
  		} else if (ZoomLevel == cZoomLevel2) {
  			IDBPositionLabel = IDB_POSITION_LABEL23;
  		} else {
  			IDBPositionLabel = IDB_POSITION_LABEL13;
  		}
  		break;
  	case 4:
  	case 7:
  		if (ZoomLevel == cZoomLevel3) {
  			IDBPositionLabel = IDB_POSITION_LABEL34;
  		} else if (ZoomLevel == cZoomLevel2) {
  			IDBPositionLabel = IDB_POSITION_LABEL24;
  		} else {
  			IDBPositionLabel = IDB_POSITION_LABEL14;
  		}
  		break;
  	case 5:
  	case 6:
  	default:
  		if (ZoomLevel == cZoomLevel3) {
  			IDBPositionLabel = IDB_POSITION_LABEL35;
  		} else if (ZoomLevel == cZoomLevel2) {
  			IDBPositionLabel = IDB_POSITION_LABEL25;
  		} else {
  			IDBPositionLabel = IDB_POSITION_LABEL15;
  		}
  		break;
  		break;
  	}
  	return IDBPositionLabel;
  }
  
  void CPositionLabel::Move(robPOINT TopCorner)
  {
  	TopCorner.x += m_Bounds.right - m_Bounds.left;
  	TopCorner.y -= m_Bounds.top - m_Bounds.bottom;
  	SetEntryPoint(TopCorner);
  }
  
  void CPositionLabel::Resize(TFrameSide DragSide, robPOINT point)
  {
  	return;
  }
  
  void CPositionLabel::SetBounds(robRECT bounds)
  {
  	robPOINT Position;
  	Position.x = bounds.left + cZoom3BmpWidth/cZoomLevel3*cWorldGridPointSize/2;
  	Position.y = bounds.top - cZoom3BmpHeight/cZoomLevel3*cWorldGridPointSize/2;
  	SetEntryPoint(Position);
  }
  
  void CPositionLabel::SetEntryPoint(robPOINT point)
  {
  	CExecutableRWGraphicObject::SetEntryPoint(point);
  	SetExitPoint(point);
  	robRECT Bounds;
  	Bounds.left = point.x - cZoom3BmpWidth/cZoomLevel3*cWorldGridPointSize/2;
  	Bounds.right = Bounds.left + cZoom3BmpWidth/cZoomLevel3*cWorldGridPointSize;
  	Bounds.top = point.y + cZoom3BmpHeight/cZoomLevel3*cWorldGridPointSize/2;
  	Bounds.bottom = Bounds.top - cZoom3BmpHeight/cZoomLevel3*cWorldGridPointSize;
  	m_Bounds = Bounds;
  }
  
  void CPositionLabel::SetCaption(CString Caption)
  {
  	if (Caption == "Home Base") Caption = "Home Base1";
  	CExecutableRWGraphicObject::SetCaption(Caption);
  }
  
  TTaskResult CPositionLabel::Execute(CRobot *Robot, short Speed)
  {
  	SetAnimate(true);
  	TTaskResult TaskResult = CExecutableRWGraphicObject::Execute(Robot, Speed);
  	SetAnimate(false);
  	return TaskResult;
  }
