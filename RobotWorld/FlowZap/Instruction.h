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
 #ifndef Instructionljdkfjdlsk
 #define Instructionljdkfjdlsk
 
 #ifdef ROBOT_WORLD_PROJECT
 #include "Robot.h"
 #endif
 
 #define dSMALL_INSTRUCTION_SIZE 16
 class CLoop;
 class CExecutableRWGraphicObjectList; 
 
 class CInstruction:public CDialog {
 	DECLARE_DYNAMIC( CInstruction )
 public:
 #ifdef ROBOT_WORLD_PROJECT
 	virtual TTaskResult Execute(CRobot* Robot, CExecutableRWGraphicObjectList* ExecutableObjectList, bool& result);
 #endif
 	int GetHeight();
 	CInstruction* m_LoopStart;
 	BOOL m_Enabled;
 	virtual void Enable (BOOL enable)=0;
 	virtual CString ToFile()=0;
 	BOOL IsMoveable();
 	
 	virtual void ShowBorder (BOOL b)=0;
 	
 	// CONSTANTS
 	BOOL			m_Expanded;
 	int				m_ExpandedHeight;
 	CLoop*			m_Loop;
 	CWnd*			m_fc;
 	
 
 	// FUNCTIONS
 	CInstruction( UINT nIDTemplate, CWnd* pParentWnd = NULL, int index =1);
 	void	MakeMoveable (BOOL moveable);
 	void	OnInit();
 	void	MouseButtonUp (UINT nFlags, CPoint point);
 	BOOL	Between (CInstruction* a, CInstruction *b);
 	BOOL	IsSelected();
 	int		GetIndex();
 	int		GetCenter();
 	void	MakeSelected(BOOL s);
 	void	MouseClick (CPoint point);
 	void	OnPaint(CDC *dc);
 	void	SetIndex(int index);
 	
 	virtual CString ToString()=0;
 	virtual void	MakeExpanded()=0;
 	virtual void	MakeCondensed()=0;
 
 private:
 	int		m_Index;
 	BOOL	m_Moveable;
 	BOOL	m_Selected;
 	
 
 protected:
 
 };
 
 
 #endif