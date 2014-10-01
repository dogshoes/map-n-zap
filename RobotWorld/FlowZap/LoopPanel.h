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
 // LoopPanel.h: interface for the CLoopPanel class.
 //
 //////////////////////////////////////////////////////////////////////
 
 #if !defined(AFX_LOOPPANEL_H__B78FAF82_7793_11D1_AC8C_0040F6B8AE22__INCLUDED_)
 #define AFX_LOOPPANEL_H__B78FAF82_7793_11D1_AC8C_0040F6B8AE22__INCLUDED_
 
 #if _MSC_VER >= 1000
 #pragma once
 #endif // _MSC_VER >= 1000
 
 #include "Zone.h"
 class CInstruction;
 class CLoop; 
 
 class CLoopPanel : public CZone  
 {
 public:
 	void EmptyList();
 	virtual void UpdateLoopLevels()=0;
 	CLoop* m_LoopPtr;
 	CInstruction* m_oldPtr;
 	CInstruction* m_ptr;
 	void Paint (CPaintDC *dc);
 	void RemoveLoop (CInstruction *in);
 	virtual CLoop* AddLoop(CInstruction* start, CInstruction* end) = 0;
 	CObList m_Loops;
 	CLoopPanel();
 	virtual ~CLoopPanel();
 
 };
 
 #endif // !defined(AFX_LOOPPANEL_H__B78FAF82_7793_11D1_AC8C_0040F6B8AE22__INCLUDED_)
