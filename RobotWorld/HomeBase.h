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
// HomeBase.h: interface for the CHomeBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOMEBASE_H__7810FAC1_1B09_11D2_B231_0040F6B87672__INCLUDED_)
#define AFX_HOMEBASE_H__7810FAC1_1B09_11D2_B231_0040F6B87672__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Calibration Zone.h"
#include "Robot.h"	// Added by ClassView
#include "RothMath.h"	// Added by ClassView

class CHomeBase : public CCornerWall
{
    public:
        robPOINT GetDockedPosition(void);
        bool Create(CRobot * Robot, bool WallToLeft, double Heading, bool SnapBaseToOrigin);
        TTaskResult Adjust(CRobot *Robot);
        TTaskResult Calibrate(CRobot * Robot, robPOINT* DPosition, double* DHeading);
        void OnDefaultClick(robPOINT point, float WorldPointsPerPixel, CRobot* Robot);
        TTaskResult SpecialFunction(CRobot * Robot, short Speed);
        CHomeBase();
        TTaskResult Dock(CRobot * Robot, short Speed = 100);
        virtual ~CHomeBase();
        void SetCaption(CString Caption)
        {
            CExecutableRWGraphicObject::SetCaption("Home Base");
        };
    private:
        DECLARE_SERIAL(CHomeBase);
        bool m_Docking;
    protected:
        bool CreateMainWall(CRobot *Robot, double Heading, bool SnapBaseToOrigin);

};

#endif // !defined(AFX_HOMEBASE_H__7810FAC1_1B09_11D2_B231_0040F6B87672__INCLUDED_)
