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
 // DbrFlowZapControl.h: interface for the CDbrFlowZapControl class.
 //
 //////////////////////////////////////////////////////////////////////
 
 #if !defined(AFX_DBRFLOWZAPCONTROL_H__59859FA5_CE2B_11D1_AC8D_0040F6B8AE22__INCLUDED_)
 #define AFX_DBRFLOWZAPCONTROL_H__59859FA5_CE2B_11D1_AC8D_0040F6B8AE22__INCLUDED_
 
 #if _MSC_VER >= 1000
 #pragma once
 #endif // _MSC_VER >= 1000
 
 #endif // !defined(AFX_DBRFLOWZAPCONTROL_H__59859FA5_CE2B_11D1_AC8D_0040F6B8AE22__INCLUDED_)
 /////////////////////////////////////////////////////////////////////////////
 // CDbrFlowZapControl dialog
 
 class CDbrFlowZapControl : public CDialogBar
 {
 // Construction
 public:
 	virtual BOOL PreTranslateMessage(MSG* pMsg);
 	void SetStopEnable(bool Enable);
 	void SetPauseEnable(bool Enable);
 	void SetPlayEnable(bool Enable);
 	void SetShowFZEnable(bool Enable);
 	void OnStop(void);
 	void OnPause(void);
 	void OnPlay(void);
 	void OnShowFZ(void);
 //	CButton* GetStop(void);
 //	CButton* GetPause(void);
 //	CButton* GetPlay(void);
 	CComboBox* GetComFileList(void);
 	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
 	CDbrFlowZapControl();   // standard constructor
 
 
 // Implementation
 protected:
 
 	// Generated message map functions
 	//{{AFX_MSG(CDbrFlowZapControl)
 	afx_msg void OnPaint();
 	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
 	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
 	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
 	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
 	afx_msg void OnSize(UINT nType, int cx, int cy);
 	//}}AFX_MSG
 	DECLARE_MESSAGE_MAP()
 private:
 	void SetToolTipRects(void);
 	CToolTipCtrl m_tooltip;
 	typedef enum {bfNone, bfPlay, bfPause, bfStop, bfShowFZ} TButtonFocused;
 	TButtonFocused m_ButtonFocused;
 	typedef enum {bsDisabled, bsPressed, bsEnabled} TButtonState;
 	TButtonState m_PlayState;
 	TButtonState m_PauseState;
 	TButtonState m_StopState;
 	TButtonState m_ShowFZState;
 	void SetPlayState(TButtonState State);
 	void SetPauseState(TButtonState State);
 	void SetStopState(TButtonState State);
 	void SetShowFZState(TButtonState State);
 	void PaintBackground(CDC* pDC);
 	CBitmap m_bmpPlay;
 	CBitmap m_bmpPause;
 	CBitmap m_bmpStop;
 	CBitmap m_bmpShowFZ;
 	CBitmap m_bmpBackground;
 	CDC m_dcDisplayMemory;
 	CRect m_recPlay;
 	CRect m_recPause;
 	CRect m_recStop;
 	CRect m_recShowFZ;
 	void SetFileList(CStringList FileList);
 };
