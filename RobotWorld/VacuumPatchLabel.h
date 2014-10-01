// VacuumPatchLabel.h: interface for the CVacuumPatchLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VACUUMPATCHLABEL_H__57636F61_9184_11D1_B231_0040F6B87672__INCLUDED_)
#define AFX_VACUUMPATCHLABEL_H__57636F61_9184_11D1_B231_0040F6B87672__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RothMath.h"
//#include "Label.h"

class CRobot;
class CVacuumPatch;
class Path;

class CVacuumPatchLabel : public CLabel
{
public:
	void Serialize(CArchive & archive);
	TTaskResult SpecialFunction(CRobot* Robot);
	robPOINT GetDestination();
	CVacuumPatchLabel(CString Caption, robPOINT WorldPoint, CView* pParentWnd, CLabelList* LabelList, CVacuumPatch* VacuumPatch);
	CVacuumPatchLabel();
	virtual ~CVacuumPatchLabel();

private:
	DECLARE_SERIAL(CVacuumPatchLabel)
	CVacuumPatch* m_VacuumPatch;
};

#endif // !defined(AFX_VACUUMPATCHLABEL_H__57636F61_9184_11D1_B231_0040F6B87672__INCLUDED_)
