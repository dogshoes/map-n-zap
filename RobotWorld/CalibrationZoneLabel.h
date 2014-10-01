// CalibrationZoneLabel.h: interface for the CCalibrationZoneLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALIBRATIONZONELABEL_H__0D769B62_8E5B_11D1_B231_0040F6B87672__INCLUDED_)
#define AFX_CALIBRATIONZONELABEL_H__0D769B62_8E5B_11D1_B231_0040F6B87672__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RothMath.h"
#include "Label.h"

class CRobot;
class CCalibrationZone;
class CLabelList;
class Path;

class CCalibrationZoneLabel : public CLabel  
{
public:
	TTaskResult SpecialFunction(CRobot* Robot);
	robPOINT GetDestination(void);
	void Serialize(CArchive& archive);
	CCalibrationZoneLabel(CString Caption, robPOINT WorldPoint, CView* pParentWnd, CLabelList* LabelList, CCalibrationZone* CalZone);
	CCalibrationZoneLabel();
	virtual ~CCalibrationZoneLabel();

private:
	DECLARE_SERIAL(CCalibrationZoneLabel)
	CCalibrationZone* m_CalibrationZone;
};

#endif // !defined(AFX_CALIBRATIONZONELABEL_H__0D769B62_8E5B_11D1_B231_0040F6B87672__INCLUDED_)
