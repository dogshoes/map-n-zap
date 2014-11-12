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
#ifndef ROBOT20398lkjdsf23490fdsl03945dflug

#define ROBOT20398lkjdsf23490fdsl03945dflug

#include "rrdim.h"
#include "rothmath.h"
#include "FindPathThread.h"

class RobotCom;
class CRobotWorldView;
class CCalibrationZone;
class CCornerWall;
class CWall;
class CDoorway;
class Path;
class CCTDPath;
class CDlgEStop;
class CProbabilityGrid;
class CDlgRobotError;
class CHomeBase;
class CCyePersona;

const double cMinHeadingZone = 2 * pi * 4 / cNHeadings;
const double cSmlHeadingZone = pi / 36;	//5 degrees
const double cModHeadingZone = pi / 9;	//20 degrees
const double cLrgHeadingZone = pi / 4.5;	//40 degrees
//const double cMinPositionZone = 4.0 * cIPT/10;
const double cMinPositionZone = 5.0;
const double cSmlPositionZone = 5.0;
const double cModPositionZone = 15.0;
const double cLrgPositionZone = 30.0;

typedef enum {stCalibrating, stWaiting, stDragging, stFollowingInstructions,
              stRelocating, stFindPath, stAutoCalibrate, stExecuting, stJoysticking
             } TState;

typedef enum {ftWood, ftThinCarpet, ftThickCarpet} TFloorType;
typedef enum {atNone, atWagon, atVacuum} TAttachmentType;
typedef enum {crNone = -1, crBlack = 0, crOrange, crYellow, crChrome} TRobotColor;
typedef enum {crLegacyBlack = 9, crLegacyOrange = 200, crLegacyYellow = 300, crLegacyChrome = 400} TRobotColorLegacy;

struct TRobotInfo
{
    bool EnableRepositionOnUnknownObstacle;
    bool FlipHandle;
    bool EnableObstacleBounce;
    bool EStopVisible;
    short HandleLength;
    short MaxSpeed;
    TObstacleSensitivity ObstacleSensitivity;
    TState RobotState;
    bool UpdateProbabilityGrid;
    bool ShowCurrentPath;
};

//enum TSearchAlgorithm {None, SearchWallLeft, SearchWallRight, SearchArea};

typedef enum {M0Back = 0, M0Front = 1, M1Back = 2, M1Front = 3,
              M0Back_M1Back = 4, M0Front_M1Back = 5, M0Back_M1Front = 6,
              M0Front_M1Front = 7, NoObstacle
             } TMotorObstacle;

struct TObstacle
{
    TMotorObstacle MotorObstacle;
    robPOINT Position;
    double Heading;
    bool ValidObstacle;
};

typedef enum {IR_InstructionComplete = 0, IR_HitObstacle = 1, IR_TimedOut = 2, IR_EmergencyStop = 3, IR_InstructionIncomplete = 4} TInstructionResult;
typedef enum {TR_TaskComplete = 0, TR_Error = 1, TR_EmergencyStop = 2} TTaskResult;
typedef enum {NDG_None = 0, NDG_M0Nudged = 1, NDG_M1Nudged = 2} TNudge;
typedef enum {meM0 = 0, meM1 = 1, meBoth = 2} TMotorEnable;
//struct TVacuumPatch {
//	robPOINT Corner1;
//	robPOINT Corner2;
//};

struct TRobotInstructionResult
{
    bool Valid;
    TInstructionResult Result;
    robPOINT Position;
    double Heading;
};

class CInstructionListNode;

struct InstructionListHead
{
    CInstructionListNode* Head;
    CInstructionListNode* Tail;
    //public:
    //	void SendPathHead(void);
};

//#define FIND_PATH_MESSAGES WM_USER + 200

enum {FIND_PATH_MESSAGES = WM_USER + 200, ID_ROBOT_FIND_PATH_FINISHED, ID_ROBOT_SHOW_FIND_PATH, ID_ROBOT_FIND_PATH_STAGE, ID_ROBOT_FIND_PATH_ERROR, ID_ROBOT_CHANGE_STATE, ID_ROBOT_ESTOP};
//#define ID_ROBOT_FIND_PATH_FINISHED FIND_PATH_MESSAGES
//#define ID_ROBOT_SHOW_FIND_PATH ID_ROBOT_FIND_PATH_FINISHED + 1
//#define ID_ROBOT_FIND_PATH_STAGE ID_ROBOT_SHOW_FIND_PATH + 1

//#define ID_ROBOT_CHANGE_STATE ID_ROBOT_SHOW_FIND_PATH + 1

class CFindPathThread;
class CGoalSet;

const short cInitKp = 3;
const short cInitKi = 6;
const short cInitKd = 5;
const short cInitKb = 8;
const short cInitKpwm = 0;

class CRobot : public CObject
{
    public:
        void ShowCyePersonaDialog(bool Show);
        bool GetEnableDancing(void);
        int GetVersion(void);
        void SetVersion(int Version);
        TRobotColor GetColor(void);
        void SetColor(TRobotColor Color);
        void Animate();
        void OnClaps(int NumberOfClaps);
        void OnError(long Error);
        void OnNewPathSegmentRequest();
        void OnNudgeDetected(bool RightWheel, bool LeftWheel);
        void OnObstacleDetected(short RightWheel, short LeftWheel);
        void OnOnHomeBase(short OnHomeBase);
        void OnUnknownMessage(CString Message);
        void OnCommunicationsEstablished();
        void OnRadioLevelChange(float RadioLevel);
        void OnBatteryLow();
        void OnBatteryChargingStateChange(short State);
        void OnTaskResult(TInstructionResult Result, robPOINT Position, double Heading);
        void OnSoftwareRevNumber(float RevNumber);
        void OnRobotOn();
        void OnRobotOnTime(long SecondsOn);
        void OnExternalInput(float Voltage);
        void OnMicrophoneVolume(short Volume);
        void OnAnalogUpdate(float RightCurrent, float LeftCurrent, float BatteryVoltage);
        void OnVelocityChange(short RightVelocity, short LeftVelocity);
        void OnPositionChange(long X, long Y, double Heading);
        void SetDrawPosition(robPOINT Position)
        {
            m_DrawPosition = Position;
        };
        robPOINT GetDrawPosition(void)
        {
            return m_DrawPosition;
        };
        void SetDrawHeading(double Heading)
        {
            m_DrawHeading = Heading;
        };
        double GetDrawHeading(void)
        {
            return m_DrawHeading;
        };
        void SetCommResponseTimeOut(long TimeOut);
        bool GetExternalOutput(void)
        {
            return m_ExternalOutput;
        };
        void SetEnableClapDetectionWhileMoving(bool Enable);
        TTaskResult FollowGoalSetWithObstacleAvoidance(CGoalSet* GoalSet, short Speed, CProbabilityGrid* ProbGrid);
        TInstructionResult FollowGoalSet(CGoalSet* GoalSet, short Speed);
        void SetEnableClapDetection(bool Enable);
        void SetEnableDancing(bool Enable);
        void OnCyeComPathBufferEmpty(void);
        short GetAddress(void);
        void SetAddress(short Address);
        int GetMicrophoneVolume(void);
        void SetMicrophoneVolume(int Volume);
        TTaskResult SpinAround(double OriginalHeading, int NumberOfRotations, bool CounterClockwise = true, short Speed = cObstacleDetectionSpeed);
        void SetHeadingCorrectionFactor(TFloorType FloorType, int HCF);
        void SetAlreadyCalibratedFloorType(TFloorType FloorType, bool Value);
        bool GetAlreadyCalibratedFloorType(TFloorType FloorType);
        TTaskResult SpinAround(short Speed, int NumberOfRotations, bool CounterClockwise);
        TTaskResult CheckHeadingCorrectionFactor(bool& GoodValue);
        int GetHeadingCorrectionFactor(TFloorType FloorType);
        void SetStatusBarText(CString Text);
        void SetBatteryChargeState(short State);
        void SendPing(void);
        void AddWaitForPingToInstructions(unsigned long TimeOut);
        bool GetEnableRepositionOnUnknownObstacle(void)
        {
            return m_EnableRepositionOnUnknownObstacle;
        };
        void SetEnableRepositionOnUnknownObstacle(bool Enable)
        {
            m_EnableRepositionOnUnknownObstacle = Enable;
        };
        bool GetPause(void)
        {
            return m_Pause;
        };
        float GetSoftwareRevision(void);
        void SetSoftwareRevision(float Revision);
        void OnSendNextTuneNotes(void);
        void AddTuneToInstructions(short Frequencies[], int Durations[], int NumberOfNotes);
        void StartFollowingCTDPath(CCTDPath* Path);
        void StartWaitingForExternalInputChange()
        {
            m_StartWaitingForExternalInputChange = true;
        };
        void SetExecutingRobotTask(bool Executing)
        {
            m_ExecutingRobotTask = Executing;
        };
        void SetFollowingPath(CCTDPath* Path);
        bool GetInstructionResultValid(void)
        {
            return m_RobotInstructionResult.Valid;
        };
        void SetInstructionResultValid(bool Value)
        {
            m_RobotInstructionResult.Valid = Value;
        };
        void SendTune(short* Frequency, int* msDuration, unsigned char NumberOfNotes, bool PurgeBuffer = false, bool StartPlaying = false);
        void AddWaitForInputLess(float Voltage, unsigned long TimeOut);
        void AddWaitForInputGreater(float Voltage, unsigned long TimeOut);
        void RequestExternalInput(void);
        int GetThickCarpetDefaultHeadingCorrectionFactor(void);
        int GetThinCarpetDefaultHeadingCorrectionFactor(void);
        bool RepositionRobotOnUnexpectedObstacle(CProbabilityGrid* ProbGrid, TMotorObstacle Obstacle);
        bool GetShowCurrentPath(void);
        bool SetShowCurrentPath(bool Show);
        bool GetCommunicationsEstablished(void);
        float GetLowBatteryVoltage(void);
        void Create(CRobotWorldView* Parent);
        float SlipPercent(float HeadingCorrectionFactor);
        void SetThickCarpetSlipPercent(float Slip);
        void SetThinCarpetSlipPercent(float Slip);
        float GetThickCarpetSlipPercent(void);
        float GetThinCarpetSlipPercent(void);
        bool GetLearnRoom(void)
        {
            return m_UpdateProbabilityGrid;
        };
        void SetLearnRoom(bool On)
        {
            m_UpdateProbabilityGrid = On;
        };
        void SendRequestEX2(void);
        void OnInitialCommunicationEstablished(void);
        void FindComPort(void);
        Path* CreatePath(robPOINT Start, robPOINT End);
        void SetExternalOutput(bool On);
        TAttachmentType GetAttachmentType(void);
        void SetAttachmentType(TAttachmentType AttachmentType);
        TFloorType GetFloorType(void);
        void SetFloorType(TFloorType FloorType);
        double GetTaskResultHeading(void);
        robPOINT GetTaskResultPosition(void);
        void AddToHeading(double Delta);
        void AddToPosition(robPOINT Delta);
        bool GetIntermittentBeep(void);
        void SetIntermittentBeep(bool On);
        void DisplayErrorText(UINT nID);
        void DisplayErrorText(CString Text);
        void SetRobotOnSeconds(long On);
        bool GetTouchedHomeBase(void);
        void SetTouchedHomeBase(bool Touched = false)
        {
            m_TouchedHomeBase = Touched;
        };
        void SendRAMData(unsigned short Address, unsigned char NumberBytes, unsigned char* Data);
        void SetRobotInfo(TRobotInfo RobotInfo);
        TRobotInfo GetRobotInfo(void);
        TTaskResult FollowPathWithObstacleAvoidance(short Speed, CProbabilityGrid* ProbGrid, bool StopAfterExecution);
        CInstructionListNode* GetInstructionListHead(void);
        TTaskResult GetAroundObstacle(robPOINT GetTo, short Speed, CProbabilityGrid* ProbGrid);
        bool GetEnableObstacleBounce(void)
        {
            return m_EnableObstacleBounce;
        };
        bool SetEnableObstacleBounce(bool Enable);
        void SetHeadingDestination(double Heading, short Speed, short MotorEnable);
        void AddHeadingToInstructions(double Heading, TCoordSystem* CoordinateSystem, double Zone, short MaxSpeed, TMotorEnable MotorEnable, unsigned long TimeOut);
        void SearchArea(robRECT Area);
        CInstructionListNode* GetCurrentInstruction(void);
        double SetFindPathBuffer(double buffer);
        float GetRadioLevel(void);
        //	bool GetVacuumAttached(void);
        //	void SetVacuumAttached(bool Attached);
        //	bool m_VacuumAttached;
        TTaskResult BackupFromWall(bool WallInFront = true, short Speed = cComputerControlSpeed);
        bool GetShowCalibrationSuccessfulDialog(void);
        void SetShowCalibrationSuccessfulDialog(bool show);
        void AddWaitLongToInstructions(int Hours, int Minutes, int Seconds);
        void Pause(bool p);
        long GetRetryLimit(void);
        void SetRetryLimit(long retry);
        void AddFindHomeBaseToInstructions(double CornerHeading, double WallHeading, robPOINT HomeBaseLocation, bool NeedToGetInFrontOfHomeBase = true);
        void SendFindHomeBase(double CornerHeading, double WallHeading, robPOINT HomeBaseLocation, bool NeedToGetInFrontOfHomeBase = true);
        void SendCornerCalibrate(robPOINT CalibrationWallCenter, double CalibrationWallHeading, unsigned char FeelWheel, bool SlipperySurface, short WheelDistanceFromWall);
        void AddCornerCalibrationToInstructions(robPOINT CalibrationWallCenter, double CalibrationWallHeading, unsigned char FeelWheel, bool SlipperySurfaceshort, short WheelDistanceFromWall = 55);
        //	void AddWallCalibrationToInstructions(robPOINT P0, double H0, robPOINT P1, double H1, bool Corner, unsigned char FeelWheel);
        //	void SendWallCalibrate(robPOINT P0, double H0, robPOINT P1, double H1, bool Corner, unsigned char FeelWheel);
        void AddFeelMotorToInstructions(unsigned char Motor, unsigned long TimeOut);
        void SetFeelingMotor(unsigned char Motor);
        TTaskResult SquareToWall(void);
        void SendSquareToWall(double Heading, bool SlipperySurface);
        void SetTaskResult(TInstructionResult TaskResult, robPOINT Position, double Heading);
        void AddSquareToWallToInstructions(double Heading, bool SlipperySurface);
        void SendSerialString(CString String);
        void GetVelocities(short& M0V, short& M1V);
        bool GetOnHomeBase(void)
        {
            return m_OnHomeBase;
        };
        void SetOnHomeBase(bool On);
        robPOINT GetHandlePosition(short HandleLength);
        robPOINT GetHandlePosition(void);
        void TurnOffPower(void);
        double GetHeading(void)
        {
            return m_Heading;
        };
        robPOINT GetPosition(void)
        {
            return m_Position;
        };
        robPOINT GetPositionError(void)
        {
            return m_PositionError;
        };
        void SetPositionError(robPOINT Error);
        double GetHeadingError(void)
        {
            return m_HeadingError;
        };
        void SetHeadingError(double Error);
        void ShowCalibrationSuccessfulDialog(robPOINT DPosition, double DHeading);
        TTaskResult FindTicksPerRotation(int ApproximateTicksPerRotation, int* TicksPerRotation, int NumberOfRotations = 1, bool CounterClockwise = true);
        float m_BatteryVoltageMeasured;
        MSG DoWindowMessages(void);
        void SendRequestState(void);
        void SetInformationToTransmit(bool Velocity, bool Acceleration, bool AD, bool Position);
        float m_M0CurrentMeasured;
        float m_M1CurrentMeasured;
        short m_M1VelocityMeasured;
        short m_M0VelocityMeasured;
        short m_M0AccelerationMeasured;
        short m_M1AccelerationMeasured;
        bool GetFlipHandle(void);
        double HeadingToDestination(robPOINT Position, robPOINT Destination);
        bool GetEStopVisible(void);
        void SetEStopVisible(bool Visible);
        //	bool GetAutoCalibrate(void);
        void SetTurn(double Heading, short MaxSpeed, short TurnDiameter);
        void Stop(void);
        void SetHeadingDestination(double Heading, short Velocity);
        void SetVelocityDirection(int Velocity, double Heading);
        bool GoodPath(Path* path, bool recalc = false);
        bool GetCommunicationOn(void);
        void SetCommunicationOn(bool On);
        void SetStraightVelocity(int Velocity, double Heading);
        void SetMaxPwm(int MaxPwm);
        void SetPositionPwmDestination(robPOINT Destination);
        void PurgeRingBuffer(void);
        void SetPositionVelocityDestination(robPOINT Destination, short Velocity);
        void SetVelocities(int M0V, int M1V);
        void SetP4(long P4);
        void SetP3(long P3);
        void SetP2(long P2);
        void SetP1(long P1);
        void SetP5(long P5);
        void SetP6(long P6);
        void SetP7(long P7);
        void SetP8(long P8);
        void SetPwm(int M0Pwm, int M1Pwm);
        void SetMotorControlConstants(char Kp, char Ki, char Kd, char Kb, char Kpwm);
        void GetMotorControlConstants(char& Kp, char& Ki, char& Kd, char& Kb, char& Kpwm);
        void SetHeadingCorrectionFactor(int Correction);
        void SetBuzzerOn(bool On);
        void SetBuzzerFrequency(short Frequency);
        void EnableComm(bool Open);
        void AddWaitUntilToInstructions(CTime Time);
        unsigned int m_FoundPathNow;
        CGoalSet* m_FindPathNowGoalSet;
        TNudge m_Nudged;
        void AddWaitForNudgeToInstructions(unsigned long TimeOut);
        void AddWaitForInputChange(unsigned long TimeOut);
        void AddWaitForInputHigh(unsigned long TimeOut);
        void AddWaitForInputLow(unsigned long TimeOut);
        //	void SetAutoCalibrate(bool AutoCalibrate);
        CRobotWorldView* GetParent(void)
        {
            return m_Parent;
        };
        //	void AutoCalibrate(void);
        void StopFindPath(void);
        TState GetState(void)
        {
            return m_State;
        };
        void SetState(TState State);
        void FindPath(robPOINT Start, robPOINT End, int Iterations, int Repeat, bool ShowPaths = true, TFoundPathFunction FoundPathFunction = NULL, UINT FoundPathParam1 = 0, void* FoundPathParam2 = NULL);
        void FindPath(Path* OldPath, int Iterations, int Repeat, bool ShowPaths = true, TFoundPathFunction FoundPathFunction = NULL, UINT FoundPathParam1 = 0, void* FoundPathParam2 = 0);
        CGoalSet* FindPathNow(robPOINT Start, robPOINT End, int Iterations, int Repeat, bool ShowPaths);
        void SetFlipHandle(bool FlipHandle);
        void SetObstacleSensitivity(TObstacleSensitivity Sensitivity);
        TObstacleSensitivity GetObstacleSensitivity(void)
        {
            return m_ObstacleSensitivity;
        };
        //	TTaskResult CalibrateAgainstCornerWall(CCornerWall* CalibrationZone);
        short GetHandleLength(void)
        {
            return m_HandleLength;
        };
        short GetMaxSpeed(void)
        {
            return m_MaxSpeed;
        };
        void SetMaxSpeed(short MaxSpeed);
        void New(void);
        void Serialize(CArchive & archive);
        void SerializeV2(CArchive & archive);
        void SetHandleLength(short HandleLength);
        void AddCTDPathToInstructions(CCTDPath* Path, double Zone = cLrgPositionZone, bool FlipHandle = false, unsigned long TimeOut = 0);
        void AddPathToInstructions(Path* path, unsigned int Speed, double Zone = cLrgPositionZone, bool FlipHandle = false, unsigned long TimeOut = 0, bool GoForward = true);
        void OnSendNextCTDPathSegment(void);
        TTaskResult CalibrateFloorSurface(int* HeadingCorrectionFactor, bool ShowEStop = true);
        void EStop(void);
        void AddPositionVelocityToInstructions(robPOINT Destination, TCoordSystem* CoordinateSystem = NULL, double Zone = cLrgPositionZone, short MaxSpeed = cComputerControlSpeed, bool FlipHandle = false, unsigned long TimeOut = 0);
        void SetPosition(robPOINT Position);
        void SetHeading(double Heading);
        robPOINT GetRobotToPoint(robPOINT Position, TCoordSystem* CoordinateSystem);
        void AddStraightVelocityToInstructions(double Heading, short Velocity, TCoordSystem* CoordinateSystem, unsigned int TravelTime);
        void GetFrontCorners(robPOINT Position, double Heading, robPOINT* C1, robPOINT* C2);
        robPOINT* GetCorners(robPOINT Corners[4]);
        robPOINT* GetCorners(robPOINT Position, double Heading, robPOINT Corners[4]);
        TLineErr GetFrontLine(robPOINT Position, double Heading, TLine* FrontLine);
        void CalculatePositionAndHeadingError(robPOINT NewPosition, double NewHeading, bool ResetPosition, bool ResetHeading);
        void AddPwmToInstructions(short M0Pwm, short M1Pwm, unsigned int TravelTime);
        void AddVelocitiesToInstructions(short M0V, short M1V, unsigned int TravelTime);
        TObstacle m_MotorObstacle;
        TInstructionResult FollowInstructions(bool PurgeInstructionList = true, bool StopRobotAfterExecution = true);
        TTaskResult FollowInstructions(bool PurgeInstructionList, bool StopRobotAfterExecution, TInstructionResult ExpectedResult, bool DisplayErrorMessages = true);
        double RobotToWorldHeading(double Heading, double WorldHeading);
        robPOINT RobotToWorldCoords(robPOINT Position, robPOINT WorldPos, double WorldHeading);
        void AddTurnToInstructions(double Heading, short TurnDiameter, TCoordSystem* CoordinateSystem, double Zone, short MaxSpeed, unsigned long TimeOut);
        void AddHeadingToInstructions(double Head, TCoordSystem* CoordinateSystem = NULL, double Zone = cLrgHeadingZone, short MaxSpeed = cComputerControlHeadingSpeed, unsigned long TimeOut = 0);
        ~CRobot();
        void SetDrag(bool value, CWnd* DispWin);
        void PurgeInstructions(void);
        void AddPositionBungeeToInstructions(robPOINT Destination, TCoordSystem* CoordinateSystem = NULL, double Zone = cLrgPositionZone,  bool FlipHandle = false, unsigned long TimeOut = 0);
        void AddBeepToInstructions(unsigned short Frequency, unsigned long TimeOut);
        void AddWaitToInstructions(unsigned int msTimeOut);
        CRobot(CRobotWorldView* RobotCommParent, CProbabilityGrid* ProbGrid);
        //	void Move(robPOINT Position, double Heading, robPOINT Destination);
        robPOINT m_Position;
        double m_Heading;
        robPOINT m_Destination;
        //	TTaskResult CalibratePosition(CCalibrationZone* CalibrationZone);
        robPOINT m_PositionError;
        double m_HeadingError;
        TTaskResult SquareToWall(TMotorObstacle Obstacle);
        TTaskResult SquareToWallFast(TMotorObstacle Obstacle);
        TTaskResult FindCorner(robPOINT* Corner, double* Heading);
        float m_KHeadErr;
        float m_KStraightHeadErr;
        RobotCom* m_RobotComm;
    protected:
        void GoHomeOnLowBattery();
        short m_BatteryLowTime;
        bool m_GoHomeOnLowBattery;
        int m_AnimateRobotStepNumber;
        short m_BatteryChargeState;
        float m_ExternalInputVoltage;
        bool m_StartWaitingForExternalInputChange;
        bool m_ShowCurrentPath;
        bool m_UpdateProbabilityGrid;
        bool m_OnHomeBase;
        bool m_TouchedHomeBase;
        char m_Kp;
        char m_Ki;
        char m_Kd;
        char m_Kb;
        char m_Kpwm;
        bool m_FlipHandle;
        bool m_CommunicationsEstablished;
        CRobotWorldView* m_Parent;
        CDlgEStop* m_DlgEStop;
        CDlgRobotError* m_DlgRobotError;
    private:
        CCyePersona* m_CyePersona;
        int m_Version;
        bool m_InRedZone;
        bool m_FirstPositionReceived;
        TRobotColor m_Color;
        void OnFollowingPath(CCTDPath *FollowingPath);
        bool m_GoingHome;
        short m_AnimateRobotVr;
        short m_AnimateRobotVl;
        bool m_ReceivedNewPosition;
        CTime m_LastClapTime;
        void OnHomeBaseCalibrate(CHomeBase* HomeBase);
        CProbabilityGrid* m_ProbabilityGrid;
        short m_Address;
        int m_MicrophoneVolume;
        bool m_EnableRepositionOnUnknownObstacle;
        float m_SoftwareRevision;
        float ProbabilityOfRobotBeingHere(robPOINT Point);
        void SetThickCarpetHeadingCorrectionFactor(short HeadingCorrectionFactor);
        void SetThinCarpetHeadingCorrectionFactor(short HeadingCorrectionFactor);
        TAttachmentType m_AttachmentType;
        TFloorType m_FloorType;
        double m_FindPathBuffer;
        bool m_EnableObstacleBounce;
        short SearchAlongWall(robRECT Area, double SearchHeading, double WallHeading, bool* done);
        short FindObstacleDirection(double ForwardDir, double NextSearchDir, TLine NextSwatchLine);
        bool m_Pause;
        bool m_ExecutingRobotTask;
        TRobotInstructionResult m_RobotInstructionResult;
        //	bool m_RobotInstructionResultValid;
        //	TInstructionResult m_RobotInstructionResult;
        int m_HeadingCorrectionFactor;
        bool m_ShowCalibrationSuccessfulDialog;
        static void FindPathNowCallback(CGoalSet* GoalSet, UINT Parm0, void* Parm1);
        //	bool m_AutoCalibrateFailed;
        //	bool m_AutoCalibrate;
        CCalibrationZone* m_CloseCalZonesList[3];
        CFindPathThread* m_FindPathThread;
        TObstacleSensitivity m_ObstacleSensitivity;
        //	FILE* m_OutFile;
        short m_MaxSpeed;
        short m_HandleLength;
        CCTDPath* m_CurrentPath;
        void SendInstructionsHead(void);
        void RemoveHeadFromInstructions(void);
        bool ReachedNextPoint(void);
        bool InPositionZone(void);
        void GoToNextNodeInInstructions(void);
        void AddNodeToInstructions(CInstructionListNode* NewNode);
        InstructionListHead* m_InstructionList;
        bool IsInside(robPOINT* point);
        TTaskResult CalibrateAgainstWall(CCalibrationZone* CalibrationZone);
        TTaskResult CalibrateAgainstDoorway(CCalibrationZone* CalibrationZone);
        TTaskResult CalibrateTicksPerRotation(int* Ticks, robPOINT& InitialPosition, double& InitialHeading);
        TState m_State;
        //	static void AutoCalFoundPathFunction(Path* FoundPath, UINT FoundPathParam, void* Robot, bool GoodPath);
        bool m_IntermittentBeepOn;
        float TotalTicksPerRotation(float HeadingCorrectionFactor);
        float HeadingCorrectionFactor(float TotalTicksPerRotation);
        float HeadingCorrectionFactorFromSlipPercent(float Slip);
        bool m_ExternalOutput;
        bool m_PositionWasReset;
        bool m_HeadingWasReset;
        robPOINT m_OldPosition;
        double m_OldHeading;
        robPOINT m_DrawPosition;
        double m_DrawHeading;
};

#endif