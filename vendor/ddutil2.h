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
 /*==========================================================================
  *
  *  Copyright (C) 1995 Microsoft Corporation. All Rights Reserved.
  *
  *  File:       ddutil.cpp
  *  Content:    Routines for loading bitmap and palettes from resources
  *
  ***************************************************************************/
 
 #ifdef __cplusplus
 extern "C" {            /* Assume C declarations for C++ */
 #endif	/* __cplusplus */
 
 extern IDirectDrawPalette * DDLoadPalette2(IDirectDraw2 *pdd, LPCSTR szBitmap);
 extern IDirectDrawSurface3 * DDLoadBitmap2(IDirectDraw2 *pdd, LPCSTR szBitmap, int dx, int dy);
 extern HRESULT              DDReLoadBitmap2(IDirectDrawSurface3 *pdds, LPCSTR szBitmap);
 extern HRESULT              DDCopyBitmap2(IDirectDrawSurface3 *pdds, HBITMAP hbm, int x, int y, int dx, int dy);
 extern DWORD                DDColorMatch2(IDirectDrawSurface3 *pdds, COLORREF rgb);
 extern HRESULT              DDSetColorKey2(IDirectDrawSurface3 *pdds, COLORREF rgb);
 
 #ifdef __cplusplus
 }
 #endif	/* __cplusplus */
