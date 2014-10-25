// VacuumPatchLabel.cpp: implementation of the CVacuumPatchLabel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RobotWorld.h"
//#include "VacuumPatchLabel.h"
#include "global.h"
#include "Robot.h"
#include "Path.h"
#include "VacuumPatch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CVacuumPatchLabel, CLabel, VERSION_NUMBER)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVacuumPatchLabel::CVacuumPatchLabel()
{
    m_VacuumPatch = NULL;
    m_LabelType = ltVacuumPatch;
}

CVacuumPatchLabel::~CVacuumPatchLabel()
{

}

CVacuumPatchLabel::CVacuumPatchLabel(CString Caption, robPOINT WorldPoint, CView * pParentWnd, CLabelList * LabelList, CVacuumPatch * VacuumPatch) : CLabel(WorldPoint, pParentWnd, LabelList)
{
    SetCaption(Caption);
    m_LabelType = ltVacuumPatch;
    m_VacuumPatch = VacuumPatch;
}

robPOINT CVacuumPatchLabel::GetDestination()
{
    return m_VacuumPatch->GetStartPoint();
}

TTaskResult CVacuumPatchLabel::SpecialFunction(CRobot * Robot)
{
    return m_VacuumPatch->Execute(Robot);
}

void CVacuumPatchLabel::Serialize(CArchive & archive)
{
    CLabel::Serialize(archive);

    if (archive.IsStoring())
    {
        archive << m_VacuumPatch;
    }
    else
    {
        archive >> m_VacuumPatch;
    }
}
