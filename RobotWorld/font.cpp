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


// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "font.h"


/////////////////////////////////////////////////////////////////////////////
// COleFont properties

CString COleFont::GetName()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void COleFont::SetName(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

CY COleFont::GetSize()
{
	CY result;
	GetProperty(0x2, VT_CY, (void*)&result);
	return result;
}

void COleFont::SetSize(const CY& propVal)
{
	SetProperty(0x2, VT_CY, &propVal);
}

BOOL COleFont::GetBold()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void COleFont::SetBold(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

BOOL COleFont::GetItalic()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void COleFont::SetItalic(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

BOOL COleFont::GetUnderline()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void COleFont::SetUnderline(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

BOOL COleFont::GetStrikethrough()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}

void COleFont::SetStrikethrough(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}

short COleFont::GetWeight()
{
	short result;
	GetProperty(0x7, VT_I2, (void*)&result);
	return result;
}

void COleFont::SetWeight(short propVal)
{
	SetProperty(0x7, VT_I2, propVal);
}

short COleFont::GetCharset()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}

void COleFont::SetCharset(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// COleFont operations
