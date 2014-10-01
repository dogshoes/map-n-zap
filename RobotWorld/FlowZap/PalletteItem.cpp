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
  // PalletteItem.cpp : implementation file
  //
  
  #include "stdafx.h"
  //#include "flowchart.h"
  #include "flowzapresource.h"
  #include "PalletteItem.h"
  #include "Pallette.h"
  
  #ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
  #endif
  
  /////////////////////////////////////////////////////////////////////////////
  // CPalletteItem
  
  CPalletteItem::CPalletteItem()
  {
  }
  
  CPalletteItem::~CPalletteItem()
  {
  }
  
  
  BEGIN_MESSAGE_MAP(CPalletteItem, CBitmapButton)
  	//{{AFX_MSG_MAP(CPalletteItem)
  	ON_WM_LBUTTONDOWN()
  	ON_WM_SETCURSOR()
  	//}}AFX_MSG_MAP
  END_MESSAGE_MAP()
  
  /////////////////////////////////////////////////////////////////////////////
  // CPalletteItem message handlers
  
  void CPalletteItem::OnLButtonDown(UINT nFlags, CPoint point) 
  {
  	CWnd* Parent = GetParent();
  	CPoint ParentPoint;
  	ParentPoint = point;
  	CRect WindowRect;
  	GetWindowRect(WindowRect);
  	Parent->ScreenToClient(WindowRect);
  	ParentPoint.x += WindowRect.left;
  	ParentPoint.y += WindowRect.top;
  	((CPallette*)GetParent())->OnPalletteItemClicked(nFlags, this);
  //	GetParent()->OnLButtonDown(nFlags, ParentPoint);
  //	CBitmapButton::OnLButtonDown(nFlags, point);
  }
  
  BOOL CPalletteItem::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
  {
  	if (nHitTest == HTCLIENT && nHitTest != HTVSCROLL && nHitTest != HTHSCROLL) {
  		::SetCursor(::LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_GRAB)));
  		return true;
  	} else {	
  		return CBitmapButton::OnSetCursor(pWnd, nHitTest, message);
  	}
  }
