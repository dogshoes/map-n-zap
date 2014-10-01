#if !defined(AFX_DLGDRAWINGTOOLS_H__B9146A21_D07C_11D1_B231_0040F6B87672__INCLUDED_)
#define AFX_DLGDRAWINGTOOLS_H__B9146A21_D07C_11D1_B231_0040F6B87672__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgDrawingTools.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDrawingTools dialog

class CDlgDrawingTools : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgDrawingTools)
// Construction
public:
	void SetPenTool(short Tool);
	short GetPenTool(void);
	void SetPenSize(short Size);
	short GetPenSize(void);
	CDlgDrawingTools();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDrawingTools)
	enum { IDD = IDD_DRAWING_TOOLS };
	int		m_radPenSize;
	int		m_radToolType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDrawingTools)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDrawingTools)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDRAWINGTOOLS_H__B9146A21_D07C_11D1_B231_0040F6B87672__INCLUDED_)
