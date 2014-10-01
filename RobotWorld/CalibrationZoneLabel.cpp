// CalibrationZoneLabel.cpp: implementation of the CCalibrationZoneLabel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "global.h"
#include "CalibrationZoneLabel.h"
#include "Calibration Zone.h"
#include "Robot.h"
#include "WorldGridConsts.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CCalibrationZoneLabel, CLabel, VERSION_NUMBER)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalibrationZoneLabel::CCalibrationZoneLabel() : CLabel()
{
	m_CalibrationZone = NULL;
	m_LabelType = ltCalibrationZone;
}

CCalibrationZoneLabel::~CCalibrationZoneLabel()
{

}

CCalibrationZoneLabel::CCalibrationZoneLabel(CString Caption, robPOINT WorldPoint, CView* pParentWnd, CLabelList* LabelList, CCalibrationZone* CalZone) : CLabel(WorldPoint, pParentWnd, LabelList)
{
	SetCaption(Caption);
	m_LabelType = ltCalibrationZone;
//	m_LabelList->GetUniqueCaption(&m_Caption, this);
	m_CalibrationZone = CalZone;
}

void CCalibrationZoneLabel::Serialize(CArchive & archive)
{
	CLabel::Serialize(archive);
	if (archive.IsStoring()) {
		archive << m_CalibrationZone;
	} else {
		archive >> m_CalibrationZone;
	}
}

robPOINT CCalibrationZoneLabel::GetDestination(void)
{
	return m_CalibrationZone->GetEntryPoint();
}

TTaskResult CCalibrationZoneLabel::SpecialFunction(CRobot * Robot)
{
//	return Robot->CalibratePosition(m_CalibrationZone);
	return TR_TaskComplete;
}
