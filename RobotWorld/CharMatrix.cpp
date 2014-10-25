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
** File: CharMatrix.cpp - matrix class to be used for Henry
**
**
*/

#include <memory.h>
#include "CharMatrix.h"

#define Min(a,b) (((a) < (b)) ? a : b)
#define Max(a,b) (((a) > (b)) ? a : b)
#define abs(a) (((a) >= 0) ? a : -a)

/*
   This function initializes the space for the Character Matrix
*/
void CharMatrix::InitSpace(int num_rows, int num_cols)
{
    assert(num_rows >= 0 && num_cols >= 0);

    rows = num_rows;
    cols = num_cols;

    if (num_rows * num_cols == 0)
    {
        mat = NULL;
        mat_rows = NULL;
        return;
    }

    mat = new unsigned char[num_rows * num_cols];
    mat_rows = new(unsigned char *[num_rows]);

    int i;

    for (i = 0; i < num_rows; i++)
    {
        mat_rows[i] = &mat[i * num_cols];
    }
}




// Construct a Character Matrix.  The matrix is initialized with the values
// in  init
CharMatrix::CharMatrix(int num_rows, int num_cols, unsigned char init,
                       unsigned char init_min_value,
                       unsigned char init_max_value)
{
    unsigned char *ptr;
    int i;

    InitSpace(num_rows, num_cols);
    ptr = mat;
    i = num_rows * num_cols;

    while (i--)
    {
        *ptr++ = init;
    }

    min_value = init_min_value;
    max_value = init_max_value;
}



CharMatrix::CharMatrix(const CharMatrix &m)
{
    InitSpace(m.rows, m.cols);
    memcpy(mat, m.mat, rows * cols);
}



// Extracts submatrix which has the top left corner at position (x,y) and
// is height x width rows.  If a portion of the sub matrix is outside
// boundary of the matrix, that portion is initialized with 0.
CharMatrix::CharMatrix(const CharMatrix &m, int row, int col, int height,
                       int width, unsigned char init)
{
    int end_row, num_cols;
    int i, j, this_i, sub_i, sub_j;
    unsigned char *ptr;

    InitSpace(height, width);
    min_value = m.MinimumValue();
    max_value = m.MaximumValue();

    // Check to see if submatrix outside bounds of "m"
    if ((row >= m.rows) || (col >= m.cols)
            || (row + height <= 0) || (col + width <= 0))
    {
        ptr = (*this)[0];
        i = rows * cols;

        while (i--)
        {
            *ptr++ = init;
        }

        return;
    }

    if (row < 0)
    {
        sub_i = -row;
        height += row;
        row = 0;
    }
    else
    {
        sub_i = 0;
    }

    if (col < 0)
    {
        sub_j = -col;
        width += col;
        col = 0;
    }
    else
    {
        sub_j = 0;
    }

    end_row = Min(row + height, m.rows);
    num_cols = Min(width, m.cols - col);

    for (i = row, this_i = sub_i; i < end_row; i++, this_i++)
    {
        for (j = 0; j < sub_j; j++)
        {
            (*this)[this_i][j] = init;
        }

        memcpy((*this)[this_i] + sub_j, m[i] + col, num_cols);

        for (j = sub_j + num_cols; j < cols; j++)
        {
            (*this)[this_i][j] = init;
        }
    }

    // Fill in the rest of the matrix
    if (sub_i != 0)  		//Above
    {
        ptr = (*this)[0];
        i = sub_i * cols;

        while (i--)
        {
            *ptr++ = init;
        }
    }

    if (sub_i + end_row - row < rows)  	//Below
    {
        ptr = (*this)[sub_i + end_row - row];
        i = (rows - sub_i - end_row + row) * cols;

        while (i--)
        {
            *ptr++ = init;
        }
    }
}



CharMatrix::~CharMatrix()
{
    delete[] mat_rows;
    delete[] mat;
}




// This functions returns the area which would be covered over by the
// matrix "m" if it was placed at row, col.
int CharMatrix::Overlap(const CharMatrix &m, int row, int col, int &row_min,
                        int &col_min, int &row_max, int &col_max) const
{
    // No overlap
    if ((row >= rows) || (col >= cols)
            || (row + m.rows <= 0) || (col + m.cols <= 0))
    {
        row_min = col_min = row_max = col_max = -1;
        return 0;
    }

    row_min = Max(row, 0);
    col_min = Max(col, 0);
    row_max = Min(rows, row + m.rows) - 1;
    col_max = Min(cols, col + m.cols) - 1;
    return 1;
}



int CharMatrix::Replace(int row, int col, const CharMatrix &m)
{
    int t_rmin, t_cmin, t_rmax, t_cmax;
    int m_rmin, m_cmin, m_rmax, m_cmax;
    int num_col, i, j;

    if (!Overlap(m, row, col, t_rmin, t_cmin, t_rmax, t_cmax))
    {
        return 0;
    }

    m.Overlap(*this, -row, -col, m_rmin, m_cmin, m_rmax, m_cmax);
    num_col = t_cmax - t_cmin + 1;

    for (i = t_rmin, j = m_rmin; i <= t_rmax; i++, j++)
    {
        memcpy((*this)[i] + t_cmin, m[j] + m_cmin, num_col);
    }

    return 1;
}



/*
void CharMatrix::Clear(int row, int col, const CharMatrix &m)
{
    int t_rmin, t_cmin, t_rmax, t_cmax;
    int m_rmin, m_cmin, m_rmax, m_cmax;
    int num_col, i, j, cnt;
    unsigned char *p;
    int sum;

    if (!Overlap(m, row, col, t_rmin, t_cmin, t_rmax, t_cmax))
	return;

    m.Overlap(*this, -row, -col, m_rmin, m_cmin, m_rmax, m_cmax);
    num_col = t_cmax - t_cmin + 1;
    for (i = t_rmin, j = m_rmin; i <= t_rmax; i++, j++) {
	p = (*this)[i] + t_cmin;
	const unsigned char *p2 = m[j] + m_cmin;
	cnt = num_col;
	while (cnt--) {
	    // Make sure it can be increased
	    if ((*p >= MinimumValue()) && (*p < MaximumValue())) {

// Note: this need to correspond to the values put into the grid
#ifdef SUM
// problems: if sitting this keeps getting more clear (no "new" data)
		sum = *p + *p2;
#endif
#ifdef MAX
// problems: if moving and move around in area of error, this won't clear it.
		sum = Max(*p, *p2);
#endif
		sum = Max(*p, *p2);
		if (sum > MaximumValue())
		    *p = MaximumValue();
		else
		    *p = sum;
	    }
	    p++;
	    p2++;
	}
    }
}



void CharMatrix::Occupy(int row, int col, const CharMatrix &m)
{
    int t_rmin, t_cmin, t_rmax, t_cmax;
    int m_rmin, m_cmin, m_rmax, m_cmax;
    int num_col, i, j, cnt;
    unsigned char *p;
    int sum;

    if (!Overlap(m, row, col, t_rmin, t_cmin, t_rmax, t_cmax))
	return;

    m.Overlap(*this, -row, -col, m_rmin, m_cmin, m_rmax, m_cmax);
    num_col = t_cmax - t_cmin + 1;
    for (i = t_rmin, j = m_rmin; i <= t_rmax; i++, j++) {
	p = (*this)[i] + t_cmin;
	const unsigned char *p2 = m[j] + m_cmin;
	cnt = num_col;
	while (cnt--) {
	    // Make sure it can be decreased
	    if ((*p > MinimumValue()) && (*p <= MaximumValue())) {
		// sum = *p - *p2;
		sum = Min(*p,*p2);
		if (sum < MinimumValue())
		    *p = MinimumValue();
		else
		    *p = sum;
	    }
	    p++;
	    p2++;
	}
    }
}
*/



void CharMatrix::Blend(int row, int col, const CharMatrix &m)
{
    int t_rmin, t_cmin, t_rmax, t_cmax;
    int m_rmin, m_cmin, m_rmax, m_cmax;
    int num_col, i, j, cnt;
    unsigned char *p;
    int old, new_info, blend;

    if (!Overlap(m, row, col, t_rmin, t_cmin, t_rmax, t_cmax))
    {
        return;
    }

    m.Overlap(*this, -row, -col, m_rmin, m_cmin, m_rmax, m_cmax);
    num_col = t_cmax - t_cmin + 1;

    for (i = t_rmin, j = m_rmin; i <= t_rmax; i++, j++)
    {
        p = (*this)[i] + t_cmin;
        const unsigned char *p2 = m[j] + m_cmin;
        cnt = num_col;

        while (cnt--)
        {
            // Make sure it can change
            if ((*p >= MinimumValue()) && (*p <= MaximumValue()))
            {
                old = *p - NEUTRAL_NUMBER;
                new_info = *p2 - NEUTRAL_NUMBER;

                if ((old >= 0) && (new_info >= 0))
                {
                    blend = Max(old, new_info) + NEUTRAL_NUMBER;
                }
                else if ((old <= 0) && (new_info <= 0))
                {
                    blend = Min(old, new_info) + NEUTRAL_NUMBER;
                }
                else
                {
                    if (abs(old) >= abs(new_info))
                    {
                        blend = old + NEUTRAL_NUMBER;
                    }
                    else
                    {
                        blend = new_info + NEUTRAL_NUMBER;
                    }

                    /*
                       This blended the numbers if one is positive and one
                       negative.  This tends to wipe out strong values.
                    // old positive and new_info negative
                    if (old > new_info) {
                    blend = ( old*old - new_info*new_info)/(old - new_info) + NEUTRAL_NUMBER;
                    } else {
                    blend = (-old*old + new_info*new_info)/(new_info - old) + NEUTRAL_NUMBER;
                    }
                    */
                }

                if (blend > MaximumValue())
                {
                    *p = MaximumValue();
                }
                else if (blend < MinimumValue())
                {
                    *p = MinimumValue();
                }
                else
                {
                    *p = blend;
                }
            }

            p++;
            p2++;
        }
    }
}




CharMatrix& CharMatrix::operator=(const CharMatrix& m)
{
    if (this != &m)    // Make sure not m=m;
    {
        delete[] mat_rows;
        delete[] mat;
        InitSpace(m.rows, m.cols);
        memcpy(mat, m.mat, rows * cols);
    }

    return *this;
}



std::ostream& operator << (std::ostream &os, const CharMatrix &m)
{
    int i, j;

    os << m.rows << " " << m.cols << std::endl;
    os << (int)m.min_value << " " << (int)m.max_value << std::endl;

    for (i = 0; i < m.rows; i++)
    {
        for (j = 0; j < m.cols - 1; j++)
        {
            os << (int)m[i][j] << " ";
        }

        os << (int)m[i][m.cols - 1] << std::endl;
    }

    return (os);
}




std::istream& operator >> (std::istream &is, CharMatrix &m)
{
    int i, j, tmp;

    if (m.rows * m.cols)
    {
        delete[] m.mat_rows;
        delete[] m.mat;
    }

    is >> i >> j;
    m.InitSpace(i, j);
    is >> tmp;
    m.min_value = (unsigned char)tmp;
    is >> tmp;
    m.max_value = (unsigned char)tmp;

    for (i = 0; i < m.rows; i++)
    {
        for (j = 0; j < m.cols; j++)
        {
            is >> tmp;
            m[i][j] = (unsigned char)tmp;
        }
    }

    return (is);
}

