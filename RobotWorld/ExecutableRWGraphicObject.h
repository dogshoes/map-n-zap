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
// ExecutableRWGraphicObject.h: interface for the CExecutableRWGraphicObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXECUTABLERWGRAPHICOBJECT_H__7F1004C1_C3DD_11D1_B231_0040F6B87672__INCLUDED_)
#define AFX_EXECUTABLERWGRAPHICOBJECT_H__7F1004C1_C3DD_11D1_B231_0040F6B87672__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RWGraphicObject.h"
#include "RothMath.h"
#include "PathList.h"

class CRobot;
class CPathList;
class CExecutableRWGraphicObjectList;
class CRobotWorldView;
class CGoalSet;

const float cCloseDistance = 12.0f * cTPI;
const int cMinNumberOfClaps = 2;
const int cMaxNumberOfClaps = 5;

class CExecutableRWGraphicObject : public CRWGraphicObject
{
    public:
        void Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbabilityGrid);
        void SetNumberOfClaps(int Claps);
        int GetNumberOfClaps(void);
        virtual double DistanceToEntryPoint(robPOINT From);
        virtual Path* CreatePathToEntryPoint(robPOINT From, CRobot* Robot);
        virtual CGoalSet* FindPathToEntryPoint(robPOINT From, CRobot* Robot, bool& GoodPath);
        CExecutableRWGraphicObject(CExecutableRWGraphicObject &ExecutableRWGraphicObject);
        void OnDefaultClick(robPOINT point, float WorldPointsPerPixel, CRobot* Robot);
        virtual void SetExitPoint(robPOINT ExitPoint);
        bool InsideObject(robPOINT point, float WorldPointsPerPixel);
        virtual void SetEntryPoint(robPOINT Point);
        void OnErase(void);
        void SetExecutableObjectList(CExecutableRWGraphicObjectList* ObjectList);
        virtual void SetCaption(CString Caption);
        virtual void Serialize(CArchive & archive);
        CExecutableRWGraphicObject();
        virtual ~CExecutableRWGraphicObject();
        virtual robPOINT GetEntryPoint(void)
        {
            return m_EntryPoint;
        };
        virtual robPOINT GetExitPoint(void)
        {
            return m_ExitPoint;
        };
        virtual TTaskResult Execute(CRobot* Robot, short Speed = 100);
        virtual bool InsideExecuteZone(robPOINT point, float WorldPointsPerPixel);
        virtual TTaskResult SpecialFunction(CRobot* Robot, short Speed);
    private:
        DECLARE_SERIAL(CExecutableRWGraphicObject)
        TTaskResult ExecutePathToEntryPoint(CRobot * Robot, short Speed);
        TTaskResult ExecuteFoundPath(Path* FoundPath, CRobot *Robot, short Speed, bool& GoodPath, bool Forward = true);
    protected:
        //	CPathList* m_PathList;
        robPOINT m_ExitPoint;
        CExecutableRWGraphicObjectList* m_ExecutableObjectList;
        robPOINT m_EntryPoint;
        int m_NumberOfClaps;
};

class CExecutableRWGraphicObjectListNode
{
    public:
        ~CExecutableRWGraphicObjectListNode()
        {
            return;
        };
        CExecutableRWGraphicObject* m_Object;
        CExecutableRWGraphicObjectListNode* m_Next;
    private:
    protected:

};

class CCalibrationZone;
class CLinePath;
class CHomeBase;

class CExecutableRWGraphicObjectList
{
    public:
        CExecutableRWGraphicObjectListNode* GetHead(void)
        {
            return m_Head;
        }
        CExecutableRWGraphicObject* GetClosestObject(robPOINT Position, CRuntimeClass* ObjectType);
        CHomeBase* GetClosestHomeBase(robPOINT Position);
        CExecutableRWGraphicObject* GetObjectWithNumberOfClaps(int NumberOfClaps);
        CString ObjectTypeToString(TRWGraphicObjectType ObjectType);
        void GetNumberOfClapsUsedList(CString ObjectType[cMaxNumberOfClaps], CString ObjectName[cMaxNumberOfClaps]);
        void RemoveFromStoredPaths(robPOINT Point);
        void RemoveFromStoredPaths(Path *RemovePath);
        void AddToStoredPaths(Path* StorePath);
        Path* FindPathBetween(robPOINT Start, CExecutableRWGraphicObject *End, bool& Forward);
        CLinePath* GetLinePathBetween(robPOINT Start, CExecutableRWGraphicObject *Destination);
        Path* GetPathBetween(robPOINT P1, robPOINT P2, bool& Forward);
        bool GetShowCaptions(void)
        {
            return m_ShowCaptions;
        };
        void ShowCaptions(bool Show);
        CCalibrationZone* FindClosestCalibrationZone(robPOINT Position);
        unsigned int Find3ClosestCalibrationZones(robPOINT Position, CCalibrationZone* CalZonesList[3]);
        CExecutableRWGraphicObject* PointNearExecutableObjectExit(robPOINT Point, CExecutableRWGraphicObject* Exclude = NULL);
        void Purge(void);
        void Remove(CExecutableRWGraphicObject * GraphicObject);
        unsigned int NumberOfNodes();
        CExecutableRWGraphicObjectList()
        {
            m_Head = NULL;
            m_ProbabilityGrid = NULL;
            m_ShowCaptions = true;
        };
        ~CExecutableRWGraphicObjectList()
        {
            Purge();
        };
        void Serialize(CArchive & archive);
        void SerializeV2(CArchive & archive);
        void GetCaptionLists(CStringList * PositionCaptions, CStringList * CalZoneCaptions, CStringList* VacuumPatchCaptions, CStringList* LinePathCaptions);
        CExecutableRWGraphicObject* FindObject(CString Caption, TRWGraphicObjectType ObjectType);
        CProbabilityGrid* m_ProbabilityGrid;
        void Add(CExecutableRWGraphicObject* New, bool NotifyParent = true);
        void GetUniqueCaption(CString * Caption, CExecutableRWGraphicObject* Object);
        void RemoveObjectFromStoredPaths(CExecutableRWGraphicObject * Object);
        CExecutableRWGraphicObject* PointInsideExecutableArea(robPOINT Point, float WorldPointsPerPixel);
        // added by Parag - 10/4/99
        CExecutableRWGraphicObjectListNode* Head(void)
        {
            return m_Head;
        }
    private:
        CPathList m_StoredPathsList;
    protected:
        CExecutableRWGraphicObject* GetClosestObject(robPOINT Position, TRWGraphicObjectType ObjectType);
        bool m_ShowCaptions;
        CExecutableRWGraphicObjectListNode* m_Head;
};


#endif // !defined(AFX_EXECUTABLERWGRAPHICOBJECT_H__7F1004C1_C3DD_11D1_B231_0040F6B87672__INCLUDED_)
