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
// FinishLine.h: interface for the CFinishLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FINISHLINE_H__4B3226A2_D374_11D1_B231_0040F6B87672__INCLUDED_)
#define AFX_FINISHLINE_H__4B3226A2_D374_11D1_B231_0040F6B87672__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ExecutableRWGraphicObject.h"
#include "Rothmath.h"

class CFinishLine : public CExecutableRWGraphicObject
{
    public:
        void Serialize(CArchive & archive);
        int GetNumberAnimationFrames(void)
        {
            return 10;
        };
        void Draw(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid* ProbGrid);
        void SetBounds(robRECT Bounds);
        unsigned int GetZLayer(void)
        {
            return 100;
        };
        unsigned short MinHeight(void)
        {
            return 165;
        };
        unsigned short MinWidth(void)
        {
            return 165;
        };
        void UpdateTime(void);
        void OnErase(void);
        bool GetRunning(void);
        void SetFinishTime(void);
        void SetStartTime(void);
        void SetCaption(float Seconds);
        void SetCaption(unsigned char Minutes, float Seconds);
        void OnDefaultClick(robPOINT point, float WorldPointsPerPixel, CRobot* Robot);
        TRWGraphicObjectType GetObjectType(void)
        {
            return goFinishLine;
        };
        void Draw(CRect ProbGridBmpUpdateRect, CDC* pDC, CProbabilityGrid* ProbabilityGrid);
        CFinishLine();
        virtual ~CFinishLine();
        void SetTimerPosition(short Position)
        {
            (Position >= 3) ? (m_TimerPosition = 3) : (m_TimerPosition = Position);
        };
        short GetTimerPosition(void)
        {
            return m_TimerPosition;
        };
    protected:
        static DWORD m_TransparencyColor;
        static LPDIRECTDRAWSURFACE3 m_lpddsAnimationBmps1[8];
        static LPDIRECTDRAWSURFACE3 m_lpddsAnimationBmps2[8];
        static LPDIRECTDRAWSURFACE3 m_lpddsAnimationBmps3[8];
        static LPDIRECTDRAWSURFACE3 m_lpddsTimerBmps[4];
        void DrawFinish(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid *ProbGrid);
        void DrawTimerBox(CRect ProbabilityGridUpdateRect, LPDIRECTDRAWSURFACE3 BackBuffer, CProbabilityGrid *ProbGrid);
        LPDIRECTDRAWSURFACE3 GetFinishBitmap(float ZoomLevel);
        void LoadBitmaps(CProbabilityGrid *ProbGrid);

        short m_TimerPosition;
        short m_StartMilliseconds;
        void SetRunning(bool Running);
        void SetCaption(CString Caption);
        bool m_Running;
        CTime m_StartTime;

    private:
        short GetTimerBoxHeight(float ZoomLevel);
        short GetTimerBoxWidth(float ZoomLevel);
        DECLARE_SERIAL(CFinishLine)
};

#endif // !defined(AFX_FINISHLINE_H__4B3226A2_D374_11D1_B231_0040F6B87672__INCLUDED_)
