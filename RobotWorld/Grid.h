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
 /*
 ** Copyright (C) 1997
 **
 **             James Hemmerle
 **		PHT, Inc.
 **
 ** All Rights Reserved
 **
 **
 ** File: Grid.h - header for the grid class to be used for Henry
 **
 **
 */
 
 
 #ifndef GRID_H
 #define GRID_H
 
 #include "CharMatrix.h"
 
 class Grid : public CharMatrix {
   private:
     double size;		// Size of grid square
     double origin[2];		// Grid location of the origin of model
     double xmin, xmax;		// x range, y range
     double ymin, ymax;
     double corner_ang[4];	// Angles to the corners
     double corner_dist[4];	// Distances to the corners
   public:
     Grid(int row, int col, double grid_size=1, unsigned char init=0,
 	 unsigned char min=0, unsigned char max=255)
 	: CharMatrix(row, col, init, min, max) {
 	    size = grid_size;
 	    SetOrigin(row/2.0, col/2.0);
 	};
     Grid() : CharMatrix() {
 	size = 0; SetOrigin(0.0, 0.0);
     };
     Grid(const Grid &g) : CharMatrix(g) {
 	size = g.size; SetOrigin(g.origin[0], g.origin[1]);};
 
     void SetOrigin(double x, double y);
     void GetOrigin(double &x, double &y) {x = origin[0]; y = origin[1];};
     double GetOriginX(void) {return origin[0];};
     double GetOriginY(void) {return origin[1];};
     double GridSize(void) {return size;};
 
     double XMax() {return xmax;};
     double XMin() {return xmin;};
     double YMax() {return ymax;};
     double YMin() {return ymin;};
 
     double CornerAngle(int i) {return corner_ang[i%4];};
     double CornerAngleNE() {return corner_ang[0];};
     double CornerAngleNW() {return corner_ang[1];};
     double CornerAngleSW() {return corner_ang[2];};
     double CornerAngleSE() {return corner_ang[3];};
 
     double CornerDistance(int i) {return corner_dist[i%4];};
     double CornerDistanceNE() {return corner_dist[0];};
     double CornerDistanceNW() {return corner_dist[1];};
     double CornerDistanceSW() {return corner_dist[2];};
     double CornerDistanceSE() {return corner_dist[3];};
 
     // Have to move 0.5 square to change value. This means that at the
     // border between cells, the border belongs to the higher index.
     void WorldToGrid(double x, double y, int &row, int &col) {
 	row = (int)(x/size + origin[0] + 0.5);
 	col = (int)(y/size + origin[1] + 0.5);
     };
     void GridToWorld(int row, int col, double &x, double &y) {
 	x = (row - origin[0])*size;
 	y = (col - origin[1])*size;
     };
 
     // Since borders belong to the higher cell, values must be less than
     // the max.  At the max value, you are in the cell outside the grid.
     int InGrid(const double x, const double y) {
 	return ((x < xmax) && (x >= xmin) && (y < ymax) && (y >= ymin));};
 
     unsigned char GridValue(const double x, const double y);
 
     // Friends
	 friend std::ostream& operator << (std::ostream &os, const Grid &g);
	 friend std::istream& operator >> (std::istream &is, Grid &g);
 };
 
 
 #endif /*GRID_H*/
