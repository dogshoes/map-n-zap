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


#ifndef CHARMATRIX_H
#define CHARMATRIX_H

#include <istream>
#include <ostream>
#include <iostream>
#include <assert.h>

#define NEUTRAL_NUMBER 127

/*
   This is the class that is used to represent the probability grid for
   the world and rover's clear and obstacle areas.
*/
class CharMatrix
{
    private:
        int rows;
        int cols;
        unsigned char *mat;
        unsigned char **mat_rows;
        unsigned char min_value;	//The first non-reserve value at the min end
        unsigned char max_value;	//The last non-reserve value at the max end
        void InitSpace(int num_rows, int num_cols);
    public:
        CharMatrix()
        {
            rows = 0;
            cols = 0;
            mat = 0;
            min_value = 0;
            max_value = 255;
            mat_rows = 0;
        };
        CharMatrix(int num_rows, int num_cols, unsigned char init = 0,
                   unsigned char min_value = 0, unsigned char max_value = 255);
        CharMatrix(const CharMatrix &);
        CharMatrix(const CharMatrix &, int row, int col, int height, int width,
                   unsigned char init = 0);
        ~CharMatrix();

        int NumberOfRows()
        {
            return rows;
        };
        int NumberOfColumns()
        {
            return cols;
        };
        unsigned char MinimumValue() const
        {
            return min_value;
        };
        unsigned char MaximumValue() const
        {
            return max_value;
        };

        void SetMinumumValue(unsigned char min)
        {
            min_value = min;
        };
        void SetMaximumValue(unsigned char max)
        {
            max_value = max;
        };

        // The portion of this matrix that would overlap the given matrix if the
        // given matrix was placed at row, col.
        int Overlap(const CharMatrix &, int row, int col,
                    int &row_min, int &col_min, int &row_max, int &col_max) const;

        // Replace values in this matrix with the values of the given matrix
        // placed at (row, col)
        int Replace(int row, int col, const CharMatrix &m);

        // This blends the matrix m into this matrix. m is unchanged
        void Blend(int row, int col, const CharMatrix &m);

        // Operators
        CharMatrix& operator=(const CharMatrix&);
        unsigned char * operator[](int pos);
        const unsigned char * operator[](int pos) const;

        // Friends
        friend std::ostream& operator << (std::ostream &os, const CharMatrix &m);
        friend std::istream& operator >> (std::istream &is, CharMatrix &m);
};


inline unsigned char *CharMatrix::operator[](int pos)
{
    assert(pos >= 0 && pos < rows);
    return mat_rows[pos];
}


inline const unsigned char *CharMatrix::operator[](int pos) const
{
    assert(pos >= 0 && pos < rows);
    return mat_rows[pos];
}





#endif /* CHARMATRIX_H */
