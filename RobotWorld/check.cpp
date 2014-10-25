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
#include "Path.h"


//
// Below assumes that the mask and world have the same grid sizes and
// that they are alligned.
//
unsigned char MinCovered(World &w, int w_row, int w_col, CharMatrix &mask,
                         unsigned char &average)
{
    int m_rmin, m_cmin, m_rmax, m_cmax;

    if (!mask.Overlap(w, -w_row, -w_col, m_rmin, m_cmin, m_rmax, m_cmax))
    {
        average = NEUTRAL_NUMBER;
        return NEUTRAL_NUMBER;
    }

    unsigned char min_value = 255, value;
    unsigned char *w_r, *m_r;
    int i, j, count = 0, sum = 0;

    for (i = m_rmin; i <= m_rmax; i++)
    {
        w_r = &w[i + w_row][w_col + m_cmin];
        m_r = mask[i];

        for (j = m_cmin; j <= m_cmax; j++)
        {
            if (*m_r != NEUTRAL_NUMBER)
            {
                value = *w_r;
                count++;
                sum += value;

                if (value < min_value)
                {
                    min_value = value;
                }
            }

            m_r++;
            w_r++;
        }
    }

    if (!count)
    {
        average = NEUTRAL_NUMBER;
        return NEUTRAL_NUMBER;
    }

    average = (unsigned char)(sum / count);
    return min_value;
}



//
// The origin of the mask moves from r1,c1 r2,c2 of the world grid.
// Note: At this point we assume that the world and mask have the same grid
//	size
// If break_off flag set, will break off if open_value hits
unsigned char Streak(World &w, int r1, int col1, int r2, int col2,
                     Grid &mask, double step_size, double stats[5],
                     unsigned char open_value, int break_off)
{
    double move_dist;
    int move[2], num_moves;
    double step_x, step_y;
    unsigned char min_value = 255, value, seg_average;
    int i;
    double j, k;


    // Find the grid for the bottom left corner.
    r1 -= (int)mask.GetOriginX();
    r2 -= (int)mask.GetOriginX();
    col1 -= (int)mask.GetOriginY();
    col2 -= (int)mask.GetOriginY();

    move[0] = r2 - r1;
    move[1] = col2 - col1;

    move_dist = sqrt((double)(move[0] * move[0] + move[1] * move[1])) * w.GridSize();
    num_moves = int(move_dist / step_size + 1);
    step_x = move[0] / (double)num_moves;
    step_y = move[1] / (double)num_moves;

    // cerr << mask;

    int hit_bad_spot = 0;
    int num_okay = 0, start_okay = (num_moves + 1), total_okay = 0;
    int sum_average = 0;

    /*
        for (k = 0; k <= num_moves; k++) {
    	value = MinCovered(w, r1 + (int)(k*step_x+0.5),
    			   col1 + (int)(k*step_y+0.5), mask, seg_average);
    */
    for (i = 0, j = 0.5, k = 0.5; i <= num_moves; i++, j += step_x, k += step_y)
    {
        value = MinCovered(w, r1 + (int)j, col1 + (int)k, mask, seg_average);
        sum_average += seg_average;

        if (value < min_value)
        {
            min_value = value;
        }

        if (value > open_value)
        {
            num_okay++;
            total_okay++;
        }
        else
        {
            if (!hit_bad_spot)
            {
                start_okay = num_okay;
            }

            num_okay = 0;
            hit_bad_spot = 1;

            if (break_off)
            {
                break;
            }
        }
    }

    stats[MinFound] = min_value;
    stats[AveFound] = sum_average / (num_moves + 1);
    stats[StartDist] = (start_okay - 1) * step_size;
    stats[EndDist] = (num_okay - 1) * step_size;
    stats[FreeDist] = (total_okay - 1) * step_size;

    if (stats[StartDist] > move_dist)
    {
        stats[StartDist] = move_dist;
    }

    if (stats[EndDist] > move_dist)
    {
        stats[EndDist] = move_dist;
    }

    if (stats[FreeDist] > move_dist)
    {
        stats[FreeDist] = move_dist;
    }

    return min_value;
}




//
// The circle around r1, c1 is searched for a door within "distance" in
// angular increments of angle_delta.
// First, distances (how far you can move before a grid less that open_value
// is hit) are measured at each angle.  These are searched for a spike
// indicating a opening is found.
// Note: At this point we assume that the world and mask have the same grid
//	size
int FindDoor(World &w, int r1, int c1, int &r2, int &c2, int distance,
             int num_angle_steps, unsigned char open_value, double base_angle)
{
    int i, ans, this_index;
    double *distance_array = new double[num_angle_steps];
    double angle, stats[5];
    double angle_delta = 3.1415926 * 2 / num_angle_steps;
    double threshold, this_dist, door_dist;

    for (i = 0; i < num_angle_steps; i++)
    {
        angle = angle_delta * i + base_angle;
        r2 = int(r1 + distance * cos(angle));
        c2 = int(c1 + distance * sin(angle));

        w.rover.SetLocation(0, 0, atan2((double)(c2 - c1),
                                        (double)(r2 - r1)));
        w.rover.SetLocationDelta(0.0, 0.0, 0.0);
        Grid *clear = w.rover.PathGrid(w.GridSize());

        Streak(w, r1, c1, r2, c2, *clear, w.rover.PathStepSize(),
               stats, open_value, 1);

        distance_array[i] = stats[StartDist];
        delete clear;
    }

    // Step through distance_array and find distance spike
    threshold = w.rover.PathStepSize() * 10;
    ans = -1;

    for (i = 0; i < (num_angle_steps + 1) / 2; i++)
    {
        this_dist = distance_array[i]
                    - 0.25 * (distance_array[(i + 2) % num_angle_steps]
                              + distance_array[(i + 1) % num_angle_steps]
                              + distance_array[(i - 1 + num_angle_steps) % num_angle_steps]
                              + distance_array[(i - 2 + num_angle_steps) % num_angle_steps]);

        if (this_dist > threshold)
        {
            ans = i;
            door_dist = (distance_array[ans]
                         - w.rover.PathStepSize()) / w.GridSize() ;
            break;
        }

        this_index = num_angle_steps - 1 - i + num_angle_steps;
        this_dist = distance_array[this_index - num_angle_steps]
                    - 0.25 * (distance_array[(this_index + 2) % num_angle_steps]
                              + distance_array[(this_index + 1) % num_angle_steps]
                              + distance_array[(this_index - 1) % num_angle_steps]
                              + distance_array[(this_index - 2) % num_angle_steps]);

        if (this_dist > threshold)
        {
            ans = num_angle_steps - 1 - i;
            door_dist = (distance_array[ans]
                         - w.rover.PathStepSize()) / w.GridSize() ;
            break;
        }
    }

    delete[] distance_array;

    if (ans == -1)
    {
        return -1;
    }

    angle = angle_delta * ans + base_angle;
    r2 = int(r1 + door_dist * cos(angle));
    c2 = int(c1 + door_dist * sin(angle));

    return 1;
}





//
// Quickly look along a line to see how open it is
//
double TestDirection(World &w, int r1, int col1, int r2, int col2,
                     unsigned char open_value)
{
    double move_dist;
    int move[2], num_moves;
    double step_x, step_y;
    unsigned char value;
    int i;
    double j, k;

    move[0] = r2 - r1;
    move[1] = col2 - col1;
    move_dist = sqrt((double)(move[0] * move[0] + move[1] * move[1]));
    num_moves = int(move_dist + 1);
    step_x = move[0] / (double)num_moves;
    step_y = move[1] / (double)num_moves;

    for (i = 0, j = 0.5, k = 0.5; i < num_moves; i++, j += step_x, k += step_y)
    {
        value = w[r1 + (int)j][col1 + (int)k];

        /*
            for (i = 0; i <= num_moves; i++) {
        	value = w[r1 + (int)(i*step_x+0.5)][col1 + (int)(i*step_y+0.5)];
        */
        if (value < open_value)
        {
            return i * move_dist / num_moves;
        }
    }

    return move_dist;
}


