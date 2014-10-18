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
 // RRRSrvView.cpp : implementation of the CRRRSrvView class
 //
 
 #include "stdafx.h"
 #include "RRRSrv.h"
 
 #include "RRRSrvDoc.h"
 #include "RRRSrvView.h"
 
 /////////////////////////////////////////////////////////////////////////////
 // CRRRSrvView
 
 IMPLEMENT_DYNCREATE(CRRRSrvView, CFormView)
 
 BEGIN_MESSAGE_MAP(CRRRSrvView, CFormView)
 	//{{AFX_MSG_MAP(CRRRSrvView)
 		// NOTE - the ClassWizard will add and remove mapping macros here.
 		//    DO NOT EDIT what you see in these blocks of generated code!
 	//}}AFX_MSG_MAP
 END_MESSAGE_MAP()
 
 /////////////////////////////////////////////////////////////////////////////
 // CRRRSrvView construction/destruction
 
 CRRRSrvView::CRRRSrvView()
 	: CFormView(CRRRSrvView::IDD)
 {
 	//{{AFX_DATA_INIT(CRRRSrvView)
 		// NOTE: the ClassWizard will add member initialization here
 	//}}AFX_DATA_INIT
 	// TODO: add construction code here
 
 }
 
 CRRRSrvView::~CRRRSrvView()
 {
 }
 
 void CRRRSrvView::DoDataExchange(CDataExchange* pDX)
 {
 	CFormView::DoDataExchange(pDX);
 	//{{AFX_DATA_MAP(CRRRSrvView)
 		// NOTE: the ClassWizard will add DDX and DDV calls here
 	//}}AFX_DATA_MAP
 }
 
 BOOL CRRRSrvView::PreCreateWindow(CREATESTRUCT& cs)
 {
 	// TODO: Modify the Window class or styles here by modifying
 	//  the CREATESTRUCT cs
 
 	return CFormView::PreCreateWindow(cs);
 }
 
 /////////////////////////////////////////////////////////////////////////////
 // CRRRSrvView diagnostics
 
 #ifdef _DEBUG
 void CRRRSrvView::AssertValid() const
 {
 	CFormView::AssertValid();
 }
 
 void CRRRSrvView::Dump(CDumpContext& dc) const
 {
 	CFormView::Dump(dc);
 }
 
 CRRRSrvDoc* CRRRSrvView::GetDocument() // non-debug version is inline
 {
 	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRRRSrvDoc)));
 	return (CRRRSrvDoc*)m_pDocument;
 }
 #endif //_DEBUG
 
 /////////////////////////////////////////////////////////////////////////////
 // CRRRSrvView message handlers
