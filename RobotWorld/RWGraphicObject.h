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
 #ifndef RWGRAPHICOBJECT234jkl346hdsfgasdf80965kjhdfg908345kjhdrfg
 
 #define RWGRAPHICOBJECT234jkl346hdsfgasdf80965kjhdfg908345kjhdrfg
 
 #include "RothMath.h"
 #include "Robot.h"
 #include "..\vendor\ddraw.h"
 
 enum TEditMode {emMove, emResize, emSelected, emNone};
 enum TFrameSide {fsNone = 0, fsTop, fsBot, fsLft, fsRgt, fsTopLft, fsTopRgt, fsBotLft, fsBotRgt};
 enum TRWGraphicObjectType {goNoObject, goVacuumPatch, goRedZone, goCalibrationZone, goPosition, goLinePath, goDisplayPath, goFinishLine, goStartLine};
 
 class CProbabilityGrid;
 class CRWGraphicObjectList;
 class CPathDisplay;
 class CRulerGrid;
 
 class CRWGraphicObject : public CObject
 {
 public:
 	CRWGraphicObject(CRWGraphicObject &RWGraphicObject);
 	bool InsideUpdateRectangle(CRect ProbGridUpdateRect, CProbabilityGrid *ProbGrid);
 //	virtual void DrawTracker(CDC* pDC);
 	void SetAnimate(bool Animate);
 	bool GetAnimate(void) {return m_Animate;};
 	void SetAnimationFrame(int Frame) {if (Frame < 0) Frame = 0; if (Frame >= GetNumberAnimationFrames()) Frame = GetNumberAnimationFrames()-1; m_AnimationFrame = Frame;};
 	int GetAnimationFrame(void) {return m_AnimationFrame;};
 	virtual int GetNumberAnimationFrames(void) {return 0;};
 	virtual void Animate(void);
 	virtual void OnEditClick(robPOINT point, CRobotWorldView* View);
 	virtual void SetCaption(CString Caption) {m_Caption = Caption;};
 	virtual void SetShowCaption(bool Visible) {m_ShowCaption = Visible;};
 	virtual bool GetShowCaption(void) {return m_ShowCaption;};
 	CString GetCaption(void);
 	void Redraw(bool UpdateNow = true);
 	CRobotWorldView* m_ParentView;
 	virtual unsigned int GetZLayer(void) {return 0;};
 	virtual void OnDefaultClick(robPOINT point, float WorldPointsPerPixel, CRobot* Robot);
 	virtual robRECT GetBounds(void);
 	virtual void OnErase(void);
 	BOOL GetVisible(void) {return m_IsVisible;};
 	virtual void SetVisible(BOOL Visible) {m_IsVisible = Visible;};
 	virtual void EditProperties(void) {return;};
 	virtual unsigned short MinWidth(void) {return 0;};
 	virtual unsigned short MinHeight(void) {return 0;};
 	virtual void SetBounds(robRECT bounds);
 	virtual void SetEditMode(TEditMode EditMode) {m_EditMode = EditMode;};
 	TEditMode GetEditMode(void) {return m_EditMode;};
 //	virtual void Resize(TFrameSide DragSide, robPOINT point);
 	virtual void Move(robPOINT TopCorner);
 	virtual bool InsideObject(robPOINT point, float WorldPointsPerPixel);
 	virtual void Draw(CRect ProbabilityGridUpdateRect, CDC* pDC, CProbabilityGrid* ProbGrid);
 	virtual void Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid) {return;};
 	virtual void DrawObject(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid);
 	virtual ~CRWGraphicObject();
 	virtual TRWGraphicObjectType GetObjectType(void) {return goNoObject;};
 	virtual void Serialize(CArchive & archive);
 	virtual BOOL SetTrackerCursor(CRobotWorldView* pWnd, UINT nHitTest);
 private:
 	DECLARE_SERIAL(CRWGraphicObject)
 	UINT m_ArchiveVersionNumber;
 protected:
 	UINT GetArchiveVersionNumber(void) {
 #ifdef _DEBUG
 		ASSERT(m_ArchiveVersionNumber != UINT(-1));
 #endif
 		return m_ArchiveVersionNumber;
 	};
 	int m_AnimationFrame;
 	bool m_Animate;
 	TEditMode m_EditMode;
 //	CRectTracker m_Tracker;
 	virtual CRectTracker SetupTracker(CRobotWorldView* View);
 	virtual CRectTracker SetupTracker(CProbabilityGrid* ProbabilityGrid, CPoint SurfaceOffset);
 	bool m_ShowCaption;
 	robRECT m_CaptionBounds;
 	CString m_Caption;
 	CRWGraphicObject();
 	robRECT m_Bounds;
 	BOOL m_IsVisible;
 };
 
 class CRWGraphicObjectListNode
 {
 public:
 	 CRWGraphicObjectListNode();
 	 ~CRWGraphicObjectListNode();
 	CRWGraphicObject* m_Object;
 	CRWGraphicObjectListNode* m_Next;
 	CRWGraphicObjectListNode* m_Previous;
 private:
 protected:
 
 };
 
 //class CExecutableRWGraphicObject;
 
 const int nZLayers = 256;
 
 class CRWGraphicObjectList : public CObject
 {
 public:
 	void AnimateObjects(void);
 	void UpdateFinishLineTimers(void);
 	void UpdateStartAndFinishLines(robPOINT RobotPosition, float WorldPointsPerPixel);
 	float GetRulerGridSpacing(void);
 	void SetRulerGridSpacing(float Spacing);
 	bool GetShowRulerGrid(void);
 	void SetShowRulerGrid(bool Show);
 	void SetRobotCurrentPath(CCTDPath* CurrentPath);
 	CPathDisplay* m_RobotCurrentPath;
 	unsigned int NumberOfNodes(void);
 	CProbabilityGrid* m_ProbabilityGrid;
 	CRWGraphicObjectListNode* GetTail(unsigned int ZLayer);
 	void MoveToHead(CRWGraphicObjectListNode * Node);
 	void MoveToTail(CRWGraphicObjectListNode* Node);
 	bool GetShowLinePaths();
 	bool GetShowVacuumPatches(void);
 	bool GetShowRedZones(void);
 	bool GetShowPositionLabels(void);
 	bool GetShowCalibrationZones(void);
 	void ShowObjects(TRWGraphicObjectType ObjectType, bool Show);
 	unsigned int NumberOfNodesInLayer(unsigned int Z);
 	void SerializeV2(CArchive & archive);
 	void Serialize(CArchive & archive);
 	CRWGraphicObject* PointInsideObject(robPOINT RobotPoint, float WorldPointsPerPixel);
 	void DrawObjects(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbabilityGrid);
 	~CRWGraphicObjectList();
 	CRWGraphicObjectList();
 	void Purge(void);
 	void Remove(CRWGraphicObject* RWGraphicObject);
 	void Add(CRWGraphicObject* New);
 private:
 	DECLARE_SERIAL(CRWGraphicObjectList)
 	CRulerGrid* m_RulerGrid;
 	bool m_ShowPositionLabels;
 	bool m_ShowCalibrationZones;
 	bool m_ShowVacuumPatches;
 	bool m_ShowRedZones;
 	bool m_ShowLinePaths;
 	bool m_RobotWasInsideStartLine;
 protected:
 	CRWGraphicObjectListNode* m_Layers[nZLayers];
 	CRWGraphicObjectListNode* m_Head;
 };
 
 #endif
