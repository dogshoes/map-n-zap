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
  // RepeatLoop.cpp: implementation of the CRepeatLoop class.
  //
  //////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "RepeatLoop.h"
  #include "FlowZap.h"
  #include "Repeat.h"
  #include "Instruction.h" 
  
  #ifdef _DEBUG
  #undef THIS_FILE
  static char THIS_FILE[]=__FILE__;
  #define new DEBUG_NEW
  #endif
  
  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////
  
  CRepeatLoop::CRepeatLoop(CRepeat* s, CInstruction* e, COLORREF c, int i)
  {
  	m_Start = s;
  	m_End   = e;
  	m_Selected = FALSE;
  	m_Color = c;
  	m_LegalPoint = TRUE;
  }
  
  CRepeatLoop::~CRepeatLoop()
  {
  
  }
  
  BOOL CRepeatLoop::PointingToSelf()
  {
  	return (m_Start == m_End);
  }
  
  void CRepeatLoop::CalculateLocation(CDialog *parent, int index, int maxIndex, int w)
  {
  	int spine = 25 + (int)((double)(w / (maxIndex +1)) * (m_Level +1) );
  	CRect r;
  	m_Start->GetWindowRect(&r);
  	((CFlowZap *)parent)->ScreenToClient(&r);
  	int y2 = r.TopLeft().y + 2*r.Height()/8;
  	m_End->GetWindowRect(&r);
  	((CFlowZap *)parent)->ScreenToClient(&r);
  	int y1 = r.TopLeft().y + (4 +  m_Level)*r.Height() /8;
  			
      m_Top.SetRect(spine-1, y2-1, 25 +w, y2+1); 
      m_Bot.SetRect(spine-1, y1-1, 25 +w, y1+1);
  	m_Spine.SetRect(spine-1, y2-1, spine+1, y1+1);
  	m_Pointer.SetRect(25+w-12, y1-4, 25+w, y1+4);
  
  	m_ArrowVertices[0].x = 25+w; m_ArrowVertices[0].y = y1;
  	m_ArrowVertices[1].x = 25+w-12; m_ArrowVertices[1].y = y1-3;
  	m_ArrowVertices[2].x = 25+w-12; m_ArrowVertices[2].y = y1+3;
  }
  
  
  
  
  
  
  void CRepeatLoop::DrawIterations(CPaintDC * dc)
  {
  	CString text = "7";
  		
  	CRepeat* s = (CRepeat*) m_Start;
  		
  	text = s->GetNumRepeats();
  	int textHeight = 12;
  	int textWidth  = 8*text.GetLength();
  	int halfway = m_Spine.TopLeft().y + m_Spine.Height()/2 - textHeight + 3;
  	int spine = m_Spine.TopLeft().x + m_Spine.Width()/2;
  	
  	COLORREF oldColor;
  	if (m_Selected) oldColor = dc->SetTextColor (COLORREF (0x0000ff) );
  	else            oldColor = dc->SetTextColor (m_Color);
  	dc->TextOut (spine - textWidth/2, halfway, text );
  	dc->SetTextColor (oldColor);
  }
