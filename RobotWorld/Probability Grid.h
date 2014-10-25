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
#ifndef PROBGRID09342lksjdfg09238aslakjh389461093806kjsrhdgnzxcnv

#define PROBGRID09342lksjdfg09238aslakjh389461093806kjsrhdgnzxcnv

#include <stdio.h>
#include "World.h"
#include "..\vendor\ddraw.h"
#include "..\vendor\ddutil2.h"
#include "RothMath.h"
#include "WorldGridConsts.h"
#include "Calibration Zone.h"	// Added by ClassView
#include "robot.h"
//#include "VacuumPatch.h"

const float cZoomLevel1 = 1.0f;
const float cZoomLevel2 = 2.0f;
const float cZoomLevel3 = 4.0f;
const short cMaxRobots = 14;	//The maximum number of robots that Map-N-Zap can control.


//int HeadingToFrame(double Heading);

class CRWGraphicObjectList;
class CExecutableRWGraphicObjectList;
class CRobotWorldView;
class CRobot;

class CPixelValue
{
    public:
        int R;
        int G;
        int B;
        long NumberOfValues;
        CPixelValue* Next;
        CPixelValue(int Red, int Green, int Blue);
};

class CPixelValueList
{
    public :
        unsigned long NumberOfValues(void);
        CPixelValue** RankedPixelValues;
        void RankPixelValues(void);
        void AddPixelValue(int R, int G, int B);
        void PrintPixelValues();
        CPixelValue** PixelValues;
        long nNodes;
        CPixelValueList(long NumberNodes);
        ~CPixelValueList();
};

class CProbabilityGrid : public CObject
{
    public:
        void SetAddingHomeBase(bool Adding)
        {
            m_AddingHomeBase = Adding;
        };
        bool GetAddingHomeBase(void)
        {
            return m_AddingHomeBase;
        };
        void SetRobotsEnableDancing(bool Enable);
        CHomeBase* GetClosestUnoccupiedHomeBase(robPOINT Position);
        CHomeBase* GetClosestHomeBase(robPOINT Position);
        bool DeleteRobot(CRobot* Robot);
        void SetRobotAnimate(bool Enable);
        bool GetRobotAnimate(void);
        void SetRobotsUpdateProbabilityGrid(bool Update);
        void SetRobotsFlipHandle(bool FlipHandle);
        void SetRobotsMaxSpeed(short Speed);
        short GetActiveRobotIndex(void);
        CRobot* PointInsideRobot(robPOINT Position);
        void PurgeRobots(void);
        bool AddRobot(CRobot* Robot);
        void AnimateRobots(void);
        short GetNumberOfRobots(void);
        CRobot* GetRobot(short Index);
        void SetActiveRobot(CRobot* Robot)
        {
            m_ActiveRobot = Robot;
        };
        CRobot* GetActiveRobot(void)
        {
            return m_ActiveRobot;
        };
        DWORD GetSurfaceTopCornerColor(LPDIRECTDRAWSURFACE3 pdds);
        void GetGridValues(robPOINT Center, int Width, int Height, int **Values);
        HRESULT TransparentBlt(LPDIRECTDRAWSURFACE3 BackBuffer, CRect destRect, LPDIRECTDRAWSURFACE3 Bmp, CRect rcRect, COLORREF TransparentColor);
        HRESULT TransparentBlt(LPDIRECTDRAWSURFACE3 BackBuffer, CRect destRect, LPDIRECTDRAWSURFACE3 Bmp, CRect rcRect, DDCOLORKEY TransparentColor);
        HRESULT Blt(LPDIRECTDRAWSURFACE3 BackBuffer, CRect destRect, LPDIRECTDRAWSURFACE3 Bmp, CRect rcRect);
        float GetZoomLevel(void);
        CRect RobotToClientCoords(robRECT RobotRect);
        HBITMAP GetLargeRobotBitmap(void);
        float GetPercentExploredArea(void);
        static void DDrawReleaseSurfaceDC(CDC* pDC, LPDIRECTDRAWSURFACE3 Surface);
        static CDC* DDrawGetSurfaceDC(LPDIRECTDRAWSURFACE3 Surface);
        CExecutableRWGraphicObjectList* m_ExecutableObjectList;
        double ClientSizeToRobotSize(int ClientSize);
        HRESULT DDrawReloadBitmap(IDirectDrawSurface3 *pdds, UINT ResourceID);
        IDirectDrawSurface3* DDrawLoadBitmap(UINT ResourceID);
        void DDrawDiagnostic(HRESULT ddrval, bool ShowMessage = false);
        HRESULT DDSetColorKey(IDirectDrawSurface3 * pdds, COLORREF rgb);
        int RobotToBitmapX(double X);
        int RobotToBitmapY(double Y);
        void SetGridValue(robPOINT StartRobotCoord, robPOINT EndRobotCoord, int ProbValue, int Width = 1, bool ReplaceReservedValues = false);
        void SetGridValue(robPOINT RobotCoord, int ProbValue, int Width = 1, bool ReplaceReservedValues = false);
        void SetGridValue(robRECT RobotCoord, int ProbValue, bool ReplaceReservedValues = false);
        int GetGridValue(robPOINT RobotCoord);
        void ProbGridToFile(void);
        void New(void);
        void RestorePalette(void);
        void Serialize(CArchive & archive);
        void SerializeV2(CArchive & archive);
        void RedrawRobot(robPOINT RobotPosition, float RobotHeading, CWnd* pWnd);
        robPOINT ClientToRobotCoords(CPoint point);
        CPoint RobotToClientCoords(robPOINT point);
        ~CProbabilityGrid();
        CProbabilityGrid(HWND hAppMainWnd, CRobotWorldView* ViewWindow);
        World* m_RobotWorld;
        CPalette m_LogicalPalette;
        float m_ZoomLevel;
        void * m_ProbGridBmpValues;
        int m_DisplayBitsPerPixel;
        CRobotWorldView* m_ViewWindow;
        void TransferWorldToDisplay(CRect* pUpdateRect, CWnd* pWnd);
        CRWGraphicObjectList* m_RWGraphicObjectList;
        //	CRobot* m_Robot;
        POINT RobotToDDSurface(robPOINT RobotPoint, CPoint SurfaceOffset);
        robPOINT ProbGridToRobotCoord(POINT ProbGridPoint);
        robRECT ProbGridToRobotCoord(CRect ProbGridRect);
        float RobotToDDSurfaceY(double Y, int SurfaceYOffset);
        float RobotToDDSurfaceX(double X, int SurfaceXOffset);
        unsigned long ProbabilityToColor(unsigned char Probability);
        void SetBitmapValues(unsigned int row, unsigned int column, long Value);
        void ProbGridBmpToClientCoordinates(CRect* Rect);
        void RectifyRectangles(CRect* rcRect, CRect* destRect);
        void RectifyStretchedRectangles(CRect* rcRect, CRect* destRect);
        //	LPDIRECTDRAWSURFACE m_lpDDSArrowBmp1;
        //	LPDIRECTDRAWSURFACE m_lpDDSArrowBmp2;
        //	LPDIRECTDRAWSURFACE m_lpDDSArrowBmp3;
        //	LPDIRECTDRAWSURFACE m_lpDDSCalibrationZoneBmp;
    private:
        bool m_AddingHomeBase;
        bool m_RobotAnimate;
        float m_OldZoomLevel;
        TRobotColor m_OldColor[cMaxRobots];
        CRobot* m_ActiveRobot;
        CRobot* m_RobotList[cMaxRobots];
        bool CheckDDCapabilities(DDCAPS HardwareCaps, DDCAPS HELCaps);
        DWORD m_RobotTransparencyColor;
        void SketchRobotError(robPOINT* Corners, robPOINT DPosition, CPoint SurfaceOffset, CDC* dc);
        void AddColorsToPalette(CPixelValueList* PixelValues);
        void LoadRobotBitmaps(void);
        unsigned short COLOR16(unsigned char R, unsigned char G, unsigned char B);
        unsigned long COLOR32(unsigned char R, unsigned char G, unsigned char B);
        unsigned long COLOR24(unsigned char R, unsigned char G, unsigned char B)
        {
            return COLOR32(R, G, B);
        };
        unsigned long m_DisplayRBitMask;
        LPDIRECTDRAWSURFACE3 m_lpDDSPrimary;
        LPDIRECTDRAWSURFACE3 m_lpDDSBack;
        LPDIRECTDRAWSURFACE3 m_lpDDSWorldBmp;
        LPDIRECTDRAWSURFACE3 m_lpDDSRobotBmp[cMaxRobots];
        LPDIRECTDRAWSURFACE3 m_lpDDSInactiveRobotBmp;
        //	LPDIRECTDRAWSURFACE m_lpDDSRobotBmp2;
        //	LPDIRECTDRAWSURFACE m_lpDDSRobotBmp3;
        LPDIRECTDRAWCLIPPER m_lpDDCClipper;
        LPDIRECTDRAW2 m_lpDD;
        void InitializeProbGridBmpValues(void);
        void ClientToProbGridBmpCoords(CRect* Rect);
        HRESULT DDrawSetSurfaceValues(IDirectDrawSurface3* lpDDSDestination, CRect DestRect, void* Values);
        //	void DrawVacuumPatch(CVacuumPatch* VacuumPatch, CRect ProbGridBmpUpdateRect, CDC* pDC);
        void RectifyPoint(CPoint* point);
#ifdef JUNK
        void TransferCalibrationZonesToSurface(CRect ProbGridBmpUpdateRect);
#endif
        LPDIRECTDRAWPALETTE m_lpDDPal;
        CPoint m_DisplayResolution;
        void InitializeDirectDrawSurfaces();
        void InitializeDirectDraw(HWND hAppMainWnd, HWND hViewWindow);
        HRESULT TransferProbGridToSurface(IDirectDrawSurface3* lpDDSDestination, CRect DestRect, CPoint ValuesOffset);
        void CreateLogicalPalette(HWND hAppWnd);
        DECLARE_SERIAL(CProbabilityGrid);
        CProbabilityGrid()
        {
            return;
        };
    protected:
        HBITMAP m_hbmLargeRobot[cMaxRobots];

};


#endif