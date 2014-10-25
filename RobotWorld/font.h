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

/////////////////////////////////////////////////////////////////////////////
// COleFont wrapper class

class COleFont : public COleDispatchDriver
{
    public:
        COleFont() {}		// Calls COleDispatchDriver default constructor
        COleFont(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
        COleFont(const COleFont& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

        // Attributes
    public:
        CString GetName();
        void SetName(LPCTSTR);
        CY GetSize();
        void SetSize(const CY&);
        BOOL GetBold();
        void SetBold(BOOL);
        BOOL GetItalic();
        void SetItalic(BOOL);
        BOOL GetUnderline();
        void SetUnderline(BOOL);
        BOOL GetStrikethrough();
        void SetStrikethrough(BOOL);
        short GetWeight();
        void SetWeight(short);
        short GetCharset();
        void SetCharset(short);

        // Operations
    public:
};
