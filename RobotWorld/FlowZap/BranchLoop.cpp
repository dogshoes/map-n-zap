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
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,   
    USA.
  
    Probotics, Inc
    http://www.personalrobots.com
    700 River Ave #223
    Pittsburgh, PA 15212
    USA
  **********************************************************************/
  
  // BranchLoop.cpp: implementation of the CBranchLoop class.
  //
  //////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "FlowZap.h"
  #include "BranchLoop.h"
  #include "Branch.h"
  #include "Instruction.h"
  
  #ifdef _DEBUG
  #undef THIS_FILE
  static char THIS_FILE[]=__FILE__;
  #define new DEBUG_NEW
  #endif
  
  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////
  
  CBranchLoop::CBranchLoop(CBranch* s, CInstruction* e, COLORREF c, int i)
  {
  	m_Start = s;
  	m_End   = e;
  	m_Selected = FALSE;
  	m_Color = c;
  	m_LegalPoint=TRUE;
  }
  
  CBranchLoop::~CBranchLoop()
  {
  
  }
  
  
  void CBranchLoop::CalculateLocation(CDialog * parent, int index, int maxIndex, int w){
  /************************************************************************************************
  * DATE CREATED: 
  *	1 Jan 98 11:07 PM EST
  * LAST MODIFIED:
  *	16 Jan 98 2:30 PM EST
  * PARAMETERS:
  *	parent		the flowchart parent window
  *	index		the number this loop is in the list of loops
  *	maxIndex	the total number of loops in the branch loop list
  *	w			the width of the branch loop panel
  * RETURN
  *	void
  ************************************************************************************************
  * DESCRIPTION
  *	determines the placement of the line and arrow for this loop
  * METHOD:
  *
  ************************************************************************************************/
  
  	int y1;  // y1 is the y location of the horizontal line on the START instruction
  	int y2;  // y2 is the y location of the horizontal line on the END instruction
  	int spine; // spine is the location of the vertical line of this loop arrow
  
  	/*
  	** CALCULATE y1 and y2
  	*/
  	CRect r;
  	m_Start->GetWindowRect(&r); 
  	((CFlowZap *)parent)->ScreenToClient(&r);
  	y2 = r.TopLeft().y + 1*r.Height()/2;
  	
  	m_End->GetWindowRect(&r);
  	((CFlowZap *)parent)->ScreenToClient(&r);
  	y1 =  (index +1)* r.Height() /(maxIndex+1);
  	if (y1 > r.Height()/2 - 1 && y1 < r.Height()/2 + 1) y1-=7; // KEEP IT OFF THE CENTER
  	y1+= r.TopLeft().y;
  
  
  	
  	//int spine =  r.BottomRight().x + (int)((double)(w / (maxIndex +1)) * (index +1) );
  	spine = r.right + ((w * (index+1)) / (maxIndex + 1) );
  	if (spine < r.right + 15) spine = r.right + 15;
  
  	m_Top.SetRect(r.right, y2-1, spine + 1, y2+1); 
  	m_Bot.SetRect(r.right, y1-1, spine + 1, y1+1);
  
  	m_Spine.SetRect(spine-1, y2-1, spine+1, y1+1);
  	m_Pointer.SetRect(r.right, y1-3, r.right+12, y1+3);
  
  	m_ArrowVertices[0].x = r.right;    m_ArrowVertices[0].y = y1;
  	m_ArrowVertices[1].x = r.right+12; m_ArrowVertices[1].y = y1-3;
  	m_ArrowVertices[2].x = r.right+12; m_ArrowVertices[2].y = y1+3;
  }
  
  void CBranchLoop::DrawIterations(CPaintDC * dc)
  {
  
  }
  
  BOOL CBranchLoop::PointingToSelf()
  {
  	return (m_Start == m_End);
  }
