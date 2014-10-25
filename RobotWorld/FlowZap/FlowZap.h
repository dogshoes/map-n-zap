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
#if !defined(AFX_FLOWZAP_H__C4DFEA63_88FC_11D1_AC8C_0040F6B8AE22__INCLUDED_)
#define AFX_FLOWZAP_H__C4DFEA63_88FC_11D1_AC8C_0040F6B8AE22__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FlowZap.h : header file
//

#include "mmsystem.h"

#include "FlowZapResource.h"
/*#include "instruction.h"
#include "leftlooppanel.h"
#include "rightlooppanel.h"
#include "pallette.h"
#include "cue.h"
#include "dropoff.h"
#include "arrow.h"
#include "util.h"
#include "execute.h"


#include "Beep.h"
#include "Repeat.h"
#include "Branch.h"
#include "Wait.h"
#include "Goto.h"
#include "Start.h"
#include "Sub.h"
#include "Stop.h"
#include "Vacuum.h"
#include "Calibrate.h"
#include "Motion.h"
#include "iostream.h"
#include "Preferences.h"	// Added by ClassView
#include "Tool.h"	// Added by ClassView
*/

extern const char* cFlowZapFilesDirectory;

class CLeftLoopPanel;
class CPallette;
class CCue;
class CDropOff;
class CRightLoopPanel;
class CInstruction;
class CTool;
class CPreferences;
class CExecute;
class CArrow;
class CRobotWorldView;
class CRWGraphicObject;

#define dZONE_NOCHANGE -1

#define CMD_FILENEW 0
#define CMD_FILEOPEN 1
#define CMD_FILECLOSE 2
#define CMD_FILESAVE 3
#define CMD_FILESAVEAS 4
#define CMD_FILEEXIT 5

/////////////////////////////////////////////////////////////////////////////
// CFlowZap dialog

enum TFZMouseMode
{
    mmFZDefault,
    mmFZVac, mmFZPath, mmFZCal, mmFZStop, mmFZBeep, mmFZRepeat, mmFZBranch, mmFZWait, mmFZSub, mmFZMotion, mmFZExternalOutput, mmFZGoto,
    mmFZMove, mmFZSelected, mmFZCue, mmFZMoveCue, mmFZRepeatLoop, mmFZMoveRepeatLoop, mmFZBranchLoop, mmFZMoveBranchLoop,
    mmFZIllegalMove
};

enum TProgramMode
{
    pmEdit, pmExecute
};


class CRobot;
class CExecutableRWGraphicObjectList;

class CFlowZap : public CDialog
{
        // Construction
    public:
        CRobot* GetActiveRobot(void);
        bool GetPlaying(void);
        void SetCueToInstruction(CInstruction* Instruction);
        CString GetDefaultZapDirectory(void);
        int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
        void SetActiveRWGraphicObject(CRWGraphicObject* ActiveObject);
        void SetIsSubroutineWindow(void);
        CInstruction* makeExternalOutput(int index);
        void OnPlayStop(void);
        void OnPlayPause(void);
        void OnPlayStart(void);
        CInstruction* MakePathFollowFromFile(CStringList *words);
        void MakePathFollowFrom();
        void UpdateZonePlacements();
        CInstruction* makePathFollow(int index);
        int m_xPosIndent;
        void UpdateMovableStatusOfInstructions();
        void ChangeMadeInFile();
        void CloseFlowZap();
        void OnFileManipulation(int Command, int Code, CString filename);
        BOOL OpenFileOnStartup();
        void MakeRobotBeep(double hz);
        BOOL	m_ChangeMadeInFile;
        CTool * m_Tool;
        void RemoveArrowFromList();
        int			CloseFile();
        bool OpenFile(CString FileName);
        void InitPallette();
        void SetCueToStart();
        BOOL IsSoundOn();
        BOOL m_FileIsOpen;
        void InsertInstructionBefore(CInstruction* bench, CInstruction* insert);
        void RemoveInstructionFromList(CInstruction* in);
        void HandlePopupFileMenu(CMenu* menu);
        CFlowZap* OpenSubProgramWindow(CString filename);
        void InsertInstructionAfter(CInstruction *first, CInstruction *second);
        CPoint m_PopUpMenuLocation;
        void UnhighlightArrow();
        void ParseFile(CStdioFile *f);
        CFlowZap* m_SubProgramWindow;
        int m_OldVScrollPosition;
        int m_ScrollingArea;
        int m_ListHeight;


        // *** MENU MANIPULATION *** ///
        void SetEnableDisableMenu(BOOL enabled);
        void EnableInstruction(CInstruction* in, BOOL enable);
        void ToggleMenuViewChecked(BOOL b);
        void MakeMenuInstructionActive(BOOL active);


        // *** MANIPULATION OF LISTS FROM ROBOTWORLD *** ///
        void SetLinePaths(CStringList* LinePaths);
        void SetVacuumPatches(CStringList *patches);
        void SetCalibrationZones(CStringList *zones);
        void SetLabelNames(CStringList *labels);
        void RemoveVacuumPatch(CString OldCaption);
        void RemoveCalibrationZone(CString OldCaption);
        void RemoveLabel(CString OldCaption);
        void RemoveLinePath(CString OldCaption);
        void PurgeStringList(CStringList * list);
        void ChangeCalibrationZone(CString OldCaption, CString NewCaption);
        void ChangeVacuumPatch(CString OldCaption, CString NewCaption);
        void ChangeLabel(CString OldCaption, CString NewCaption);
        void ChangeLinePath(CString OldCaption, CString NewCaption);
        void AddVacuumPatch(CString name);
        void AddCalibrationZone(CString name);
        void AddLabel(CString name);
        void AddLinePath(CString name);


        // *** RUNNING THE PROGRAM *** ///
        void			Stop();
        CInstruction*	Play();
        CInstruction*	GetNextExecutableInstruction(CInstruction* in);
        void			SwitchToExecuteMode(BOOL Execute);
        CInstruction*	GetInstructionToExecute(BOOL test);


        //  *** INFORMATION RETRIEVAL ***
        int				GetInstructionsSize();
        POSITION		GetInstructionPosition(CInstruction *in);
        BOOL			IsExpanded();
        CInstruction*	GetInstructionAtIndex(int index);
        CInstruction*   GetInstructionAtPixel(CPoint p);
        CInstruction*	GetNextInstructionInList(CInstruction *in);



        // *** INSTRUCTION MANIPULATION
        void MoveInstruction(CPoint point);
        CInstruction* DeselectInstruction();
        CInstruction* m_SelectedInstruction;


        // *** LOOKUP TABLES *** //
        double  m_HzConversion[36];
        CString m_NoteConversion[36];


        // *** INITILIZATION *** ///
        CFlowZap(CWnd* pParent = NULL, CExecutableRWGraphicObjectList* ExecutableObjectList = NULL);   // standard constructor

        // destructor
        ~CFlowZap();
        int InitInstructionList();


        void SetCursor();
        void UpdateWindowPositions();
        TFZMouseMode   m_MouseMode;
        TProgramMode m_ProgramMode;
        void AdjustScrollBars(int windowHeight, int ClientHeight);
        virtual BOOL PreTranslateMessage(MSG* pMsg);




        // Dialog Data
        //{{AFX_DATA(CFlowZap)
        enum { IDD = IDD_FLOWZAP };
        CStatic	m_Border;
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CFlowZap)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        afx_msg void OnContextMenu(CWnd*, CPoint point);
        CToolTipCtrl m_tooltip;

        // Generated message map functions
        //{{AFX_MSG(CFlowZap)
        afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnFileClose();
        afx_msg void OnFileExit();
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        virtual BOOL OnInitDialog();
        afx_msg void OnCancelMode();
        afx_msg void OnPaint();
        afx_msg void OnCaptureChanged(CWnd *pWnd);
        afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
        afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        afx_msg void OnClose();
        afx_msg void OnViewCondensed();
        afx_msg void OnInstructionDelete();
        afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
        afx_msg void OnInstructionSetCue();
        afx_msg void OnViewExpanded();
        afx_msg void OnHelpAbout();
        afx_msg void OnFileSaveas();
        afx_msg void OnFileSave();
        afx_msg void OnFileNew();
        afx_msg void OnFileOpen();
        afx_msg void OnUpdateViewCondensed(CCmdUI* pCmdUI);
        afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
        afx_msg void OnInstructionDisable();
        afx_msg void OnInstructionEnable();
        afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnSetFocus(CWnd* pOldWnd);
        afx_msg void OnInsertCalibrate();
        afx_msg void OnInsertExecute();
        afx_msg void OnInsertGoto();
        afx_msg void OnInsertIf();
        afx_msg void OnInsertRepeat();
        afx_msg void OnInsertSing();
        afx_msg void OnInsertStop();
        afx_msg void OnInsertVacuum();
        afx_msg void OnInsertMotion();
        afx_msg void OnInsertWait();
        afx_msg void OnInsertFollowPath();
        afx_msg void OnInsertExternalOutput();
        afx_msg void OnPopupDelete();
        afx_msg void OnPopupDisable();
        afx_msg void OnPopupEnable();
        afx_msg void OnEditPreferences();
        afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
        afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnDestroy();
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

    private:
        void SetActiveRobot(CRobot* Robot);
        CRobot* m_ActiveRobot;
        bool m_IsSubroutineWindow;
        int m_Origional_Origin;
        CInstruction* MakeExternalOutputFromFile(CStringList* words);
        bool m_Playing;
        CRobotWorldView* m_RobotWorldView;
        bool m_Pause;
        CInstruction* m_OrigionalPointerLocation;
        CInstruction* MakeVacuumFromFile(CStringList * words);
        CInstruction* MakeCalibrateFromFile(CStringList * words);
        CInstruction* MakeMotionFromFile(CStringList * words);
        CInstruction* MakeWaitFromFile(CStringList *words);
        CInstruction* MakeBranchFromFile(CStringList * words);
        CInstruction* MakeGotoFromFile(CStringList *words);
        CInstruction* MakeRepeatFromFile(CStringList *words, int index);

        CExecutableRWGraphicObjectList* m_ExecutableObjectList;
        CStringList * m_CalibrationZones;
        CStringList * m_VacuumPatches;
        CStringList * m_LabelNames;
        CStringList * m_LinePaths;
        CPreferences *	m_PreferencesControl;
        CExecute*		m_ExecuteControl;


        int  TrackMouse(CPoint point);
        void HandlePopupInstructionMenu(CInstruction *ptr, CMenu* menu);
        BOOL OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult);

        // *** INITILIZATION ***
        void MakeLookupTables();
        void InitMenuBar();

        // *** THE LISTS ***
        CObList m_arrows;
        CObList m_instructions;


        // *** ADDING TO THE LISTS ***
        int  AddArrow();
        void AddInstruction(TFZMouseMode mode, CInstruction* ptr);


        // *** PURGING THE LISTS ***
        void EmptyArrowList();
        void EmptyInstructionList();


        // *** ZONES  ***
        CLeftLoopPanel*	m_llp;
        CPallette*		m_Pallette;
        CCue*			m_Cue;
        CDropOff*		m_DropOff;
        CRightLoopPanel*m_rlp;


        //CAboutDlg m_AboutDlg;
        // *** EXPANDED VIEW *** ///
        BOOL m_ExpandedView;
        void MakeViewExpanded(BOOL expanded);


        // *** ARROWS *** ///
        CArrow* m_hArrow;  // the highlighted arrow
        void PaintArrows(CPaintDC *dc);


        // *** MAKING INSTRUCTIONS ***
        CInstruction* makeStart(int index);
        CInstruction* makeWait(int index);
        CInstruction* makeBranch(int index);
        CInstruction* makeGoto(int index);
        CInstruction* makeRepeat(int index);
        CInstruction* makeBeep(int index);
        CInstruction* makeStop(int index);
        CInstruction* makeSub(int index);
        CInstruction* makeVac(int index);
        CInstruction* makeCal(int index);
        CInstruction* makeMotion(int index);


        // *** WINDOW POSITIONS ***
        CRect	UpdateInstructionPositions(int arrowHeight);
        void	UpdateArrowPositions(int arrowHeight);
        void	UpdateRepeats();
        int  m_Origin;
        int	 m_OrigionalWindowWidth;
        int  m_InstructionMidPoint;


        //  *** FILE MANIPULATION ***

        int			SaveFile();
        int			SaveAs();
        void		WriteInstructionsToFile(CStdioFile *f);
        void		MakeNewInstructionFromFile(CStringList * words, int index);

        CString m_CurrentFileName;
        BOOL	m_NeedToSaveAs;

        CInstruction* RemoveInstruction(CInstruction* in);
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOWZAP_H__C4DFEA63_88FC_11D1_AC8C_0040F6B8AE22__INCLUDED_)
