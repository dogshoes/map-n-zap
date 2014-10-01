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
 #if !defined(AFX_PROPROBOTCOLOR_H__30E5C3E3_F617_11D1_B231_0040F6B87672__INCLUDED_)
 #define AFX_PROPROBOTCOLOR_H__30E5C3E3_F617_11D1_B231_0040F6B87672__INCLUDED_
 
 #if _MSC_VER >= 1000
 #pragma once
 #endif // _MSC_VER >= 1000
 // PropRobotColor.h : header file
 //
 
 /////////////////////////////////////////////////////////////////////////////
 // CPropRobotColor dialog
 
 class CPropRobotColor : public CPropertyPage
 {
 	DECLARE_DYNCREATE(CPropRobotColor)
 
 // Construction
 public:
 	int GetRobotColor(void);
 	void SetRobotColor(int RobotColor);
 	CPropRobotColor();
 	~CPropRobotColor();
 
 // Dialog Data
 	//{{AFX_DATA(CPropRobotColor)
 	enum { IDD = IDD_PROP_ROBOT_COLOR };
 	CStatic	m_BmpChrome;
 	CStatic	m_Bmp08;
 	CStatic	m_Bmp300;
 	CStatic	m_Bmp200;
 	CStatic	m_Bmp044;
 	CStatic	m_Bmp033;
 	CStatic	m_Bmp022;
 	CStatic	m_Bmp011;
 	CStatic	m_Bmp000;
 	//}}AFX_DATA
 
 
 // Overrides
 	// ClassWizard generate virtual function overrides
 	//{{AFX_VIRTUAL(CPropRobotColor)
 	protected:
 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 	//}}AFX_VIRTUAL
 
 // Implementation
 protected:
 	// Generated message map functions
 	//{{AFX_MSG(CPropRobotColor)
 	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
 	afx_msg void OnPaint();
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 
 private:
 	CStatic* m_HighlightedBmp;
 	void HighlightBitmap(CDC* pDC, CStatic* Bitmap);
 	enum {cNumRobotColors = 9};
 	CStatic* m_BmpRobotColors[cNumRobotColors];
 };
 
 //{{AFX_INSERT_LOCATION}}
 // Microsoft Developer Studio will insert additional declarations immediately before the previous line.
 
 #endif // !defined(AFX_PROPROBOTCOLOR_H__30E5C3E3_F617_11D1_B231_0040F6B87672__INCLUDED_)
