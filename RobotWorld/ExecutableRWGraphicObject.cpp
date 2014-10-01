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
 
  // ExecutableRWGraphicObject.cpp: implementation of the CExecutableRWGraphicObject class.
  //
  //////////////////////////////////////////////////////////////////////
  //#include <afxtempl.h>		//	MFC templates
  #include "stdafx.h"
  //#include "robotworld.h"
  #include "ExecutableRWGraphicObject.h"
  #include "Robot.h"
  #include "global.h"
  #include "PathList.h"
  #include "Path.h"
  #include "WorldGridConsts.h"
  #include "Probability Grid.h"
  #include "RobotWorld Resource.h"
  #include "RobotWorldView.h"
  #include "PositionLabel.h"
  #include "LinePath.h"
  #include "GoalSet.h"
  #include "HomeBase.h"
  
  #ifdef _DEBUG
  #undef THIS_FILE
  static char THIS_FILE[]=__FILE__;
  #define new DEBUG_NEW
  #endif
  
  IMPLEMENT_SERIAL(CExecutableRWGraphicObject, CRWGraphicObject, VERSIONABLE_SCHEMA | VERSION_NUMBER)
  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////
  
  CExecutableRWGraphicObject::CExecutableRWGraphicObject() : CRWGraphicObject()
  {
  	m_ParentView = NULL;
  //	m_CaptionDimensions.x = m_CaptionDimensions.y = 340;
  //	m_PathList = new CPathList;
  	m_ExecutableObjectList = NULL;
  	m_ShowCaption = true;
  	m_NumberOfClaps = -1;
  }
  
  CExecutableRWGraphicObject::~CExecutableRWGraphicObject()
  {
  //	delete m_PathList;
  }
  
  bool CExecutableRWGraphicObject::InsideExecuteZone(robPOINT point, float WorldPointsPerPixel)
  {
  	return m_Bounds.Inside(point);
  }
  
  TTaskResult CExecutableRWGraphicObject::Execute(CRobot* Robot, short Speed)
  {
  	Robot->SetStatusBarText("");
  	TRobotInfo RobotInfo = Robot->GetRobotInfo();
  	Robot->SetState(stExecuting);
  	Robot->SetEStopVisible(true);
  	TTaskResult TaskResult;
  	if (Robot->GetOnHomeBase()) {
  		if ( (TaskResult = Robot->BackupFromWall()) != TR_TaskComplete) {
  			goto CLEANUP;
  		}
  	}
  	TaskResult = ExecutePathToEntryPoint(Robot, Speed);
  	if (TaskResult == TR_TaskComplete) {
  		TaskResult = SpecialFunction(Robot, Speed);
  	}
  CLEANUP:
  	Robot->Stop();
  	Robot->SetRobotInfo(RobotInfo);
  	return TaskResult;
  }
  
  double CExecutableRWGraphicObject::DistanceToEntryPoint(robPOINT From)
  {
  	return Distance(From, GetEntryPoint());
  }
  
  Path* CExecutableRWGraphicObject::CreatePathToEntryPoint(robPOINT From, CRobot* Robot)
  {
  	return Robot->CreatePath(From, GetEntryPoint());
  }
  
  CGoalSet* CExecutableRWGraphicObject::FindPathToEntryPoint(robPOINT From, CRobot* Robot, bool& GoodPath)
  {
  	return Robot->FindPathNow(From, GetEntryPoint(), cFindPathIterationsDefault, cFindPathRepeatDefault, true);
  }
  
  TTaskResult CExecutableRWGraphicObject::ExecutePathToEntryPoint(CRobot * Robot, short Speed)
  {
  //	bool Forward;
  	bool GoodPath = false;
  	TTaskResult PathResult = TR_TaskComplete;
  	Path* FoundPath = NULL;
  	CGoalSet* FoundGoalSet = NULL;
  /*See if we're near another RWGraphicObject right now*/
  	assert(m_ExecutableObjectList);
  	CExecutableRWGraphicObject* NearObject = NULL;
  	CLinePath *LinePathTo = NULL;
  	if (DistanceToEntryPoint(Robot->GetPosition()) < 12.0*cTPI) {
  	/*We're right on top of the object.  Try just moving there.*/
  		FoundPath = CreatePathToEntryPoint(Robot->GetPosition(), Robot);
  		GoodPath = true;
  		PathResult = ExecuteFoundPath(FoundPath, Robot, Speed, GoodPath);
  		delete FoundPath;
  		FoundPath = NULL;
  	} else if ( (LinePathTo = m_ExecutableObjectList->GetLinePathBetween(Robot->GetPosition(), this)) != NULL) {
  		PathResult = LinePathTo->Execute(Robot, Speed);
  #ifdef IMPLEMENTED
  	} else if ( (FoundPath = m_ExecutableObjectList->FindPathBetween(Robot->GetPosition(), this, Forward)) != NULL ) {
  		if (Robot->GoodPath(FoundPath, true)) {
  		/*If the path is still good, run it*/
  			GoodPath = true;
  			PathResult = ExecuteFoundPath(FoundPath, Robot, Speed, GoodPath, Forward);
  			if (!GoodPath) m_ExecutableObjectList->RemoveFromStoredPaths(FoundPath);
  		} else {
  		/*If not, find a new one based on the old*/
  			Path* NewPath = new Path(*FoundPath);
  			m_ExecutableObjectList->RemoveFromStoredPaths(FoundPath);
  			FoundPath = Robot->FindPathNow(NewPath, cFindPathIterationsDefault, cFindPathRepeatDefault, true, GoodPath);
  			PathResult = ExecuteFoundPath(FoundPath, Robot, Speed, GoodPath);
  			if (GoodPath) {
  				m_ExecutableObjectList->AddToStoredPaths(FoundPath);
  			} else {
  				delete FoundPath;
  			}
  		}
  	} else if ( (NearObject = m_ExecutableObjectList->PointNearExecutableObjectExit(Robot->GetPosition(), this)) != NULL ) {
  		if (NearObject->GetObjectType() == goLinePath) {
  			FoundGoalSet = FindPathToEntryPoint(((CLinePath*)NearObject)->GetEntryPoint(Robot->GetPosition()), Robot, GoodPath);
  		} else {
  			FoundGoalSet = FindPathToEntryPoint(NearObject->GetExitPoint(), Robot, GoodPath);
  		}
  		PathResult = ExecuteFoundPath(FoundGoalSet, Robot, Speed, GoodPath);
  		delete FoundGoalSet;
  #endif
  	} else {
  /*We're not close to a label.  Just find the path*/
  		FoundGoalSet = FindPathToEntryPoint(Robot->GetPosition(), Robot, GoodPath);
  		if (FoundGoalSet != NULL) {
  			PathResult = Robot->FollowGoalSetWithObstacleAvoidance(FoundGoalSet, Speed, m_ExecutableObjectList->m_ProbabilityGrid);
  		} else {
  			PathResult = TR_EmergencyStop;
  		}
  //		PathResult = ExecuteFoundPath(FoundGoalSet, Robot, Speed, GoodPath);
  		delete FoundGoalSet;
  //		delete FoundPath;
  	}
  	return PathResult;
  }
  
  TTaskResult CExecutableRWGraphicObject::ExecuteFoundPath(Path* FoundPath, CRobot *Robot, short Speed, bool& GoodPath, bool Forward)
  {
  	TTaskResult PathResult = TR_TaskComplete;
  	if (FoundPath != NULL) {
  		if (GoodPath) {
  			Robot->SetObstacleSensitivity(osMediumSensitivity);
  			Robot->AddPathToInstructions(FoundPath, (unsigned int)Speed, cModPositionZone, false, 0, Forward);
  			TInstructionResult InstructionResult;
  			PathResult = TR_TaskComplete;
  			if ( (InstructionResult = Robot->FollowInstructions(TRUE, FALSE)) != IR_InstructionComplete ) {
  				GoodPath = false;
  				if (InstructionResult == IR_EmergencyStop) PathResult = TR_EmergencyStop;
  				else PathResult = TR_Error;
  			/*Try to go around obstacles*/
  				if (InstructionResult == IR_HitObstacle) {
  					robPOINT Destination;
  					if (Forward) {
  						FoundPath->GetPoint(FoundPath->GetNumberOfPoints()-1, Destination.x, Destination.y);
  					} else {
  						FoundPath->GetPoint(0, Destination.x, Destination.y);
  					}
  					PathResult = Robot->GetAroundObstacle(Destination, (unsigned short)Speed, m_ExecutableObjectList->m_ProbabilityGrid);
  				}
  			}
  		} else {
  			Robot->DisplayErrorText("Did not find a good path");
  			m_ParentView->RedrawWindow(NULL, NULL, RDW_INVALIDATE);
  			PathResult = TR_Error;
  		}
  	} else {
  		GoodPath = false;
  		PathResult = TR_EmergencyStop;
  	}
  	return PathResult;
  }
  
  Path* CExecutableRWGraphicObjectList::FindPathBetween(robPOINT Start, CExecutableRWGraphicObject *End, bool& Forward)
  {
  	robPOINT P1, P2;
  	P1 = Start;
  	if (End->GetObjectType() == goLinePath) {
  		P2 = ((CLinePath*)End)->GetEntryPoint(Start);
  	} else {
  		P2 = End->GetEntryPoint();
  	}
  	return m_StoredPathsList.GetPathBetween(P1, P2, Forward);
  }
  
  TTaskResult CExecutableRWGraphicObject::SpecialFunction(CRobot * Robot, short Speed)
  {
  	return TR_TaskComplete;
  }
  
  CExecutableRWGraphicObject* CExecutableRWGraphicObjectList::PointInsideExecutableArea(robPOINT Point, float WorldPointsPerPixel)
  {
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  	while (CurrentNode != NULL) {
  		if (CurrentNode->m_Object->GetVisible()) {
  			if (CurrentNode->m_Object->InsideExecuteZone(Point, WorldPointsPerPixel)) {
  				return CurrentNode->m_Object;
  			}
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  	return NULL;
  }
  
  void CExecutableRWGraphicObject::Serialize(CArchive & archive)
  {
  	CRWGraphicObject::Serialize(archive);
  	if (archive.IsStoring()) {
  		archive << m_EntryPoint.x << m_EntryPoint.y;
  		archive << m_ExitPoint.x << m_ExitPoint.y;
  		archive << m_NumberOfClaps;
  	} else {
  		if (GetArchiveVersionNumber() < 4) {
  			CPathList PathList;
  			PathList.Serialize(archive);
  		}
  		archive >> m_EntryPoint.x >> m_EntryPoint.y;
  		archive >> m_ExitPoint.x >> m_ExitPoint.y;
  		if (GetArchiveVersionNumber() >= 7) {
  			archive >> m_NumberOfClaps;
  		}
  	}
  
  }
  
  void CExecutableRWGraphicObject::SetCaption(CString Caption)
  {
  	assert(m_ExecutableObjectList);
  	if (Caption == "") Caption = "1";
  	CString OldCaption = m_Caption;
  	m_ExecutableObjectList->GetUniqueCaption(&Caption, this);
  	m_Caption = Caption;
  	assert(m_ParentView);
  	m_ParentView->OnObjectCaptionChanged(OldCaption, m_Caption, GetObjectType());
  }
  
  void CExecutableRWGraphicObject::SetExecutableObjectList(CExecutableRWGraphicObjectList * ObjectList)
  {
  	m_ExecutableObjectList = ObjectList;
  }
  
  void CExecutableRWGraphicObjectList::Add(CExecutableRWGraphicObject * New, bool NotifyParent)
  {
  	if (New != NULL) {
  		if (m_Head == NULL) {
  			m_Head = new CExecutableRWGraphicObjectListNode;
  			m_Head->m_Object = New;
  			m_Head->m_Next = NULL;
  		} else {
  			CExecutableRWGraphicObjectListNode* temp = new CExecutableRWGraphicObjectListNode;
  			temp->m_Object = New;
  			temp->m_Next = m_Head;
  			m_Head = temp;
  		}
  		New->SetExecutableObjectList(this);
  		New->SetShowCaption(m_ShowCaptions);
  		New->m_ParentView = (CRobotWorldView*)(m_ProbabilityGrid->m_ViewWindow);
  		if (NotifyParent) {
  			((CRobotWorldView*)(m_ProbabilityGrid->m_ViewWindow))->OnObjectAdded(New->GetCaption(), New->GetObjectType());
  		}
  	}
  }
  
  CExecutableRWGraphicObject* CExecutableRWGraphicObjectList::FindObject(CString Caption, TRWGraphicObjectType ObjectType)
  {
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  	while (CurrentNode != NULL) {
  		if (CurrentNode->m_Object->GetObjectType() == ObjectType) {
  			if (CurrentNode->m_Object->GetCaption() == Caption) {
  				return CurrentNode->m_Object;
  			}
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  	return NULL;
  }
  
  void CExecutableRWGraphicObjectList::GetCaptionLists(CStringList * PositionCaptions, CStringList * CalZoneCaptions, CStringList* VacuumPatchCaptions, CStringList* LinePathCaptions)
  {
  	CRuntimeClass* CalZoneLabelType = RUNTIME_CLASS(CCalibrationZone);
  	CRuntimeClass* PositionLabelType = RUNTIME_CLASS(CPositionLabel);
  	CRuntimeClass* VacuumPatchLabelType = RUNTIME_CLASS(CVacuumPatch);
  	CRuntimeClass* LinePathLabelType = RUNTIME_CLASS(CLinePath);
  	CRuntimeClass* HomeBaseLabelType = RUNTIME_CLASS(CHomeBase);
  
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  	while (CurrentNode != NULL) {
  		if (CurrentNode->m_Object->IsKindOf(PositionLabelType)) {
  			PositionCaptions->AddHead(CurrentNode->m_Object->GetCaption());
  		} else if (CurrentNode->m_Object->IsKindOf(CalZoneLabelType)) {
  			CalZoneCaptions->AddHead(CurrentNode->m_Object->GetCaption());
  			if (CurrentNode->m_Object->IsKindOf(HomeBaseLabelType)) {
  				PositionCaptions->AddHead(CurrentNode->m_Object->GetCaption());
  			}
  		} else if (CurrentNode->m_Object->IsKindOf(VacuumPatchLabelType)) {
  			VacuumPatchCaptions->AddHead(CurrentNode->m_Object->GetCaption());
  		} else if (CurrentNode->m_Object->IsKindOf(LinePathLabelType)) {
  			LinePathCaptions->AddHead(CurrentNode->m_Object->GetCaption());
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  }
  
  void CExecutableRWGraphicObjectList::GetUniqueCaption(CString * Caption, CExecutableRWGraphicObject* Object)
  /*******************************************************************************
  //Description:
  //		Given an object, and a new caption suggestion, this function will find a
  //	unique caption for the object.  Two labels of a certain type cannot have the
  //	same caption, but two labels of differing types can.
  //Method:
  //		The function will append numbers to the end of caption until a unique
  //	caption is obtained.
  *******************************************************************************/
  {
  	unsigned int i = 0;
  	TRWGraphicObjectType ObjectType = Object->GetObjectType();
  //	TLabelType LabelType = Label->m_LabelType;
  	bool UniqueCaption;
  	do {
  		UniqueCaption = true;
  		CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  		while (CurrentNode != NULL) {
  			if (CurrentNode->m_Object->GetObjectType() == ObjectType) {
  				if ( (CurrentNode->m_Object->GetCaption() == *Caption) && (CurrentNode->m_Object != Object) ) {
  				/*Add ones digit when necessary*/
  					if ((i == 0) || (i == 10) || (i == 1000) || (i == 10000)) {
  						*Caption += "0";
  					}
  				/*Write ones digit*/
  					Caption->SetAt(Caption->GetLength() - 1, i%10+48);
  				/*Write correct tens digit*/
  					if (i > 9) {
  						Caption->SetAt(Caption->GetLength() - 2, i%100/10+48);
  					}
  				/*Write correct hundreds digit*/
  					if (i > 99) {
  						Caption->SetAt(Caption->GetLength() - 3, i%1000/100+48);
  					}
  					i++;
  					UniqueCaption = false;
  				}
  			}
  			CurrentNode = CurrentNode->m_Next;
  		}
  	} while (!UniqueCaption && (i != 999) );
  }
  
  void CExecutableRWGraphicObjectList::RemoveObjectFromStoredPaths(CExecutableRWGraphicObject * Object)
  {
  	robPOINT P1, P2;
  	P1 = Object->GetEntryPoint();
  	P2 = Object->GetExitPoint();
  	bool RemoveP1FromList = true;
  	bool RemoveP2FromList = true;
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  	while (CurrentNode != NULL) {
  		if (CurrentNode->m_Object != Object) {
  			if ( Distance(CurrentNode->m_Object->GetEntryPoint(), P1) < cCloseDistance ) {
  				RemoveP1FromList = false;
  			}
  			if ( Distance(CurrentNode->m_Object->GetExitPoint(), P1) < cCloseDistance ) {
  				RemoveP1FromList = false;
  			}
  			if ( Distance(CurrentNode->m_Object->GetEntryPoint(), P2) < cCloseDistance ) {
  				RemoveP2FromList = false;
  			}
  			if ( Distance(CurrentNode->m_Object->GetExitPoint(), P2) < cCloseDistance ) {
  				RemoveP2FromList = false;
  			}
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  	if (RemoveP1FromList) {
  		m_StoredPathsList.RemovePathsTo(Object->GetEntryPoint());
  	}
  	if (RemoveP2FromList) {
  		m_StoredPathsList.RemovePathsTo(Object->GetExitPoint());
  	}
  }
  
  void CExecutableRWGraphicObject::OnErase()
  {
  	assert(m_ExecutableObjectList);
  	m_ExecutableObjectList->Remove(this);
  	m_ExecutableObjectList->RemoveObjectFromStoredPaths(this);
  	assert(m_ParentView);
  	m_ParentView->OnObjectErased(m_Caption, GetObjectType());
  }
  
  void CExecutableRWGraphicObject::SetEntryPoint(robPOINT Point)
  {
  	if (m_ExecutableObjectList) {
  		if ( (m_EntryPoint.x != Point.x) || (m_EntryPoint.y != Point.y) ) m_ExecutableObjectList->RemoveFromStoredPaths(m_EntryPoint);
  	}
  	m_EntryPoint = Point;
  }
  
  bool CExecutableRWGraphicObject::InsideObject(robPOINT point, float WorldPointsPerPixel)
  {
  	if (CRWGraphicObject::InsideObject(point, WorldPointsPerPixel)) return true;
  	return false;
  }
  
  void CExecutableRWGraphicObjectList::Serialize(CArchive & archive)
  {
  	short temp;
  	UINT nVersion;
  	if (archive.IsStoring()) {
  		nVersion = VERSION_NUMBER;
  		archive << nVersion;
  		CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  		archive << NumberOfNodes();
  		while (CurrentNode != NULL) {
  			archive << CurrentNode->m_Object;
  			CurrentNode = CurrentNode->m_Next;
  		}
  		temp = m_ShowCaptions;
  		archive << temp;
  		m_StoredPathsList.Serialize(archive);
  	} else {
  		archive >> nVersion;
  		unsigned int NumberOfNodes;
  		archive >> NumberOfNodes;
  		while (NumberOfNodes) {
  			CExecutableRWGraphicObject* NewObject;
  			archive >> NewObject;
  			Add(NewObject, false);
  			NumberOfNodes--;
  		}
  		archive >> temp;
  		m_ShowCaptions = (temp != 0);
  		ShowCaptions(m_ShowCaptions);
  		if (nVersion > 4) {
  			m_StoredPathsList.Serialize(archive);
  		}
  	}
  }
  
  void CExecutableRWGraphicObjectList::SerializeV2(CArchive & archive)
  {
  	short temp;
  	if (archive.IsStoring()) {
  		CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  		archive << NumberOfNodes();
  		while (CurrentNode != NULL) {
  			archive << CurrentNode->m_Object;
  			CurrentNode = CurrentNode->m_Next;
  		}
  		temp = m_ShowCaptions;
  		archive << temp;
  	} else {
  		unsigned int NumberOfNodes;
  		archive >> NumberOfNodes;
  		while (NumberOfNodes) {
  			CExecutableRWGraphicObject* NewObject;
  			archive >> NewObject;
  			Add(NewObject, false);
  			NumberOfNodes--;
  		}
  		archive >> temp;
  		m_ShowCaptions = (temp != 0);
  		ShowCaptions(m_ShowCaptions);
  	}
  }
  
  void CExecutableRWGraphicObjectList::Purge(void)
  {
  	while (m_Head != NULL) {
  		CExecutableRWGraphicObjectListNode* temp = m_Head->m_Next;
  		delete m_Head;
  		m_Head = temp;
  	}
  	m_StoredPathsList.Purge();
  }
  
  void CExecutableRWGraphicObjectList::Remove(CExecutableRWGraphicObject * GraphicObject)
  {
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  	CExecutableRWGraphicObjectListNode* PreviousNode = NULL;
  	while (CurrentNode != NULL) {
  		if (CurrentNode->m_Object == GraphicObject) {
  			if (PreviousNode != NULL) {
  				PreviousNode->m_Next = CurrentNode->m_Next;
  			} else {
  				m_Head = CurrentNode->m_Next;
  			}
  			delete CurrentNode;
  			return;
  		}
  		PreviousNode = CurrentNode;
  		CurrentNode = CurrentNode->m_Next;
  	}
  }
  
  unsigned int CExecutableRWGraphicObjectList::NumberOfNodes()
  {
  	unsigned int NumberOfNodes = 0;
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  	while (CurrentNode != NULL) {
  		CurrentNode = CurrentNode->m_Next;
  		NumberOfNodes++;
  	}
  	return NumberOfNodes;
  }
  
  CExecutableRWGraphicObject* CExecutableRWGraphicObjectList::PointNearExecutableObjectExit(robPOINT Point, CExecutableRWGraphicObject* Exclude)
  {
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  	const double cCloseEnoughDist = 12*cTPI;
  	double MinDist = cCloseEnoughDist + 10*cTPI;
  	CExecutableRWGraphicObject* NearestObject = NULL;
  	bool FoundOne = false;
  	while ( (CurrentNode != NULL) && (!FoundOne) ) {
  		if (CurrentNode->m_Object != Exclude) {
  			ASSERT(CurrentNode->m_Object != NULL);
  //	if (CurrentNode != NULL) {
  			MinDist = Distance(CurrentNode->m_Object->GetExitPoint(), Point);
  			NearestObject = CurrentNode->m_Object;
  			FoundOne = true;
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  
  	while (CurrentNode != NULL) {
  		if (CurrentNode->m_Object != Exclude) {
  			double CurrentDist = Distance(CurrentNode->m_Object->GetExitPoint(), Point);
  			if (CurrentDist < MinDist) {
  				MinDist = CurrentDist;
  				NearestObject = CurrentNode->m_Object;
  			}
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  /*If Position within 1 foot of the MinDist, call that close enough*/
  	if (MinDist < cCloseEnoughDist) {
  		return NearestObject;
  	} else {
  		return NULL;
  	}
  }
  
  void CExecutableRWGraphicObject::SetExitPoint(robPOINT ExitPoint)
  {
  	if (m_ExecutableObjectList != NULL) {
  		if ( (m_ExitPoint.x != ExitPoint.x) || (m_ExitPoint.y != ExitPoint.y) ) {
  			m_ExecutableObjectList->RemoveFromStoredPaths(m_ExitPoint);
  	//		m_PathList->Purge();
  		}
  	}
  	m_ExitPoint = ExitPoint;
  }
  
  unsigned int CExecutableRWGraphicObjectList::Find3ClosestCalibrationZones(robPOINT Position, CCalibrationZone* CalZonesList[3])
  /*******************************************************************************
  //Created: 12/30/97 S.R.
  //Last Revision: 12/30/97 S.R.
  //Parameters:
  //	Position:	point to measure distances from
  //	CalZonesList:	array that will hold the 3 calibration zones
  //Return Value:
  //	number of unique calibration zones found
  ********************************************************************************
  //Description:
  //		This function will find the 3 closest calibration zones (or less if the calibration
  //	zone list holds less than 3.)  The return value holds the number of unique calibration
  //	zones in CalZonesList.
  //Method:
  //
  *******************************************************************************/
  {
  	CalZonesList[0] = CalZonesList[1] = CalZonesList[2] = NULL;
  	unsigned int nNodes = NumberOfNodes();
  /*Take care of empty lists*/
  	if (nNodes == 0) return 0;
  
  	double distance;
  	double Min0, Min1, Min2;
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  	Min0 = Min1 = Min2 = MAX_DOUBLE;
  //	Min0 = Min1 = Min2 = CurrentNode->m_CalibrationZone->DistanceFromCalibrationZone(Position);
  //	CalZonesList[0] = CalZonesList[1] = CalZonesList[2] = CurrentNode->m_CalibrationZone;
  	for (unsigned int i = 0; i < nNodes; i++) {
  		if (CurrentNode->m_Object->GetObjectType() == goCalibrationZone) {
  			distance = Distance(CurrentNode->m_Object->GetEntryPoint(), Position);
  			if (distance <= Min0) {
  				Min2 = Min1;
  				CalZonesList[2] = CalZonesList[1];
  				Min1 = Min0;
  				CalZonesList[1] = CalZonesList[0];
  				Min0 = distance;
  				CalZonesList[0] = ((CCalibrationZone*)CurrentNode->m_Object);
  			} else if (distance <= Min1) {
  				Min2 = Min1;
  				CalZonesList[2] = CalZonesList[1];
  				Min1 = distance;
  				CalZonesList[1] = ((CCalibrationZone*)CurrentNode->m_Object);
  			} else if (distance <= Min2) {
  				Min2 = distance;
  				CalZonesList[2] = ((CCalibrationZone*)CurrentNode->m_Object);
  			}
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  /*NULL any repetetive calibration zones*/
  //	for ( ; i < 3; i++) {
  //		CalZonesList[i] = NULL;
  //	}
  	if (i < 3) return i;
  	else return 3;
  }
  
  CCalibrationZone* CExecutableRWGraphicObjectList::FindClosestCalibrationZone(robPOINT Position)
  /*******************************************************************************
  //Created: 01/14/97 S.R.
  //Last Revision: 01/14/97 S.R.
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
  	double MinDist = 0.0;
  	CCalibrationZone* MinDistNode = NULL;
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  /*Initialize MinDist and MinDistNode*/
  	while ( (CurrentNode != NULL) && (MinDistNode == NULL) ) {
  		if (CurrentNode->m_Object->GetObjectType() == goCalibrationZone) {
  			MinDist = Distance(CurrentNode->m_Object->GetEntryPoint(), Position);
  			MinDistNode = ((CCalibrationZone*)CurrentNode->m_Object);
  		}
  	}
  	while (CurrentNode != NULL) {
  		if (CurrentNode->m_Object->GetObjectType() == goCalibrationZone) {
  			if ( Distance(CurrentNode->m_Object->GetEntryPoint(), Position) < MinDist ) {
  				MinDist = Distance(CurrentNode->m_Object->GetEntryPoint(), Position);
  				MinDistNode = ((CCalibrationZone*)CurrentNode->m_Object);
  			}
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  	return MinDistNode;
  }
  
  
  void CExecutableRWGraphicObjectList::ShowCaptions(bool Show)
  {
  	m_ShowCaptions = Show;
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  	while (CurrentNode != NULL) {
  		CurrentNode->m_Object->SetShowCaption(Show);
  		CurrentNode = CurrentNode->m_Next;
  	}
  }
  
  void CExecutableRWGraphicObject::OnDefaultClick(robPOINT point, float WorldPointsPerPixel, CRobot* Robot)
  {
  	if (InsideExecuteZone(point, WorldPointsPerPixel)) {
  		TRobotInfo OldRobotInfo = Robot->GetRobotInfo();
  //		bool OldEStopVisible = Robot->GetEStopVisible();
  		Robot->SetEStopVisible(true);
  		Execute(Robot, cComputerControlSpeed);
  		Robot->Stop();
  //		Robot->SetEStopVisible(OldEStopVisible);
  		Robot->SetRobotInfo(OldRobotInfo);
  	}
  }
  
  CExecutableRWGraphicObject::CExecutableRWGraphicObject(CExecutableRWGraphicObject &ExecutableRWGraphicObject) : CRWGraphicObject(ExecutableRWGraphicObject)
  {
  //	m_PathList = new CPathList;
  	m_ExitPoint = ExecutableRWGraphicObject.m_ExitPoint;
  	m_ExecutableObjectList = ExecutableRWGraphicObject.m_ExecutableObjectList;
  	m_EntryPoint = ExecutableRWGraphicObject.m_EntryPoint;
  	m_NumberOfClaps = ExecutableRWGraphicObject.m_NumberOfClaps;
  }
  
  CLinePath* CExecutableRWGraphicObjectList::GetLinePathBetween(robPOINT Start, CExecutableRWGraphicObject *Destination)
  {
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  	CLinePath *LinePath = NULL;
  	while ( (CurrentNode != NULL) && (LinePath == NULL) ) {
  		if (CurrentNode->m_Object->GetObjectType() == goLinePath) {
  			if ( ((CLinePath*)(CurrentNode->m_Object))->IsPathBetween(Start, Destination) ) {
  				LinePath = ((CLinePath*)(CurrentNode->m_Object));
  			}
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  	return LinePath;
  }
  
  Path* CExecutableRWGraphicObjectList::GetPathBetween(robPOINT P1, robPOINT P2, bool& Forward)
  {
  	return m_StoredPathsList.GetPathBetween(P1, P2, Forward);
  }
  
  void CExecutableRWGraphicObjectList::AddToStoredPaths(Path *StorePath)
  {
  	m_StoredPathsList.Add(StorePath);
  	return;
  }
  
  void CExecutableRWGraphicObjectList::RemoveFromStoredPaths(Path *RemovePath)
  {
  	m_StoredPathsList.Remove(RemovePath);
  }
  
  void CExecutableRWGraphicObjectList::RemoveFromStoredPaths(robPOINT Point)
  {
  	m_StoredPathsList.RemovePathsTo(Point);
  }
  
  void CExecutableRWGraphicObjectList::GetNumberOfClapsUsedList(CString ObjectType[], CString ObjectName[])
  {
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  	while (CurrentNode != NULL) {
  		int NumberOfClaps = CurrentNode->m_Object->GetNumberOfClaps();
  		if (NumberOfClaps >= cMinNumberOfClaps) {
  			ASSERT(NumberOfClaps <= cMaxNumberOfClaps);
  			ObjectType[NumberOfClaps-1] = ObjectTypeToString(CurrentNode->m_Object->GetObjectType());
  			ObjectName[NumberOfClaps-1] = CurrentNode->m_Object->GetCaption();
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  }
  
  int CExecutableRWGraphicObject::GetNumberOfClaps()
  {
  	ASSERT( ( (m_NumberOfClaps >= cMinNumberOfClaps ) || (m_NumberOfClaps < 0) ) && (m_NumberOfClaps <= cMaxNumberOfClaps) );
  	return m_NumberOfClaps;
  }
  
  void CExecutableRWGraphicObject::SetNumberOfClaps(int Claps)
  {
  	ASSERT( ( (Claps >= cMinNumberOfClaps ) || (Claps < 0) ) && (Claps <= cMaxNumberOfClaps) );
  	ASSERT(m_ExecutableObjectList != NULL);
  	if (Claps > 0) {
  		if (m_ExecutableObjectList != NULL) {
  			CExecutableRWGraphicObject* Object = m_ExecutableObjectList->GetObjectWithNumberOfClaps(Claps);
  			if ( (Object != NULL) && (Object != this) ) {
  				Object->SetNumberOfClaps(-1);
  			}
  		}
  	}
  	m_NumberOfClaps = Claps;
  }
  
  CString CExecutableRWGraphicObjectList::ObjectTypeToString(TRWGraphicObjectType ObjectType)
  {
  	CString Name;
  	switch (ObjectType) {
  	case goNoObject:
  		ASSERT(FALSE);
  		Name = "No Object";
  		break;
  	case goVacuumPatch:
  		Name = "Vacuum Path";
  		break;
  	case goRedZone:
  		Name = "Danger Zone";
  		break;
  	case goCalibrationZone:
  		Name = "Check Point";
  		break;
  	case goPosition:
  		Name = "Hot Point";
  		break;
  	case goLinePath:
  		Name = "Line Path";
  		break;
  	case goDisplayPath:
  		Name = "Display Path";
  		break;
  	case goFinishLine:
  		Name = "Finish Line";
  		break;
  	case goStartLine:
  		Name = "Start Line";
  		break;
  	default:
  		ASSERT(FALSE);
  		break;
  	};
  	return Name;
  }
  
  CExecutableRWGraphicObject* CExecutableRWGraphicObjectList::GetObjectWithNumberOfClaps(int NumberOfClaps)
  {
  	CExecutableRWGraphicObject* Object = NULL;
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  	while ( (CurrentNode != NULL) && (Object == NULL) ) {
  		if (CurrentNode->m_Object->GetNumberOfClaps() == NumberOfClaps) {
  			Object = CurrentNode->m_Object;
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  	return Object;
  }
  
  void CExecutableRWGraphicObject::Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbabilityGrid)
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
  			CString Caption = GetCaption();
  			if (m_NumberOfClaps > 1) {
  				CString NumberOfClaps;
  				NumberOfClaps.Format(", %d Claps", m_NumberOfClaps);
  				Caption += NumberOfClaps;
  			}
  			CFont* OldFont = pDC->SelectObject(&CaptionFont);
  			CPoint SurfaceOffset = ProbGridBmpUpdateRect.TopLeft();
  			int OldBkMode = pDC->SetBkMode(TRANSPARENT);
  			COLORREF OldTextColor = pDC->SetTextColor(COLORREF(0xFF));
  			CSize CaptionSize = pDC->GetTextExtent(Caption);
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
  			pDC->TextOut(x,y,Caption);
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
  
  
  
  CHomeBase* CExecutableRWGraphicObjectList::GetClosestHomeBase(robPOINT Position)
  {
  	return (CHomeBase*)GetClosestObject(Position, RUNTIME_CLASS(CHomeBase));
  }
  
  CExecutableRWGraphicObject* CExecutableRWGraphicObjectList::GetClosestObject(robPOINT Position, TRWGraphicObjectType ObjectType)
  /******************************************************************************
  This function returns the executable map object that is closest to the given
  position, and is of the given type.  The minimized distance is the straight line
  distance not the path distance.
  ******************************************************************************/
  {
  	double MinDist = 0.0;
  	CExecutableRWGraphicObjectListNode* MinDistNode = NULL;
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  /*Initialize MinDist and MinDistNode*/
  	while ( (CurrentNode != NULL) && (MinDistNode == NULL) ) {
  		if (CurrentNode->m_Object->GetObjectType() == ObjectType) {
  			MinDist = Distance(CurrentNode->m_Object->GetEntryPoint(), Position);
  			MinDistNode = CurrentNode;
  		}
  	}
  	while (CurrentNode != NULL) {
  		if (CurrentNode->m_Object->GetObjectType() == ObjectType) {
  			if ( Distance(CurrentNode->m_Object->GetEntryPoint(), Position) < MinDist ) {
  				MinDist = Distance(CurrentNode->m_Object->GetEntryPoint(), Position);
  				MinDistNode = CurrentNode;
  			}
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  	return MinDistNode->m_Object;
  }
  
  CExecutableRWGraphicObject* CExecutableRWGraphicObjectList::GetClosestObject(robPOINT Position, CRuntimeClass *ObjectType)
  /******************************************************************************
  This function returns the executable map object that is closest to the given
  position, and is of the given type.  The minimized distance is the straight line
  distance not the path distance.
  ******************************************************************************/
  {
  	double MinDist = 0.0;
  	CExecutableRWGraphicObjectListNode* MinDistNode = NULL;
  	CExecutableRWGraphicObjectListNode* CurrentNode = m_Head;
  /*Initialize MinDist and MinDistNode*/
  	while ( (CurrentNode != NULL) && (MinDistNode == NULL) ) {
  		if (CurrentNode->m_Object->IsKindOf(ObjectType)) {
  			MinDist = Distance(CurrentNode->m_Object->GetEntryPoint(), Position);
  			MinDistNode = CurrentNode;
  		}
  	}
  	while (CurrentNode != NULL) {
  		if (CurrentNode->m_Object->IsKindOf(ObjectType)) {
  			if ( Distance(CurrentNode->m_Object->GetEntryPoint(), Position) < MinDist ) {
  				MinDist = Distance(CurrentNode->m_Object->GetEntryPoint(), Position);
  				MinDistNode = CurrentNode;
  			}
  		}
  		CurrentNode = CurrentNode->m_Next;
  	}
  	return MinDistNode->m_Object;
  }
