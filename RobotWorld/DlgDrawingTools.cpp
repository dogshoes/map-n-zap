// DlgDrawingTools.cpp : implementation file
//

#include "stdafx.h"
#include "robotworld.h"
#include "DlgDrawingTools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDrawingTools dialog
IMPLEMENT_DYNCREATE(CDlgDrawingTools, CPropertyPage)

CDlgDrawingTools::CDlgDrawingTools()
	: CPropertyPage(CDlgDrawingTools::IDD)
{
	//{{AFX_DATA_INIT(CDlgDrawingTools)
	m_radPenSize = -1;
	m_radToolType = -1;
	//}}AFX_DATA_INIT
}


void CDlgDrawingTools::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDrawingTools)
	DDX_Radio(pDX, IDC_RADIO1, m_radPenSize);
	DDX_Radio(pDX, IDC_RADIO4, m_radToolType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDrawingTools, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgDrawingTools)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDrawingTools message handlers

short CDlgDrawingTools::GetPenSize()
{
	if (m_hWnd != NULL) UpdateData(TRUE);
	return m_radPenSize;
}


void CDlgDrawingTools::SetPenSize(short Size)
{
	m_radPenSize = Size;
	if (m_hWnd != NULL) UpdateData(FALSE);
}

short CDlgDrawingTools::GetPenTool()
{
	if (m_hWnd != NULL) UpdateData(TRUE);
	return m_radToolType;
}

void CDlgDrawingTools::SetPenTool(short Tool)
{
	m_radToolType = Tool;
	if (m_hWnd != NULL) UpdateData(FALSE);
}
