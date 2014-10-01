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
  // Loop.cpp: implementation of the CLoop class.
  //
  //////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  //#include "FlowZap.h"
  #include "Loop.h"
  #include "Instruction.h" 
  
  #ifdef _DEBUG
  #undef THIS_FILE
  static char THIS_FILE[]=__FILE__;
  #define new DEBUG_NEW
  #endif
  
  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////
  
  CLoop::CLoop()
  {
  
  }
  
  CLoop::~CLoop()
  {
  
  }
  
  void CLoop::Draw(CPaintDC * dc)
  {
  	//if (start.isDisabled() ) g.setColor (Color.gray);
  	
  	CPen *oldPen;
  	CBrush *oldBrush;
  	CBrush cb;
  	CPen cp;
  	if (m_Selected) {
  		cp.CreatePen(PS_SOLID, 1, COLORREF(0x0000ff) );
  		oldPen = dc->SelectObject (&cp);	
  		cb.CreateSolidBrush (COLORREF(0x0000ff) );
  		oldBrush = dc->SelectObject (&cb);	
  	}
  
  	else if (!m_Start->m_Enabled) {
  		cp.CreatePen(PS_SOLID, 1, RGB(150,150,150) );
  		oldPen = dc->SelectObject (&cp);	
  		cb.CreateSolidBrush (RGB(150,150,150) );
  		oldBrush = dc->SelectObject (&cb);
  
  	}
  	else {
  		cp.CreatePen(PS_SOLID, 1, m_Color);
  		oldPen = dc->SelectObject (&cp);
  		cb.CreateSolidBrush(m_Color);
  		oldBrush = dc->SelectObject (&cb);	
  	}
  
  	
  	
  	dc->FillRect (m_Spine, &cb);
  	dc->FillRect (m_Top, &cb);
  	
  		
  	if (m_LegalPoint) {
  		dc->FillRect (m_Bot, &cb);
  		dc->Polygon (m_ArrowVertices, 3);
  		
  	}
  
  	if (m_Start->m_Enabled) DrawIterations(dc);
  
  	dc->SelectObject (oldPen);
  	dc->SelectObject (oldBrush);
  	cp.DeleteObject();
  	cb.DeleteObject();
  }
  
  BOOL CLoop::contains(CPoint p)
  {
  	if (!m_Start->m_Enabled) return FALSE;
  	return (m_Top.PtInRect(p) || m_Bot.PtInRect(p) || m_Spine.PtInRect(p) || m_Pointer.PtInRect(p) );
  }
  
  
  
