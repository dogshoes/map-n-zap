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
  //Robot.cpp
  //Created: 03/29/96 S.R.
  ********************************************************************************
  //Description:
  //This file contains the functions used to draw and control the robot.
  *******************************************************************************/
  
  /*******************************************************************************
  //Created: 01/26/98 S.R.
  //Last Revision: 01/26/98 S.R.
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
  
  #include "stdafx.h"
  #include "global.h"
  #include <math.h>
  #include <assert.h>
  #include "RobotWorld.h"
  #include "RobotWorld Resource.h"
  #include "DlgEStop.h"
  #include "RothTime.h"
  #include "Robot.h"
  #include "stdafx.h"
  #include "RobotWorldDoc.h"
  #include "RobotWorldView.h"
  #include "Red Zone.h"
  #include "MainFrm.h"
  #include "Calibration Zone.h"
  #include "Path.h"
  #include "CTDPath.h"
  #include "Probability Grid.h"
  #include "DlgCalibrationSuccessful.h"
  #include "World.h"
  #include "Rover.h"
  #include "Robotcom.h"
  #include "RothMath.h"	// Added by ClassView
  #include "Probability Grid.h"
  #include "DlgRobotError.h"
  #include "RobotInstruction.h"
  #include "GoalSet.h"
  #include "HomeBase.h"
  #include "PathDisplay.h"
  #include "CyePersona.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  //#pragma warning( once : 4244 )        //Issue warning 4244 only once.
  //#pragma warning( once : 4800 )
  
  //const long cInfiniteRetry = 100;
  const long cInfiniteRetry = 0;
  const double cMinHeadingError = 2.0/cNHeadings * 2 * pi;
  #define GetMainFrame() ((CMainFrame*)(m_Parent->GetParentFrame()))
  
  void CRobot::FindPath(robPOINT Start, robPOINT End, int Iterations, int Repeat, bool ShowPaths, TFoundPathFunction FoundPathFunction, UINT FoundPathParam1, void* FoundPathParam2)
  /*******************************************************************************
  //Created: 01/22/98 S.R.
  //Last Revision: 01/22/98 S.R.
  //Parameters:
  //	Start, End	start and end points of the path
  //	Iterations	number of iterations the findpath algorithm should take
  //	Repeat		number of times FindPath should start again.
  //	ShowPaths	true if you want the paths to be shown on the screen.
  //	FoundPathFunction	Function to be called when find path thread done.
  //	FoundPathParam1, FoundPathParam2:	parameters to FoundPathFunction
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		Given a start and end point, this function will try to find a clear path.
  //Method:
  //
  *******************************************************************************/
  {
  	SetState(stFindPath);
  	m_FindPathThread->AddToQueue(Start, End, Iterations, Repeat, ShowPaths, 
  		m_Parent->m_ProbabilityGrid, m_Parent->GetSafeHwnd(),
  		FoundPathFunction, FoundPathParam1, FoundPathParam2);
  }
  
  void CRobot::FindPathNowCallback(CGoalSet* GoalSet, UINT Parm0, void* Parm1) {
  /*******************************************************************************
  //Created: 01/26/98 S.R.
  //Last Revision: 01/26/98 S.R.
  //Parameters:
  //	Path:	path found
  //	Parm0, Parm1:	parameters to this function.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This is a callback function for the FindPath thread.
  //Method:
  //
  *******************************************************************************/
  	CRobot* Robot = (CRobot*)Parm1;
  	Robot->m_FoundPathNow++;
  	Robot->m_FindPathNowGoalSet = GoalSet;
  }
  
  bool globalFindPathNowAlreadyRunning = false;
  
  CGoalSet* CRobot::FindPathNow(robPOINT Start, robPOINT End, int Iterations, int Repeat, bool ShowPaths)
  /*******************************************************************************
  //Created: 01/26/98 S.R.
  //Last Revision: 01/26/98 S.R.
  //Parameters:
  //	Start, End	start and end points of the path
  //	Iterations	number of iterations the findpath algorithm should take
  //	Repeat		number of times FindPath should start again.
  //	ShowPaths	true if you want the paths to be shown on the screen.
  //	GoodPath		true if found path is a good path
  //Return Value:
  //	The path found.
  ********************************************************************************
  //Description:
  //		This function finds a path between the points Start and End.  The function
  //	returns when the path has been found.
  //Method:
  //		This function cannot be called while a previous FindPathNow is still running.
  //	That's because if another FindPathNow's thread finishes soon after this
  //	FindPathNow's thread finishes, the second FindPathNow will overwrite the 
  //	m_FindPathPath variable before the first function has a chance to return
  //	its value.  Then both FindPathNow functions will return the second FindPathNow's
  //	path.  If I want to allow multiple simultaneous FindPathNow calls I'll have
  //	to implement some kind of queue for the found paths with some kind of unique
  //	identifier for each of the paths found.
  *******************************************************************************/
  {
  /*Make sure FindPathNow isn't already called.  See note above*/
  	ASSERT( !globalFindPathNowAlreadyRunning );
  	globalFindPathNowAlreadyRunning = true;
  	SetState(stFindPath);
  
  	m_FindPathThread->AddToQueue(Start, End, Iterations, Repeat, ShowPaths, 
  		m_Parent->m_ProbabilityGrid, m_Parent->GetSafeHwnd(),
  		FindPathNowCallback, m_FoundPathNow+1, this);
  	while (!m_FoundPathNow) {
  		DoWindowMessages();
  		WaitMessage();
  	}
  	m_FoundPathNow--;
  	globalFindPathNowAlreadyRunning = false;
  	return m_FindPathNowGoalSet;
  }
  
  bool CRobot::IsInside(robPOINT* point)
  /*******************************************************************************
  //Created: 04/15/96 S.R.
  //Last Revision: 09/12/96 S.R.
  //Parameters:
  //	point		x,y position of test point
  //Return Value:
  //	TRUE	if point is inside the drawn robot
  //	FALSE	if point is outside the drawn robot
  ********************************************************************************
  //Description:
  //		This function determines if a point is inside the drawn robot.
  //Method:
  //		The function first gets the coordinates of the four
  //	corners of the robot.  Next the function determines if point lies inside the four
  //	corners of the robot.  This is done by taking the cross product of a vector
  //	from one of the corners to the point with a vector from that corner to the
  //	next corner.  If these cross products are all >= 0 then the point is inside.
  //	This is done for all four corners.
  *******************************************************************************/
  {
  	const double THETA = atan(2*cRRNoseLength/cRRWidth);
  
  	robPOINT CornerPoint[4];
  	GetCorners(CornerPoint);
  /*See if point is inside the robot*/
  	robPOINT V1,V2;
  	bool Inside = TRUE;
  	for (int i = 0; ((i<3) && (Inside)); i++) {
  		V1.x = CornerPoint[i+1].x - CornerPoint[i].x;
  		V1.y = CornerPoint[i+1].y - CornerPoint[i].y;
  		V2.x = point->x - CornerPoint[i].x;
  		V2.y = point->y - CornerPoint[i].y;
  		if (XPROD(V1,V2) < 0) Inside = FALSE;
  	}
  	V1.x = CornerPoint[0].x - CornerPoint[3].x;
  	V1.y = CornerPoint[0].y - CornerPoint[3].y;
  	V2.x = point->x - CornerPoint[3].x;
  	V2.y = point->y - CornerPoint[3].y;
  	if (XPROD(V1,V2) < 0) Inside = FALSE;
  	return Inside;
  }
  
  CRobot::CRobot(CRobotWorldView* Parent, CProbabilityGrid* ProbGrid)
  /*******************************************************************************
  //Created: 05/13/96 S.R.
  //Last Revision: 01/26/98 S.R.
  //Parameters:
  //	Parent:	The window that is the parent for the robot
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //	Initializes the CRobot class.
  //Method:
  //	Initializes all the member variables.
  *******************************************************************************/
  {
  	m_CyePersona  = NULL;
  	m_Version = 0;
  	if (!((CRobotWorldApp*)AfxGetApp())->GetColorAssociatedAddresses()) {
  		m_Version = 1;
  	} else {
  		m_Version = ((CRobotWorldApp*)AfxGetApp())->GetDefaultRobotVersion();
  	}
  	m_InRedZone = false;
  	m_FirstPositionReceived = true;
  	m_RobotComm = NULL;
  	m_Color = crBlack;
  	m_GoingHome = false;
  	m_AnimateRobotVr = 0;
  	m_AnimateRobotVl = 0;
  	m_BatteryLowTime = 0;
  	m_GoHomeOnLowBattery = true;
  	m_AnimateRobotStepNumber = 0;
  	m_ReceivedNewPosition = false;
  	m_DrawPosition.x = 0.0; m_DrawPosition.y = 0;
  	m_DrawHeading = 0.0;
  	m_PositionWasReset = false;
  	m_HeadingWasReset = false;
  	m_OldPosition.x = m_OldPosition.y = 0;
  	m_OldHeading = 0.0;
  	m_ExternalOutput = false;
  	m_Address = 0;
  	m_BatteryChargeState = 0;
  	m_EnableRepositionOnUnknownObstacle = true;
  	m_SoftwareRevision = 0.0f;
  	m_ExternalInputVoltage = 0;
  	m_StartWaitingForExternalInputChange = false;
  	m_ExternalInputVoltage = 0;
  	m_StartWaitingForExternalInputChange = false;
  	m_M0CurrentMeasured = 0;
  	m_M1CurrentMeasured = 0;
  	m_M1VelocityMeasured = 0;
  	m_M0VelocityMeasured = 0;
  	m_M0AccelerationMeasured = 0;
  	m_M1AccelerationMeasured = 0;
  	m_ShowCurrentPath = true;
  	m_CommunicationsEstablished = false;
  	m_ObstacleSensitivity = osMediumSensitivity;
  	m_FindPathBuffer = cDefaultPathBuffer;
  	m_EnableObstacleBounce = true;
  //	m_VacuumAttached = false;
  	m_Pause = false;
  	m_ExecutingRobotTask = false;
  	m_RobotInstructionResult.Result = IR_InstructionComplete;
  	m_RobotInstructionResult.Valid = false;
  	m_OnHomeBase = false;
  	m_TouchedHomeBase = false;
  	m_ShowCalibrationSuccessfulDialog = false;
  //Create the EStop dialog
  	m_FlipHandle = false;
  	m_DlgEStop = new CDlgEStop();
  	m_DlgEStop->Create(IDD_ESTOP, Parent);
  	m_DlgEStop->m_Robot = this;
  
  	m_Parent = Parent;
  	m_ProbabilityGrid = ProbGrid;
  	m_HandleLength = cRRDefaultHandleLength;
  /*Initialize destination*/
  	m_Destination.x = 0;
  	m_Destination.y = 0;
  /*Initialize heading*/
  	m_Heading = 0.0f;
  /*Initialize position*/
  	m_Position.x = 0;
  	m_Position.y = 0;
  /*Initialize instruction list*/
  	m_InstructionList = new InstructionListHead;
  	m_InstructionList->Head = NULL;
  	m_InstructionList->Tail = NULL;
  /*Initialize Motor Obstacle*/
  	m_MotorObstacle.ValidObstacle = FALSE;
  /*Initialize Nudges*/
  	m_Nudged = NDG_None;
  /*Create robot error dialog*/
  	m_DlgRobotError = new CDlgRobotError;
  	m_DlgRobotError->Create(IDD_ROBOT_ERROR, Parent);
  /*Initialize Errors*/
  	m_PositionError.x = 0;
  	m_PositionError.y = 0;
  	m_HeadingError = 0;
  //	m_KHeadErr = 0.0028f;
  	m_State = stWaiting;
  	m_UpdateProbabilityGrid = TRUE;
  	m_FoundPathNow = false;
  	m_FindPathNowGoalSet = NULL;
  /*Initialize the find path thread*/
  	m_FindPathThread  = new CFindPathThread(this, Parent);
  	m_MaxSpeed = cMaxSpeed/2;
  
  	m_Kp = cInitKp;
  	m_Ki = cInitKi;
  	m_Kd = cInitKd;
  	m_Kb = cInitKb;
  	m_Kpwm = cInitKpwm;
  //	m_OutFile = fopen("robot.dat", "w");
  	m_IntermittentBeepOn = false;
  	SetAttachmentType(atNone);
  	m_FloorType = ftThinCarpet;
  }
  
  void CRobot::Create(CRobotWorldView* Parent)
  {
  /*Initialize RobotComm*/
  	m_RobotComm = new RobotCom(Parent, this);
  	m_CyePersona = new CCyePersona(Parent);
  	m_CyePersona->SetRobot(this);
  	m_CyePersona->Create(IDD_CYE_PERSONA, Parent);
  	if (m_Version == 1) {
  	/*With old style robots at 9600 baud, the response time out needs to be 200ms*/
  		SetCommResponseTimeOut(200);
  	}
  
  }
  
  CRobot::~CRobot()
  /*******************************************************************************
  //Created: 08/20/96 S.R.
  //Last Revision: 12/06/96 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //	Deletes the CRobot class.
  //Method:
  //	deletes things from the heap that need deleting.
  *******************************************************************************/
  {
  	PurgeInstructions();
  	delete m_InstructionList;
  	m_InstructionList = NULL;
  	delete m_RobotComm;
  	delete m_FindPathThread;
  	delete m_DlgEStop;
  	delete m_DlgRobotError;
  	delete m_CyePersona;
  //	fflush(m_OutFile);
  //	fclose(m_OutFile);
  }
  
  void CRobot::AddPositionBungeeToInstructions(robPOINT Destination, TCoordSystem* CoordinateSystem, double Zone, bool FlipHandle, unsigned long TimeOut)
  /*******************************************************************************
  //Created: 06/21/96 S.R.
  //Last Revision: 04/10/97 S.R.
  //Parameters:
  //	Destination:		The robot's destination to place on the instruction list
  //	CoordinateSystem:	The displaced coordinate system Destination is in.  A
  //							NULL value means Destination is in a non-displaced system.
  //	Zone:					The radius of the zone around the destination the robot must
  //							reach before going onto the next point
  //	FlipHandle			The value of FlipHandle when going to destination
  //	TimeOut				The maximum amount of time the robot is allowed to get to its
  //							position (in ms).  Value = 0 represent an infinite time out.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function adds a bungee destination to the robot's instruction list.  The
  //	bungee destination node causes the robot to go to a certain destination using
  //	pwm control.  The pwm control makes stalls more likely, and obstacle detection
  //	is harder.
  //Method:
  //		First the function calculates the position of Destination in the
  //	non-displaced coordinate sysstem.  Then a new CInstructionListNode is created and all
  //	the member values are set.  Then this node is added to the instruction list.
  *******************************************************************************/
  {
  	if (CoordinateSystem != NULL) {
  		Destination = TransformToNonDisplacedCoordinates(CoordinateSystem, Destination);
  	}
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->Position = Destination;
  	NewNode->InstructionNodeType = inPositionBungeeNode;
  	NewNode->SetTimeOut(TimeOut);
  	if (Zone <= cMinPositionZone) NewNode->Zone = cMinPositionZone;
  	else NewNode->Zone = Zone;
  	NewNode->bParm0 = FlipHandle;
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::PurgeInstructions(void)
  /*******************************************************************************
  //Created: 07/01/96 S.R.
  //Last Revision: 07/01/96 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function purges the robot's path list
  //Method:
  //		This function calls RemoveHeadFromInstructions() until the head is NULL.
  *******************************************************************************/
  {
  	while (m_InstructionList->Head != NULL) RemoveHeadFromInstructions();
  }
  
  void CRobot::SetDrag(bool value, CWnd* DispWin)
  /*******************************************************************************
  //Created: 08/19/96 S.R.
  //Last Revision: 12/19/96 S.R.
  //Parameters:
  //	value:	the value to set the Drag parameter to.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		Any time the drag parameter is set to true the function performs certain
  //	functions that must be done so the robot is dragged correctly.
  //Method:
  //		If value == TRUE the robot's path is purged, the max pwm isset to 255 and
  //	m_State is set to Dragging.  If value == FALSE then m_State is set to Waiting.
  *******************************************************************************/
  {
  	if (value == TRUE) {
  //		EStop();
  	/*Set the max speed so the user can race the robot around*/
  //		SetMaxSpeed(m_MaxSpeed);
  		SetState(stDragging);
  		DispWin->SetCapture();
  	} else {
  		ReleaseCapture();
  		if (m_State == stDragging) {
  			SetState(stWaiting);
  		}
  	}
  }
  
  void CRobot::AddTurnToInstructions(double Heading, short TurnDiameter, TCoordSystem* CoordinateSystem, double Zone, short MaxSpeed, unsigned long TimeOut)
  /*******************************************************************************
  //Created: 09/17/97 S.R.
  //Last Revision: 09/17/97 S.R.
  //	Heading:				The Heading to turn to.
  //	TurnDiameter:			The turning diameter to use when turning. (Distance between
  //								robot origins when robot turns 180 degrees) (in 0.1 inches.)
  //	CoordinateSystem		The coordinate system Heading is in.  A NULL value means Heading is
  //								in a non-rotated coordinate system.
  //	Zone:					How close to Heading the robot must be before going to the next node
  //								in the instruction list
  //	MaxSpeed:				The maximum Speed to use when adjusting the Heading.
  //	TimeOut:				The maximum amount of time the robot is allowed to get to the 
  //								final Heading.  Value = 0 indicates an infinite time out.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //
  //Method:
  //
  *******************************************************************************/
  {
  	if (CoordinateSystem != NULL) {
  		Heading += CoordinateSystem->Phi;
  	}
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->Heading = Heading;
  	NewNode->InstructionNodeType = inTurnNode;
  	NewNode->SetTimeOut(TimeOut);
  	if (Zone < cMinHeadingZone) NewNode->Zone = cMinHeadingZone;
  	else NewNode->Zone = Zone;
  /*	if (TurnDiameter >= 0) {
  		NewNode->M0Velocity = MaxSpeed;
  		NewNode->M1Velocity = MaxSpeed * (TurnDiameter - cRRWheelBase)/(TurnDiameter + cRRWheelBase);
  	} else {
  		NewNode->M1Velocity = MaxSpeed;
  		NewNode->M0Velocity = MaxSpeed * (TurnDiameter + cRRWheelBase)/(TurnDiameter - cRRWheelBase);
  	}
  */
  	NewNode->TurnDiameter = TurnDiameter;
  	NewNode->MaxSpeed = MaxSpeed;
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddHeadingToInstructions(double Heading, TCoordSystem* CoordinateSystem, double Zone, short MaxSpeed, unsigned long TimeOut)
  /*******************************************************************************
  //Created: 08/29/96 S.R.
  //Last Revision: 01/23/97 S.R.
  //Parameters:
  //	Heading:				The Heading to add to the instruction list.
  //	CoordinateSystem		The coordinate system Heading is in.  A NULL value means Heading is
  //								in a non-rotated coordinate system.
  //	Zone:					How close to Heading the robot must be before going to the next node
  //								in the instruction list
  //	MaxSpeed:				The maximum Speed to use when adjusting the Heading.
  //	TimeOut:				The maximum amount of time the robot is allowed to get to the 
  //								final Heading.  Value = 0 indicates an infinite time out.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function adds a Heading onto the robot's instruction list.  If Heading is in a
  //	rotated coordinate system, first the Heading is transformed to a non-rotated
  //	system.
  //Method:
  //		If CoordinateSystem != NULL then Heading is transformed to the non-displaced
  //	system. A new path list node is created.  Then the members of the new node are
  //	initialized, and finally the new node is added to the instruction list.
  *******************************************************************************/
  {
  	if (CoordinateSystem != NULL) {
  		Heading += CoordinateSystem->Phi;
  	}
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->Heading = Heading;
  	NewNode->InstructionNodeType = inHeadingNode;
  	NewNode->SetTimeOut(TimeOut);
  	if (Zone < cMinHeadingZone) NewNode->Zone = cMinHeadingZone;
  	else NewNode->Zone = Zone;
  	NewNode->MaxSpeed = MaxSpeed;
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddNodeToInstructions(CInstructionListNode* NewNode)
  /*******************************************************************************
  //Created: 08/29/96 S.R.
  //Last Revision: 10/30/96 S.R.
  //Parameters:
  //	NewNode:	The new node to add to the instruction list.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //	This function adds a new CInstructionListNode to the instruction list.
  //Method:
  //		If the path list is empty (Head == NULL) then the new node is placed at
  //	the head of the list, and the tail is made to point to it as well.
  //		If the path list is not empty then the new node is tacked onto the tail
  //	of the list.
  *******************************************************************************/
  {
  	NewNode->Next = NULL;
  	if (m_InstructionList->Head == NULL) {
  		m_InstructionList->Head = NewNode;
  		m_InstructionList->Tail = NewNode;
  	} else {
  		m_InstructionList->Tail->Next = NewNode;
  		m_InstructionList->Tail = NewNode;
  	}
  }
  
  robPOINT CRobot::RobotToWorldCoords(robPOINT RobotCoord, robPOINT WorldPos, double WorldHeading)
  /*******************************************************************************
  //Created: 08/30/96 S.R.
  //Last Revision: 01/07/97 S.R.
  //Parameters:
  //	RobotCoord:		The robot coordinate to change to world coordinates.
  //	WorldPos:		The robot's position in the wolrd
  //	WorldHeading:	The robot's heading in the world
  //Return Value:
  //	The world coordinate corresponding to RobotCoord.
  ********************************************************************************
  //Description:
  //		This function transforms a coordinate in the roobt's coordinate frame into
  //	the corresponding coordinate in the world coordinate system.  The robot's
  //	coordinate frame is as follows:
  //
  //                   Y
  //                  |
  //               ---|---
  //               |\ | |
  //               | \| |<--Robot
  //               |  | |
  //               |__|\|___________ X
  //               |   /|
  //               |  / |
  //               | /  |
  //               |/   |
  //               ------
  //	So the X axis points along the robot's current heading, and the Y axis points
  //	to the left side of the robot.
  //Method:
  //	A trigonometric transformation is used to find the world coordinate.
  *******************************************************************************/
  {
  	TCoordSystem RobotSystem;
  	RobotSystem.Phi = WorldHeading;
  	RobotSystem.Origin.x = WorldPos.x;
  	RobotSystem.Origin.y = WorldPos.y;
  	robPOINT TransformedCoord = TransformToNonDisplacedCoordinates(&RobotSystem, RobotCoord);
  	return TransformedCoord;
  }
  
  double CRobot::RobotToWorldHeading(double Heading, double WorldHeading)
  /*******************************************************************************
  //Created: 09/04/96 S.R.
  //Last Revision: 09/04/96 S.R.
  //Parameters:
  //	Heading:			The heading in Robot coordinates
  //	WorldHeading:	The current heading of the robot in world coordinates.
  //Return Value:
  //	Heading transformed to world coordinates.
  ********************************************************************************
  //Description:
  //		This function takes a heading in robot coordinates and transforms it into
  //	world coordinates.
  //Method:
  //	Heading is added to WorldHeading.
  *******************************************************************************/
  {
  	Heading += WorldHeading;
  	return Heading;
  }
  
  bool CRobot::InPositionZone(void)
  /*******************************************************************************
  //Created: 09/05/96 S.R.
  //Last Revision: 09/05/96 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	TRUE:		The robot's handle is within the desired position zone.
  //	FALSE:	It isn't.
  ********************************************************************************
  //Description:
  //		This function determines if the robot's handle is within a circular zone
  //	of a point in the robot world.  The zone and end point are taken from the
  //	robot's path list.
  //Method:
  //		First the function calculates the distance between the desired position and
  //	the end of the robot's handle.  This has to be done separately for the two
  //	cases where FlipHandle is true and false.  If this distance is within the
  //	zone then TRUE is returned.  Otherwise, FALSE is returned.
  *******************************************************************************/
  {
  	short HandleLength;
  	if (m_InstructionList->Head->InstructionNodeType == inCTDPathNode) {
  		HandleLength = cFollowCTDPathHandleLength;
  	} else {
  		HandleLength = GetHandleLength();
  	}
  
  	double distance;
  	if (!GetFlipHandle()) {
  		distance = SQR(m_Position.x + HandleLength*cos(m_Heading) - m_InstructionList->Head->Position.x);
  		distance += SQR(m_Position.y + HandleLength*sin(m_Heading) - m_InstructionList->Head->Position.y);
  		if ( distance <= SQR(m_InstructionList->Head->Zone) ) return TRUE;
  	} else {
  		double V = (m_InstructionList->Head->Position.x - m_Position.x) * cos(m_Heading);
  		V += (m_InstructionList->Head->Position.y - m_Position.y) * sin(m_Heading);
  		if (V >= 0) {
  			distance = SQR(m_Position.x + HandleLength*cos(m_Heading) - m_InstructionList->Head->Position.x);
  			distance += SQR(m_Position.y + HandleLength*sin(m_Heading) - m_InstructionList->Head->Position.y);
  			if ( distance <= SQR(m_InstructionList->Head->Zone) ) return TRUE;
  		} else {
  			distance = SQR(m_Position.x - HandleLength*cos(m_Heading) - m_InstructionList->Head->Position.x);
  			distance += SQR(m_Position.y - HandleLength*sin(m_Heading) - m_InstructionList->Head->Position.y);
  			if ( distance <= SQR(m_InstructionList->Head->Zone) ) return TRUE;
  		}
  	}
  	return FALSE;
  }
  
  void CRobot::RemoveHeadFromInstructions(void)
  /*******************************************************************************
  //Created: 06/21/96 S.R.
  //Last Revision: 06/21/96 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function removes the head from the robot's path list
  //Method:
  //		The head of the path list is removed and deleted.
  *******************************************************************************/
  {
  	CInstructionListNode* temp;
  	temp = m_InstructionList->Head;
  	if (temp != NULL) {
  		m_InstructionList->Head = temp->Next;
  		if (m_InstructionList->Head == NULL) m_InstructionList->Tail = NULL;
  		delete temp;
  	}
  }
  
  bool CRobot::ReachedNextPoint(void)
  /*******************************************************************************
  //Created: 06/21/96 S.R.
  //Last Revision: 01/14/98 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	TRUE:		robot has reached its destination.
  //	FALSE:	robot has not reached its destination
  ********************************************************************************
  //Description:
  //		This function determines if the end of the robot's handle has reached the
  //	next destination on the path list.  If it has then the function returns TRUE.
  //	If not the function returns FALSE.
  //Method:
  //		For the different position nodes, the function calculates the distance
  //	from the end of the robot's handle to the robot's destination and sees if that
  //	is less than or equal to the destination's zone.
  //		For a HeadingNode, the function calculates how far away the robot is from
  //	the destination heading.  If the robot is within the zone then TRUE is returned.
  //		Otherwise, false is returned.
  *******************************************************************************/
  {
  	bool Reached = false;
  //	ASSERT(m_InstructionList->Head != NULL);
  	if (m_InstructionList->Head != NULL) {
  		Reached = m_InstructionList->Head->ReachedEndOfInstruction(this);
  	}
  	return Reached;
  }
  
  void CRobot::GoToNextNodeInInstructions(void)
  /*******************************************************************************
  //Created: 08/29/96 S.R.
  //Last Revision: 11/06/96 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function sends the robot to the next node on the path list.
  //Method:
  //		First the head of the path list is removed.  Then the path list head is
  //	sent.  Finally the start time is calculated if necessary.
  *******************************************************************************/
  {
  	m_Nudged = NDG_None;
  	RemoveHeadFromInstructions();
  	SendInstructionsHead();
  	if (m_InstructionList->Head != NULL) {
  		m_InstructionList->Head->SetStartTime();
  	}
  }
  
  void CRobot::SetFollowingPath(CCTDPath* Path)
  {
  	m_ProbabilityGrid->m_RWGraphicObjectList->m_RobotCurrentPath->Redraw(false);
  	m_ProbabilityGrid->m_RWGraphicObjectList->SetRobotCurrentPath(Path);
  	m_ProbabilityGrid->m_RWGraphicObjectList->m_RobotCurrentPath->Redraw(false);
  }
  
  void CRobot::SendInstructionsHead(void)
  /*******************************************************************************
  //Created: 10/28/96 S.R.
  //Last Revision: 02/04/97 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function sends the robot commands to execute the command stored
  //	in the head of the path list.
  //Method:
  //		If the head of the path list is not NULL, the function determines what
  //	kind of command is stored in the path list node, and sends commands to the
  //	robot to execute it.
  *******************************************************************************/
  {
  	CInstructionListNode* Head = m_InstructionList->Head;
  	m_ExecutingRobotTask = false;
  	m_RobotInstructionResult.Valid = false;
  	if (Head != NULL) {
  		Head->SendInstruction(this);
  	}
  }
  
  TInstructionResult CRobot::FollowInstructions(bool PurgeInstructionList, bool StopRobotAfterExecution)
  /*******************************************************************************
  //Created: 10/28/96 S.R.
  //Last Revision: 12/03/97 S.R.
  //Parameters:
  //	PurgeInstructionsList:					TRUE if the path list should be purged before the
  //											function returns.
  //	StopRobotAfterExecution:	TRUE if the robot should stop after it is done
  //											executing the path.
  //	Return Value:
  //		The result of the path following.  Whether that be an obstacle, time out
  //	or getting there.
  ********************************************************************************
  //Description:
  //		This function makes the robot follow a list of instructions.  It does not return until the
  //	instructions are completed or something has interfered with the robot's ability to
  //	finish the instructions.  While this function is working, the State of the robot is
  //	set to FollowingInstruction.  After execution the State is set to what it was when
  //	the robot started execution.
  //		If something goes wrong, the robot is stopped and if PurgeInstructionsList is TRUE
  //	the robot's path list is purged.
  //	If StopRobotAfterExecution is TRUE, the robot is stopped after reaching its
  //	destination.
  //Method:
  //		First the old value of m_State is stored, then m_State is set to FollowingInstruction.
  //	Next the m_MotorObstacle.ValidObstacle flag is set to FALSE so the function
  //	doesn't mistakenly stop execution because of an old obstacle.  Then the head
  //	of the path list is sent, and the starting time is calculated.  Finally, the
  //	function runs down the path list; sending each position, velocity or heading
  //	in the path.  While following the path, the DoWindowMessages function is
  //	called to handle any window messages that might occur while the function is
  //	busy.  If the robot times out or hits an obstacle while executing a
  //	path, then if necessary the path list is purged, and the corresponding error
  //	code is returned.
  //		When determining if the robot has reached the next point, if the zone
  //	is small, the robot must stay in the zone for 0.25 seconds.  This is to make
  //	sure the robot doesn't overshoot its destination.
  //		Also, every second the path's head is sent to the robot.  This is to
  //	compensate for lost packets.
  //		Before returning the function sets m_State to OldState.
  *******************************************************************************/
  {
  	m_RobotInstructionResult.Valid = false;
  	short OldMaxSpeed = GetMaxSpeed();
  	TState OldState = m_State;
  	SetState(stFollowingInstructions);
  	bool StartNextPointTimer = true;
  	TInstructionResult InstructionsResult = IR_InstructionComplete;
  /*Reset the motor obstacle member*/
  	m_MotorObstacle.ValidObstacle = FALSE;
  /*While the path list is not empty, walk down it*/
  /*Send the first node in the path, and set its starting time*/
  	SendInstructionsHead();
  	if (m_InstructionList->Head != NULL) {
  		m_InstructionList->Head->SetStartTime();
  	}
  
  	while (m_InstructionList->Head != NULL) {
  	/*Check for hitting obstacles*/
  		if (m_MotorObstacle.ValidObstacle) {
  			if (!m_ExecutingRobotTask) {
  				m_MotorObstacle.ValidObstacle = FALSE;
  				InstructionsResult = IR_HitObstacle;
  				goto INSTRUCTIONS_ERROR_HANDLER;
  			}
  		}
  	/*Check for error task result*/
  		if (m_RobotInstructionResult.Valid) {
  			if (m_RobotInstructionResult.Result != IR_InstructionComplete) {
  				InstructionsResult = m_RobotInstructionResult.Result;
  				goto INSTRUCTIONS_ERROR_HANDLER;
  			}
  		}
  	/*Check for time out if necessary*/
  		if (m_InstructionList->Head->InstructionTimedOut()) {
  		/*If time out while in a VelocityNode, go to the next node in path*/
  			switch (m_InstructionList->Head->InstructionNodeType) {
  			case inPwmNode:
  			case inVelocityNode:
  			case inStraightVelocityNode:
  			case inWait:
  			case inFeelMotor:
  				GoToNextNodeInInstructions();
  				break;
  			case inWaitForNudge:
  				if (m_Nudged) {
  					GoToNextNodeInInstructions();
  					m_Nudged = NDG_None;
  				} else {
  					InstructionsResult = IR_TimedOut;
  					goto INSTRUCTIONS_ERROR_HANDLER;
  				}
  				break;
  			case inBeep:
  				if (m_InstructionList->Head->Next != NULL) {
  					if (m_InstructionList->Head->Next->InstructionNodeType != inBeep) {
  						SetBuzzerFrequency(0);
  					}
  				} else {
  					SetBuzzerFrequency(0);
  				}
  				GoToNextNodeInInstructions();
  				break;
  			default:
  				InstructionsResult = IR_TimedOut;
  				goto INSTRUCTIONS_ERROR_HANDLER;
  				break;
  			}
  		}
  	/*If reached next point, go to the next node*/
  		if (ReachedNextPoint()) {
  			GoToNextNodeInInstructions();
  		}
  	/*Do other window messages*/
  		MSG message = DoWindowMessages();
  	/*wait while paused*/
  		if (m_Pause) {
  			if (m_InstructionList->Head != NULL) {
  				m_InstructionList->Head->Pause();
  			}
  			while (m_Pause) message = DoWindowMessages();
  			if (m_InstructionList->Head != NULL) {
  				m_InstructionList->Head->Resume();
  			}
  		}
  	/*Check for EStop*/
  		if (m_State != stFollowingInstructions) {
  			InstructionsResult = IR_EmergencyStop;
  			goto INSTRUCTIONS_ERROR_HANDLER;
  		}
  	}
  	if (PurgeInstructionList) {
  		PurgeInstructions();
  	}
  	if (StopRobotAfterExecution) {
  		Stop();
  	}
  	SetState(OldState);
  	SetFollowingPath(NULL);
  	return IR_InstructionComplete;
  
  INSTRUCTIONS_ERROR_HANDLER:
  	if (PurgeInstructionList) {
  		PurgeInstructions();
  	}
  	Stop();
  	SetState(OldState);
  	SetFollowingPath(NULL);
  	return InstructionsResult;
  }
  
  TTaskResult CRobot::FollowInstructions(bool PurgeInstructionsList, bool StopRobotAfterExecution, TInstructionResult ExpectedResult, bool DisplayErrorMessages)
  /*******************************************************************************
  //Created: 02/28/97 S.R.
  //Last Revision: 09/30/97 S.R.
  //Parameters:
  //	PurgeInstructionsList:	TRUE if the path list should be purged after the path is followed.
  //	StopRobotAfterExecution:	TRUE if the robot should stop after executing the path.
  //	ExpectedResult:	The expected result of the path.
  //Return Value:
  //	The result of following the path.
  ********************************************************************************
  //Description:
  //		This function will follow a path, and if the path isn't the expected result,
  //	it will display a dialog box with the unexpected result and stop the robot.
  //Method:
  //
  *******************************************************************************/
  {
  	TInstructionResult InstructionResult = FollowInstructions(PurgeInstructionsList, StopRobotAfterExecution);
  	if (InstructionResult == ExpectedResult) {
  		return TR_TaskComplete;
  	} else {
  		Stop();
  		switch (InstructionResult) {
  		case IR_HitObstacle:
  			if (DisplayErrorMessages) DisplayErrorText(IDS_ERR_PATH_UNEXPECTED_OBSTACLE); //AfxMessageBox(IDS_ERR_PATH_UNEXPECTED_OBSTACLE, MB_OK, 0);
  			break;
  		case IR_TimedOut:
  			if (DisplayErrorMessages) DisplayErrorText(IDS_ERR_PATH_TIMEOUT);//AfxMessageBox(IDS_ERR_PATH_TIMEOUT, MB_OK, 0);
  			break;
  		case IR_EmergencyStop:
  			break;
  		case IR_InstructionComplete:
  			switch (ExpectedResult) {
  			case IR_HitObstacle:
  				if (DisplayErrorMessages) DisplayErrorText(IDS_ERR_PATH_NO_OBSTACLE); //AfxMessageBox(IDS_ERR_PATH_NO_OBSTACLE);
  				break;
  			case IR_TimedOut:
  				if (DisplayErrorMessages) DisplayErrorText(IDS_ERR_PATH_NO_TIMEOUT); //AfxMessageBox(IDS_ERR_PATH_NO_TIMEOUT);
  				break;
  			case IR_EmergencyStop:
  				if (DisplayErrorMessages) DisplayErrorText(IDS_ERR_PATH_NO_ESTOP); //AfxMessageBox(IDS_ERR_PATH_NO_ESTOP);
  				break;
  			default:
  				if (DisplayErrorMessages) DisplayErrorText(IDS_ERR_PATH_GENERAL_ERROR); //AfxMessageBox(IDS_ERR_PATH_GENERAL_ERROR, MB_OK, 0);
  				break;
  			}
  			break;
  		default:
  			if (DisplayErrorMessages) DisplayErrorText(IDS_ERR_PATH_GENERAL_ERROR); //AfxMessageBox(IDS_ERR_PATH_GENERAL_ERROR, MB_OK, 0);
  			break;
  		}
  	}
  	if (InstructionResult == IR_EmergencyStop) {
  		return TR_EmergencyStop;
  	} else {
  		return TR_Error;
  	}
  }
  
  MSG CRobot::DoWindowMessages(void)
  /*******************************************************************************
  //Created: 10/25/96 S.R.
  //Last Revision: 10/28/96 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	The last message processed.
  ********************************************************************************
  //Description:
  //		This function can be used to process messages while in the middle of an
  //	intensive algorithm or while waiting for something to happen.  It will
  //	process all messages in the queue before returning.
  //Method:
  //		The PeekMessage function is used to see if there are any messages, and
  //	if there are, they are removed and returned through message.  TranslateMessage
  //	and DispatchMessage then take care of the message. 
  *******************************************************************************/
  {
  	MSG message;
  //	while (PeekMessage(&message,NULL,0,0,PM_REMOVE)) {
  #ifdef JUNK
  	while (::PeekMessage(&message,NULL,0,0,PM_NOREMOVE)) {
  		switch (message.message) {
  		case WM_TIMER:
  		case WM_PAINT:
  		case WM_KILLFOCUS:
  		case WM_SETFOCUS:
  		case WM_NCPAINT:
  		case WM_ERASEBKGND:
  		case WM_SETCURSOR:
  		case WM_NCHITTEST:
  		case WM_MOUSEMOVE:
  		case 1127:
  		case 1126:
  		case 1125:
  		case 1130:
  		case 256:
  			break;
  		case WM_DESTROY:
  		case WM_CLOSE:
  		case WM_ENDSESSION:
  		case WM_QUIT:
  			PeekMessage(&message, NULL, 0, 0, PM_REMOVE);
  			return message;
  			break;
  		case ID_ROBOT_ESTOP:
  			int j;
  			j = 9;
  			break;
  		default:
  			int i;
  			i = 1;
  			i++;
  			break;
  		}
  
  		::PeekMessage(&message,NULL,0,0,PM_REMOVE);
  		::TranslateMessage(&message);
  		::DispatchMessage(&message);
  		PumpMessage();
  	}
  #endif
  	bool EStopped = false;
  	do {
  		GetMessage(&message, NULL, 0, 0);
  		if (message.message == ID_ROBOT_ESTOP) {
  			EStopped = true;
  		} else {
  			TranslateMessage(&message);
  			DispatchMessage(&message);
  		}
  	} while (::PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE) && !EStopped);
  
  	if (!EStopped) {
  		long lIdle = 0;
  		while ( AfxGetApp()->OnIdle(lIdle++ ) );
  	}
  	return message;
  }
  
  void CRobot::AddWaitForInputChange(unsigned long TimeOut)
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inWaitForInputChange;
  	NewNode->SetTimeOut(TimeOut);
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddWaitForInputHigh(unsigned long TimeOut)
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inWaitForInputHigh;
  	NewNode->SetTimeOut(TimeOut);
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddWaitForInputLow(unsigned long TimeOut)
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inWaitForInputLow;
  	NewNode->SetTimeOut(TimeOut);
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddWaitForInputGreater(float Voltage, unsigned long TimeOut)
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inWaitForInputGreater;
  	NewNode->SetTimeOut(TimeOut);
  	NewNode->Parm0 = Voltage;
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddWaitForInputLess(float Voltage, unsigned long TimeOut)
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inWaitForInputLess;
  	NewNode->SetTimeOut(TimeOut);
  	NewNode->Parm0 = Voltage;
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddWaitForNudgeToInstructions(unsigned long TimeOut)
  /*******************************************************************************
  //Created: 01/14/98 S.R.
  //Last Revision: 01/14/98 S.R.
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
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inWaitForNudge;
  	NewNode->SetTimeOut(TimeOut);
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddBeepToInstructions(unsigned short Frequency, unsigned long TimeOut)
  /*******************************************************************************
  //Created: 01/14/98 S.R.
  //Last Revision: 01/14/98 S.R.
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
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inBeep;
  	NewNode->sParm0 = Frequency;
  	NewNode->SetTimeOut(TimeOut);
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddWaitToInstructions(unsigned int msTimeOut)
  /*******************************************************************************
  //Created: 01/14/98 S.R.
  //Last Revision: 01/14/98 S.R.
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
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inWait;
  	NewNode->SetTimeOut(msTimeOut);
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddWaitUntilToInstructions(CTime Time)
  /*******************************************************************************
  //Created: 01/27/98 S.R.
  //Last Revision: 01/27/98 S.R.
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
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inWaitUntil;
  	NewNode->SetTimeOut(0);
  	NewNode->m_WaitUntilTime = Time;
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddWaitLongToInstructions(int Hours, int Minutes, int Seconds)
  {
  /*Make sure total time < 24 hours.  Otherwise things get screwed up*/
  	assert(Hours + ((float)Minutes)/60 + ((float)Seconds)/3600 < 24);
  
  	CTime CurrentTime = CTime::GetCurrentTime();
  	CTimeSpan DT(0,Hours,Minutes,Seconds);
  	CTime WaitTime = CurrentTime + DT;
  
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inWait;
  	NewNode->SetTimeOut((3600*Hours + 60*Minutes + Seconds) * 1000);
  //	NewNode->m_WaitUntilTime = WaitTime;
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddVelocitiesToInstructions(short M0Velocity, short M1Velocity, unsigned int TravelTime)
  /*******************************************************************************
  //Created: 11/05/96 S.R.
  //Last Revision: 11/05/96 S.R.
  //Parameters:
  //	M0Velocity, M1Velocity:		The velocities each of the robot's motors should go.
  //	TravelTime:						The amount of time to set those velocities.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		The function adds a velocity node to the robot's instruction list.
  //Method:
  //		The function creates a new path list node and sets all the parameters
  //	to the correct value.  Then this new node is added to the path list.
  *******************************************************************************/
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->M0Velocity = M0Velocity;
  	NewNode->M1Velocity = M1Velocity;
  	NewNode->InstructionNodeType = inVelocityNode;
  	NewNode->SetTimeOut(TravelTime);
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddTuneToInstructions(short Frequencies[], int Durations[], int NumberOfNotes)
  {
  	CTuneInstruction* NewNode = new CTuneInstruction(Frequencies, Durations, NumberOfNotes);
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::AddPwmToInstructions(short M0Pwm, short M1Pwm, unsigned int TravelTime)
  /*******************************************************************************
  //Created: 11/05/96 S.R.
  //Last Revision: 11/05/96 S.R.
  //Parameters:
  //	M0Velocity, M1Velocity:		The velocities each of the robot's motors should go.
  //	TravelTime:						The amount of time to set those velocities.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		The function adds a velocity node to the robot's instruction list.
  //Method:
  //		The function creates a new path list node and sets all the parameters
  //	to the correct value.  Then this new node is added to the path list.
  *******************************************************************************/
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->M0Velocity = M0Pwm;
  	NewNode->M1Velocity = M1Pwm;
  	NewNode->InstructionNodeType = inPwmNode;
  	NewNode->SetTimeOut(TravelTime);
  	AddNodeToInstructions(NewNode);
  }
  
  TTaskResult CRobot::SquareToWall(TMotorObstacle Obstacle)
  /*******************************************************************************
  //Created: 11/05/96 S.R.
  //Last Revision: 02/28/97 S.R.
  //Parameters:
  //	Obstacle:	The motor obstacle the robot hit prior to this funciton call.
  //Return Value:
  //		The result of squaring to the wall.
  ********************************************************************************
  //Description:
  //		This function tries to square the robot to a wall.  The exact algorithm
  //	used depends on which wheel is currently against the wall.  Due to this fact,
  //	the robot must have hit a wall, and which motor hit the wall must be sent
  //	to the function through the Obstacle parameter.
  //		Currently, the function can only handle squaring to a wall that is in
  //	front of the robot.
  //		If the robot is unable to square up to the wall, it returns IR_InstructionIncomplete,
  //	if the wall is in back of the robot, or IR_TimedOut if the robot can't square
  //	to the wall quickly.
  //Method:
  //		The robot follows different paths to square to the wall depending on which
  //	wheel is initially in contact with the wall.
  *******************************************************************************/
  {
  	const unsigned int cSquareTimeOut = 7000;
  	const unsigned int cRotateTimeOut = 1000;
  	const unsigned int cBackupTimeOut = 250;
  	const char cForwardVelocity = cObstacleDetectionSpeed;
  //	const char cBackwardVelocity = -cObstacleDetectionSpeed/2;
  	const char cBackwardVelocity = 0;
  
  	TTaskResult TaskResult;
  /*Set moderate obstacle detection*/
  	SetObstacleSensitivity(osMediumSensitivity);
  	PurgeInstructions();
  	switch (Obstacle) {
  	case M0Front:
  //		AddVelocitiesToInstructions(cBackwardVelocity, 0, cBackupTimeOut);
  //		AddVelocitiesToInstructions(-1, cForwardVelocity, cSquareTimeOut);
  		AddVelocitiesToInstructions(cBackwardVelocity, cForwardVelocity, cSquareTimeOut);
  		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  			return TaskResult;
  		}
  //		AddVelocitiesToInstructions(cForwardVelocity, -1, cSquareTimeOut);
  //		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  //			return TaskResult;
  //		}
  		AddVelocitiesToInstructions(cForwardVelocity, cForwardVelocity, cSquareTimeOut);
  		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  			return TaskResult;
  		}
  		break;
  	case M1Front:
  //		AddVelocitiesToInstructions(0, cBackwardVelocity, cBackupTimeOut);
  //		AddVelocitiesToInstructions(cForwardVelocity, -1, cSquareTimeOut);
  		AddVelocitiesToInstructions(cForwardVelocity, cBackwardVelocity, cSquareTimeOut);
  		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  			return TaskResult;
  		}
  //		AddVelocitiesToInstructions(-1, cForwardVelocity, cSquareTimeOut);
  //		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  //			return TaskResult;
  //		}
  		AddVelocitiesToInstructions(cForwardVelocity, cForwardVelocity, cSquareTimeOut);
  		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  			return TaskResult;
  		}
  		break;
  	case M0Front_M1Front:
  //		AddVelocitiesToInstructions(cBackwardVelocity, cForwardVelocity, cSquareTimeOut);
  //		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  //			return TaskResult;
  //		}
  //		AddVelocitiesToInstructions(cForwardVelocity, cBackwardVelocity, cSquareTimeOut);
  //		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  //			return TaskResult;
  //		}
  //		AddVelocitiesToInstructions(-1, cForwardVelocity, cSquareTimeOut);
  //		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  //			return TaskResult;
  //		}
  		AddVelocitiesToInstructions(cForwardVelocity, cForwardVelocity, cSquareTimeOut);
  		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  			return TaskResult;
  		}
  		break;
  	default:
  //		DisplayErrorText("Obstacle Not in front of robot");
  		AfxMessageBox("Obstacle Not in front of robot");
  		return TR_Error;
  		break;
  	}
  	return TR_TaskComplete;
  }
  
  TTaskResult CRobot::SquareToWall(void)
  {
  	AddSquareToWallToInstructions(GetHeading(), false);
  	return FollowInstructions(true, true, IR_InstructionComplete);
  }
  
  TTaskResult CRobot::SquareToWallFast(TMotorObstacle Obstacle)
  {
  	const unsigned int cSquareTimeOut = 7000;
  	const short cForwardPwm = 500;
  	const short cForwardVelocity = cObstacleDetectionSpeed;
  	const short cBackwardVelocity = 0;
  
  	TTaskResult TaskResult;
  /*Set moderate obstacle detection*/
  	SetObstacleSensitivity(osMediumSensitivity);
  	PurgeInstructions();
  	AddVelocitiesToInstructions(cForwardVelocity, cForwardVelocity, cSquareTimeOut);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	return TR_TaskComplete;
  }
  
  void CRobot::CalculatePositionAndHeadingError(robPOINT NewPosition, double NewHeading, bool ResetPosition, bool ResetHeading)
  /*******************************************************************************
  //Created: 12/06/96 S.R.
  //Last Revision: 02/25/97 S.R.
  //Parameters:
  //	NewPosition:	The robot's new position
  //	NewHeading:		The robot's new heading
  //	ResetPosition:	TRUE if the position error should be reset to 0, FALSE otherwise.
  //	ResetHeading:	TRUE if the heading error should be reset to 0, FALSE otherwise.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function calculates the error in the position and heading when the
  //	robot moves.  When ResetPosition == TRUE, the position errors are all set to
  //	0, and the new position to calculate subsequent errors are set to NewPosition.
  //	A similar procedure is followed for ResetPosition == TRUE.
  //Method:
  //		The heading error accumulated by turning is assumed to be KHeadERR * DHeading
  //	This accumulated error is added onto the old error
  //		The position error accumulated in going from the old position to the new
  //	one is calculated as if the robot went between the two points in a straight
  //	line, with a heading error equal to the previously calculated heading error.
  *******************************************************************************/
  {
  //	const float KHeadLErr = 0.0011f;
  //Check Position Resets
  	if (ResetPosition) {
  		m_OldPosition = NewPosition;
  		m_PositionWasReset = TRUE;
  		m_Parent->RedrawRobot(m_Position, m_Heading);
  		m_PositionError.x = 0;
  		m_PositionError.y = 0;
  	} else {
  		if (!ResetHeading) {
  			if (m_PositionWasReset) {
  				if ( Distance(NewPosition,m_OldPosition) < (1.0 * cTPI) ) {
  					m_PositionWasReset = FALSE;
  				}
  			}
  		}
  	}
  //Check Heading Resets
  	if (ResetHeading) {
  		m_OldHeading = NewHeading;
  		m_HeadingWasReset = TRUE;
  	/*Heading Error is never zero.*/
  		m_HeadingError = cMinHeadingError;
  //		m_HeadingError = 0;
  	} else {
  		if (!ResetPosition) {
  			if (m_HeadingWasReset) {
  				if (AbsAngularSeparation(NewHeading, m_OldHeading) < 0.175) {
  					m_HeadingWasReset = FALSE;
  				}
  			}
  		}
  	}
  	if (m_PositionWasReset || m_HeadingWasReset) return;
  //Calculate Errors
  	double L = Distance(NewPosition,m_OldPosition);
  	double phi = atan2(NewPosition.y - m_OldPosition.y, NewPosition.x - m_OldPosition.x);
  
  /*Don't use standard error calculation because these measurements are not independent.  Each measurement's
  	error depends on the previous measurements (so there's a covariance term.)*/
  /*	m_HeadingError = sqrt( SQR(m_HeadingError) + 
  			SQR(m_KHeadErr*AbsAngularSeparation(NewHeading, OldHeading)) + SQR(m_KStraightHeadErr * L) );
  	m_PositionError.x = sqrt( SQR(m_PositionError.x) + SQR( L*sin(phi)*m_HeadingError) );
  	m_PositionError.y = sqrt( SQR(m_PositionError.y) + SQR( L*cos(phi)*m_HeadingError) );
  */
  	switch (GetFloorType()) {
  	case ftWood:
  	case ftThinCarpet:
  		m_KHeadErr = 0.00075f;
  		m_KStraightHeadErr = 0.0000014f;
  //		m_HeadingError = 2.0/cNHeadings * 2 * pi;
  		break;
  	case ftThickCarpet:
  		m_KHeadErr = 0.002f;
  		m_KStraightHeadErr = 0.0000034f;
  //		m_HeadingError = 6.0/cNHeadings * 2 * pi;
  		break;
  	}
  	m_HeadingError += m_KHeadErr*AbsAngularSeparation(NewHeading, m_OldHeading) + m_KStraightHeadErr * L;
  	double temp = sin(phi) * L * m_HeadingError;
  	if (temp >= 0) m_PositionError.x = m_PositionError.x + temp;
  	else m_PositionError.x = m_PositionError.x - temp;
  	temp = cos(phi) * L * m_HeadingError;
  	if (temp >= 0) m_PositionError.y = m_PositionError.y + temp;
  	else m_PositionError.y = m_PositionError.y - temp;
  
  /*Don't let the errors get so large that they cause problems*/
  	if (m_PositionError.x > 1440) m_PositionError.x = 1440;
  	if (m_PositionError.y > 1440) m_PositionError.y = 1440;
  	if (m_HeadingError > pi/2) m_HeadingError = pi/2;
  
  	m_OldPosition = NewPosition;
  	m_OldHeading = NewHeading;
  }
  
  TTaskResult CRobot::CalibrateAgainstDoorway(CCalibrationZone* CalibrationZone)
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
  	CalCoordSys.Origin.x = (CalibrationZone->m_P0.x + CalibrationZone->m_P1.x) / 2;
  	CalCoordSys.Origin.y = (CalibrationZone->m_P0.y + CalibrationZone->m_P1.y) / 2;
  	CalCoordSys.Phi = CalibrationZone->m_Heading;
  /*Get in front of doorway and turn towards it*/
  //	P.x = - 2*m_HandleLength;
  //	P.y = 0;
  //	AddPositionVelocityToInstructions(GetRobotToPoint(P, &CalCoordSys), &CalCoordSys, 0, cComputerControlSpeed, TRUE, 0);
  //	AddHeadingToInstructions(0, &CalCoordSys, 0, cComputerControlHeadingSpeed, 0);
  //	if ( (TaskResult = FollowInstructions(TRUE, FALSE, IR_InstructionComplete)) != TR_TaskComplete ) {
  //		return TaskResult;
  //	}
  ///*Get in middle of doorway, turn to the left*/
  //	P.x = 0;
  //	P.y = 0;
  //	AddPositionVelocityToInstructions(GetRobotToPoint(P, &CalCoordSys), &CalCoordSys, 0, cComputerControlSpeed, FALSE, 0);
  /*Assume we're in the middle of the doorway, and turn to the left*/
  	AddHeadingToInstructions(pi/2, &CalCoordSys, 0, cComputerControlHeadingSpeed, 0);
  	if ( (TaskResult = FollowInstructions(TRUE, FALSE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Hit doorway*/
  	Corner.x = 0;
  	Corner.y = Distance(CalibrationZone->m_P0, CalibrationZone->m_P1) / 2;
  	P = TransformToDisplacedCoordinates(&CalCoordSys,m_Position);
  	AddStraightVelocityToInstructions(HeadingToDestination(P,Corner), cObstacleDetectionSpeed, &CalCoordSys, 10000);
  //	AddStraightVelocityToInstructions(pi/2, cObstacleDetectionSpeed, &CalCoordSys, 10000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	TLine L1;
  	GetFrontLine(m_Position, m_Heading, &L1);
  /*Rotate a bit*/
  	AddVelocitiesToInstructions(-cObstacleDetectionSpeed,0,1000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Hit the doorway again*/
  	AddVelocitiesToInstructions(cObstacleDetectionSpeed,cObstacleDetectionSpeed,10000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	TLine L2;
  	GetFrontLine(m_Position, m_Heading, &L2);
  /*Correct Robot's Position*/
  	robPOINT FirstMeasuredCornerPosition;
  	IntersectionOfLines(L1,L2,&FirstMeasuredCornerPosition);
  	robPOINT FirstActualCornerPosition;
  	FirstActualCornerPosition = TransformToNonDisplacedCoordinates(&CalCoordSys, Corner);
  	m_Position.x += FirstActualCornerPosition.x - FirstMeasuredCornerPosition.x;
  	m_Position.y += FirstActualCornerPosition.y - FirstMeasuredCornerPosition.y;
  	SetPosition(m_Position);
  
  /*Get Ready to find second corner*/
  	robPOINT Destination;
  	Destination.x = m_Position.x - (8*cTPI + m_HandleLength) * cos(m_Heading);
  	Destination.y = m_Position.y - (8*cTPI + m_HandleLength) * sin(m_Heading); 
  	AddPositionVelocityToInstructions(Destination, NULL, 3*cTPI, cObstacleDetectionSpeed, TRUE, 9000);
  //	AddVelocitiesToInstructions(-cComputerControlSpeed,-cComputerControlSpeed,1000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	P.x = 0;
  	P.y = 0;
  	AddPositionVelocityToInstructions(GetRobotToPoint(P, &CalCoordSys), &CalCoordSys, 0, cComputerControlSpeed, FALSE, 9000);
  	AddHeadingToInstructions(-pi/2, &CalCoordSys, 0, cComputerControlHeadingSpeed, 0);
  	if ( (TaskResult = FollowInstructions(TRUE, FALSE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Hit doorway*/
  	Corner.x = 0;
  	Corner.y = -Distance(CalibrationZone->m_P0, CalibrationZone->m_P1) / 2;
  	P = TransformToDisplacedCoordinates(&CalCoordSys,m_Position);
  	AddStraightVelocityToInstructions(HeadingToDestination(P,Corner), cObstacleDetectionSpeed, &CalCoordSys, 10000);
  //	AddStraightVelocityToInstructions(-pi/2, cObstacleDetectionSpeed, &CalCoordSys, 10000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	GetFrontLine(m_Position, m_Heading, &L1);
  /*Rotate a bit*/
  	AddVelocitiesToInstructions(0,-cObstacleDetectionSpeed,1000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Hit the doorway again*/
  	AddVelocitiesToInstructions(cObstacleDetectionSpeed,cObstacleDetectionSpeed,10000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	GetFrontLine(m_Position, m_Heading, &L2);
  
  	robPOINT SecondMeasuredCornerPosition;
  	IntersectionOfLines(L1,L2,&SecondMeasuredCornerPosition);
  	robPOINT SecondActualCornerPosition;
  	SecondActualCornerPosition.x = 0;
  	SecondActualCornerPosition.y = -Distance(CalibrationZone->m_P0, CalibrationZone->m_P1) / 2;
  	SecondActualCornerPosition = TransformToNonDisplacedCoordinates(&CalCoordSys, SecondActualCornerPosition);
  /*Correct the robot's heading*/
  	double theta, thetaprime;
  	theta = atan2(SecondActualCornerPosition.y - FirstActualCornerPosition.y, SecondActualCornerPosition.x - FirstActualCornerPosition.x);
  	thetaprime = atan2(SecondMeasuredCornerPosition.y - FirstActualCornerPosition.y, SecondMeasuredCornerPosition.x - FirstActualCornerPosition.x);
  	SetHeading(m_Heading + theta - thetaprime);
  	m_HeadingError = 0;
  /*Correct Robot's Position*/
  	m_Position.x += SecondActualCornerPosition.x - SecondMeasuredCornerPosition.x;
  	m_Position.y += SecondActualCornerPosition.y - SecondMeasuredCornerPosition.y;
  	SetPosition(m_Position);
  	m_PositionError.x = 0;
  	m_PositionError.y = 0;
  	return TR_TaskComplete;
  }
  
  TTaskResult CRobot::CalibrateAgainstWall(CCalibrationZone* CalibrationZone)
  /*******************************************************************************
  //Created: 02/27/97 S.R.
  //Last Revision: 01/27/98 S.R.
  //	CalibrationZone:	the calibration zone to calibrate against.
  //Return Value:
  //	The result of the calibration task.
  ********************************************************************************
  //Description:
  //		Given a calibration zone that is a calibration wall, the function will
  //	calibrate the robot against it.  This function assumes the robot is already
  //	in front of the calibration wall.
  //Method:
  //
  *******************************************************************************/
  {
  	TTaskResult TaskResult;
  	TCoordSystem CalCoordSys;
  	robPOINT P;
  	double theta;
  /*Create a coordinate system for the calibration doorway*/
  	CalCoordSys.Origin.x = (CalibrationZone->m_P0.x + CalibrationZone->m_P1.x) / 2;
  	CalCoordSys.Origin.y = (CalibrationZone->m_P0.y + CalibrationZone->m_P1.y) / 2;
  	CalCoordSys.Phi = CalibrationZone->m_Heading;
  /*Set moderate obstacle detection for backup*/
  	SetObstacleSensitivity(osMediumSensitivity);
  /*Get in front of the wall, then run into it*/
  //	P.x = -2*m_HandleLength;
  //	P.y = 0;
  //	AddPositionVelocityToInstructions(GetRobotToPoint(P, &CalCoordSys), &CalCoordSys, 0, cComputerControlSpeed, TRUE, 0);
  //	if ( (TaskResult = FollowInstructions(TRUE, FALSE, IR_InstructionComplete)) != TR_TaskComplete ) {
  //		return TaskResult;
  //	}
  /*Assume we're in front of the calibration wall, and just hit it.*/
  /*Set high obstacle detection for hitting the wall*/
  //	SetObstacleSensitivity(osHighSensitivity);
  	theta = HeadingToDestination(m_Position, CalCoordSys.Origin);
  	AddHeadingToInstructions(theta, NULL, cModHeadingZone, cComputerControlHeadingSpeed, 10000);
  	AddStraightVelocityToInstructions(theta, cObstacleDetectionSpeed, NULL, 10000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Square to the wall*/
  	if ( (TaskResult = SquareToWall(m_MotorObstacle.MotorObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Update the position and heading*/
  	m_Heading = CalibrationZone->m_Heading;
  	TLine L;
  	FindLineFrom2Points(CalibrationZone->m_P0, CalibrationZone->m_P1, &L);
  	P.x = m_Position.x;
  	P.y = m_Position.y;
  	P = FindPerpendicularIntersection(L, P);
  
  	m_Position.x = P.x + cRRNoseLength * cos(CalibrationZone->m_Heading + pi);
  	m_Position.y = P.y + cRRNoseLength * sin(CalibrationZone->m_Heading + pi);
  
  	SetHeading(m_Heading);
  	SetPosition(m_Position);
  /*Update Errors*/
  	m_PositionError.x = 0;
  	m_PositionError.y = 0;
  	m_HeadingError = 0;
  	return TR_TaskComplete;
  }
  
  double CRobot::HeadingToDestination(robPOINT Position, robPOINT Destination)
  /*******************************************************************************
  //Created: 04/17/97 S.R.
  //Last Revision: 04/17/97 S.R.
  //Parameters:
  //	Position:	the robot's starting position
  //	Destination:	the robot's destination
  //Return Value:
  //	the heading the robot will go in to reach the destination.
  ********************************************************************************
  //Description:
  //		Given a position and destination, this function calculates the heading
  //	to get to the destination.
  //Method:
  //
  *******************************************************************************/
  {
  	double theta;
  	theta = atan2(Destination.y - Position.y, Destination.x - Position.x);
  	return theta;
  }
  
  TTaskResult CRobot::FindCorner(robPOINT* Corner, double* Heading)
  /*******************************************************************************
  //Created: 01/10/97 S.R.
  //Last Revision: 02/28/97 S.R.
  //Parameters:
  //	Corner:	Used to return the location of the corner
  //	Heading:	Used to return a heading that points into the corner.
  //Return Value:
  //	The error result of the search.
  ********************************************************************************
  //Description:
  //		The robot finds the corner by running into it several times and finding
  //	the intersection of the lines of the collisions.
  //Method:
  //		This function finds the corner by hitting it at three different angles,
  //	finding the line lying along the front bumper of the robot, and finding the
  //	intersections of those three lines.  The returned value is the average
  //	intersection point.
  *******************************************************************************/
  {
  	PurgeInstructions();
  /*Hit the corner*/
  	AddStraightVelocityToInstructions(m_Heading, cObstacleDetectionSpeed, NULL, 5000);
  	TTaskResult TaskResult;
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	double H0 = m_Heading;
  	robPOINT P0 = m_Position;
  /*Back up 1/4 second*/
  	AddVelocitiesToInstructions(-10, -10, 250);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Rotate*/
  	AddHeadingToInstructions(H0 + pi/5, NULL, cSmlHeadingZone, cComputerControlHeadingSpeed, 0);
  	TaskResult = FollowInstructions(TRUE, FALSE, IR_InstructionComplete);
  /*Hit the post*/
  	AddVelocitiesToInstructions(cObstacleDetectionSpeed, cObstacleDetectionSpeed, 10000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ){
  		return TaskResult;
  	}
  
  	double H1 = m_Heading;
  	robPOINT P1 = m_Position;
  /*Back up 1/4 second*/
  	AddVelocitiesToInstructions(-10, -10, 250);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Rotate again*/	
  	AddHeadingToInstructions(H0 - pi/5, NULL, cSmlHeadingZone, cComputerControlHeadingSpeed, 0);
  	TaskResult = FollowInstructions(TRUE, FALSE, IR_InstructionComplete);
  /*Hit post*/
  	AddVelocitiesToInstructions(cObstacleDetectionSpeed, cObstacleDetectionSpeed, 10000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  
  	double H2 = m_Heading;
  	robPOINT P2 = m_Position;
  /*Find the corner point*/
  	*Heading = AngularBisector(H1, H2);
  
  	TLine L0, L1, L2;
  	robPOINT Intersections[3];
  	GetFrontLine(P0, H0, &L0);
  	GetFrontLine(P1, H1, &L1);
  	GetFrontLine(P2, H2, &L2);
  
  	IntersectionOfLines(L0, L1, &Intersections[0]);
  	IntersectionOfLines(L1, L2, &Intersections[1]);
  	IntersectionOfLines(L0, L2, &Intersections[2]);
  	
  	double Points[3];
  	double std;
  	Points[0] = Intersections[0].x;
  	Points[1] = Intersections[1].x;
  	Points[2] = Intersections[2].x;
  	Corner->x = Average(Points, 3, &std);
  	Points[0] = Intersections[0].y;
  	Points[1] = Intersections[1].y;
  	Points[2] = Intersections[2].y;
  	Corner->y = Average(Points, 3, &std);
  	
  	return TR_TaskComplete;
  }
  
  TLineErr CRobot::GetFrontLine(robPOINT Position, double Heading, TLine* FrontLine)
  /*******************************************************************************
  //Created: 01/10/97 S.R.
  //Last Revision: 01/10/97 S.R.
  //Parameters:
  //	Position		The robot's position
  //	Heading		The robot's heading
  //	FrontLine	The line across the robot's bumper.
  //Return Value:
  //	Any error that occur when finding the robot's front line.
  ********************************************************************************
  //Description:
  //		This function finds the line across the robot's front bumper given the
  //	robot's position and heading.
  //Method:
  //		The function finds the robot's front corners and forms a line between
  //	those two points.
  *******************************************************************************/
  {
  	robPOINT C1, C2;
  	GetFrontCorners(Position, Heading, &C1, &C2);
  	return FindLineFrom2Points(C1, C2, FrontLine);
  }
  
  void CRobot::GetFrontCorners(robPOINT Position, double Heading, robPOINT* C0, robPOINT* C1)
  /*******************************************************************************
  //Created: 01/10/97 S.R.
  //Last Revision: 01/10/97 S.R.
  //Parameters:
  //	Position		The robot's position
  //	Heading		The robot's heading
  //	C0				The coordinates of the robot's right front corner.
  //	C1				The coordinates of the robot's left front corner.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function finds the coordinates of the robot's front corners.
  //Method:
  //		Trigonometry is used to find the corners.
  *******************************************************************************/
  {
  	C0->x = Position.x + cRRWidth/2 * cos(Heading - pi/2) + cRRNoseLength * cos(Heading);
  	C0->y = Position.y + cRRWidth/2 * sin(Heading - pi/2) + cRRNoseLength * sin(Heading);
  	C1->x = Position.x + cRRWidth/2 * cos(Heading + pi/2) + cRRNoseLength * cos(Heading);
  	C1->y = Position.y + cRRWidth/2 * sin(Heading + pi/2) + cRRNoseLength * sin(Heading);
  }
  
  robPOINT* CRobot::GetCorners(robPOINT Corners[4])
  /*******************************************************************************
  //Created: 02/25/97 S.R.
  //Last Revision: 02/26/97 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //		An robPOINT array containing the coordinates of the robot's four corners.
  //	The points are in the order right front, left front, left back, right back.
  //	Do not delete this array!!
  ********************************************************************************
  //Description:
  //		This function calculates the positions of the robot's four corners and
  //	returns them in an robPOINT array.  This array is static and must never be deleted!!
  //	The points are returned in the order right front, left front, left back, right back.
  //Method:
  //		The positions of the four corners are calculated using trigonometry.
  *******************************************************************************/
  {
  //	static robPOINT Corners[4];
  /*Right Front Corner*/
  	Corners[0].x = m_Position.x + cRRWidth/2 * cos(m_Heading - pi/2) + cRRNoseLength * cos(m_Heading);
  	Corners[0].y = m_Position.y + cRRWidth/2 * sin(m_Heading - pi/2) + cRRNoseLength * sin(m_Heading);
  /*Left Front Corner*/
  	Corners[1].x = m_Position.x + cRRWidth/2 * cos(m_Heading + pi/2) + cRRNoseLength * cos(m_Heading);
  	Corners[1].y = m_Position.y + cRRWidth/2 * sin(m_Heading + pi/2) + cRRNoseLength * sin(m_Heading);
  /*Left Back Corner*/
  	Corners[2].x = m_Position.x - cRRWidth/2 * cos(m_Heading - pi/2) - cRRNoseLength * cos(m_Heading);
  	Corners[2].y = m_Position.y - cRRWidth/2 * sin(m_Heading - pi/2) - cRRNoseLength * sin(m_Heading);
  /*Right Back Corner*/
  	Corners[3].x = m_Position.x - cRRWidth/2 * cos(m_Heading + pi/2) - cRRNoseLength * cos(m_Heading);
  	Corners[3].y = m_Position.y - cRRWidth/2 * sin(m_Heading + pi/2) - cRRNoseLength * sin(m_Heading);
  	return Corners;
  }
  
  robPOINT* CRobot::GetCorners(robPOINT Position, double Heading, robPOINT Corners[4])
  /*Same as GetCorners above, but is used to get the robot's corners hwen the robot is at Position*/
  {
  //	static robPOINT Corners[4];
  /*Right Front Corner*/
  	Corners[0].x = Position.x + cRRWidth/2 * cos(Heading - pi/2) + cRRNoseLength * cos(Heading);
  	Corners[0].y = Position.y + cRRWidth/2 * sin(Heading - pi/2) + cRRNoseLength * sin(Heading);
  /*Left Front Corner*/
  	Corners[1].x = Position.x + cRRWidth/2 * cos(Heading + pi/2) + cRRNoseLength * cos(Heading);
  	Corners[1].y = Position.y + cRRWidth/2 * sin(Heading + pi/2) + cRRNoseLength * sin(Heading);
  /*Left Back Corner*/
  	Corners[2].x = Position.x - cRRWidth/2 * cos(Heading - pi/2) - cRRNoseLength * cos(Heading);
  	Corners[2].y = Position.y - cRRWidth/2 * sin(Heading - pi/2) - cRRNoseLength * sin(Heading);
  /*Right Back Corner*/
  	Corners[3].x = Position.x - cRRWidth/2 * cos(Heading + pi/2) - cRRNoseLength * cos(Heading);
  	Corners[3].y = Position.y - cRRWidth/2 * sin(Heading + pi/2) - cRRNoseLength * sin(Heading);
  	return Corners;
  }
  
  void CRobot::AddStraightVelocityToInstructions(double Heading, short Velocity, TCoordSystem* CoordinateSystem, unsigned int TravelTime)
  /*******************************************************************************
  //Created: 01/15/97 S.R.
  //Last Revision: 02/04/97 S.R.
  //Parameters:
  //	Heading				The heading the robot should go
  //	Velocity				How fast the robot should go
  //	CoordinateSystem	The rotated coordinate system Heading is in.  If this is
  //								NULL, Heading is in a non-rotated system.
  //	TravelTime			The amount of time to go in this direction before going to
  //								the next node in the instruction list.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function adds a Staight Velocity node to the robot's instruction list.  This node
  //	type makes the robot go in a straight line at a given velocity for a given
  //	time.  Heading is the direction the robot is to face when going in the straight
  //	line.  Velocity is how fast and in which direction the robot is to go.
  //	TimeOut is the amount of time to go in the straight line.
  //Method:
  //		If CoordinateSstem != NULL, Heading has to be transformed into a non-rotated
  //	coordinate system.  Then a new CInstructionListNode is created, initialized and
  //	placed on the path list.
  *******************************************************************************/
  {
  	if (CoordinateSystem != NULL) {
  		Heading += CoordinateSystem->Phi;
  	}
  
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->Heading = Heading;
  	NewNode->InstructionNodeType = inStraightVelocityNode;
  	NewNode->SetTimeOut(TravelTime);
  	NewNode->MaxSpeed = Velocity;
  	AddNodeToInstructions(NewNode);
  }
  
  robPOINT CRobot::GetRobotToPoint(robPOINT Destination, TCoordSystem* CoordinateSystem)
  /*******************************************************************************
  //Created: 01/16/97 S.R.
  //Last Revision: 01/16/97 S.R.
  //Parameters:
  //	Destination:		Robot origin's destination.
  //	CoordinateSystem:	Displaced coordinate system that Destination is in.
  //Return Value:
  //	Robot handle's destination to get robot's origin to Destination.
  ********************************************************************************
  //Description:
  //		This function gets the robot's origin to a given Destination.  Given the
  //	displaced coordinate sytem Destination is in, the function returns the
  //	displaced coordinate the handle needs to get to for the robot's origin to
  //	get to Destination.
  //Method:
  //
  *******************************************************************************/
  {
  	robPOINT RobotPosition;
  	if (CoordinateSystem != NULL) {
  		RobotPosition = TransformToDisplacedCoordinates(CoordinateSystem, m_Position);
  	} else {
  		RobotPosition = m_Position;
  	}
  	double theta = atan2(Destination.y - RobotPosition.y, Destination.x - RobotPosition.x);
  	double Dist = Distance(Destination, RobotPosition);
  	Destination.x = RobotPosition.x + (Dist + m_HandleLength) * cos(theta);
  	Destination.y = RobotPosition.y + (Dist + m_HandleLength) * sin(theta);
  	return Destination;
  }
  
  void CRobot::SetPosition(robPOINT Position)
  /*******************************************************************************
  //Created: 01/23/97 S.R.
  //Last Revision: 01/12/98 S.R.
  //Parameters:
  //	Position		The robot's position is set to this value
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function sets the robot's position and resets the robot's position
  //	error.
  //Method:
  //		The function sets m_Position to Position, resets the position error and
  //	corrects the robot's position.
  *******************************************************************************/
  {
  	m_Position = Position;
  	CalculatePositionAndHeadingError(Position, m_Heading, TRUE, FALSE);
  	m_RobotComm->SendPosition(m_Position);
  	return;
  }
  
  void CRobot::SetHeading(double Heading)
  /*******************************************************************************
  //Created: 02/25/97 S.R.
  //Last Revision: 01/12/98 S.R.
  //Parameters:
  //	Heading		The robot's heading is set to this value.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //	This function sets the robot's heading and resets the robot's heading error.
  //Method:
  //		The function sets m_Heading to Heading, resets the heading error and
  //	corrects the robot's heading.
  *******************************************************************************/
  {
  	m_Heading = Heading;
  	CalculatePositionAndHeadingError(m_Position, Heading, FALSE, TRUE);
  	m_RobotComm->SendHeading(m_Heading);
  	return;
  }
  
  void CRobot::AddPositionVelocityToInstructions(robPOINT Destination, TCoordSystem* CoordinateSystem, double Zone, short MaxSpeed, bool FlipHandle, unsigned long TimeOut)
  /*******************************************************************************
  //Created: 01/23/97 S.R.
  //Last Revision: 01/23/97 S.R.
  //Parameters:
  //	Destination				The robot's destination
  //	CoordinateSystem		The coordinate system Destination is in.  If this is
  //									NULL, Destination is in a non-displaced system.
  //	Zone:						The radius of the zone around the destination the robot must
  //									reach before going onto the next point
  //	MaxSpeed					The maximum speed to use in going to this point
  //	FlipHandle				The value of FlipHandle when going to destination
  //	TimeOut					The maximum amount of time the robot is allowed to get to its
  //									position (in ms).  Value = 0 represent an infinite time out.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function adds a position velocity node to the instruction list.  A position
  //	velocity node tells the robot to go to a certain position using velocity
  //	control.  The velocity control makes obstacle detection better, and there is
  //	less chance of the robot stalling before reaching its destination.
  //Method:
  //		If CoordinateSystem != NULL, Destination has to be transformed into the
  //	non-displaced coordinate system.  Then a new path node is created, initialized
  //	and put onto the path list.
  *******************************************************************************/
  {
  	if (CoordinateSystem != NULL) {
  		Destination = TransformToNonDisplacedCoordinates(CoordinateSystem, Destination);
  	}
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->Position = Destination;
  	NewNode->InstructionNodeType = inPositionVelocityNode;
  	NewNode->SetTimeOut(TimeOut);
  //	if (Zone <= cMinPositionZone) NewNode->Zone = cMinPositionZone;
  //	else NewNode->Zone = Zone;
  	NewNode->Zone = Zone;
  	NewNode->MaxSpeed = MaxSpeed;
  	NewNode->bParm0 = FlipHandle;
  	AddNodeToInstructions(NewNode);
  }
  
  TTaskResult CRobot::CalibrateFloorSurface(int* HCF, bool ShowEStop)
  /*******************************************************************************
  //Created: 02/28/97 S.R.
  //Last Revision: 07/10/97 S.R.
  //Parameters:
  //	TicksPerRotation:	the number of encoder ticks to make 1 full rotation
  //Return Value:
  //	The result of the calibration task.
  ********************************************************************************
  //Description:
  //		This function will find the number of ticks it takes the robot to rotate
  //	360 degrees.
  //Method:
  //
  *******************************************************************************/
  {
  	ASSERT(HCF != NULL);
  	TState OldState = GetState();
  	m_UpdateProbabilityGrid = false;
  	TRobotInfo OldRobotInfo = GetRobotInfo();
  	SetEStopVisible(ShowEStop);
  	SetEnableRepositionOnUnknownObstacle(false);
  	SetState(stCalibrating);
  	int TicksPerRotation = -1;
  	SetStatusBarText("");
  	int OldHeadingCorrectionFactor = m_HeadingCorrectionFactor;
  	TTaskResult TaskResult;
  	bool GoodValue = false;
  	do {
  		robPOINT InitialPosition;
  		double InitialHeading;
  		TaskResult = CalibrateTicksPerRotation(&TicksPerRotation, InitialPosition, InitialHeading);
  		if (TaskResult == TR_TaskComplete) {
  			*HCF = (int)HeadingCorrectionFactor((float)TicksPerRotation);
  			SetHeadingCorrectionFactor(*HCF);
  			TaskResult = CheckHeadingCorrectionFactor(GoodValue);
  		}
  		if (TaskResult == TR_TaskComplete) {
  			SetPosition(InitialPosition);
  			SetHeading(InitialHeading);
  		}
  	} while ((TaskResult == TR_TaskComplete) && (GoodValue == false));
  	SetHeadingCorrectionFactor(OldHeadingCorrectionFactor);
  	SetRobotInfo(OldRobotInfo);
  	SetState(OldState);
  	return TaskResult;
  }
  
  TTaskResult CRobot::CalibrateTicksPerRotation(int* Ticks, robPOINT& InitialPosition, double& InitialHeading)
  /*******************************************************************************
  //Created: 02/11/97 S.R.
  //Last Revision: 02/28/97 S.R.
  //Parameters:
  //	Ticks:	The average number of ticks for one rotation.  When passed in, if 
  //					*Ticks > 0, it is the initial guess at the number of ticks per
  //					rotation.  If *Ticks <= 0, the function needs to measure the
  //					initial guess at the number of ticks per rotation.
  //Return Value:
  //	The result of the task.
  ********************************************************************************
  //Description:
  //		This function calculates the number of encoder ticks needed for the robot
  //	to make 1 full rotation on a given surface.  The robot should be close to and
  //	facing the middle of a calibration wall.
  //		To determine the number of encoder ticks for 1 rotation, the robot runs
  //	into the wall, squares to it, backs up and does a 360 then runs into the wall
  //	and squares to it again.  The result of this is used
  //	to calibrate the robot's heading and heading error calculations.
  *******************************************************************************/
  {
  	const int cNumberOfRotations = 5;
  	const int nSamples = 1;
  	int i;
  	TTaskResult TaskResult;
  
  	AddSquareToWallToInstructions(m_Heading, false);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		goto Error;
  	}
  	InitialPosition = GetTaskResultPosition();
  	InitialHeading = GetTaskResultHeading();
  /*If necessary, measure the approximate number of ticks per rotation*/
  	int TPR[nSamples];
  	if (*Ticks <= 0) {
  		if ( (TaskResult = FindTicksPerRotation((int)TotalTicksPerRotation((float)GetHeadingCorrectionFactor(GetFloorType())), TPR, 1)) != TR_TaskComplete ) {
  			goto Error;
  		}
  	} else {
  		TPR[0] = *Ticks;
  	}
  /*Get first sample*/
  	if ( (TaskResult = FindTicksPerRotation(TPR[0], TPR, cNumberOfRotations)) != TR_TaskComplete ) {
  		goto Error;
  	}
  /*Collect rest of samples*/
  	for (i = 1; i < nSamples; i++) {
  		if ( (TaskResult = FindTicksPerRotation(Round(Average(TPR,i)), TPR+i, cNumberOfRotations)) != TR_TaskComplete ) {
  			goto Error;
  		}
  	}
  /*Calculate the heading correction factor*/
  	double std;
  	double AverageEncoderTicks;
  	AverageEncoderTicks = Average(TPR,nSamples,&std);
  /*Set the m_KHeadErr parameter*/
  	*Ticks = Round(AverageEncoderTicks);
  	return TR_TaskComplete;
  Error:
  	return TaskResult;
  }
  
  
  TTaskResult CRobot::SpinAround(double OriginalHeading, int NumberOfRotations, bool CounterClockwise, short Speed)
  {
  	if (CounterClockwise) {
  		for (int i = 1; i <= NumberOfRotations; i++) {
  			AddHeadingToInstructions(OriginalHeading + 3*pi/4, NULL, pi/3, Speed, 0);
  			AddHeadingToInstructions(OriginalHeading + 5*pi/4, NULL, pi/3, Speed, 0);
  			AddHeadingToInstructions(OriginalHeading + 7*pi/4, NULL, pi/3, Speed, 0);
  			AddHeadingToInstructions(OriginalHeading + 2*pi, NULL, pi/3, Speed, 0);
  		}
  	/*Go to final heading with small heading zone*/
  		AddHeadingToInstructions(OriginalHeading + ((i-2)*2*pi + 2*pi), NULL, cSmlHeadingZone, Speed, 0);
  	} else {
  		for (int i = 1; i <= NumberOfRotations; i++) {
  			AddHeadingToInstructions(OriginalHeading - 3*pi/4, NULL, pi/3, Speed, 0);
  			AddHeadingToInstructions(OriginalHeading - 5*pi/4, NULL, pi/3, Speed, 0);
  			AddHeadingToInstructions(OriginalHeading - 7*pi/4, NULL, pi/3, Speed, 0);
  			AddHeadingToInstructions(OriginalHeading - 2*pi, NULL, pi/3, Speed, 0);
  		}
  	/*Go to final heading with small heading zone*/
  		AddHeadingToInstructions(OriginalHeading - ((i-2)*2*pi + 2*pi), NULL, cSmlHeadingZone, Speed, 0);
  	}
  	TTaskResult TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete);
  	return TaskResult;
  }
  
  TTaskResult CRobot::FindTicksPerRotation(int ApproximateTicksPerRotation, int* TicksPerRotation, int NumberOfRotations, bool CounterClockwise)
  /*******************************************************************************
  //Created: 11/05/96 S.R.
  //Last Revision: 02/28/97 S.R.
  //Parameters:
  //	ApproximateTicksPerRotation	A guess at how many encoder ticks it will take
  //												to make 1 full rotation.
  //	TicksPerRotation					Used to return the measured ticksper rotation.
  //	NumberOfRotations					The number of times to spin around to find
  //												ticks per rotation
  //	CounterClockwise:					Whether to turn clockwise or counterclockwise.
  //Return Value:
  //	The result of the task.
  ********************************************************************************
  //Description:
  //		This function will calibrate the number of ticks per rotation for a
  //	given surface.  The robot should be square to a wall before the function
  //	is called.  The number of ticks per rotation is calculated by first having
  //	the robot squared against a wall.  Then the robot backs up
  //	and spins around NumberOfRotations * 360 degrees.  Finally the robot runs into the wall and
  //	squares to it again.
  //Method:
  //		The function first purges the path to avoid confusion from some old path
  //	being executed.  Then the Heading Correction Factor is set to 0 so there is
  //	no heading correction term in the robot.  Then the robot tries to execute its
  //	calibration path.  If something goes wrong during the path, a negative number
  //	is returned.  Then the total number of encoder ticks needed for a full
  //	rotation is calculated.
  *******************************************************************************/
  {
  	ASSERT(TicksPerRotation != NULL);
  	ASSERT(NumberOfRotations != 0);
  	ASSERT(ApproximateTicksPerRotation >= 0);
  
  	TTaskResult TaskResult;
  
  	if (ApproximateTicksPerRotation <= 0) {
  		ApproximateTicksPerRotation = cNHeadings;
  	}
  /*Set the heading correction factor to 0 for the calibration*/
  	int HCF = (int)HeadingCorrectionFactor((float)ApproximateTicksPerRotation);
  	float TicksPer2pi = TotalTicksPerRotation((float)HCF);
  	SetHeadingCorrectionFactor(HCF);
  /*Store the initial heading*/
  	double WallHeading = GetTaskResultHeading();
  	int InitialHeading = int(WallHeading * cNHeadings / (2*pi));
  /*Back up from wall, do 360, smack into wall*/
  /*Back up far enough to avoid Home Base plus the travel I saw while spinning on white carpet.*/
  	robPOINT Destination;
  	Destination.x = m_Position.x - (cRRWidth/2 + 50 + m_HandleLength) * cos(WallHeading);
  	Destination.y = m_Position.y - (cRRWidth/2 + 50 + m_HandleLength) * sin(WallHeading); 
  	AddPositionVelocityToInstructions(Destination, NULL, cMinPositionZone, cObstacleDetectionSpeed, TRUE, 0);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Do 360*/
  	const short cTurnSpeed = cObstacleDetectionSpeed;
  	TaskResult = SpinAround(WallHeading, NumberOfRotations, CounterClockwise, cTurnSpeed);
  	if ( TaskResult != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Smack Into Wall*/
  	AddStraightVelocityToInstructions(m_Heading, cObstacleDetectionSpeed, NULL, 10000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Square To Wall*/
  	AddSquareToWallToInstructions(m_Heading, false);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  	double FinalHeading = GetTaskResultHeading();
  /*Figure out number of encoder ticks*/
  	double TotalEncoderTicks;
  	if (CounterClockwise) {
  		double delta = FinalHeading - WallHeading;
  		if ( (delta < 0) && (delta < -pi) ) delta += 2*pi;
  		else if ( (delta > 0) && (delta > pi) ) delta -= 2*pi;
  
  		TotalEncoderTicks = TicksPer2pi + (delta/NumberOfRotations)/(2*pi)*TicksPer2pi;
  	} else {
  		double delta = WallHeading - FinalHeading;
  		if ( (delta < 0) && (delta < -pi) ) delta += 2*pi;
  		else if ( (delta > 0) && (delta > pi) ) delta -= 2*pi;
  
  		TotalEncoderTicks = TicksPer2pi + (delta/NumberOfRotations)/(2*pi)*TicksPer2pi;
  	}
  	*TicksPerRotation = Round(TotalEncoderTicks);
  	return TR_TaskComplete;
  }
  
  TTaskResult CRobot::CheckHeadingCorrectionFactor(bool &GoodValue)
  /*******************************************************************************
  This function checks the heading correction factor by backing up from a wall,
  spinning 360 degrees, squaring up with the wall and seeing if the robot thinks
  it rotated a full 360 degrees.
  Before running this check, the robot must have been squared to the wall
  *******************************************************************************/
  {
  	GoodValue = false;
  	double WallHeading = GetTaskResultHeading();
  /*Back up from wall, do 360, smack into wall*/
  /*Back up far enough to avoid Home Base plus the travel I saw while spinning on white carpet.*/
  	robPOINT Destination;
  	Destination.x = m_Position.x - (cRRWidth/2 + 50 + m_HandleLength) * cos(WallHeading);
  	Destination.y = m_Position.y - (cRRWidth/2 + 50 + m_HandleLength) * sin(WallHeading); 
  	AddPositionVelocityToInstructions(Destination, NULL, cMinPositionZone, cObstacleDetectionSpeed, TRUE, 0);
  	TTaskResult TaskResult;
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Do 360*/
  	const int cNumberOfRotations = 5;
  	const short cTurnSpeed = cObstacleDetectionSpeed;
  	TaskResult = SpinAround(WallHeading, cNumberOfRotations, true, cTurnSpeed);
  	if ( TaskResult != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Smack Into Wall*/
  	AddStraightVelocityToInstructions(m_Heading, cObstacleDetectionSpeed, NULL, 10000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		return TaskResult;
  	}
  /*Square To Wall*/
  	AddSquareToWallToInstructions(m_Heading, false);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) == TR_TaskComplete ) {
  		double FinalHeading = GetTaskResultHeading();
  		double Delta = AbsAngularSeparation(WallHeading, FinalHeading);
  		bool OverRotated = AngularDifference(WallHeading, FinalHeading) >= pi;
  
  		if (Delta > 0.0175 * 5) {
  			GoodValue = false;
  		} else {
  			GoodValue = true;
  		}
  		CString DisplayText;
  		if (GoodValue) {
  			if (OverRotated) {
  				DisplayText.Format("%6.3f slip compensation is a good value.  Robot over rotated %6.3f degrees.", SlipPercent((float)m_HeadingCorrectionFactor), Delta*180/pi);
  			} else {
  				DisplayText.Format("%6.3f slip compensation is a good value.  Robot under rotated %6.3f degrees.", SlipPercent((float)m_HeadingCorrectionFactor), Delta*180/pi);
  			}
  		} else {
  			if (OverRotated) {
  				DisplayText.Format("%6.3f slip compensation is a bad value.  Robot over rotated %6.3f degrees.", SlipPercent((float)m_HeadingCorrectionFactor), Delta*180/pi);
  			} else {
  				DisplayText.Format("%6.3f slip compensation is a bad value.  Robot under rotated %6.3f degrees.", SlipPercent((float)m_HeadingCorrectionFactor), Delta*180/pi);
  			}
  		}
  		SetStatusBarText(DisplayText);
  	}
  	return TaskResult;
  }
  
  void CRobot::EStop(void)
  /*******************************************************************************
  //Created: 02/27/97 S.R.
  //Last Revision: 02/27/97 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //	This function performs an emergency stop on the robot.
  //Method:
  //		The function first sends a StopMotors command to the robot.  Then it
  //	purges the robot's path, and sets the State to Waiting.
  *******************************************************************************/
  {
  /*Stop the motors*/
  	Stop();
  	Pause(false);
  	if (m_InstructionList != NULL) {
  		if (m_InstructionList->Head != NULL) {
  			if (m_InstructionList->Head->InstructionNodeType == inBeep) {
  				SetBuzzerFrequency(0);
  			}
  		}
  	}
  /*Purge the robot's path*/
  	PurgeInstructions();
  	StopFindPath();
  	SetState(stWaiting);
  	SetFollowingPath(NULL);
  	m_Parent->RedrawWindow(NULL, NULL, RDW_NOERASE | RDW_INVALIDATE);
  /*Send the EStop message*/
  	m_Parent->PostMessage(ID_ROBOT_ESTOP, 0, 0);
  }
  
  void CRobot::OnCyeComPathBufferEmpty()
  {
  	if (m_InstructionList->Head != NULL) {
  		if (m_InstructionList->Head->InstructionNodeType == inCTDPathNode) {
  			CCTDPath* CurrentPath = m_InstructionList->Head->CTDPath;
  			if (CurrentPath != NULL) {
  				if (CurrentPath->m_PathPosition < CurrentPath->m_Size) {
  #ifdef _DEBUG
  //					AfxMessageBox("Path Buffer Empty");
  #endif
  				}
  			}
  		}
  	}
  }
  
  void CRobot::StartFollowingCTDPath(CCTDPath* Path)
  /*******************************************************************************
  //Created: 10/20/97 S.R.
  //Last Revision: 01/16/98 S.R.
  //Parameters:
  //	Path	pointer to the connect the dot path to start executing.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function starts the robot following a connect-the-dot path.
  //Method:
  //
  *******************************************************************************/
  {
  	const unsigned char cPathBufferSize = 24;
  	const unsigned char cInitialPathSend = 10;
  	if (Path != NULL) {
  //		if (Path->m_Size < cPathBufferSize) {
  //			m_RobotComm->SendPathSegment(TRUE, Path->m_Path, Path->m_Size);
  //			Path->m_PathPosition = Path->m_Size;
  //		} else {
  //			m_RobotComm->SendPathSegment(TRUE, Path->m_Path, cPathBufferSize);
  //			Path->m_PathPosition = cPathBufferSize;
  //		}
  //		m_RobotComm->SendFollowPath(Path->GetKb());	
  		if (Path->m_Size <= 0) return;
  		if (Path->m_PathPosition >= Path->m_Size) Path->m_PathPosition = Path->m_Size - 1;
  		if (Path->m_Size - Path->m_PathPosition < cInitialPathSend) {
  			m_RobotComm->SendPathSegment(true, Path->m_Path + Path->m_PathPosition, Path->m_Size - Path->m_PathPosition);
  			Path->m_PathPosition = Path->m_Size;
  		} else {
  			m_RobotComm->SendPathSegment(true, Path->m_Path + Path->m_PathPosition, cInitialPathSend);
  			Path->m_PathPosition += cInitialPathSend;
  		}
  		m_RobotComm->SendFollowPath(Path->GetSpeed());	
  
  		if (Path->m_PathPosition < Path->m_Size) {
  			if (Path->m_PathPosition + (cPathBufferSize - cInitialPathSend) < Path->m_Size) {
  				m_RobotComm->SendPathSegment(false, Path->m_Path + Path->m_PathPosition,
  					cPathBufferSize - cInitialPathSend);
  				Path->m_PathPosition += cPathBufferSize - cInitialPathSend;
  			} else {
  				m_RobotComm->SendPathSegment(false, Path->m_Path + Path->m_PathPosition,
  					Path->m_Size - Path->m_PathPosition);
  				Path->m_PathPosition = Path->m_Size;
  			}
  		}
  /*		
  		if (Path->m_Size < cPathBufferSize - cInitialPathSend) {
  			m_RobotComm->SendPathSegment(false, Path->m_Path, Path->m_Size);
  			Path->m_PathPosition = Path->m_Size;
  		} else {
  			m_RobotComm->SendPathSegment(false, Path->m_Path, cPathBufferSize - cInitialPathSend);
  			Path->m_PathPosition = cPathBufferSize - cInitialPathSend;
  		}
  */
  	}
  }
  
  void CRobot::AddPathToInstructions(Path* path, unsigned int Speed, double Zone, bool FlipHandle, unsigned long TimeOut, bool GoForward)
  /*******************************************************************************
  //Created: 01/07/98 S.R.
  //Last Revision: 01/07/98 S.R.
  //	path:	path to add to the instructions.
  //	Zone:	zone around end point must reach before going to next path point.
  //	FlipHandle:	value of flip handle while executing path.
  //	TimeOut:		amount of time to follow path.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function adds a path to the robot's instruction list.
  //Method:
  //
  *******************************************************************************/
  {
  	robPOINT Start;
  	if (path == NULL) return;
  
  	CCTDPath* NewPath = new CCTDPath(Speed);
  
  	int nPoints = path->GetNumberOfPoints();
  	if (GoForward) {
  		for (int i = 0; i < nPoints; i++) {
  			path->GetPoint(i, Start.x, Start.y);
  			NewPath->AddPoint(Start);
  		}
  	} else {
  		for (int i = nPoints - 1; i >= 0; i--) {
  			path->GetPoint(i, Start.x, Start.y);
  			NewPath->AddPoint(Start);
  		}
  	}
  	AddCTDPathToInstructions(NewPath, Zone, FlipHandle, TimeOut);
  }
  
  void CRobot::AddCTDPathToInstructions(CCTDPath* Path, double Zone, bool FlipHandle, unsigned long TimeOut)
  /*******************************************************************************
  //Created: 10/21/97 S.R.
  //Last Revision: 10/21/97 S.R.
  //Parameters:
  //	Path:	connect-the-dots path to add to the instructions.
  //	Zone:	zone around end point must reach before going to next path point.
  //	FlipHandle:	value of flip handle while executing path.
  //	TimeOut:		amount of time to follow path.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		Adds a connect-the-dot path segment to the robot's instruction list.  The
  //	path will be deleted after execution.
  //Method:
  //
  *******************************************************************************/
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->Position = Path->m_Path[Path->m_Size - 1];
  	NewNode->InstructionNodeType = inCTDPathNode;
  	NewNode->SetTimeOut(TimeOut);
  	if (Zone <= cMinPositionZone) NewNode->Zone = cMinPositionZone;
  	else NewNode->Zone = Zone;
  	NewNode->bParm0 = FlipHandle;
  	NewNode->CTDPath = Path;
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::SetHandleLength(short HandleLength)
  {
  	m_HandleLength = HandleLength;
  	m_RobotComm->SendHandleLength(m_HandleLength);
  }
  
  void CRobot::SerializeV2(CArchive & archive)
  {
  	UINT temp;
  	unsigned char uctemp;
  	CObject::Serialize(archive);
  	if (archive.IsStoring()) {
  		temp = m_AttachmentType;
  		archive << temp;
  		temp = m_FloorType;
  		archive << temp;
  		uctemp = unsigned char(m_HandleLength), archive << uctemp;
  		archive << m_HeadingError;
  		archive << m_PositionError.x, archive << m_PositionError.y;
  		archive << m_HeadingError;
  		temp = m_UpdateProbabilityGrid;
  		archive << temp;
  		archive << m_MaxSpeed;
  		archive << m_KHeadErr;
  		temp = false;
  		archive << temp;
  		temp = m_ShowCalibrationSuccessfulDialog;
  		archive << temp;
  	} else {
  		archive >> temp;
  		SetAttachmentType((TAttachmentType)temp);
  		archive >> temp;
  		SetFloorType((TFloorType)temp);
  		archive >> uctemp, m_HandleLength = uctemp;
  		archive >> m_HeadingError;
  		archive >> m_PositionError.x, archive >> m_PositionError.y;
  		archive >> m_HeadingError;
  		archive >> temp;
  		m_UpdateProbabilityGrid = temp != 0;
  		archive >> m_MaxSpeed;
  		archive >> m_KHeadErr;
  		archive >> temp;
  //		m_AutoCalibrate = temp != 0;
  		archive >> temp;
  		m_ShowCalibrationSuccessfulDialog = temp != 0;
  
  		SetMaxSpeed(m_MaxSpeed);
  		SetHandleLength(m_HandleLength);
  
  		((CMainFrame*)m_Parent->GetParentFrame())->OnSetEnableLearnRoom(m_UpdateProbabilityGrid);
  //		((CMainFrame*)m_Parent->GetParentFrame())->OnSetAutoCalibrate(m_AutoCalibrate);
  		((CMainFrame*)m_Parent->GetParentFrame())->OnSetMaxSpeed(m_MaxSpeed);
  	}
  }
  
  void CRobot::Serialize(CArchive & archive)
  {
  	UINT temp;
  	UINT nVersion;
  	unsigned char uctemp;
  	CObject::Serialize(archive);
  	if (archive.IsStoring()) {
  		nVersion = VERSION_NUMBER;
  		archive << nVersion;
  		temp = m_AttachmentType;
  		archive << temp;
  		temp = m_FloorType;
  		archive << temp;
  		uctemp = (unsigned char)m_HandleLength, archive << uctemp;
  		archive << m_HeadingError;
  		archive << m_PositionError.x, archive << m_PositionError.y;
  		archive << m_HeadingError;
  		temp = m_UpdateProbabilityGrid;
  		archive << temp;
  		archive << m_MaxSpeed;
  		archive << m_KHeadErr;
  //		temp = m_AutoCalibrate;
  		archive << temp;
  		temp = m_ShowCalibrationSuccessfulDialog;
  		archive << temp;
  		archive << m_Address;
  		temp = m_Color;
  		archive << temp;
  		archive << m_Version;
  	} else {
  		archive >> nVersion;
  		archive >> temp;
  		SetAttachmentType((TAttachmentType)temp);
  		archive >> temp;
  		SetFloorType((TFloorType)temp);
  		archive >> uctemp, m_HandleLength = uctemp;
  		archive >> m_HeadingError;
  		archive >> m_PositionError.x, archive >> m_PositionError.y;
  		archive >> m_HeadingError;
  		archive >> temp;
  		m_UpdateProbabilityGrid = temp != 0;
  		archive >> m_MaxSpeed;
  		archive >> m_KHeadErr;
  		archive >> temp;
  //		m_AutoCalibrate = temp != 0;
  		archive >> temp;
  		m_ShowCalibrationSuccessfulDialog = temp != 0;
  		short readAddress;
  		if (nVersion > 7) {
  			archive >> readAddress;
  		} else {
  			readAddress = m_Address;
  		}
  		if (nVersion > 8) {
  			archive >> temp;
  			m_Color = (TRobotColor)temp;
  			archive >> m_Version;
  		}
  		SetAddress(readAddress);
  		SetMaxSpeed(m_MaxSpeed);
  		SetHandleLength(m_HandleLength);
  
  		((CMainFrame*)m_Parent->GetParentFrame())->OnSetEnableLearnRoom(m_UpdateProbabilityGrid);
  //		((CMainFrame*)m_Parent->GetParentFrame())->OnSetAutoCalibrate(m_AutoCalibrate);
  		((CMainFrame*)m_Parent->GetParentFrame())->OnSetMaxSpeed(m_MaxSpeed);
  	}
  }
  
  void CRobot::New()
  /*******************************************************************************
  //Created: 11/14/97 S.R.
  //Last Revision: 11/14/97 S.R.
  //Parameters:
  //	none.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		Call this function when the application creates a new document.
  //Method:
  //
  *******************************************************************************/
  {
  	m_ShowCalibrationSuccessfulDialog = false;
  	m_HandleLength = (short)cRRDefaultHandleLength;
  /*Initialize destination*/
  	m_Destination.x = 0;
  	m_Destination.y = 0;
  /*Initialize Motor Obstacle*/
  	m_MotorObstacle.ValidObstacle = FALSE;
  /*Initialize Nudges*/
  	m_Nudged = NDG_None;
  /*Initialize Errors*/
  	m_PositionError.x = 0;
  	m_PositionError.y = 0;
  	m_HeadingError = 0;
  	SetState(stWaiting);
  	m_UpdateProbabilityGrid = TRUE;
  //	SetAutoCalibrate(FALSE);
  /*Send default data to the robot*/
  	((CMainFrame*)m_Parent->GetParentFrame())->OnSetEnableLearnRoom(m_UpdateProbabilityGrid);
  //	((CMainFrame*)m_Parent->GetParentFrame())->OnSetAutoCalibrate(m_AutoCalibrate);
  	((CMainFrame*)m_Parent->GetParentFrame())->OnSetMaxSpeed(m_MaxSpeed);
  }
  
  void CRobot::SetMaxSpeed(short MaxSpeed)
  {
  	ASSERT(MaxSpeed >= 0);
  	if (MaxSpeed < 0) MaxSpeed = -MaxSpeed;
  	m_MaxSpeed = MaxSpeed;
  	m_RobotComm->SendMaxSpeed(MaxSpeed);
  }
  
  #ifdef IMPLEMENTED
  TTaskResult CRobot::CalibrateAgainstCornerWall(CCornerWall* CalibrationZone)
  /*******************************************************************************
  //Created: S.R.
  //Last Revision: 01/27/98 S.R.
  //	CalibrationZone:	the calibration zone to calibrate against.
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
  	TTaskResult TaskResult;
  	TCoordSystem CalCoordSys;
  	robPOINT P;
  	double theta;
  /*Create a coordinate system for the calibration doorway*/
  	CalCoordSys.Origin.x = (CalibrationZone->m_P0.x + CalibrationZone->m_P1.x) / 2;
  	CalCoordSys.Origin.y = (CalibrationZone->m_P0.y + CalibrationZone->m_P1.y) / 2;
  	CalCoordSys.Phi = CalibrationZone->m_Heading;
  /*Assume we're already in front of the calibration wall, and just run into it.*/
  	theta = HeadingToDestination(m_Position, CalCoordSys.Origin);
  	SetObstacleSensitivity(osMediumSensitivity);
  	AddHeadingToInstructions(theta, NULL, cModHeadingZone, cComputerControlHeadingSpeed, 10000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  		goto Error;
  	}
  /*Set high obstacle detection for hitting the wall*/
  //	SetObstacleSensitivity(osHighSensitivity);
  	AddStraightVelocityToInstructions(theta, cObstacleDetectionSpeed, NULL, 10000);
  	if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  		goto Error;
  	}
  /*Square to the wall*/
  	if ( (TaskResult = SquareToWall(m_MotorObstacle.MotorObstacle)) != TR_TaskComplete ) {
  		goto Error;
  	}
  /*Update the position and heading*/
  	m_Heading = CalibrationZone->m_Heading;
  	TLine L;
  	FindLineFrom2Points(CalibrationZone->m_P0, CalibrationZone->m_P1, &L);
  	P.x = m_Position.x;
  	P.y = m_Position.y;
  	P = FindPerpendicularIntersection(L, P);
  
  	m_Position.x = P.x + cRRNoseLength * cos(CalibrationZone->m_Heading + pi);
  	m_Position.y = P.y + cRRNoseLength * sin(CalibrationZone->m_Heading + pi);
  
  	SetHeading(CalibrationZone->m_Heading);
  	SetPosition(m_Position);
  /*Update Errors*/
  	m_PositionError.x = 0;
  	m_PositionError.y = 0;
  	m_HeadingError = 0;
  
  /*Calibrate on second wall*/
  	SetHandleLength(10);
  //	unsigned char FeelingMotor;
  	double phi;
  	phi = CalibrationZone->m_Heading2;
  /*	if ( AbsAngularSeparation(phi, CalibrationZone->m_Heading - pi/2) < AbsAngularSeparation(phi, CalibrationZone->m_Heading + pi/2) ) {
  		phi = CalibrationZone->m_Heading - pi/2;
  		FeelingMotor = 1;
  	} else {
  		phi = CalibrationZone->m_Heading + pi/2;
  		FeelingMotor = 0;
  	}
  */
  	TInstructionResult InstructionResult;
  	InstructionResult = IR_InstructionComplete;
  	CCTDPath* StraightPath;
  	while (InstructionResult == IR_InstructionComplete) {
  	/*Back up*/
  		P.x = m_Position.x - (cRRWidth/2 + CCornerWall::CornerBackupDistance + 10/*Handle Length*/ - cRRNoseLength) * cos(CalibrationZone->m_Heading);
  		P.y = m_Position.y - (cRRWidth/2 + CCornerWall::CornerBackupDistance + 10/*Handle Length*/ - cRRNoseLength) * sin(CalibrationZone->m_Heading);
  		AddPositionVelocityToInstructions(P, NULL, 10, cComputerControlSpeed, TRUE, 10000);
  		P.x = m_Position.x - (cRRWidth/2 + CCornerWall::CornerBackupDistance - cRRNoseLength) * cos(CalibrationZone->m_Heading);
  		P.y = m_Position.y - (cRRWidth/2 + CCornerWall::CornerBackupDistance - cRRNoseLength) * sin(CalibrationZone->m_Heading);
  	/*Turn towards second wall and try to hit it*/
  		AddHeadingToInstructions(phi, NULL, 0, cComputerControlHeadingSpeed, 10000);
  		SetObstacleSensitivity(osMediumSensitivity);
  		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  			goto Error;
  		}
  		StraightPath = new CCTDPath();
  		robPOINT EndPoint;
  		EndPoint.x = P.x + (30.0*cTPI) * cos(CalibrationZone->m_Heading2);
  		EndPoint.y = P.y + (30.0*cTPI) * sin(CalibrationZone->m_Heading2);
  		StraightPath->AddStraightLine(P, EndPoint);
  //		AddStraightVelocityToInstructions(phi, cObstacleDetectionSpeed, NULL, 2500);
  		AddCTDPathToInstructions(StraightPath, cSmlPositionZone);
  //		SetObstacleSensitivity(osHighSensitivity);
  		if ( (InstructionResult = FollowInstructions(TRUE, TRUE)) == IR_InstructionComplete ) {
  	/*Missed Corner*/
  		/*Backup, face main wall*/
  			SetObstacleSensitivity(osMediumSensitivity);
  			P.x = EndPoint.x;
  			P.y = EndPoint.y;
  			EndPoint.x = P.x - (cRRWidth/2 + 3*cTPI) * cos(CalibrationZone->m_Heading2);
  			EndPoint.y = P.y - (cRRWidth/2 + 3*cTPI) * sin(CalibrationZone->m_Heading2);
  			StraightPath = new CCTDPath();
  			StraightPath->AddStraightLine(P, EndPoint);
  			AddCTDPathToInstructions(StraightPath, cSmlPositionZone, true);
  			AddHeadingToInstructions(CalibrationZone->m_Heading, NULL, 0, cComputerControlHeadingSpeed, 10000);
  			if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_InstructionComplete)) != TR_TaskComplete ) {
  				goto Error;
  			}
  //			SetObstacleSensitivity(osHighSensitivity);
  		/*Run into the wall and square up*/
  			AddStraightVelocityToInstructions(CalibrationZone->m_Heading, cObstacleDetectionSpeed, NULL, 10000);
  			if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  				fprintf(stderr, "CornerWallCalibration Error!!, Back up Screwed up, TaskResult = %d\n", TaskResult);
  				goto Error;
  			}
  			if ( (TaskResult = SquareToWall(m_MotorObstacle.MotorObstacle)) != TR_TaskComplete ) {
  				fprintf(stderr, "CornerWallCalibration Error!!, Square To Wall Screwed up, TaskResult = %d\n", TaskResult);
  				goto Error;
  			}
  			m_Heading = CalibrationZone->m_Heading;
  		} else if (InstructionResult == IR_HitObstacle) {
  			/*Done searching for corner wall*/
  		} else {
  			fprintf(stderr, "CornerWallCalibration Error!!, InstructionResult = %d\n",InstructionResult);
  			goto Error;
  		}
  	}
  /*Run against the corner and recalculate the robot's position from this point*/
  	if (CalibrationZone->m_FeelingMotor == 0) {
  		AddVelocitiesToInstructions(cObstacleDetectionSpeed, 0, 0);
  		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  			goto Error;
  		}
  		P.x = m_Position.x + cRRWidth/2 * cos(m_Heading - pi/2) + cRRNoseLength * cos(m_Heading);
  		P.y = m_Position.y + cRRWidth/2 * sin(m_Heading - pi/2) + cRRNoseLength * sin(m_Heading);
  
  		m_Position.x += CalibrationZone->m_CalPoint3.x - P.x;
  		m_Position.y += CalibrationZone->m_CalPoint3.y - P.y;
  		SetPosition(m_Position);
  	} else {
  		AddVelocitiesToInstructions(0, cObstacleDetectionSpeed, 0);
  		if ( (TaskResult = FollowInstructions(TRUE, TRUE, IR_HitObstacle)) != TR_TaskComplete ) {
  			goto Error;
  		}
  		P.x = m_Position.x + cRRWidth/2 * cos(m_Heading + pi/2) + cRRNoseLength * cos(m_Heading);
  		P.y = m_Position.y + cRRWidth/2 * sin(m_Heading + pi/2) + cRRNoseLength * sin(m_Heading);
  
  		m_Position.x += CalibrationZone->m_CalPoint3.x - P.x;
  		m_Position.y += CalibrationZone->m_CalPoint3.y - P.y;
  		SetPosition(m_Position);
  	}
  /*Update Errors*/
  	m_PositionError.x = 0;
  	m_PositionError.y = 0;
  	m_HeadingError = 0;
  //	if ( (TaskResult = SquareToWall(m_MotorObstacle.MotorObstacle)) != TR_TaskComplete ) {
  //		return TaskResult;
  //	}
  ///*Update the position and heading*/
  //	m_Heading = CalibrationZone->m_Heading2;
  //	FindLineFromPointAndSlope(CalibrationZone->m_P2, tan(CalibrationZone->m_Heading2), &L);
  //	P.x = m_Position.x;
  //	P.y = m_Position.y;
  //	P = FindPerpendicularIntersection(L, P);
  //
  //	m_Position.x = P.x - cRRNoseLength * cos(CalibrationZone->m_Heading2);
  //	m_Position.y = P.y - cRRNoseLength * sin(CalibrationZone->m_Heading2);
  //
  //	m_RobotComm->SendHeading(m_Heading);
  //	m_RobotComm->SendPosition(m_Position);
  ///*Update Errors*/
  //	m_PositionError.x = 0;
  //	m_PositionError.y = 0;
  //	m_HeadingError = 0;
  	return TR_TaskComplete;
  Error:
  	return TR_Error;
  }
  #endif
  
  void CRobot::SetObstacleSensitivity(TObstacleSensitivity Sensitivity)
  {
  	m_ObstacleSensitivity = Sensitivity;
  	m_RobotComm->SendObstacleSensitivity(Sensitivity);
  }
  
  void CRobot::SetFlipHandle(bool FlipHandle)
  {
  	m_FlipHandle = FlipHandle;
  	m_RobotComm->SendFlipHandle(FlipHandle);
  }
  
  void CRobot::SetState(TState State)
  {
  	if ( (State != stJoysticking) && (State != stDragging) ) {
  		SetRetryLimit(cInfiniteRetry);
  	} else {
  		SetRetryLimit(7);
  	}
  	m_State = State;
  	m_Parent->SendMessage(ID_ROBOT_CHANGE_STATE, State, 0);
  }
  
  void CRobot::StopFindPath()
  /*******************************************************************************
  //Created: 01/05/98 S.R.
  //Last Revision: 01/07/98 S.R.
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
  	m_FindPathThread->SetStopFindPath();
  }
  
  
  void CRobot::EnableComm(bool Enable)
  {
  }
  
  void CRobot::SetBuzzerFrequency(short Frequency)
  {
     m_RobotComm->SendBuzzerFrequency(Frequency);
  }
  
  void CRobot::SetBuzzerOn(bool On)
  {
     m_RobotComm->SendBuzzerOn(On);
  }
  
  void CRobot::SetHeadingCorrectionFactor(int Correction)
  {
  	m_HeadingCorrectionFactor = Correction;
  	m_RobotComm->SendHeadingCorrectionFactor(Correction);
  }
  
  void CRobot::SetMotorControlConstants(char Kp, char Ki, char Kd, char Kb, char Kpwm)
  {
  	m_Kp = Kp;
  	m_Ki = Ki;
  	m_Kd = Kd;
  	m_Kb = Kb;
  	m_Kpwm = Kpwm;
  	m_RobotComm->SendMotorControlConstants(Kp, Ki, Kd, Kb, Kpwm);
  }
  
  void CRobot::GetMotorControlConstants(char& Kp, char& Ki, char& Kd, char& Kb, char& Kpwm)
  {
  	Kp = m_Kp;
  	Ki = m_Ki;
  	Kd = m_Kd;
  	Kb = m_Kb;
  	Kpwm = m_Kpwm;
  }
  
  void CRobot::SetPwm(int M0Pwm, int M1Pwm)
  {
  	m_RobotComm->SendMotorsPWM(M0Pwm, M1Pwm);
  }
  
  void CRobot::SetP1(long P1)
  {
     m_RobotComm->SendP1(P1);
  }
  
  void CRobot::SetP2(long P2)
  {
     m_RobotComm->SendP2(P2);
  }
  
  void CRobot::SetP3(long P3)
  {
     m_RobotComm->SendP3(P3);
  }
  
  void CRobot::SetP4(long P4)
  {
     m_RobotComm->SendP4(P4);
  }
  
  void CRobot::SetP5(long P5)
  {
     m_RobotComm->SendP5(P5);
  }
  
  void CRobot::SetP6(long P6)
  {
     m_RobotComm->SendP6(P6);
  }
  
  void CRobot::SetP7(long P7)
  {
     m_RobotComm->SendP7(P7);
  }
  
  void CRobot::SetP8(long P8)
  {
     m_RobotComm->SendP8(P8);
  }
  
  void CRobot::SetVelocities(int M0V, int M1V)
  {
  	m_RobotComm->SendMotorVelocities(M0V, M1V);
  }
  
  void CRobot::SetPositionVelocityDestination(robPOINT Destination, short Velocity)
  {
     m_RobotComm->SendPositionVelocityDestination(Destination, Velocity);
  }
  
  void CRobot::PurgeRingBuffer()
  {
  	m_RobotComm->SendPurgeRingBuffer();
  }
  
  void CRobot::SetPositionPwmDestination(robPOINT Destination)
  {
  	m_RobotComm->SendPositionDestination(Destination);
  }
  
  void CRobot::SetMaxPwm(int MaxPwm)
  {
  	m_RobotComm->SendMaxPWM(MaxPwm);
  }
  
  void CRobot::SetStraightVelocity(int Velocity, double Heading)
  {
  	if ( (Velocity >= 0) && (Velocity > m_MaxSpeed) ){
  		Velocity = m_MaxSpeed;
  	} else if (Velocity < -m_MaxSpeed) {
  		Velocity = -m_MaxSpeed;
  	}
  	m_RobotComm->SendStraightVelocity(Velocity, Heading);
  }
  
  void CRobot::SetCommunicationOn(bool On)
  {
  }
  
  bool CRobot::GetCommunicationOn()
  {
  	return true;
  }
  
  bool CRobot::GoodPath(Path * path, bool recalc)
  {
  	return (CFindPathThread::GoodPath(path, recalc));
  }
  
  void CRobot::SetVelocityDirection(int Velocity, double Heading)
  {
  	if ( (Velocity >= 0) && (Velocity > m_MaxSpeed) ){
  		Velocity = m_MaxSpeed;
  	} else if (Velocity < -m_MaxSpeed) {
  		Velocity = -m_MaxSpeed;
  	}
  	m_RobotComm->SendVelocityDirection(Velocity, Heading);
  }
  
  void CRobot::SetHeadingDestination(double Heading, short Velocity)
  {
  	m_RobotComm->SendHeadingDestination(Heading, Velocity);
  }
  
  void CRobot::SetTurn(double Heading, short MaxSpeed, short TurnDiameter)
  {
  	m_RobotComm->SendTurn(Heading, MaxSpeed, TurnDiameter);
  }
  
  void CRobot::Stop()
  {
  	m_RobotComm->SendStopMotors();
  }
  
  void CRobot::SetEStopVisible(bool Visible)
  {
  	if (Visible) {
  		m_DlgEStop->ShowWindow(SW_SHOW);
  	} else {
  		m_DlgEStop->ShowWindow(SW_HIDE);
  	}
  }
  
  bool CRobot::GetEStopVisible()
  {
  	return (m_DlgEStop->IsWindowVisible() == TRUE);
  }
  
  bool CRobot::GetFlipHandle()
  {
  	return m_FlipHandle;
  }
  
  void CRobot::SetInformationToTransmit(bool Velocity, bool Acceleration, bool AD, bool Position)
  {
  	long mask;
  	mask = ((long)Velocity) | ((long)Acceleration<<1) | ((long)AD<<2) | ((long)Position<<3);
  	m_RobotComm->SendInformationToTransmit(mask);
  }
  
  void CRobot::SendRequestState()
  {
  	m_RobotComm->SendRequestState();
  }
  
  void CRobot::ShowCalibrationSuccessfulDialog(robPOINT DPosition, double DHeading)
  {
  	if (GetFloorType() != ftWood) {
  		DPosition.x /= cTPI;
  		DPosition.y /= cTPI;
  		DHeading = DHeading / (pi) * 180;
  		if (DHeading > 180) DHeading -= 360;
  		if (DHeading < -180) DHeading += 360;
  
  		CString ResultText;
  		ResultText.Format("dX = %4.1f'', dY = %4.1f'', dHeading = %3.1f degrees", DPosition.x, DPosition.y, DHeading);
  
  		SetStatusBarText(ResultText);
  
  		if (m_ShowCalibrationSuccessfulDialog) {
  			CDlgCalibrationSuccessful CalSuccessful;
  			CalSuccessful.m_edtDX = float(DPosition.x);
  			CalSuccessful.m_edtDY = float(DPosition.y);
  			CalSuccessful.m_edtDH = float(DHeading);
  			CalSuccessful.DoModal();
  			m_ShowCalibrationSuccessfulDialog = !CalSuccessful.m_chkDontShowAgain;
  		}
  	}
  }
  
  void CRobot::TurnOffPower()
  {
  	m_RobotComm->PowerDown();
  }
  
  robPOINT CRobot::GetHandlePosition(short HandleLength)
  {
  	robPOINT HandlePos = GetPosition();
  	HandlePos.x += HandleLength * cos(GetHeading());
  	HandlePos.y += HandleLength * sin(GetHeading());
  	return HandlePos;
  }
  
  robPOINT CRobot::GetHandlePosition(void)
  {
  	robPOINT HandlePos = GetPosition();
  	HandlePos.x += m_HandleLength * cos(GetHeading());
  	HandlePos.y += m_HandleLength * sin(GetHeading());
  	return HandlePos;
  }
  
  void CRobot::SetOnHomeBase(bool On)
  {
  	if (On) m_TouchedHomeBase = true;
  	m_OnHomeBase = On;
  	m_CyePersona->SetOnHomeBase(On);
  }
  
  void CRobot::GetVelocities(short & M0V, short & M1V)
  {
  	M0V = m_M0VelocityMeasured;
  	M1V = m_M1VelocityMeasured;
  	return;
  }
  
  void CRobot::SendSerialString(CString String)
  {
  	m_RobotComm->SendString(String);
  }
  
  void CRobot::AddSquareToWallToInstructions(double Heading, bool SlipperySurface)
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inSquareToWall;
  	NewNode->SetTimeOut(0);
  	NewNode->Heading =  Heading;
  	NewNode->bParm0 = SlipperySurface;
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::SetTaskResult(TInstructionResult TaskResult, robPOINT Position, double Heading)
  {
  	m_RobotInstructionResult.Result = TaskResult;
  	m_RobotInstructionResult.Position = Position;
  	m_RobotInstructionResult.Heading = Heading;
  	m_RobotInstructionResult.Valid = true;
  }
  
  void CRobot::SendSquareToWall(double Heading, bool SlipperySurface)
  {
  	m_RobotComm->SquareToWall(Heading, 7.0f, SlipperySurface);
  }
  
  void CRobot::SetFeelingMotor(unsigned char Motor)
  {
  	m_RobotComm->SendFeelingMotor(Motor);
  }
  
  void CRobot::AddFeelMotorToInstructions(unsigned char Motor, unsigned long TimeOut)
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inFeelMotor;
  	NewNode->SetTimeOut(TimeOut);
  	NewNode->cParm0 = Motor;
  	AddNodeToInstructions(NewNode);
  }
  
  /*void CRobot::SendWallCalibrate(robPOINT P0, double H0, robPOINT P1, double H1, bool Corner, unsigned char FeelWheel)
  {
  	m_RobotComm->SendWallCalibrate(P0, H0, P1, H1, Corner, FeelWheel);
  }
  */
  
  /*void CRobot::AddWallCalibrationToInstructions(robPOINT P0, double H0, robPOINT P1, double H1, bool Corner, unsigned char FeelWheel)
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inWallCalibration;
  	NewNode->SetTimeOut(0);
  	NewNode->Position = P0;
  	NewNode->Heading = H0;
  	NewNode->Position2 = P1;
  	NewNode->Heading2 = H1;
  	NewNode->bParm0 = Corner;
  	NewNode->cParm0 = FeelWheel;
  	AddNodeToInstructions(NewNode);
  }
  */
  
  void CRobot::AddCornerCalibrationToInstructions(robPOINT CalibrationWallCenter, double CalibrationWallHeading, unsigned char FeelWheel, bool SlipperySurface, short WheelDistanceFromWall)
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inCornerCalibration;
  	NewNode->SetTimeOut(0);
  	NewNode->Position = CalibrationWallCenter;
  	NewNode->Heading = CalibrationWallHeading;
  	NewNode->cParm0 = FeelWheel;
  	NewNode->bParm0 = SlipperySurface;
  	NewNode->sParm0 = WheelDistanceFromWall;
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::SendCornerCalibrate(robPOINT CalibrationWallCenter, double CalibrationWallHeading, unsigned char FeelWheel, bool SlipperySurface,short WheelDistanceFromWall)
  {
  	if (SlipperySurface) {
  		m_RobotComm->SendCornerCalibrate(CalibrationWallCenter, CalibrationWallHeading, FeelWheel, 4.0f, SlipperySurface, WheelDistanceFromWall);
  	} else {
  		m_RobotComm->SendCornerCalibrate(CalibrationWallCenter, CalibrationWallHeading, FeelWheel, 7.0f, SlipperySurface, WheelDistanceFromWall);
  	}
  }
  
  void CRobot::SendFindHomeBase(double CornerHeading, double WallHeading, robPOINT HomeBaseLocation, bool NeedToGetInFrontOfHomeBase)
  {
  	m_RobotComm->SendFindHomeBase(CornerHeading, WallHeading, HomeBaseLocation, NeedToGetInFrontOfHomeBase);
  }
  
  void CRobot::AddFindHomeBaseToInstructions(double CornerHeading, double WallHeading, robPOINT HomeBaseLocation, bool NeedToGetInFrontOfHomeBase)
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inFindHomeBase;
  	NewNode->SetTimeOut(0);
  	NewNode->Heading = CornerHeading;
  	NewNode->Heading2 = WallHeading;
  	NewNode->Position = HomeBaseLocation;
  	NewNode->bParm0 = NeedToGetInFrontOfHomeBase;
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::SetRetryLimit(long retry)
  {
  	m_RobotComm->SetRetryLimit(retry);
  }
  
  long CRobot::GetRetryLimit()
  {
  	return m_RobotComm->GetRetryLimit();
  }
  
  void CRobot::Pause(bool p)
  {
  	m_RobotComm->SendPause(p);
  	m_Pause = p;
  }
  
  void CRobot::SetShowCalibrationSuccessfulDialog(bool show)
  {
  	m_ShowCalibrationSuccessfulDialog = show;
  }
  
  bool CRobot::GetShowCalibrationSuccessfulDialog()
  {
  	return m_ShowCalibrationSuccessfulDialog;
  }
  
  TTaskResult CRobot::BackupFromWall(bool WallInFront, short Speed)
  /*Backs the robot up from a wall.  Note!!! This function will change the state of FlipHandle*/
  {
  	robPOINT Backup = GetHandlePosition(-GetHandleLength());
  	if (WallInFront) {
  		Backup.x -= (cRRWidth) * cos(m_Heading);
  		Backup.y -= (cRRWidth) * sin(m_Heading);
  	} else {
  		Backup.x += (cRRWidth) * cos(m_Heading);
  		Backup.y += (cRRWidth) * sin(m_Heading);
  	}
  	AddPositionVelocityToInstructions(Backup, NULL, cModPositionZone, Speed, true, 0);
  	return FollowInstructions(true, true, IR_InstructionComplete);
  }
  
  /*void CRobot::SetVacuumAttached(bool Attached)
  {
  	m_VacuumAttached = Attached;
  	const short cVacuumingHeadingCorrectionFactor = -39;
  	SetRetryLimit(0);
  	if (m_VacuumAttached) {
  		SetHeadingCorrectionFactor(cVacuumingHeadingCorrectionFactor);
  	} else {
  		SetHeadingCorrectionFactor(cRRDefaultHeadingCorrection);
  	}
  	AddWaitLongToInstructions(0,0,2);
  	FollowInstructions();
  }
  */
  
  /*bool CRobot::GetVacuumAttached()
  {
  	return m_VacuumAttached;
  }
  */
  
  float CRobot::GetRadioLevel()
  {
  	return m_RobotComm->GetRadioLevel();
  }
  
  double CRobot::SetFindPathBuffer(double buffer)
  /*This function sets the buffer zone around the robot for finding paths*/
  /*buffer should be in inches*/
  {
  	double OldFindPathBuffer = m_FindPathBuffer;
  	m_FindPathBuffer = buffer;
  	m_Parent->m_ProbabilityGrid->m_RobotWorld->rover.SetPathBuffer(buffer);
  	return OldFindPathBuffer;
  }
  
  CInstructionListNode* CRobot::GetCurrentInstruction()
  {
  	return m_InstructionList->Head;
  }
  
  double CalculateNextSwatchDistance(robRECT Area, TLine CurrentSwatchLine, double NextSwatchHeading);
  double CalculateNextSwatchDistance(robRECT Area, robPOINT SwatchStart, double NextSwatchHeading, double SearchHeading);
  double CalculateNextSwatchDistance(robPOINT CurrentPosition, TLine NextSwatchLine, double NextSwatchHeading);
  
  void CRobot::SearchArea(robRECT Area)
  /*this function controls the robot as it is searching out a rectangular area*/
  {
  	Area.Normalize();
  	if ((Area.Width() < cRRWidth) && (Area.Height() < cRRWidth)) return;
  	TRobotInfo OldRobotInfo = GetRobotInfo();
  //	bool OldEStopVisible = GetEStopVisible();
  	SetEStopVisible(true);
  //	TState OldState = GetState();
  	SetState(stExecuting);
  //	unsigned char OldHandleLength = GetHandleLength();
  //	TObstacleSensitivity OldSensitivity = GetObstacleSensitivity();
  	SetObstacleSensitivity(osHighSensitivity);
  	SetEnableObstacleBounce(false);
  
  	/*Find nearest corner*/
  	robPOINT Corner = Area.TopLeft();
  	double MinDist = Distance(m_Position, Corner);
  	int NearestCorner = 0;
  	Corner.x = Area.right;
  	double dist;
  	if ( (dist = Distance(m_Position, Corner)) < MinDist ) {
  		MinDist = dist;
  		NearestCorner = 1;
  	}
  	Corner.y = Area.bottom;
  	if ( (dist = Distance(m_Position, Corner)) < MinDist ) {
  		MinDist = dist;
  		NearestCorner = 2;
  	}
  	Corner.x = Area.left;
  	if ( (dist = Distance(m_Position, Corner)) < MinDist ) {
  		MinDist = dist;
  		NearestCorner = 3;
  	}
  	double SearchHeading;
  	double NextSwatchHeading;
  	robPOINT SwatchStart;
  	TLine CurrentSwatchLine;
  	if (Area.Height() >= Area.Width()) {
  		switch (NearestCorner) {
  		case 0:
  			SearchHeading = -pi/2;
  			NextSwatchHeading = 0;
  			SwatchStart = Area.TopLeft();
  
  			CurrentSwatchLine.m = 0;
  			CurrentSwatchLine.Vertical = true;
  			CurrentSwatchLine.b = Area.left + cRRWidth/2;
  			break;
  		case 1:
  			SearchHeading = -pi/2;
  			NextSwatchHeading = pi;
  			SwatchStart.x = Area.right;
  			SwatchStart.y = Area.top;
  
  			CurrentSwatchLine.m = 0;
  			CurrentSwatchLine.Vertical = true;
  			CurrentSwatchLine.b = Area.right - cRRWidth/2;
  			break;
  		case 2:
  			SearchHeading = pi/2;
  			NextSwatchHeading = pi;
  			SwatchStart.x = Area.right;
  			SwatchStart.y = Area.bottom;
  
  			CurrentSwatchLine.m = 0;
  			CurrentSwatchLine.Vertical = true;
  			CurrentSwatchLine.b = Area.right - cRRWidth/2;
  			break;
  		case 3:
  			SearchHeading = pi/2;
  			NextSwatchHeading = 0;
  			SwatchStart.x = Area.left;
  			SwatchStart.y = Area.bottom;
  
  			CurrentSwatchLine.m = 0;
  			CurrentSwatchLine.Vertical = true;
  			CurrentSwatchLine.b = Area.left + cRRWidth/2;
  			break;
  		default:
  			ASSERT(FALSE);
  			SearchHeading = -pi/2;
  			NextSwatchHeading = 0;
  			SwatchStart = Area.TopLeft();
  
  			CurrentSwatchLine.m = 0;
  			CurrentSwatchLine.Vertical = true;
  			CurrentSwatchLine.b = Area.left + cRRWidth/2;
  			break;
  		}
  	} else {
  		switch (NearestCorner) {
  		case 0:
  			SearchHeading = 0;
  			NextSwatchHeading = -pi/2;
  			SwatchStart = Area.TopLeft();
  
  			CurrentSwatchLine.m = 0;
  			CurrentSwatchLine.Vertical = false;
  			CurrentSwatchLine.b = Area.top - cRRWidth/2;
  			break;
  		case 1:
  			SearchHeading = pi;
  			NextSwatchHeading = -pi/2;
  			SwatchStart.x = Area.right;
  			SwatchStart.y = Area.top;
  
  			CurrentSwatchLine.m = 0;
  			CurrentSwatchLine.Vertical = false;
  			CurrentSwatchLine.b = Area.top - cRRWidth/2;
  			break;
  		case 2:
  			SearchHeading = pi;
  			NextSwatchHeading = pi/2;
  			SwatchStart.x = Area.right;
  			SwatchStart.y = Area.bottom;
  
  			CurrentSwatchLine.m = 0;
  			CurrentSwatchLine.Vertical = false;
  			CurrentSwatchLine.b = Area.bottom + cRRWidth/2;
  			break;
  		case 3:
  			SearchHeading = 0;
  			NextSwatchHeading = pi/2;
  			SwatchStart.x = Area.left;
  			SwatchStart.y = Area.bottom;
  
  			CurrentSwatchLine.m = 0;
  			CurrentSwatchLine.Vertical = false;
  			CurrentSwatchLine.b = Area.bottom + cRRWidth/2;
  			break;
  		default:
  			ASSERT(FALSE);
  			SearchHeading = 0;
  			NextSwatchHeading = -pi/2;
  
  			CurrentSwatchLine.m = 0;
  			CurrentSwatchLine.Vertical = false;
  			CurrentSwatchLine.b = Area.top - cRRWidth/2;
  			break;
  		}
  	}
  
  	SwatchStart.x += cRRWidth/2 * cos(NextSwatchHeading) + cRRWidth * cos(SearchHeading);
  	SwatchStart.y += cRRWidth/2 * sin(NextSwatchHeading) + cRRWidth * sin(SearchHeading);
  	bool done = false;
  	int Sign = 1;
  /*Get to starting point*/
  	if (GetOnHomeBase()) {
  		if ( BackupFromWall() != TR_TaskComplete) {
  			goto CLEANUP;
  		}
  	}
  	CCTDPath* SearchPath;
  	SearchPath = new CCTDPath();
  	SearchPath->AddPoint(GetHandlePosition(short(cFollowCTDPathHandleLength)));
  	SearchPath->AddPoint(SwatchStart);
  	AddCTDPathToInstructions(SearchPath, cModPositionZone);
  	if (FollowPathWithObstacleAvoidance(SearchPath->GetSpeed(), GetParent()->m_ProbabilityGrid, false) != TR_TaskComplete) done = true;
  //	if (FollowInstructions(TRUE, TRUE, IR_InstructionComplete) != TR_TaskComplete) done = true;
  /*start search routine*/
  	while (!done) {
  	/*Calculate NextSwatchLine*/
  		TLine NextSwatchLine = CurrentSwatchLine;
  		double NextSwatchDistance = CalculateNextSwatchDistance(Area, CurrentSwatchLine, NextSwatchHeading);
  		if ( (NextSwatchHeading == pi/2) || (NextSwatchHeading == 0) ) {
  			NextSwatchLine.b += NextSwatchDistance;
  		} else {
  			NextSwatchLine.b -= NextSwatchDistance;
  		}
  
  		SearchPath = new CCTDPath();
  		SearchPath->AddPoint(GetHandlePosition(cFollowCTDPathHandleLength));
  		SwatchStart.x += (Area.Width() - 2*cRRWidth) * cos(SearchHeading);
  		SwatchStart.y += (Area.Height() - 2*cRRWidth) * sin(SearchHeading);
  		SearchPath->AddPoint(SwatchStart);
  		robPOINT P1, P2, BeginningNextPath;
  
  		if (NextSwatchDistance < 3*cTPI) {
  			done = true;
  		} else {
  			BeginningNextPath.x = SwatchStart.x + NextSwatchDistance * cos(NextSwatchHeading);
  			BeginningNextPath.y = SwatchStart.y + NextSwatchDistance * sin(NextSwatchHeading);
  			P2.x = (BeginningNextPath.x + SwatchStart.x) / 2 + cRRWidth/2 * cos(SearchHeading);
  			P2.y = (BeginningNextPath.y + SwatchStart.y) / 2 + cRRWidth/2 * sin(SearchHeading);;
  			SearchPath->AddArc(SwatchStart, BeginningNextPath, P2);
  		}
  		if (SearchPath != NULL) {
  			AddCTDPathToInstructions(SearchPath, cMinPositionZone);
  			TInstructionResult InstructionResult;
  			InstructionResult = FollowInstructions(TRUE, TRUE);
  		/*Handle Obstacles*/
  			if (InstructionResult == IR_HitObstacle) {
  				short ObstacleDirection = FindObstacleDirection(SearchHeading, NextSwatchHeading, NextSwatchLine);
  				if (ObstacleDirection == 1) {	//Obstacle to the robot's side
  					SearchAlongWall(Area, SearchHeading - pi, NextSwatchHeading, &done);
  				} else if ( (ObstacleDirection == 2) && !done ) {	//Obstacle in front of the robot
  					robPOINT Dest = GetHandlePosition(-cFollowCTDPathHandleLength);
  					SearchPath = new CCTDPath(cComputerControlSpeed);
  					SearchPath->AddPoint(Dest);
  					Dest.x -= 6*cTPI*cos(SearchHeading);
  					Dest.y -= 6*cTPI*sin(SearchHeading);
  					SearchPath->AddPoint(Dest);
  					AddCTDPathToInstructions(SearchPath, cMinPositionZone, true);
  					if (FollowInstructions(TRUE, FALSE) != IR_InstructionComplete) {
  						done = true;
  					} else {
  						robPOINT P3;
  						P1 = GetHandlePosition(cFollowCTDPathHandleLength);
  						NextSwatchDistance = CalculateNextSwatchDistance(m_Position, NextSwatchLine, NextSwatchHeading);
  						P3.x = P1.x + NextSwatchDistance * cos(NextSwatchHeading) - NextSwatchDistance * cos(SearchHeading);
  						P3.y = P1.y + NextSwatchDistance * sin(NextSwatchHeading) - NextSwatchDistance * sin(SearchHeading);
  
  						P2.x = P1.x - NextSwatchDistance * cos(SearchHeading);
  						P2.y = P1.y - NextSwatchDistance * sin(SearchHeading);
  						if (BringAngleInRange(SearchHeading - NextSwatchHeading) < pi) {
  							P2.x += NextSwatchDistance * cos(NextSwatchHeading + pi/4);
  							P2.y += NextSwatchDistance * sin(NextSwatchHeading + pi/4);
  						} else {
  							P2.x += NextSwatchDistance * cos(NextSwatchHeading - pi/4);
  							P2.y += NextSwatchDistance * sin(NextSwatchHeading - pi/4);
  						}
  						SearchPath = new CCTDPath(cComputerControlSpeed);
  						SearchPath->AddArc(P1, P3, P2);
  						AddCTDPathToInstructions(SearchPath, cMinPositionZone);
  						if (FollowInstructions(TRUE, FALSE) != IR_InstructionComplete) {
  							SearchAlongWall(Area, SearchHeading - pi, NextSwatchHeading, &done);
  						}
  					}
  				} else if (ObstacleDirection == 4) {
  					done = true;
  				}
  			} else if (InstructionResult != IR_InstructionComplete) {
  				done = true;
  			}
  		}
  		SwatchStart = BeginningNextPath;
  		SearchHeading = SearchHeading + Sign*pi;
  		Sign = -Sign;
  		CurrentSwatchLine = NextSwatchLine;
  	};
  
  CLEANUP:
  	SetRobotInfo(OldRobotInfo);
  //	SetEnableObstacleBounce(OldEnableObstacleBounce);
  //	SetObstacleSensitivity(OldSensitivity);
  //	SetHandleLength(OldHandleLength);
  //	SetState(OldState);
  //	SetEStopVisible(OldEStopVisible);
  	return;
  }
  
  short CRobot::FindObstacleDirection(double ForwardDir, double NextSearchDir, TLine NextSwatchLine)
  {
  	const short cObstacleParallelToForwardDir = 1;
  	const short cObstaclePerpendicularToForwardDir = 2;
  	const short cObstacleAvoided = 3;
  	const short cStopExecution = 4;
  	robPOINT Dest = GetHandlePosition();
  	Dest.x -= (cRRWidth/2 - cRRNoseLength + 2*cTPI) * cos(m_Heading);
  	Dest.y -= (cRRWidth/2 - cRRNoseLength + 2*cTPI) * sin(m_Heading);
  	AddPositionVelocityToInstructions(Dest, NULL, cModPositionZone, cComputerControlSpeed, true);
  	if ( BringAngleInRange(ForwardDir - NextSearchDir) < pi) {
  		double phi = ForwardDir - 1.0/18*pi;
  		AddHeadingToInstructions(NextSearchDir, NULL, cMinHeadingZone, cComputerControlSpeed);
  	} else {
  		double phi = ForwardDir + 1.0/18*pi;
  		AddHeadingToInstructions(NextSearchDir, NULL, cMinHeadingZone, cComputerControlSpeed);
  	}
  	TInstructionResult InstructionResult = FollowInstructions();
  	if (InstructionResult == IR_InstructionComplete) {
  		robPOINT Dest;
  		Dest = GetHandlePosition();
  		double NextSwatchDistance = CalculateNextSwatchDistance(m_Position, NextSwatchLine, NextSearchDir);
  		Dest.x += (NextSwatchDistance) * cos(NextSearchDir);
  		Dest.y += (NextSwatchDistance) * sin(NextSearchDir);
  		AddPositionVelocityToInstructions(Dest, NULL, cMinPositionZone, cObstacleDetectionSpeed);
  		InstructionResult = FollowInstructions();
  		if (InstructionResult == IR_HitObstacle) {
  			return cObstacleParallelToForwardDir;
  		} else if (InstructionResult == IR_InstructionComplete) {
  			return cObstacleAvoided;
  		} else {
  			return cStopExecution;
  		}
  	} else {
  		if (InstructionResult == IR_HitObstacle) {
  			return cObstaclePerpendicularToForwardDir;
  		} else {
  			return cStopExecution;
  		}
  	}
  }
  
  const double SearchSwatchDistance = cRRWidth - 2*cTPI;
  
  double CalculateNextSwatchDistance(robRECT Area, TLine CurrentSwatchLine, double NextSwatchHeading)
  {
  	double DistanceToEnd;
  	robPOINT EndPoint;
  	if (NextSwatchHeading == 0) {
  		EndPoint = Area.BottomRight();
  	} else if (NextSwatchHeading == pi/2) {
  		EndPoint = Area.TopLeft();
  	} else if (NextSwatchHeading == pi) {
  		EndPoint = Area.TopLeft();
  	} else if (NextSwatchHeading == -pi/2) {
  		EndPoint = Area.BottomRight();
  	}
  	DistanceToEnd = PerpendicularDistanceFromLine(CurrentSwatchLine, EndPoint);
  	
  	if ( DistanceToEnd <= (SearchSwatchDistance + cRRWidth/2) ) {
  		return DistanceToEnd - cRRWidth/2;
  	} else {
  		return SearchSwatchDistance;
  	}
  }
  
  double CalculateNextSwatchDistance(robPOINT CurrentPosition, TLine NextSwatchLine, double NextSwatchHeading)
  {
  	double Dist = PerpendicularDistanceFromLine(NextSwatchLine, CurrentPosition);
  	robPOINT PerpInt = FindPerpendicularIntersection(NextSwatchLine, CurrentPosition);
  	double ToLineHeading = atan2(PerpInt.y - CurrentPosition.y, PerpInt.x - CurrentPosition.x);
  	if (AbsAngularSeparation(ToLineHeading, NextSwatchHeading) > pi/2) {
  		Dist = -Dist;
  	}
  	return Dist;
  }
  
  double CalculateNextSwatchDistance(robRECT Area, robPOINT SwatchStart, double NextSwatchHeading, double SearchHeading)
  {
  	robPOINT P3;
  	P3.x = SwatchStart.x + 3/2 * cRRWidth * cos(NextSwatchHeading);
  	P3.y = SwatchStart.y + 3/2 * cRRWidth * sin(NextSwatchHeading);
  	if (Area.Inside(P3)) {
  		return cRRWidth;
  	} else {
  		if (NextSwatchHeading == 0) {
  			P3.x = Area.right - cRRWidth/2;
  			P3.y = SwatchStart.y;
  		} else if (NextSwatchHeading == pi/2) {
  			P3.x = SwatchStart.x;
  			P3.y = Area.top - cRRWidth/2;
  		} else if (NextSwatchHeading == pi) {
  			P3.x = Area.left + cRRWidth/2;
  			P3.y = SwatchStart.y;
  		} else if (NextSwatchHeading == -pi/2) {
  			P3.x = SwatchStart.x;
  			P3.y = Area.bottom + cRRWidth/2;
  		}
  		return Distance(SwatchStart, P3);
  	}
  }
  
  void CRobot::AddHeadingToInstructions(double Heading, TCoordSystem *CoordinateSystem, double Zone, short MaxSpeed, TMotorEnable MotorEnable, unsigned long TimeOut)
  /*******************************************************************************
  //Parameters:
  //	Heading:				The Heading to add to the instruction list.
  //	CoordinateSystem		The coordinate system Heading is in.  A NULL value means Heading is
  //								in a non-rotated coordinate system.
  //	Zone:					How close to Heading the robot must be before going to the next node
  //								in the instruction list
  //	MaxSpeed:				The maximum Speed to use when adjusting the Heading.
  //	MotorEnable:		Which motor is enabled while executing.
  //	TimeOut:				The maximum amount of time the robot is allowed to get to the 
  //								final Heading.  Value = 0 indicates an infinite time out.
  //Return Value:
  //	none.
  ********************************************************************************
  //Description:
  //		This function adds a Heading onto the robot's instruction list.  If Heading is in a
  //	rotated coordinate system, first the Heading is transformed to a non-rotated
  //	system.
  //Method:
  //		If CoordinateSystem != NULL then Heading is transformed to the non-displaced
  //	system. A new path list node is created.  Then the members of the new node are
  //	initialized, and finally the new node is added to the instruction list.
  *******************************************************************************/
  {
  	if (CoordinateSystem != NULL) {
  		Heading += CoordinateSystem->Phi;
  	}
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->Heading = Heading;
  	NewNode->InstructionNodeType = inHeadingMotorEnableNode;
  	NewNode->SetTimeOut(TimeOut);
  	if (Zone < cMinHeadingZone) NewNode->Zone = cMinHeadingZone;
  	else NewNode->Zone = Zone;
  	NewNode->MaxSpeed = MaxSpeed;
  	NewNode->sParm0 = (short)MotorEnable;
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::SetHeadingDestination(double Heading, short Speed, short MotorEnable)
  {
  	m_RobotComm->SendHeadingDestination(Heading, Speed, MotorEnable);
  }
  
  short CRobot::SearchAlongWall(robRECT Area, double SearchHeading, double WallHeading, bool *done)
  {
  	TInstructionResult InstructionResult = IR_InstructionComplete;
  	robPOINT Dest;
  	SearchHeading += pi;
  	do {
  		robPOINT OldPos = m_Position;
  		Dest = m_Position;
  		Dest.x -= cRRWidth/2*cos(SearchHeading);
  		Dest.y -= cRRWidth/2 * sin(SearchHeading);
  		if ( Area.Inside(Dest) ) {
  			if (BringAngleInRange(SearchHeading - WallHeading) < pi) {
  				AddHeadingToInstructions(SearchHeading, NULL, cMinHeadingZone, cComputerControlSpeed, meM1, 0);
  			} else {
  				AddHeadingToInstructions(SearchHeading, NULL, cMinHeadingZone, cComputerControlSpeed, meM0, 0);
  			}
  			AddHeadingToInstructions(WallHeading, NULL, cMinHeadingZone);
  			InstructionResult = FollowInstructions();
  			if (InstructionResult == IR_InstructionComplete) {
  				Dest = GetHandlePosition();
  				Dest.x += cRRWidth * cos(WallHeading);
  				Dest.y += cRRWidth * sin(WallHeading);
  				AddPositionVelocityToInstructions(Dest, NULL, cModPositionZone, cObstacleDetectionSpeed);
  				InstructionResult = FollowInstructions();
  				if ( (InstructionResult != IR_HitObstacle) && (InstructionResult != IR_InstructionComplete) ) {
  					*done = true;
  				}
  			/*Make sure we haven't backed into something that prevented us from moving.*/
  				if (Distance(OldPos, m_Position) < cRRWidth / 4) {
  					Dest = GetHandlePosition(-m_HandleLength);
  					Dest.x -= (cRRWidth/2) * cos(WallHeading);
  					Dest.y -= (cRRWidth/2) * sin(WallHeading);
  					AddPositionVelocityToInstructions(Dest, NULL, cModPositionZone, cComputerControlSpeed, true);
  					AddHeadingToInstructions(SearchHeading - pi, NULL, cMinHeadingZone, cComputerControlSpeed);
  					if (FollowInstructions() == IR_InstructionComplete) {
  						Dest = GetHandlePosition();
  						Dest.x -= (cRRWidth + 3*cTPI) * cos(SearchHeading);
  						Dest.y -= (cRRWidth + 3*cTPI) * sin(SearchHeading);
  						AddPositionVelocityToInstructions(Dest, NULL, cModPositionZone);
  						FollowInstructions();
  					}
  					*done = true;
  				}
  			} else {
  				*done = true;
  			}
  		} else {
  			*done = true;
  		}
  	} while ( (InstructionResult == IR_HitObstacle) && (*done != true) );
  	return 0;
  }
  
  bool CRobot::SetEnableObstacleBounce(bool Enable)
  {
  	m_RobotComm->SendEnableObstacleBounce(Enable);
  	bool OldVal = m_EnableObstacleBounce;
  	m_EnableObstacleBounce = Enable;
  	return OldVal;
  }
  
  CInstructionListNode* CRobot::GetInstructionListHead()
  {
  	return m_InstructionList->Head;
  }
  
  TTaskResult CRobot::FollowPathWithObstacleAvoidance(short Speed, CProbabilityGrid* ProbGrid, bool StopAfterExecution)
  {
  	const unsigned short cRobotPathBufferSize = 25;
  	TTaskResult TaskResult = TR_Error;
  	TInstructionResult InstructionResult = FollowInstructions(false, StopAfterExecution);
  	if ( (InstructionResult != IR_InstructionComplete) && (InstructionResult != IR_EmergencyStop) ) {
  		CInstructionListNode* InstructionsHead;
  		InstructionsHead = GetInstructionListHead();
  		TaskResult = TR_Error;
  		if (InstructionsHead != NULL) {
  			if (InstructionsHead->InstructionNodeType == inCTDPathNode) {
  			/*Find where in path you are*/
  				unsigned short PathPosition;
  				if (InstructionsHead->CTDPath->m_PathPosition <= cRobotPathBufferSize) {
  					PathPosition = InstructionsHead->CTDPath->FindClosestPoint(m_Position, 0, InstructionsHead->CTDPath->m_PathPosition);
  				} else {
  					PathPosition = InstructionsHead->CTDPath->FindClosestPoint(m_Position, InstructionsHead->CTDPath->m_PathPosition - cRobotPathBufferSize, cRobotPathBufferSize);
  				}
  //				PathPosition = InstructionsHead->CTDPath->FindClosestPoint(m_Position);
  			/*Go forward into path 3 feet (to get through obstacle)*/
  				unsigned short MoveForward = unsigned short(36*cTPI / InstructionsHead->CTDPath->GetPointSpacing());
  				if (MoveForward >= InstructionsHead->CTDPath->m_Size - PathPosition) PathPosition = InstructionsHead->CTDPath->m_Size - 1;
  				else PathPosition += MoveForward;
  				InstructionsHead->CTDPath->m_PathPosition = PathPosition;
  			/*Save old instruction list*/
  				InstructionListHead  OldInstructionList;
  				OldInstructionList.Head = m_InstructionList->Head;
  				m_InstructionList->Head = NULL;
  				OldInstructionList.Tail = m_InstructionList->Tail;
  				m_InstructionList->Tail = NULL;
  			/*Get around obstacle*/
  				TaskResult = GetAroundObstacle(InstructionsHead->CTDPath->m_Path[PathPosition], Speed, ProbGrid);
  			/*Restore old instruction list*/
  				m_InstructionList->Tail = OldInstructionList.Tail;
  				m_InstructionList->Head = OldInstructionList.Head;
  				if (TaskResult == TR_TaskComplete) TaskResult = FollowPathWithObstacleAvoidance(Speed, ProbGrid, StopAfterExecution);
  			}
  		}
  	} else {
  		if (InstructionResult == IR_InstructionComplete) TaskResult = TR_TaskComplete;
  		else if (InstructionResult == IR_EmergencyStop) TaskResult = TR_EmergencyStop;
  		else TaskResult = TR_Error;
  	}
  	PurgeInstructions();
  	return TaskResult;
  }
  
  TRobotInfo CRobot::GetRobotInfo()
  /*This function saves important robot information that is often changed while
  	running a calibration zone, following a path, etc.  This information often
  	needs to be restored after the robot is finished executing the path or whatever*/
  {
  	TRobotInfo RobotInfo;
  	RobotInfo.EnableRepositionOnUnknownObstacle = GetEnableRepositionOnUnknownObstacle();
  	RobotInfo.FlipHandle = m_FlipHandle;
  	RobotInfo.EnableObstacleBounce = m_EnableObstacleBounce;
  	RobotInfo.EStopVisible = GetEStopVisible();
  	RobotInfo.HandleLength = m_HandleLength;
  	RobotInfo.MaxSpeed = m_MaxSpeed;
  	RobotInfo.ObstacleSensitivity = m_ObstacleSensitivity;
  	RobotInfo.RobotState = m_State;
  	RobotInfo.UpdateProbabilityGrid = m_UpdateProbabilityGrid;
  	RobotInfo.ShowCurrentPath = m_ShowCurrentPath;
  	return RobotInfo;
  }
  
  void CRobot::SetRobotInfo(TRobotInfo RobotInfo)
  /*This function is often used to restore the robot to a previous state.
  	this is often useful for restoring the robot to the state it was in
  	before running a calibration zone or something else like it.*/
  {
  	SetEnableRepositionOnUnknownObstacle(RobotInfo.EnableRepositionOnUnknownObstacle);
  	if (m_FlipHandle != RobotInfo.FlipHandle) {
  		SetFlipHandle(RobotInfo.FlipHandle);
  	}
  	if (m_EnableObstacleBounce != RobotInfo.EnableObstacleBounce) {
  		SetEnableObstacleBounce(RobotInfo.EnableObstacleBounce);
  	}
  	SetEStopVisible(RobotInfo.EStopVisible);
  	if (m_HandleLength != RobotInfo.HandleLength) {
  		SetHandleLength(RobotInfo.HandleLength);
  	}
  //	if (m_MaxSpeed != RobotInfo.MaxSpeed) {
  //		SetMaxSpeed(RobotInfo.MaxSpeed);
  //	}
  	if (m_ObstacleSensitivity != RobotInfo.ObstacleSensitivity) {
  		SetObstacleSensitivity(RobotInfo.ObstacleSensitivity);
  	}
  	SetState(RobotInfo.RobotState);
  	m_UpdateProbabilityGrid = RobotInfo.UpdateProbabilityGrid;
  	m_ShowCurrentPath = RobotInfo.ShowCurrentPath;
  }
  
  void CRobot::SendRAMData(unsigned short Address, unsigned char NumberBytes, unsigned char *Data)
  {
  	m_RobotComm->SendRAMData(Address, NumberBytes, Data);
  }
  
  void CRobot::SetRobotOnSeconds(long On)
  {
  	m_RobotComm->SendRobotOnSeconds(On);
  }
  
  void CRobot::DisplayErrorText(CString Text)
  {
  	m_DlgRobotError->DisplayText(Text);
  }
  
  void CRobot::DisplayErrorText(UINT nID)
  {
  	m_DlgRobotError->DisplayText(nID);
  }
  
  
  void CRobot::SetIntermittentBeep(bool On)
  {
  	m_RobotComm->SendIntermittentBeep(On);
  	m_IntermittentBeepOn = On;
  }
  
  bool CRobot::GetIntermittentBeep()
  {
  	return m_IntermittentBeepOn;
  }
  
  void CRobot::AddToPosition(robPOINT Delta)
  {
  	m_Position.x += Delta.x;
  	m_Position.y += Delta.y;
  	CalculatePositionAndHeadingError(m_Position, m_Heading, TRUE, FALSE);
  	m_RobotComm->AddToPosition(Delta);
  	return;
  }
  
  void CRobot::AddToHeading(double Delta)
  {
  	m_Heading += Delta;
  	CalculatePositionAndHeadingError(m_Position, m_Heading, FALSE, TRUE);
  	m_RobotComm->AddToHeading(Delta);
  	return;
  }
  
  robPOINT CRobot::GetTaskResultPosition()
  {
  	return m_RobotInstructionResult.Position;
  }
  
  double CRobot::GetTaskResultHeading()
  {
  	return m_RobotInstructionResult.Heading;
  }
  
  void CRobot::SetFloorType(TFloorType FloorType)
  {
  	m_FloorType = FloorType;
  	switch (m_FloorType) {
  	case ftWood:
  		SetHeadingCorrectionFactor(((CRobotWorldApp*)AfxGetApp())->GetWoodHeadingCorrectionFactor());
  		m_KHeadErr = cThinCarpetKHeadError;
  		m_KStraightHeadErr = cThinCarpetKStraightHeadError;
  		break;
  	case ftThinCarpet:
  		SetHeadingCorrectionFactor(((CRobotWorldApp*)AfxGetApp())->GetThinCarpetHeadingCorrectionFactor());
  		m_KHeadErr = cThinCarpetKHeadError;
  		m_KStraightHeadErr = cThinCarpetKStraightHeadError;
  		break;
  	case ftThickCarpet:
  		SetHeadingCorrectionFactor(((CRobotWorldApp*)AfxGetApp())->GetThickCarpetHeadingCorrectionFactor());
  		m_KHeadErr = cThickCarpetKHeadError;
  		m_KStraightHeadErr = cThickCarpetKStraightHeadError;
  		break;
  	default:
  		ASSERT(FALSE);
  		break;
  	}
  }
  
  TFloorType CRobot::GetFloorType()
  {
  	return m_FloorType;
  }
  
  void CRobot::SetAttachmentType(TAttachmentType AttachmentType)
  {
  	m_AttachmentType = AttachmentType;
  }
  
  TAttachmentType CRobot::GetAttachmentType()
  {
  	return m_AttachmentType;
  }
  
  void CRobot::SetExternalOutput(bool On)
  {
  	GetMainFrame()->OnExternalOutputChange(On);
  //	if (m_ExternalOutput != On) m_Parent->OnRobotExternalOutputChange(On);
  	m_ExternalOutput = On;
  	m_RobotComm->SetExternalOutput(On);
  }
  
  Path* CRobot::CreatePath(robPOINT Start, robPOINT End)
  /*Creates a new path from the start to the end point*/
  {
  	Path* NewPath = new Path(m_Parent->m_ProbabilityGrid->m_RobotWorld, Start.x, Start.y, End.x, End.y);
  	return NewPath;
  }
  
  void CRobot::FindComPort()
  {
  	m_RobotComm->InitialSearchForRobot();
  }
  
  void CRobot::OnInitialCommunicationEstablished()
  {
  	SetRetryLimit(cInfiniteRetry);
  	EStop();
  	SetMaxSpeed(GetMaxSpeed());
  	SetFloorType(GetFloorType());
  	SetHandleLength(GetHandleLength());
  	SetFlipHandle(GetFlipHandle());
  	SetObstacleSensitivity(GetObstacleSensitivity());
  	SetMotorControlConstants(cInitKp,cInitKi,cInitKd,cInitKb,cInitKpwm);
  	SetEnableDancing(GetEnableDancing());
  	Pause(false);
  	SendRequestState();
  	SetRobotOnSeconds(((CRobotWorldApp*)AfxGetApp())->GetRobotOnSeconds());
  	m_CommunicationsEstablished = true;
  }
  
  void CRobot::SendRequestEX2()
  {
  	m_RobotComm->SendRequestEX2();
  }
  
  float CRobot::SlipPercent(float HeadingCorrectionFactor)
  {
  	float DefaultTicks = TotalTicksPerRotation((float)GetThinCarpetDefaultHeadingCorrectionFactor());
  	return 100 * (TotalTicksPerRotation(HeadingCorrectionFactor) - DefaultTicks) / DefaultTicks;
  }
  
  float CRobot::TotalTicksPerRotation(float HeadingCorrectionFactor)
  {
  	return (HeadingCorrectionFactor * cNHeadings) / (1 + HeadingCorrectionFactor);
  }
  
  float CRobot::HeadingCorrectionFactor(float TotalTicksPerRotation)
  {
  	return (TotalTicksPerRotation / (cNHeadings - TotalTicksPerRotation));
  }
  
  float CRobot::GetThinCarpetSlipPercent()
  {
  	return SlipPercent(((CRobotWorldApp*)AfxGetApp())->GetThinCarpetHeadingCorrectionFactor());
  }
  
  float CRobot::GetThickCarpetSlipPercent()
  {
  	return SlipPercent(((CRobotWorldApp*)AfxGetApp())->GetThickCarpetHeadingCorrectionFactor());
  }
  
  void CRobot::SetThinCarpetSlipPercent(float Slip)
  {
  	SetThinCarpetHeadingCorrectionFactor((short)Round(HeadingCorrectionFactorFromSlipPercent(Slip)));
  }
  
  void CRobot::SetThickCarpetSlipPercent(float Slip)
  {
  	SetThickCarpetHeadingCorrectionFactor((short)Round(HeadingCorrectionFactorFromSlipPercent(Slip)));
  }
  
  float CRobot::HeadingCorrectionFactorFromSlipPercent(float Slip)
  {
  	float DefaultTicks = TotalTicksPerRotation((float)GetThinCarpetDefaultHeadingCorrectionFactor());
  	DefaultTicks = DefaultTicks * (Slip/100 + 1);
  	return (DefaultTicks / (1160 - DefaultTicks));
  }
  
  void CRobot::SetThinCarpetHeadingCorrectionFactor(short HeadingCorrectionFactor)
  {
  	SetFloorType(GetFloorType());
  	((CRobotWorldApp*)AfxGetApp())->SetThinCarpetHeadingCorrectionFactor(HeadingCorrectionFactor);
  }
  
  void CRobot::SetThickCarpetHeadingCorrectionFactor(short HeadingCorrectionFactor)
  {
  	SetFloorType(GetFloorType());
  	((CRobotWorldApp*)AfxGetApp())->SetThickCarpetHeadingCorrectionFactor(HeadingCorrectionFactor);
  }
  
  float CRobot::GetLowBatteryVoltage()
  {
  	return 11.0f;
  }
  
  bool CRobot::GetCommunicationsEstablished()
  {
  	return m_CommunicationsEstablished;
  }
  
  bool CRobot::SetShowCurrentPath(bool Show)
  {
  	bool OldVal = m_ShowCurrentPath;
  	m_ShowCurrentPath = Show;
  	return OldVal;
  }
  
  bool CRobot::GetShowCurrentPath()
  {
  	return m_ShowCurrentPath;
  }
  
  const int cRobotGridSize = 12;
  void RotateRobotGrid(int Before[cRobotGridSize][cRobotGridSize], int After[cRobotGridSize][cRobotGridSize], double phi)
  {
  	const float xorigin = (cRobotGridSize-1) / 2.0f;
  	const float yorigin = (cRobotGridSize-1) / 2.0f;
  	double cosphi = cos(phi);
  	double sinphi = sin(phi);
  /*	for (int iprime = 0; iprime < cRobotGridSize; iprime++) {
  		for (int jprime = 0; jprime < cRobotGridSize; jprime++) {
  			float x, y, xprime, yprime;
  			xprime = iprime - xorigin;
  			yprime = jprime - yorigin;
  			x = xprime * cosphi - yprime * sinphi;
  			y = yprime * cosphi + xprime * sinphi;
  			int i = x + xorigin;
  			int j = y + yorigin;
  			After[i][j] = Before[iprime][jprime];
  		}
  	}
  */
  	for (int i = 0; i < cRobotGridSize; i++) {
  		for (int j = 0; j < cRobotGridSize; j++) {
  			double x, y, xprime, yprime;
  			x = i - xorigin;
  			y = j - yorigin;
  			xprime = x * cosphi + y * sinphi;
  			yprime = y * cosphi - x * sinphi;
  			int iprime = int(xprime + xorigin);
  			int jprime = int(yprime + yorigin);
  			if ( (iprime < 0) || (iprime >= cRobotGridSize) || (jprime < 0) || (jprime >= cRobotGridSize) ) {
  				After[i][j] = 0;
  			} else {
  				After[i][j] = Before[iprime][jprime];
  			}
  		}
  	}
  #ifdef _DEBUG
  //#define PRINT_THIS
  #endif
  #ifdef PRINT_THIS
  	fprintf(stderr, "Before Grid\n");
  	for (int y = cRobotGridSize-1; y >= 0; y--) {
  		for (int x = 0; x < cRobotGridSize; x++) {
  			fprintf(stderr, "%3d, ", Before[x][y]);
  		}
  		fprintf(stderr, "\n");
  	}
  	fprintf(stderr, "After Grid\n");
  	for (y = cRobotGridSize-1; y >= 0; y--) {
  		for (int x = 0; x < cRobotGridSize; x++) {
  			fprintf(stderr, "%3d, ", After[x][y]);
  		}
  		fprintf(stderr, "\n");
  	}
  #endif
  }
  
  bool CRobot::RepositionRobotOnUnexpectedObstacle(CProbabilityGrid *ProbGrid, TMotorObstacle Obstacle)
  /*This function tries to reposition the robot when it hits an unexpected obstacle.  It considers the
  robot's position, its error, and the known obstacles in the probabillity grid when trying to relocate
  the robot.*/
  {
  	int RobotGridM0M1[cRobotGridSize][cRobotGridSize] = {
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0,-1,-1,-1,-1, 0, 0, 0, 0,
  		0, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,
  		0, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,
  		0, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,
  		0, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,
  		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  	};
  
  	int RobotGridM0[cRobotGridSize][cRobotGridSize] = {
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0,-1,-1,-1,-1, 0, 0, 0, 0,
  		0, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,
  		0, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,
  		0, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,
  		0, 1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,
  		0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  	};
  
  	int RobotGridM1[cRobotGridSize][cRobotGridSize] = {
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0,-1,-1,-1,-1, 0, 0, 0, 0,
  		0, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,
  		0, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,
  		0, 0,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,
  		0, 0,-1,-1,-1,-1,-1,-1,-1,-1, 1, 0,
  		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  	};
  
  //	m_PositionError.x = m_PositionError.y = 100.0;
  //	m_Position.x = 1760;
  //	m_Position.y = 360;
  
  /*If total error < 5 inches, don't try to relocate the robot*/
  	if (sqr(m_PositionError.x) + sqr(m_PositionError.y) < 5000) return false;
  
  /*Get the grid representing the robot and bumper rotated to the given heading.*/
  	int RobotGrid[cRobotGridSize][cRobotGridSize];
  	int HeadingCorrectionSign = 0;
  	switch (Obstacle) {
  	case M0Front_M1Front:
  		RotateRobotGrid(RobotGridM0M1, RobotGrid, m_Heading);
  		break;
  	case M0Front:
  	case M0Front_M1Back:
  		HeadingCorrectionSign = -1;
  		RotateRobotGrid(RobotGridM0, RobotGrid, m_Heading);
  		break;
  	case M1Front:
  	case M0Back_M1Front:
  		HeadingCorrectionSign = +1;
  		RotateRobotGrid(RobotGridM1, RobotGrid, m_Heading);
  		break;
  	default:
  		return false;
  		break;
  	}
  
  /*Get the probability grid values around the robot*/
  	int WorldGridWidth = int(2 * m_PositionError.x / cWorldGridPointSize + cRobotGridSize);
  	int WorldGridHeight = int(2 * m_PositionError.y / cWorldGridPointSize + cRobotGridSize);
  
  	int **ProbGridValues = new int *[WorldGridWidth];
  	float **Probs = new float *[WorldGridWidth];
  	int i,j;
  
  	for (i = 0; i < WorldGridWidth; i++) {
  		ProbGridValues[i] = new int[WorldGridHeight];
  		Probs[i] = new float[WorldGridHeight];
  	}
  /*Transform probability grid values into obstacle probabilities*/
  	ProbGrid->GetGridValues(m_Position, WorldGridWidth, WorldGridHeight, ProbGridValues);	
  #ifdef PRINT_THIS
  /*Print out obstacle probabilities*/
  	fprintf(stderr, "ProbabilityGrid Values\n");
  	for (int y = WorldGridHeight- 1; y >= 0; y--) {
  		for (int x = 0; x < WorldGridWidth; x++) {
  			fprintf(stderr, "%3d, ", ProbGridValues[x][y]);
  		}
  		fprintf(stderr, "\n");
  	}
  #endif
  	for (i = 0; i < WorldGridWidth; i++) {
  		for (j = 0; j < WorldGridHeight; j++) {
  			Probs[i][j] = ((float)ProbGridValues[i][j] - cMaxProb) / (cMinProb - cMaxProb);
  /*			if (ProbGridValues[i][j]  <= (cMinProb + cMaxProb) / 4) {
  				Probs[i][j] = 1.0f;
  			} else {
  				Probs[i][j] = 0.0f;
  			}
  */
  		}
  	}
  #ifdef PRINT_THIS
  /*Print out obstacle probabilities*/
  	fprintf(stderr, "Obstacle Probabilities\n");
  	for (y = WorldGridHeight- 1; y >= 0; y--) {
  		for (int x = 0; x < WorldGridWidth; x++) {
  			fprintf(stderr, "%3.1f, ", Probs[x][y]);
  		}
  		fprintf(stderr, "\n");
  	}
  #endif
  	robPOINT WorldGridOrigin, RobotGridOrigin;
  	WorldGridOrigin.x = (WorldGridWidth - 1) / 2.0;
  	WorldGridOrigin.y = (WorldGridHeight - 1) / 2.0;
  	RobotGridOrigin.x = (cRobotGridSize - 1) / 2.0;
  	RobotGridOrigin.y = (cRobotGridSize - 1) / 2.0;
  	for (i = 0; i < WorldGridWidth - cRobotGridSize; i++) {
  		for (j = 0; j < WorldGridHeight - cRobotGridSize; j++) {
  		/*See if this is a good spot for the robot*/
  			float RobotPositionProbability;  /*Probability that the robot is displaced this far from the origin*/
  			RobotPositionProbability = float( exp( (sqr((i + RobotGridOrigin.x - WorldGridOrigin.x)*cWorldGridPointSize/m_PositionError.x) +
  				sqr((j + RobotGridOrigin.y - WorldGridOrigin.y)*cWorldGridPointSize/m_PositionError.x)) / -2 ) );
  			RobotPositionProbability /= float(2*pi*m_PositionError.x*m_PositionError.y);
  //			RobotPositionProbability *= cWorldGridPointSize;	//Approximate probability the robot is inside this grid square
  			bool BumperHit = false;
  			bool BodyHit = false;
  			float ProbabilityRobotHere = RobotPositionProbability;
  			for (int Roboti = 0; (Roboti < cRobotGridSize) && !BodyHit; Roboti++) {
  				for (int Robotj = 0; (Robotj < cRobotGridSize) && !BodyHit; Robotj++) {
  /*					if ( (RobotGrid[Roboti][Robotj] == -1) ) {
  						ProbabilityRobotHere *= 1 - Probs[i+Roboti][j+Robotj];
  					} else if ( (RobotGrid[Roboti][Robotj] == 1) ) {
  						ProbabilityRobotHere *= Probs[i+Roboti][j+Robotj];
  					}
  */
  					if ( (RobotGrid[Roboti][Robotj] == -1) && (Probs[i+Roboti][j+Robotj] >= 0.50f) ) {
  						BodyHit = true;
  					} else if ( (RobotGrid[Roboti][Robotj] == 1) && (Probs[i+Roboti][j+Robotj] >= 0.75f) ) {
  						BumperHit = true;
  					}
  				}
  			}
  			if (BumperHit && !BodyHit) {
  				Probs[i][j] = RobotPositionProbability;
  			} else {
  				Probs[i][j] = 0.0f;
  			}
  		}
  	}
  #ifdef PRINT_THIS
  /*Print out final Robot position probabilities*/
  	fprintf(stderr, "Robot Position Probabilities\n");
  	for (y = WorldGridHeight - cRobotGridSize - 1; y >= 0; y--) {
  		for (int x = 0; x < WorldGridWidth - cRobotGridSize; x++) {
  			fprintf(stderr, "%8.6f, ", Probs[x][y]);
  		}
  		fprintf(stderr, "\n");
  	}
  #endif
  /*Find the highest probability position*/
  	float MaxProb = Probs[0][0];
  	int iMaxProb = 0;
  	int jMaxProb = 0;
  	for (i = 0; i < WorldGridWidth - cRobotGridSize; i++) {
  		for (j = 0; j < WorldGridHeight - cRobotGridSize; j++) {
  			if (Probs[i][j] >= MaxProb) {
  				MaxProb = Probs[i][j];
  				iMaxProb = i;
  				jMaxProb = j;
  			}
  		}
  	}
  /*Find the robot displacement to get to the highest, non zero probability position*/
  	double dx = 0;
  	double dy = 0;
  	bool RepositionedRobot = false;
  	if (Probs[iMaxProb][jMaxProb] != 0) {
  		dx = (RobotGridOrigin.x + iMaxProb - WorldGridOrigin.x) * cWorldGridPointSize;
  		dy = (RobotGridOrigin.y + jMaxProb - WorldGridOrigin.y) * cWorldGridPointSize;
  		if ( (dx != 0) || (dy != 0) ) {
  			m_Position.x += dx;
  			m_Position.y += dy;
  			m_RobotComm->SendPosition(m_Position);		
  			RepositionedRobot = true;
  		/*Heading correction is X degrees towards the bumper that hit plus a constant times the amount of
  		position shift perpendicular to the robot's heading*/
  			double dHeading = HeadingCorrectionSign * (0.0175) + (cos(m_Heading)*dy - sin(m_Heading)*dx)*0.0002;
  			if (dHeading != 0) {
  				m_RobotComm->AddToHeading(dHeading);
  			}
  			CString Output;
  			Output.Format("Repositioned Robot on Obstacle: dx = %4.1f"" dy = %4.1f"" dHeading = %5.2f degrees", dx / 10, dy / 10, dHeading / pi * 180);
  			SetStatusBarText(Output);
  		}
  	}
  /*Delete the ProbGridValues and Probs arrays*/
  	for (i = 0; i < WorldGridWidth; i++) {
  		delete ProbGridValues[i];
  		delete Probs[i];
  	}
  	delete ProbGridValues;
  	delete Probs;
  	return RepositionedRobot;
  }
  
  float CRobot::ProbabilityOfRobotBeingHere(robPOINT Point)
  {
  	if ( (m_PositionError.x == 0) || (m_PositionError.y == 0) ) {
  		if (Point == m_Position) return 1.0f;
  		else return 0.0f;
  	}
  	float temp;
  	temp = (float)sqr((Point.x - m_Position.x) / m_PositionError.x);
  	temp += (float)sqr((Point.y - m_Position.y) / m_PositionError.y);
  	temp /= -2;
  	temp = (float)exp(temp);
  	temp /= float(m_PositionError.x * m_PositionError.y * 2 * pi);
  	return temp;
  }
  
  int CRobot::GetThinCarpetDefaultHeadingCorrectionFactor()
  {
  	return ((CRobotWorldApp*)AfxGetApp())->GetThinCarpetDefaultHeadingCorrectionFactor();
  }
  
  int CRobot::GetThickCarpetDefaultHeadingCorrectionFactor()
  {
  	return ((CRobotWorldApp*)AfxGetApp())->GetThickCarpetDefaultHeadingCorrectionFactor();
  }
  
  void CRobot::RequestExternalInput()
  {
  	m_RobotComm->SendRequestEX2();
  }
  
  void CRobot::OnExternalInput(float Voltage)
  {
  	const float cTransitionVoltage = 2.5f;
  	TInstructionNodeType InstructionNodeType;
  	if (m_InstructionList->Head != NULL) {
  		InstructionNodeType = m_InstructionList->Head->InstructionNodeType;
  	} else {
  		InstructionNodeType =  inNoInstruction;
  	}
  
  	switch (InstructionNodeType) {
  	case inWaitForInputChange:
  		RequestExternalInput();
  		if (m_StartWaitingForExternalInputChange) {
  			m_StartWaitingForExternalInputChange = false;
  		} else {
  			if ( ((Voltage > cTransitionVoltage) && (m_ExternalInputVoltage <= cTransitionVoltage)) ||
  			((Voltage < cTransitionVoltage) && (m_ExternalInputVoltage >= cTransitionVoltage)) ) {
  				m_RobotInstructionResult.Valid = true;
  				m_RobotInstructionResult.Result = IR_InstructionComplete;
  			}
  		}
  		break;
  	case inWaitForInputHigh:
  		RequestExternalInput();
  		if (Voltage > cTransitionVoltage) {
  			m_RobotInstructionResult.Valid = true;
  			m_RobotInstructionResult.Result = IR_InstructionComplete;
  		}
  		break;
  	case inWaitForInputLow:
  		if (Voltage < cTransitionVoltage) {
  			m_RobotInstructionResult.Valid = true;
  			m_RobotInstructionResult.Result = IR_InstructionComplete;
  		}
  		RequestExternalInput();
  		break;
  	case inWaitForInputGreater:
  		if (Voltage > m_InstructionList->Head->Parm0) {
  			m_RobotInstructionResult.Valid = true;
  			m_RobotInstructionResult.Result = IR_InstructionComplete;
  		}
  		RequestExternalInput();
  		break;
  	case inWaitForInputLess:
  		if (Voltage < m_InstructionList->Head->Parm0) {
  			m_RobotInstructionResult.Valid = true;
  			m_RobotInstructionResult.Result = IR_InstructionComplete;
  		}
  		RequestExternalInput();
  		break;
  	}
  	m_ExternalInputVoltage = Voltage;
  	if (m_Parent->GetActiveRobot() == this) {
  		m_Parent->SetRobotExternalInput(Voltage);
  	}
  }
  
  void CRobot::SendTune(short *Frequency, int *msDuration, unsigned char NumberOfNotes, bool PurgeBuffer, bool StartPlaying)
  {
  	m_RobotComm->SendTune(Frequency, msDuration, NumberOfNotes, PurgeBuffer, StartPlaying);
  }
  
  void CRobot::OnSendNextTuneNotes()
  {
  	if (m_InstructionList->Head != NULL) {
  		if (m_InstructionList->Head->InstructionNodeType == inTune) {
  			((CTuneInstruction*)(m_InstructionList->Head))->SendNextNotes(this);
  		}
  	}
  }
  
  void CRobot::SetSoftwareRevision(float Revision)
  {
  	((CRobotWorldApp*)AfxGetApp())->SetRobotSoftwareRevision(Revision);
  	m_SoftwareRevision = Revision;
  }
  
  float CRobot::GetSoftwareRevision()
  {
  	float Revision = 1.0f;
  	if (m_SoftwareRevision == 0.0f) {
  		((CRobotWorldApp*)AfxGetApp())->GetRobotSoftwareRevision();
  	} else {
  		Revision = m_SoftwareRevision;
  	}
  	return Revision;
  }
  
  void CRobot::AddWaitForPingToInstructions(unsigned long TimeOut /*In ms*/)
  {
  	CInstructionListNode* NewNode = new CInstructionListNode;
  	NewNode->Next = NULL;
  	NewNode->InstructionNodeType = inWaitForPing;
  	NewNode->SetTimeOut(TimeOut);
  	AddNodeToInstructions(NewNode);
  }
  
  void CRobot::SendPing()
  {
  	m_RobotComm->SendPing();
  }
  
  bool CRobot::GetTouchedHomeBase(void)
  {
  #ifdef _DEBUG
  	if (m_BatteryChargeState != 0) {
  		ASSERT(m_TouchedHomeBase);
  	}
  #endif
  	return m_TouchedHomeBase;
  }
  
  void CRobot::SetBatteryChargeState(short State)
  {
  	m_BatteryChargeState = State;
  }
  
  void CRobot::SetStatusBarText(CString Text)
  {
  	m_Parent->SetStatusBarText(Text);
  }
  
  void CRobot::SetPositionError(robPOINT Error)
  {
  	m_Parent->RedrawRobot(m_Position, m_Heading);
  	m_PositionError = Error; /*To reset the position error, call CalculatePositionAndHeadingError with the ResetPosition set to TRUE*/
  };
  
  int CRobot::GetHeadingCorrectionFactor(TFloorType FloorType)
  {
  	int HeadingCorrectionFactor;
  	switch (FloorType) {
  	case ftWood:
  		HeadingCorrectionFactor = ((CRobotWorldApp*)AfxGetApp())->GetWoodHeadingCorrectionFactor();
  		break;
  	case ftThinCarpet:
  		HeadingCorrectionFactor = ((CRobotWorldApp*)AfxGetApp())->GetThinCarpetHeadingCorrectionFactor();
  		break;
  	case ftThickCarpet:
  		HeadingCorrectionFactor = ((CRobotWorldApp*)AfxGetApp())->GetThickCarpetHeadingCorrectionFactor();
  		break;
  	default:
  		ASSERT(FALSE);
  		HeadingCorrectionFactor = ((CRobotWorldApp*)AfxGetApp())->GetThinCarpetHeadingCorrectionFactor();
  		break;
  	}
  	return HeadingCorrectionFactor;
  }
  
  void CRobot::SetHeadingError(double Error)
  /*To reset the heading error, call CalculatePositionAndHeadingError with the ResetHeading set to TRUE*/
  {
  	if (Error <= cMinHeadingError) {
  		m_HeadingError = cMinHeadingError;
  	} else {
  		m_HeadingError = Error;
  	}
  }
  
  bool CRobot::GetAlreadyCalibratedFloorType(TFloorType FloorType)
  {
  	bool Done = false;
  	switch (FloorType) {
  	case ftWood:
  		Done = true;
  		break;
  	case ftThinCarpet:
  		Done = ((CRobotWorldApp*)AfxGetApp())->GetCalibratedThinCarpet();
  		break;
  	case ftThickCarpet:
  		Done = ((CRobotWorldApp*)AfxGetApp())->GetCalibratedThickCarpet();
  		break;
  	default:
  		ASSERT(FALSE);
  		break;
  	}
  	return Done;
  }
  
  void CRobot::SetAlreadyCalibratedFloorType(TFloorType FloorType, bool Value)
  {
  	switch (FloorType) {
  	case ftWood:
  		break;
  	case ftThinCarpet:
  		((CRobotWorldApp*)AfxGetApp())->SetCalibratedThinCarpet(Value);
  		break;
  	case ftThickCarpet:
  		((CRobotWorldApp*)AfxGetApp())->SetCalibratedThickCarpet(Value);
  		break;
  	default:
  		ASSERT(FALSE);
  		break;
  	}
  }
  
  void CRobot::SetHeadingCorrectionFactor(TFloorType FloorType, int HCF)
  {
  	switch (FloorType) {
  	case ftWood:
  		ASSERT(FALSE);
  //		SetWoodHeadingCorrectionFactor(HCF);
  		break;
  	case ftThinCarpet:
  		SetThinCarpetHeadingCorrectionFactor(HCF);
  		break;
  	case ftThickCarpet:
  		SetThickCarpetHeadingCorrectionFactor(HCF);
  		break;
  	default:
  		ASSERT(FALSE);
  		break;
  	}
  }
  
  void CRobot::SetMicrophoneVolume(int Volume)
  {
  	m_MicrophoneVolume = Volume;
  }
  
  int CRobot::GetMicrophoneVolume()
  {
  	return m_MicrophoneVolume;
  }
  
  void CRobot::SetAddress(short Address)
  {
  	if (m_Address != Address) {
  		m_RobotComm->SetRobotAddress(Address);
  	}
  	m_Address = Address;
  }
  
  short CRobot::GetAddress()
  {
  	return m_Address;
  }
  
  void CRobot::SetEnableDancing(bool Enable)
  {
  	m_RobotComm->SetEnableDancing(Enable);
  	((CRobotWorldApp*)AfxGetApp())->SetEnableDancing(Enable);
  }
  
  
  bool CRobot::GetEnableDancing()
  {
  	return ((CRobotWorldApp*)AfxGetApp())->GetEnableDancing();
  }
  
  void CRobot::SetEnableClapDetection(bool Enable)
  {
  	m_RobotComm->SetEnableClapDetection(Enable);
  }
  
  TInstructionResult CRobot::FollowGoalSet(CGoalSet *GoalSet, short Speed)
  {
  	TInstructionResult InstructionResult = IR_InstructionComplete;
  	if (GoalSet != NULL) {
  		if ((*GoalSet)[0].thePath == NULL) InstructionResult = IR_InstructionIncomplete;
  		else {
  			for (int i = 0; ((*GoalSet)[i].thePath != NULL) && (InstructionResult == IR_InstructionComplete); i++) {
  				AddPathToInstructions((*GoalSet)[i].thePath, Speed, cMinPositionZone);
  				InstructionResult = FollowInstructions(true, false);
  				if (InstructionResult == IR_InstructionComplete) {
  					if ((*GoalSet)[i].calZone != NULL) {
  						TTaskResult TaskResult = (*GoalSet)[i].calZone->SpecialFunction(this, Speed);
  						switch (TaskResult) {
  						case TR_TaskComplete:
  							InstructionResult = IR_InstructionComplete;
  							break;
  						case TR_EmergencyStop:
  							InstructionResult = IR_EmergencyStop;
  							break;
  						default:
  							InstructionResult = IR_InstructionIncomplete;
  							break;
  						}
  					}
  				}
  			}
  		}
  	} else {
  		InstructionResult = IR_InstructionIncomplete;
  	}
  	return InstructionResult;
  }
  
  TTaskResult CRobot::FollowGoalSetWithObstacleAvoidance(CGoalSet *GoalSet, short Speed, CProbabilityGrid *ProbGrid)
  {
  	TTaskResult TaskResult = TR_Error;
  	if (GoalSet != NULL) {
  		if ((*GoalSet)[0].thePath == NULL) TaskResult = TR_Error;
  		else {
  			TaskResult = TR_TaskComplete;
  			for (int i = 0; ((*GoalSet)[i].thePath != NULL) && (TaskResult == TR_TaskComplete); i++) {
  				AddPathToInstructions((*GoalSet)[i].thePath, Speed, cMinPositionZone);
  //				TaskResult = FollowPathWithObstacleAvoidance(Speed, ProbGrid, false);
  				TInstructionResult InstructionResult;
  				if ( (InstructionResult = FollowInstructions(TRUE, FALSE)) == IR_InstructionComplete ) {
  //				if (TaskResult == TR_TaskComplete) {
  					if ((*GoalSet)[i].calZone != NULL) {
  						TaskResult = (*GoalSet)[i].calZone->SpecialFunction(this, Speed);
  					}
  				} else {
  //					double x,y;
  //					x = (*GoalSet)[i].thePath->GetPoint(0, x, y);
  //					m_FindPathThread->RemovePathFromCache(x, y);
  //					x = (*GoalSet)[i].thePath->GetPoint((*GoalSet)[i].thePath->GetNumberOfPoints()-1, x, y);
  //					m_FindPathThread->RemovePathFromCache(x, y);
  					if (InstructionResult == IR_EmergencyStop) TaskResult = TR_EmergencyStop;
  					else TaskResult = TR_Error;
  				/*Try to go around obstacles*/
  					if (InstructionResult == IR_HitObstacle) {
  						robPOINT Destination;
  						(*GoalSet)[i].thePath->GetPoint((*GoalSet)[i].thePath->GetNumberOfPoints()-1, Destination.x, Destination.y);
  						if ( ((*GoalSet)[i].Cached) ) {
  							m_FindPathThread->RemovePathFromCache((*GoalSet)[i].thePath);
  						}
  						TaskResult = GetAroundObstacle(Destination, (unsigned short)Speed, ProbGrid);
  						if (TaskResult == TR_TaskComplete) {
  							if ((*GoalSet)[i].calZone != NULL) {
  								TaskResult = (*GoalSet)[i].calZone->SpecialFunction(this, Speed);
  							}
  						}
  					} else {
  						if ( ((*GoalSet)[i].Cached) ) {
  							m_FindPathThread->RemovePathFromCache((*GoalSet)[i].thePath);
  						}
  					}
  				}
  				if ( !((*GoalSet)[i].Cached) ) {
  //					m_FindPathThread->RemovePathFromCache((*GoalSet)[i].thePath);
  					delete ((*GoalSet)[i].thePath);
  				}
  			}
  		}
  	} else {
  		TaskResult = TR_Error;
  	}
  	return TaskResult;
  }
  
  TTaskResult CRobot::GetAroundObstacle(robPOINT GetTo, short Speed, CProbabilityGrid* ProbGrid)
  {
  /*Erect temporary obstacle*/
  	const int cNumGridSpacesInTempObstacle = 3;
  	int TempObstacleOldValues[cNumGridSpacesInTempObstacle];
  	robPOINT TempObstaclePoints[cNumGridSpacesInTempObstacle];
  	TempObstaclePoints[0] = m_MotorObstacle.Position;
  	double dx, dy;
  	double phi = m_MotorObstacle.Heading;
  	bool ObstacleInFront;
  	switch (m_MotorObstacle.MotorObstacle) {
  	case M0Front:
  	case M0Front_M1Back:
  		ObstacleInFront = true;
  		TempObstaclePoints[0].x += cRRNoseLength * cos(phi) + cRRWidth/2 * cos(phi - pi/2);
  		TempObstaclePoints[0].y += cRRNoseLength * sin(phi) + cRRWidth/2 * sin(phi - pi/2);
  		dx = cWorldGridPointSize * cos(phi + pi/2);
  		dy = cWorldGridPointSize * sin(phi + pi/2);
  		break;
  	case M1Front:
  	case M0Back_M1Front:
  		ObstacleInFront = true;
  		TempObstaclePoints[0].x += cRRNoseLength * cos(phi) + cRRWidth/2 * cos(phi + pi/2);
  		TempObstaclePoints[0].y += cRRNoseLength * sin(phi) + cRRWidth/2 * sin(phi + pi/2);
  		dx = cWorldGridPointSize * cos(phi - pi/2);
  		dy = cWorldGridPointSize * sin(phi - pi/2);
  		break;
  	case M0Front_M1Front:
  		ObstacleInFront = true;
  		TempObstaclePoints[0].x += cRRNoseLength * cos(phi) + cWorldGridPointSize * cos(phi + pi/2);
  		TempObstaclePoints[0].y += cRRNoseLength * sin(phi) + cWorldGridPointSize * sin(phi + pi/2);
  		dx = cWorldGridPointSize * cos(phi - pi/2);
  		dy = cWorldGridPointSize * sin(phi - pi/2);
  		break;
  	case M0Back:
  	case M1Back:
  	case M0Back_M1Back:
  	default:
  		ObstacleInFront = false;
  		TempObstaclePoints[0].x += cRRNoseLength * cos(phi + pi) + cWorldGridPointSize * cos(phi + pi/2);
  		TempObstaclePoints[0].y += cRRNoseLength * sin(phi + pi) + cWorldGridPointSize * sin(phi + pi/2);
  		dx = cWorldGridPointSize * cos(phi - pi/2);
  		dy = cWorldGridPointSize * sin(phi - pi/2);
  		break;
  	}
  
  	for (int i = 0; i < cNumGridSpacesInTempObstacle; i++) {
  		if (i != 0) {
  			TempObstaclePoints[i].x = TempObstaclePoints[i-1].x + dx;
  			TempObstaclePoints[i].y = TempObstaclePoints[i-1].y + dy;
  		}
  		TempObstacleOldValues[i] = ProbGrid->GetGridValue(TempObstaclePoints[i]);
  		ProbGrid->SetGridValue(TempObstaclePoints[i], 0, 1, true);
  	}
  
  /*Back away from obstacle*/
  	TTaskResult TaskResult = TR_Error;
  	TaskResult = BackupFromWall(ObstacleInFront, Speed);
  	if (TaskResult == TR_TaskComplete) {
  	/*Find path around obstacle*/
  		CGoalSet* AroundObstacle;
  //		double OldPathBuffer = SetFindPathBuffer(8*cTPI);
  		AroundObstacle = FindPathNow(GetHandlePosition(cFollowCTDPathHandleLength), GetTo, cFindPathIterationsDefault, cFindPathRepeatDefault, true); 
  //		SetFindPathBuffer(OldPathBuffer);
  		if (AroundObstacle != NULL) {
  //			AddPathToInstructions(AroundObstacle, Speed, cModPositionZone, false, 0);
  			TaskResult = FollowGoalSetWithObstacleAvoidance(AroundObstacle, Speed, ProbGrid); 
  //			InstructionResult = FollowInstructions(TRUE, TRUE);
  //			if (InstructionResult != IR_InstructionComplete) {
  //				if (InstructionResult  == IR_EmergencyStop) {
  //					TaskResult = TR_EmergencyStop;
  ////					goto CLEANUP;
  //				} else {
  //					TaskResult = GetAroundObstacle(GetTo, Speed, ProbGrid);
  //				}
  //			} else {
  //				TaskResult = TR_TaskComplete;
  //			}
  			delete AroundObstacle;
  		} else {
  			TaskResult = TR_Error;
  		}
  	}
  //CLEANUP:
  /*Remove temporary obstacle*/
  	for (i = cNumGridSpacesInTempObstacle-1; i >= 0; i--) {
  		ProbGrid->SetGridValue(TempObstaclePoints[i], TempObstacleOldValues[i], 1, true);
  	}
  	return TaskResult;
  }
  
  void CRobot::SetEnableClapDetectionWhileMoving(bool Enable)
  {
  	m_RobotComm->SetEnableClapDetectionWhileMoving(Enable);
  }
  
  void CRobot::SetCommResponseTimeOut(long TimeOut)
  {
  	m_RobotComm->SetResponseTimeout(TimeOut);
  }
  
  
  void CRobot::OnObstacleDetected(short RightWheel, short LeftWheel)
  {
  	short M0V, M1V;
  	GetVelocities(M0V, M1V);
  	if ( (abs(M0V) >= 200) || (abs(M1V) >= 200) ) {
  		CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  		SoundPath += "/sounds/RobCollision.wav";
  		PlaySound (SoundPath, NULL, SND_FILENAME | SND_ASYNC ); 
  	}
  	if (GetState() == stDragging) {
  		SetDrag(FALSE, m_Parent);
  	}
  /*Add to heading error on collision*/
  	if (RightWheel || LeftWheel) {
  		double HeadingError = GetHeadingError();
  		HeadingError += 0.00085;
  		SetHeadingError(HeadingError);
  	}
  	RoverGrid ObstacleGrid = M0_F;
  	bool MarkObstacle = false;
  	if ( (RightWheel < 0) && (LeftWheel == 0) ) {
  		MarkObstacle = false;
  		m_MotorObstacle.MotorObstacle = M0Back;
  	} else if ( (RightWheel > 0) && (LeftWheel == 0) ) {
  		m_MotorObstacle.MotorObstacle = M0Front;
  		if (GetLearnRoom()) {
  			MarkObstacle = true;
  			ObstacleGrid = M0_F;
  		}
  	} else if ( (RightWheel == 0) && (LeftWheel < 0) ) {
  		MarkObstacle = false;
  		m_MotorObstacle.MotorObstacle = M1Back;
  	} else if ( (RightWheel == 0) && (LeftWheel > 0) ) {
  		m_MotorObstacle.MotorObstacle = M1Front;
  		if (GetLearnRoom()) {
  			MarkObstacle = true;
  			ObstacleGrid = M1_F;
  		}
  	} else if ( (RightWheel < 0) && (LeftWheel < 0) ) {
  		m_MotorObstacle.MotorObstacle = M0Back_M1Back;
  		if (GetLearnRoom()) {
  			MarkObstacle = true;
  			ObstacleGrid = M2_B;
  		}
  	} else if ( (RightWheel > 0) && (LeftWheel < 0) ) {
  		MarkObstacle = false;
  		m_MotorObstacle.MotorObstacle = M0Front_M1Back;
  	} else if ( (RightWheel < 0) && (LeftWheel > 0) ) {
  		MarkObstacle = false;
  		m_MotorObstacle.MotorObstacle = M0Back_M1Front;
  	} else if ( (RightWheel > 0) && (LeftWheel > 0) ) {
  		m_MotorObstacle.MotorObstacle = M0Front_M1Front;
  		if (GetLearnRoom()) {
  			MarkObstacle = true;
  			ObstacleGrid = M2_F;
  		}
  	} else {
  //		fprintf(stderr, "Unknown Obstacle\n");
  		ASSERT(FALSE);
  		MarkObstacle = false;
  	}
  /*Reposition on an unexpected obstacle*/
  	if (GetEnableRepositionOnUnknownObstacle()) {
  		if (RepositionRobotOnUnexpectedObstacle(m_ProbabilityGrid, m_MotorObstacle.MotorObstacle)) {
  		/*Don't mark the obstacle when the robot is relocated*/
  			MarkObstacle = false;
  		}
  	}
  
  	m_MotorObstacle.ValidObstacle = TRUE;
  	m_MotorObstacle.Heading = m_Heading;
  	m_MotorObstacle.Position = m_Position;
  
  	if (MarkObstacle) {
  		m_ProbabilityGrid->m_RobotWorld->ObstacleRoverPosition(m_Position.x,
  			m_Position.y, m_Heading,
  			m_PositionError.x, m_PositionError.y,
  			m_HeadingError, ObstacleGrid);
  	}
  }
  
  void CRobot::OnNudgeDetected(bool RightWheel, bool LeftWheel) 
  {
  	if (RightWheel) {
  		m_Nudged = NDG_M0Nudged;
  	}
  	if (LeftWheel) {
  		m_Nudged = NDG_M1Nudged;
  	}
  }
  
  void CRobot::OnNewPathSegmentRequest() 
  {
  	const unsigned char cNewPathSegmentLength = 5;
  	if (m_InstructionList->Head != NULL) {
  		if (m_InstructionList->Head->InstructionNodeType == inCTDPathNode) {
  			CCTDPath* CurrentPath = m_InstructionList->Head->CTDPath;
  			if (CurrentPath != NULL) {
  				if (CurrentPath->m_PathPosition < CurrentPath->m_Size) {
  					if (CurrentPath->m_PathPosition + ((long)cNewPathSegmentLength) < CurrentPath->m_Size) {
  						m_RobotComm->SendPathSegment(false, CurrentPath->m_Path + CurrentPath->m_PathPosition,
  							cNewPathSegmentLength);
  						CurrentPath->m_PathPosition += cNewPathSegmentLength;
  					} else {
  						m_RobotComm->SendPathSegment(false, CurrentPath->m_Path + CurrentPath->m_PathPosition,
  							CurrentPath->m_Size - CurrentPath->m_PathPosition);
  					/*Send Final Position just to make sure the robot gets there.  I've been having trouble with
  					the robot stalling at the end of the paths.*/
  						m_RobotComm->SendPathSegment(false, CurrentPath->m_Path + (CurrentPath->m_Size - 1), 1);
  						CurrentPath->m_PathPosition = CurrentPath->m_Size;
  					}
  				}
  			}
  		}
  	}
  }
  
  void CRobot::OnError(long Error) 
  {
  #ifdef _DEBUG
  	ASSERT(FALSE);
  #endif
  	fprintf(stderr, "ERROR!!! Robot error: %ld\n", Error);
  }
  
  void CRobot::OnOnHomeBase(short OnHomeBase) 
  {
  	if (OnHomeBase) {
  		SetOnHomeBase(true);
  		CHomeBase* HomeBase = m_ProbabilityGrid->GetClosestUnoccupiedHomeBase(GetPosition());
  		if ( (HomeBase != NULL) && (!m_ProbabilityGrid->GetAddingHomeBase()) ) {
  			bool PositionWrong = false;
  			bool RobotErrorLarge = false;
  			robPOINT RobotHomedPosition;
  			RobotHomedPosition.x = (HomeBase->m_P0.x + HomeBase->m_P1.x) / 2 - cRRNoseLength * cos(HomeBase->GetHeading());
  			RobotHomedPosition.y = (HomeBase->m_P0.y + HomeBase->m_P1.y) / 2 - cRRNoseLength * sin(HomeBase->GetHeading());
  			robPOINT RobotError = GetPositionError();
  			PositionWrong = (Distance(RobotHomedPosition, GetPosition()) > 20.0) || (AbsAngularSeparation(HomeBase->GetHeading(), GetHeading()) > 0.0175);
  			RobotErrorLarge = (sqrt(sqr(RobotError.x) + sqr(RobotError.y)) > 30.0) || (GetHeadingError() > 0.0524);
  			if ( (GetState() == stWaiting) && (PositionWrong || RobotErrorLarge) ) {
  			/*Calibrate on the home base*/
  				OnHomeBaseCalibrate(HomeBase);
  			}
  		}
  	} else {
  		SetOnHomeBase(false);
  	}
  	return;
  }
  
  void CRobot::OnHomeBaseCalibrate(CHomeBase* HomeBase)
  {
  	TRobotInfo OldRobotInfo = GetRobotInfo();
  	SetEnableRepositionOnUnknownObstacle(false);
  	SetEStopVisible(true);
  	SetState(stAutoCalibrate);
  	SetHandleLength(100);
  	SetLearnRoom(false);
  	AddBeepToInstructions(1000, 2000);
  	AddSquareToWallToInstructions(GetHeading(), HomeBase->GetFloorType() == ftWood);
  	TTaskResult TaskResult = FollowInstructions(true, false, IR_InstructionComplete);
  	if (TaskResult == TR_TaskComplete) {
  		robPOINT Position;
  		Position.x = (HomeBase->m_P0.x + HomeBase->m_P1.x) / 2 - cRRNoseLength * cos(HomeBase->GetHeading());
  		Position.y = (HomeBase->m_P0.y + HomeBase->m_P1.y) / 2 - cRRNoseLength * sin(HomeBase->GetHeading());
  		SetPosition(Position);
  		AddToHeading(HomeBase->GetHeading() - GetTaskResultHeading());
  	/*Try a small backup (for when have wagon or vacuum on*/
  		Position = GetHandlePosition(-GetHandleLength());
  		Position.x -= 0.3*cTPI * cos(HomeBase->GetHeading());
  		Position.y -= 0.3*cTPI * sin(HomeBase->GetHeading());
  		AddPositionVelocityToInstructions(Position, NULL, 2.0, cComputerControlSpeed, TRUE);
  		AddHeadingToInstructions(HomeBase->GetHeading(), NULL, cMinHeadingZone);
  		AddWaitToInstructions(1000);
  		FollowInstructions(true, false);
  		if (!GetOnHomeBase()) {
  		/*If small backup didn't work, try bigger one*/
  			Position.x -= 0.45*cTPI * cos(HomeBase->GetHeading());
  			Position.y -= 0.45*cTPI * sin(HomeBase->GetHeading());
  			AddPositionVelocityToInstructions(Position, NULL, 2.0, cComputerControlSpeed, TRUE);
  			AddHeadingToInstructions(HomeBase->GetHeading(), NULL, cMinHeadingZone);
  			AddWaitToInstructions(1000);
  			FollowInstructions(true, false);
  		}
  
  		AddWaitToInstructions(4000);
  		FollowInstructions(true, false);
  		if (!GetOnHomeBase()) {
  			HomeBase->Dock(this);
  		}
  	} else {
  		SetBuzzerFrequency(0);
  	}
  	SetEnableRepositionOnUnknownObstacle(true);
  	SetRobotInfo(OldRobotInfo);
  }
  
  long BytesToLong(unsigned char* Bytes) {return (long)Bytes[0] | ((long)Bytes[1]<<8) | ((long)Bytes[2]<<16) | ((long)Bytes[3]<<24);}
  long BytesToLong(unsigned char Byte0, unsigned char Byte1, unsigned char Byte2, unsigned char Byte3)
  {
  	return (long)Byte0 | ((long)Byte1<<8) | ((long)Byte2<<16) | ((long)Byte3<<24);
  }
  
  short BytesToInt(unsigned char Byte0, unsigned char Byte1)
  {
  	return ((short)Byte0 | ((short)Byte1<<8));
  }
  
  double RobotToWorldUnits(long RobotUnit)
  {
  	return (RobotUnit * cTPI / cRobotEncoderTicksPerInch);
  }
  
  void CRobot::OnUnknownMessage(CString Message) 
  {
  	const char cTaskResultInput = 'r';
  	const char cSoftwareRevNumber = 't';
  	const char cRobotOn = 'o';
  	const char cRobotOnSeconds = 'Z';
  	const char cSendEX2 = '2';
  	const char cExtendedPromptPathSegment = 'Y';
  	const char cRequestTuneNotes = 'X';
  	const char cSendNumberOfClapsMessage = 'W';
  	const cPathBufferEmptyMessage = 'U';
  	const cTuneBufferEmptyMessage = 'T';
  	const cSendDestination = 'M';
  
  	robPOINT Position;
  	double Heading;
  
  	switch (Message.GetAt(0)) {
  	case cExtendedPromptPathSegment:
  		Position.x = RobotToWorldUnits(BytesToLong(Message.GetAt(1), Message.GetAt(2), Message.GetAt(3), Message.GetAt(4)));
  		Position.y = RobotToWorldUnits(BytesToLong(Message.GetAt(5), Message.GetAt(6), Message.GetAt(7), Message.GetAt(8)));
  		Heading = BytesToInt(Message.GetAt(9), Message.GetAt(10))*2*pi/cNHeadings;
  		OnPositionChange((long)Position.x, (long)Position.y, Heading);
  		OnNewPathSegmentRequest();
  		break;
  	case cRequestTuneNotes:
  		OnSendNextTuneNotes();
  		break;
  	case cSendNumberOfClapsMessage:
  		unsigned int NumberOfClaps;
  		NumberOfClaps = BytesToInt(Message.GetAt(1), Message.GetAt(2));
  		OnClaps(NumberOfClaps);
  		break;
  	case cPathBufferEmptyMessage:
  		OnCyeComPathBufferEmpty();
  		break;
  	case cTuneBufferEmptyMessage:
  #ifdef _DEBUG
  //		AfxMessageBox("Tune Buffer Empty");
  #endif
  		break;
  	case cSendDestination:
  		{
  #ifdef IMPLEMENTED
  		robPOINT OldDest;
  		OldDest = m_RobotDestination;
  		long RobotX, RobotY;
  		RobotX = BytesToLong( Message.GetAt(1), Message.GetAt(2), Message.GetAt(3), Message.GetAt(4));
  		RobotY = BytesToLong( Message.GetAt(5), Message.GetAt(6), Message.GetAt(7), Message.GetAt(8));
  		m_RobotDestination.x = RobotToWorldUnits(RobotX);
  		m_RobotDestination.y = RobotToWorldUnits(RobotY);
  		m_FollowingPath = (((unsigned char)Message.GetAt(9)) & 1) != 0;
  		bool ReceivedFollowPath = (((unsigned char)Message.GetAt(9)) & 2) != 0;
  		bool ReceivedPathSegment = (((unsigned char)Message.GetAt(9)) & 4) != 0;
  		m_MusicPlaying = (unsigned char)Message.GetAt(10);
  		m_NumberOfClaps = (unsigned char)Message.GetAt(11);
  		CRect UpdateRect;
  		if (GetActiveRobot() != NULL) {
  			UpdateRect = CalculateUpdateRect(OldDest, GetActiveRobot()->GetPositionError(), m_RobotDestination, GetActiveRobot()->GetPositionError());
  			RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
  		}
  		PrintTimeStamp(cout);
  		cout << setw(8) << RobotX << ", ";
  		cout << setw(8) << RobotY << ", ";
  		cout << setw(8) << m_RobotDestination.x << ", ";
  		cout << setw(8) << m_RobotDestination.y << ", ";
  		cout << setw(3) << m_FollowingPath << ", ";
  		cout << setw(3) << ReceivedFollowPath << ", ";
  		cout << setw(3) << ReceivedPathSegment << ", ";
  		cout << setw(3) << m_MusicPlaying << ", ";
  		cout << setw(3) << m_NumberOfClaps << '\n';
  #endif
  		}
  		break;
  	default:
  #ifdef _DEBUG
  		fprintf(stderr, "CRobot::OnUnknownMessage Unhandled Message\n");
  		fprintf(stderr, "Unknown Message = ");
  		for (int i = 0; i < Message.GetLength(); i++) {
  			fprintf(stderr, "%3d ", (unsigned char)Message.GetAt(i));
  		}
  		fprintf(stderr, "\n");
  #endif
  		ASSERT(FALSE);
  		break;
  	}
  }
  
  void CRobot::OnClaps(int NumberOfClaps)
  {
  //	m_Parent->m_DlgMotorControls.UpdateData(TRUE);
  //	m_Parent->m_DlgMotorControls.m_edtOutput.Format("%d", NumberOfClaps);
  //	m_Parent->m_DlgMotorControls.UpdateData(FALSE);
  	if (NumberOfClaps == -1) {
  		CTimeSpan MaxStopClapTime(0, 0, 0, 5);
  		if ( MaxStopClapTime > (CTime::GetCurrentTime() - m_LastClapTime) ) {
  			if (GetState() != stWaiting) {
  				EStop();
  			}
  		}
  	} else {
  		m_LastClapTime = CTime::GetCurrentTime();
  		if (NumberOfClaps == 1) {
  			CHomeBase* HomeBase = m_ProbabilityGrid->GetClosestUnoccupiedHomeBase(GetPosition());
  			if (HomeBase != NULL) {
  				HomeBase->Dock(this, cComputerControlSpeed);
  			}
  		} else {
  			CExecutableRWGraphicObject* ClapObject = m_ProbabilityGrid->m_ExecutableObjectList->GetObjectWithNumberOfClaps(NumberOfClaps);
  			if (ClapObject != NULL) {
  				ClapObject->Execute(this, cComputerControlSpeed);
  			}
  		}
  	}
  }
  
  void CRobot::OnCommunicationsEstablished() 
  {
  	OnInitialCommunicationEstablished();
  	m_Parent->OnRobotCommunicationEstablished();
  }
  
  void CRobot::OnRadioLevelChange(float RadioLevel)
  {
  	m_Parent->OnRadioLevelChange(RadioLevel, this);
  }
  
  void CRobot::OnBatteryLow()
  {
  	m_Parent->OnBatteryLow();
  }
  
  void CRobot::OnBatteryChargingStateChange(short State)
  {
  	m_Parent->OnBatteryChargingStateChange(State, this);
  	SetBatteryChargeState(State);
  }
  
  void CRobot::OnTaskResult(TInstructionResult Result, robPOINT Position, double Heading)
  {
  	SetTaskResult(Result, Position, Heading);
  }
  
  void CRobot::OnSoftwareRevNumber(float RevNumber) 
  {
  	SetSoftwareRevision(RevNumber);
  }
  
  void CRobot::OnRobotOn() 
  {
  	OnInitialCommunicationEstablished();
  	CHomeBase *ClosestHomeBase = m_ProbabilityGrid->GetClosestUnoccupiedHomeBase(GetPosition());
  	if ( (GetState() == stWaiting) && (ClosestHomeBase != NULL) ) {
  		OnHomeBaseCalibrate(ClosestHomeBase);
  	}
  }
  
  void CRobot::OnRobotOnTime(long SecondsOn)
  {
  	((CRobotWorldApp*)AfxGetApp())->SetRobotOnSeconds(SecondsOn);
  }
  
  void CRobot::OnPositionChange(long X, long Y, double Heading)
  {
  //	static BOOL InRedZone = FALSE;
  //	static bool FirstPosition = true;
  	robPOINT OldPosition = GetPosition();
  	double OldHeading = GetHeading();
  	if (GetState() != stRelocating) {
  		m_ReceivedNewPosition = true;
  		m_Position.x = X;
  		m_Position.y = Y;
  		m_Heading = Heading;
  //		m_DlgMotorControls.m_CurrentHeading = long(Heading*cNHeadings/(2*pi));
  	/*On the first position received from the robot, initialize everything*/
  		if (m_FirstPositionReceived) {
  			m_FirstPositionReceived = false;
  			robPOINT PositionError = GetPositionError();
  			double HeadingError = GetHeadingError();
  			CalculatePositionAndHeadingError(m_Position, m_Heading, TRUE, TRUE);
  			SetPositionError(PositionError);
  			SetHeadingError(HeadingError);
  			m_Parent->RedrawWindow();
  			m_Parent->CenterRobot();
  		}
  		if ( (m_Position.x != OldPosition.x) || (m_Position.y != OldPosition.y) || (m_Heading != OldHeading) ) {
  			robPOINT OldPositionError = GetPositionError();
  		/*Calculate the position error now*/
  			CalculatePositionAndHeadingError(m_Position, m_Heading, FALSE, FALSE);
  		/*Do Probability Grid Stuff*/
  			if (GetLearnRoom()) {
  				m_Parent->GetDocument()->SetModifiedFlag();
  				robPOINT PositionError = m_PositionError;
  
  				const double cMaxPositionError = 240.0;
  				if (PositionError.x > cMaxPositionError) PositionError.x = cMaxPositionError;
  				if (PositionError.y > cMaxPositionError) PositionError.y = cMaxPositionError;
  
  				m_ProbabilityGrid->m_RobotWorld->ClearRoverPosition(m_Position.x,
  					m_Position.y, m_Heading, PositionError.x, PositionError.y,
  					m_HeadingError);
  			} else {
  			}
  			if (!m_ProbabilityGrid->GetRobotAnimate()) {
  				SetDrawPosition(GetPosition());
  				SetDrawHeading(GetHeading());
  				m_AnimateRobotStepNumber = 0;
  				m_Parent->RedrawWindow(m_Parent->CalculateUpdateRect(OldPosition, OldPositionError, GetPosition(), GetPositionError()), NULL, RDW_INVALIDATE | RDW_NOERASE);
  			}
  		/*See if robot's in a Red Zone*/
  			if (m_Parent->m_RedZoneList->InRedZone(m_Position)) {
  				if (!m_InRedZone) {
  					m_InRedZone = true;
  					EStop();
  				}
  			} else {
  				m_InRedZone = false;
  			}
  		/*See if robot's inside the selected finish line*/
  			m_ProbabilityGrid->m_RWGraphicObjectList->UpdateStartAndFinishLines(GetPosition(), float(m_ProbabilityGrid->ClientSizeToRobotSize(1)));
  		}
  	}
  }
  
  void CRobot::Animate()
  {
  	const float cAnimationDT = 0.041f;
  	if (!m_ReceivedNewPosition) {
  		if (GetState() != stRelocating) {
  			GetVelocities(m_AnimateRobotVr, m_AnimateRobotVl);
  			if ( (m_AnimateRobotVl != 0) || (m_AnimateRobotVr != 0) ) {
  				if (m_AnimateRobotStepNumber < 2) {	//Only animate for 1/8 second
  					++m_AnimateRobotStepNumber;
  					double NewHeading = GetDrawHeading();
  					robPOINT NewPosition = GetDrawPosition();
  					robPOINT OldPosition = NewPosition;
  					double OldHeading = GetDrawHeading();
  					NewHeading += (m_AnimateRobotVr - m_AnimateRobotVl) * cAnimationDT / (cRRWidth);
  					NewPosition.x += (m_AnimateRobotVr + m_AnimateRobotVl) * cAnimationDT / 2 * cos(GetDrawHeading());
  					NewPosition.y += (m_AnimateRobotVr + m_AnimateRobotVl) * cAnimationDT / 2 * sin(GetDrawHeading());
  					SetDrawPosition(NewPosition);
  					SetDrawHeading(NewHeading);
  					if ( (NewPosition != OldPosition) || (NewHeading != OldHeading) ) {
  						m_Parent->RedrawWindow(m_Parent->CalculateUpdateRect(OldPosition, GetPositionError(), NewPosition, GetPositionError()), NULL, RDW_INVALIDATE | RDW_NOERASE);
  					}
  				}
  			}
  		}
  	} else {
  		m_Parent->RedrawWindow(m_Parent->CalculateUpdateRect(GetDrawPosition(), GetPositionError(), m_Position, GetPositionError()), NULL, RDW_INVALIDATE | RDW_NOERASE);
  		SetDrawPosition(GetPosition());
  		SetDrawHeading(GetHeading());
  		m_ReceivedNewPosition = false;
  		m_AnimateRobotStepNumber = 0;
  		m_Parent->UpdateWindow();
  	}
  }
  
  void CRobot::OnVelocityChange(short RightVelocity, short LeftVelocity)
  {
  	const short cFast = short(0.9*cMaxSpeed);
  	short OldVelocity = (m_M0VelocityMeasured + m_M1VelocityMeasured) / 2;
  	m_M0VelocityMeasured = RightVelocity;
  	m_M1VelocityMeasured = LeftVelocity;
  	m_Parent->OnMotorVelocityUpdate(RightVelocity, LeftVelocity, this);
  
  	if ( (OldVelocity < 0.5*cFast) && ((m_M0VelocityMeasured + m_M1VelocityMeasured) / 2 >= cFast) ) {
  		CString SoundPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
  		SoundPath += "/sounds/RobFast.wav";
  		PlaySound(SoundPath, NULL, SND_FILENAME | SND_ASYNC ); 
  	}
  	OldVelocity = (RightVelocity + LeftVelocity) / 2;
  }
  
  void CRobot::OnAnalogUpdate(float RightCurrent, float LeftCurrent, float BatteryVoltage)
  {
  	m_M0CurrentMeasured = RightCurrent;
  	m_M1CurrentMeasured = LeftCurrent;
  	m_BatteryVoltageMeasured = BatteryVoltage;
  	m_Parent->OnMotorCurrentUpdate(RightCurrent, LeftCurrent, this);
  	m_Parent->OnBatteryVoltageUpdate(BatteryVoltage, this);
  
  	float BatteryLevel;
  	float MinV, MaxV;
  	float TotalCurrent = m_M0CurrentMeasured + m_M1CurrentMeasured + 0.23f;
  	
  	if (TotalCurrent >= 2.9) MinV = 7.75f;
  	else if (TotalCurrent <= 0.5075) MinV = 10.5f;
  	else MinV = -1.1368f * TotalCurrent + 11.107f;
  
  	MaxV = -0.1752f * TotalCurrent + 12.796f;
  
  	if (BatteryVoltage >= MaxV) BatteryLevel = 100.0f;
  	else if (BatteryVoltage <= MinV) BatteryLevel = 0;
  	else BatteryLevel = 100 * (BatteryVoltage - MinV) / (MaxV - MinV);
  
  	if (m_BatteryVoltageMeasured < GetLowBatteryVoltage()) {
  		m_BatteryLowTime++;
  		if (m_BatteryLowTime == 3) {
  			GoHomeOnLowBattery();
  		}
  	} else {
  		m_BatteryLowTime = 0;
  		m_GoHomeOnLowBattery = true;
  	}
  }
  
  void CRobot::GoHomeOnLowBattery()
  {
  	if (!GetOnHomeBase()) {
  		DisplayErrorText("Battery Low, I'm going home.");
  		if ( (!m_GoingHome) && m_GoHomeOnLowBattery ) {
  			m_GoingHome = true;
  			EStop();
  			CHomeBase* HomeBase = m_ProbabilityGrid->GetClosestUnoccupiedHomeBase(GetPosition());
  			if (HomeBase != NULL) {
  				if (HomeBase->Dock(this, cComputerControlSpeed) != TR_TaskComplete) {
  					m_GoHomeOnLowBattery = false;
  				}
  			}
  			m_GoingHome = false;
  		}
  	}
  }
  
  void CRobot::OnMicrophoneVolume(short Volume) 
  {
  //	m_Parent->m_DlgMotorControls.UpdateData(TRUE);
  //	m_Parent->m_DlgMotorControls.m_edtMicrophoneVolume = Volume;
  //	m_Parent->m_DlgMotorControls.UpdateData(FALSE);
  }
  
  void CRobot::SetColor(TRobotColor Color)
  {
  	m_Color = Color;
  //	m_ProbabilityGrid->SeTRobotColor(RobotColor);
  	if (m_Version > 1) {
  		switch (Color) {
  		case crBlack:		//Black
  			SetAddress(1);
  			break;
  		case crOrange:	//Orange
  			SetAddress(3);
  			break;
  		case crYellow:	//Yellow
  			SetAddress(4);
  			break;
  		case crChrome:	//Chrome
  			SetAddress(5);
  			break;
  		default:
  			ASSERT(FALSE);
  			SetAddress(1);
  			break;
  		} 
  	}
  	m_Parent->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_NOERASE);
  }
  
  TRobotColor CRobot::GetColor()
  {
  	return m_Color;
  }
  
  void CRobot::SetVersion(int Version)
  {
  	ASSERT(Version > 0);
  	m_Version = Version;
  }
  
  int CRobot::GetVersion()
  {
  	return m_Version;
  }
  
  void CRobot::ShowCyePersonaDialog(bool Show)
  {
  	if (Show) {
  		m_CyePersona->ShowWindow(SW_SHOW);
  	} else {
  		m_CyePersona->ShowWindow(SW_HIDE);
  	}
  }
