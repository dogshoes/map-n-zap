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
   // RRRSrvView.h : interface of the CRRRSrvView class
   //
   /////////////////////////////////////////////////////////////////////////////
   
   class CRRRSrvView : public CFormView
   {
   protected: // create from serialization only
   	CRRRSrvView();
   	DECLARE_DYNCREATE(CRRRSrvView)
   
   public:
   	//{{AFX_DATA(CRRRSrvView)
   	enum{ IDD = IDD_RRRSRV_FORM };
   		// NOTE: the ClassWizard will add data members here
   	//}}AFX_DATA
   
   // Attributes
   public:
   	CRRRSrvDoc* GetDocument();
   
   // Operations
   public:
   
   // Overrides
   	// ClassWizard generated virtual function overrides
   	//{{AFX_VIRTUAL(CRRRSrvView)
   	public:
   	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   	protected:
   	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   	//}}AFX_VIRTUAL
   
   // Implementation
   public:
   	virtual ~CRRRSrvView();
   #ifdef _DEBUG
   	virtual void AssertValid() const;
   	virtual void Dump(CDumpContext& dc) const;
   #endif
   
   protected:
   
   // Generated message map functions
   protected:
   	//{{AFX_MSG(CRRRSrvView)
   		// NOTE - the ClassWizard will add and remove member functions here.
   		//    DO NOT EDIT what you see in these blocks of generated code !
   	//}}AFX_MSG
   	DECLARE_MESSAGE_MAP()
   };
   
   #ifndef _DEBUG  // debug version in RRRSrvView.cpp
   inline CRRRSrvDoc* CRRRSrvView::GetDocument()
      { return (CRRRSrvDoc*)m_pDocument; }
   #endif
   
   /////////////////////////////////////////////////////////////////////////////
