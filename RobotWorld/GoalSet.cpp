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
#include "RothMath.h"
#include "FindPathThread.h"
#include "GoalSet.h"
#include "Path.h"

CGoalSet::CGoalSet()
{
    memset(m_GoalSet, 0, sizeof(m_GoalSet));
}

CGoalSet::~CGoalSet()
{
    for (int cntr = 0; (cntr < cMaxGoalSets) && (m_GoalSet[cntr].thePath != NULL); cntr++)
    {
        //		delete (m_GoalSet[cntr].thePath);
    }
}

goalSet& CGoalSet::operator[](int Position)
{
    ASSERT(Position < cMaxGoalSets);
    return m_GoalSet[Position];
}

CGoalSet::CGoalSet(goalSet *goalList)
{
    int cntr;

    memset(m_GoalSet, 0, sizeof(m_GoalSet));

    for (cntr = 0; (cntr < cMaxGoalSets) && (goalList[cntr].thePath != NULL); cntr++)
    {
        m_GoalSet[cntr].calZone = goalList[cntr].calZone;
        m_GoalSet[cntr].thePath = goalList[cntr].thePath;
        m_GoalSet[cntr].Cached = goalList[cntr].Cached;
    }

    ASSERT(cntr <= cMaxGoalSets);
}
