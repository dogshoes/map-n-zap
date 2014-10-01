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

#include <math.h>
#include "Grid.h"

unsigned char Grid::GridValue(const double x, const double y)
{
    int row, col;

    WorldToGrid(x, y, row, col);
    return (*this)[row][col];
}

void Grid::SetOrigin(double x, double y) 
{
    origin[0] = x;
    origin[1] = y;
    GridToWorld(0, 0, xmin, ymin);
    GridToWorld(NumberOfRows()-1, NumberOfColumns()-1, xmax, ymax);

    // Adjust to edge of cell since above gives midpoint
    xmin -= size/2.0;
    ymin -= size/2.0;
    xmax += size/2.0;
    ymax += size/2.0;

    corner_ang[0] = atan2(ymax, xmax);
    corner_ang[1] = atan2(ymax, xmin);
    corner_ang[2] = atan2(ymin, xmin);
    corner_ang[3] = atan2(ymin, xmax);

    corner_dist[0] = sqrt(ymax*ymax + xmax*xmax);
    corner_dist[1] = sqrt(ymax*ymax + xmin*xmin);
    corner_dist[2] = sqrt(ymin*ymin + xmin*xmin);
    corner_dist[3] = sqrt(ymin*ymin + xmax*xmax);
}



ostream& operator << (ostream &os, const Grid &g) {
    os << g.size << endl;
    os << g.origin[0] << " " << g.origin[1] << endl;
    os << (CharMatrix &)g;

    return os;
}


istream& operator >> (istream &is, Grid &g) {
    int origin_x, origin_y;

    is >> g.size;
    is >> origin_x >> origin_y;
    is >> (CharMatrix &)g;
    g.SetOrigin(origin_x, origin_y);

    return is;
}

