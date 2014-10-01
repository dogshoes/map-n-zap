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
 #ifndef VACUUMPATCH239084sadfklnlkjq4309u8
 
 #define VACUUMPATCH239084sadfklnlkjq4309u8
 
 #include "rothmath.h"
 #include "ExecutableRWGraphicObject.h"
 #include "Robot.h"
 
 enum TEnterSide {esTopLft, esTopRgt, esBotLft, esBotRgt, esLftTop, esLftBot, esRgtTop, esRgtBot};
 
 struct TVacPath;
 
 class CVacuumPatch : public CExecutableRWGraphicObject
 {
 public:
 	unsigned int GetZLayer(void) {return 50;};
 	robPOINT GetStartPoint(void) {return m_EntryPoint;};
 	void Serialize(CArchive & archive);
 	TRWGraphicObjectType GetObjectType(void) {return goVacuumPatch;};
 	 ~CVacuumPatch();
 	void EditProperties(void);
 	void SetBounds(robRECT PatchBounds);
 
 	void Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbabilityGrid);
 	unsigned short m_NumberPaths;
 	void Resize(TFrameSide FrameSide, robPOINT point);
 	void Move(robPOINT TopCorner);
 	CVacuumPatch(robRECT PatchBounds, TEnterSide EnterSide, unsigned short PathWidth, unsigned short TurnDiameter, CExecutableRWGraphicObjectList* ExecutableObjectList);
 	TEnterSide m_EnterSide;
 	double m_HeadForward;
 	double m_HeadNext;
 	unsigned short m_PathWidth;
 	unsigned short m_TurnDiameter;
 	unsigned short MinWidth(void);
 	unsigned short MinHeight(void);
 	TTaskResult SpecialFunction(CRobot* Robot, short Speed);
 	void SetVacuumingSpeed(short Speed);
 	short GetVacuumingSpeed(void) {return m_VacuumingSpeed;};
 private:
 	void AddArcToPath(robPOINT P1, robPOINT P2, robPOINT P3, CCTDPath** RobotPath, CRobot* Robot);
 	void AddStraightLineToPath(robPOINT P1, robPOINT P2, CCTDPath** RobotPath, CRobot* Robot);
 	DECLARE_SERIAL(CVacuumPatch)
 	void CalculateVacuumPaths(void);
 	unsigned short m_VacuumLength;
 	void SetEntryPointAndHeadings(void);
 protected:
 	short m_VacuumingSpeed;
 	typedef struct {
 		POINT p1, p2, p3;
 		short TurnDiameter;
 		robPOINT Arc1s;		/*Starting point of Arc 1*/
 		robPOINT Arc1e;		/*Ending point of Arc 1*/
 		robPOINT Arc1m;		/*third point on Arc 1*/
 		robPOINT Arc2s;		/*Starting point of Arc 2*/
 		robPOINT Arc2e;		/*Ending point of Arc 2*/
 		robPOINT Arc2m;		/*third point on Arc 2*/
 		robPOINT Arc2c;		/*Center of Arc 2*/
 		double Arc2r;		/*Radius of Arc 2*/
 	} TVacPath;
 	TVacPath* m_VacuumPath;
 	CVacuumPatch();
 };
 #endif