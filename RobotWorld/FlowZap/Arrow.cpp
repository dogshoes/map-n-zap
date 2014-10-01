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
  
  // Arrow.cpp: implementation of the CArrow class.
  //
  //////////////////////////////////////////////////////////////////////
  
  #include "stdafx.h"
  #include "FlowZap.h"
  #include "Arrow.h"
  
  #ifdef _DEBUG
  #undef THIS_FILE
  static char THIS_FILE[]=__FILE__;
  #define new DEBUG_NEW
  #endif
  
  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////
  
  CArrow::CArrow(CPoint& p, CWnd *parent)
  : m_loc(p)
  , m_Parent (parent)
  {
  }
  
  CArrow::~CArrow()
  {
  	m_Parent = NULL;
  }
  
  
  
  void CArrow::PaintArrow(COLORREF color, CPaintDC *dc){	
  /************************************************************************************************
  * DATE CREATED: 
  *	1 Jan 98 11:07 PM EST
  * LAST MODIFIED:
  *	16 Jan 98 10:30 PM EST
  * PARAMETERS:
  *	color	: the color to draw the arrow
  	dc		: the handle to the device context
  * RETURN
  *	void
  ************************************************************************************************
  * DESCRIPTION
  *	paints one arrow at the arrow location as defined in m_loc in the specified color
  * METHOD:
  *
  ************************************************************************************************/
  
    /*
     *Select the pen and brush according to color
     */
  	CPen cp (PS_SOLID, 1, color );
  	CPen *oldPen = dc->SelectObject (&cp);
  	CBrush cb (color);
  	CBrush *oldBrush = dc->SelectObject (&cb);
  
    /*
     *Assign the verticies and draw the polygon
     */	
  	SetVerticies(m_loc);
  	dc->Polygon (vertices, 7);
  
    /*
     *Reset the the pen the the originals
     */
  	dc->SelectObject (oldPen);
  	dc->SelectObject (oldBrush);
  	cp.DeleteObject();
  	cb.DeleteObject();
  
  }
  
  
  
  
  
  
  void CArrow::SetLocation(int x, int y){
  /************************************************************************************************
  * DATE CREATED: 
  *	1 Jan 98 11:07 PM EST
  * LAST MODIFIED:
  *	16 Jan 98 10:30 PM EST
  * PARAMETERS:
  *	x	: the x location of the tip of the arrow
  	y	: the y location of the tip of the arrow
  * RETURN
  *	void
  ************************************************************************************************
  * DESCRIPTION
  *	sets the location of the arrow based on the parameters
  * METHOD:
  *
  ************************************************************************************************/
  	m_loc.x = x;
  	m_loc.y = y;
  }
  
  
  
  
  
  
  
  
  void CArrow::SetVerticies(CPoint p){
  /************************************************************************************************
  * DATE CREATED: 
  *	1 Jan 98 11:07 PM EST
  * LAST MODIFIED:
  *	16 Jan 98 10:30 PM EST
  * PARAMETERS:
  *	p	the point of the arrow
  * RETURN
  *	void
  ************************************************************************************************
  * DESCRIPTION
  *	sets each of the points of the polygon of the arrow so that it can be drawn
  * METHOD:
  *	PRIVATE : ONLY CALLED FROM PAINT ARROW
  ************************************************************************************************/
  	CFlowZap * fc = (CFlowZap *)m_Parent;
  	if (fc->IsExpanded() ) {
  		vertices[0].x = p.x - 1;	vertices[0].y = p.y - 14;
  		vertices[1].x = p.x + 1;	vertices[1].y = p.y - 14;
  		vertices[2].x = p.x + 1;	vertices[2].y = p.y -  9;
  		vertices[3].x = p.x + 4;	vertices[3].y = p.y -  9;
  		vertices[4].x = p.x ;		vertices[4].y = p.y ;
  		vertices[5].x = p.x - 4;	vertices[5].y = p.y -  9;	
  		vertices[6].x = p.x - 1;	vertices[6].y = p.y -  9;
  	}
  
  	else {
  		vertices[0].x = p.x;		vertices[0].y = p.y - 7;
  		vertices[1].x = p.x;		vertices[1].y = p.y - 7;
  		vertices[2].x = p.x;		vertices[2].y = p.y -  3;
  		vertices[3].x = p.x + 2;	vertices[3].y = p.y -  3;
  		vertices[4].x = p.x ;		vertices[4].y = p.y ;
  		vertices[5].x = p.x - 2;	vertices[5].y = p.y -  3;	
  		vertices[6].x = p.x;		vertices[6].y = p.y -  3;
  
  	}
  }
  
  
  
  
  
  
  CRect CArrow::GetBoundingRectangle(){
  /************************************************************************************************
  * CREATED: 1/21/98 1:19PM
  *
  * LAST MODIFIED: 1/21/98 1:19PM
  *
  * PARAMETERS
  *	none
  * RETURN
  *	CRect	the rectangle around the arrow
  ************************************************************************************************
  * DESCRIPTION
  *	determines the bounding box around an arrow
  * METHOD:
  *
  ************************************************************************************************/
  	CRect r(vertices[5].x, vertices[0].y, vertices[3].x, vertices[4].y);
  	return r;
  
  }
