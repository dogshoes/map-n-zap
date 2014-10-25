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
/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (C) 1992 - 1997 Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/
/****************************************************************************
 *
 *   vidframe.h: Frame for capture window
 *
 *   Vidcap32 Source code
 *
 ***************************************************************************/

/*
 * interface to vidframe window class - this window class creates a child
 * AVICAP window, and is responsible for positioning it within the vidframe
 * window, framing it, and scrolling it if it will not fit. Call
 * vidframeCreate to create the window, and vidframeLayout if the
 * video image size might have changed.
 */



/*
 * create a frame window and child capture window at the
 * given location. Initialise the class if this is the
 * first time through.
 *
 * returns the window handle of the frame window
 * (or NULL if failure). returns the window handle of the AVICAP window
 * via phwndCap.
 */
HWND vidframeCreate(
    HWND hwndParent,
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    int x,
    int y,
    int cx,
    int cy,
    HWND * phwndCap
);


/*
 * layout the window  - decide if we need scrollbars or
 * not, and position the avicap window correctly
 *
 * called if the size of the vidframe window changes or can be
 * called externally whenever the video size might have changed.
 */
void vidframeLayout(HWND hwndFrame, HWND hwndCap);

/*
 * change the background fill brush to be one of-
 *  IDD_PrefsDefBackground  - windows default background colour
 *  IDD_PrefsLtGrey - light grey
 *  IDD_PrefsDkGrey - dark grey
 *  IDD_PrefsBlack - black
 */
void vidframeSetBrush(HWND hwnd, int iPref);
