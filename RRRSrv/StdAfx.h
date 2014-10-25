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
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN	// Exclude rarely-used items from Windows headers.
#endif

// Modify the following defines if you have to target an OS before the ones
// specified in the following code. See MSDN for the latest information
// about corresponding values for different operating systems.
#ifndef WINVER		// Permit use of features specific to Windows 95 and Windows NT 4.0 or later.
#define WINVER 0x0400	// Change this to the appropriate value to target 
#endif                     // Windows 98 and Windows 2000 or later.

#ifndef _WIN32_WINNT	// Permit use of features specific to Windows NT 4.0 or later.
#define _WIN32_WINNT 0x0400	// Change this to the appropriate value to target 
#endif		         // Windows 98 and Windows 2000 or later.			

#ifndef _WIN32_WINDOWS	      // Permit use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410    // Change this to the appropriate value to target 
#endif			      // Windows Millennium Edition or later.

#ifndef _WIN32_IE		// Permit use of features specific to Internet Explorer 4.0 or later.
#define _WIN32_IE 0x0400   // Change this to the appropriate value to target 
#endif			// Internet Explorer 5.0 or later.

// Turns off MFC feature that hides of some common warning messages
// that are frequently and safely ignored .
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC OLE automation classes
#include <afxmt.h>          // MFC multithreading support
#include <afxtempl.h>       // MFC template classes
#include <mmsystem.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT




