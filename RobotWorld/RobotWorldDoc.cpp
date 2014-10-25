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
// RobotWorldDoc.cpp : implementation of the CRobotWorldDoc class
//
/*******************************************************************************
//Created: 08/02/96 S.R.
//Last Revision: 08/02/96 S.R.
//Parameters:
//	none
//Return Value:
//	none
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/

#include "stdafx.h"

#include "RobotWorld.h"
#include "RobotWorldDoc.h"
#include "RobotWorldView.h"

/////////////////////////////////////////////////////////////////////////////
// CRobotWorldDoc

IMPLEMENT_DYNCREATE(CRobotWorldDoc, CDocument)

BEGIN_MESSAGE_MAP(CRobotWorldDoc, CDocument)
    //{{AFX_MSG_MAP(CRobotWorldDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CRobotWorldDoc, CDocument)
    //{{AFX_DISPATCH_MAP(CRobotWorldDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //      DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IRobotW to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {6D1CD101-E9F8-11CF-A7FE-FF3FAF3C334B}
static const IID IID_IRobotW =
{ 0x6d1cd101, 0xe9f8, 0x11cf, { 0xa7, 0xfe, 0xff, 0x3f, 0xaf, 0x3c, 0x33, 0x4b } };

BEGIN_INTERFACE_MAP(CRobotWorldDoc, CDocument)
INTERFACE_PART(CRobotWorldDoc, IID_IRobotW, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotWorldDoc construction/destruction

CRobotWorldDoc::CRobotWorldDoc()
{
    /*Do Initializations of member variables*/

    EnableAutomation();

    AfxOleLockApp();
}

CRobotWorldDoc::~CRobotWorldDoc()
{
    AfxOleUnlockApp();
}

#define GetRobotWorldApp() ((CRobotWorldApp*)AfxGetApp())

BOOL CRobotWorldDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
    {
        return FALSE;
    }

    POSITION pos = GetFirstViewPosition();
    CRobotWorldView* pView = (CRobotWorldView*)GetNextView(pos);
    pView->OnNewDocument();
    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRobotWorldDoc serialization

void CRobotWorldDoc::Serialize(CArchive& ar)
{
    ar.m_pDocument = this;
    GetRobotWorldApp()->SetCurrentFile(ar.GetFile()->GetFilePath());

    if (ar.IsStoring())
    {
        // TODO: add storing code here
    }
    else
    {
        // TODO: add loading code here
    }

    POSITION pos = GetFirstViewPosition();
    CView* pView = GetNextView(pos);
    TRY
    {
        pView->Serialize(ar);
    }
    CATCH_ALL(e)
    {
        OnNewDocument();
        TRY
        {
            ReportSaveLoadException(ar.GetFile()->GetFilePath(), e,
            FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
        }
        END_TRY
    }
    END_CATCH_ALL
}

/////////////////////////////////////////////////////////////////////////////
// CRobotWorldDoc diagnostics

#ifdef _DEBUG
void CRobotWorldDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CRobotWorldDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRobotWorldDoc commands
