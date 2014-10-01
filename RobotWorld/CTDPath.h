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
 #include "RothMath.h"
 
 
 class CCTDPath {
 //Connect the dot robot path implementation class.
 public:
 	CCTDPath(CCTDPath& CTDPath);
 	unsigned short FindClosestPoint(robPOINT Point, unsigned short StartPosition, unsigned short NumPositionsToSearch);
 	unsigned short FindClosestPoint(robPOINT Point);
 	void SetSpeed(short Speed);
 	typedef enum TPathSpeed {ps100, ps150, ps200, ps250, ps300};
 	unsigned short GetSpeed(void);
 	bool AddArc(robPOINT start, robPOINT end, robPOINT OnArc);
 	 ~CCTDPath();
 	unsigned short m_Size;
 	unsigned short m_PathPosition;
 	CCTDPath(unsigned int Speed = 100);
 	bool AddStraightLine(robPOINT p1, robPOINT p2);
 	bool AddPoint(robPOINT p);
 	robPOINT* m_Path;
 	char GetPointSpacing(void) {return m_PointSpacing;};
 	bool operator != (CCTDPath P2);
 	bool operator == (CCTDPath P2);
 private:
 	unsigned short m_Speed;
 	char m_PointSpacing;
 
 protected:
 
 };
 
