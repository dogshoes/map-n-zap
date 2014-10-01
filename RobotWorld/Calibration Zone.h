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
 #ifndef CALIBRATIONZONE02030349860983405982034985
 
 #define CALIBRATIONZONE02030349860983405982034985
 
 #include "ExecutableRWGraphicObject.h"
 #include "rothmath.h"
 #include "rrdim.h"
 #include "Robot.h"
 #include "ddraw.h"
 
 enum TCalibrationZone {czWall, czDoorway, czCornerWall};
 
 class CCalibrationZoneList;
 
 class CCalibrationZone : public CExecutableRWGraphicObject {
 public:
 	TTaskResult GoToEntrance(CRobot *Robot, short Speed = 100);
 	virtual TTaskResult Adjust(CRobot * Robot);
 	void DrawObject(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid);
 	void Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbGrid);
 	TFloorType GetFloorType(void) {return m_FloorType;};
 	void SetCaption(CString Caption);
 	double GetHeading(void) {return m_Heading;};
 	virtual TTaskResult Execute(CRobot* Robot, short Speed = 100);
 	unsigned int GetZLayer(void) {return 150;};
 
 	TRWGraphicObjectType GetObjectType(void) {return goCalibrationZone;};
 	robPOINT GetExitPoint(void);
 	void EditProperties(void);
 	void SetBounds(robRECT bounds);
 	void Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid);
 	virtual robPOINT GetEntryPoint(void);
 //	TCalibrationZone GetZoneType(void);
 	virtual void Serialize(CArchive & archive);
 	~CCalibrationZone();
 	CCalibrationZone(robPOINT P0, robPOINT P1, double Heading, TCalibrationZone ZoneType);
 	double m_Heading;
 	robPOINT m_P0;
 	robPOINT m_P1;
 	bool InCalibrationZone(double CalibrateDistance, robPOINT Position);
 	TTaskResult SpecialFunction(CRobot* Robot, short Speed);
 private:
 	DECLARE_SERIAL(CCalibrationZone)
 protected:
 	TFloorType m_FloorType;
 	static LPDIRECTDRAWSURFACE3 m_lpDDSCalibrationZoneBmp;
 	static LPDIRECTDRAWSURFACE3 m_lpDDSArrowBmp;
 	static float m_OldZoomLevel;
 	virtual TTaskResult Calibrate(CRobot* Robot, robPOINT* DPosition, double* DHeading);
 	int ArrowHeadingToArrowBitmapFrame(double Heading, int TotalFrames);
 	void CalculateBounds(void);
 	CCalibrationZone();
 //	TCalibrationZone m_ZoneType;
 };
 
 class CWall : public CCalibrationZone {
 public:
 	TTaskResult Adjust(CRobot *Robot);
 	void operator=(CWall Wall);
 	bool Create(CRobot* Robot);
 	CWall(robPOINT P0, double H0);
 	CWall();
 	CWall(CWall& Wall);
 private:
 	DECLARE_SERIAL(CWall)
 protected:
 	bool CreateFrontWall(CRobot* Robot);
 	TTaskResult Calibrate(CRobot* Robot, robPOINT* DPosition, double* DHeading);
 };
 
 class CDoorway : public CCalibrationZone {
 public:
 	robPOINT GetEntryPoint(void);
 	CDoorway(robPOINT P0, robPOINT P1, double H0, double H1);
 private:
 	 CDoorway();
 	DECLARE_SERIAL(CDoorway)
 protected:
 	TTaskResult Calibrate(CRobot* Robot, robPOINT* DPosition, double* DHeading);
 };
 
 const int cDefaultWallBuffer = 55;
 
 class CCornerWall : public CWall {
 public:
 	void SetWallBuffer(int WallBuffer);
 	int GetWallBuffer(void);
 	TTaskResult Adjust(CRobot *Robot);
 	void operator=(CCornerWall Corner);
 	bool Create(CRobot* Robot, bool WallToLeft, int WallBuffer);
 	void Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbGrid);
 	void Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid);
 	robPOINT m_CalPoint3;
 	unsigned char m_FeelingMotor;
 	void Serialize(CArchive & archive);
 	CCornerWall(robPOINT P0, robPOINT P2, double H0, double H2);
 	robPOINT m_P2;
 	double m_Heading2;
 	CCornerWall();
 	CCornerWall(CCornerWall& Wall);
 private:
 	DECLARE_SERIAL(CCornerWall)
 protected:
 	int m_WallBuffer;
 	bool CreateCorner(CRobot* Robot, bool WallToLeft, int WallBuffer);
 	static LPDIRECTDRAWSURFACE3 m_lpDDSGreyArrowBmp;
 	static float m_OldGreyArrowZoomLevel;
 	TTaskResult CalibrateCorner(CRobot * Robot, robPOINT* DPosition, double* DHeading);
 	TTaskResult Calibrate(CRobot* Robot, robPOINT* DPosition, double* DHeading);
 	void CalculateBounds(void);
 };
 
 #endif