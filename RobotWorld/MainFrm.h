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
// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#include "DlgCalibrateSurface.h"
#include "DbrFlowZapControl.h"
#include "DbrRobotExternals.h"
#include "DlgVideo.h"
#include "RobotStatus.h"

class CRobotWorldView;

class CMainFrame : public CFrameWnd
{
    protected: // create from serialization only
        afx_msg void OnUpdateTime(CCmdUI* pCmdUI);
        //	CToolBar m_wndRobotControls;
        CToolBar m_wndMapTools;
        CToolBar m_wndRobotTools;
        //	CToolBar m_wndRobotExternals;
        CDbrRobotStatus m_wndRobotStatus;
        CDbrFlowZapControl m_wndFlowZapControl;
        CDbrRobotExternals m_wndRobotExternals;
        CToolBar m_wndDrawingTools;
        CDlgVideo m_video;
        CDialogBar m_wndMapBar;
        CMainFrame();
        DECLARE_DYNCREATE(CMainFrame)

        // Attributes
    public:
        int m_nTimePaneNo;
        CRobotWorldView* GetRobotWorldView()
        {
            return ((CRobotWorldView*)GetActiveView());
        };

        // Operations
    public:

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMainFrame)
    public:
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
        //}}AFX_VIRTUAL

        // Implementation
    public:
        void OnExternalInputUpdate(float Volts);
        void SetExternalOutput(bool On);
        void OnExternalOutputChange(bool On);
        bool GetPlay(void);
        bool GetPause(void);
        void OnSetDrawingPenSize(short Size);
        void OnSetDrawingPenMode(short Mode);
        void OnFzcShowFZ(void);
        void OnFzcButPlay(void);
        void OnFzcButPause(void);
        void OnFzcButStop(void);
        void OnSetWagonAttached(bool Attached);
        CToolBar m_wndPracticeToolbar;
        void OnFzcChangeFileOpen(CString FileOpen);
        void SetFlowZapControlFileList(CStringList FileList);
        void SetRadioLevel(float RadioLevel);
        void OnSetVacuumAttached(bool Attached);
        void OnSetBatteryLevel(float BatteryLevel);
        void OnBatteryChargeStateChange(short BatteryChargeState);
        void OnSetAutoCalibrate(bool AutoCalibrate);
        void OnSetMaxSpeed(short MaxSpeed);
        void OnSetEnableLearnRoom(bool Enable);
        void OnEnableRobotPCControl(bool Enable);
        virtual ~CMainFrame();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

    protected:  // control bar embedded members
        CStatusBar  m_wndStatusBar;
        CToolBar    m_wndToolBar;

        // Generated message map functions
    protected:
        //{{AFX_MSG(CMainFrame)
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnViewRobotControls();
        afx_msg void OnUpdateViewRobotControls(CCmdUI* pCmdUI);
        afx_msg void OnViewRobotStatus();
        afx_msg void OnUpdateViewRobotStatus(CCmdUI* pCmdUI);
        afx_msg void OnPlaceLabel();
        afx_msg void OnViewMotorControls();
        afx_msg void OnUpdateViewMotorControls(CCmdUI* pCmdUI);
        afx_msg void OnClose();
        afx_msg void OnZoom100();
        afx_msg void OnUpdateZoom100(CCmdUI* pCmdUI);
        afx_msg void OnZoom200();
        afx_msg void OnUpdateZoom200(CCmdUI* pCmdUI);
        afx_msg void OnZoom400();
        afx_msg void OnUpdateZoom400(CCmdUI* pCmdUI);
        afx_msg void OnViewLabels();
        afx_msg void OnUpdateViewLabels(CCmdUI* pCmdUI);
        afx_msg void OnUpdateViewRedZones(CCmdUI* pCmdUI);
        afx_msg void OnCenterRobot();
        afx_msg void OnViewRedZones();
        afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
        afx_msg BOOL OnQueryNewPalette();
        afx_msg void OnViewVacuumPatches();
        afx_msg void OnUpdateViewVacuumPatches(CCmdUI* pCmdUI);
        afx_msg void OnMMBlockGrid();
        afx_msg void OnMMClearGrid();
        afx_msg void OnMMUnknownGrid();
        afx_msg void OnMMDefault();
        afx_msg void OnMMErase();
        afx_msg void OnMMMove();
        afx_msg void OnBlockArea();
        afx_msg void OnCalibrateSurface();
        afx_msg void OnUpdateRobotControls(CCmdUI* pCmdUI);
        afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnViewTestRobot();
        afx_msg void OnUpdateViewTestRobot(CCmdUI* pCmdUI);
        afx_msg void OnUpdateBlockArea(CCmdUI* pCmdUI);
        afx_msg void OnUpdateMMBlockGrid(CCmdUI* pCmdUI);
        afx_msg void OnUpdateMMClearGrid(CCmdUI* pCmdUI);
        afx_msg void OnUpdateMMUnknownGrid(CCmdUI* pCmdUI);
        afx_msg void OnUpdateMMDefault(CCmdUI* pCmdUI);
        afx_msg void OnUpdateMMErase(CCmdUI* pCmdUI);
        afx_msg void OnUpdateMMMove(CCmdUI* pCmdUI);
        afx_msg void OnUpdatePlaceLabel(CCmdUI* pCmdUI);
        afx_msg void OnVacuum();
        afx_msg void OnUpdateVacuum(CCmdUI* pCmdUI);
        afx_msg void OnViewPositionSpots();
        afx_msg void OnUpdateViewPositionSpots(CCmdUI* pCmdUI);
        afx_msg void OnViewDrawingTools();
        afx_msg void OnUpdateViewDrawingTools(CCmdUI* pCmdUI);
        afx_msg void OnMmPlaceFinishLine();
        afx_msg void OnUpdateMmPlaceFinishLine(CCmdUI* pCmdUI);
        afx_msg void OnViewSettings();
        afx_msg void OnUpdateViewSettings(CCmdUI* pCmdUI);
        afx_msg void OnHelpResetlaygo();
        afx_msg void OnHelpDisablelaygo();
        afx_msg void OnUpdateHelpDisablelaygo(CCmdUI* pCmdUI);
        afx_msg void OnLinePath();
        afx_msg void OnUpdateLinePath(CCmdUI* pCmdUI);
        afx_msg void OnUpdatePowerOff(CCmdUI* pCmdUI);
        afx_msg void OnUpdateSearchArea(CCmdUI* pCmdUI);
        afx_msg void OnFzcDropdownComFlowzapFile();
        afx_msg void OnFzcSelchangeComFlowzapFile();
        afx_msg void OnLoadToolbars();
        afx_msg void OnSaveToolbars();
        afx_msg void OnDrawLine();
        afx_msg void OnDrawRectangle();
        afx_msg void OnDrawPenSize();
        afx_msg void OnDrawPenStyle();
        afx_msg void OnDrawFreehand();
        afx_msg void OnUpdateDrawFreehand(CCmdUI* pCmdUI);
        afx_msg void OnUpdateDrawLine(CCmdUI* pCmdUI);
        afx_msg void OnUpdateDrawRectangle(CCmdUI* pCmdUI);
        afx_msg void OnMMMoveRobot();
        afx_msg void OnUpdateMMMoveRobot(CCmdUI* pCmdUI);
        afx_msg void OnPaint();
        afx_msg void OnViewMapObjectsToolbar();
        afx_msg void OnUpdateViewMapObjectsToolbar(CCmdUI* pCmdUI);
        afx_msg void OnViewZapControls();
        afx_msg void OnUpdateViewZapControls(CCmdUI* pCmdUI);
        afx_msg void OnViewFlowZap();
        afx_msg void OnViewInstrumentPanel();
        afx_msg void OnUpdateViewInstrumentPanel(CCmdUI* pCmdUI);
        afx_msg void OnHelpLaygoShowWelcomeTip();
        afx_msg void OnHelpLaygoShowRobotColorTip();
        afx_msg void OnHelpLaygoShowComPortSearchTip();
        afx_msg void OnHelpLaygoShowCreateHomeBaseTip();
        afx_msg void OnHelpLaygoShowNavigateTip();
        afx_msg void OnHelpLaygoShowOccupancyGridTip();
        afx_msg void OnHelpLaygoShowPositionUncertaintyTip();
        afx_msg void OnHelpLaygoShowCheckPointTip();
        afx_msg void OnHelpLaygoShowAdditionalCheckPointTip();
        afx_msg void OnHelpLaygoShowHotPointTip();
        afx_msg void OnHelpLaygoShowVacuumPathsTip();
        afx_msg void OnUpdateHelpLaygoShowWelcomeTip(CCmdUI* pCmdUI);
        afx_msg void OnUpdateHelpLaygoShowRobotColorTip(CCmdUI* pCmdUI);
        afx_msg void OnUpdateHelpLaygoShowComPortSearchTip(CCmdUI* pCmdUI);
        afx_msg void OnUpdateHelpLaygoShowCreateHomeBaseTip(CCmdUI* pCmdUI);
        afx_msg void OnUpdateHelpLaygoShowNavigateTip(CCmdUI* pCmdUI);
        afx_msg void OnUpdateHelpLaygoShowOccupancyGridTip(CCmdUI* pCmdUI);
        afx_msg void OnUpdateHelpLaygoShowPositionUncertaintyTip(CCmdUI* pCmdUI);
        afx_msg void OnUpdateHelpLaygoShowCheckPointTip(CCmdUI* pCmdUI);
        afx_msg void OnUpdateHelpLaygoShowAdditionalCheckPointTip(CCmdUI* pCmdUI);
        afx_msg void OnUpdateHelpLaygoShowHotPointTip(CCmdUI* pCmdUI);
        afx_msg void OnUpdateHelpLaygoShowVacuumPathsTip(CCmdUI* pCmdUI);
        afx_msg void OnViewLinePaths();
        afx_msg void OnUpdateViewLinePaths(CCmdUI* pCmdUI);
        afx_msg void OnHelpSamplesCyeclock();
        afx_msg void OnHelpSamplesCyeguide();
        afx_msg void OnHelpSamplesCyepatrol();
        afx_msg void OnHelpSamplesCyepost();
        afx_msg void OnHelpSamplesCyepup();
        afx_msg void OnHelpSamplesCyeserve();
        afx_msg void OnHelpSamplesCyetruck();
        afx_msg void OnUpdateHelpSamplesCyeclock(CCmdUI* pCmdUI);
        afx_msg void OnUpdateHelpSamplesCyeserve(CCmdUI* pCmdUI);
        afx_msg void OnUpdateHelpSamplesCyepatrol(CCmdUI* pCmdUI);
        afx_msg void OnDestroy();
        afx_msg void OnViewRulerGrid();
        afx_msg void OnUpdateViewRulerGrid(CCmdUI* pCmdUI);
        afx_msg void OnMmPlaceStartLine();
        afx_msg void OnUpdateMmPlaceStartLine(CCmdUI* pCmdUI);
        afx_msg void OnHelpLaygoShowDangerZonesTip();
        afx_msg void OnUpdateHelpLaygoShowDangerZonesTip(CCmdUI* pCmdUI);
        afx_msg void OnEditProperties();
        afx_msg void OnUpdateEditProperties(CCmdUI* pCmdUI);
        afx_msg void OnEditDelete();
        afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
        afx_msg void OnDrawPenClear();
        afx_msg void OnUpdateDrawPenClear(CCmdUI* pCmdUI);
        afx_msg void OnDrawPenObstacle();
        afx_msg void OnUpdateDrawPenObstacle(CCmdUI* pCmdUI);
        afx_msg void OnDrawPenUnknown();
        afx_msg void OnUpdateDrawPenUnknown(CCmdUI* pCmdUI);
        afx_msg void OnDrawPenWidth1();
        afx_msg void OnUpdateDrawPenWidth1(CCmdUI* pCmdUI);
        afx_msg void OnDrawPenWidth2();
        afx_msg void OnUpdateDrawPenWidth2(CCmdUI* pCmdUI);
        afx_msg void OnDrawPenWidth3();
        afx_msg void OnUpdateDrawPenWidth3(CCmdUI* pCmdUI);
        afx_msg void OnDrawPenWidth4();
        afx_msg void OnUpdateDrawPenWidth4(CCmdUI* pCmdUI);
        afx_msg void OnAdjustCheckPoint();
        afx_msg void OnUpdateAdjustCheckPoint(CCmdUI* pCmdUI);
        afx_msg void OnHelpLaygoShowFlowzapTip();
        afx_msg void OnUpdateHelpLaygoShowFlowzapTip(CCmdUI* pCmdUI);
        afx_msg void OnExternalOutputOn();
        afx_msg void OnUpdateExternalOutputOn(CCmdUI* pCmdUI);
        afx_msg void OnExternalOutputOff();
        afx_msg void OnUpdateExternalOutputOff(CCmdUI* pCmdUI);
        afx_msg void OnViewAddRobot();
        afx_msg void OnUpdateViewAddRobot(CCmdUI* pCmdUI);
        afx_msg void OnViewAddHomebase();
        afx_msg void OnUpdateViewAddHomebase(CCmdUI* pCmdUI);
        afx_msg void OnShowVideoButton();
        //}}AFX_MSG
        afx_msg void OnUpdateFileMenu(CCmdUI* pCmdUI);
        afx_msg void OnUpdateFileMRUMenu(CCmdUI* pCmdUI);
        DECLARE_MESSAGE_MAP()
    private:
        bool RobotWaiting(void);
        void ArrangeToolbarsDefault(void);
        bool m_FirstShowing;
        CString m_OldFzcFile;
        short m_MaxSpeed;
        BOOL InitStatusBar(UINT *pIndicators, int nSize, int nSeconds);
        CDlgCalibrateSurface* m_DlgCalibrateSurface;
};

/////////////////////////////////////////////////////////////////////////////
