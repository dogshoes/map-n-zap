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
  // Zone.cpp: implementation of the CZone class.
  //
  //////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "FlowZap.h"
  #include "Zone.h"

  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////
  
  CZone::CZone()
  {
  
  }
  
  CZone::~CZone()
  {
  	m_Parent = NULL;
  }
  
  
  
  BOOL CZone::Contains(CPoint p){
  /************************************************************************************************
  * CREATED: 1/21/98 1:19PM
  *
  * LAST MODIFIED: 6/16/98 1:40PM
  *
  * PARAMETERS
  *	p	the point we are checking
  * RETURN
  *	BOOL	TRUE if the point is inside this zone
  ************************************************************************************************
  * DESCRIPTION
  *	determines if the mouse click (point) is inside this boundaries of this zone
  * METHOD:
  *
  ************************************************************************************************/
  	int i=1;
  	return m_Loc.PtInRect(p);
  }
  
  
  
  
  void CZone::UpdateRectangle(int pLeft, int pTop, int pRight, int pBottom)
  {
  	if (pLeft   != dZONE_NOCHANGE) m_Loc.left	= pLeft;
  	if (pTop    != dZONE_NOCHANGE) m_Loc.top	= pTop;
  	if (pRight  != dZONE_NOCHANGE) m_Loc.right	= pRight;
  	if (pBottom != dZONE_NOCHANGE) m_Loc.bottom = pBottom;
  }
  
  int CZone::GetWidth()
  {
  	return m_Loc.right - m_Loc.left;
  }
