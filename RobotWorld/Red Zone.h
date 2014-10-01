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
 #if !defined(AFX_REDZONE_H__0DD572E1_F76E_11D0_B231_FAE66F55053E__INCLUDED_)
 #define AFX_REDZONE_H__0DD572E1_F76E_11D0_B231_FAE66F55053E__INCLUDED_
 
 #include "RothMath.h"	// Added by ClassView
 #if _MSC_VER >= 1000
 #pragma once
 #endif // _MSC_VER >= 1000
 // Red Zone.h : header file
 //
 
 #include "RWGraphicObject.h"
 #include "WorldGridConsts.h"
 
 class CRedZoneList;
 
 class CRedZone : public CRWGraphicObject
 {
 public:
 	void OnEditClick(robPOINT point, CRobotWorldView * View);
 	unsigned int GetZLayer(void) {return 0;};
 	void SetRedZoneList(CRedZoneList* RedZoneList);
 	void OnErase(void);
 	unsigned short MinWidth(void) {return unsigned short(5*cTPI);};
 	unsigned short MinHeight(void) {return unsigned short(5*cTPI);};
 	void Serialize(CArchive & archive);
 	void SetProbabilityGrid(CProbabilityGrid* ProbGrid);
 	void SetBounds(robRECT bounds);
 //	void Resize(TFrameSide DragSide, robPOINT point);
 	CRedZone(robRECT Bounds, CProbabilityGrid* ProbGrid);
 	virtual ~CRedZone();
 	void Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid);
 	void Draw(CRect ProbabilityGridUpdateRect, CDC* pDC, CProbabilityGrid* ProbGrid);
 	TRWGraphicObjectType GetObjectType(void) {return goRedZone;};
 protected:
 
 private:
 	void ReplaceOldGridValues(void);
 	CRedZoneList* m_RedZoneList;
 	DECLARE_SERIAL(CRedZone)
 	unsigned int HorizontalGridSize(void) { return unsigned int((m_Bounds.right - m_Bounds.left) / cWorldGridPointSize + 1);};
 	unsigned int VerticalGridSize(void) { return unsigned int((m_Bounds.top - m_Bounds.bottom) / cWorldGridPointSize + 1);};
 	CRedZone();
 	bool m_FirstSetBoundsCall;
 	CProbabilityGrid* m_ProbabilityGrid;
 	static LPDIRECTDRAWSURFACE3 m_ZoneBitmap;
 	int* m_RightOldGridValues;
 	int* m_LeftOldGridValues;
 	int* m_BottomOldGridValues;
 	int* m_TopOldGridValues;
 };
 
 class CRedZoneListNode
 {
 public:
 	~CRedZoneListNode();
 	CRedZone* m_RedZone;
 	CRedZoneListNode* m_Next;
 	CRedZoneListNode* m_Previous;
 protected:
 
 private:
 
 };
 
 class CRobotWorldView;
 
 class CRedZoneList : public CObject
 {
 public:
 	unsigned int NumberOfNodes(void);
 	void SerializeV2(CArchive & archive);
 	void Serialize(CArchive& archive);
 	BOOL InRedZone(robPOINT WorldPoint);
 	CRedZone* Add(robRECT WorldRect);
 	void Add(CRedZone* RedZone);
 	void Purge(void);
 	CRedZoneList(CProbabilityGrid* ProbGrid);
 	~CRedZoneList();
 	void Remove(CRedZone* RedZone);
 	CRedZoneListNode* m_RedZoneListHead;
 protected:
 
 private:
 	CRedZoneList();
 	CProbabilityGrid* m_ProbabilityGrid;
 };
 
 /////////////////////////////////////////////////////////////////////////////
 
 //{{AFX_INSERT_LOCATION}}
 // Microsoft Developer Studio will insert additional declarations immediately before the previous line.
 
 #endif // !defined(AFX_REDZONE_H__0DD572E1_F76E_11D0_B231_FAE66F55053E__INCLUDED_)
