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
#ifndef WORLDGRIDCONSTS29380982340982397502092756

#define WORLDGRIDCONSTS29380982340982397502092756

#include "rrdim.h"

const int cWorldGridXSize = 1000;
const int cWorldGridYSize = 1000;
const double cWorldGridPointSize = 2.0 * cTPI;
//const double cRobotLength = 7.0;
//const double cRobotWidth = 15.5;
const unsigned char cWorldGridInitialValue = 127;
const unsigned char cMinProb = 3;		/*Definite Obstacle*/
const unsigned char cMaxProb = 252;		/*Definite Clear*/
const unsigned char cRedZoneProb = 2;	/*Values for Red Zones*/
const int cProbArraySize = 16;
const int cGoodPathMinFoundValue = 127;		/*value of MinFound member of path's quality for an obstacle free path*/
const int cVeryClearPathMinValue = 200;
const unsigned int cFindPathIterationsDefault = 100;
const unsigned int cFindPathRepeatDefault = 3;
const double cDefaultPathBuffer = 0.0;
#endif