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
 
 
 #ifndef ROVER_H
 #define ROVER_H
 
 #include <math.h>
 #include "Grid.h"
 
 #define ROVER_WIDTH	11	// The default number of grid point in x
 #define ROVER_LENGTH	16	// The default number of grid point in y
 #define ROVER_GRID_SIZE	 1	// The default grid size
 
 #define ROVER_PATH_BUFFER_GRIDS 1 /* The default buffer about the path grid
 				     in grid spaces*/
 
 enum RoverGrid {Body, M0_F, M1_F, M2_F, M0_B, M1_B, M2_B, PathBody};
 
 
 // This class is used to keep track of the rover's current location and
 // also the error (delta) range.
 class RoverLocation {
   private:
     double x;
     double y;
     double th;
   public:
     RoverLocation() {x = y = th = 0;};
     RoverLocation(double lx, double ly, double lth) {x = lx; y = ly; th =lth;};
     
     double X(void) {return x;};
     double Y(void) {return y;};
     double Theta(void) {return th;};
 
     friend class Rover;
 };
 
 
 /*
    This class represents the rover.  The size of the rover is set upon
    creation.  The half_size, diagonal_half_length, and corner_angle are
    used internally to help spped up some of the calculations for the grid
    values.
 */
 class Rover {
   private:
     RoverLocation position;		// The rovers location
     RoverLocation delta;		// The error in location
     Grid body;				// The body representation
     Grid m0_f;				// The collision grids
     Grid m1_f;
     Grid m2_f;
     Grid m0_b;
     Grid m1_b;
     Grid m2_b;
     Grid path;
 
     double buffer_size;			// The buffer around the rover (world)
     double path_step_size;		// The step size (world)
     
     // A function to find the size of the grid to make to represent
     // the rotated/expanded version of grid g for the rover's size,
     // location, and delta.
     Grid *FindRotatedGridSize(Grid &g, double grid_size);
 
     // The rotated/expanded grid for the given size, location, and delta
     // of the rover.
     // The grid spacing is the box size of the grid.
     // The grid is centered at the center of the rover.
     // We are assuming square boxes.
     Grid *FindNewGrid(Grid &g, double grid_spacing);
 
     // Set the path grid whenever the body grid is set
     void  MakePathGrid(double new_buffer_size);
 
     // Set up the other private variables
     void InitRover();
   public:
     Rover(int rows = ROVER_WIDTH, int columns = ROVER_LENGTH,
 	  double grid_size=ROVER_GRID_SIZE)
 	: body(rows, columns, grid_size, NEUTRAL_NUMBER) {InitRover();};
     Rover(/*const*/ Rover &r) 
 	: body(r.body), m0_f(r.m0_f), m1_f(r.m1_f), m2_f(r.m2_f),
 	  m0_b(r.m0_b), m1_b(r.m1_b), m2_b(r.m2_b) {
 	      InitRover(); SetPathBuffer(r.PathBuffer());};
     Rover(const Grid &g) : body (g) {InitRover();};
 
     void SetLocation(double x, double y, double th) {
         position.x = x; position.y = y; position.th=th;};
     void SetLocationDelta(double x, double y, double th) {
 	delta.x = x; delta.y = y; delta.th=th;};
 
     void GetLocation(double &x, double &y, double &z) {
 	x = position.x; y = position.y; z = position.th;};
     void GetLocationDelta(double &x, double &y, double &z) {
 	x = delta.x; y = delta.y; z = delta.th;};
 
     double PathStepSize() {return path_step_size;};
 
     double PathBuffer() {return buffer_size;};
     void   SetPathBuffer(double p) {MakePathGrid(p);};
 
     void SetGrid (const RoverGrid c, const Grid &g);
     Grid *GetGrid (const RoverGrid c);
 
     // The function which finds the clear grid
     Grid *ClearGrid(const double grid_size) {
 	return FindNewGrid(body, grid_size);};
 
     // The function which finds the obstacle grid
     Grid *ObstacleGrid(const double grid_size, const RoverGrid ind) {
 	return FindNewGrid(*GetGrid(ind), grid_size);};
 
     // The function which finds the grid used in the path finding algorithm
     Grid *PathGrid(const double grid_size) {
 	return FindNewGrid(path, grid_size);};
 };
 
 
 #endif /* ROVER_H */
