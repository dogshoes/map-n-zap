// DlgPositionError.cpp : implementation file
//

#include "stdafx.h"
#include <stdio.h>
#include "RobotWorld.h"
#include "RothMath.h"
#include "DlgPositionError.h"
#include "WorldGridConsts.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPositionError dialog


CDlgPositionError::CDlgPositionError(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPositionError::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPositionError)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPositionError::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPositionError)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPositionError, CDialog)
	//{{AFX_MSG_MAP(CDlgPositionError)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPositionError message handlers

void CDlgPositionError::OnPaint() 
{
const unsigned long cBmpFrameWidth = 38;
const unsigned long cBmpFrameHeight = 38;
const unsigned long cBmpFramePerLine = 5;
const unsigned long cTotalFrames = 40;

	CPaintDC dc(this); // device context for painting

	if (m_dcDisplayMemory.GetSafeHdc() == NULL) {
		if (!m_dcDisplayMemory.CreateCompatibleDC(&dc)) {
			AfxMessageBox("CreateCompatibleDC failed");
		}
		CBitmap Bitmap;
//		Bitmap.LoadBitmap(IDB_RED);
		CBitmap* temp = m_dcDisplayMemory.SelectObject(m_pRobotBitmap);
		if (temp == NULL) {
			AfxMessageBox("SelectObject Error");
		}
	}

	CRect rcRect;
	int Frame;
	if (m_Heading <= 3*pi/2) {
		Frame = -20/pi * m_Heading + 30;
	} else {
		const float X1 = 3*pi/2 + 2*pi/cTotalFrames;
		const float X2 = 2*pi - 2*pi/cTotalFrames;
		const float m = 8 / (X1 - X2);
		Frame = m*m_Heading - m*X1 + 39;
	}
	if (Frame > cTotalFrames - 1) Frame = 0;
	rcRect.left   = Frame%cBmpFramePerLine*cBmpFrameWidth;
	rcRect.top    = Frame/cBmpFramePerLine*cBmpFrameHeight;
	rcRect.right  = rcRect.left + cBmpFrameWidth;
	rcRect.bottom = rcRect.top + cBmpFrameHeight;
	CRect ClientRect;
	GetClientRect(&ClientRect);
	COLORREF White = 0xFFFFFF;
	dc.SetBkMode(OPAQUE);
	dc.SetBkColor(White);
	dc.FillSolidRect(ClientRect, White);

/*Blit Robot*/
	ClientRect.left = ClientRect.Width()/2 - cBmpFrameWidth / 2;
	ClientRect.top = ClientRect.Height()/2 - cBmpFrameHeight / 2;
	dc.BitBlt( ClientRect.left, ClientRect.top, rcRect.Width(), rcRect.Height(), &m_dcDisplayMemory, rcRect.left, rcRect.top, SRCCOPY );
/*Sketch Error Octagon*/	
	SketchError(&dc);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgPositionError::Create(UINT idRobotBitmap, UINT idDialogID, CView* pView, CRobot* Robot)
{
	CDialog::Create(idDialogID, pView);
	m_pRobotBitmap->LoadBitmap(idRobotBitmap);
	m_pView = pView;
	m_pRobot = Robot;
}

void CDlgPositionError::Update(robPOINT dPosition, double Heading, double dHeading)
{
	m_DPosition = dPosition;
	m_Heading = Heading;
	m_DHeading = dHeading;
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

int CDlgPositionError::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pRobotBitmap = new CBitmap;
	m_Heading = 0;
	m_DHeading = 0;
	m_DPosition.x = 0;
	m_DPosition.y = 0;
	
	return 0;
}

void CDlgPositionError::OnDestroy() 
{
	CDialog::OnDestroy();
	delete m_pRobotBitmap;	
}

void CDlgPositionError::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

void CDlgPositionError::SketchError(CDC* dc)
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
	robPOINT* Corners = m_pRobot->GetCorners();
	robPOINT RobotPosition = m_pRobot->m_Position;
	for (i = 0; i < 4; i++) {
		Corners[i].x -= RobotPosition.x;
		Corners[i].y -= RobotPosition.y;
	}

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
				PointArray[j].x = Corners[i].x + m_DPosition.x;
				PointArray[j++].y = Corners[i].y + m_DPosition.y;
				PointArray[j].x = Corners[i].x - m_DPosition.x;
				PointArray[j++].y = Corners[i].y + m_DPosition.y;
			} else if (Corners[i].y == MinY) {
				PointArray[j].x = Corners[i].x - m_DPosition.x;
				PointArray[j++].y = Corners[i].y - m_DPosition.y;
				PointArray[j].x = Corners[i].x + m_DPosition.x;
				PointArray[j++].y = Corners[i].y - m_DPosition.y;
			} else {
				PointArray[j].x = Corners[i].x + m_DPosition.x;
				PointArray[j++].y = Corners[i].y - m_DPosition.y;
				PointArray[j].x = Corners[i].x + m_DPosition.x;
				PointArray[j++].y = Corners[i].y + m_DPosition.y;
			}
		} else if (Corners[i].x == MinX) {
			if (Corners[i].y == MaxY) {
				PointArray[j].x = Corners[i].x - m_DPosition.x;
				PointArray[j++].y = Corners[i].y + m_DPosition.y;
				PointArray[j].x = Corners[i].x - m_DPosition.x;
				PointArray[j++].y = Corners[i].y - m_DPosition.y;
			} else if (Corners[i].y == MinY) {
				PointArray[j].x = Corners[i].x - m_DPosition.x;
				PointArray[j++].y = Corners[i].y - m_DPosition.y;
				PointArray[j].x = Corners[i].x + m_DPosition.x;
				PointArray[j++].y = Corners[i].y - m_DPosition.y;
			} else {
				PointArray[j].x = Corners[i].x - m_DPosition.x;
				PointArray[j++].y = Corners[i].y + m_DPosition.y;
				PointArray[j].x = Corners[i].x - m_DPosition.x;
				PointArray[j++].y = Corners[i].y - m_DPosition.y;
			}
		} else {
			if (Corners[i].y == MaxY) {
				PointArray[j].x = Corners[i].x + m_DPosition.x;
				PointArray[j++].y = Corners[i].y + m_DPosition.y;
				PointArray[j].x = Corners[i].x - m_DPosition.x;
				PointArray[j++].y = Corners[i].y + m_DPosition.y;
			} else if (Corners[i].y == MinY) {
				PointArray[j].x = Corners[i].x - m_DPosition.x;
				PointArray[j++].y = Corners[i].y - m_DPosition.y;
				PointArray[j].x = Corners[i].x + m_DPosition.x;
				PointArray[j++].y = Corners[i].y - m_DPosition.y;
			}
		}
	}
	if (j != 8) {
		AfxMessageBox("Error Octagon Error!!",MB_OK,0); 
#ifdef _DEBUG
		ASSERT(FALSE);
#endif
	}
	CRect ClientRect;
	GetClientRect(&ClientRect);
	ClientRect.left = ClientRect.Width()/2;
	ClientRect.top = ClientRect.Height()/2;
//	POINT WindowPoint[8] = {0,0,100,0,200,0,200,100,100,100,100,200,0,200,0,0};
	POINT WindowPoint[8];
	for (j = 0; j < 8; j++) {
		PointArray[j].x *= 4.0 / cWorldGridPointSize;
		WindowPoint[j].x = Round(PointArray[j].x + ClientRect.left);
		PointArray[j].y *= 4.0 / cWorldGridPointSize;
		WindowPoint[j].y = Round(ClientRect.top - PointArray[j].y);
	}
	CGdiObject* OldObject = dc->SelectStockObject(BLACK_PEN);
//	dc->Polygon(WindowPoint, 8);
	dc->Polyline(WindowPoint, 8);
	dc->SelectObject(OldObject);

	return;
}
