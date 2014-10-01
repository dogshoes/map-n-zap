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
 
  
  #include "Rover.h"
  
  #define Min(a,b) (((a) < (b)) ? a : b)
  #define Max(a,b) (((a) > (b)) ? a : b)
  
  
  
  /* 
     This smearing function keeps a running total instead of reading each time
     one number is added and the old number at the end of the filter is dropped.
     Since I am keeping a running total, I can just write in the grid given.
  */
  static void XSmear(CharMatrix &base, int x_smear)
  {
      int i, j, k;
      int sum, k_max;
  
      int denom = 2*x_smear + 1;
      int *remember = new int[denom];
      int remember_ind, new_num;
      
      for(j = 0; j < base.NumberOfColumns(); j++) {
  	// initialize the running sum
  	for(k = 0; k < denom; k++)
  	    remember[k] = NEUTRAL_NUMBER;
  	k_max = Min(x_smear + 1, base.NumberOfRows());
  	sum = 0;
  	for (k = 0; k < k_max; k++) {
  	    remember[k] = base[k][j];
  	    sum += remember[k];
  	}
  	sum += NEUTRAL_NUMBER*(denom-k_max);
  	remember_ind = k_max;
  	k = k_max;
  	base[0][j] = (unsigned char)(sum/(double)denom);
  
  	for(i = 1; i < base.NumberOfRows(); i++) {
  	    if (k < base.NumberOfRows())
  		new_num = base[k][j];
  	    else
  		new_num = NEUTRAL_NUMBER;
  	    sum += new_num - remember[remember_ind];
  	    remember[remember_ind] = new_num;
  	    k++;
  	    remember_ind++;
  	    remember_ind %= denom;
  	    base[i][j] = (unsigned char)(sum/(double)denom);
  	}
      }
      delete[] remember;
  }
  
  
  
  static void YSmear(CharMatrix &base, int y_smear)
  {
      int i, j, k;
      int sum, k_max;
  
      int denom = 2*y_smear + 1;
      int *remember = new int[denom];
      int remember_ind, new_num;
  
      for(i = 0; i < base.NumberOfRows(); i++) {
  	// initialize the running sum
  	for(k = 0; k < denom; k++)
  	    remember[k] = NEUTRAL_NUMBER;
  	k_max = Min(y_smear + 1, base.NumberOfColumns());
  	sum = 0;
  	for (k = 0; k < k_max; k++) {
  	    remember[k] = base[i][k];
  	    sum += remember[k];
  	}
  	sum += NEUTRAL_NUMBER*(denom-k_max);
  	remember_ind = k_max;
  	k = k_max;
  	base[i][0] = (unsigned char)(sum/(double)denom);
  
  	for(j = 1; j < base.NumberOfColumns(); j++) {
  	    if (k < base.NumberOfColumns())
  		new_num = base[i][k];
  	    else
  		new_num = NEUTRAL_NUMBER;
  	    sum += new_num - remember[remember_ind];
  	    remember[remember_ind] = new_num;
  	    k++;
  	    remember_ind++;
  	    remember_ind %= denom;
  	    base[i][j] = (unsigned char)(sum/(double)denom);
  	}
      }
      delete[] remember;
  }
  
  
  
  /* 
      This routine will return the size of the grid needed to hold the rotated
      rover grid with a buffer zone of the rover delta around it.
  */
  Grid *Rover::FindRotatedGridSize(Grid &g, double grid_spacing)
  {
      double min_x, max_x, min_y, max_y;
      
      min_x = max_x = g.CornerDistance(0) * cos(position.th + g.CornerAngle(0));
      min_y = max_y = g.CornerDistance(0) * sin(position.th + g.CornerAngle(0));
  
      int i;
      double pos_x, pos_y;
      for (i = 1; i < 4; i++) {
  	pos_x = g.CornerDistance(i) * cos(position.th + g.CornerAngle(i));
  	pos_y = g.CornerDistance(i) * sin(position.th + g.CornerAngle(i));
  	
  	if (pos_x < min_x)
  	    min_x = pos_x;
  	else if (pos_x > max_x)
  	    max_x = pos_x;
  
  	if (pos_y < min_y)
  	    min_y = pos_y;
  	else if (pos_y > max_y)
  	    max_y = pos_y;
      }
      min_x -= delta.x;
      min_y -= delta.y;
      max_x += delta.x;
      max_y += delta.y;
  
      int num_rows, num_cols;
      num_rows = (int)((max_x - min_x)/grid_spacing);
      num_cols = (int)((max_y - min_y)/grid_spacing);
      
      Grid *grid = new Grid(num_rows, num_cols, grid_spacing, NEUTRAL_NUMBER,
  			  0, 255);
  
      // Have to compensate if the origin coordinate is negative (because of
      // the way negative numbers are rounding).
      if (min_x > 0)
  	min_x += grid_spacing;
      if (min_y > 0)
  	min_y += grid_spacing;
      grid->SetOrigin((int)(-min_x/grid_spacing), (int)(-min_y/grid_spacing));
      return grid;
  }
  
  
  
  void Rover::SetGrid (const RoverGrid c, const Grid &g) 
  {
      switch (c) {
        case Body:
  	body = g;
  	MakePathGrid(buffer_size);
  	break;
        case M0_F:
  	m0_f = g;
  	break;
        case M1_F:
  	m1_f = g;
  	break;
        case M2_F:
  	m2_f = g;
  	break;
        case M0_B:
  	m0_b = g;
  	break;
        case M1_B:
  	m1_b = g;
  	break;
        case M2_B:
  	m2_b = g;
  	break;
        case PathBody:
  	path = g;
  	break;
      }
  }
  
  
  Grid *Rover::GetGrid (const RoverGrid c) 
  {
      switch (c) {
        case Body:
  	return &body;
        case M0_F:
  	return &m0_f;
        case M1_F:
  	return &m1_f;
        case M2_F:
  	return &m2_f;
        case M0_B:
  	return &m0_b;
        case M1_B:
  	return &m1_b;
        case M2_B:
  	return &m2_b;
        case PathBody:
  	return &path;
      }
      return NULL;
  }
  
  
  
  /*
     Create the probability grid for clear area around the rover.
     This area is centered around the center of the rover.
  */
  Grid *Rover::FindNewGrid(Grid &g, double grid_spacing)
  {
      Grid *base = FindRotatedGridSize(g, grid_spacing);
  
      int base_rows = base->NumberOfRows();
      int base_cols = base->NumberOfColumns();
      double x, y, norm_x, norm_y;
      double cth = cos(position.th), sth = sin(position.th);
  
      double *s_x = new double[base_rows];
      double *c_x = new double[base_rows];
      double *s_y = new double[base_cols];
      double *c_y = new double[base_cols];
      
      int i, j;
      double origin_x, origin_y;
      base->GetOrigin(origin_x, origin_y);
      for (i = 0; i < base_rows; i++) {
  	x = (i - origin_x)*grid_spacing;
  	s_x[i] = sth*x;
  	c_x[i] = cth*x;
      }
      for (i = 0; i < base_cols; i++) {
  	y = (i - origin_y)*grid_spacing;
  	s_y[i] = sth*y;
  	c_y[i] = cth*y;
      }
  
      for(i = 0; i < base_rows; i++) {
  	for(j = 0; j < base_cols; j++) {
  	    // Find position relative to unrotated grid
  	    norm_x = c_x[i] + s_y[j];
  	    norm_y =-s_x[i] + c_y[j];
  	    if (g.InGrid(norm_x, norm_y)) {
  		(*base)[i][j] = g.GridValue(norm_x, norm_y);
  	    }
  	}
      }
      delete[] c_y;
      delete[] s_y;
      delete[] c_x;
      delete[] s_x;
  
      // cerr << "Input" << endl << g;
      // cerr << "Base" << endl << *base;
      
      int x_smear = (int)(delta.x/grid_spacing);
      int y_smear = (int)(delta.y/grid_spacing);
  
      // cout << "Smear Amounts" << x_smear << " " << y_smear << endl;
  
      if (x_smear) {
  	XSmear(*base, x_smear);
      }
  
      if (y_smear) {
  	YSmear(*base, y_smear);
      }
  
      // cerr << "Base Smeared" << endl << *base;
  
      return base;
  }
  
  
  void Rover::InitRover() 
  {
      MakePathGrid(ROVER_PATH_BUFFER_GRIDS * body.GridSize());
  }
  
  
  
  void Rover::MakePathGrid(double new_buffer_size)
  {
      double x, y;
      int i, j, ii, jj;
  
      buffer_size = new_buffer_size;
      int extra_grids = (int)(buffer_size/body.GridSize());
      Grid *new_path = new Grid(body.NumberOfRows() + 2*extra_grids,
  			      body.NumberOfColumns() + 2*extra_grids,
  			      body.GridSize(), NEUTRAL_NUMBER);
      body.GetOrigin(x, y);
  
      new_path->SetOrigin(x + extra_grids, y + extra_grids);
      for(i = 0; i < body.NumberOfRows(); i++)
  	for(j = 0; j < body.NumberOfColumns(); j++)
  	    if (body[i][j] > NEUTRAL_NUMBER) {
  		for (ii = 0; ii <= 2*extra_grids; ii++)
  		    for (jj = 0; jj <= 2*extra_grids; jj++)
  			(*new_path)[ii + i][jj + j] = 255;
  	    }
      
      int min_step_size = 1;
      int started;
      for(j = 0; j < body.NumberOfColumns(); j++) {
  	started = 0;
  	for(i = 0; i < body.NumberOfRows(); i++) {
  	    if ((*new_path)[i][j] == 255) 
  		started++;
  	    else {
  		if (started && (started < min_step_size))
  		    min_step_size = started;
  		started = 0;
  	    }
  	}
      }
      path_step_size = started*body.GridSize();
      path = *new_path;
      delete new_path;
  }
  
  
  
  /*
  Grid *Rover::ObstacleGrid(const double grid_size, const RoverGrid ind)
  {
      switch (ind) {
        case M0_F:
  	return FindNewGrid(m0_f, grid_size);
        case M1_F:
  	return FindNewGrid(m1_f, grid_size);
        case M2_F:
  	return FindNewGrid(m2_f, grid_size);
        case M0_B:
  	return FindNewGrid(m0_b, grid_size);
        case M1_B:
  	return FindNewGrid(m1_b, grid_size);
        case M2_B:
  	return FindNewGrid(m2_b, grid_size);
      }
      return NULL;
  }
  */
