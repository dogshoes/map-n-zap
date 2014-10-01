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
  #include "stdafx.h"
  #include <winuser.h>
  #include <iomanip.h>
  #include "rothutil.h"
  
  CPoint GetDesktopSize(void) {
  	CPoint Dim;
  #if(WINVER >= 0x0500)
  	Dim.x = GetSystemMetrics(SM_CXVIRTUALSCREEN);
  	if (Dim.x <= 0) {
  		Dim.x = GetSystemMetrics(SM_CXSCREEN);
  		Dim.y = GetSystemMetrics(SM_CYSCREEN);
  	} else {
  		Dim.y = GetSystemMetrics(SM_CYVIRTUALSCREEN);
  	}
  #else
  	Dim.x = GetSystemMetrics(SM_CXSCREEN);
  	Dim.y = GetSystemMetrics(SM_CYSCREEN);
  #endif
  	return Dim;
  }
  
  void PrintTimeStamp(ofstream stream) {
  	SYSTEMTIME TimeStruct;
  	GetSystemTime(&TimeStruct);
  	stream << setw(2) << setfill('0') << TimeStruct.wHour << ':';
  	stream << setw(2) << setfill('0') << TimeStruct.wMinute << ':';
  	stream << setw(2) << setfill('0') << TimeStruct.wSecond << '.';
  	stream << setw(3) << setfill('0') << TimeStruct.wMilliseconds << ' ';
  	return;
  }
  
  void PrintTimeStamp(ostream_withassign stream) {
  	SYSTEMTIME TimeStruct;
  	GetSystemTime(&TimeStruct);
  	stream << setw(2) << setfill('0') << TimeStruct.wHour << ':';
  	stream << setw(2) << setfill('0') << TimeStruct.wMinute << ':';
  	stream << setw(2) << setfill('0') << TimeStruct.wSecond << '.';
  	stream << setw(3) << setfill('0') << TimeStruct.wMilliseconds << ' ';
  	return;
  }
  
  CString TimeStampString() {
  	CString Stamp("00:00:00.000");
  	SYSTEMTIME TimeStruct;
  	GetSystemTime(&TimeStruct);
  	Stamp.Format("%2d:%2d:%2d.%d", TimeStruct.wHour, TimeStruct.wMinute, TimeStruct.wSecond, TimeStruct.wMilliseconds);
  	Stamp.Replace(' ', '0');
  	return Stamp;
  }