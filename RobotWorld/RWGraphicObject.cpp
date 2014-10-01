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
  #include "RWGraphicObject.h"
  #include "global.h"
  #include "ExecutableRWGraphicObject.h"
  #include "Probability Grid.h"
  #include "RobotWorld Resource.h"
  #include "RobotWorldView.h"
  #include "PathDisplay.h"
  #include "RulerGrid.h"
  #include "FinishLine.h"
  
  #ifdef _DEBUG
  #undef THIS_FILE
  static char THIS_FILE[]=__FILE__;
  #define new DEBUG_NEW
  #endif
  
  IMPLEMENT_SERIAL(CRWGraphicObject, CObject, VERSIONABLE_SCHEMA | VERSION_NUMBER)
  IMPLEMENT_SERIAL(CRWGraphicObjectList, CObject, VERSIONABLE_SCHEMA | VERSION_NUMBER)
  
  CRWGraphicObject::CRWGraphicObject()
  {
  	m_ArchiveVersionNumber = UINT(-1);
  	m_EditMode = emNone;
  	m_ShowCaption = true;
  	m_IsVisible = true;
  	m_ParentView = NULL;
  	m_Caption = "0";
  	m_Animate = false;
  	m_AnimationFrame = 0;
  }
  
  CRWGraphicObject::~CRWGraphicObject()
  {
  
  }
  
  void CRWGraphicObjectList::Add(CRWGraphicObject * New)
  /*******************************************************************************
  //Created: 10/24/97 S.R.
  ********************************************************************************
  //Description:
  //	Adds New to the head of the list
  *******************************************************************************/
  {
  	if (New == NULL) return;
  	unsigned int Z = New->GetZLayer();
  	if (Z >= nZLayers) Z = nZLayers-1;
  	CRWGraphicObjectListNode* NewNode = new CRWGraphicObjectListNode;
  	NewNode->m_Object = New;
  	if (m_Layers[Z] == NULL) {		//Add to empty list
  		m_Layers[Z] = NewNode;
  	} else {
  		NewNode->m_Next = m_Layers[Z];
  		NewNode->m_Next->m_Previous = NewNode;
  		m_Layers[Z] = NewNode;
  	}
  	New->m_ParentView = m_ProbabilityGrid->m_ViewWindow;
  	switch (New->GetObjectType()) {
  	case goCalibrationZone:
  		New->SetVisible(m_ShowCalibrationZones);
  		break;
  	case goPosition:
  		New->SetVisible(m_ShowPositionLabels);
  		break;
  	case goRedZone:
  		New->SetVisible(m_ShowRedZones);
  		break;
  	case goVacuumPatch:
  		New->SetVisible(m_ShowVacuumPatches);
  		break;
  	case goLinePath:
  		New->SetVisible(m_ShowLinePaths);
  		break;
  	}
  }
  
  void CRWGraphicObjectList::Remove(CRWGraphicObject * RWGraphicObject)
  /*******************************************************************************
  //Created: 10/24/97 S.R.
  //Last Revision: 10/24/97 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	unsigned int Z = RWGraphicObject->GetZLayer();
  	CRWGraphicObjectListNode* CurrentNode = m_Layers[Z];
  	while (CurrentNode != NULL) {
  		if (CurrentNode->m_Object == RWGraphicObject) {
  			if (CurrentNode->m_Previous != NULL) {
  				CurrentNode->m_Previous->m_Next = CurrentNode->m_Next;
  			} else {
  				m_Layers[Z] = CurrentNode->m_Next;
  			}
  			if (CurrentNode->m_Next != NULL) {
  				CurrentNode->m_Next->m_Previous = CurrentNode->m_Previous;
  			}
  			delete CurrentNode;
  			return;
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  }
  
  void CRWGraphicObjectList::Purge(void)
  /*******************************************************************************
  //Created: 10/24/97 S.R.
  //Last Revision: 10/24/97 S.R.
  //Parameters:
  //
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	unsigned int Z;
  	for (Z = 0; Z < nZLayers; Z++) {
  		while (m_Layers[Z] != NULL) {
  			CRWGraphicObjectListNode* temp = m_Layers[Z]->m_Next;
  			delete m_Layers[Z];
  			m_Layers[Z] = temp;
  		}
  	}
  }
  
  CRWGraphicObjectList::CRWGraphicObjectList() : CObject()
  {
  	for (unsigned int i = 0; i < nZLayers; i++) m_Layers[i] = NULL;
  	m_ShowPositionLabels = true;
  	m_ShowRedZones = true;
  	m_ShowVacuumPatches = true;
  	m_ShowCalibrationZones = true;
  	m_ShowLinePaths = true;
  	m_RobotCurrentPath = new CPathDisplay();
  	m_RulerGrid = new CRulerGrid();
  	m_RobotCurrentPath->SetVisible(false);
  	m_RobotWasInsideStartLine = false;
  }
  
  CRWGraphicObjectList::~CRWGraphicObjectList()
  {
  	Purge();
  	delete m_RobotCurrentPath;
  	delete m_RulerGrid;
  }
  
  CRWGraphicObjectListNode::~CRWGraphicObjectListNode()
  {
  	if (m_Object != NULL) {
  		delete m_Object;
  	}
  }
  
  bool CRWGraphicObject::InsideObject(robPOINT point, float WorldPointsPerPixel)
  /*******************************************************************************
  //Created: 09/25/97 S.R.
  //Last Revision: 10/24/97 S.R.
  //Parameters:
  //	point:	point in robot world coordinates.
  //Return Value:
  //	TRUE if point is inside the object.
  //	FALSE if not.
  ********************************************************************************
  //Description:
  //	The function returns TRUE if point is inside the object.  If not, then
  //	FALSE is returned.
  //Method:
  //
  *******************************************************************************/
  {
  	if (m_Bounds.Inside(point)) return true;
  	if (m_ShowCaption) {
  		return (m_CaptionBounds.Inside(point));
  	}
  	return false;
  }
  
  void CRWGraphicObject::Move(robPOINT TopCorner)
  /*******************************************************************************
  //Created: 09/26/97 S.R.
  //Last Revision: 10/24/97 S.R.
  //Parameters:
  //	TopCorner:	The new top corner for the object (in robot world coordinates).
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function moves the object so TopCorner is the new top left
  //	corner for the object.
  //Method:
  //
  *******************************************************************************/
  {
  	m_Bounds.right += TopCorner.x - m_Bounds.left;
  	m_Bounds.left = TopCorner.x;
  	m_Bounds.bottom += TopCorner.y - m_Bounds.top;
  	m_Bounds.top = TopCorner.y;
  }
  
  void CRWGraphicObject::Serialize(CArchive & archive)
  {
  	CObject::Serialize(archive);
  	if (archive.IsStoring()) {
  		archive << m_Bounds.top;
  		archive << m_Bounds.left;
  		archive << m_Bounds.bottom;
  		archive << m_Bounds.right;
  		archive << m_IsVisible;
  		archive << m_Caption;
  		archive << (char)m_ShowCaption;
  	} else {
  		char temp;
  		m_ArchiveVersionNumber = archive.GetObjectSchema();
  		archive >> m_Bounds.top;
  		archive >> m_Bounds.left;
  		archive >> m_Bounds.bottom;
  		archive >> m_Bounds.right;
  		archive >> m_IsVisible;
  		archive >> m_Caption;
  		archive >> temp; m_ShowCaption = temp != 0;
  	}
  }
  
  void CRWGraphicObjectList::DrawObjects(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbabilityGrid)
  /*******************************************************************************
  //Created: 10/24/97 S.R.
  //Last Revision: 10/24/97 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  /*Draw RulerGrid*/
  	CDC* pDC = CProbabilityGrid::DDrawGetSurfaceDC(BackBuffer);
  	m_RulerGrid->Draw(ProbabilityGridUpdateRect, pDC, ProbabilityGrid);
  	CProbabilityGrid::DDrawReleaseSurfaceDC(pDC, BackBuffer);
  
  	for (unsigned int Z = 0; Z < nZLayers; Z++) {
  		if (Z == m_RobotCurrentPath->GetZLayer()) {
  //			m_RobotCurrentPath->Draw(ProbabilityGridUpdateRect, BackBuffer, ProbabilityGrid);
  
  			pDC = CProbabilityGrid::DDrawGetSurfaceDC(BackBuffer);
  			m_RobotCurrentPath->Draw(ProbabilityGridUpdateRect, pDC, ProbabilityGrid);
  			CProbabilityGrid::DDrawReleaseSurfaceDC(pDC, BackBuffer);
  		}
  		CRWGraphicObjectListNode* Current = GetTail(Z);
  		while (Current != NULL) {
  		/*Don't draw object if it isn't inside the update rectangle*/
  			if ( Current->m_Object->InsideUpdateRectangle(ProbabilityGridUpdateRect, ProbabilityGrid) ) {
  				Current->m_Object->DrawObject(ProbabilityGridUpdateRect, BackBuffer, ProbabilityGrid);
  			}
  			Current = Current->m_Previous;
  		}
  	}
  }
  
  CRWGraphicObject* CRWGraphicObjectList::PointInsideObject(robPOINT RobotPoint, float WorldPointsPerPixel)
  /*******************************************************************************
  //Created: 10/24/97 S.R.
  //Last Revision: 10/24/97 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	for (unsigned int Z = nZLayers; Z > 0; Z--) {
  		CRWGraphicObjectListNode* CurrentNode = m_Layers[Z-1];
  		while (CurrentNode != NULL) {
  			if (CurrentNode->m_Object->GetVisible()) {
  				if (CurrentNode->m_Object->InsideObject(RobotPoint, WorldPointsPerPixel)) {
  				/*Bring selected object to tail of list*/
  					MoveToHead(CurrentNode);
  					return CurrentNode->m_Object;
  				}
  			}
  			CurrentNode = CurrentNode->m_Next;
  		}
  	}
  	return NULL;
  }
  
  void CRWGraphicObject::SetBounds(robRECT bounds)
  /*******************************************************************************
  //Created: 10/27/97 S.R.
  //Last Revision: 10/27/97 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	m_Bounds = bounds;
  	m_Bounds.Normalize();
  	if (m_Bounds.top - m_Bounds.bottom < MinHeight()) {
  		m_Bounds.bottom = m_Bounds.top - MinHeight();
  	}
  	if (m_Bounds.right - m_Bounds.left < MinWidth()) {
  		m_Bounds.right = m_Bounds.left + MinWidth();
  	}
  }
  
  void CRWGraphicObjectList::Serialize(CArchive & archive)
  {
  	UINT nVersion;
  	CObject::Serialize(archive);
  	unsigned int Z;
  	if (archive.IsStoring()) {
  		nVersion = VERSION_NUMBER;
  		archive << nVersion;
  		for (Z = 0; Z < nZLayers; Z++) {
  			archive << NumberOfNodesInLayer(Z);
  			CRWGraphicObjectListNode* CurrentNode = m_Layers[Z];
  			while (CurrentNode != NULL) {
  				archive << CurrentNode->m_Object;
  				CurrentNode = CurrentNode->m_Next;
  			}
  		}
  		archive << (char)m_ShowCalibrationZones;
  		archive << (char)m_ShowPositionLabels;
  		archive << (char)m_ShowRedZones;
  		archive << (char)((char)m_ShowVacuumPatches | ((char)m_ShowLinePaths << 1));
  	} else {
  		archive >> nVersion;
  		unsigned int NumberOfNodes;
  		for (Z = 0; Z < nZLayers; Z++) {
  			archive >> NumberOfNodes;
  			while (NumberOfNodes) {
  				CRWGraphicObject* NewObject;
  				archive >> NewObject;
  				Add(NewObject);
  				NumberOfNodes--;
  			}
  		}
  		char temp;
  		archive >> temp; m_ShowCalibrationZones = temp != 0;
  		archive >> temp; m_ShowPositionLabels = temp != 0;
  		archive >> temp; m_ShowRedZones = temp != 0;
  		archive >> temp; m_ShowVacuumPatches = ((temp & 0x01) == 0x01); m_ShowLinePaths = ((temp & 0x02) == 0x02);
  		m_ShowCalibrationZones = true;
  		ShowObjects(goCalibrationZone, m_ShowCalibrationZones);
  		ShowObjects(goPosition, m_ShowPositionLabels);
  		ShowObjects(goRedZone, m_ShowRedZones);
  		ShowObjects(goVacuumPatch, m_ShowVacuumPatches);
  		ShowObjects(goLinePath, m_ShowLinePaths);
  	}
  }
  
  void CRWGraphicObjectList::SerializeV2(CArchive & archive)
  {
  	CObject::Serialize(archive);
  	unsigned int Z;
  	if (archive.IsStoring()) {
  		for (Z = 0; Z < nZLayers; Z++) {
  			archive << NumberOfNodesInLayer(Z);
  			CRWGraphicObjectListNode* CurrentNode = m_Layers[Z];
  			while (CurrentNode != NULL) {
  				archive << CurrentNode->m_Object;
  				CurrentNode = CurrentNode->m_Next;
  			}
  		}
  		archive << (char)m_ShowCalibrationZones;
  		archive << (char)m_ShowPositionLabels;
  		archive << (char)m_ShowRedZones;
  		archive << (char)((char)m_ShowVacuumPatches | ((char)m_ShowLinePaths << 1));
  	} else {
  		unsigned int NumberOfNodes;
  		for (Z = 0; Z < nZLayers; Z++) {
  			archive >> NumberOfNodes;
  			while (NumberOfNodes) {
  				CRWGraphicObject* NewObject;
  				archive >> NewObject;
  				Add(NewObject);
  			/*Add old style finish line to the executable object list*/
  				if (NewObject->GetObjectType() == goFinishLine) {
  					m_ProbabilityGrid->m_ExecutableObjectList->Add((CExecutableRWGraphicObject*)NewObject);
  				}
  				NumberOfNodes--;
  			}
  		}
  		char temp;
  		archive >> temp; m_ShowCalibrationZones = temp != 0;
  		archive >> temp; m_ShowPositionLabels = temp != 0;
  		archive >> temp; m_ShowRedZones = temp != 0;
  		archive >> temp; m_ShowVacuumPatches = ((temp & 0x01) == 0x01); m_ShowLinePaths = ((temp & 0x02) == 0x02);
  		m_ShowCalibrationZones = true;
  		ShowObjects(goCalibrationZone, m_ShowCalibrationZones);
  		ShowObjects(goPosition, m_ShowPositionLabels);
  		ShowObjects(goRedZone, m_ShowRedZones);
  		ShowObjects(goVacuumPatch, m_ShowVacuumPatches);
  		ShowObjects(goLinePath, m_ShowLinePaths);
  	}
  }
  
  unsigned int CRWGraphicObjectList::NumberOfNodesInLayer(unsigned int Z)
  {
  	if (Z >= nZLayers) Z = nZLayers - 1;
  	unsigned int NumberOfNodes = 0;
  	CRWGraphicObjectListNode* CurrentNode = m_Layers[Z];
  	while (CurrentNode != NULL) {
  		CurrentNode = CurrentNode->m_Next;
  		NumberOfNodes++;
  	}
  	return NumberOfNodes;
  }
  
  void CRWGraphicObjectList::ShowObjects(TRWGraphicObjectType ObjectType, bool Show)
  /*******************************************************************************
  //Created: 11/21/97 S.R.
  //Last Revision: 11/21/97 S.R.
  //Parameters:
  //	ObjectType:	the type of objects to hide or show.
  //	Show:			Whether to show or hide the objects.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		Call this function to show or hide a group of objects.
  //Method:
  //
  *******************************************************************************/
  {
  	switch (ObjectType) {
  	case goCalibrationZone:
  		m_ShowCalibrationZones = Show;
  		break;
  	case goPosition:
  		m_ShowPositionLabels = Show;
  		break;
  	case goRedZone:
  		m_ShowRedZones = Show;
  		break;
  	case goVacuumPatch:
  		m_ShowVacuumPatches = Show;
  		break;
  	case goLinePath:
  		m_ShowLinePaths = Show;
  		break;
  	}
  	for (unsigned int Z = 0; Z < nZLayers; Z++) {
  		CRWGraphicObjectListNode* CurrentNode = m_Layers[Z];
  		while (CurrentNode != NULL) {
  			if (CurrentNode->m_Object->GetObjectType() == ObjectType) {
  				CurrentNode->m_Object->SetVisible(Show);
  			}
  			CurrentNode = CurrentNode->m_Next;
  		}
  	}
  }
  
  void CRWGraphicObject::OnErase()
  {
  	return;
  }
  
  robRECT CRWGraphicObject::GetBounds()
  {
  	return m_Bounds;
  }
  
  bool CRWGraphicObjectList::GetShowCalibrationZones()
  {
  	return m_ShowCalibrationZones;
  }
  
  bool CRWGraphicObjectList::GetShowPositionLabels()
  {
  	return m_ShowPositionLabels;
  }
  
  bool CRWGraphicObjectList::GetShowRedZones()
  {
  	return m_ShowRedZones;
  }
  
  bool CRWGraphicObjectList::GetShowVacuumPatches()
  {
  	return m_ShowVacuumPatches;
  }
  
  bool CRWGraphicObjectList::GetShowLinePaths()
  {
  	return m_ShowLinePaths;
  }
  
  void CRWGraphicObject::OnDefaultClick(robPOINT point, float WorldPointsPerPixel, CRobot* Robot)
  {
  	return;
  }
  
  CRWGraphicObjectListNode::CRWGraphicObjectListNode()
  {
  	m_Next = NULL;
  	m_Previous = NULL;
  }
  
  void CRWGraphicObjectList::MoveToTail(CRWGraphicObjectListNode * Node)
  {
  	if (Node->m_Next != NULL) {	//Object not already at tail
  	/*Find Tail*/
  		CRWGraphicObjectListNode* TailNode = Node;
  		while (TailNode->m_Next != NULL) {
  			TailNode = TailNode->m_Next;
  		}
  
  
  		if (Node->m_Previous == NULL) {
  			m_Layers[Node->m_Object->GetZLayer()] = Node->m_Next;
  			Node->m_Next->m_Previous = NULL;
  		} else {
  			Node->m_Previous->m_Next = Node->m_Next;
  			Node->m_Next->m_Previous = Node->m_Previous;
  		}
  		Node->m_Next = NULL;
  		Node->m_Previous = TailNode;
  		TailNode->m_Next = Node;
  	}
  }
  
  void CRWGraphicObjectList::MoveToHead(CRWGraphicObjectListNode * Node)
  {
  	unsigned int Z = Node->m_Object->GetZLayer();
  	if (m_Layers[Z] != Node) {
  		Node->m_Previous->m_Next = Node->m_Next;
  		if (Node->m_Next != NULL) {
  			Node->m_Next->m_Previous = Node->m_Previous;
  		}
  		Node->m_Next = m_Layers[Z];
  		Node->m_Next->m_Previous = Node;
  		Node->m_Previous = NULL;
  		m_Layers[Z] = Node;
  	}
  }
  
  CRWGraphicObjectListNode* CRWGraphicObjectList::GetTail(unsigned int ZLayer)
  {
  	CRWGraphicObjectListNode* Current = m_Layers[ZLayer];
  	if (Current == NULL) return NULL;
  	while (Current->m_Next != NULL) {
  		Current = Current->m_Next;
  	}
  	return Current;
  }
  
  void CRWGraphicObject::Redraw(bool UpdateNow)
  {
  	ASSERT(m_ParentView != NULL);
  	if (m_ParentView == NULL) return;
  	
  	robRECT RobotUpdate = m_Bounds;
  	if (GetShowCaption()) {
  		RobotUpdate.Union(m_CaptionBounds);
  	}
  
  	CRect UpdateRect = m_ParentView->RobotToClientCoords(RobotUpdate);
  	if (UpdateNow) {
  		m_ParentView->RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_UPDATENOW);
  	} else {
  		m_ParentView->RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	}
  }
  
  bool CRWGraphicObject::InsideUpdateRectangle(CRect ProbGridUpdateRect, CProbabilityGrid *ProbGrid)
  {
  #ifdef _DEBUG
  	ASSERT(ProbGrid != NULL);
  #endif
  
  	if (ProbGrid == NULL) return false;
  
  	robRECT RobotUpdate = m_Bounds;
  	if (GetShowCaption()) {
  		RobotUpdate.Union(m_CaptionBounds);
  	}
  
  	CRect BmpBounds;
  
  	BmpBounds.left = ProbGrid->RobotToBitmapX(RobotUpdate.left);
  	BmpBounds.right = ProbGrid->RobotToBitmapX(RobotUpdate.right);
  	BmpBounds.top = ProbGrid->RobotToBitmapY(RobotUpdate.top);
  	BmpBounds.bottom = ProbGrid->RobotToBitmapY(RobotUpdate.bottom);
  	BmpBounds.InflateRect((int)ProbGrid->m_ZoomLevel, (int)ProbGrid->m_ZoomLevel);
  	BmpBounds.NormalizeRect();
  
  	if ( BmpBounds.IntersectRect(&BmpBounds, &ProbGridUpdateRect) ) {
  		return true;
  	} else {
  		return false;
  	}
  }
  
  void CRWGraphicObject::Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbabilityGrid)
  {
  /*Draw the caption*/
  	int cTextGap = 3;
  	if (GetVisible()) {
  		if (GetShowCaption()) {
  			CFont CaptionFont;
  			if (ProbabilityGrid->m_ZoomLevel == cZoomLevel3) {
  				CaptionFont.CreatePointFont(110, "Arial", pDC);
  			} else if (ProbabilityGrid->m_ZoomLevel == cZoomLevel2) {
  				CaptionFont.CreatePointFont(70, "Arial", pDC);
  			} else if (ProbabilityGrid->m_ZoomLevel == cZoomLevel1) {
  				CaptionFont.CreatePointFont(40, "Arial", pDC);
  			}
  
  			CFont* OldFont = pDC->SelectObject(&CaptionFont);
  			CPoint SurfaceOffset = ProbGridBmpUpdateRect.TopLeft();
  			int OldBkMode = pDC->SetBkMode(TRANSPARENT);
  			COLORREF OldTextColor = pDC->SetTextColor(COLORREF(0xFF));
  			CSize CaptionSize = pDC->GetTextExtent(GetCaption());
  			CaptionSize.cy += cTextGap;
  			robPOINT CaptionDimensions;
  			CaptionDimensions.x = ProbabilityGrid->ClientSizeToRobotSize(CaptionSize.cx);
  			CaptionDimensions.y = ProbabilityGrid->ClientSizeToRobotSize(CaptionSize.cy);
  
  			m_CaptionBounds.top = m_Bounds.bottom - ProbabilityGrid->ClientSizeToRobotSize(cTextGap);
  			m_CaptionBounds.bottom = m_CaptionBounds.top - CaptionDimensions.y;
  			m_CaptionBounds.left = m_Bounds.left;
  			m_CaptionBounds.right = m_CaptionBounds.left + CaptionDimensions.x;
  
  			int x = (int)ProbabilityGrid->RobotToDDSurfaceX(m_CaptionBounds.left, SurfaceOffset.x);
  			int y = (int)ProbabilityGrid->RobotToDDSurfaceY(m_CaptionBounds.top, SurfaceOffset.y);
  			pDC->TextOut(x,y,GetCaption());
  			pDC->SetBkMode(OldBkMode);
  			pDC->SetTextColor(OldTextColor);
  			pDC->SelectObject(OldFont);
  		}
  		if (m_EditMode != emNone) {
  			CRectTracker Tracker = SetupTracker(ProbabilityGrid, ProbGridBmpUpdateRect.TopLeft());
  			Tracker.Draw(pDC);
  //			DrawTracker(pDC);
  //			Tracker.Draw(pDC);
  		}
  	}
  }
  
  CString CRWGraphicObject::GetCaption()
  {
  	return m_Caption;
  }
  
  
  unsigned int CRWGraphicObjectList::NumberOfNodes()
  {
  	unsigned int nNodes = 0;
  	for (unsigned int i = 0; i < nZLayers; i++) {
  		nNodes += NumberOfNodesInLayer(i);
  	}
  	return nNodes;
  }
  
  CRectTracker CRWGraphicObject::SetupTracker(CProbabilityGrid* ProbabilityGrid, CPoint SurfaceOffset)
  {
  	CRectTracker Tracker;
  	CRect GraphicObjectClientRect;
  	robPOINT WorldPoint;
  	CPoint ClientPoint;
  /*Set Tracker rect bounds*/
  	robRECT ObjectBounds = GetBounds();
  	WorldPoint.x = ObjectBounds.left;
  	WorldPoint.y = ObjectBounds.top;
  	ClientPoint = ProbabilityGrid->RobotToDDSurface(WorldPoint, SurfaceOffset);
  //	ClientPoint = m_ParentView->RobotToClientCoords(WorldPoint);
  	GraphicObjectClientRect.left = ClientPoint.x;
  	GraphicObjectClientRect.top = ClientPoint.y;
  	WorldPoint.x = ObjectBounds.right;
  	WorldPoint.y = ObjectBounds.bottom;
  	ClientPoint = ProbabilityGrid->RobotToDDSurface(WorldPoint, SurfaceOffset);
  //	ClientPoint = m_ParentView->RobotToClientCoords(WorldPoint);
  	GraphicObjectClientRect.right = ClientPoint.x;
  	GraphicObjectClientRect.bottom = ClientPoint.y;
  	GraphicObjectClientRect.NormalizeRect();
  	Tracker.m_rect = GraphicObjectClientRect;
  /*Set style*/
  	Tracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::solidLine;
  /*Set min width and height*/
  	WorldPoint.x = MinWidth();
  	WorldPoint.y = MinHeight();
  	ClientPoint.x = Round(WorldPoint.x*ProbabilityGrid->m_ZoomLevel/(cWorldGridPointSize));
  	ClientPoint.y = Round(WorldPoint.y*ProbabilityGrid->m_ZoomLevel/(cWorldGridPointSize));
  	if (ClientPoint.x > Tracker.m_sizeMin.cx) Tracker.m_sizeMin.cx = ClientPoint.x;
  	if (ClientPoint.y > Tracker.m_sizeMin.cy) Tracker.m_sizeMin.cy = ClientPoint.y;
  /*Set handle size*/
  	const short cHandleSize = 7;
  	if (Tracker.m_nHandleSize < cHandleSize) Tracker.m_nHandleSize = cHandleSize;
  	return Tracker;
  }
  
  void CRWGraphicObject::OnEditClick(robPOINT point, CRobotWorldView * View)
  {
  	m_EditMode = emSelected;
  
  	CRectTracker Tracker = SetupTracker(View);
  	View->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	if (Tracker.Track(View, View->RobotToClientCoords(point))) {
  		SetBounds(View->ClientToRobotCoords(Tracker.m_rect));
  		View->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  	}
  //	m_EditMode = emNone;
  }
  
  BOOL CRWGraphicObject::SetTrackerCursor(CRobotWorldView* pWnd, UINT nHitTest)
  {
  	CRectTracker Tracker = SetupTracker(pWnd);
  	return Tracker.SetCursor(pWnd, nHitTest);
  }
  
  CRectTracker CRWGraphicObject::SetupTracker(CRobotWorldView* View)
  {
  	CRectTracker Tracker;
  	CRect GraphicObjectClientRect;
  	robPOINT WorldPoint;
  	CPoint ClientPoint;
  /*Set tracker rect bounds*/
  	robRECT ObjectBounds = GetBounds();
  	WorldPoint.x = ObjectBounds.left;
  	WorldPoint.y = ObjectBounds.top;
  	ClientPoint = m_ParentView->RobotToClientCoords(WorldPoint);
  	GraphicObjectClientRect.left = ClientPoint.x;
  	GraphicObjectClientRect.top = ClientPoint.y;
  	WorldPoint.x = ObjectBounds.right;
  	WorldPoint.y = ObjectBounds.bottom;
  	ClientPoint = m_ParentView->RobotToClientCoords(WorldPoint);
  	GraphicObjectClientRect.right = ClientPoint.x;
  	GraphicObjectClientRect.bottom = ClientPoint.y;
  	GraphicObjectClientRect.NormalizeRect();
  	Tracker.m_rect = GraphicObjectClientRect;
  /*Set style*/
  	Tracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::solidLine;
  /*Set min width and height*/
  	WorldPoint.x = MinWidth();
  	WorldPoint.y = MinHeight();
  	ClientPoint.x = Round(WorldPoint.x*View->m_ProbabilityGrid->m_ZoomLevel/(cWorldGridPointSize));
  	ClientPoint.y = Round(WorldPoint.y*View->m_ProbabilityGrid->m_ZoomLevel/(cWorldGridPointSize));
  	if (ClientPoint.x > Tracker.m_sizeMin.cx) Tracker.m_sizeMin.cx = ClientPoint.x;
  	if (ClientPoint.y > Tracker.m_sizeMin.cy) Tracker.m_sizeMin.cy = ClientPoint.y;
  /*Set handle size*/
  	const short cHandleSize = 7;
  	if (Tracker.m_nHandleSize < cHandleSize) Tracker.m_nHandleSize = cHandleSize;
  	return Tracker;
  }
  
  /*void CRWGraphicObject::DrawTracker(CDC * pDC)
  {
  	Tracker.Draw(pDC);
  }
  */
  
  void CRWGraphicObjectList::SetRobotCurrentPath(CCTDPath* CurrentPath)
  {
  	m_RobotCurrentPath->SetPath(CurrentPath);
  }
  
  void CRWGraphicObject::DrawObject(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid)
  {
  	Draw(ProbabilityGridUpdateRect, BackBuffer, ProbGrid);
  
  	CDC* pDC = CProbabilityGrid::DDrawGetSurfaceDC(BackBuffer);
  	Draw(ProbabilityGridUpdateRect, pDC, ProbGrid);
  	CProbabilityGrid::DDrawReleaseSurfaceDC(pDC, BackBuffer);
  	return;
  }
  
  void CRWGraphicObjectList::SetShowRulerGrid(bool Show)
  {
  	m_RulerGrid->SetVisible(Show);
  }
  
  bool CRWGraphicObjectList::GetShowRulerGrid()
  {
  	return m_RulerGrid->GetVisible() == TRUE;
  }
  
  void CRWGraphicObjectList::SetRulerGridSpacing(float Spacing)
  {
  	m_RulerGrid->SetGridSpacing(Spacing);
  }
  
  float CRWGraphicObjectList::GetRulerGridSpacing()
  {
  	return m_RulerGrid->GetGridSpacing();
  }
  
  void CRWGraphicObject::Animate()
  {
  	if (GetAnimate()) {
  		int Frame = GetAnimationFrame();
  		Frame++;
  		if (Frame >= GetNumberAnimationFrames()) {
  			Frame = 0;
  		}
  		SetAnimationFrame(Frame);
  		Redraw(true);
  	}
  }
  
  void CRWGraphicObject::SetAnimate(bool Animate)
  {
  	if ( (m_Animate == true) && (Animate == false) ) Redraw(true);
  	if (Animate == false) {
  		SetAnimationFrame(0);
  	}
  	m_Animate = Animate;
  }
  
  void CRWGraphicObjectList::UpdateStartAndFinishLines(robPOINT RobotPosition, float WorldPointsPerPixel)
  {
  /*Check to see if the robot's inside a start line*/
  	bool InsideStartLine = false;
  	for (unsigned int Z = 0; Z < nZLayers; Z++) {
  		CRWGraphicObjectListNode* Current = GetTail(Z);
  		while (Current != NULL) {
  			if (Current->m_Object->GetObjectType() == goStartLine) {
  				if (Current->m_Object->GetVisible()) {
  					if (Current->m_Object->InsideObject(RobotPosition, WorldPointsPerPixel)) {
  						InsideStartLine = true;
  					}
  				}
  			}
  			Current = Current->m_Previous;
  		}
  	}
  /*If the robot's inside a start line, reset all the finish lines*/
  	if (InsideStartLine) {
  		for (unsigned int Z = 0; Z < nZLayers; Z++) {
  			CRWGraphicObjectListNode* Current = GetTail(Z);
  			while (Current != NULL) {
  				if (Current->m_Object->GetObjectType() == goFinishLine) {
  					if (((CFinishLine*)(Current->m_Object))->GetRunning()) {
  						((CFinishLine*)(Current->m_Object))->SetFinishTime();
  					}
  				}
  				Current = Current->m_Previous;
  			}
  		}
  	} else if (m_RobotWasInsideStartLine) {
  /*if the robot leaves a start line, start timing all the finish lines*/
  		for (unsigned int Z = 0; Z < nZLayers; Z++) {
  			CRWGraphicObjectListNode* Current = GetTail(Z);
  			while (Current != NULL) {
  				if (Current->m_Object->GetObjectType() == goFinishLine) {
  					((CFinishLine*)(Current->m_Object))->SetStartTime();
  				}
  				Current = Current->m_Previous;
  			}
  		}
  	} else {
  /*if a finish line is running and the robot enters it, set the stop time*/
  		for (unsigned int Z = 0; Z < nZLayers; Z++) {
  			CRWGraphicObjectListNode* Current = GetTail(Z);
  			while (Current != NULL) {
  				if (Current->m_Object->GetObjectType() == goFinishLine) {
  					if (Current->m_Object->GetVisible()) {
  						if (Current->m_Object->InsideObject(RobotPosition, WorldPointsPerPixel)) {
  							if (((CFinishLine*)(Current->m_Object))->GetRunning()) {
  								((CFinishLine*)(Current->m_Object))->SetFinishTime();
  							}
  						}
  					}
  				}
  				Current = Current->m_Previous;
  			}
  		}
  	}
  	m_RobotWasInsideStartLine = InsideStartLine;
  }
  
  void CRWGraphicObjectList::UpdateFinishLineTimers()
  {
  	for (unsigned int Z = 0; Z < nZLayers; Z++) {
  		CRWGraphicObjectListNode* Current = GetTail(Z);
  		while (Current != NULL) {
  			if (Current->m_Object->GetObjectType() == goFinishLine) {
  				if (Current->m_Object->GetVisible()) {
  					if ( ((CFinishLine*)(Current->m_Object))->GetRunning() ) {
  						((CFinishLine*)(Current->m_Object))->UpdateTime();
  					}
  				}
  			}
  			Current = Current->m_Previous;
  		}
  	}
  }
  
  void CRWGraphicObjectList::AnimateObjects()
  {
  	bool NeedToRedraw = false;
  	for (unsigned int Z = 0; Z < nZLayers; Z++) {
  		CRWGraphicObjectListNode* Current = GetTail(Z);
  		while (Current != NULL) {
  			if (Current->m_Object->GetAnimate()) {
  				Current->m_Object->Animate();
  				NeedToRedraw = true;
  			}
  			Current = Current->m_Previous;
  		}
  	}
  	if (NeedToRedraw) {
  //		m_ProbabilityGrid->m_ViewWindow->UpdateWindow();
  	}
  }
  
  CRWGraphicObject::CRWGraphicObject(CRWGraphicObject &RWGraphicObject)
  {
  	m_ParentView = RWGraphicObject.m_ParentView;
  	m_AnimationFrame = RWGraphicObject.m_AnimationFrame;
  	m_Animate = RWGraphicObject.m_Animate;
  	m_EditMode = RWGraphicObject.m_EditMode;
  	m_ShowCaption = RWGraphicObject.m_ShowCaption;
  	m_CaptionBounds = RWGraphicObject.m_CaptionBounds;
  	m_Caption = RWGraphicObject.m_Caption;
  	m_Bounds = RWGraphicObject.m_Bounds;
  	m_IsVisible = RWGraphicObject.m_IsVisible;
  }
