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

   #include "stdafx.h"
   #include "global.h"
   #include <fstream>
   #include <iomanip>
   #include "RobotWorld.h"
   #include "RRDim.h"
   #include "Probability Grid.h"
   #include "RobotWorld Resource.h"
   #include <stdio.h>
   #include "Path.h"
   #include "Robot.h"
   //#include "FindPathThread.h"
   #include "RWGraphicObject.h"
   #include "ExecutableRWGraphicObject.h"
   #include "PathList.h"
   #include "RobotWorldView.h"
   #include "HomeBase.h"
   /*******************************************************************************
   //Created: 10/24/97 S.R.
   //Last Revision: 10/24/97 S.R.
   //Parameters:
   //	none.
   //Return Value:
   //	none.
   ********************************************************************************
   //Description:
   //
   //Method:
   //
   *******************************************************************************/
   IMPLEMENT_SERIAL(CProbabilityGrid, CObject, VERSIONABLE_SCHEMA | VERSION_NUMBER)
   
   const unsigned char cDarkShiftPercent = 35;
   const unsigned char nStandardColors = 20;
   const unsigned int cMaxPaletteEntries = 256;
   //const unsigned char cSizeProbabilityPalette = cMaxPaletteEntries - nStandardColors - 10;
   const unsigned char cSizeProbabilityPalette = 100;
   
   const unsigned long cRobotBmp1FrameWidth = 10;
   const unsigned long cRobotBmp1FrameHeight = 10;
   const unsigned long cRobotBmp1FramePerLine = 5;
   //const unsigned long cRobotBmp2FrameWidth = 19;
   //const unsigned long cRobotBmp2FrameHeight = 19;
   const unsigned long cRobotBmp2FrameWidth = 20;
   const unsigned long cRobotBmp2FrameHeight = 20;
   const unsigned long cRobotBmp2FramePerLine = 5;
   const unsigned long cRobotBmp3FrameWidth = 40;
   const unsigned long cRobotBmp3FrameHeight = 40;
   //const unsigned long cRobotBmp3FrameWidth = 38;
   //const unsigned long cRobotBmp3FrameHeight = 38;
   const unsigned long cRobotBmp3FramePerLine = 5;
   const unsigned long cTotalFrames = 40;
   const unsigned long cTransparencyColor = 0xFFFFFF;
   
   const DWORD cBlack =	0X000000;
   const DWORD cBlue =	0XFF0000;
   const DWORD cGreen =	0X00FF00;
   const DWORD cCyan =	0XFFFF00;
   const DWORD cRed =	0X0000FF;
   const DWORD cMagenta =	0XFF00FF;
   const DWORD cYellow =	0X00FFFF;
   const DWORD cWhite =	0XFFFFFF;
   const DWORD cDarkBlue =	0X800000;
   const DWORD cDarkGreen =	0X008000;
   const DWORD cDarkCyan =	0X808000;
   const DWORD cDarkRed =	0X000080;
   const DWORD cDarkMagenta =	0X800080;
   const DWORD cDarkYellow =	0X008080;
   const DWORD cDarkGray =	0X808080;
   const DWORD cLightGray =	0XC0C0C0;
   const DWORD cMoneyGreen =	0XC0DCC0;
   const DWORD cSkyBlue =	0XF0CAA6;
   const DWORD cCream =	0XF0FBFF;
   const DWORD cMediumGray =	0XA4A0A0;
   
   
   
   IDirectDrawSurface* DDrawLoadBitmap(IDirectDraw *pdd, CBitmap* tempBmap);
   IDirectDrawSurface* DDrawLoadBitmap(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy);
   HRESULT DDrawReLoadBitmap(IDirectDrawSurface *pdds, CBitmap* tempBmap);
   HRESULT DDrawCopyBitmap(IDirectDrawSurface *pdds, HBITMAP hbm, int x, int y, int dx, int dy);
   IDirectDrawSurface3* DDrawLoadBitmap(IDirectDraw2 *pdd, CBitmap* tempBmap);
   IDirectDrawSurface3* DDrawLoadBitmap(IDirectDraw2 *pdd, LPCSTR szBitmap, int dx, int dy);
   HRESULT DDrawReLoadBitmap(IDirectDrawSurface3 *pdds, CBitmap* tempBmap);
   HRESULT DDrawCopyBitmap(IDirectDrawSurface3 *pdds, HBITMAP hbm, int x, int y, int dx, int dy);
   void DDrawDiagnostic(HRESULT ddrval, bool ShowMessage = false);
   
   CProbabilityGrid* GlobalProbGrid = NULL;
   void ChangeScreen(int row, int col, CharMatrix &m, CharMatrix &world);
   extern void ShowPath(Path* path, int type);
   
   void ChangeScreen(int row, int col, CharMatrix &m, CharMatrix &world)
   /*******************************************************************************
   //Created: 05/13/97 S.R.
   //Last Revision: 05/15/97 S.R.
   //Parameters:
   //	x, y - the location of the bottom left corner of the
   //		       new values relative to the bottom left of world
   //	m - the matrix of new values to display on the screen
   //			(m[0][0] world be drawn at world[x][y]
   //			 world[0][0] is bottom left of screen
   //			 world[0][grid_y-1] is bottom right of screen)
   //	world - the matrix of the current world values.
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //	This function is called every time the probability grid is updated.
   //Method:
   //
   *******************************************************************************/
   {
   	for (int MatrixRow = row; MatrixRow < row + m.NumberOfRows(); MatrixRow++) {
   		ASSERT( (MatrixRow >= 0) && (MatrixRow < cWorldGridXSize) );
   		unsigned char* XValues = world[MatrixRow];
   		for (int MatrixColumn = col; MatrixColumn < col + m.NumberOfColumns(); MatrixColumn++) {
   			ASSERT( (MatrixColumn >= 0) && (MatrixColumn < cWorldGridYSize) );
   			GlobalProbGrid->SetBitmapValues(cWorldGridYSize - 1 - MatrixColumn, MatrixRow, GlobalProbGrid->ProbabilityToColor(XValues[MatrixColumn]));
   		}
   	}
   	CRect UpdateRect(row - 4,cWorldGridYSize - (col + m.NumberOfColumns()) - 4,row + m.NumberOfRows() + 4,cWorldGridYSize - col + 4);
   	UpdateRect.NormalizeRect();
   	GlobalProbGrid->ProbGridBmpToClientCoordinates(&UpdateRect);
   	GlobalProbGrid->m_ViewWindow->RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
   	return;
   }
   
   CProbabilityGrid::CProbabilityGrid(HWND hAppMainWnd, CRobotWorldView* ViewWindow)
   /*******************************************************************************
   //Created:  S.R.
   //Last Revision:  S.R.
   //Parameters:
   //	hAppMainWnd:	Window handle used for the application.
   //	ViewWindow:		pointer to the view window.
   //	CalZoneList:	pointer to the calibration zone list.
   //	VacuumPatchList:pointer to the vacuum patch list.
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //
   //Method:
   //
   *******************************************************************************/
   {
   	m_AddingHomeBase = false;
   	m_RobotAnimate = ((CRobotWorldApp*)AfxGetApp())->GetRobotAnimate();
   	m_OldZoomLevel = 0.0f;
   	for (int i = 0; i < cMaxRobots; i++) {
   		m_RobotList[i] = NULL;
   		m_hbmLargeRobot[i] = NULL;
   		m_OldColor[i] = crNone;
   		m_lpDDSRobotBmp[i] = NULL;
   	}
   	m_lpDDSInactiveRobotBmp = NULL;
   //Create and initialize the m_Robot
   	m_ActiveRobot = new CRobot(ViewWindow, this);
   	m_RobotList[0] = m_ActiveRobot;
   	ASSERT(m_ActiveRobot != NULL);
   //	m_ProbabilityGrid->m_Robot = m_Robot;
   	m_ActiveRobot->Create(ViewWindow);
   
   	m_lpDDSPrimary		= NULL;
   	m_lpDDSBack			= NULL;
   	m_lpDDSWorldBmp	= NULL;
   //	m_lpDDSRobotBmp2	= NULL;
   //	m_lpDDSRobotBmp3	= NULL;
   //	m_lpDDSCalibrationZoneBmp = NULL;
   //	m_lpDDSArrowBmp1	= NULL;
   //	m_lpDDSArrowBmp2	= NULL;
   //	m_lpDDSArrowBmp3	= NULL;
   	m_lpDDCClipper		= NULL;			//	Clipper region
   	m_lpDD				= NULL;
   //	m_lpDD2				= NULL;
   	m_lpDDPal = NULL;
   	m_ProbGridBmpValues		= NULL;
   
   	GlobalProbGrid = this;
   /*Initialize Other Stuff*/
   	m_ViewWindow = ViewWindow;
   	m_ZoomLevel = cZoomLevel2;
   //	m_pBitmap = NULL;
   //	m_DisplayResolution.x = 1024;
   //	m_DisplayResolution.y = 768;
   	m_DisplayResolution.x = 1600;
   	m_DisplayResolution.y = 1200;
   //	m_DisplayBitsPerPixel = 16;
   //	m_DisplayBitsPerPixel = 8;
   	InitializeDirectDraw(hAppMainWnd, ViewWindow->GetSafeHwnd());
   	fprintf(stderr, "CProbabilityGrid:: Initialized the DirectDraw stuff\n");
   /*Create Jim's probability Grid stuff*/
   	/*Call to initialize Jim Hemmerle's .dlls*/
   	// BootList();
   
   	Grid grid;
   	Rover rover;
   	char str[32];
   	CFile RoverFileTest;
   
   	CString datapath = ((CRobotWorldApp*) AfxGetApp())->m_DefaultPath + "\\rover.dat";
   	if (!RoverFileTest.Open(datapath, CFile::modeRead)) {
   		AfxMessageBox("Rover.dat File Opening Error");
   		ASSERT(FALSE);
   	}
   	RoverFileTest.Close();
	std::ifstream rover_file(datapath);
   	rover_file >> str >> grid;
   
   	rover.SetGrid(Body, grid);
   	for (int i=M0_F; i <= M2_B; i++) {
   		rover_file >> str >> grid;
   		rover.SetGrid((RoverGrid)i, grid);
   	}
   
      m_RobotWorld = new World(cWorldGridXSize, cWorldGridYSize, cWorldGridPointSize,
   		rover, cWorldGridInitialValue, cMinProb, cMaxProb);
   	CPathList::world = m_RobotWorld;
   
   	m_RobotWorld->SetOrigin(m_RobotWorld->NumberOfRows()/2, m_RobotWorld->NumberOfColumns()/2);
   	m_RobotWorld->ChangeScreen = &ChangeScreen;
   	m_RobotWorld->ShowPath = CFindPathThread::ShowPath;
   	m_RobotWorld->rover.SetPathBuffer(cDefaultPathBuffer);	/*Set Path Creation Buffer Zone*/
   
   	fprintf(stderr, "CProbabilityGrid:: Created robot world\n");
   /*Do more of my stuff*/
   	m_RWGraphicObjectList = new CRWGraphicObjectList();
   	m_RWGraphicObjectList->m_ProbabilityGrid = this;
   	m_ExecutableObjectList = new CExecutableRWGraphicObjectList();
   	m_ExecutableObjectList->m_ProbabilityGrid = this;
   //	m_Robot = Robot;
   }
   
   CProbabilityGrid::~CProbabilityGrid()
   {
   	m_lpDDSBack->Release();
   	for (int i = 0; i < cMaxRobots; i++) {
   		if (m_lpDDSRobotBmp[i] != NULL) {
   			m_lpDDSRobotBmp[i]->Release();
   			m_lpDDSRobotBmp[i] = NULL;
   		}
   	}
   	m_lpDDSInactiveRobotBmp->Release();
   //	m_lpDDSRobotBmp2->Release();
   //	m_lpDDSRobotBmp3->Release();
   	m_lpDDSWorldBmp->Release();
   //	m_lpDDSWorldUpdateBmp->Release();
   	m_lpDD->RestoreDisplayMode();
   	PurgeRobots();
   	delete m_ProbGridBmpValues;
   	delete m_RobotWorld;
   	/*Remove all objects from the m_RWGraphicObjectList before deleting it*/
   	delete m_RWGraphicObjectList;
   	delete m_ExecutableObjectList;
   }
   
   unsigned short CProbabilityGrid::COLOR16(unsigned char R, unsigned char G, unsigned char B)
   /******************************************************************************/
   /*03/23/98 S.R.
   /*This function takes R,G,B values and turns them into a pixel value for the display card*/
   /*This function works when the display card is in 16 bpp mode*/
   /*R,G,B must be in range [0,31]*/
   /******************************************************************************/
   {
   #ifdef _DEBUG
   	if (R > 31) {
   		fprintf(stderr, "COLOR16 ERROR!!!! R value out of range\n");
   		R = 31;
   	}
   	if (G > 31) {
   		fprintf(stderr, "COLOR16 ERROR!!!! G value out of range\n");
   		G = 31;
   	}
   	if (B > 31) {
   		fprintf(stderr, "COLOR16 ERROR!!!! B value out of range\n");
   		B = 31;
   	}
   #endif
   
   	if (m_DisplayRBitMask == 0x7C00) {
   		return (((unsigned short)(B)) | (((unsigned short)(G)) << 5) | (((unsigned short)(R)) << 10));
   	} else if (m_DisplayRBitMask == 0xF800) {
   		return (((unsigned short)(B)) | (((unsigned short)(G)) << 6) | (((unsigned short)(R)) << 11));
   	} else {
   #ifdef _DEBUG
   		fprintf(stderr, "COLOR16 ERROR!!! Unsupported RBitMask\n");
   		return (0);
   #else
   		return (((unsigned short)(B)) | (((unsigned short)(G)) << 6) | (((unsigned short)(R)) << 11));
   #endif
   	}
   }
   
   unsigned long CProbabilityGrid::COLOR32(unsigned char R, unsigned char G, unsigned char B)
   /******************************************************************************/
   /*03/23/98 S.R.
   /*This function takes R,G,B values and turns them into a pixel value for the display card*/
   /*This function works when the display card is in 32 bpp mode*/
   /*R,G,B must be in range [0,255]*/
   /******************************************************************************/
   //R,G,B range [0,255]
   {
   #ifdef _DEBUG
   	if (R > 255) {
   		fprintf(stderr, "COLOR32 ERROR!!!! R value out of range\n");
   		R = 255;
   	}
   	if (G > 255) {
   		fprintf(stderr, "COLOR32 ERROR!!!! G value out of range\n");
   		G = 255;
   	}
   	if (B > 255) {
   		fprintf(stderr, "COLOR32 ERROR!!!! B value out of range\n");
   		B = 255;
   	}
   #endif
   	if (m_DisplayRBitMask == 0xFF0000) {
   		return (((unsigned long)(B)) | (((unsigned long)(G)) << 8) | (((unsigned long)(R)) << 16));
   	} else if (m_DisplayRBitMask == 0xFF) {
   		return (((unsigned long)(R)) | (((unsigned long)(G)) << 8) | (((unsigned long)(B)) << 16));
   	} else if (m_DisplayRBitMask == 0x7C00) {
   		return (((unsigned short)(B)) | (((unsigned short)(G)) << 5) | (((unsigned short)(R)) << 10));
   	} else if (m_DisplayRBitMask == 0xF800) {
   		return (((unsigned short)(B)) | (((unsigned short)(G)) << 6) | (((unsigned short)(R)) << 11));
   	} else {
   #ifdef _DEBUG
   		fprintf(stderr, "COLOR32 ERROR!!! Unsupported RBitMask\n");
   		return 0;
   #else
   		return (((unsigned long)(B)) | (((unsigned long)(G)) << 8) | (((unsigned long)(R)) << 16));
   #endif
   	}
   
   }
   
   unsigned long CProbabilityGrid::ProbabilityToColor(unsigned char Probability)
   /*******************************************************************************
   //Created: 05/14/97 S.R.
   //Last Revision: 05/15/97 S.R.
   //Parameters:
   //	Probability:	the probability that needs to be turned into a display color.
   //Return Value:
   //		The color corresponding to the probability.  This can be a palette number
   //	or RGB number depending on the display type.
   ********************************************************************************
   //Description:
   //		This function figures out what color should be displayed for a given
   //	occupancy probability.
   //Method:
   //
   *******************************************************************************/
   {
   //#define COLOR16(R,G,B) (((unsigned short)(B)) | (((unsigned short)(G)) << 6) | (((unsigned short)(R)) << 11))	//R,G,B range [0,31]
   //#define COLOR16(R,G,B) (((unsigned short)(B)) | (((unsigned short)(G)) << 5) | (((unsigned short)(R)) << 10))	//R,G,B range [0,31]
   //#define COLOR32(R,G,B) (((unsigned long)(B)) | (((unsigned long)(G)) << 8) | (((unsigned long)(R)) << 16))		//R,G,B range [0,255]
   
   	const unsigned char cMinColor8 = 10;
   	const unsigned char cMaxColor8 = cSizeProbabilityPalette + 9;
   	const float m8 = ((float)cMaxColor8 - cMinColor8) / (cMaxProb - cMinProb);
   		
   	const unsigned char cMaxColor16 = 31;
   	const unsigned char cMinColor16 = ((long)cMaxColor16) * cDarkShiftPercent/100;
   	const float m16 = ((float)cMaxColor16 - cMinColor16) / (cMaxProb - cMinProb);
   
   	const unsigned char cMaxColor32 = 0xFF;
   	const unsigned char cMinColor32 = ((long)cMaxColor32) * cDarkShiftPercent / 100;
   	const float m32 = ((float)cMaxColor32 - cMinColor32) / (cMaxProb - cMinProb);
   
   	unsigned char temp;
   
   	switch (m_DisplayBitsPerPixel) {
   	case 8:
   		if (Probability <= cMinProb) {
   			return Round(m8*cMinProb + cMinColor8 - m8 * cMinProb);
   		} else if (Probability >= cMaxProb) {
   			return Round(m8*cMaxProb + cMinColor8 - m8 * cMinProb);
   		} else {
   			return Round(m8*Probability + cMinColor8 - m8 * cMinProb);
   		}
   		break;
   	case 16:
   		if (Probability <= cMinProb) {
   //			temp = Round(m16*cMinProb + cMinColor16 - m16 * cMinProb);
   			return COLOR16(0, 0, 0);
   		} else if (Probability >= cMaxProb) {
   			temp = (unsigned char)Round(m16*cMaxProb + cMinColor16 - m16 * cMinProb);
   			return COLOR16(temp,temp,temp);
   		} else {
   			temp = (unsigned char)Round(m16*Probability + cMinColor16 - m16 * cMinProb);
   //			if (temp > cMaxColor16) {
   //				temp = cMaxColor16;
   //			}
   			return COLOR16(temp,temp,temp);
   //			return COLOR16(19,19,19);
   //			return COLOR16(15,15,15);
   		}
   		break;
   	case 32:
   	case 24:
   		if (Probability <= cMinProb) {
   			temp = 0;
   //			temp = Round(m32*cMinProb + cMinColor32 - m32 * cMinProb);
   			return COLOR32(temp,temp,temp);
   		} else if (Probability >= cMaxProb) {
   			temp = (unsigned char)Round(m32*cMaxProb + cMinColor32 - m32 * cMinProb);
   			return COLOR32(temp,temp,temp);
   		} else {
   			temp = (unsigned char)Round(m32*Probability + cMinColor32 - m32 * cMinProb);
   			return COLOR32(temp,temp,temp);
   		}
   		break;
   	default:
   #ifdef _DEBUG
   		fprintf(stderr, "ProbabilityToColor Error!!!! Unsupported m_DisplayBitsPerPixel\n");
   #endif
   		if (Probability <= cMinProb) {
   			temp = (unsigned char)Round(m32*cMinProb + cMinColor32 - m32 * cMinProb);
   			return COLOR32(temp,temp,temp);
   		} else if (Probability >= cMaxProb) {
   			temp = (unsigned char)Round(m32*cMaxProb + cMinColor32 - m32 * cMinProb);
   			return COLOR32(temp,temp,temp);
   		} else {
   			temp = (unsigned char)Round(m32*Probability + cMinColor32 - m32 * cMinProb);
   			return COLOR32(temp,temp,temp);
   		}
   		break;
   	}
   }
   
   void CProbabilityGrid::SetBitmapValues(unsigned int row, unsigned int column, long Value)
   /*******************************************************************************
   //Created: 05/14/97 S.R.
   //Last Revision: 05/15/97 S.R.
   //Parameters:
   //	row, column:	the row and column of the bitmap value to set.
   //	Value:			the value to set the bitmap pixel to.
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //	Sets the bitmap pixel to a value.
   //Method:
   //		Depending on the number of bits per pixel in the bitmap, the function sets
   //	the corresponding array value to Value.
   *******************************************************************************/
   {
   	ASSERT((row < cWorldGridYSize) && (column < cWorldGridXSize) );
   	switch (m_DisplayBitsPerPixel) {
   	case 8:
   		((unsigned char*)m_ProbGridBmpValues)[row*cWorldGridXSize + column] = unsigned char(Value);
   		break;
   	case 16:
   		((unsigned short*)m_ProbGridBmpValues)[row*cWorldGridXSize + column] = unsigned short(Value);
   		break;
   	case 24:
   		((unsigned char*)m_ProbGridBmpValues)[row*cWorldGridXSize*3 + column*3] = unsigned char(Value);	
   		((unsigned char*)m_ProbGridBmpValues)[row*cWorldGridXSize*3 + column*3 + 1] = unsigned char(Value >> 8);
   		((unsigned char*)m_ProbGridBmpValues)[row*cWorldGridXSize*3 + column*3 + 2] = unsigned char(Value >> 16);
   		break;
   	case 32:
   		((unsigned long*)m_ProbGridBmpValues)[row*cWorldGridXSize + column] = Value;	
   		break;
   	}
   }
   
   void CProbabilityGrid::InitializeProbGridBmpValues(void)
   /*******************************************************************************
   //Created: 05/14/97 S.R.
   //Last Revision: 05/17/97 S.R.
   //Parameters:
   //	none
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		Initializes the probability grid bitmap values.
   //Method:
   //
   *******************************************************************************/
   {
   	switch (m_DisplayBitsPerPixel) {
   	case 8:
   		assert(cWorldGridXSize%2 == 0);		//Must be even for an 8-bit bitmap
   		assert(cWorldGridYSize%2 == 0);		//Must be even for an 8-bit bitmap
   		m_ProbGridBmpValues = new unsigned char[cWorldGridXSize*cWorldGridYSize];
   		break;
   	case 16:
   		m_ProbGridBmpValues = new unsigned short[cWorldGridXSize*cWorldGridYSize];
   		break;
   	case 32:
   	case 24:
   		m_ProbGridBmpValues = new unsigned long[cWorldGridXSize*cWorldGridYSize];
   		break;
   	default:
   		AfxMessageBox("Error!! Unsupported BitsPerPixel Display Mode");
   		assert(FALSE);
   		break;
   	}
   	unsigned long InitValue = ProbabilityToColor(cWorldGridInitialValue);
   	for (unsigned int i = 0; i < cWorldGridYSize; i++) {
   		for (unsigned int j = 0; j < cWorldGridXSize; j++) {
   			SetBitmapValues(i,j,InitValue);
   //			SetBitmapValues(i,j,Round(255.0/cWorldGridXSize*j));
   //			SetBitmapValues(i,j,ProbabilityToColor(Round( ((float)cMaxProb - cMinProb) / cWorldGridXSize*j + cMinProb)) );
   		}
   	}
   }
   
   void CProbabilityGrid::SketchRobotError(robPOINT* Corners, robPOINT DPosition, CPoint SurfaceOffset, CDC* dc)
   /*******************************************************************************
   //Created: 02/23/97 S.R.
   //Last Revision: 07/11/97 S.R.
   //Parameters:
   //	dc:	The DC where the robot is being displayed.
   //Return Value:
   //	none.
   ********************************************************************************
   //Description:
   //	This function sketches the error octagon on the screen.
   //Method:
   //		The function calculates the positions of all the corners of the octagon
   //	then displays it on the screen.
   *******************************************************************************/
   {
   	unsigned char i;
   	double temp[4];
   /*	robPOINT RobotPosition = m_pRobot->m_Position;
   	for (i = 0; i < 4; i++) {
   		Corners[i].x -= RobotPosition.x;
   		Corners[i].y -= RobotPosition.y;
   	}
   */
   	for (i = 0; i < 4; i++) {
   		temp[i] = Corners[i].x;
   	}
   	double MaxX = Max(temp,4);
   	double MinX = Min(temp,4);
   	for (i = 0; i < 4; i++) {
   		temp[i] = Corners[i].y;
   	}
   	double MaxY = Max(temp,4);
   	double MinY = Min(temp,4);
   
   	robPOINT PointArray[8];
   	char j = 0;
   	for (i = 0; i < 4; i++) {
   		if (Corners[i].x == MaxX) {
   			if (Corners[i].y == MaxY) {
   				PointArray[j].x = Corners[i].x + DPosition.x;
   				PointArray[j++].y = Corners[i].y + DPosition.y;
   				PointArray[j].x = Corners[i].x - DPosition.x;
   				PointArray[j++].y = Corners[i].y + DPosition.y;
   			} else if (Corners[i].y == MinY) {
   				PointArray[j].x = Corners[i].x - DPosition.x;
   				PointArray[j++].y = Corners[i].y - DPosition.y;
   				PointArray[j].x = Corners[i].x + DPosition.x;
   				PointArray[j++].y = Corners[i].y - DPosition.y;
   			} else {
   				PointArray[j].x = Corners[i].x + DPosition.x;
   				PointArray[j++].y = Corners[i].y - DPosition.y;
   				PointArray[j].x = Corners[i].x + DPosition.x;
   				PointArray[j++].y = Corners[i].y + DPosition.y;
   			}
   		} else if (Corners[i].x == MinX) {
   			if (Corners[i].y == MaxY) {
   				PointArray[j].x = Corners[i].x - DPosition.x;
   				PointArray[j++].y = Corners[i].y + DPosition.y;
   				PointArray[j].x = Corners[i].x - DPosition.x;
   				PointArray[j++].y = Corners[i].y - DPosition.y;
   			} else if (Corners[i].y == MinY) {
   				PointArray[j].x = Corners[i].x - DPosition.x;
   				PointArray[j++].y = Corners[i].y - DPosition.y;
   				PointArray[j].x = Corners[i].x + DPosition.x;
   				PointArray[j++].y = Corners[i].y - DPosition.y;
   			} else {
   				PointArray[j].x = Corners[i].x - DPosition.x;
   				PointArray[j++].y = Corners[i].y + DPosition.y;
   				PointArray[j].x = Corners[i].x - DPosition.x;
   				PointArray[j++].y = Corners[i].y - DPosition.y;
   			}
   		} else {
   			if (Corners[i].y == MaxY) {
   				PointArray[j].x = Corners[i].x + DPosition.x;
   				PointArray[j++].y = Corners[i].y + DPosition.y;
   				PointArray[j].x = Corners[i].x - DPosition.x;
   				PointArray[j++].y = Corners[i].y + DPosition.y;
   			} else if (Corners[i].y == MinY) {
   				PointArray[j].x = Corners[i].x - DPosition.x;
   				PointArray[j++].y = Corners[i].y - DPosition.y;
   				PointArray[j].x = Corners[i].x + DPosition.x;
   				PointArray[j++].y = Corners[i].y - DPosition.y;
   			}
   		}
   	}
   	POINT WindowPoint[9];
   	for (j = 0; j < 8; j++) {
   		WindowPoint[j] = RobotToDDSurface(PointArray[j], SurfaceOffset);
   	}
   	WindowPoint[8] = WindowPoint[0];
   	CPen Pen(PS_SOLID, 1, COLORREF(cYellow));
   	CPen* OldPen = dc->SelectObject(&Pen);
   	int OldROP2 = dc->SetROP2(R2_COPYPEN);
   //		CGdiObject* OldObject = dc->SelectStockObject(BLACK_PEN);
   	dc->Polyline(WindowPoint, 9);
   	dc->SelectObject(OldPen);
   	dc->SetROP2(OldROP2);
   
   	return;
   }
   
   void CProbabilityGrid::TransferWorldToDisplay(CRect* pUpdateRect, CWnd* pWnd)
   /*******************************************************************************
   //Created: 05/14/97 S.R.
   //Last Revision: 10/24/97 S.R.
   //Parameters:
   //	pUpdateRect:	The rectangle to be updated.  In client coordinates.
   //	RobotPosition:	The current robot position.
   //	RobotHeading:	The current robot heading.
   //	pWnd:			a pointer to the display window.
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		This function transfers the robot world to the screen.
   //Method:
   //
   *******************************************************************************/
   {
   	CRect rcRect;								/*resource rectangle (bitmap rectangles)*/
   	CRect destRect;								/*blit destination rectangle*/
   	CRect UpdateRect;							/*the update rectangle in client coords*/
   	HRESULT ddrval;								/*Direct Draw function error values*/
   	CRect ProbGridBmpUpdateRect;					/*Update rectangle in the world bitmap coordinates*/
   	static robPOINT OldRobotPosition(0,0);
   //	LPDIRECTDRAWSURFACE lpDDSRobotBmp;
   	unsigned long RobotBmpFrameWidth;
   	unsigned long RobotBmpFrameHeight;
   	unsigned long RobotBmpFramePerLine;
   	
   /*Get the right robot bitmap surface*/
   	if (m_ZoomLevel == cZoomLevel1) {
   //		lpDDSRobotBmp = m_lpDDSRobotBmp1;
   		RobotBmpFrameWidth = cRobotBmp1FrameWidth;
   		RobotBmpFrameHeight = cRobotBmp1FrameHeight;
   		RobotBmpFramePerLine = cRobotBmp1FramePerLine;
   	} else if (m_ZoomLevel == cZoomLevel2) {
   //		lpDDSRobotBmp = m_lpDDSRobotBmp2;
   		RobotBmpFrameWidth = cRobotBmp2FrameWidth;
   		RobotBmpFrameHeight = cRobotBmp2FrameHeight;
   		RobotBmpFramePerLine = cRobotBmp2FramePerLine;
   	} else if (m_ZoomLevel == cZoomLevel3) {
   //		lpDDSRobotBmp = m_lpDDSRobotBmp3;
   		RobotBmpFrameWidth = cRobotBmp3FrameWidth;
   		RobotBmpFrameHeight = cRobotBmp3FrameHeight;
   		RobotBmpFramePerLine = cRobotBmp3FramePerLine;
   	} else {
   		RobotBmpFrameWidth = cRobotBmp1FrameWidth;
   		RobotBmpFrameHeight = cRobotBmp1FrameHeight;
   		RobotBmpFramePerLine = cRobotBmp1FramePerLine;
   		AfxMessageBox("Illegal m_ZoomLevel");
   		assert(FALSE);
   	}
   /*Restore all the surfaces*/
   	if (m_lpDDSPrimary->IsLost() == DDERR_SURFACELOST) {
   		m_lpDDSPrimary->Restore();
   	}
   	if (m_lpDDSBack->IsLost() == DDERR_SURFACELOST) {
   		m_lpDDSBack->Restore();
   	}
   	if (m_lpDDSWorldBmp->IsLost() == DDERR_SURFACELOST) {
   		m_lpDDSWorldBmp->Restore();
   	}
   	LoadRobotBitmaps();
   	for (int i = 0; i < cMaxRobots; i++) {
   		if (m_lpDDSRobotBmp[i] != NULL) {
   			if (m_lpDDSRobotBmp[i]->IsLost() == DDERR_SURFACELOST) {
   				m_lpDDSRobotBmp[i]->Restore();
   			}
   		}
   	}
   //	if (m_lpDDSCalibrationZoneBmp->IsLost() == DDERR_SURFACELOST) {
   //		m_lpDDSCalibrationZoneBmp->Restore();
   //	}
   
   	if (pUpdateRect == NULL) {
   		pWnd->GetClientRect(&UpdateRect);
   	} else {
   		UpdateRect = *pUpdateRect;
   	}
   
   #ifdef IMPLEMENTED
   	fprintf(stderr, "****ProbabilityGrid::TransferWorldToDisplay****\n");
   	fprintf(stderr, "UpdateRect = %d, %d, %d, %d,\n", UpdateRect.left, UpdateRect.top, UpdateRect.right, UpdateRect.bottom);
   #endif
   
   //Move the update rectangle so it falls in line with the pixels in the zoomed bitmap
   	UpdateRect.left -= UpdateRect.left % (int)m_ZoomLevel;
   	UpdateRect.right += (int)m_ZoomLevel - UpdateRect.right % (int)m_ZoomLevel;
   //	UpdateRect.right += (int)m_ZoomLevel - UpdateRect.right % (int)m_ZoomLevel - 1;
   	UpdateRect.top -= UpdateRect.top % (int)m_ZoomLevel;
   	UpdateRect.bottom += (int)m_ZoomLevel - UpdateRect.bottom % (int)m_ZoomLevel;
   //	UpdateRect.bottom += (int)m_ZoomLevel - UpdateRect.bottom % (int)m_ZoomLevel - 1;
   	if (pUpdateRect != NULL) *pUpdateRect = UpdateRect;
   #ifdef IMPLEMENTED
   	fprintf(stderr, "Shift for Zoom Level\n");
   	fprintf(stderr, "UpdateRect = %d, %d, %d, %d,\n", UpdateRect.left, UpdateRect.top, UpdateRect.right, UpdateRect.bottom);
   #endif
   /*Find the boundaries of the world bitmap in the window*/
   	CRect BoundsRect;
   	pWnd->GetClientRect(&BoundsRect);
   	ClientToProbGridBmpCoords(&BoundsRect);
   	if (BoundsRect.left < 0) BoundsRect.left = 0;
   	if (BoundsRect.left > cWorldGridXSize - 1) BoundsRect.left = cWorldGridXSize - 1;
   	if (BoundsRect.right < 0) BoundsRect.right = 0;
   	if (BoundsRect.right > cWorldGridXSize - 1) BoundsRect.right = cWorldGridXSize - 1;
   	if (BoundsRect.top < 0) BoundsRect.top = 0;
   	if (BoundsRect.top > cWorldGridYSize - 1) BoundsRect.top = cWorldGridYSize - 1;
   	if (BoundsRect.bottom < 0) BoundsRect.bottom = 0;
   	if (BoundsRect.bottom > cWorldGridYSize - 1) BoundsRect.bottom = cWorldGridYSize - 1;
   /*Find the world bitmap's update rectangle*/
   	ProbGridBmpUpdateRect = UpdateRect;
   	ClientToProbGridBmpCoords(&ProbGridBmpUpdateRect);
   ///*
   	if (ProbGridBmpUpdateRect.left < BoundsRect.left) ProbGridBmpUpdateRect.left = BoundsRect.left;
   	if (ProbGridBmpUpdateRect.left > BoundsRect.right) ProbGridBmpUpdateRect.left = BoundsRect.right;
   	if (ProbGridBmpUpdateRect.right < BoundsRect.left) ProbGridBmpUpdateRect.right = BoundsRect.left;
   	if (ProbGridBmpUpdateRect.right > BoundsRect.right) ProbGridBmpUpdateRect.right = BoundsRect.right;
   	if (ProbGridBmpUpdateRect.top < BoundsRect.top) ProbGridBmpUpdateRect.top = BoundsRect.top;
   	if (ProbGridBmpUpdateRect.top > BoundsRect.bottom) ProbGridBmpUpdateRect.top = BoundsRect.bottom;
   	if (ProbGridBmpUpdateRect.bottom < BoundsRect.top) ProbGridBmpUpdateRect.bottom = BoundsRect.top;
   	if (ProbGridBmpUpdateRect.bottom > BoundsRect.bottom) ProbGridBmpUpdateRect.bottom = BoundsRect.bottom;
   //*/
   #ifdef IMPLEMENTED
   	fprintf(stderr, "Shift for Zoom Level\n");
   	fprintf(stderr, "ProbGridBmpUpdateRect = %d, %d, %d, %d,\n", ProbGridBmpUpdateRect.left, ProbGridBmpUpdateRect.top, ProbGridBmpUpdateRect.right, ProbGridBmpUpdateRect.bottom);
   #endif
   	if ((ProbGridBmpUpdateRect.Width() == 0) || (ProbGridBmpUpdateRect.Height() == 0)) return;
   /*Transfer probability Grid to back buffer*/
   	CPoint ValuesOffset;
   	ValuesOffset.x = ProbGridBmpUpdateRect.left;
   	ValuesOffset.y = ProbGridBmpUpdateRect.top;
   	destRect.left = 0;
   	destRect.top = 0;
   #define HARDWARE_ZOOM
   #ifdef HARDWARE_ZOOM
   	destRect.right = ProbGridBmpUpdateRect.Width();
   	destRect.bottom = ProbGridBmpUpdateRect.Height();
   #else
   	destRect.right = ProbGridBmpUpdateRect.Width();
   	destRect.bottom = ProbGridBmpUpdateRect.Height();
   #endif
   	ddrval = TransferProbGridToSurface(m_lpDDSWorldBmp, destRect, ValuesOffset);
   	if (ddrval != DD_OK) {
   		DDrawDiagnostic(ddrval);
   		ASSERT(FALSE);
   	}
   	destRect.left = 0;
   	destRect.right = long(ProbGridBmpUpdateRect.Width() * m_ZoomLevel);
   	destRect.top = 0;
   	destRect.bottom = long(ProbGridBmpUpdateRect.Height() * m_ZoomLevel);
   
   	rcRect.left = 0;
   	rcRect.top = 0;
   #ifdef HARDWARE_ZOOM
   	rcRect.right = ProbGridBmpUpdateRect.Width();
   	rcRect.bottom = ProbGridBmpUpdateRect.Height();
   #else
   	rcRect.right = ProbGridBmpUpdateRect.Width() * m_ZoomLevel;
   	rcRect.bottom = ProbGridBmpUpdateRect.Height() * m_ZoomLevel;
   #endif
   //		fprintf(stderr, "ProbGridBmpUpdateRect = %d %d %d %d\n", ProbGridBmpUpdateRect.left,ProbGridBmpUpdateRect.top, ProbGridBmpUpdateRect.right, ProbGridBmpUpdateRect.bottom);
   //		fprintf(stderr, "ProbGridBmpUpdateRect.Width() = %d\nProbGridBmpUpdateRect.Height() = %d\n",ProbGridBmpUpdateRect.Width(), ProbGridBmpUpdateRect.Height());
   //		fprintf(stderr, "rcRect = %d %d %d %d\n", rcRect.left,rcRect.top, rcRect.right, rcRect.bottom);
   //		fprintf(stderr, "destRect = %d %d %d %d\n", destRect.left,destRect.top, destRect.right, destRect.bottom);
   	DDBLTFX DDBltFX;
   	ZeroMemory(&DDBltFX, sizeof(DDBltFX));
   	DDBltFX.dwSize = sizeof(DDBltFX);
   //	DDBltFX.
   	while (m_lpDDSBack->GetBltStatus(DDGBS_CANBLT) != DD_OK);
   	ddrval = m_lpDDSBack->Blt( &destRect, m_lpDDSWorldBmp, &rcRect, DDBLT_DDFX, &DDBltFX);
   	if (ddrval != DD_OK) {
   		fprintf(stderr, "ddrval = %d\n",ddrval);
   		fprintf(stderr, "destRect = %d %d %d %d\n",destRect.left, destRect.top, destRect.right, destRect.bottom);
   		fprintf(stderr, "rcRect = %d %d %d %d\n", rcRect.left,rcRect.top, rcRect.right, rcRect.bottom);
   		fprintf(stderr, "UpdateRect = %d %d %d %d\n", UpdateRect.top,UpdateRect.left, UpdateRect.right, UpdateRect.bottom);
   		fflush(stderr);
   		AfxMessageBox("Back Blt1 Failed");
   		DDrawDiagnostic(ddrval);
   		ASSERT(FALSE);
   	}
   /*Put in the graphic objects*/
   //#define DRAW_UPDATE_RECT
   #ifdef DRAW_UPDATE_RECT
   	CDC* pDC = DDrawGetSurfaceDC(m_lpDDSBack);
   
   	CPoint P(0,0);
   	pDC->MoveTo(P);
   	P.x = ProbGridBmpUpdateRect.Width() * m_ZoomLevel - 1, P.y = 0;
   	pDC->LineTo(P);
   	P.x = ProbGridBmpUpdateRect.Width() * m_ZoomLevel - 1, P.y = ProbGridBmpUpdateRect.Height() * m_ZoomLevel - 1;
   	pDC->LineTo(P);
   	P.x = 0, P.y = ProbGridBmpUpdateRect.Height() * m_ZoomLevel - 1;
   	pDC->LineTo(P);
   	P.x = 0, P.y = 0;
   	pDC->LineTo(P);
   
   	DDrawReleaseSurfaceDC(pDC, m_lpDDSBack);
   //	pDC->DeleteTempMap();	//Causes Dragging Rectangle to disappear
   #endif
   
   /*Sketch the robots' errors*/
   	for (int i = 0; m_RobotList[i] != NULL; i++) {
   		CRobot* Robot = m_RobotList[i];
   		if (Robot != NULL) {
   			robPOINT RobotPosition = Robot->GetDrawPosition();
   			double RobotHeading = Robot->GetDrawHeading();
   			CDC* pDC = DDrawGetSurfaceDC(m_lpDDSBack);
   			robPOINT Corners[4];
   			Robot->GetCorners(RobotPosition, RobotHeading, Corners);
   			SketchRobotError(Corners, Robot->GetPositionError(), ProbGridBmpUpdateRect.TopLeft(), pDC);
   			DDrawReleaseSurfaceDC(pDC, m_lpDDSBack);
   		}
   	}
   
   
   	m_RWGraphicObjectList->DrawObjects(ProbGridBmpUpdateRect, m_lpDDSBack, this);
   
   /*Draw robots*/
   	for (int i = 0; m_RobotList[i] != NULL; i++) {
   		CRobot* Robot = m_RobotList[i];
   		if (Robot != NULL) {
   			robPOINT RobotPosition = Robot->GetDrawPosition();
   			double RobotHeading = Robot->GetDrawHeading();
   		/*Put in Robot*/
   			int Frame = RobotHeadingToBitmapFrame(RobotHeading, cTotalFrames);
   			rcRect.left   = Frame%RobotBmpFramePerLine*RobotBmpFrameWidth;
   			rcRect.top    = Frame/RobotBmpFramePerLine*RobotBmpFrameHeight;
   			rcRect.right  = rcRect.left + RobotBmpFrameWidth;
   			rcRect.bottom = rcRect.top + RobotBmpFrameHeight;
   			float temp;
   			temp = RobotToDDSurfaceX(RobotPosition.x, ProbGridBmpUpdateRect.left);
   			temp -= RobotBmpFrameWidth/2.0f;
   			destRect.left = Round(temp);
   		//	destRect.left = Round((RobotPosition.x / cWorldGridPointSize + cWorldGridXSize / 2 - ProbGridBmpUpdateRect.left)*m_ZoomLevel - RobotBmpFrameWidth/2.0 );
   			destRect.right = destRect.left + RobotBmpFrameWidth;
   			temp = RobotToDDSurfaceY(RobotPosition.y, ProbGridBmpUpdateRect.top);
   			temp -= RobotBmpFrameHeight/2.0f;
   			destRect.top = Round(temp);
   		//	destRect.top = Round((-RobotPosition.y / cWorldGridPointSize + cWorldGridYSize / 2 - ProbGridBmpUpdateRect.top)*m_ZoomLevel - RobotBmpFrameHeight/2.0 );
   			destRect.bottom = destRect.top + RobotBmpFrameHeight;
   			if (destRect.left < 0) {
   				rcRect.left -= destRect.left;
   				if (rcRect.left < 0) rcRect.left = 0;
   				destRect.left = 0;
   			}
   			if (destRect.right < 0) {
   				rcRect.right -= destRect.right;
   				if (rcRect.right < 0) rcRect.right = 0;
   				destRect.right = 0;
   			}
   			if (destRect.top < 0) {
   				rcRect.top -= destRect.top;
   				if (rcRect.top < 0) rcRect.top = 0;
   				destRect.top = 0;
   			}
   			if (destRect.bottom < 0) {
   				rcRect.bottom -= destRect.bottom;
   				if (rcRect.bottom < 0) rcRect.bottom = 0;
   				destRect.bottom = 0;
   			}
   			if ( (destRect.top >= m_DisplayResolution.y) || (destRect.bottom >= m_DisplayResolution.y) ) {
   				destRect.top = 0;
   				destRect.bottom = 0;
   			}
   			if ( (destRect.left >= m_DisplayResolution.x) || (destRect.right >= m_DisplayResolution.x) ) {
   				destRect.left = 0;
   				destRect.right = 0;
   			}
   			if (rcRect.left > rcRect.right) {
   				rcRect.left = rcRect.right;
   			}
   			if (rcRect.top > rcRect.bottom) {
   				rcRect.top = rcRect.bottom;
   			}
   
   		//	COLORREF TransparencyColor = cTransparencyColor;
   			DDCOLORKEY RobotColorKey;
   			RobotColorKey.dwColorSpaceHighValue = m_RobotTransparencyColor;
   			RobotColorKey.dwColorSpaceLowValue = m_RobotTransparencyColor;
   			LPDIRECTDRAWSURFACE3 RobotBmp = m_lpDDSRobotBmp[i];
   			if (Robot != GetActiveRobot()) {
   				RobotBmp = m_lpDDSInactiveRobotBmp;
   			}
   			ASSERT(RobotBmp != NULL);
   			if (RobotBmp != NULL) {
   				RobotBmp->SetColorKey(DDCKEY_SRCBLT, &RobotColorKey);
   			//	DDSetColorKey(m_lpDDSRobotBmp, m_RobotTransparencyColor);
   				if ( (destRect.Height() != 0) && (destRect.Width() != 0) && (rcRect.Height() != 0) && (rcRect.Width() != 0) ) {
   					while (m_lpDDSBack->GetBltStatus(DDGBS_CANBLT) != DD_OK);
   					ddrval = m_lpDDSBack->BltFast(destRect.left, destRect.top, RobotBmp, rcRect, DDBLTFAST_SRCCOLORKEY);
   					if (ddrval != DD_OK) {
   						AfxMessageBox("DrawRobot Failed");
   						DDrawDiagnostic(ddrval);
   						fprintf(stderr, "***DrawRobot Error***\n");
   						fprintf(stderr, "destRect = %d %d %d %d\n",destRect.left, destRect.top, destRect.right, destRect.bottom);
   						fprintf(stderr, "rcRect = %d %d %d %d\n", rcRect.left,rcRect.top, rcRect.right, rcRect.bottom);
   						fflush(stderr);
   						ASSERT(FALSE);
   					}
   				}
   			}
   		}
   	}
   
   /*Blt from Back surface to Primary surface*/
   	CPoint pt;
   	pt.x = 0;
   	pt.y = 0;
   	pWnd->ClientToScreen(&pt);
   	int ScrollPos = pWnd->GetScrollPos(SB_HORZ);
   	destRect.left = long(pt.x + ProbGridBmpUpdateRect.left*m_ZoomLevel - ScrollPos * m_ZoomLevel);
   	destRect.right = long(destRect.left + ProbGridBmpUpdateRect.Width()*m_ZoomLevel);
   	ScrollPos = pWnd->GetScrollPos(SB_VERT);
   	destRect.top = long(pt.y + ProbGridBmpUpdateRect.top*m_ZoomLevel - ScrollPos * m_ZoomLevel);
   	destRect.bottom = long(destRect.top + ProbGridBmpUpdateRect.Height()*m_ZoomLevel);
   	rcRect.left = 0;
   	rcRect.top = 0;
   	rcRect.right = long(ProbGridBmpUpdateRect.Width() * m_ZoomLevel);
   	rcRect.bottom = long(ProbGridBmpUpdateRect.Height() * m_ZoomLevel);
   	while (m_lpDDSPrimary->GetBltStatus(DDGBS_CANBLT) != DD_OK) {
   		fflush(stderr);
   	}
   	ddrval = m_lpDDSPrimary->Blt(&destRect, m_lpDDSBack, &rcRect, 0, NULL);
   	if (ddrval != DD_OK) {
   #ifdef _DEBUG
   		AfxMessageBox("Primary Blt Failed");
   #endif
   		DDrawDiagnostic(ddrval);
   		ASSERT(FALSE);
   	}
   }
   
   void CProbabilityGrid::CreateLogicalPalette(HWND hAppWnd)
   /*******************************************************************************
   //Created: 05/14/97 S.R.
   //Last Revision: 05/15/97 S.R.
   //Parameters:
   //	hAppWnd:	Window handle used for the application.
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		This function creates a palette for the application when the display is
   //	in 8 bits per pixel mode.
   //Method:
   //
   *******************************************************************************/
   {
   #define PALETTECOLOR(nMember) (unsigned char(Round((255.0 - MinPal)/(cSizeProbabilityPalette)*(nMember) + MinPal)))
   /*Create Windows Palette*/
   	unsigned int i;
   
   	LPLOGPALETTE pLogicalPalette = (LPLOGPALETTE) new char[2 * sizeof(WORD) +
   	cMaxPaletteEntries * sizeof(PALETTEENTRY)];
   
   	pLogicalPalette->palVersion = 0x300;
   	pLogicalPalette->palNumEntries = cMaxPaletteEntries;
   
   
   	unsigned char MinPal = unsigned char(cDarkShiftPercent/100.0*255);
   	for (i = 0; i < cSizeProbabilityPalette; i++) {
   //		if (i == (cSizeProbabilityPalette)/2) {
   //			i = (cSizeProbabilityPalette)/2;
   //		}
   //		pLogicalPalette->palPalEntry[i].peRed = Round((255.0 - MinPal)/(cSizeProbabilityPalette)*i + MinPal);
   		pLogicalPalette->palPalEntry[i].peRed = PALETTECOLOR(i);
   		pLogicalPalette->palPalEntry[i].peGreen = pLogicalPalette->palPalEntry[i].peRed;
   		pLogicalPalette->palPalEntry[i].peBlue = pLogicalPalette->palPalEntry[i].peRed;
   		pLogicalPalette->palPalEntry[i].peFlags = PC_NOCOLLAPSE;
   	}
   /*Clear remaining palette entries*/
   	for (i = cSizeProbabilityPalette; i < cMaxPaletteEntries; i++) {
   		pLogicalPalette->palPalEntry[i].peRed = 0;
   		pLogicalPalette->palPalEntry[i].peGreen = pLogicalPalette->palPalEntry[i].peRed;
   		pLogicalPalette->palPalEntry[i].peBlue = pLogicalPalette->palPalEntry[i].peRed;
   		pLogicalPalette->palPalEntry[i].peFlags = PC_NOCOLLAPSE;
   	}
   
   	pLogicalPalette->palPalEntry[cSizeProbabilityPalette - 1].peRed = 0xFF;
   	pLogicalPalette->palPalEntry[cSizeProbabilityPalette - 1].peGreen = 0xFF;
   	pLogicalPalette->palPalEntry[cSizeProbabilityPalette - 1].peBlue = 0xFF;
   	pLogicalPalette->palPalEntry[cSizeProbabilityPalette - 1].peFlags = PC_NOCOLLAPSE;
   
   	if ( m_LogicalPalette.CreatePalette(pLogicalPalette) == 0) {
   		AfxMessageBox("Palette Creation Failed!!");
   		assert(FALSE);
   	}
   	delete pLogicalPalette;
   	HDC hDC = GetDC(hAppWnd);
   	HPALETTE hpal = HPALETTE(m_LogicalPalette);
   	HPALETTE temp1 = SelectPalette(hDC, hpal, FALSE);
   	DeleteObject(temp1);
   
   	RealizePalette(hDC);
   
   	unsigned int temp = GetSystemPaletteEntries(hDC, 0, 256, NULL);
   	ASSERT(temp == 256);
   	PALETTEENTRY* SysPalEntries = new PALETTEENTRY[temp];
   	GetSystemPaletteEntries(hDC, 0, temp, SysPalEntries);
   
   
   /*Create DirectDraw Palette*/
   	PALETTEENTRY pPaletteEntry[256];
   // First set up the Windows static entries.
   	for (unsigned int index = 0; index < 10 ; index++) {
   /*	// The first 10 static entries:
   		pPaletteEntry[index].peFlags = PC_EXPLICIT;
   		pPaletteEntry[index].peRed = index;
   		pPaletteEntry[index].peGreen = 0;
   		pPaletteEntry[index].peBlue = 0;
   	// The last 10 static entries:
   		pPaletteEntry[index+246].peFlags = PC_EXPLICIT;
   		pPaletteEntry[index+246].peRed = index+246;
   		pPaletteEntry[index+246].peGreen = 0;
   		pPaletteEntry[index+246].peBlue = 0;
   */
   	// The first 10 static entries:
   		pPaletteEntry[index].peFlags = SysPalEntries[index].peFlags;
   		pPaletteEntry[index].peRed = SysPalEntries[index].peRed;
   		pPaletteEntry[index].peGreen = SysPalEntries[index].peGreen;
   		pPaletteEntry[index].peBlue = SysPalEntries[index].peBlue;
   	// The last 10 static entries:
   		pPaletteEntry[index+246].peFlags = SysPalEntries[index+246].peFlags;
   		pPaletteEntry[index+246].peRed = SysPalEntries[index+246].peRed;
   		pPaletteEntry[index+246].peGreen = SysPalEntries[index+246].peGreen;
   		pPaletteEntry[index+246].peBlue = SysPalEntries[index+246].peBlue;
   	}
   // Now set up private entries.
   	for (unsigned int index = 10; index < cSizeProbabilityPalette + 10; index ++) {
   		pPaletteEntry[index].peFlags = PC_NOCOLLAPSE;
   		pPaletteEntry[index].peRed = PALETTECOLOR(index - 10);
   		pPaletteEntry[index].peGreen = pPaletteEntry[index].peRed;
   		pPaletteEntry[index].peBlue = pPaletteEntry[index].peRed;
   	}
   	pPaletteEntry[cSizeProbabilityPalette + 9].peFlags = PC_NOCOLLAPSE;
   	pPaletteEntry[cSizeProbabilityPalette + 9].peRed = 0xFF;
   	pPaletteEntry[cSizeProbabilityPalette + 9].peGreen = 0xFF;
   	pPaletteEntry[cSizeProbabilityPalette + 9].peBlue = 0xFF;
   	for (unsigned int index = cSizeProbabilityPalette + 10; index < 246; index++) {
   		pPaletteEntry[index].peFlags = PC_NOCOLLAPSE;
   		pPaletteEntry[index].peRed = 0;
   		pPaletteEntry[index].peGreen = pPaletteEntry[index].peRed;
   		pPaletteEntry[index].peBlue = pPaletteEntry[index].peRed;
   	}
   // All 256 entries are filled. Create the palette.
   	HRESULT ddrval = m_lpDD->CreatePalette(DDPCAPS_8BIT, pPaletteEntry, &m_lpDDPal,NULL);
   	if (ddrval != DD_OK) {
   		AfxMessageBox("Palette Creation Failed");
   		DDrawDiagnostic(ddrval);
   		assert(FALSE);
   	}
   	delete SysPalEntries;
   }
   
   void CProbabilityGrid::ProbGridBmpToClientCoordinates(CRect* Rect)
   /*******************************************************************************
   //Created: 05/18/97 S.R.
   //Last Revision: 05/18/97 S.R.
   //Parameters:
   //	Rect:	The rectangle whose coordinates are to be transformed from world bitmap
   //				to window client coordinates.
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		This function takes a rectangle in probability grid bitmap coordinates
   //	and transforms them into client coordinates.
   //Method:
   //
   *******************************************************************************/
   {
   	CPoint P = Rect->TopLeft();
   	int ScrollPos = m_ViewWindow->GetScrollPos(SB_HORZ);
   	P.x = long((P.x - ScrollPos)*m_ZoomLevel);
   	ScrollPos = m_ViewWindow->GetScrollPos(SB_VERT);
   	P.y = long((P.y - ScrollPos)*m_ZoomLevel);
   	Rect->TopLeft() = P;
   	P = Rect->BottomRight();
   	ScrollPos = m_ViewWindow->GetScrollPos(SB_HORZ);
   	P.x = long((P.x - ScrollPos)*m_ZoomLevel);
   	ScrollPos = m_ViewWindow->GetScrollPos(SB_VERT);
   	P.y = long((P.y - ScrollPos)*m_ZoomLevel);
   	Rect->BottomRight() = P;
   }
   
   void CProbabilityGrid::ClientToProbGridBmpCoords(CRect* ClientRect)
   /*******************************************************************************
   //Created: 05/18/97 S.R.
   //Last Revision: 06/11/97 S.R.
   //Parameters:
   //	ClientRect:		A rectangle in client coordinates.
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		This function transforms a rectangle in client coordinates and transforms
   //	it into probability grid bitmap coordinates.
   //Method:
   //
   *******************************************************************************/
   {
   	CPoint ClientOffset;
   	int ScrollPos = m_ViewWindow->GetScrollPos(SB_HORZ);
   	ClientOffset.x = long(ScrollPos * m_ZoomLevel);
   	ScrollPos = m_ViewWindow->GetScrollPos(SB_VERT);
   	ClientOffset.y = long(ScrollPos * m_ZoomLevel);
   
   	ClientRect->OffsetRect(ClientOffset);
   	CPoint P = ClientRect->TopLeft();
   	P.x = long(P.x / m_ZoomLevel);
   	P.y = long(P.y / m_ZoomLevel);
   	ClientRect->TopLeft() = P;
   	P = ClientRect->BottomRight();
   	P.x = long(P.x / m_ZoomLevel);
   	P.y = long(P.y / m_ZoomLevel);
   	ClientRect->BottomRight() = P;
   }
   
   HRESULT CProbabilityGrid::DDrawReloadBitmap(IDirectDrawSurface3 *pdds, UINT ResourceID)
   /*This function reloads a direct draw surface's bitmap.  The bitmap will be of the same
   	dimensions as the original*/
   {
   	CBitmap tempBmap;
   	if (!tempBmap.LoadBitmap(ResourceID)) {
   //#ifdef _DEBUG
   		AfxMessageBox("DDrawReloadBitmap Error!! Did not load bitmap");
   		fprintf(stderr, "DDrawReloadBitmap Error!! Did not load bitmap\n");
   //#endif
   		assert(FALSE);
   	}
   
   	HBITMAP hbm = HBITMAP(tempBmap);
   	HRESULT ddrval = ::DDCopyBitmap2(pdds, hbm, 0, 0, 0, 0);
   	tempBmap.DeleteObject();
   	return ddrval;
   }
   
   
   IDirectDrawSurface* DDrawLoadBitmap(IDirectDraw *pdd, CBitmap* tempBmap)
   /*******************************************************************************
   //Created: 06/07/97 S.R.
   //Last Revision: 06/10/97 S.R.
   //Parameters:
   //	pdd:
   //	tempBmap:	bitmap to load into the new surface.
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		This function loads a CBitmap into a direct draw surface.
   //Method:
   //
   *******************************************************************************/
   {
   	HBITMAP             hbm;
   	BITMAP              bm;
   	DDSURFACEDESC       ddsd;
   	IDirectDrawSurface *pdds = NULL;
   
   	hbm = HBITMAP(*tempBmap);
   	if (hbm == NULL)
   	return NULL;
   
   	GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap
   
   //Create a DirectDrawSurface for the bitmap
   	ZeroMemory(&ddsd, sizeof(ddsd));
   	ddsd.dwSize = sizeof(ddsd);
   	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
   	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
   	ddsd.dwWidth = bm.bmWidth;
   	ddsd.dwHeight = bm.bmHeight;
   
   	HRESULT ddrval = pdd->CreateSurface(&ddsd, &pdds, NULL);
   	if (ddrval != DD_OK) {
   #ifdef _DEBUG
   	AfxMessageBox("Did not create Bitmap surface");
   		DDrawDiagnostic(ddrval);
   #endif
   	}
   	ddrval = DDrawCopyBitmap(pdds, hbm, 0, 0, 0, 0);
   	if (ddrval != DD_OK) {
   #ifdef _DEBUG
   		AfxMessageBox("Did not do the bitmap copy");
   		DDrawDiagnostic(ddrval);
   #endif
   	}
   	return pdds;
   }
   
   IDirectDrawSurface* DDrawLoadBitmap(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy)
   /*******************************************************************************
   //Created: 07/14/97 S.R.
   //Last Revision: 07/14/97 S.R.
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
   {
       HBITMAP             hbm;
       BITMAP              bm;
       DDSURFACEDESC       ddsd;
       IDirectDrawSurface *pdds;
   
       //
       //  try to load the bitmap as a resource, if that fails, try it as a file
       //
       hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx, dy, LR_CREATEDIBSECTION);
   
       if (hbm == NULL)
           hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
   
       if (hbm == NULL)
           return NULL;
   
       //
       // get size of the bitmap
       //
       GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap
   
       //
       // create a DirectDrawSurface for this bitmap
       //
       ZeroMemory(&ddsd, sizeof(ddsd));
       ddsd.dwSize = sizeof(ddsd);
       ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
       ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
       ddsd.dwWidth = bm.bmWidth;
       ddsd.dwHeight = bm.bmHeight;
   
       if (pdd->CreateSurface(&ddsd, &pdds, NULL) != DD_OK)
           return NULL;
   
       DDrawCopyBitmap(pdds, hbm, 0, 0, 0, 0);
   
       DeleteObject(hbm);
   
       return pdds;
   }
   
   HRESULT DDrawCopyBitmap(IDirectDrawSurface *pdds, HBITMAP hbm, int x, int y, int dx, int dy)
   /*******************************************************************************
   //Created: 07/14/97 S.R.
   //Last Revision: 07/14/97 S.R.
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
   {
   	HDC                 hdcImage;
   	HDC                 hdc;
   	BITMAP              bm;
   	DDSURFACEDESC       ddsd;
   	HRESULT             hr;
   
   	if (hbm == NULL || pdds == NULL)
   	  return E_FAIL;
   
   // make sure this surface is restored.
   	pdds->Restore();
   
   //  select bitmap into a memoryDC so we can use it.
   	hdcImage = CreateCompatibleDC(NULL);
   	if (!hdcImage) {
   #ifdef _DEBUG
   		OutputDebugString("createcompatible dc failed\n");
   		AfxMessageBox("CreateCompatibleDC  failed");
   #endif
   	}
   	SelectObject(hdcImage, hbm);
   
   // get size of the bitmap
   	GetObject(hbm, sizeof(bm), &bm);    // get size of bitmap
   	dx = dx == 0 ? bm.bmWidth  : dx;    // use the passed size, unless zero
   	dy = dy == 0 ? bm.bmHeight : dy;
   
   // get size of surface.
   	ddsd.dwSize = sizeof(ddsd);
   	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
   	pdds->GetSurfaceDesc(&ddsd);
   
   	if ((hr = pdds->GetDC(&hdc)) == DD_OK)
   	{
   	  StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, dx, dy, SRCCOPY);
   	  pdds->ReleaseDC(hdc);
   	}
   
   	DeleteDC(hdcImage);
   
   	return hr;
   }
   
   HRESULT DDrawReLoadBitmap(IDirectDrawSurface *pdds, CBitmap* tempBmap)
   /*******************************************************************************
   //Created: 06/07/97 S.R.
   //Last Revision: 06/10/97 S.R.
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
   {
   	HBITMAP hbm;
   	HRESULT hr;
   
   	hbm = HBITMAP(*tempBmap);
   	if (hbm == NULL)
   	return E_FAIL;
   
   	hr = DDrawCopyBitmap(pdds, hbm, 0, 0, 0, 0);
   	if (hr != DD_OK) {
   		OutputDebugString("ddcopybitmap failed\n");
   	}
   	return hr;
   }
   
   IDirectDrawSurface3* DDrawLoadBitmap(IDirectDraw2 *pdd, CBitmap* tempBmap)
   /*******************************************************************************
   //Created: 06/07/97 S.R.
   //Last Revision: 06/10/97 S.R.
   //Parameters:
   //	pdd:
   //	tempBmap:	bitmap to load into the new surface.
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		This function loads a CBitmap into a direct draw surface.
   //Method:
   //
   *******************************************************************************/
   {
   	HBITMAP             hbm;
   	BITMAP              bm;
   	DDSURFACEDESC       ddsd;
   	IDirectDrawSurface *pdds = NULL;
   
   	hbm = HBITMAP(*tempBmap);
   	if (hbm == NULL)
   	return NULL;
   
   	GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap
   
   //Create a DirectDrawSurface for the bitmap
   	ZeroMemory(&ddsd, sizeof(ddsd));
   	ddsd.dwSize = sizeof(ddsd);
   	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
   	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
   	ddsd.dwWidth = bm.bmWidth;
   	ddsd.dwHeight = bm.bmHeight;
   
   	HRESULT ddrval = pdd->CreateSurface(&ddsd, &pdds, NULL);
   	if (ddrval != DD_OK) {
   #ifdef _DEBUG
   		AfxMessageBox("Did not create Bitmap surface");
   		DDrawDiagnostic(ddrval);
   #endif
   	}
   	ddrval = DDrawCopyBitmap(pdds, hbm, 0, 0, 0, 0);
   	if (ddrval != DD_OK) {
   #ifdef _DEBUG
   		AfxMessageBox("Did not do the bitmap copy");
   		DDrawDiagnostic(ddrval);
   #endif
   	}
   
   	IDirectDrawSurface3 *pdds2 = NULL;
   	ddrval = pdds->QueryInterface(IID_IDirectDrawSurface3,(LPVOID *)&pdds2);
   	if(ddrval != DD_OK) {
   #ifdef _DEBUG
   		DDrawDiagnostic(ddrval);
   		AfxMessageBox("DD QueryInterface failed");
   #endif
   		return NULL;
   	}
   	pdds->Release();
   	return pdds2;
   }
   
   IDirectDrawSurface3* DDrawLoadBitmap(IDirectDraw2 *pdd, LPCSTR szBitmap, int dx, int dy)
   /*******************************************************************************
   //Created: 07/14/97 S.R.
   //Last Revision: 07/14/97 S.R.
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
   {
       HBITMAP             hbm;
       BITMAP              bm;
       DDSURFACEDESC       ddsd;
       IDirectDrawSurface *pdds;
   
       //
       //  try to load the bitmap as a resource, if that fails, try it as a file
       //
       hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx, dy, LR_CREATEDIBSECTION);
   
       if (hbm == NULL)
           hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
   
       if (hbm == NULL)
           return NULL;
   
       //
       // get size of the bitmap
       //
       GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap
   
       //
       // create a DirectDrawSurface for this bitmap
       //
       ZeroMemory(&ddsd, sizeof(ddsd));
       ddsd.dwSize = sizeof(ddsd);
       ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
       ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
       ddsd.dwWidth = bm.bmWidth;
       ddsd.dwHeight = bm.bmHeight;
   
       if (pdd->CreateSurface(&ddsd, &pdds, NULL) != DD_OK)
           return NULL;
   
       DDrawCopyBitmap(pdds, hbm, 0, 0, 0, 0);
   
       DeleteObject(hbm);
   
   	IDirectDrawSurface3 *pdds2 = NULL;
   	HRESULT ddrval = pdds->QueryInterface(IID_IDirectDrawSurface3,(LPVOID *)&pdds2);
   	if(ddrval != DD_OK) {
   		DDrawDiagnostic(ddrval);
   		AfxMessageBox("DD QueryInterface failed");
   		return NULL;
   	}
   	pdds->Release();
   	return pdds2;
   }
   
   HRESULT DDrawCopyBitmap(IDirectDrawSurface3 *pdds, HBITMAP hbm, int x, int y, int dx, int dy)
   /*******************************************************************************
   //Created: 07/14/97 S.R.
   //Last Revision: 07/14/97 S.R.
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
   {
   	HDC                 hdcImage;
   	HDC                 hdc;
   	BITMAP              bm;
   	DDSURFACEDESC       ddsd;
   	HRESULT             hr;
   
   	if (hbm == NULL || pdds == NULL)
   	  return E_FAIL;
   
   // make sure this surface is restored.
   	pdds->Restore();
   
   //  select bitmap into a memoryDC so we can use it.
   	hdcImage = CreateCompatibleDC(NULL);
   	if (!hdcImage) {
   		OutputDebugString("createcompatible dc failed\n");
   		AfxMessageBox("CreateCompatibleDC  failed");
   	}
   	SelectObject(hdcImage, hbm);
   
   // get size of the bitmap
   	GetObject(hbm, sizeof(bm), &bm);    // get size of bitmap
   	dx = dx == 0 ? bm.bmWidth  : dx;    // use the passed size, unless zero
   	dy = dy == 0 ? bm.bmHeight : dy;
   
   // get size of surface.
   	ddsd.dwSize = sizeof(ddsd);
   	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
   	pdds->GetSurfaceDesc(&ddsd);
   
   	if ((hr = pdds->GetDC(&hdc)) == DD_OK)
   	{
   	  StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, dx, dy, SRCCOPY);
   	  pdds->ReleaseDC(hdc);
   	}
   
   	DeleteDC(hdcImage);
   
   	return hr;
   }
   
   HRESULT DDrawReLoadBitmap(IDirectDrawSurface3 *pdds, CBitmap* tempBmap)
   /*******************************************************************************
   //Created: 06/07/97 S.R.
   //Last Revision: 06/10/97 S.R.
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
   {
   	HBITMAP hbm;
   	HRESULT hr;
   
   	hbm = HBITMAP(*tempBmap);
   	if (hbm == NULL)
   	return E_FAIL;
   
   	hr = DDrawCopyBitmap(pdds, hbm, 0, 0, 0, 0);
   	if (hr != DD_OK) {
   		OutputDebugString("ddcopybitmap failed\n");
   	}
   	return hr;
   }
   void DDrawDiagnostic(HRESULT ddrval, bool ShowMessage)
   /*******************************************************************************
   //Created: 06/10/97 S.R.
   //Last Revision: 06/10/97 S.R.
   //Parameters:
   //	ddrval:		Direct Draw error value to process.
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		This function takes in a direct draw error value, and prints out an
   //	appropriate message.
   //Method:
   //
   *******************************************************************************/
   {
   	switch (ddrval) {
   	case DD_OK:
   		fprintf(stderr, "DDrawDiagnostic DD_OK\n");
   		break;
   	case DDERR_ALREADYINITIALIZED:
   	//The object has already been initialized. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_ALREADYINITIALIZED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_ALREADYINITIALIZED\n",ddrval);
   		break;
   	case DDERR_BLTFASTCANTCLIP:
   	//A DirectDrawClipper object is attached to a source surface that has passed into a call to the IDirectDrawSurface2::BltFast method. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_BLTFASTCANTCLIP");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_BLTFASTCANTCLIP\n",ddrval);
   		break;
   	case DDERR_CANNOTATTACHSURFACE:
   	//A surface cannot be attached to another requested surface. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_CANNOTATTACHSURFACE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_CANNOTATTACHSURFACE\n",ddrval);
   		break;
   	case DDERR_CANNOTDETACHSURFACE:
   	//A surface cannot be detached from another requested surface. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_CANNOTDETACHSURFACE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_CANNOTDETACHSURFACE\n",ddrval);
   		break;
   	case DDERR_CANTCREATEDC:
   	//Windows cannot create any more device contexts (DCs). 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_CANTCREATEDC");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_CANTCREATEDC\n",ddrval);
   		break;
   	case DDERR_CANTDUPLICATE:
   		//Primary and 3D surfaces, or surfaces that are implicitly created, cannot be duplicated. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_CANTDUPLICATE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_CANTDUPLICATE\n",ddrval);
   		break;
   	case DDERR_CANTLOCKSURFACE:
   		//Access to this surface is refused because an attempt was made to lock the primary surface without DCI support. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_CANTLOCKSURFACE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_CANTLOCKSURFACE\n",ddrval);
   		break;
   	case DDERR_CANTPAGELOCK:
   		//An attempt to page lock a surface failed. Page lock will not work on a display-memory surface or an emulated primary surface. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_CANTPAGELOCK");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_CANTPAGELOCK\n",ddrval);
   		break;
   	case DDERR_CANTPAGEUNLOCK:
   		//An attempt to page unlock a surface failed. Page unlock will not work on a display-memory surface or an emulated primary surface. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_CANTPAGEUNLOCK");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_CANTPAGEUNLOCK\n",ddrval);
   		break;
   	case DDERR_CLIPPERISUSINGHWND:
   		//An attempt was made to set a clip list for a DirectDrawClipper object that is already monitoring a window handle. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_CLIPPERISUSINGHWND");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_CLIPPERISUSINGHWND\n",ddrval);
   		break;
   	case DDERR_COLORKEYNOTSET:
   		//No source color key is specified for this operation. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_COLORKEYNOTSET");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_COLORKEYNOTSET\n",ddrval);
   		break;
   	case DDERR_CURRENTLYNOTAVAIL:
   		//No support is currently available. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_CURRENTLYNOTAVAIL");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_CURRENTLYNOTAVAIL\n",ddrval);
   		break;
   	case DDERR_DCALREADYCREATED:
   		//A device context (DC) has already been returned for this surface. Only one DC can be retrieved for each surface. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_DCALREADYCREATED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_DCALREADYCREATED\n",ddrval);
   		break;
   	case DDERR_DIRECTDRAWALREADYCREATED:
   		//A DirectDraw object representing this driver has already been created for this process. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_DIRECTDRAWALREADYCREATED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_DIRECTDRAWALREADYCREATED\n",ddrval);
   		break;
   	case DDERR_EXCEPTION:
   		//An exception was encountered while performing the requested operation. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_EXCEPTION");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_EXCEPTION\n",ddrval);
   		break;
   	case DDERR_EXCLUSIVEMODEALREADYSET:
   		//An attempt was made to set the cooperative level when it was already set to exclusive. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_EXCLUSIVEMODEALREADYSET");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_EXCLUSIVEMODEALREADYSET\n",ddrval);
   		break;
   	case DDERR_GENERIC:
   		//There is an undefined error condition. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_GENERIC");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_GENERIC\n",ddrval);
   		break;
   	case DDERR_HEIGHTALIGN:
   		//The height of the provided rectangle is not a multiple of the required alignment. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_HEIGHTALIGN");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_HEIGHTALIGN\n",ddrval);
   		break;
   	case DDERR_HWNDALREADYSET:
   		//The DirectDraw cooperative level window handle has already been set. It cannot be reset while the process has surfaces or palettes created. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_HWNDALREADYSET");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_HWNDALREADYSET\n",ddrval);
   		break;
   	case DDERR_HWNDSUBCLASSED:
   		//DirectDraw is prevented from restoring state because the DirectDraw cooperative level window handle has been subclassed. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_HWNDSUBCLASSED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_HWNDSUBCLASSED\n",ddrval);
   		break;
   	case DDERR_IMPLICITLYCREATED:
   		//The surface cannot be restored because it is an implicitly created surface. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_IMPLICITLYCREATED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_IMPLICITLYCREATED\n",ddrval);
   		break;
   	case DDERR_INCOMPATIBLEPRIMARY:
   		//The primary surface creation request does not match with the existing primary surface. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_INCOMPATIBLEPRIMARY");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_INCOMPATIBLEPRIMARY\n",ddrval);
   		break;
   	case DDERR_INVALIDCAPS:
   		//One or more of the capability bits passed to the callback function are incorrect. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_INVALIDCAPS");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_INVALIDCAPS\n",ddrval);
   		break;
   	case DDERR_INVALIDCLIPLIST:
   		//DirectDraw does not support the provided clip list. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_INVALIDCLIPLIST");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_INVALIDCLIPLIST\n",ddrval);
   		break;
   	case DDERR_INVALIDDIRECTDRAWGUID:
   		//The globally unique identifier (GUID) passed to the DirectDrawCreate function is not a valid DirectDraw driver identifier. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_INVALIDDIRECTDRAWGUID");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_INVALIDDIRECTDRAWGUID\n",ddrval);
   		break;
   	case DDERR_INVALIDMODE:
   		//DirectDraw does not support the requested mode. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_INVALIDMODE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_INVALIDMODE\n",ddrval);
   		break;
   	case DDERR_INVALIDOBJECT:
   		//DirectDraw received a pointer that was an invalid DirectDraw object. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_INVALIDOBJECT");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_INVALIDOBJECT\n",ddrval);
   		break;
   	case DDERR_INVALIDPARAMS:
   		//One or more of the parameters passed to the method are incorrect. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_INVALIDPARAMS");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_INVALIDPARAMS\n",ddrval);
   		break;
   	case DDERR_INVALIDPIXELFORMAT:
   		//The pixel format was invalid as specified. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_INVALIDPIXELFORMAT");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_INVALIDPIXELFORMAT\n",ddrval);
   		break;
   	case DDERR_INVALIDPOSITION:
   		//The position of the overlay on the destination is no longer legal. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_INVALIDPOSITION");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_INVALIDPOSITION\n",ddrval);
   		break;
   	case DDERR_INVALIDRECT:
   		//The provided rectangle was invalid. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_INVALIDRECT");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_INVALIDRECT\n",ddrval);
   		break;
   	case DDERR_INVALIDSURFACETYPE:
   		//The requested operation could not be performed because the surface was of the wrong type. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_INVALIDSURFACETYPE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_INVALIDSURFACETYPE\n",ddrval);
   		break;
   	case DDERR_LOCKEDSURFACES:
   		//One or more surfaces are locked, causing the failure of the requested operation. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_LOCKEDSURFACES");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_LOCKEDSURFACES\n",ddrval);
   		break;
   	case DDERR_NO3D:
   	//No 3D hardware or emulation is present. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NO3D");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NO3D\n",ddrval);
   		break;
   	case DDERR_NOALPHAHW:
   	//No alpha acceleration hardware is present or available, causing the failure of the requested operation. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOALPHAHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOALPHAHW\n",ddrval);
   		break;
   	case DDERR_NOBLTHW:
   	//No blitter hardware is present. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOBLTHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOBLTHW\n",ddrval);
   		break;
   	case DDERR_NOCLIPLIST:
   	//No clip list is available. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOCLIPLIST");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOCLIPLIST\n",ddrval);
   		break;
   	case DDERR_NOCLIPPERATTACHED:
   	//No DirectDrawClipper object is attached to the surface object. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOCLIPPERATTACHED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOCLIPPERATTACHED\n",ddrval);
   		break;
   	case DDERR_NOCOLORCONVHW:
   	//The operation cannot be carried out because no color-conversion hardware is present or available. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOCOLORCONVHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOCOLORCONVHW\n",ddrval);
   		break;
   	case DDERR_NOCOLORKEY:
   	//The surface does not currently have a color key. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOCOLORKEY");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOCOLORKEY\n",ddrval);
   		break;
   	case DDERR_NOCOLORKEYHW:
   	//The operation cannot be carried out because there is no hardware support for the destination color key. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOCOLORKEYHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOCOLORKEYHW\n",ddrval);
   		break;
   	case DDERR_NOCOOPERATIVELEVELSET:
   	//A create function is called without the IDirectDraw2::SetCooperativeLevel method being called. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOCOOPERATIVELEVELSET");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOCOOPERATIVELEVELSET\n",ddrval);
   		break;
   	case DDERR_NODC:
   	//No DC has ever been created for this surface. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NODC");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NODC\n",ddrval);
   		break;
   	case DDERR_NODDROPSHW:
   	//No DirectDraw raster operation (ROP) hardware is available. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NODDROPSHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NODDROPSHW\n",ddrval);
   		break;
   	case DDERR_NODIRECTDRAWHW:
   	//Hardware-only DirectDraw object creation is not possible; the driver does not support any hardware. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NODIRECTDRAWHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NODIRECTDRAWHW\n",ddrval);
   		break;
   	case DDERR_NODIRECTDRAWSUPPORT:
   	//DirectDraw support is not possible with the current display driver. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NODIRECTDRAWSUPPORT");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NODIRECTDRAWSUPPORT\n",ddrval);
   		break;
   	case DDERR_NOEMULATION:
   	//Software emulation is not available. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOEMULATION");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOEMULATION\n",ddrval);
   		break;
   	case DDERR_NOEXCLUSIVEMODE:
   	//The operation requires the application to have exclusive mode, but the application does not have exclusive mode. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOEXCLUSIVEMODE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOEXCLUSIVEMODE\n",ddrval);
   		break;
   	case DDERR_NOFLIPHW:
   	//Flipping visible surfaces is not supported. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOFLIPHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOFLIPHW\n",ddrval);
   		break;
   	case DDERR_NOGDI:
   	//No GDI is present. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOGDI");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOGDI\n",ddrval);
   		break;
   	case DDERR_NOHWND:
   	//Clipper notification requires a window handle, or no window handle has been previously set as the cooperative level window handle. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOHWND");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOHWND\n",ddrval);
   		break;
   	case DDERR_NOMIPMAPHW:
   	//The operation cannot be carried out because no mipmap texture mapping hardware is present or available. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOMIPMAPHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOMIPMAPHW\n",ddrval);
   		break;
   	case DDERR_NOMIRRORHW:
   	//The operation cannot be carried out because no mirroring hardware is present or available. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOMIRRORHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOMIRRORHW\n",ddrval);
   		break;
   	case DDERR_NOOVERLAYDEST:
   	//The IDirectDrawSurface2::GetOverlayPosition method is called on an overlay that the IDirectDrawSurface2::UpdateOverlay method has not been called on to establish a destination. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOOVERLAYDEST");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOOVERLAYDEST\n",ddrval);
   		break;
   	case DDERR_NOOVERLAYHW:
   	//The operation cannot be carried out because no overlay hardware is present or available. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOOVERLAYHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOOVERLAYHW\n",ddrval);
   		break;
   	case DDERR_NOPALETTEATTACHED:
   	//No palette object is attached to this surface. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOPALETTEATTACHED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOPALETTEATTACHED\n",ddrval);
   		break;
   	case DDERR_NOPALETTEHW:
   	//There is no hardware support for 16- or 256-color palettes. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOPALETTEHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOPALETTEHW\n",ddrval);
   		break;
   	case DDERR_NORASTEROPHW:
   	//The operation cannot be carried out because no appropriate raster operation hardware is present or available. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NORASTEROPHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NORASTEROPHW\n",ddrval);
   		break;
   	case DDERR_NOROTATIONHW:
   	//The operation cannot be carried out because no rotation hardware is present or available. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOROTATIONHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOROTATIONHW\n",ddrval);
   		break;
   	case DDERR_NOSTRETCHHW:
   	//The operation cannot be carried out because there is no hardware support for stretching. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOSTRETCHHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOSTRETCHHW\n",ddrval);
   		break;
   	case DDERR_NOT4BITCOLOR:
   	//The DirectDrawSurface object is not using a 4-bit color palette and the requested operation requires a 4-bit color palette. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOT4BITCOLOR");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOT4BITCOLOR\n",ddrval);
   		break;
   	case DDERR_NOT4BITCOLORINDEX:
   	//The DirectDrawSurface object is not using a 4-bit color index palette and the requested operation requires a 4-bit color index palette. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOT4BITCOLORINDEX");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOT4BITCOLORINDEX\n",ddrval);
   		break;
   	case DDERR_NOT8BITCOLOR:
   	//The DirectDrawSurface object is not using an 8-bit color palette and the requested operation requires an 8-bit color palette. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOT8BITCOLOR");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOT8BITCOLOR\n",ddrval);
   		break;
   	case DDERR_NOTAOVERLAYSURFACE:
   	//An overlay component is called for a non-overlay surface. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOTAOVERLAYSURFACE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOTAOVERLAYSURFACE\n",ddrval);
   		break;
   	case DDERR_NOTEXTUREHW:
   	//The operation cannot be carried out because no texture-mapping hardware is present or available. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOTEXTUREHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOTEXTUREHW\n",ddrval);
   		break;
   	case DDERR_NOTFLIPPABLE:
   	//An attempt has been made to flip a surface that cannot be flipped. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOTFLIPPABLE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOTFLIPPABLE\n",ddrval);
   		break;
   	case DDERR_NOTFOUND:
   	//The requested item was not found. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOTFOUND");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOTFOUND\n",ddrval);
   		break;
   	case DDERR_NOTINITIALIZED:
   	//An attempt was made to call an interface method of a DirectDraw object created by CoCreateInstance before the object was initialized. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOTINITIALIZED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOTINITIALIZED\n",ddrval);
   		break;
   	case DDERR_NOTLOCKED:
   	//An attempt is made to unlock a surface that was not locked. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOTLOCKED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOTLOCKED\n",ddrval);
   		break;
   	case DDERR_NOTPAGELOCKED:
   	//An attempt is made to page unlock a surface with no outstanding page locks. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOTPAGELOCKED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOTPAGELOCKED\n",ddrval);
   		break;
   	case DDERR_NOTPALETTIZED:
   	//The surface being used is not a palette-based surface. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOTPALETTIZED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOTPALETTIZED\n",ddrval);
   		break;
   	case DDERR_NOVSYNCHW:
   	//The operation cannot be carried out because there is no hardware support for vertical blank synchronized operations. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOVSYNCHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOVSYNCHW\n",ddrval);
   		break;
   	case DDERR_NOZBUFFERHW:
   	//The operation to create a z-buffer in display memory or to perform a blit using a z-buffer cannot be carried out because there is no hardware support for z-buffers. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOZBUFFERHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOZBUFFERHW\n",ddrval);
   		break;
   	case DDERR_NOZOVERLAYHW:
   	//The overlay surfaces cannot be z-layered based on the z-order because the hardware does not support z-ordering of overlays. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_NOZOVERLAYHW");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_NOZOVERLAYHW\n",ddrval);
   		break;
   	case DDERR_OUTOFCAPS:
   	//The hardware needed for the requested operation has already been allocated. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_OUTOFCAPS");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_OUTOFCAPS\n",ddrval);
   		break;
   	case DDERR_OUTOFMEMORY:
   	//DirectDraw does not have enough memory to perform the operation. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_OUTOFMEMORY");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_OUTOFMEMORY\n",ddrval);
   		break;
   	case DDERR_OUTOFVIDEOMEMORY:
   	//DirectDraw does not have enough display memory to perform the operation. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_OUTOFVIDEOMEMORY");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_OUTOFVIDEOMEMORY\n",ddrval);
   		break;
   	case DDERR_OVERLAYCANTCLIP:
   	//The hardware does not support clipped overlays. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_OVERLAYCANTCLIP");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_OVERLAYCANTCLIP\n",ddrval);
   		break;
   	case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
   	//An attempt was made to have more than one color key active on an overlay. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_OVERLAYCOLORKEYONLYONEACTIVE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_OVERLAYCOLORKEYONLYONEACTIVE\n",ddrval);
   		break;
   	case DDERR_OVERLAYNOTVISIBLE:
   	//The IDirectDrawSurface2::GetOverlayPosition method is called on a hidden overlay. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_OVERLAYNOTVISIBLE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_OVERLAYNOTVISIBLE\n",ddrval);
   		break;
   	case DDERR_PALETTEBUSY:
   	//Access to this palette is refused because the palette is locked by another thread. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_PALETTEBUSY");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_PALETTEBUSY\n",ddrval);
   		break;
   	case DDERR_PRIMARYSURFACEALREADYEXISTS:
   	//This process has already created a primary surface. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_PRIMARYSURFACEALREADYEXISTS");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_PRIMARYSURFACEALREADYEXISTS\n",ddrval);
   		break;
   	case DDERR_REGIONTOOSMALL:
   	//The region passed to the IDirectDrawClipper::GetClipList method is too small. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_REGIONTOOSMALL");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_REGIONTOOSMALL\n",ddrval);
   		break;
   	case DDERR_SURFACEALREADYATTACHED:
   	//An attempt was made to attach a surface to another surface to which it is already attached. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_SURFACEALREADYATTACHED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_SURFACEALREADYATTACHED\n",ddrval);
   		break;
   	case DDERR_SURFACEALREADYDEPENDENT:
   	//An attempt was made to make a surface a dependency of another surface to which it is already dependent. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_SURFACEALREADYDEPENDENT");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_SURFACEALREADYDEPENDENT\n",ddrval);
   		break;
   	case DDERR_SURFACEBUSY:
   	//Access to the surface is refused because the surface is locked by another thread. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_SURFACEBUSY");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_SURFACEBUSY\n",ddrval);
   		break;
   	case DDERR_SURFACEISOBSCURED:
   	//Access to the surface is refused because the surface is obscured. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_SURFACEISOBSCURED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_SURFACEISOBSCURED\n",ddrval);
   		break;
   	case DDERR_SURFACELOST:
   	//Access to the surface is refused because the surface memory is gone. The DirectDrawSurface object representing this surface should have the IDirectDrawSurface2::Restore method called on it. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_SURFACELOST");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_SURFACELOST\n",ddrval);
   		break;
   	case DDERR_SURFACENOTATTACHED:
   	//The requested surface is not attached. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_SURFACENOTATTACHED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_SURFACENOTATTACHED\n",ddrval);
   		break;
   	case DDERR_TOOBIGHEIGHT:
   	//The height requested by DirectDraw is too large. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_TOOBIGHEIGHT");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_TOOBIGHEIGHT\n",ddrval);
   		break;
   	case DDERR_TOOBIGSIZE:
   	//The size requested by DirectDraw is too large. However, the individual height and width are OK. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_TOOBIGSIZE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_TOOBIGSIZE\n",ddrval);
   		break;
   	case DDERR_TOOBIGWIDTH:
   	//The width requested by DirectDraw is too large. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_TOOBIGWIDTH");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_TOOBIGWIDTH\n",ddrval);
   		break;
   	case DDERR_UNSUPPORTED:
   	//The operation is not supported. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_UNSUPPORTED");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_UNSUPPORTED\n",ddrval);
   		break;
   	case DDERR_UNSUPPORTEDFORMAT:
   	//The FourCC format requested is not supported by DirectDraw. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_UNSUPPORTEDFORMAT");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_UNSUPPORTEDFORMAT\n",ddrval);
   		break;
   	case DDERR_UNSUPPORTEDMASK:
   	//The bitmask in the pixel format requested is not supported by DirectDraw. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_UNSUPPORTEDMASK");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_UNSUPPORTEDMASK\n",ddrval);
   		break;
   	case DDERR_UNSUPPORTEDMODE:
   	//The display is currently in an unsupported mode. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_UNSUPPORTEDMODE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_UNSUPPORTEDMODE\n",ddrval);
   		break;
   	case DDERR_VERTICALBLANKINPROGRESS:
   	//A vertical blank is in progress. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_VERTICALBLANKINPROGRESS");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_VERTICALBLANKINPROGRESS\n",ddrval);
   		break;
   	case DDERR_WASSTILLDRAWING:
   	//The previous blit operation that is transferring information to or from this surface is incomplete. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_WASSTILLDRAWING");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_WASSTILLDRAWING\n",ddrval);
   		break;
   	case DDERR_WRONGMODE:
   	//This surface cannot be restored because it was created in a different mode. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_WRONGMODE");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_WRONGMODE\n",ddrval);
   		break;
   	case DDERR_XALIGN:
   	//The provided rectangle was not horizontally aligned on a required boundary. 
   		if (ShowMessage) {
   			AfxMessageBox("DDERR_XALIGN");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR_XALIGN\n",ddrval);
   		break;
   	default:
   	//Error not covered
   		if (ShowMessage) {
   			AfxMessageBox("DDERR not handled");
   		}
   		fprintf(stderr, "DDrawDiagnostic Error!!!!! DDERR not handled %d\n",ddrval);
   		break;
   	}
   }
   HRESULT CProbabilityGrid::DDrawSetSurfaceValues(IDirectDrawSurface3* lpDDSDestination, CRect DestRect, void* Values)
   /*******************************************************************************
   //Created: 07/14/97 S.R.
   //Last Revision: 07/14/97 S.R.
   //Parameters:
   //	none
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		This function transfers Values into the direct draw surface.  Values is
   //	a matrix of size [0,DestRect.Width()][0,DestRect.Height()]
   //Method:
   //
   *******************************************************************************/
   {
   	HRESULT ddrval;
   	DDSURFACEDESC SurfaceDesc;
   	memset(&SurfaceDesc, 0, sizeof(SurfaceDesc));
   	SurfaceDesc.dwSize = sizeof(SurfaceDesc);
   	do {
   		ddrval = lpDDSDestination->Lock(NULL, &SurfaceDesc, DDLOCK_SURFACEMEMORYPTR, NULL);
   		if (ddrval == DDERR_SURFACELOST) {
   			lpDDSDestination->Restore();
   		} else if (ddrval != DDERR_WASSTILLDRAWING) break;
   		if (ddrval != DD_OK) {
   			fprintf(stderr, "Waiting for lock\n");
   			fflush(stderr);
   		}
   	} while (ddrval != DD_OK);
   
   	if (ddrval != DD_OK) return ddrval;
   
   /*Get Surface Information*/
   	unsigned long SurfaceWidth = SurfaceDesc.dwWidth;
   	unsigned long SurfaceHeight = SurfaceDesc.dwHeight;
   	unsigned long SurfacePitch = SurfaceDesc.lPitch;
   	if (SurfacePitch == SurfaceWidth*2u) SurfacePitch = SurfaceWidth;
   /*Get Source Information*/
   	unsigned short SourceWidth = DestRect.Width() + 1;
   	unsigned short SourceHeight = DestRect.Height() + 1;
   /*Adjust DestRect if it is outside of Surface*/
   	if (DestRect.left >= (long)SurfaceWidth) return DDERR_GENERIC;
   	if (DestRect.right >= (long)SurfaceWidth) DestRect.right = SurfaceWidth-1;
   	if (DestRect.top >= (long)SurfaceHeight) return DDERR_GENERIC;
   	if (DestRect.bottom >= (long)SurfaceHeight) DestRect.bottom = SurfaceHeight-1;
   
   	unsigned short* pSurfaceBits = ((unsigned short*)SurfaceDesc.lpSurface) +
   		SurfaceWidth * DestRect.top + DestRect.left;
   	unsigned short* pSourceBits = (unsigned short*)Values;
   /*Transfer bytes*/
   	unsigned short DestWidth = DestRect.Width();
   	unsigned short DestHeight = DestRect.Height();
   	for ( unsigned short row = 0; row <= DestHeight; row++ ) {
   		memcpy(pSurfaceBits, pSourceBits, (DestWidth + 1)*(m_DisplayBitsPerPixel / 8));
   		pSurfaceBits += SurfacePitch;
   		pSourceBits += SourceWidth;
   	}
   
   	ddrval = lpDDSDestination->Unlock(NULL);
   	if (ddrval != DD_OK) {
   		AfxMessageBox("DDrawSetSurfaceValues Failed");
   		DDrawDiagnostic(ddrval);
   		assert(FALSE);
   	}
   	return DD_OK;
   }
   
   struct TEnumModesCallbackContext {
   /*	long XResolution;
   	long YResolution;
   	long RGBBitCount;
   	long SecondBestXResolution;
   	long SecondBestYResolution;
   	long SecondBestRGBBitCount;
   	bool UseSecondBestResult;
   */	bool Successful;
   	CStringList* DisplayModes;
   };
   
   HRESULT WINAPI EnumModesCallback( LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext)
   {
   	TEnumModesCallbackContext* Context = (TEnumModesCallbackContext*)lpContext;
   	if (lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount > 8) {
   		CString NewString(' ', 20);
   		char tempstr[10];
   		itoa(lpDDSurfaceDesc->dwWidth, tempstr, 10);
   		NewString = tempstr;
   		NewString += " X ";
   		itoa(lpDDSurfaceDesc->dwHeight, tempstr, 10);
   		NewString += tempstr;
   		NewString += " ";
   		itoa(lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount, tempstr, 10);
   		NewString += tempstr;
   		NewString += "-bit";
   		Context->DisplayModes->AddHead(NewString);
   		Context->Successful = true;
   /*		if ( (lpDDSurfaceDesc->dwWidth >= Context->XResolution) && (lpDDSurfaceDesc->dwHeight >= Context->YResolution) ) {
   			Context->XResolution = lpDDSurfaceDesc->dwWidth;
   			Context->YResolution = lpDDSurfaceDesc->dwHeight;
   			Context->RGBBitCount = lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount;
   			Context->UseSecondBestResult = false;
   			Context->Successful = true;
   			return DDENUMRET_CANCEL;
   		} else {
   			if (lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount < Context->SecondBestRGBBitCount) {
   				Context->SecondBestXResolution = lpDDSurfaceDesc->dwWidth;
   				Context->SecondBestYResolution = lpDDSurfaceDesc->dwHeight;
   				Context->SecondBestRGBBitCount = lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount;
   			} else if (lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount == Context->SecondBestRGBBitCount) {
   				if ( (Context->SecondBestXResolution < lpDDSurfaceDesc->dwWidth) ||
   					(Context->SecondBestYResolution < lpDDSurfaceDesc->dwHeight) ) {
   					Context->SecondBestXResolution = lpDDSurfaceDesc->dwWidth;
   					Context->SecondBestYResolution = lpDDSurfaceDesc->dwHeight;
   					Context->SecondBestRGBBitCount = lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount;
   				}
   			}
   			Context->Successful = true;
   			return DDENUMRET_OK;
   		}
   */
   	}
   
   	bool Continue = true;
   	if (Continue) {
   		return DDENUMRET_OK;
   	} else {
   		return DDENUMRET_CANCEL;
   	}
   }
   
   #include "DlgSetDisplayResolution.h"
   void CProbabilityGrid::InitializeDirectDraw(HWND hAppMainWnd, HWND hViewWindow)
   /*******************************************************************************
   //Created: 07/14/97 S.R.
   //Last Revision: 07/14/97 S.R.
   //Parameters:
   //	hAppMainWnd:	Window handle used for the application.
   //	hViewWindow:	Window handle for the view window.
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		This function initializes direct draw, and creates all the direct draw
   //	surfaces and stuff.
   //Method:
   //
   *******************************************************************************/
   {
   /*Create direct draw*/
   	LPDIRECTDRAW lpDDT;
   
   	HRESULT ddrval = DirectDrawCreate(NULL, &lpDDT, NULL);
   	if(ddrval != DD_OK) {
   		AfxMessageBox("DirectDraw Creation Failed");
   		DDrawDiagnostic(ddrval);
   		assert(FALSE);
   	}
   
   	ddrval = lpDDT->QueryInterface(IID_IDirectDraw2,(LPVOID *)&m_lpDD);
   	if(ddrval != DD_OK) {
   #ifdef _DEBUG
   		DDrawDiagnostic(ddrval);
   		AfxMessageBox("DD QueryInterface failed");
   #endif
   		return;
   	}
   	lpDDT->Release();
   
   /*Get the Bit Per Pixel and resolutions*/
   	DDSURFACEDESC ddsd;
   	ZeroMemory(&ddsd, sizeof(ddsd));
   	ddsd.dwSize = sizeof(ddsd);
   //	setmem(&ddsd, sizeof(ddsd), 0);
   	ddsd.dwFlags = DDSD_PIXELFORMAT | DDSD_HEIGHT | DDSD_WIDTH;
   	m_lpDD->GetDisplayMode(&ddsd);
   	
   	m_DisplayBitsPerPixel = ddsd.ddpfPixelFormat.dwRGBBitCount;
   	m_DisplayResolution.x = ddsd.dwWidth;
   	m_DisplayResolution.y = ddsd.dwHeight;
   	if (m_DisplayBitsPerPixel == 8) {
   		AfxMessageBox("This application works best if the display is set to 16, 24 or 32 bit color");
   		TEnumModesCallbackContext EnumModesCallbackContext;
   		EnumModesCallbackContext.Successful = false;
   		CStringList ModesList;
   		EnumModesCallbackContext.DisplayModes = &ModesList;
   		m_lpDD->EnumDisplayModes(0, NULL, &EnumModesCallbackContext, EnumModesCallback);
   		if (EnumModesCallbackContext.Successful) {
   			CDlgSetDisplayResolution DisplayResolution;
   			DisplayResolution.m_Resolutions = &ModesList;
   			DisplayResolution.DoModal();
   			CString SelectedResolution;
   			SelectedResolution = DisplayResolution.m_DisplayResolution;
   			CString temp = SelectedResolution.SpanExcluding(" ");
   			m_DisplayResolution.x = atoi(temp);
   			SelectedResolution = SelectedResolution.Mid(temp.GetLength()+3);
   			temp = SelectedResolution.SpanExcluding(" ");
   			m_DisplayResolution.y = atoi(temp);
   			SelectedResolution = SelectedResolution.Mid(temp.GetLength()+1);
   			temp = SelectedResolution.SpanExcluding("-");
   			m_DisplayBitsPerPixel = atoi(temp);
   		} else {
   			AfxMessageBox("Your display card does not support larger than 8 bit per pixel resolution");
   		}
   		ddrval = m_lpDD->SetCooperativeLevel(hAppMainWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
   	} else {
   		ddrval = m_lpDD->SetCooperativeLevel(hAppMainWnd, DDSCL_NORMAL);
   	}
   	if (ddrval != DD_OK) {
   		AfxMessageBox("DirectDraw SetCooperativeLevel failed");
   		DDrawDiagnostic(ddrval);
   		assert(FALSE);
   	}
   
   	ddrval = m_lpDD->SetDisplayMode(m_DisplayResolution.x, m_DisplayResolution.y, m_DisplayBitsPerPixel, 0, 0);
   	if(ddrval != DD_OK) {
   		AfxMessageBox("DDraw SetDisplayMode failed");
   		DDrawDiagnostic(ddrval);
   		assert(FALSE);
   	}
   
   //	Sleep(3000);
   /*Get the hardware capabilities*/
   	DDCAPS HardwareCaps, HELCaps;
   	ZeroMemory(&HardwareCaps, sizeof(HardwareCaps));
   	ZeroMemory(&HELCaps, sizeof(HELCaps));
   	HardwareCaps.dwSize = sizeof(HardwareCaps);
   	HELCaps.dwSize = HardwareCaps.dwSize;
   	m_lpDD->GetCaps(&HardwareCaps, &HELCaps);
   	CheckDDCapabilities(HardwareCaps, HELCaps);
   
   /*Get the information after the display mode has been changed*/
   	ZeroMemory(&ddsd, sizeof(ddsd));
   	ddsd.dwSize = sizeof(ddsd);
   //	setmem(&ddsd, sizeof(ddsd), 0);
   	ddsd.dwFlags = DDSD_PIXELFORMAT | DDSD_HEIGHT | DDSD_WIDTH;
   	m_lpDD->GetDisplayMode(&ddsd);
   	
   	m_DisplayBitsPerPixel = ddsd.ddpfPixelFormat.dwRGBBitCount;
   	m_DisplayRBitMask = ddsd.ddpfPixelFormat.dwRBitMask;
   	DWORD GBitMask, BBitMask;
   	GBitMask = ddsd.ddpfPixelFormat.dwGBitMask;
   	BBitMask = ddsd.ddpfPixelFormat.dwBBitMask;
   	m_DisplayResolution.x = ddsd.dwWidth;
   	m_DisplayResolution.y = ddsd.dwHeight;
   
   	
   /*	{
   	DDSCAPS ddscaps;
   	ddscaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
   	ddrval = m_lpDD2->GetAvailableVidMem(&ddscaps, &total,
   	    &free);
   	if(ddrval != DD_OK)
   	    return;
   	}
   */
   /*Create the DirectDraw objects*/
   
   // Create the primary surface with one back buffer, and clipper.
   /*	ddsd.dwSize = sizeof(ddsd);
   	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
   	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
   
   	ddsd.dwBackBufferCount = 1;
   */	
   	ZeroMemory(&ddsd, sizeof(ddsd));
   	ddsd.dwSize = sizeof(ddsd);
   	ddsd.dwFlags = DDSD_CAPS;
   	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
   
   	IDirectDrawSurface *pdds = NULL;
   	ddrval = m_lpDD->CreateSurface(&ddsd, &pdds, NULL);
   	if(ddrval != DD_OK) {
   		AfxMessageBox("DDraw CreateSurface Primary Surface Failed");
   		DDrawDiagnostic(ddrval);
   		assert(FALSE);
   	}
   	ddrval = pdds->QueryInterface(IID_IDirectDrawSurface3,(LPVOID *)&m_lpDDSPrimary);
   	if(ddrval != DD_OK) {
   #ifdef _DEBUG
   		DDrawDiagnostic(ddrval);
   		AfxMessageBox("DD QueryInterface failed");
   #endif
   	}
   	pdds->Release();
   
   /*Get the pixel format information*/
   /*	DDPIXELFORMAT PixelFormat;
   
   	PixelFormat.dwSize = sizeof(DDPIXELFORMAT);
   	PixelFormat.dwRBitMask = 0;
   	PixelFormat.dwGBitMask = 0;
   	PixelFormat.dwBBitMask = 0;
   	ddrval = m_lpDDSPrimary->GetPixelFormat(&PixelFormat);
   	if (ddrval == DD_OK) {
   		fprintf(stderr, "PixelFormat.DDPF_FOURCC = %d\n",(PixelFormat.dwFlags & DDPF_FOURCC) != 0);
   		fprintf(stderr, "PixelFormat.DDPF_RGB = %d\n",(PixelFormat.dwFlags & DDPF_RGB) != 0);
   		fprintf(stderr, "PixelFormat.dwRGBBitCount = %d\n",PixelFormat.dwRGBBitCount);
   		fprintf(stderr, "PixelFormat.dwRBitMask = %ul\n",PixelFormat.dwRBitMask);
   		fprintf(stderr, "PixelFormat.dwGBitMask = %ul\n",PixelFormat.dwGBitMask);
   		fprintf(stderr, "PixelFormat.dwBBitMask = %ul\n",PixelFormat.dwBBitMask);
   		fprintf(stderr, "PixelFormat.dwRGBAlphaBitMask = %ul\n",PixelFormat.dwRGBAlphaBitMask);
   	} else {
   		DDrawDiagnostic(ddrval, false);
   	}
   */
   
   	ZeroMemory(&ddsd, sizeof(ddsd));
   	ddsd.dwSize = sizeof(ddsd);
   	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
   	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
   	ddsd.dwWidth = m_DisplayResolution.x;
   	ddsd.dwHeight = m_DisplayResolution.y;
   
   	ddrval = m_lpDD->CreateSurface(&ddsd, &pdds, NULL);
   	if (ddrval != DD_OK) {
   		AfxMessageBox("Did not create Back Buffer");
   		DDrawDiagnostic(ddrval);
   		assert(FALSE);
   	}
   
   	ddrval = pdds->QueryInterface(IID_IDirectDrawSurface3,(LPVOID *)&m_lpDDSBack);
   	if(ddrval != DD_OK) {
   #ifdef _DEBUG
   		DDrawDiagnostic(ddrval);
   		AfxMessageBox("DD QueryInterface failed");
   #endif
   	}
   	pdds->Release();
   
   //Create the clipper
   	m_lpDD->CreateClipper(0, &m_lpDDCClipper, NULL);
   	if(ddrval != DD_OK) {
   		AfxMessageBox("DDraw CreateClipper Failed");
   		DDrawDiagnostic(ddrval);
   		assert(FALSE);
   	}
   	ddrval = m_lpDDCClipper->SetHWnd(0,hViewWindow);
   	if(ddrval != DD_OK) {
   		AfxMessageBox("DDraw ClipperSetHWnd Failed");
   		DDrawDiagnostic(ddrval);
   		assert(FALSE);
   	}
   	ddrval = m_lpDDSPrimary->SetClipper(m_lpDDCClipper);
   	if(ddrval != DD_OK) {
   		AfxMessageBox("DDraw SetClipper Failed");
   		DDrawDiagnostic(ddrval);
   		assert(FALSE);
   	}
   
   //Create the palette if necessary
   	if (m_DisplayBitsPerPixel == 8) {
   		CreateLogicalPalette(hAppMainWnd);
   		ddrval = m_lpDDSPrimary->SetPalette(m_lpDDPal);
   		if(ddrval != DD_OK) {
   			AfxMessageBox("DDraw SetPalette Failed");
   			DDrawDiagnostic(ddrval);
   			assert(FALSE);
   		}
   	}
   
   	InitializeDirectDrawSurfaces();
   }
   
   bool CProbabilityGrid::CheckDDCapabilities(DDCAPS HardwareCaps, DDCAPS HELCaps)
   {
   	bool everythingOK = true;
   	if ( !(HardwareCaps.dwCaps & DDCAPS_COLORKEY) ) {
   		if ( !(HELCaps.dwCaps & DDCAPS_COLORKEY) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   	if ( !(HardwareCaps.dwCaps & DDCAPS_BLT) ) {
   		if ( !(HELCaps.dwCaps & DDCAPS_BLT) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   	if ( !(HardwareCaps.dwCaps & DDCAPS_BLTSTRETCH) ) {
   		if ( !(HELCaps.dwCaps & DDCAPS_BLTSTRETCH) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   	if ( !(HardwareCaps.dwCaps & DDCAPS_CANCLIP) ) {
   		if ( !(HELCaps.dwCaps & DDCAPS_CANCLIP) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   	if ( !(HardwareCaps.dwCaps & DDCAPS_CANBLTSYSMEM) ) {
   		if ( !(HELCaps.dwCaps & DDCAPS_CANBLTSYSMEM) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   	if ( !(HardwareCaps.dwCaps & DDCAPS_CANCLIPSTRETCHED) ) {
   		if ( !(HELCaps.dwCaps & DDCAPS_CANCLIPSTRETCHED) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   	if ( !(HardwareCaps.dwCKeyCaps & DDCKEYCAPS_SRCBLT) ) {
   		if ( !(HELCaps.dwCKeyCaps & DDCKEYCAPS_SRCBLT) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   	if ( !(HardwareCaps.dwFXCaps & DDFXCAPS_BLTSTRETCHX) ) {
   		if ( !(HELCaps.dwFXCaps & DDFXCAPS_BLTSTRETCHX) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   	if ( !(HardwareCaps.dwFXCaps & DDFXCAPS_BLTSTRETCHY) ) {
   		if ( !(HELCaps.dwFXCaps & DDFXCAPS_BLTSTRETCHY) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   	if ( !(HardwareCaps.dwFXCaps & DDFXCAPS_BLTSTRETCHXN) ) {
   		if ( !(HELCaps.dwFXCaps & DDFXCAPS_BLTSTRETCHXN) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   	if ( !(HardwareCaps.dwFXCaps & DDFXCAPS_BLTSTRETCHYN) ) {
   		if ( !(HELCaps.dwFXCaps & DDFXCAPS_BLTSTRETCHYN) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   /*	if ( !(HardwareCaps.dwFXCaps & DDFXCAPS_BLTARITHSTRETCHY) ) {
   		if ( !(HELCaps.dwFXCaps & DDFXCAPS_BLTARITHSTRETCHY) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   	if ( !(HardwareCaps.dwFXCaps & DDFXCAPS_BLTARITHSTRETCHYN) ) {
   		if ( !(HELCaps.dwFXCaps & DDFXCAPS_BLTARITHSTRETCHYN) ) {
   			everythingOK = false;
   			ASSERT(FALSE);
   		}
   	}
   */
   	return everythingOK;
   }
   
   void CProbabilityGrid::InitializeDirectDrawSurfaces()
   /*******************************************************************************
   //Created: 07/14/97 S.R.
   //Last Revision: 07/14/97 S.R.
   //Parameters:
   //	none
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		Initializes all the Direct Draw surfaces we're using.
   //Method:
   //
   *******************************************************************************/
   {
   	CBitmap tempBmap;
   	LoadRobotBitmaps();
   /*Initialize the ProbabilityGrid Bitmap*/
   	InitializeProbGridBmpValues();
   //Create a DirectDrawSurface for the world bitmap
   	DDSURFACEDESC ddsd;
   	ZeroMemory(&ddsd, sizeof(ddsd));
   	ddsd.dwSize = sizeof(ddsd);
   	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
   	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
   	ddsd.dwWidth = m_DisplayResolution.x;
   	ddsd.dwHeight = m_DisplayResolution.y;
   
   	IDirectDrawSurface *pdds = NULL;
   	HRESULT ddrval = m_lpDD->CreateSurface(&ddsd, &pdds, NULL);
   	if (ddrval != DD_OK) {
   		AfxMessageBox("DDraw CreateSurface Primary Surface Failed");
   		DDrawDiagnostic(ddrval);
   		assert(FALSE);
   	}
   	ddrval = pdds->QueryInterface(IID_IDirectDrawSurface3,(LPVOID *)&m_lpDDSWorldBmp);
   	if(ddrval != DD_OK) {
   #ifdef _DEBUG
   		DDrawDiagnostic(ddrval);
   		AfxMessageBox("DD QueryInterface failed");
   #endif
   	}
   	pdds->Release();
   }
   
   //HRESULT CProbabilityGrid::TransferProbGridToSurface(IDirectDrawSurface* lpDDSDestination, CRect DestRect, unsigned char* Values, CPoint ValuesOffset)
   HRESULT CProbabilityGrid::TransferProbGridToSurface(IDirectDrawSurface3* lpDDSDestination, CRect DestRect, CPoint ValuesOffset)
   /*******************************************************************************
   //Created: 07/14/97 S.R.
   //Last Revision: 07/14/97 S.R.
   //Parameters:
   //	lpDDSDestination:	DirectDraw surface to transfer the probability grid to.
   //	DestRect:			The destination rectangle in surface coords.
   //	ValuesOffset:		The probability grid offset (which values should be transferred).
   //Return Value:
   //	The result of the transfer (any errors if they occur or DD_OK if everything o.k.)
   ********************************************************************************
   //Description:
   //		This function transfers a section of the probability grid to a direct
   //	draw surface.  This function directly transfers the probability grid values
   //	to the surface (no zooming involved.)  The zooming of the grid must be
   //	done separately.  The probability grid points starting at the grid point
   //	ValuesOffset are transferred to DestRect in the direct draw surface.
   //Method:
   //
   *******************************************************************************/
   {
   	HRESULT ddrval;
   	DDSURFACEDESC SurfaceDesc;
   	memset(&SurfaceDesc, 0, sizeof(SurfaceDesc));
   	SurfaceDesc.dwSize = sizeof(SurfaceDesc);
   /*Lock the surface*/
   	do {
   		ddrval = lpDDSDestination->Lock(NULL, &SurfaceDesc, DDLOCK_SURFACEMEMORYPTR, NULL);
   		if (ddrval == DDERR_SURFACELOST) {
   			lpDDSDestination->Restore();
   		} else if (ddrval != DDERR_WASSTILLDRAWING) break;
   		if (ddrval != DD_OK) {
   			fprintf(stderr,"surface lock is failing, %d\n",ddrval);
   			fflush(stderr);
   		}
   	} while (ddrval != DD_OK);
   	if (ddrval != DD_OK) {
   		lpDDSDestination->Unlock(NULL);
   		return ddrval;
   	}
   /*Get Surface Information*/
   	unsigned long SurfaceWidth = SurfaceDesc.dwWidth;
   	unsigned long SurfaceHeight = SurfaceDesc.dwHeight;
   	unsigned long SurfacePitch = SurfaceDesc.lPitch;
   /*Get Source Information*/
   	short SourceWidth = cWorldGridXSize;
   	short SourceHeight = cWorldGridYSize;
   /*Adjust DestRect if it is outside of Surface*/
   	if (DestRect.left >= (long)SurfaceWidth) {
   		lpDDSDestination->Unlock(NULL);
   		return DDERR_GENERIC;
   	}
   	if (DestRect.right >= (long)SurfaceWidth) DestRect.right = SurfaceWidth-1;
   	if (DestRect.top >= (long)SurfaceHeight) {
   		lpDDSDestination->Unlock(NULL);
   		return DDERR_GENERIC;
   	}
   	if (DestRect.bottom >= (long)SurfaceHeight) DestRect.bottom = SurfaceHeight-1;
   /*Transfer the data*/
   	unsigned short DestWidth = DestRect.Width();
   	unsigned short DestHeight = DestRect.Height();
   	World* RobotWorld = m_RobotWorld;
   	unsigned char SurfaceBytesPerPixel = m_DisplayBitsPerPixel/8;
   	unsigned char* pSurfaceBits = ((unsigned char*)SurfaceDesc.lpSurface) +
   		(SurfaceWidth * DestRect.top + DestRect.left) * (SurfaceBytesPerPixel);
   	unsigned char* pSourceBits = (unsigned char*)m_ProbGridBmpValues +
   		(SourceWidth * ValuesOffset.y + ValuesOffset.x) * (SurfaceBytesPerPixel);
   #ifdef HARDWARE_ZOOM
   /*Make sure the DestHeight and DestWidth don't overrun the bounds of the surface*/
   	if (DestHeight * m_ZoomLevel >= SurfaceHeight) DestHeight = unsigned short(SurfaceHeight / m_ZoomLevel);
   	if (DestWidth * m_ZoomLevel >=SurfaceWidth) DestWidth = unsigned short(SurfaceWidth / m_ZoomLevel);
   #endif
   #ifdef _DEBUG
   //	static bool Blah = false;
   //	BYTE Color;
   //	if (Blah) {
   //		Blah = false;
   //		Color = 255;
   //	} else {
   //		Blah = true;
   //		Color = 0;
   //	}
   //	fprintf(stderr, "TransferProbGridToSurface\n");
   //	fprintf(stderr, "DestHeight = %d\n", DestHeight);
   //	fprintf(stderr, "DestWidth = %d\n", DestWidth);
   #endif
   
   /*Transfer bytes*/
   	for ( unsigned short row = 0; row <= DestHeight; row++ ) {
   //#define USEPROBGRIDVALUES
   #ifdef USEPROBGRIDVALUES
   		for ( unsigned short column = 0; column <= DestWidth; column++ ) {
   			unsigned long temp;
   			temp = ProbabilityToColor((*RobotWorld)[column + ValuesOffset.x][cWorldGridXSize - 1 - (row + ValuesOffset.y)]);
   			switch (m_DisplayBitsPerPixel) {
   			case 32:
   				pSurfaceBits[column*SurfaceBytesPerPixel + 3] = temp >> 24;
   			case 24:
   				pSurfaceBits[column*SurfaceBytesPerPixel + 2] = temp >> 16;
   			case 16:
   				pSurfaceBits[column*SurfaceBytesPerPixel + 1] = temp >> 8;
   			case 8:
   				pSurfaceBits[column*SurfaceBytesPerPixel] = temp;
   				break;
   			}
   		}
   		pSurfaceBits += SurfacePitch;
   		pSourceBits += SourceWidth*(SurfaceBytesPerPixel);
   #else
   #ifdef HARDWARE_ZOOM
   /*		for ( unsigned short column = 0; column <= DestWidth*SurfaceBytesPerPixel; column++ ) {
   			pSurfaceBits[column] = pSourceBits[column];
   		}
   */
   		CopyMemory(pSurfaceBits, pSourceBits, DestWidth*SurfaceBytesPerPixel);
   //		FillMemory(pSurfaceBits, DestWidth*SurfaceBytesPerPixel, Color);
   		pSurfaceBits += SurfacePitch;
   		pSourceBits += SourceWidth*(SurfaceBytesPerPixel);
   #else
   		int Zoom = m_ZoomLevel;
   		unsigned short FinalColumn = DestWidth * SurfaceBytesPerPixel;
   		for ( unsigned short column = 0; column <= FinalColumn; column++ ) {
   			unsigned short StartColumn = (column/SurfaceBytesPerPixel)*Zoom*SurfaceBytesPerPixel + (column%SurfaceBytesPerPixel);
   			unsigned short FinalJ = StartColumn + Zoom * SurfaceBytesPerPixel;
   			for (unsigned short j = StartColumn; j < FinalJ; j += SurfaceBytesPerPixel) {
   				pSurfaceBits[j] = pSourceBits[column];
   			}
   		}
   		unsigned char* pOriginalBits = pSurfaceBits;
   		pSurfaceBits += SurfacePitch;
   		for (int i = 1; i < m_ZoomLevel; i++) {
   			memcpy(pSurfaceBits, pOriginalBits, FinalColumn*Zoom);
   			pSurfaceBits += SurfacePitch;
   		}
   
   		pSourceBits += SourceWidth*(SurfaceBytesPerPixel);
   #endif	//Hardware Zoom
   #endif
   	}
   /*Unlock the surface*/
   	do {
   		ddrval = lpDDSDestination->Unlock(NULL);
   		if (ddrval == DDERR_SURFACELOST) {
   			lpDDSDestination->Restore();
   		} else if (ddrval != DDERR_WASSTILLDRAWING) break;
   	} while (ddrval != DD_OK);
   	
   	if (ddrval != DD_OK) {
   		fprintf(stderr, "TransferProbGridToSurface Failed\n");
   		DDrawDiagnostic(ddrval);
   		assert(FALSE);
   	}
   	return DD_OK;
   }
   
   
   CRect CProbabilityGrid::RobotToClientCoords(robRECT RobotRect)
   {
   	CRect ClientCoords;
   	CPoint ClientPoint;
   	ClientPoint = RobotToClientCoords(RobotRect.TopLeft());
   	ClientCoords.top = ClientPoint.y;
   	ClientCoords.left = ClientPoint.x;
   	ClientPoint = RobotToClientCoords(RobotRect.BottomRight());
   	ClientCoords.bottom = ClientPoint.y;
   	ClientCoords.right = ClientPoint.x;
   	ClientCoords.NormalizeRect();
   	return ClientCoords;
   }
   
   CPoint CProbabilityGrid::RobotToClientCoords(robPOINT point)
   /*******************************************************************************
   //Created: 07/14/97 S.R.
   //Last Revision: 07/14/97 S.R.
   //Parameters:
   //	point:	Robot point to transform to client coordinates.
   //Return Value:
   //	the transformed point.
   ********************************************************************************
   //Description:
   //		This function transforms a robot coordinate into a window client coordinate.
   //Method:
   //
   *******************************************************************************/
   {
   	int ScrollPos = m_ViewWindow->GetScrollPos(SB_HORZ);
   	point.x /= cWorldGridPointSize;
   	point.x -= ScrollPos;
   	point.x += cWorldGridXSize/2.0;
   	point.x *= m_ZoomLevel;
   
   	ScrollPos = m_ViewWindow->GetScrollPos(SB_VERT);
   	point.y /= -cWorldGridPointSize;
   	point.y -= ScrollPos;
   	point.y += cWorldGridYSize/2.0;
   	point.y *= m_ZoomLevel;
   	CPoint temp;
   	temp.x = Round(point.x);
   	temp.y = Round(point.y);
   	return temp;
   }
   
   robPOINT CProbabilityGrid::ClientToRobotCoords(CPoint point)
   /*******************************************************************************
   //Created: 07/14/97 S.R.
   //Last Revision: 07/14/97 S.R.
   //Parameters:
   //	point:	A client coordinate to transform to robot coordinates.
   //Return Value:
   //	The transformed point.
   ********************************************************************************
   //Description:
   //		This function transforms a window client point into robot coordinates.
   //Method:
   //
   *******************************************************************************/
   {
   	robPOINT temp;
   	temp.x = point.x;
   	temp.y = point.y;
   
   	int ScrollPos = m_ViewWindow->GetScrollPos(SB_HORZ);
   	temp.x /= m_ZoomLevel;
   	temp.x += ScrollPos;
   	temp.x -= cWorldGridXSize/2.0;
   	temp.x *= cWorldGridPointSize;
   
   	ScrollPos = m_ViewWindow->GetScrollPos(SB_VERT);
   	temp.y /= m_ZoomLevel;
   	temp.y += ScrollPos;
   	temp.y = -temp.y + cWorldGridYSize/2.0;
   	temp.y *= cWorldGridPointSize;
   	return temp;
   }
   
   void CProbabilityGrid::RectifyRectangles(CRect* rcRect, CRect* destRect)
   /*******************************************************************************
   //Created: 07/14/97 S.R.
   //Last Revision: 07/14/97 S.R.
   //Parameters:
   //	rcRect:		Rectangle of bitmap resource.
   //	destRect:	The Destination of the bitmap on the back buffer.
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //
   //Method:
   //
   *******************************************************************************/
   {
   /*	if ( (destRect->right < 0) || (destRect->bottom < 0) ) {
   		destRect->right = destRect->left = 0;
   		destRect->top = destRect->bottom = 0;
   		rcRect->right = rcRect->left = 0;
   		rcRect->top = rcRect->bottom = 0;
   		return;
   	}
   	if ( (destRect->top > m_DisplayResolution.y) || (destRect->left >= m_DisplayResolution.x) ) {
   		destRect->right = destRect->left = 0;
   		destRect->top = destRect->bottom = 0;
   		rcRect->right = rcRect->left = 0;
   		rcRect->top = rcRect->bottom = 0;
   		return;
   	}
   	if (destRect->left < 0) {
   		rcRect->left -= destRect->left;
   		if (rcRect->left < 0) rcRect->left = 0;
   		destRect->left = 0;
   	}
   	if (destRect->top < 0) {
   		rcRect->top -= destRect->top;
   		if (rcRect->top < 0) rcRect->top = 0;
   		destRect->top = 0;
   	}
   	if (destRect->bottom >= m_DisplayResolution.y) {
   		rcRect->bottom -= destRect->bottom - m_DisplayResolution.y + 1;
   		destRect->bottom = m_DisplayResolution.y - 1;
   	}
   	if (destRect->right >= m_DisplayResolution.x) {
   		rcRect->right -= destRect->right - m_DisplayResolution.y + 1;
   		destRect->right = m_DisplayResolution.x - 1;
   	}
   	if (rcRect->left > rcRect->right) {
   		rcRect->left = rcRect->right;
   	}
   	if (rcRect->top > rcRect->bottom) {
   		rcRect->top = rcRect->bottom;
   	}
   */
   	if (destRect->left < 0) {
   		rcRect->left -= destRect->left;
   		if (rcRect->left < 0) rcRect->left = 0;
   		destRect->left = 0;
   	}
   	if (destRect->right < 0) {
   		rcRect->right -= destRect->right;
   		if (rcRect->right < 0) rcRect->right = 0;
   		destRect->right = 0;
   	}
   	if (destRect->top < 0) {
   		rcRect->top -= destRect->top;
   		if (rcRect->top < 0) rcRect->top = 0;
   		destRect->top = 0;
   	}
   	if (destRect->bottom < 0) {
   		rcRect->bottom -= destRect->bottom;
   		if (rcRect->bottom < 0) rcRect->bottom = 0;
   		destRect->bottom = 0;
   	}
   	if ( (destRect->top >= m_DisplayResolution.y) || (destRect->bottom >= m_DisplayResolution.y) ) {
   		destRect->top = 0;
   		destRect->bottom = 0;
   	}
   	if ( (destRect->left >= m_DisplayResolution.x) || (destRect->right >= m_DisplayResolution.x) ) {
   		destRect->left = 0;
   		destRect->right = 0;
   	}
   	if (rcRect->left > rcRect->right) {
   		rcRect->left = rcRect->right;
   	}
   	if (rcRect->top > rcRect->bottom) {
   		rcRect->top = rcRect->bottom;
   	}
   
   }
   
   void CProbabilityGrid::RectifyStretchedRectangles(CRect* rcRect, CRect* destRect)
   {
   	double HorzSlope = ((double)rcRect->left - rcRect->right) / (destRect->left - destRect->right);
   	double HorzX1 = destRect->left;
   	double HorzX2 = destRect->right;
   	double HorzY1 = rcRect->left;
   	double HorzY2 = rcRect->right;
   	double VertSlope = ((double)rcRect->top - rcRect->bottom) / (destRect->top - destRect->bottom);
   	double VertX1 = destRect->top;
   	double VertX2 = destRect->bottom;
   	double VertY1 = rcRect->top;
   	double VertY2 = rcRect->bottom;
   
   	if (destRect->left < 0) {
   		rcRect->left = long(HorzY1 - HorzX1 * HorzSlope);
   		if (rcRect->left < 0) rcRect->left = 0;
   		destRect->left = 0;
   	}
   	if (destRect->right < 0) {
   		rcRect->right = long(HorzY1 - HorzX1 * HorzSlope);
   		if (rcRect->right < 0) rcRect->right = 0;
   		destRect->right = 0;
   	}
   	if (destRect->top < 0) {
   		rcRect->top = long(VertY1 - VertX1 * VertSlope);
   		if (rcRect->top < 0) rcRect->top = 0;
   		destRect->top = 0;
   	}
   	if (destRect->bottom < 0) {
   		rcRect->bottom = long(VertY1 - VertX1 * VertSlope);
   		if (rcRect->bottom < 0) rcRect->bottom = 0;
   		destRect->bottom = 0;
   	}
   	if ( (destRect->top >= m_DisplayResolution.y) || (destRect->bottom >= m_DisplayResolution.y) ) {
   		destRect->top = 0;
   		destRect->bottom = 0;
   	}
   	if ( (destRect->left >= m_DisplayResolution.x) || (destRect->right >= m_DisplayResolution.x) ) {
   		destRect->left = 0;
   		destRect->right = 0;
   	}
   	if (rcRect->left > rcRect->right) {
   		rcRect->left = rcRect->right;
   	}
   	if (rcRect->top > rcRect->bottom) {
   		rcRect->top = rcRect->bottom;
   	}
   }
   
   
   void CProbabilityGrid::RedrawRobot(robPOINT RobotPosition, float RobotHeading, CWnd* pWnd)
   /*******************************************************************************
   //Created: 07/14/97 S.R.
   //Last Revision: 07/14/97 S.R.
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
   {
   	CRect UpdateRect;
   
   	unsigned long RobotBmpFrameWidth;
   	unsigned long RobotBmpFrameHeight;
   
   /*Get the right robot bitmap dimensions*/
   	if (m_ZoomLevel == cZoomLevel1) {
   		RobotBmpFrameWidth = cRobotBmp1FrameWidth;
   		RobotBmpFrameHeight = cRobotBmp1FrameHeight;
   	} else if (m_ZoomLevel == cZoomLevel2) {
   		RobotBmpFrameWidth = cRobotBmp2FrameWidth;
   		RobotBmpFrameHeight = cRobotBmp2FrameHeight;
   	} else if (m_ZoomLevel == cZoomLevel3) {
   		RobotBmpFrameWidth = cRobotBmp3FrameWidth;
   		RobotBmpFrameHeight = cRobotBmp3FrameHeight;
   	} else {
   		RobotBmpFrameWidth = cRobotBmp1FrameWidth;
   		RobotBmpFrameHeight = cRobotBmp1FrameHeight;
   		AfxMessageBox("Illegal m_ZoomLevel");
   		ASSERT(FALSE);
   	}
   /*Get the update rectangle*/
   	CPoint RobotClientCoord;
   	RobotClientCoord = RobotToClientCoords(RobotPosition);
   	UpdateRect.left = RobotClientCoord.x - RobotBmpFrameWidth / 2 - 1;
   	UpdateRect.right = RobotClientCoord.x + RobotBmpFrameWidth / 2 + 1;
   	UpdateRect.top = RobotClientCoord.y - RobotBmpFrameHeight / 2 - 1;
   	UpdateRect.bottom = RobotClientCoord.y + RobotBmpFrameHeight / 2 + 1;
   	CRect ClientRect;
   	pWnd->GetClientRect(&ClientRect);
   	if (UpdateRect.left < 0) UpdateRect.left = 0;
   	if (UpdateRect.top < 0) UpdateRect.top = 0;
   	if (UpdateRect.right > ClientRect.right) UpdateRect.right = ClientRect.right;
   	if (UpdateRect.bottom > ClientRect.bottom) UpdateRect.bottom = ClientRect.bottom;
   
   	m_ViewWindow->RedrawWindow(UpdateRect, NULL, RDW_INVALIDATE | RDW_NOERASE);
   }
   /*
   void CProbabilityGrid::OnMoveRobot(robPOINT RobotPosition, float RobotHeading)
   {
   //Move the update rectangle to include the old robot position
   	static CRect OldRobotRect = (0,0,0,0);
   	CPoint RobotClientCoord;
   	RobotClientCoord = RobotToClientCoords(OldRobotPosition);
   	OldRobotRect.left = RobotClientCoord.x - RobotBmpFrameWidth / 2 - 1;
   	OldRobotRect.right = RobotClientCoord.x + RobotBmpFrameWidth / 2 + 1;
   	OldRobotRect.top = RobotClientCoord.y - RobotBmpFrameHeight / 2 - 1;
   	OldRobotRect.bottom = RobotClientCoord.y + RobotBmpFrameHeight / 2 + 1;
   	UpdateRect.UnionRect(&OldRobotRect, &UpdateRect);
   	CRect ClientRect;
   	pWnd->GetClientRect(&ClientRect);
   	if (UpdateRect.left < 0) UpdateRect.left = 0;
   	if (UpdateRect.top < 0) UpdateRect.top = 0;
   	if (UpdateRect.right > ClientRect.right) UpdateRect.right = ClientRect.right;
   	if (UpdateRect.bottom > ClientRect.bottom) UpdateRect.bottom = ClientRect.bottom;
   	OldRobotPosition = RobotPosition;
   }
   */
   
   POINT CProbabilityGrid::RobotToDDSurface(robPOINT RobotPoint, CPoint SurfaceOffset)
   /*******************************************************************************
   //Created:  12/19/97 S.R.
   //Last Revision:  12/19/97 S.R.
   //Parameters:
   //	RobotPoint:				Robot coordinate to transform
   //	SurfaceXOffset:		The offset of the surface with respect to the probability grid bitmap.
   //Return Value:
   //	Transformed value of RobotPoint.
   ********************************************************************************
   //Description:
   //		This function takes a robot coordinate and transforms it into a Direct Draw
   //	surface coordinate.  The surface is offset with respect to the probability grid
   //	bitmap by an amount SurfaceOffset, and the surface is zoomed in a value
   //	m_ZoomLevel.
   //Method:
   //
   *******************************************************************************/
   {
   	POINT SurfacePoint;
   	SurfacePoint.x = 	Round(((RobotPoint.x) / cWorldGridPointSize + cWorldGridXSize / 2.0 - SurfaceOffset.x)*m_ZoomLevel);
   	SurfacePoint.y = 	Round(((-RobotPoint.y) / cWorldGridPointSize + cWorldGridYSize / 2.0 - SurfaceOffset.y)*m_ZoomLevel);
   	return SurfacePoint;
   }
   
   robPOINT CProbabilityGrid::ProbGridToRobotCoord(POINT ProbGridPoint)
   /*This function transforms a probability grid coordinate into a robot world
   coordinate*/
   {
   	robPOINT RobotPoint;
   	RobotPoint.x = (ProbGridPoint.x - cWorldGridXSize / 2.0) * cWorldGridPointSize;
   	RobotPoint.y = (cWorldGridYSize / 2.0 - ProbGridPoint.y) * cWorldGridPointSize;
   	return RobotPoint;
   }
   
   robRECT CProbabilityGrid::ProbGridToRobotCoord(CRect ProbGridRect)
   /*This function transforms a rectangle in probability grid coordinates into robot world
   coordinates*/
   {
   	robRECT RobotRect;
   	robPOINT RobotPoint;
   	
   	RobotPoint = ProbGridToRobotCoord(ProbGridRect.TopLeft());
   	RobotRect.top = RobotPoint.y;
   	RobotRect.left = RobotPoint.x;
   	
   	RobotPoint = ProbGridToRobotCoord(ProbGridRect.BottomRight());
   	RobotRect.bottom = RobotPoint.y;
   	RobotRect.right = RobotPoint.x;
   	
   	RobotRect.Normalize();
   	
   	return RobotRect;
   }
   
   float CProbabilityGrid::RobotToDDSurfaceX(double X, int SurfaceXOffset)
   /*******************************************************************************
   //Created:  S.R.
   //Last Revision:  S.R.
   //Parameters:
   //	X:						Robot x coordinate to transform
   //	SurfaceXOffset:	The offset of the surface with respect to the probability grid bitmap.
   //Return Value:
   //	Transformed value of X.
   ********************************************************************************
   //Description:
   //		This function takes a robot x coordinate and transforms it into a DirectDraw
   //	surface x coordinate.  The surface is offset with respect to the probability grid
   //	bitmap by an amount BmpXOffset, and the surface is zoomed in a value
   //	m_ZoomLevel.
   //********NOTE!!!!!!!!!!!!*******
   //	This value must be rounded before being used to draw to the surface.
   //	Otherwise you end up with a "fencepost" problem.
   //Method:
   //
   *******************************************************************************/
   {
   	return float(((X) / cWorldGridPointSize + cWorldGridXSize / 2.0 - SurfaceXOffset)*m_ZoomLevel);
   }
   
   float CProbabilityGrid::RobotToDDSurfaceY(double Y, int SurfaceYOffset)
   /*******************************************************************************
   //Created:  S.R.
   //Last Revision:  S.R.
   //Parameters:
   //	Y:					Robot y coordinate to transform
   //	SurfaceYOffset:		The offset of the surface with respect to the probability grid bitmap.
   //Return Value:
   //	Transformed value of Y.
   ********************************************************************************
   //Description:
   //		This function takes a robot y coordinate and transforms it into a DirectDraw
   //	surface y coordinate.  The surface is offset with respect to the probability grid
   //	bitmap by an amount BmpYOffset, and the surface is zoomed in a value
   //	m_ZoomLevel.
   //********NOTE!!!!!!!!!!!!*******
   //	This value must be rounded before being used to draw to the surface.
   //	Otherwise you end up with a "fencepost" problem.
   //Method:
   //
   *******************************************************************************/
   {
   	return float(((-Y) / cWorldGridPointSize + cWorldGridYSize / 2.0 - SurfaceYOffset)*m_ZoomLevel);
   }
   
   void CProbabilityGrid::Serialize(CArchive & archive)
   /*******************************************************************************
   //Created: 07/17/97 S.R.
   //Last Revision: 07/17/97 S.R.
   //Parameters:
   //	archive:	the archive where the information is stored
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		This function serializes the probability grid.  This is used in saving and
   //	opening the program.
   //Method:
   //
   *******************************************************************************/
   {
   	UINT nVersion;
   	unsigned long i, j;
   	unsigned char temp;
   	CObject::Serialize(archive);
   	if (archive.IsStoring()) {
   		nVersion = VERSION_NUMBER;
   		archive << nVersion;
   		archive << m_ZoomLevel;
   		for (i = 0; i < cWorldGridXSize; i++) {
   			unsigned char* WorldRow = (*m_RobotWorld)[i];
   			for (j = 0; j < cWorldGridYSize; j++) {
   				temp = WorldRow[j];
   				archive << temp;
   			}
   		}
   //		archive << m_ColorRobot;
   		short NumberOfRobots = GetNumberOfRobots();
   		archive << NumberOfRobots;
   		for (i = 0;NumberOfRobots > 0; i++, NumberOfRobots--) {
   			m_RobotList[i]->Serialize(archive);
   		}
   	} else {
   		archive >> nVersion;
   		New();
   		archive >> m_ZoomLevel;
   		for (i = 0; i < cWorldGridXSize; i++) {
   			unsigned char* WorldRow = (*m_RobotWorld)[i];
   			for (j = 0; j < cWorldGridYSize; j++) {
   				archive >> temp;
   				WorldRow[j] = temp;
   				SetBitmapValues(cWorldGridYSize - 1 - j, i, ProbabilityToColor(WorldRow[j]));
   			}
   		}
   //		if ( (nVersion > 1) && (nVersion != (UINT)-1) ) archive >> m_ColorRobot;
   		if (nVersion > 8) {
   			PurgeRobots();
   			short NumberOfRobots;
   			archive >> NumberOfRobots;
   			for (; NumberOfRobots != 0; NumberOfRobots--) {
   				CRobot* NewRobot = new CRobot(m_ViewWindow, this);
   				NewRobot->Create(m_ViewWindow);
   				NewRobot->Serialize(archive);
   				if (!AddRobot(NewRobot)) {
   					delete NewRobot;
   				}
   //				archive >> m_RobotList[i];
   			}
   		} else {
   			int ColorRobot;
   			archive >> ColorRobot;
   			switch (ColorRobot) {
   			case 9:		//Black Green
   				ColorRobot = crBlack;
   				break;
   			case 200:	//Orange Black
   				ColorRobot = crOrange;
   				break;
   			case 300:	//Yellow Black
   				ColorRobot = crYellow;
   				break;
   			case 400:	//Chrome
   				ColorRobot = crChrome;
   				break;
   			} 
   			GetRobot(0)->SetColor(TRobotColor(ColorRobot));
   		}
   	}
   	m_RWGraphicObjectList->Serialize(archive);
   	m_ExecutableObjectList->Serialize(archive);
   }
   
   void CProbabilityGrid::SerializeV2(CArchive & archive)
   /*******************************************************************************
   //Created: 07/17/97 S.R.
   //Last Revision: 07/17/97 S.R.
   //Parameters:
   //	archive:	the archive where the information is stored
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		This function serializes the probability grid.  This is used in saving and
   //	opening the program.
   //Method:
   //
   *******************************************************************************/
   {
   	unsigned long i, j;
   	unsigned char temp;
   	CObject::Serialize(archive);
   	if (archive.IsStoring()) {
   		ASSERT(FALSE);
   	} else {
   		New();
   		archive >> m_ZoomLevel;
   		for (i = 0; i < cWorldGridXSize; i++) {
   			unsigned char* WorldRow = (*m_RobotWorld)[i];
   			for (j = 0; j < cWorldGridYSize; j++) {
   				archive >> temp;
   				WorldRow[j] = temp;
   				SetBitmapValues(cWorldGridYSize - 1 - j, i, ProbabilityToColor(WorldRow[j]));
   			}
   		}
   //		if ( (nVersion > 1) && (nVersion != (UINT)-1) ) archive >> m_ColorRobot;
   		int ColorRobot;
   		archive >> ColorRobot;
   		GetRobot(0)->SetColor(TRobotColor(ColorRobot));
   	}
   	m_RWGraphicObjectList->SerializeV2(archive);
   	m_ExecutableObjectList->SerializeV2(archive);
   }
   
   void CProbabilityGrid::RestorePalette()
   /*******************************************************************************
   //Created: 07/18/97 S.R.
   //Last Revision: 10/29/97 S.R.
   //Parameters:
   //	none
   //Return Value:
   //	none
   ********************************************************************************
   //Description:
   //		Use this when the window is brought in focus, and its palette needs to
   //	be restored.
   //Method:
   //
   *******************************************************************************/
   {
   /*Select the right palette*/
   	if (m_DisplayBitsPerPixel == 8) {
   		HRESULT ddrval = m_lpDDSPrimary->SetPalette(m_lpDDPal);
   		if(ddrval != DD_OK) {
   			AfxMessageBox("DDraw SetPalette Failed");
   			DDrawDiagnostic(ddrval);
   			assert(FALSE);
   		}
   	}
   }
   
   void CProbabilityGrid::RectifyPoint(CPoint* point) {
   /*******************************************************************************
   //Created: 09/23/97 S.R.
   //Last Revision: 09/23/97 S.R.
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
   	if (point->x < 0) point->x = 0;
   	if (point->x > m_DisplayResolution.x) point->x = m_DisplayResolution.x;
   	if (point->y < 0) point->y = 0;
   	if (point->y > m_DisplayResolution.y) point->y = m_DisplayResolution.y;
   }
   
   void CProbabilityGrid::New()
   /*******************************************************************************
   //Created: 11/14/97 S.R.
   //Last Revision: 11/14/97 S.R.
   //Parameters:
   //	none.
   //Return Value:
   //	none.
   ********************************************************************************
   //Description:
   //		Call this function whenever the application wants a new document.
   //Method:
   //
   *******************************************************************************/
   {
   	unsigned long InitValue = ProbabilityToColor(cWorldGridInitialValue);
   	for (unsigned int i = 0; i < cWorldGridYSize; i++) {
   		for (unsigned int j = 0; j < cWorldGridXSize; j++) {
   			(*m_RobotWorld)[i][j] = cWorldGridInitialValue;
   			SetBitmapValues(i,j,InitValue);
   //			SetBitmapValues(i,j,Round(255.0/cWorldGridXSize*j));
   		}
   	}
   	m_RWGraphicObjectList->Purge();
   	m_ExecutableObjectList->Purge();
   	PurgeRobots();
   	CRobot* NewRobot = new CRobot(m_ViewWindow, this);
   	NewRobot->Create(m_ViewWindow);
   	AddRobot(NewRobot);
   }
   
   void CProbabilityGrid::ProbGridToFile()
   {
   	FILE* ProbGridFile = fopen("ProbabilityGrid.dat","w");
   	for (unsigned int i = 0; i < cWorldGridXSize; i++) {
   		unsigned char* WorldRow = (*m_RobotWorld)[i];
   		for (unsigned int j = 0; j < cWorldGridYSize; j++) {
   			fputc(WorldRow[j],ProbGridFile);
   		}
   	}
   	fflush(ProbGridFile);
   	fclose(ProbGridFile);
   }
   
   int CProbabilityGrid::GetGridValue(robPOINT RobotCoord)
   {
   	int x = RobotToBitmapX(RobotCoord.x);
   	int y = RobotToBitmapY(RobotCoord.y);
   	return((*m_RobotWorld)[x][cWorldGridYSize - 1 - y]);
   }
   
   void CProbabilityGrid::GetGridValues(robPOINT Center, int Width, int Height, int **Values)
   {
   	if (Values == NULL) return;
   	int x = int(Center.x / cWorldGridPointSize + cWorldGridXSize / 2);
   	int y = int(Center.y / cWorldGridPointSize + cWorldGridYSize / 2 - 1);
   	x -= Width / 2;
   	y -= Height / 2;
   	int gi, gj;
   	for (int i = 0; i < Width; i++) {
   		gi = x+i;
   		for (int j = 0; j < Height; j++) {
   			gj = y+j;
   			if ( (gi < 0) || (gj < 0) || (gi >= cWorldGridXSize) || (gj >= cWorldGridYSize) ) {
   				Values[i][j] = (cMinProb + cMaxProb) / 2;
   			} else {
   				Values[i][j] = (*m_RobotWorld)[gi][gj];
   			}
   		}
   	}
   }
   
   void CProbabilityGrid::SetGridValue(robPOINT RobotCoord, int ProbValue, int Width, bool ReplaceReservedValues)
   /*******************************************************************************
   //Created: 12/18/97 S.R.
   //Last Revision: 12/18/97 S.R.
   //Parameters:
   //	RobotCoord: The robot coordinate to set to a value.
   //	ProbValue:	The value to set.
   //Return Value:
   //	none.
   ********************************************************************************
   //Description:
   //		This function sets the given robot coordinate in the probability grid to
   //	ProbValue.
   //Method:
   //
   *******************************************************************************/
   {
   	int OldGridValue;
   	int x0 = RobotToBitmapX(RobotCoord.x);
   	int y0 = RobotToBitmapY(RobotCoord.y);
   	int x = x0 - Width/2;
   	int y = y0 - Width/2;
   	for (int i = 0; i < Width; i++) {
   		for (int j = 0; j < Width; j++) {
   			OldGridValue = (*m_RobotWorld)[x][cWorldGridYSize - 1 - y];
   			if ( ((OldGridValue >= cMinProb) && (OldGridValue <= cMaxProb)) || ReplaceReservedValues ) {
   				SetBitmapValues(y,x,ProbabilityToColor(ProbValue));
   				(*m_RobotWorld)[x][cWorldGridYSize - 1 - y] = ProbValue;
   			} 
   			y++;
   		}
   		y = y0 - Width/2;
   		x++;
   	}
   }
   
   void CProbabilityGrid::SetGridValue(robPOINT StartRobotCoord, robPOINT EndRobotCoord, int ProbValue, int Width, bool ReplaceReservedValues)
   /*******************************************************************************
   //Created: 12/18/97 S.R.
   //Last Revision: 12/18/97 S.R.
   //Parameters:
   //	StartRobotCoord:	The robot coordinate to set to a value.
   //	EndRobotCoord:		The end robot coordinate to set to a value.
   //	ProbValue:			The value to set.
   //Return Value:
   //	none.
   ********************************************************************************
   //Description:
   //		This function sets the given robot coordinates between StartRobotCoord and
   //	EndRobotCoord in the probability grid to ProbValue.
   //Method:
   //
   *******************************************************************************/
   {
   /*	int x0 = RobotToBitmapX(StartRobotCoord.x);
   	int y0 = RobotToBitmapY(StartRobotCoord.y);
   	int x1 = RobotToBitmapX(EndRobotCoord.x);
   	int y1 = RobotToBitmapY(EndRobotCoord.y);
   	double x = x0;
   	double y = y0;
   	double theta = atan2(y1 - y0, x1 - x0);
   	double c = cos(theta);
   	double s = sin(theta);
   	int x2,y2;
   	do {
   		x2 = x - Width/2;
   		y2 = y - Width/2;
   		for (int i = 0; i < Width; i++) {
   			for (int j = 0; j < Width; j++) {
   				SetBitmapValues(y2,x2,ProbabilityToColor(ProbValue));
   				(*m_RobotWorld)[x2][cWorldGridYSize - 1 - (int)y2] = ProbValue;
   				y2++;
   			}
   			y2 = y - Width/2;
   			x2++;
   		}
   		x += c;
   		y += s;
   	} while ( ( (x1 - x) * c > 0 ) || ( (y1 - y) * s > 0) );
   */
   	int x0 = RobotToBitmapX(StartRobotCoord.x);
   	int y0 = RobotToBitmapY(StartRobotCoord.y);
   	int x1 = RobotToBitmapX(EndRobotCoord.x);
   	int y1 = RobotToBitmapY(EndRobotCoord.y);
   	int x = x0;
   	int y = y0;
   	int OldGridValue;
   //	double theta = atan2(y1 - y0, x1 - x0);
   	bool Vertical = false;
   	double slope = 0;
   	if (x1 == x0) {
   		Vertical = true;
   	} else {
   		slope = ((float)(y1 - y0)) / (x1 - x0);
   	}
   	float intercept = y0 - ((float)x0) * (y0 - y1) / (x0 - x1);
   	int x2,y2;
   	if (Vertical || (abs((int)slope) >= 0.5) ) {
   		if (y0 > y1) {
   			y = y0;
   			y0 = y1;
   			y1 = y;
   		}
   		for (y = y0; y <= y1; y++) {
   			if (Vertical) x = x0;
   			else x = Round((y - intercept) / slope);
   			x2 = x - Width/2;
   			y2 = y - Width/2;
   			for (int i = 0; i < Width; i++) {
   				for (int j = 0; j < Width; j++) {
   					OldGridValue = (*m_RobotWorld)[x2][cWorldGridYSize - 1 - (int)y2];
   					if ( ((OldGridValue >= cMinProb) && (OldGridValue <= cMaxProb)) || ReplaceReservedValues) {
   						SetBitmapValues(y2,x2,ProbabilityToColor(ProbValue));
   						(*m_RobotWorld)[x2][cWorldGridYSize - 1 - (int)y2] = ProbValue;
   					}
   					y2++;
   				}
   				y2 = y - Width/2;
   				x2++;
   			}
   //			SetBitmapValues(y,x,ProbabilityToColor(ProbValue));
   //			(*m_RobotWorld)[x][cWorldGridYSize - 1 - (int)y] = ProbValue;
   		}
   	} else {
   		if (x0 > x1) {
   			x = x0;
   			x0 = x1;
   			x1 = x;
   		}
   		for (x = x0; x <= x1; x++) {
   			y = Round(slope * x + intercept);
   //			SetBitmapValues(y,x,ProbabilityToColor(ProbValue));
   //			(*m_RobotWorld)[x][cWorldGridYSize - 1 - (int)y] = ProbValue;
   			x2 = x - Width/2;
   			y2 = y - Width/2;
   			for (int i = 0; i < Width; i++) {
   				for (int j = 0; j < Width; j++) {
   					OldGridValue = (*m_RobotWorld)[x2][cWorldGridYSize - 1 - (int)y2];
   					if ( ((OldGridValue >= cMinProb) && (OldGridValue <= cMaxProb)) || ReplaceReservedValues ) {
   						SetBitmapValues(y2,x2,ProbabilityToColor(ProbValue));
   						(*m_RobotWorld)[x2][cWorldGridYSize - 1 - (int)y2] = ProbValue;
   					}
   					y2++;
   				}
   				y2 = y - Width/2;
   				x2++;
   			}
   		}
   	}
   }
   
   void CProbabilityGrid::SetGridValue(robRECT RobotCoord, int ProbValue, bool ReplaceReservedValues)
   /*******************************************************************************
   //Created: 12/18/97 S.R.
   //Last Revision: 12/18/97 S.R.
   //Parameters:
   //	StartRobotCoord:	The robot coordinate to set to a value.
   //	EndRobotCoord:		The end robot coordinate to set to a value.
   //	ProbValue:			The value to set.
   //Return Value:
   //	none.
   ********************************************************************************
   //Description:
   //		This function sets the given robot coordinates between StartRobotCoord and
   //	EndRobotCoord in the probability grid to ProbValue.
   //Method:
   //
   *******************************************************************************/
   {
   	int OldGridValue;
   	RobotCoord.Normalize();
   	int x0 = RobotToBitmapX(RobotCoord.left);
   	int y0 = RobotToBitmapY(RobotCoord.bottom);
   	int x1 = RobotToBitmapX(RobotCoord.right);
   	int y1 = RobotToBitmapY(RobotCoord.top);
   	int x;
   	int y;
   	for (x = x0; x <= x1; x++) {
   		for (y = y0; y >= y1; y--) {
   			OldGridValue = int((*m_RobotWorld)[x][cWorldGridYSize - 1 - y]);
   			if ( ((OldGridValue >= cMinProb) && (OldGridValue <= cMaxProb)) || ReplaceReservedValues ) {
   				SetBitmapValues(y,x,ProbabilityToColor(ProbValue));
   				(*m_RobotWorld)[x][cWorldGridYSize - 1 - y] = ProbValue;
   			}
   		}
   	}
   }
   
   int CProbabilityGrid::RobotToBitmapX(double X)
   /*******************************************************************************
   //Created: 12/18/97 S.R.
   //Last Revision: 12/18/97 S.R.
   //Parameters:
   //	X:	The robot coordinate to turn into .
   //Return Value:
   //	none.
   ********************************************************************************
   //Description:
   //
   //Method:
   //
   *******************************************************************************/
   {
   //	return Round(X / cWorldGridPointSize + cWorldGridXSize / 2);
   	return int(X / cWorldGridPointSize + cWorldGridXSize / 2);
   }
   
   int CProbabilityGrid::RobotToBitmapY(double Y)
   /*******************************************************************************
   //Created: 12/18/97 S.R.
   //Last Revision: 12/18/97 S.R.
   //Parameters:
   //	none.
   //Return Value:
   //	none.
   ********************************************************************************
   //Description:
   //
   //Method:
   //
   *******************************************************************************/
   {
   //	return Round(-Y / cWorldGridPointSize + cWorldGridYSize / 2);
   	return int(-Y / cWorldGridPointSize + cWorldGridYSize / 2);
   }
   
   HRESULT CProbabilityGrid::DDSetColorKey(IDirectDrawSurface3 * pdds, COLORREF rgb)
   {
   	return (::DDSetColorKey2(pdds, rgb));
   }
   
   void CProbabilityGrid::DDrawDiagnostic(HRESULT ddrval, bool ShowMessage)
   {
   	::DDrawDiagnostic(ddrval, ShowMessage);
   	fflush(stderr);
   }
   
   IDirectDrawSurface3* CProbabilityGrid::DDrawLoadBitmap(UINT ResourceID)
   {
   	CBitmap tempBmap;
   	if (!tempBmap.LoadBitmap(ResourceID)) {
   		AfxMessageBox("DDrawLoadBitmap Error!! Did not load bitmap");
   		fprintf(stderr, "DDrawLoadBitmap Error!! Did not load bitmap\n");
   		assert(FALSE);
   	}
   	IDirectDrawSurface3* tDDSurface;
   	tDDSurface = ::DDrawLoadBitmap(m_lpDD, &tempBmap);
   	if( tDDSurface == NULL ) {
   		AfxMessageBox("DDraw LoadBitmap Failed");
   		fprintf(stderr, "DDraw LoadBitmap Failed\n");
   		assert(FALSE);
   	}
   	tempBmap.DeleteObject();
   	return tDDSurface;
   }
   
   double CProbabilityGrid::ClientSizeToRobotSize(int ClientSize)
   {
   	double RobotSize = ClientSize;
   	RobotSize /= m_ZoomLevel;
   	RobotSize *= cWorldGridPointSize;
   	return RobotSize;
   }
   
   CDC* CProbabilityGrid::DDrawGetSurfaceDC(LPDIRECTDRAWSURFACE3 Surface)
   {
   	HDC BackBufferHDC;
   	Surface->GetDC(&BackBufferHDC);
   	CDC* pDC = CDC::FromHandle(BackBufferHDC);
   	return pDC;
   }
   
   void CProbabilityGrid::DDrawReleaseSurfaceDC(CDC * pDC, LPDIRECTDRAWSURFACE3 Surface)
   {
   	Surface->ReleaseDC(pDC->m_hDC);
   }
   
   float CProbabilityGrid::GetPercentExploredArea()
   {
   	long UpdatedGridPoints = 0;
   	for (int i = 0; i < cWorldGridYSize; i++) {
   		unsigned char* GridValues = (*m_RobotWorld)[i];
   		for (int j = 0; j < cWorldGridXSize; j++) {
   			if (GridValues[j] != cWorldGridInitialValue) UpdatedGridPoints++;
   		}
   	}
   	return UpdatedGridPoints/10000.0f;
   }
   
   CPixelValue::CPixelValue(int Red, int Green, int Blue)
   {
   	R = Red, G = Green, B = Blue;
   	NumberOfValues = 1;
   	Next = NULL;
   }
   
   CPixelValueList::CPixelValueList(long NumberNodes)
   {
   	PixelValues = new CPixelValue*[NumberNodes];
   	for (long i = 0; i < NumberNodes; i++) PixelValues[i] = NULL;
   	nNodes = NumberNodes;
   	RankedPixelValues = NULL;
   }
   
   CPixelValueList::~CPixelValueList()
   {
   	for (long i = 0; i < nNodes; i++) {
   		CPixelValue* CurrentPos = PixelValues[i];
   		CPixelValue* NextPos;
   		while (CurrentPos != NULL) {
   			NextPos = CurrentPos->Next;
   			delete CurrentPos;
   			CurrentPos = NextPos;
   		}
   	}
   	delete RankedPixelValues;
   }
   
   void CPixelValueList::AddPixelValue(int R, int G, int B) {
   	long HashValue = (R|(G<<8)|(B<<16)) % nNodes;
   	CPixelValue* CurrentPos = PixelValues[HashValue];
   	while (CurrentPos != NULL) {
   		if ( (CurrentPos->R == R) && (CurrentPos->G == G) && (CurrentPos->B == B) ) {
   			CurrentPos->NumberOfValues++;
   			break;
   		} else {
   			CurrentPos = CurrentPos->Next;
   		}
   	}
   	if (CurrentPos == NULL) {
   		CPixelValue* NewValue = new CPixelValue(R,G,B);
   		if (PixelValues[HashValue] == NULL) PixelValues[HashValue] = NewValue;
   		else {
   			CurrentPos = PixelValues[HashValue];
   			while (CurrentPos->Next != NULL) {
   				CurrentPos = CurrentPos->Next;
   			}
   			CurrentPos->Next = NewValue;
   		}
   	}
   }
   
   void CPixelValueList::PrintPixelValues()
   {
	std::ofstream out("Palette Values.txt");
   	for (long i = 0; i < nNodes; i++) {
   		CPixelValue* CurrentPos = PixelValues[i];
   		while (CurrentPos != NULL) {
   			out << std::setw(4) << CurrentPos->R << std::setw(4) << CurrentPos->G << std::setw(4) << CurrentPos->B << std::setw(9) << CurrentPos->NumberOfValues << '\n';
   			CurrentPos = CurrentPos->Next;
   		}
   	}
   }
   
   unsigned long CPixelValueList::NumberOfValues(void)
   {
   /*Get number of pixel values*/
   	unsigned long nPixelValues = 0;
   	for (long i = 0; i < nNodes; i++) {
   		CPixelValue* CurrentPos = PixelValues[i];
   		while (CurrentPos != NULL) {
   			CurrentPos = CurrentPos->Next;
   			nPixelValues++;
   		}
   	}
   	return nPixelValues;
   }
   
   void CPixelValueList::RankPixelValues()
   {
   	long nPixelValues = (long)NumberOfValues();
   	RankedPixelValues = new CPixelValue*[nPixelValues];
   	long j = 0;
   	for (long i = 0; i < nNodes; i++) {
   		CPixelValue* CurrentPos = PixelValues[i];
   		while (CurrentPos != NULL) {
   			RankedPixelValues[j] = CurrentPos;
   			CurrentPos = CurrentPos->Next;
   			j++;
   		}
   	}
   	bool Changed = false;
   	do {
   		Changed = false;
   		for (int i = 0; i < nPixelValues - 1; i++) {
   			if (RankedPixelValues[i]->NumberOfValues < RankedPixelValues[i+1]->NumberOfValues) {
   				CPixelValue* temp = RankedPixelValues[i];
   				RankedPixelValues[i] = RankedPixelValues[i+1];
   				RankedPixelValues[i+1] = temp;
   				Changed = true;
   			}
   		}
   	} while (Changed);
   }
   
   void GetBitmapPalette(char* Path, CPixelValueList* PixelValues)
   {
	std::ofstream out("Palette.txt");
   	HBITMAP hbm = (HBITMAP)LoadImage(NULL, Path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
   	CBitmap* pBitmap = CBitmap::FromHandle(hbm);
   	BITMAP BitmapStruct;
   	pBitmap->GetBitmap(&BitmapStruct);
   	byte* BitmapBits = (byte*)BitmapStruct.bmBits;
   	byte* pPixel;
   	int R = 0,G = 0,B = 0;
   	long Pixel;
   	long i;
   
   	for (i = 0; i < BitmapStruct.bmHeight; i++) {
   		for (long j = 0; j < BitmapStruct.bmWidth; j++) {
   			switch (BitmapStruct.bmBitsPixel) {
   			case 8:
   				pPixel = BitmapBits + i*BitmapStruct.bmWidthBytes + j*BitmapStruct.bmBitsPixel/8;
   				Pixel = *pPixel;
   				out << Pixel << "\n";
   				PixelValues->AddPixelValue(R, G, B);
   				break;
   			case 16:
   				pPixel = BitmapBits + i*BitmapStruct.bmWidthBytes + j*BitmapStruct.bmBitsPixel/8;
   				Pixel = pPixel[0] | (pPixel[1] << 8);
   				B = (Pixel & 0x1F);
   				G = ( (Pixel >> 5) & 0x1F);
   				R = ( (Pixel >> 10) & 0x1F);
   				out << std::setw(4) << R << std::setw(4) << G << std::setw(4) << B << '\n';
   				PixelValues->AddPixelValue(R, G, B);
   				break;
   			case 24:
   				pPixel = BitmapBits + i*BitmapStruct.bmWidthBytes + j*BitmapStruct.bmBitsPixel/8;
   				Pixel = pPixel[0] | (pPixel[1] << 8) | (pPixel[2] << 16);
   				B = (Pixel & 0xFF);
   				G = ( (Pixel >> 8) & 0xFF );
   				R = ( (Pixel >> 16) & 0xFF );
   //				if ( (R != G) || (R != B) || (B != G) ) out << "RGB not const\n";
   				out << std::setw(4) << R << std::setw(4) << G << std::setw(4) << B << '\n';
   				PixelValues->AddPixelValue(R, G, B);
   				break;
   			case 32:
   				pPixel = BitmapBits + i*BitmapStruct.bmWidthBytes + j*BitmapStruct.bmBitsPixel/8;
   				Pixel = pPixel[0] | (pPixel[1] << 8) | (pPixel[2] << 16);
   				B = (Pixel & 0xFF);
   				G = ( (Pixel >> 8) & 0xFF );
   				R = ( (Pixel >> 16) & 0xFF );
   				out << std::setw(4) << R << std::setw(4) << G << std::setw(4) << B << '\n';
   				PixelValues->AddPixelValue(R, G, B);
   				break;
   			}
   		}
   	}
   	PixelValues->PrintPixelValues();
   	PixelValues->RankPixelValues();
   	DeleteObject(hbm);
   }
   
   void CProbabilityGrid::LoadRobotBitmaps(void)
   {
   	CString RobotBitmapPath;
   	for (int i = 0; i < cMaxRobots; i++) {
   		CRobot* Robot = GetRobot(i);
   		TRobotColor ColorRobot = crNone;
   		if (Robot != NULL) {
   			ColorRobot = Robot->GetColor();
   		/*Load the m_hbmLargeRobot bitmap*/
   			if ( (m_hbmLargeRobot[i] == NULL) || (ColorRobot != m_OldColor[i]) ) {
   				RobotBitmapPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
   				switch (ColorRobot) {
   				case crBlack:
   					RobotBitmapPath += "\\Robot Bitmaps\\011 large.bmp";
   					break;
   				case crOrange:
   					RobotBitmapPath += "\\Robot Bitmaps\\200 large.bmp";
   					break;
   				case crYellow:
   					RobotBitmapPath += "\\Robot Bitmaps\\300 large.bmp";
   					break;
   				case crChrome:
   					RobotBitmapPath += "\\Robot Bitmaps\\large_chrome.bmp";
   					break;
   				default:
   					ASSERT(FALSE);
   					RobotBitmapPath += "\\Robot Bitmaps\\011 large.bmp";
   					break;
   				}
   				if (m_hbmLargeRobot[i] != NULL) {
   					DeleteObject(m_hbmLargeRobot[i]);
   				}
   				m_hbmLargeRobot[i] = (HBITMAP)LoadImage(NULL, RobotBitmapPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
   			}
   
   		/*Initialize the robot bitmap*/
   			if (m_lpDDSRobotBmp[i] == NULL) {
   				m_OldZoomLevel = cZoomLevel3;
   				m_OldColor[i] = crBlack;
   				RobotBitmapPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath + "\\Robot Bitmaps\\011 large.bmp";
   				m_lpDDSRobotBmp[i] = ::DDrawLoadBitmap(m_lpDD, RobotBitmapPath, 0, 0);
   				if( m_lpDDSRobotBmp[i] == NULL ) {
   					AfxMessageBox("DDraw LoadBitmap Failed");
   					assert(FALSE);
   				}
   			}
   		/*Initialize the inactive robot bitmap*/
   			if (m_lpDDSInactiveRobotBmp == NULL) {
   				RobotBitmapPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath + "\\Robot Bitmaps\\Inactive Large.bmp";
   				m_lpDDSInactiveRobotBmp = ::DDrawLoadBitmap(m_lpDD, RobotBitmapPath, 0, 0);
   				if( m_lpDDSInactiveRobotBmp == NULL ) {
   					AfxMessageBox("DDraw LoadBitmap Failed");
   					assert(FALSE);
   				}
   			}
   		/*Load the new inactive robot bitmap if zoom level changes*/
   			if ( m_ZoomLevel != m_OldZoomLevel) {
   				RobotBitmapPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
   				if (m_ZoomLevel == cZoomLevel1) {
   					RobotBitmapPath += "\\Robot Bitmaps\\Inactive Small.bmp";
   				} else if (m_ZoomLevel == cZoomLevel2) {
   					RobotBitmapPath += "\\Robot Bitmaps\\Inactive Medium.bmp";
   				} else if (m_ZoomLevel == cZoomLevel3) {
   					RobotBitmapPath += "\\Robot Bitmaps\\Inactive Large.bmp";
   				} else {
   					ASSERT(FALSE);
   				}
   				if (::DDReLoadBitmap2(m_lpDDSInactiveRobotBmp, RobotBitmapPath) != DD_OK) {
   					AfxMessageBox("DDraw LoadBitmap Failed");
   					assert(FALSE);
   				}
   			}
   
   			if ( (m_ZoomLevel != m_OldZoomLevel) || (ColorRobot != m_OldColor[i]) ) {
   				if (m_ZoomLevel == cZoomLevel1) {
   					RobotBitmapPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
   					switch (ColorRobot) {
   					case crBlack:
   						RobotBitmapPath += "\\Robot Bitmaps\\011 small.bmp";
   						break;
   					case crOrange:
   						RobotBitmapPath += "\\Robot Bitmaps\\200 small.bmp";
   						break;
   					case crYellow:
   						RobotBitmapPath += "\\Robot Bitmaps\\300 small.bmp";
   						break;
   					case crChrome:
   						RobotBitmapPath += "\\Robot Bitmaps\\small_chrome.bmp";
   						break;
   					default:
   						RobotBitmapPath += "\\Robot Bitmaps\\011 small.bmp";
   						break;
   					}
   
   					if (::DDReLoadBitmap2(m_lpDDSRobotBmp[i], RobotBitmapPath) != DD_OK) {
   						AfxMessageBox("DDraw LoadBitmap Failed");
   						assert(FALSE);
   					}
   				} else if (m_ZoomLevel == cZoomLevel2) {
   					RobotBitmapPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
   					switch (ColorRobot) {
   					case crBlack:
   						RobotBitmapPath += "\\Robot Bitmaps\\011 med.bmp";
   						break;
   					case crOrange:
   						RobotBitmapPath += "\\Robot Bitmaps\\200 med.bmp";
   						break;
   					case crYellow:
   						RobotBitmapPath += "\\Robot Bitmaps\\300 med.bmp";
   						break;
   					case crChrome:
   						RobotBitmapPath += "\\Robot Bitmaps\\med_chrome.bmp";
   						break;
   					default:
   						RobotBitmapPath += "\\Robot Bitmaps\\011 med.bmp";
   						break;
   					}
   
   					if (::DDReLoadBitmap2(m_lpDDSRobotBmp[i], RobotBitmapPath) != DD_OK) {
   						AfxMessageBox("DDraw LoadBitmap Failed");
   						assert(FALSE);
   					}
   				} else if (m_ZoomLevel == cZoomLevel3) {
   					RobotBitmapPath = ((CRobotWorldApp*)AfxGetApp())->m_DefaultPath;
   					switch (ColorRobot) {
   					case crBlack:
   						RobotBitmapPath += "\\Robot Bitmaps\\011 large.bmp";
   						break;
   					case crOrange:
   						RobotBitmapPath += "\\Robot Bitmaps\\200 large.bmp";
   						break;
   					case crYellow:
   						RobotBitmapPath += "\\Robot Bitmaps\\300 large.bmp";
   						break;
   					case crChrome:
   						RobotBitmapPath += "\\Robot Bitmaps\\large_chrome.bmp";
   						break;
   					default:
   						RobotBitmapPath += "\\Robot Bitmaps\\011 large.bmp";
   						break;
   					}
   					if (::DDReLoadBitmap2(m_lpDDSRobotBmp[i], RobotBitmapPath) != DD_OK) {
   						AfxMessageBox("DDraw LoadBitmap Failed");
   						assert(FALSE);
   					}
   				}
   			}
   			m_RobotTransparencyColor = GetSurfaceTopCornerColor(m_lpDDSRobotBmp[i]);
   		} else {
   			if (m_hbmLargeRobot[i] != NULL) {
   				DeleteObject(m_hbmLargeRobot[i]);
   				m_hbmLargeRobot[i] = NULL;
   			}
   			if (m_lpDDSRobotBmp[i] != NULL) {
   				m_lpDDSRobotBmp[i]->Release();
   				m_lpDDSRobotBmp[i] = NULL;
   			}
   			m_OldColor[i] = crNone;
   		}
   		m_OldColor[i] = ColorRobot;
   	}
   	m_OldZoomLevel = m_ZoomLevel;
   }
   
   HBITMAP CProbabilityGrid::GetLargeRobotBitmap(void)
   {
   	return m_hbmLargeRobot[GetActiveRobotIndex()];
   }
   
   void CProbabilityGrid::AddColorsToPalette(CPixelValueList * PixelValues)
   {
   /*Set DirectDraw Palette*/
   	PALETTEENTRY PaletteEntries[256];
   	if (m_lpDDPal->GetEntries(0,0,255, PaletteEntries) != DD_OK) {
   #ifdef _DEBUG
   		AfxMessageBox("AddColorsToPalette GetEntries failed");
   		ASSERT(FALSE);
   #endif
   		return;
   	}
   /*Merge the Pixel Values with the palette entries*/
   	long nPixelValues = long(PixelValues->NumberOfValues());
   	unsigned long AnimatedPaletteEntry = 10 + cSizeProbabilityPalette;
   //	unsigned long AnimatedPaletteEntry = 10;
	std::ofstream out("Palettes Comp.txt");
   	out << nPixelValues << "\n";
   	for (long i = 0; i < nPixelValues; i++) {
   		out << std::setw(4) << PixelValues->RankedPixelValues[i]->R << std::setw(4) << PixelValues->RankedPixelValues[i]->G << std::setw(4) << PixelValues->RankedPixelValues[i]->B << '\n';
   	}
   	out << "\n\n\n";
   	for (long j = 0; j < 256; j++) {
   		out << std::setw(4) << (int)PaletteEntries[j].peRed << std::setw(4) << (int)PaletteEntries[j].peGreen << std::setw(4) << (int)PaletteEntries[j].peBlue << '\n';
   	}
   	out << "\n\n\n";
   	for (long i = 0; i < nPixelValues; i++) {
   		bool InPalette = false;
   		for (long j = 0; (j < 256) && !InPalette; j++) {
   //			if ( (PixelValues->RankedPixelValues[i]->R == PaletteEntries[j].peRed) && 
   //				(PixelValues->RankedPixelValues[i]->G == PaletteEntries[j].peGreen) &&
   //				(PixelValues->RankedPixelValues[i]->B == PaletteEntries[j].peBlue) ) {
   //				InPalette = true;
   //			}
   			if ( (abs(PixelValues->RankedPixelValues[i]->R - PaletteEntries[j].peRed) < 4) && 
   				(abs(PixelValues->RankedPixelValues[i]->G - PaletteEntries[j].peGreen) < 4) &&
   				(abs(PixelValues->RankedPixelValues[i]->B - PaletteEntries[j].peBlue) < 4) ) {
   				InPalette = true;
   			}
   		}
   		if (InPalette) {
   			out << std::setw(4) << PixelValues->RankedPixelValues[i]->R << std::setw(4) << PixelValues->RankedPixelValues[i]->G << std::setw(4) << PixelValues->RankedPixelValues[i]->B << '\n';
   		} else {
   			out << "! " << std::setw(4) << PixelValues->RankedPixelValues[i]->R << std::setw(4) << PixelValues->RankedPixelValues[i]->G << std::setw(4) << PixelValues->RankedPixelValues[i]->B << '\n';
   		}
   		if (!InPalette) {
   			if (AnimatedPaletteEntry < 246) {
   				PaletteEntries[AnimatedPaletteEntry].peRed = PixelValues->RankedPixelValues[i]->R;
   				PaletteEntries[AnimatedPaletteEntry].peGreen = PixelValues->RankedPixelValues[i]->G;
   				PaletteEntries[AnimatedPaletteEntry].peBlue = PixelValues->RankedPixelValues[i]->B;
   			}
   			AnimatedPaletteEntry++;
   		}
   	}
   	if (m_lpDDPal->SetEntries(0,0,255,PaletteEntries) != DD_OK) {
   #ifdef _DEBUG
   		AfxMessageBox("AddColorsToPalette SetEntries failed");
   		ASSERT(FALSE);
   #endif
   		return;
   	}
   
   /*Set Windows Palette*/
   	m_LogicalPalette.SetPaletteEntries(0,236,PaletteEntries + 10);
   	CDC* pDC = m_ViewWindow->GetDC();
   	if (pDC != NULL) pDC->RealizePalette();
   }
   
   float CProbabilityGrid::GetZoomLevel()
   {
   	return m_ZoomLevel;
   }
   
   
   HRESULT CProbabilityGrid::TransparentBlt(LPDIRECTDRAWSURFACE3 BackBuffer, CRect destRect, LPDIRECTDRAWSURFACE3 Bmp, CRect rcRect, COLORREF TransparentColor)
   {
   	HRESULT ddrval;
   	RectifyRectangles(&rcRect, &destRect);
   //	ddrval = DDSetColorKey2(Bmp, CLR_INVALID);
   //	ddrval = DDSetColorKey(Bmp, TransparentColor);
   	
   	DDCOLORKEY ColorKey;
   	ColorKey.dwColorSpaceHighValue = TransparentColor;
   	ColorKey.dwColorSpaceLowValue = TransparentColor;
   	ddrval = Bmp->SetColorKey(DDCKEY_SRCBLT, &ColorKey);
   
   	if (ddrval != DD_OK) {
   		DDrawDiagnostic(ddrval);
   	}
   	if ( (destRect.Height() != 0) && (destRect.Width() != 0) && (rcRect.Height() != 0) && (rcRect.Width() != 0) ) {
   		while (BackBuffer->GetBltStatus(DDGBS_CANBLT) != DD_OK);
   		ddrval = BackBuffer->BltFast(destRect.left, destRect.top, Bmp, rcRect, DDBLTFAST_SRCCOLORKEY);
   		if (ddrval != DD_OK) {
   			DDrawDiagnostic(ddrval);
   			fprintf(stderr, "***CProbabilityGrid::TransparentBlt Error***\n");
   			fprintf(stderr, "destRect = %d %d %d %d\n",destRect.left, destRect.top, destRect.right, destRect.bottom);
   			fprintf(stderr, "rcRect = %d %d %d %d\n", rcRect.left,rcRect.top, rcRect.right, rcRect.bottom);
   			fflush(stderr);
   		}
   	}
   	return ddrval;
   }
   
   HRESULT CProbabilityGrid::TransparentBlt(LPDIRECTDRAWSURFACE3 BackBuffer, CRect destRect, LPDIRECTDRAWSURFACE3 Bmp, CRect rcRect, DDCOLORKEY ColorKey)
   {
   	HRESULT ddrval;
   	RectifyRectangles(&rcRect, &destRect);
   	
   	ddrval = Bmp->SetColorKey(DDCKEY_SRCBLT, &ColorKey);
   	if (ddrval != DD_OK) {
   		DDrawDiagnostic(ddrval);
   	}
   	if ( (destRect.Height() != 0) && (destRect.Width() != 0) && (rcRect.Height() != 0) && (rcRect.Width() != 0) ) {
   		while (BackBuffer->GetBltStatus(DDGBS_CANBLT) != DD_OK);
   		ddrval = BackBuffer->BltFast(destRect.left, destRect.top, Bmp, rcRect, DDBLTFAST_SRCCOLORKEY);
   		if (ddrval != DD_OK) {
   			DDrawDiagnostic(ddrval);
   			fprintf(stderr, "***CProbabilityGrid::TransparentBlt Error***\n");
   			fprintf(stderr, "destRect = %d %d %d %d\n",destRect.left, destRect.top, destRect.right, destRect.bottom);
   			fprintf(stderr, "rcRect = %d %d %d %d\n", rcRect.left,rcRect.top, rcRect.right, rcRect.bottom);
   			fflush(stderr);
   		}
   	}
   	return ddrval;
   }
   
   HRESULT CProbabilityGrid::Blt(LPDIRECTDRAWSURFACE3 BackBuffer, CRect destRect, LPDIRECTDRAWSURFACE3 Bmp, CRect rcRect)
   {
   	HRESULT ddrval = DD_OK;
   	RectifyRectangles(&rcRect, &destRect);
   	if ( (destRect.Height() != 0) && (destRect.Width() != 0) && (rcRect.Height() != 0) && (rcRect.Width() != 0) ) {
   		while (BackBuffer->GetBltStatus(DDGBS_CANBLT) != DD_OK);
   		ddrval = BackBuffer->BltFast(destRect.left, destRect.top, Bmp, rcRect, DDBLTFAST_NOCOLORKEY);
   		if (ddrval != DD_OK) {
   			DDrawDiagnostic(ddrval);
   			fprintf(stderr, "***CProbabilityGrid::Blt Error***\n");
   			fprintf(stderr, "destRect = %d %d %d %d\n",destRect.left, destRect.top, destRect.right, destRect.bottom);
   			fprintf(stderr, "rcRect = %d %d %d %d\n", rcRect.left,rcRect.top, rcRect.right, rcRect.bottom);
   			fflush(stderr);
   		}
   	}
   	return ddrval;
   }
   
   DWORD CProbabilityGrid::GetSurfaceTopCornerColor(LPDIRECTDRAWSURFACE3 pdds)
   {
   	HRESULT hres;
   	DDSURFACEDESC ddsd;
   	DWORD dw = -1;
   	memset(&ddsd, 0, sizeof(ddsd));
   	ddsd.dwSize = sizeof(ddsd);
   	while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING);
   
   	if (hres == DD_OK) {
   		dw  = *(DWORD *)ddsd.lpSurface;                     // get DWORD
   		if (ddsd.ddpfPixelFormat.dwRGBBitCount != 32) {
   			dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;  // mask it to bpp
   		}
   		pdds->Unlock(NULL);
   	}
   	return dw;
   }
   
   CRobot* CProbabilityGrid::GetRobot(short Index)
   {
   	ASSERT(Index < cMaxRobots);
   	if (Index >= cMaxRobots) Index = cMaxRobots - 1;
   	ASSERT(Index >= 0);
   	if (Index < 0) Index = 0;
   	return m_RobotList[Index];
   }
   
   short CProbabilityGrid::GetNumberOfRobots()
   {
    short i;
   	for (i = 0; (i < cMaxRobots) && (m_RobotList[i] != NULL); i++);
   	return i;
   }
   
   void CProbabilityGrid::AnimateRobots(void)
   {
   	if (GetRobotAnimate()) {
   		for (int i = 0; m_RobotList[i] != NULL; i++) {
   				m_RobotList[i]->Animate();
   		}
   	}
   }
   
   bool CProbabilityGrid::AddRobot(CRobot *Robot)
   {
    int i;
   	bool Added = false;
   	for (i = 0; (i < cMaxRobots) && (m_RobotList[i] != NULL); i++);
   	ASSERT(i < cMaxRobots);
   	if (i < cMaxRobots) {
   		m_RobotList[i] = Robot;
   		if (i == 0) {
   			m_ActiveRobot = Robot;	//Adding robot to an empty list, set active robot
   		}
   		Added = true;
   	}
   	return Added;
   }
   
   bool CProbabilityGrid::DeleteRobot(CRobot *Robot)
   {
   	bool Deleted = false;
   	if (GetNumberOfRobots() > 1) {
   		for (int i = 0; i < cMaxRobots; i++) {
   			if (m_RobotList[i] == Robot) {
   				Deleted = true;
   				delete Robot;
   				for ( ; i < cMaxRobots - 1; i++) m_RobotList[i] = m_RobotList[i+1];
   			}
   		}
   	}
   	return Deleted;
   }
   
   void CProbabilityGrid::PurgeRobots()
   {
   	for (int i = 0; i < cMaxRobots; i++) {
   		delete (m_RobotList[i]);
   		m_RobotList[i] = NULL;
   	}
   	m_ActiveRobot = NULL;
   }
   
   CRobot* CProbabilityGrid::PointInsideRobot(robPOINT Position)
   {
   	CRobot* Robot = NULL;
   	for (int i = 0; i < cMaxRobots; i++) {
   		if (m_RobotList[i] != NULL) {
   			bool InsideRobot = false;
   			const unsigned char cMinClickDist = 5;
   			if ( (ABS(Position.x - m_RobotList[i]->m_Position.x) < cRRWidth) && (ABS(Position.y - m_RobotList[i]->m_Position.y) < cRRWidth) ) {
   				InsideRobot = true;
   			}
   			POINT RobotScreenPoint = RobotToClientCoords(m_RobotList[i]->m_Position);
   			POINT RobotScreenClick = RobotToClientCoords(Position);
   			if ( (ABS(RobotScreenPoint.x - RobotScreenClick.x) < cMinClickDist) && (ABS(RobotScreenPoint.y - RobotScreenClick.y) < cMinClickDist) ) {
   				InsideRobot = true;
   			}
   			if (InsideRobot) Robot = m_RobotList[i];
   		}
   	}
   	return Robot;
   }
   
   short CProbabilityGrid::GetActiveRobotIndex()
   {
   	CRobot* ActiveRobot = GetActiveRobot();
   	short ActiveIndex = -1;
   	for (int i = 0; i < cMaxRobots; i++) {
   		if (m_RobotList[i] == m_ActiveRobot) {
   			ActiveIndex = i;
   		}
   	}
   	ASSERT(ActiveIndex >= 0);
   	if (ActiveIndex < 0) ActiveIndex = 0;
   	return ActiveIndex;
   }
   
   void CProbabilityGrid::SetRobotsMaxSpeed(short Speed)
   {
   	for (int i = 0; i < cMaxRobots; i++) {
   		if (m_RobotList[i] != NULL) {
   			m_RobotList[i]->SetMaxSpeed(Speed);
   		}
   	}
   }
   
   void CProbabilityGrid::SetRobotsFlipHandle(bool FlipHandle)
   {
   	for (int i = 0; i < cMaxRobots; i++) {
   		if (m_RobotList[i] != NULL) {
   			m_RobotList[i]->SetFlipHandle(FlipHandle);
   		}
   	}
   }
   
   void CProbabilityGrid::SetRobotsUpdateProbabilityGrid(bool Update)
   {
   	for (int i = 0; i < cMaxRobots; i++) {
   		if (m_RobotList[i] != NULL) {
   			m_RobotList[i]->SetLearnRoom(Update);
   		}
   	}
   }
   
   bool CProbabilityGrid::GetRobotAnimate()
   {
   	return m_RobotAnimate;
   }
   
   void CProbabilityGrid::SetRobotAnimate(bool Enable)
   {
   	((CRobotWorldApp*)AfxGetApp())->SetRobotAnimate(Enable);
   	m_RobotAnimate = Enable;
   }
   
   CHomeBase* CProbabilityGrid::GetClosestHomeBase(robPOINT Position)
   {
   	return m_ExecutableObjectList->GetClosestHomeBase(Position);
   }
   
   bool PositionOccupied(robPOINT *RobotPositions, short NumberOfRobots, robPOINT Position)
   /*This function returns true if there is a robot position close to Position*/
   {
   	const double cOccupiedDistance = 120.0; //If a robot is within 4" of Position, it is occupied
   	bool Occupied = false;
   	for (int i = 0; (i < NumberOfRobots) && (!Occupied); i++) {
   		if (Distance(RobotPositions[i], Position) < cOccupiedDistance) {
   			Occupied = true;
   		}
   	}
   	return Occupied;
   }
   
   CHomeBase* CProbabilityGrid::GetClosestUnoccupiedHomeBase(robPOINT Position)
   /*This function finds the home base that is closest to Position that doesn't have
   a robot sitting on top of it.*/
   {
   /*Get the positions of all the robots*/
   	robPOINT RobotPositions[cMaxRobots];
   	short NumberOfRobots = GetNumberOfRobots();
   	for (int i = 0; i < NumberOfRobots; i++) {
   		RobotPositions[i] = m_RobotList[i]->GetPosition();
   	}
   /*You need to get rid of the one robot position that represents Position.  Otherwise
   if you create a new robot, and it's on the primary home base then this function will
   think the new robot is occupying the primary home base and not return it as the
   closest one.*/
   	for (int i = 0; i < NumberOfRobots; i++) {
   		if (RobotPositions[i] == Position) {
   			for (; i< NumberOfRobots - 1; i++) {
   				RobotPositions[i] = RobotPositions[i+1];
   			}
   			NumberOfRobots--;
   		}
   	}
   
   	CRuntimeClass* HomebaseType = RUNTIME_CLASS(CHomeBase);
   	double MinDist = 0.0;
   	CExecutableRWGraphicObjectListNode* MinDistNode = NULL;
   	CExecutableRWGraphicObjectListNode* CurrentNode = m_ExecutableObjectList->GetHead();
   /*Initialize MinDist and MinDistNode*/
   	while ( (CurrentNode != NULL) && (MinDistNode == NULL) ) {
   		if (CurrentNode->m_Object->IsKindOf(HomebaseType)) {
   			robPOINT DockedPosition = ((CHomeBase*)(CurrentNode->m_Object))->GetDockedPosition();
   			if ( !PositionOccupied(RobotPositions, NumberOfRobots, DockedPosition) ) {
   				MinDist = Distance(DockedPosition, Position);
   				MinDistNode = CurrentNode;
   			}
   		}
   		CurrentNode = CurrentNode->m_Next;
   	}
   	while (CurrentNode != NULL) {
   		if (CurrentNode->m_Object->IsKindOf(HomebaseType)) {
   			robPOINT DockedPosition = ((CHomeBase*)(CurrentNode->m_Object))->GetDockedPosition();
   			if ( !PositionOccupied(RobotPositions, NumberOfRobots, DockedPosition) ) {
   				if ( Distance(DockedPosition, Position) < MinDist ) {
   					MinDist = Distance(CurrentNode->m_Object->GetEntryPoint(), Position);
   					MinDistNode = CurrentNode;
   				}
   			}
   		}
   		CurrentNode = CurrentNode->m_Next;
   	}
   	if (MinDistNode != NULL) {
   		return (CHomeBase*)(MinDistNode->m_Object);
   	} else {
   		return NULL;
   	}
   }
   
   void CProbabilityGrid::SetRobotsEnableDancing(bool Enable)
   {
   	for (int i = 0; i < cMaxRobots; i++) {
   		if (m_RobotList[i] != NULL) {
   			m_RobotList[i]->SetEnableDancing(Enable);
   		}
   	}
   }
