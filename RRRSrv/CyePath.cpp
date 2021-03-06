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
// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "CyePath.h"

/////////////////////////////////////////////////////////////////////////////
// ICyePath properties

short ICyePath::GetNumPoints()
{
    short result;

    // This call dispatches over to the RRRPath class in the RRRCom project.
    // See mapping defined in RRRCom/CyeCom.odl.
    GetProperty(0x64, VT_I2, (void*)&result);

    return result;
}

void ICyePath::SetNumPoints(short propVal)
{
    // This call dispatches over to the RRRPath class in the RRRCom project.
    // See mapping defined in RRRCom/CyeCom.odl.
    SetProperty(0x64, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// ICyePath operations

double ICyePath::GetX(short Index)
{
    double result;
    static BYTE parms[] = VTS_I2;

    // This call dispatches over to the RRRPath class in the RRRCom project.
    // See mapping defined in RRRCom/CyeCom.odl.
    InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, Index);

    return result;
}

void ICyePath::SetX(short Index, double newValue)
{
    static BYTE parms[] = VTS_I2 VTS_R8;

    // This call dispatches over to the RRRPath class in the RRRCom project.
    // See mapping defined in RRRCom/CyeCom.odl.
    InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Index, newValue);
}

double ICyePath::GetY(short Index)
{
    double result;
    static BYTE parms[] = VTS_I2;

    // This call dispatches over to the RRRPath class in the RRRCom project.
    // See mapping defined in RRRCom/CyeCom.odl.
    InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, Index);

    return result;
}

void ICyePath::SetY(short Index, double newValue)
{
    static BYTE parms[] = VTS_I2 VTS_R8;

    // This call dispatches over to the RRRPath class in the RRRCom project.
    // See mapping defined in RRRCom/CyeCom.odl.
    InvokeHelper(0x66, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Index, newValue);
}