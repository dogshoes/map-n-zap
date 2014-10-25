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

#include "World.h"


void World::ClearRoverPosition(double x, double y, double th, double dx,
                               double dy, double dth)
{
    Grid *clear;
    int row, col;
    int t_rmin, t_cmin, t_rmax, t_cmax;

    rover.SetLocation(x, y, th);
    rover.SetLocationDelta(dx, dy, dth);
    clear = rover.ClearGrid(GridSize());

    //cerr << *clear;

    // Get the origin point of the clear grid in world grid coordinates
    WorldToGrid(x, y, row, col);

    // Find corner of grid (both have same grid size)
    row -= (int)clear->GetOriginX();
    col -= (int)clear->GetOriginY();

    if (!Overlap(*clear, row, col, t_rmin, t_cmin, t_rmax, t_cmax))
    {
        return;
    }

    Blend(row, col, *clear);
    delete clear;
    CharMatrix *changed = new CharMatrix(*this, t_rmin, t_cmin,
                                         t_rmax - t_rmin + 1, t_cmax - t_cmin + 1);

    //cerr << *changed;

    if (ChangeScreen != NULL)
    {
        (*ChangeScreen)(t_rmin, t_cmin, *changed, *this);
    }

    delete changed;
}




void World::ObstacleRoverPosition(double x, double y, double th, double dx,
                                  double dy, double dth, RoverGrid ind)
{
    Grid *clear;
    int row, col;
    int t_rmin, t_cmin, t_rmax, t_cmax;

    rover.SetLocation(x, y, th);
    rover.SetLocationDelta(dx, dy, dth);
    clear = rover.ObstacleGrid(GridSize(), ind);

    //cerr << *clear;

    // Get the origin point of the clear grid in world grid coordinates
    WorldToGrid(x, y, row, col);

    // Find corner of grid (both have same grid size)
    row -= (int)clear->GetOriginX();
    col -= (int)clear->GetOriginY();

    if (!Overlap(*clear, row, col, t_rmin, t_cmin, t_rmax, t_cmax))
    {
        return;
    }

    Blend(row, col, *clear);
    std::cerr << "Hit " << row << " " << col << std::endl;
    delete clear;

    CharMatrix *changed = new CharMatrix(*this, t_rmin, t_cmin,
                                         t_rmax - t_rmin + 1, t_cmax - t_cmin + 1);

    //cerr << *changed;

    if (ChangeScreen != NULL)
    {
        (*ChangeScreen)(t_rmin, t_cmin, *changed, *this);
    }

    delete changed;
}

