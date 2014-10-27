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
///////////////////////////////////////////////////////////////////////////////
//
// Module:     RRRPath.cpp
//
// Abstract:   Robot path OLE object.
//
// Author:     J.C.H
//
// Revisions:
//    $History: RRRPath.cpp $
//
// *****************  Version 4  *****************
// User: Harnden      Date: 3/07/99    Time: 2:12p
// Updated in $/RRR/RRRCom
// Changed name to CyeCom.ocx.
//
// *****************  Version 3  *****************
// User: Harnden      Date: 5/26/98    Time: 10:51p
// Updated in $/RRR/RRRCom
// First radio interface release sent to Cycle Time Corp.
//
// *****************  Version 2  *****************
// User: Harnden      Date: 1/03/98    Time: 5:58p
// Updated in $/RRRCom
// Completion of all ActiveX method interfaces for RobotWorld.
//
// *****************  Version 1  *****************
// User: Harnden      Date: 12/29/97   Time: 6:24p
// Created in $/RRRCom
//
// Copyright 1997 Cycle Time Corp
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RRRCom.h"
#include "RRRPath.h"
#include "RRRConst.h"
#include "RRRDispID.h"

/////////////////////////////////////////////////////////////////////////////
// RRRPath

IMPLEMENT_DYNCREATE(RRRPath, CCmdTarget)

RRRPath::RRRPath()
{
    EnableAutomation();

    // To keep the application running as long as an OLE automation
    //	object is active, the constructor calls AfxOleLockApp.

    AfxOleLockApp();

    // set number of points in path to zero
    m_NumPoints = 0;
}

RRRPath::~RRRPath()
{
    // To terminate the application when all objects created with
    // 	with OLE automation, the destructor calls AfxOleUnlockApp.

    AfxOleUnlockApp();
}

void RRRPath::OnFinalRelease()
{
    // When the last reference for an automation object is released
    // OnFinalRelease is called.  The base class will automatically
    // deletes the object.  Add additional cleanup required for your
    // object before calling the base class.

    CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(RRRPath, CCmdTarget)
    //{{AFX_MSG_MAP(RRRPath)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(RRRPath, CCmdTarget)
    //{{AFX_DISPATCH_MAP(RRRPath)
    //}}AFX_DISPATCH_MAP
    DISP_PROPERTY_EX_ID(RRRPath, "NumPoints", RRRPTH_NUMPOINTS, GetNumPoints, SetNumPoints, VT_I2)
    DISP_PROPERTY_PARAM_ID(RRRPath, "x", RRRPTH_X, GetX, SetX, VT_R8, VTS_I2)
    DISP_PROPERTY_PARAM_ID(RRRPath, "y", RRRPTH_Y, GetY, SetY, VT_R8, VTS_I2)
END_DISPATCH_MAP()

// Note: we add support for IID_IRRRPath to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {723BB6A6-8075-11D1-BCC1-444553540000}
static const IID IID_ICyePath = { 0x723bb6a6, 0x8075, 0x11d1, { 0xbc, 0xc1, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };

BEGIN_INTERFACE_MAP(RRRPath, CCmdTarget)
INTERFACE_PART(RRRPath, IID_ICyePath, Dispatch)
END_INTERFACE_MAP()

// {723BB6A7-8075-11D1-BCC1-444553540000}
IMPLEMENT_OLECREATE(RRRPath, "CyeCom.Path", 0x723bb6a7, 0x8075, 0x11d1, 0xbc, 0xc1, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0)

BOOL RRRPath::ValidIndex(short Index)
{
    BOOL IndexOk = FALSE;

    if ((Index < 0) || (Index >= m_NumPoints))
    {
        AfxThrowOleDispatchException(INDEX_OUT_OF_RANGE, "Invalid index passed to RRRCom.Path object.");
    }
    else
    {
        IndexOk = TRUE;
    }

    return IndexOk;
}

/////////////////////////////////////////////////////////////////////////////
// RRRPath message handlers

double RRRPath::GetX(short Index)
{
    if (ValidIndex(Index))
    {
        return m_x[Index];
    }
    else
    {
        return 0.0;
    }
}

void RRRPath::SetX(short Index, double newValue)
{
    if (ValidIndex(Index))
    {
        m_x[Index] = newValue;
    }
}

double RRRPath::GetY(short Index)
{
    if (ValidIndex(Index))
    {
        return m_y[Index];
    }
    else
    {
        return 0.0;
    }
}

void RRRPath::SetY(short Index, double newValue)
{
    if (ValidIndex(Index))
    {
        m_y[Index] = newValue;
    }
}

short RRRPath::GetNumPoints()
{
    return m_NumPoints;
}

void RRRPath::SetNumPoints(short nNewValue)
{
    if (nNewValue > 0)
    {
        m_x.SetSize(nNewValue);
        m_y.SetSize(nNewValue);

        // is the array being increased?
        if (nNewValue > m_NumPoints)
        {
            // yes set the new values to zero
            for (short i = m_NumPoints; i < nNewValue; i++)
            {
                m_x[i] = 0.0;
                m_y[i] = 0.0;
            }
        }

        m_NumPoints = nNewValue;
    }
}