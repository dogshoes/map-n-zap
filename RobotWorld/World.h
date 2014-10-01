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
 
 #ifndef WORLD_H
 #define WORLD_H
 
 #include "Grid.h"
 #include "Rover.h"
 
 /*
    This is the class that represents the world of the rover
 */
 
 class Path;
 
 class World : public Grid {
   public:
     Rover	rover;
   private:
   public:
     unsigned char open_value;
     void (*ChangeScreen)(int row, int col, CharMatrix &m, CharMatrix &world);
     void (*ShowPath)(Path *path, int type);
 
     World(int row, int col, double size, int rover_width, int rover_length,
 	  unsigned char init=0, unsigned char min=0, unsigned char max=255)
 	: Grid(row, col, size, init, min, max),
 	  rover(rover_width, rover_length) {
 	      ChangeScreen = NULL;
 	      SetOrigin(0.0, 0.0);
 	      open_value = 200;
 	};
 
     World(int row, int col, double size, Rover &r, unsigned char init=0,
 	  unsigned char min=0, unsigned char max=255)
 	: Grid(row, col, size, init, min, max),
 
 	  rover(r) {
 	      ChangeScreen = NULL;
 	      SetOrigin(0.0, 0.0);
 	      open_value = 200;
 	};
 
     World(int row, int col, double size, Grid &g, unsigned char init=0,
 	  unsigned char min=0, unsigned char max=255)
 	: Grid(row, col, size, init, min, max),
 
 	  rover(g) {
 	      ChangeScreen = NULL;
 	      SetOrigin(0.0, 0.0);
 	      open_value = 200;
 	};
 
     unsigned char OpenValue() {return open_value;};
     void ClearRoverPosition(double x, double y, double th, double dx,
 			    double dy, double dth);
     void ObstacleRoverPosition(double x, double y, double th, double dx,
 			       double dy, double dth, RoverGrid ind);
     Path *FindPath(Path *path, int max_iter);
 };
 
 
 #endif /*WORLD_H*/
 
 
