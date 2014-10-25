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

#include "stdafx.h"
#include <float.h>
#include "RothMath.h"


/*******************************************************************************
//Created: 10/21/97 S.R.
//Last Revision: 10/21/97 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/

double sqr(double x)
/*******************************************************************************
//Created: 10/23/97 S.R.
//Last Revision: 10/23/97 S.R.
//Parameters:
//	x1,x2:	number to square.
//Return Value:
//	x1*x1
********************************************************************************
//Description:
//	This function takes x and squares it.
//Method:
//
*******************************************************************************/
{
    return (x * x);
}

double XPROD(robPOINT V1, robPOINT V2)
/*******************************************************************************
//Created: 04/15/96 S.R.
//Last Revision: 04/15/96 S.R.
//Parameters:
//	V1,V2		The vectors to take the cross product of.
//Return Value:
//	The Z value of the cross product
********************************************************************************
//Description:
//		This function takes the cross product of two vectors in the x-y plane
//	starting at the origin.
//Method:
//	Uses algebra to calculate the cross product.
*******************************************************************************/
{
    return (V1.x * V2.y - V2.x * V1.y);
}

double Distance(POINT P1, POINT P2)
/*******************************************************************************
//S.R. 09/05/96
//last revision: 09/05/96 S.R.
//Parameters:
//	P1,P2:	points to find the distance between.
//Return Value:
//	The distance between P1 and P2.
********************************************************************************
//Description:
//	This function returns the distance between points P1 and P2.
//Method:
//	The function uses the pythagorean theorem to find the distance between P1 and P2.
*******************************************************************************/
{
    return sqrt((double)SQR(P1.x - P2.x) + SQR(P1.y - P2.y));
}

double Distance(robPOINT P1, robPOINT P2)
/*******************************************************************************
//S.R. 07/16/96
//last revision: 09/05/96 S.R.
//Parameters:
//	P1,P2:	points to find the distance between.
//Return Value:
//	The distance between P1 and P2.
********************************************************************************
//Description:
//	This function returns the distance between points P1 and P2.
//Method:
//	The function uses the pythagorean theorem to find the distance between P1 and P2.
*******************************************************************************/
{
    return sqrt(SQR(P1.x - P2.x) + SQR(P1.y - P2.y));
}

TLineErr FindBisectorsOfLines(TLine* L1, TLine* L2)
/*******************************************************************************
//S.R. 07/10/96
//last revision: 07/12/96 S.R.
//Parameters:
//	L1,L2:	On Input, the two lines to bisect.  On Output, the two bisectors.
//Return Value:
//	NoErr:				Nothing out of the ordinary happened.
//	ErrParallelLines:	The two lines were parallel.
********************************************************************************
//Description:
//		This function finds and returns the lines which bisect the angle between
//	L1 and L2.
//Method:
//		First the function tries to find the intersection of the two lines.  If
//	there isn't one then the two lines are parallel.  In this case, the function
//	returns the line that runs between the original lines, and returns the error
// code ERRParallelLines.
//		Otherwise, the function finds the angle of the two lines from the x-axis.
//	The angle of the first bisector is simply the average of these two angles.
//	The angle of the second bisecotr is perpendicular to the first bisector.
//	The slope of the bisector's line is the tangent of its angle.  The line
// must pass through the intersection of L1 and L2.  Using this and the slope
// of the line we find the bisector's y intercept.
//
//        Bisector 2                       Bisector
//   L2 \     |     / L1               L2|    |    |L1
//        \   |   /                      |    |    |
//          \ | /                        |    |    |
//     -------|------- Bisector 1        |    |    |
//          / | \                        |    |    |
//        /   |   \                      |    |    |
//      /     |     \                    |    |    |
*******************************************************************************/
{
    robPOINT Intersection;

    if (IntersectionOfLines(*L1, *L2, &Intersection) == ERRParallelLines)
    {
        /*Take care of parallel lines*/
        L1->b = (L1->b + L2->b) / 2;
        L2->b = L1->b;
        return ERRParallelLines;
    }
    else
    {
        /*Take care of none parallel lines*/
        /*Find angles of the two lines*/
        double A1, A2;

        if (L1->Vertical == TRUE)
        {
            A1 = pi / 2;
            A2 = atan(L2->m);
        }
        else if (L2->Vertical == TRUE)
        {
            A1 = atan(L1->m);
            A2 = pi / 2;
        }
        else
        {
            A1 = atan(L1->m);
            A2 = atan(L2->m);
        }

        /*First bisector's angle is average of two lines' angles*/
        A1 = (A1 + A2) / 2;
        L1->m = tan(A1);
        L1->b = Intersection.y - Intersection.x * L1->m;
        L1->Vertical = FALSE;

        /*Second bisector's angle is perpendicular to first bisector*/
        if (L1->m != 0)
        {
            L2->m = -1 / L1->m;
            L2->b = Intersection.y - Intersection.x * L2->m;
            L2->Vertical = FALSE;
        }
        else
        {
            L2->m = 0.0f;
            L2->b = Intersection.x;
            L2->Vertical = TRUE;
        }

        return NoErr;
    }
}


TLineErr FindLineFrom2Points(POINT P1, POINT P2, TLine* Line)
/*******************************************************************************
//Created: 09/05/96 S.R.
//Last Revision: 09/05/96 S.R.
//Parameters:
//
//Return Value:
//
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    if ((P1.x == P2.x) && (P1.y == P2.y))
    {
        return ERRNoSolution;
    }

    if (P1.x == P2.x)
    {
        Line->Vertical = TRUE;
        Line->m = 0.0f;
        Line->b = P1.x;
    }
    else
    {
        Line->Vertical = FALSE;
        Line->m = ((double)P1.y - P2.y) / (P1.x - P2.x);
        Line->b = P1.y - P1.x * Line->m;
    }

    return NoErr;
}

TLineErr FindLineFrom2Points(robPOINT P1, robPOINT P2, TLine* Line)
/*******************************************************************************
//Created: 12/02/96 S.R.
//Last Revision: 12/02/96 S.R.
//Parameters:
//	P1, P2: points that form the line.
//	Line:		holds the resulting line
//Return Value:
//	any errors that may occur.
********************************************************************************
//Description:
//		This function finds the line formed by the two points P1 and P2.
//	If P1 == P2, the function returns ERRNoSolution.
//Method:
//
*******************************************************************************/
{
    if ((P1.x == P2.x) && (P1.y == P2.y))
    {
        return ERRNoSolution;
    }

    if (P1.x == P2.x)
    {
        Line->Vertical = TRUE;
        Line->m = 0.0f;
        Line->b = P1.x;
    }
    else
    {
        Line->Vertical = FALSE;
        Line->m = (P1.y - P2.y) / (P1.x - P2.x);
        Line->b = P1.y - P1.x * Line->m;
    }

    return NoErr;
}

TLineErr FindPerpendicularLine(TLine Line, robPOINT P1, TLine* Perpendicular)
/*******************************************************************************
//	This function finds a line perpendicular to Line, and through the point P1.
*******************************************************************************/
{
    if (Line.Vertical)
    {
        Perpendicular->Vertical = FALSE;
        Perpendicular->m = 0.0;
        Perpendicular->b = P1.y;
    }
    else if (Line.m == 0.0)
    {
        Perpendicular->Vertical = TRUE;
        Perpendicular->m = 0.0;
        Perpendicular->b = P1.x;
    }
    else
    {
        Perpendicular->Vertical = FALSE;
        Perpendicular->m = -1.0 / Line.m;
        Perpendicular->b = P1.y - Perpendicular->m * P1.x;
    }

    return NoErr;
}

TLineErr FindLineFromPointAndSlope(robPOINT P1, double Slope, TLine* Line)
/*******************************************************************************
//Created: 12/02/96 S.R.
//Last Revision: 12/02/96 S.R.
//Parameters:
//	P1, P2: points that form the line.
//	Line:		holds the resulting line
//Return Value:
//	any errors that may occur.
********************************************************************************
//Description:
//		This function finds the line formed by the two points P1 and P2.
//	If P1 == P2, the function returns ERRNoSolution.
//Method:
//
*******************************************************************************/
{
    if (_isnan(Slope))
    {
        Line->Vertical = TRUE;
        Line->m = 0.0;
        Line->b = P1.x;
    }
    else
    {
        Line->Vertical = FALSE;
        Line->m = Slope;
        Line->b = -Slope * P1.x + P1.y;
    }

    return NoErr;
}

TLineErr IntersectionOfLines(TLine L1, TLine L2, robPOINT* Intersection)
/*******************************************************************************
//S.R. 07/10/96
//last revision: 07/12/96 S.R.
//Parameters:
//	L1,L2:			Two lines to find the intersection of.
//	Intersection:	Pointer to the variable to hold the intersection point.
//Return Value:
//	ERRParallelLines	If the two lines are parallel.
//	NoErr					If nothing out of the ordinary happens.
********************************************************************************
//Description:
//		This function finds the intersection of two non-parallel lines.
//Method:
//		First the function checks to see if both lines are vertical, in which case
//	there is no intersection point and ERRParallelLines is returned.  Next the
//	function takes care of the cases where one line is vertical.  Finally the
//	function finds the intersection for two non parallel lines.
*******************************************************************************/
{
    if ((L1.Vertical == TRUE) && (L2.Vertical == TRUE))
    {
        /*Two vertical lines*/
        return ERRParallelLines;
    }
    else if (L1.Vertical == TRUE)
    {
        /*L1 vertical*/
        Intersection->x = L1.b;
        Intersection->y = L2.m * Intersection->x + L2.b;
        return NoErr;
    }
    else if (L2.Vertical == TRUE)
    {
        /*L2 vertical*/
        Intersection->x = L2.b;
        Intersection->y = L1.m * Intersection->x + L1.b;
        return NoErr;
    }
    else if (L1.m != L2.m)
    {
        /*Two non parallel lines*/
        Intersection->x = -(L1.b - L2.b) / (L1.m - L2.m);
        Intersection->y = L1.b - L1.m * (L1.b - L2.b) / (L1.m - L2.m);
        return NoErr;
    }
    else
    {
        return ERRParallelLines;
    }
}

double PerpendicularDistanceFromLine(TLine L, POINT P)
/*******************************************************************************
//S.R. 09/05/96
//last revision: 09/05/96 S.R.
//Parameters:
//	L:			The line
//	P:	The point
//Return Value:
//	The perpendicular distance from P to L.
********************************************************************************
//Description:
//		This function calculates the perpendicular distance between the point P and
//	the line L.
//Method:
//		First the function takes care of the cases where L is horizontal and
//	vertical.  These are easy.  The non horizontal or vertical case has a complicated
// formula which was derived elsewhere, and applied here.  You can derive it
// yourself, but this function works.
*******************************************************************************/
{
    double temp;

    /*Take care of vertical lines*/
    if (L.Vertical == TRUE)
    {
        return ABS(P.x - L.b);
    }

    /*Take care of horizontal lines*/
    if (L.m == 0)
    {
        return ABS(P.y - L.b);
    }

    /*Take care of all other lines*/
    temp = SQR(P.y - L.m * P.x - L.b);
    temp += SQR(P.x + L.b / L.m - P.y / L.m);
    temp = sqrt(temp);
    temp /= ABS(L.m + 1 / L.m);
    return temp;
}

double PerpendicularDistanceFromLine(TLine L, robPOINT P)
/*******************************************************************************
//S.R. 07/10/96
//last revision: 07/12/96 S.R.
//Parameters:
//	L:	The line
//	P:	The point
//Return Value:
//	The perpendicular distance from P to L.
********************************************************************************
//Description:
//		This function calculates the perpendicular distance between the point P and
//	the line L.
//Method:
//		First the function takes care of the cases where L is horizontal and
//	vertical.  These are easy.  The non horizontal or vertical case has a complicated
// formula which was derived elsewhere, and applied here.  You can derive it
// yourself, but this function works.
*******************************************************************************/
{
    double temp;

    /*Take care of vertical lines*/
    if (L.Vertical == TRUE)
    {
        return ABS(P.x - L.b);
    }

    /*Take care of horizontal lines*/
    if (L.m == 0)
    {
        return ABS(P.y - L.b);
    }

    /*Take care of all other lines*/
    temp = SQR(P.y - L.m * P.x - L.b);
    temp += SQR(P.x + L.b / L.m - P.y / L.m);
    temp = sqrt(temp);
    temp /= ABS(L.m + 1 / L.m);
    return temp;
}

BOOL PointAboveOrOnLine(robPOINT P, TLine L)
/*******************************************************************************
//S.R. 07/17/96
//last revision: 07/17/96 S.R.
//Parameters:
//	P:		the Point
//	L:		the Line
//Return Value:
//	TRUE:		If P is on or above the line L
//	FALSE:	If P is below the line L
********************************************************************************
//Description:
//		This function is used to find if the point P is on or above the line L.
//	When L is vertical above is considered to be x values greater than the line's
//	x intercept.
//Method:
//		When the line L is vertical, if P.x >= L.b then the function returns TRUE.
//	Otherwise, if the y value of the point is greater than or equal to the y value
//	of the line at the point's x position then the point is above the line.
*******************************************************************************/
{
    if (L.Vertical == TRUE)
    {
        if (P.x >= L.b)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if (P.y >= (P.x * L.m + L.b))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

BOOL PointBelowOrOnLine(robPOINT P, TLine L)
/*******************************************************************************
//S.R. 07/17/96
//last revision: 07/17/96 S.R.
//Parameters:
//	P:		the Point
//	L:		the Line
//Return Value:
//	TRUE:		If P is on or below the line L
//	FALSE:	If P is above the line L
********************************************************************************
//Description:
//		This function is used to find if the point P is on or below the line L.
//	When L is vertical below is considered to be x values less than the line's
//	x intercept.
//Method:
//		When the line L is vertical, if P.x <= L.b then the function returns TRUE.
//	Otherwise, if the y value of the point is less than or equal to the y value
//	of the line at the point's x position then the point is below the line.
*******************************************************************************/
{
    if (L.Vertical == TRUE)
    {
        if (P.x <= L.b)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if (P.y <= (P.x * L.m + L.b))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

BOOL PointIsBetween(robPOINT PointBetween, robPOINT P2, robPOINT P3)
/*******************************************************************************
//S.R. 07/11/96
//last revision: 10/22/97 S.R.
//Parameters:
//	PointBetween,P2,P3:	Three colinear points
//Return Value:
//	TRUE if PointBetween is between P2 and P3
//	FALSE if PointBetween is not between P2 and P3.
********************************************************************************
//Description:
//	Sees if PointBetween is between P2,P3.
//Method:
//		If the direction in x or y traveled going from PointBetween to P2 is opposite in
//	sign to that traveled going from PointBetween to P3 then PointBetween is between P2 and P3.
//	Otherwise it isn't.  That's how this function figures out if PointBetween is between
//	P2 and P3.
*******************************************************************************/
{
    if (((PointBetween.x - P3.x) * (PointBetween.x - P2.x) <= 0) && ((PointBetween.y - P3.y) * (PointBetween.y - P2.y) <= 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL PointsOnSameSide(robPOINT P1, robPOINT P2, robPOINT P3)
/*******************************************************************************
//S.R. 07/16/96
//last revision: 07/16/96 S.R.
//Parameters:
//	P1,P2,P3:	3 colinear points
//Return Value:
//	TRUE:		P1,P2 on the same side of P3
//	FALSE:	P1,P2 on opposite sides of P3
********************************************************************************
//Description:
//		Given 3 colinear points P1,P2,P3 this function returns TRUE if P1 and P2
//	are on the same side of P3, and FALSE otherwise.
//Method:
//		The function sees if the direction in x or y from P1 to P3 is the same as
//	that from P2 to P3.  If it is then P1 and P2 are on the same side of P3.  If
//	not then the function returns FALSE.
*******************************************************************************/
{
    if (((P1.x - P3.x) * (P2.x - P3.x)) > 0 || ((P1.y - P3.y) * (P2.y - P3.y) > 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL TwoLinesEquivalent(TLine L1, TLine L2)
/*******************************************************************************
//S.R. 07/18/96
//last revision: 07/18/96 S.R.
//Parameters:
//	L1,L2:	Two lines whose equivalency is in question.
//Return Value:
//	TRUE:		L1 and L2 are equivalent.
//	FALSE:	L1 and L2 are not equivalent.
********************************************************************************
//Description:
//		This function checks to see if L1 and L2 are equivalent.
//Method:
//		First the function checks to see if one line is vertical while the other
//	is not.  If one line is vertical and the other is not then obviously L1 and
//	L2 are not equivalent.  Next the function takes care of the case when both
//	lines are vertical.  If they are, and both of the b values of the line are
//	the same then the two lines are equivalent.  Finally the function checks the
//	case when both lines are not vertical.  Here if both the m and b values of
//	the two lines are equal the lines are equivalent.
*******************************************************************************/
{
    if (L1.Vertical != L2.Vertical)
    {
        return FALSE;
    }

    if (L1.Vertical == TRUE)
    {
        if (L1.b == L2.b)
        {
            return TRUE;
        }
    }
    else
    {
        if ((L1.b == L2.b) && (L1.m == L2.m))
        {
            return TRUE;
        }
    }

    return FALSE;
}

double DotProduct(robPOINT P1, robPOINT P2)
/*******************************************************************************
//Created: 12/02/96 S.R.
//Last Revision: 12/02/96 S.R.
//Parameters:
//	P1, P2:	the points (vectors) to find the dot product of.
//Return Value:
//	The Dot Product of P1, P2.
********************************************************************************
//Description:
//		This function calculates and returns the dot product of P1, P2.
//Method:
//	Does the algebra.
*******************************************************************************/
{
    return (P1.x * P2.x + P1.y * P2.y);
}

robPOINT FindPerpendicularIntersection(TLine L, robPOINT P)
/*******************************************************************************
//Created: 12/02/96 S.R.
//Last Revision: 12/02/96 S.R.
//Parameters:
//	L:	line to find perpendicular intersection of.
//	P:	point from which to find perpendicular intersector
//Return Value:
//	the point that goes through P and intersects L perpendicularly.
********************************************************************************
//Description:
//		Finds the line that perpendicularly intersects L and goes through P.
//Method:
//
*******************************************************************************/
{
    robPOINT Intersection;

    if (L.Vertical == TRUE)
    {
        Intersection.x = L.b;
        Intersection.y = P.y;
        return Intersection;
    }

    if (L.m == 0)
    {
        Intersection.x = P.x;
        Intersection.y = L.b;
        return Intersection;
    }

    Intersection.x = ((P.y + P.x / L.m) - L.b) / (L.m + 1 / L.m);
    Intersection.y = L.m * Intersection.x + L.b;
    return Intersection;
}

double AbsAngularSeparation(double theta1, double theta2)
/*******************************************************************************
//Created: 08/16/96 S.R.
//Last Revision: 01/09/97 S.R.
//Parameters:
//	theta1, theta2:	the two angles to find the difference between.
//Return Value:
//	the absolute value of the angular separation in two angles.  Has a range [0,pi].
********************************************************************************
//Description:
//		This function finds the absolute value of the angular separation between
//	theta1 and theta2	measured in radians.  The returned value will be in the
//	range [0,pi]
//Method:
//	kind of obvious.
*******************************************************************************/
{
    theta1 = BringAngleInRange(theta1);
    //	fprintf(stderr, "theta1 = %f\n",theta1);
    theta2 = BringAngleInRange(theta2);
    //	fprintf(stderr, "theta2 = %f\n",theta2);
    double delta = BringAngleInRange(theta1 - theta2);

    //	fprintf(stderr, "delta = %f\n",delta);
    /*get rid of angles > pi*/
    if (delta > pi)
    {
        delta = 2 * pi - delta;
    }

    //	fprintf(stderr, "delta = %f\n",delta);
    return delta;
}

double BringAngleInRange(double theta)
/*******************************************************************************
//Created: 12/02/96 S.R.
//Last Revision: 12/02/96 S.R.
//Parameters:
//	theta:	angle to bring into range.
//Return Value:
//	the value of theta in the range [0, 2pi)
********************************************************************************
//Description:
//		Brins theta into the range [0,2pi)
//Method:
//
*******************************************************************************/
{
    /*get rid of negative angles*/
    if (theta < 0)
    {
        theta += ((long)(theta / (-2 * pi)) + 1) * 2 * pi;
    }

    /*get rid of angles >= 2pi*/
    if (theta >= 2 * pi)
    {
        theta -= ((long)(theta / (2 * pi))) * 2 * pi;
    }

    ASSERT(theta >= 0);
    ASSERT(theta <= 2 * pi);
    return theta;
}

double AngularDifference(double theta1, double theta2)
/*******************************************************************************
//Created: 12/02/96 S.R.
//Last Revision: 01/09/97 S.R.
//Parameters:
//	Finds the angular difference between theta1, theta2.  In range [0,2*pi)
//Return Value:
//	Finds theta2 - theta1.  In range [0,2*pi)
********************************************************************************
//Description:
//
//Method:
//
*******************************************************************************/
{
    theta1 = BringAngleInRange(theta1);
    theta2 = BringAngleInRange(theta2);
    return BringAngleInRange(theta2 - theta1);
}

long Round(double X)
/*******************************************************************************
//Created: 11/08/96 S.R.
//Last Revision: 11/08/96 S.R.
//Parameters:
//	X:	The floating point number to round to a long integer.
//Return Value:
//	The rounded value of X.
********************************************************************************
//Description:
//		The function rounds X to the nearest integer.
//Method:
//		The function finds the value to the right of the decimal point.  If this
//	is >= 0.5, the number is rounded up.  If not, the number is rounded down.
*******************************************************************************/
{
    long temp;

    if (X >= 0)
    {
        //		temp = floor(X);
        temp = (long)X;

        if ((X - temp) >= 0.5)
        {
            temp ++;
        }

        return temp;
    }
    else
    {
        X = -X;
        //		temp = floor(X);
        temp = (long)X;

        if ((X - temp) >= 0.5)
        {
            temp ++;
        }

        return -temp;
    }
}

robPOINT TransformToNonDisplacedCoordinates(TCoordSystem* DisplacedSystem, robPOINT DisplacedPoint)
/*******************************************************************************
//Created: 01/07/97 S.R.
//Last Revision: 01/16/97 S.R.
//Parameters:
//	DispalcedSystem:	A coordinate system displaced and rotated relative to another.
//	Point:				Point in the displaced system to find the non-displaced
//								coordinates of
//Return Value:
//	The coordinates of DisplacedPoint in the non-displaced coordinate system.
********************************************************************************
//Description:
//		This function takes the coordinates of a point in one coordinate system
//	that is offset and rotated relative to another, and finds the point's coordinates
//	in the non-displaced coordinate system.
//Method:
//	A trigonometric transform is used.
*******************************************************************************/
{
    if (DisplacedSystem == NULL)
    {
        return DisplacedPoint;
    }

    robPOINT Transform;
    Transform.x = DisplacedSystem->Origin.x + DisplacedPoint.x * cos(DisplacedSystem->Phi) - DisplacedPoint.y * sin(DisplacedSystem->Phi);
    Transform.y = DisplacedSystem->Origin.y + DisplacedPoint.x * sin(DisplacedSystem->Phi) + DisplacedPoint.y * cos(DisplacedSystem->Phi);
    return (Transform);
}

robPOINT TransformToDisplacedCoordinates(TCoordSystem *DisplacedSystem, robPOINT NonDisplacedPoint)
/*******************************************************************************
//Created: 01/16/97 S.R.
//Last Revision: 01/16/97 S.R.
//Parameters:
//	DispalcedSystem:	A coordinate system displaced and rotated relative to another.
//	Point:				Point in the non-displaced system to find the displaced
//								coordinates of
//Return Value:
//	The coordinates of NonDisplacedPoint in the displaced coordinate system.
********************************************************************************
//Description:
//		This function takes the coordinates of a point a non-displaced coordinate
//	system, and finds the point's coordinates in the displaced coordinate system.
//Method:
//	A trigonometric transform is used.
*******************************************************************************/
{
    if (DisplacedSystem == NULL)
    {
        return NonDisplacedPoint;
    }

    robPOINT Transform;
    Transform.x = (NonDisplacedPoint.x - DisplacedSystem->Origin.x) * cos(DisplacedSystem->Phi) +
                  (NonDisplacedPoint.y - DisplacedSystem->Origin.y) * sin(DisplacedSystem->Phi);
    Transform.y = -(NonDisplacedPoint.x - DisplacedSystem->Origin.x) * sin(DisplacedSystem->Phi) +
                  (NonDisplacedPoint.y - DisplacedSystem->Origin.y) * cos(DisplacedSystem->Phi);
    return (Transform);
}

double AngularBisector(double phi1, double phi2)
/*******************************************************************************
//Created: 01/09/97 S.R.
//Last Revision: 01/09/97 S.R.
//Parameters:
//	phi1, phi2:	Angles of the vectors to find the bisector of.
//Return Value:
//		The angle of the vector which bisects 2 vectors pointing at the angles
//	phi1 and phi2.
********************************************************************************
//Description:
//		Given the angular direction of 2 vectors (phi1 is the direction of V1,
//	phi2 is the direction of V2), this function will return the
//	angle of the vector which bisects those 2 vectors (V3).
//					V3
//			 V2	|	  V1
//			  \phi2	 /
//				\	|	/
//				 \	| /
//				  \|/	phi1
//					|--------------
//
//Method:
//
*******************************************************************************/
{
    phi1 = BringAngleInRange(phi1);
    phi2 = BringAngleInRange(phi2);

    if (ABS(phi1 - phi2) <= pi)
    {
        return BringAngleInRange((phi1 + phi2) / 2);
    }
    else
    {
        return BringAngleInRange((phi1 + phi2) / 2 - pi);
    }
}

double Average(double* Numbers, unsigned long nElements, double* std)
/*******************************************************************************
//Created: 01/10/97 S.R.
//Last Revision: 01/10/97 S.R.
//Parameters:
//	Numbers:	array of numbers to find average of
//	nElements:	number of elements in Numbers
//	std:			Standard deviation of list
//Return Value:
//	the average of Numbers.
********************************************************************************
//Description:
//		This function finds the average and standard deviation of a list of numbers.
//Method:
//
*******************************************************************************/
{
    double Avg = 0;

    for (unsigned long i = 0; i < nElements; i++)
    {
        Avg += Numbers[i];
    }

    Avg /= nElements;

    *std = 0;

    for (unsigned long i = 0; i < nElements; i++)
    {
        *std += SQR(Numbers[i] - Avg);
    }

    if (nElements > 1)
    {
        *std /= nElements - 1;
        *std = sqrt(*std);
    }
    else
    {
        *std = 0;
    }

    return Avg;
}

int Average(int* x, int n)
/*******************************************************************************
//Created: 02/11/97 S.R.
//Last Revision: 02/11/97 S.R.
//Parameters:
//	x:	list of numbers to find average of
//	n:	number of elements in x
//Return Value:
//	average of the list x
********************************************************************************
//Description:
//	Find the average of a list of numbers
//Method:
//
*******************************************************************************/
{
    if (n == 0)
    {
        return 0;
    }

    long Avg = 0;

    for (int i = 0; i < n; i++)
    {
        Avg += x[i];
    }

    Avg /= n;
    return Avg;
}

float Average(float* Numbers, unsigned long nElements, double* std)
/*******************************************************************************
//Created: 02/12/97 S.R.
//Last Revision: 02/12/97 S.R.
//Parameters:
//	Numbers:	array of numbers to find average of
//	nElements:	number of elements in Numbers
//	std:			Standard deviation of list
//Return Value:
//	the average of Numbers.
********************************************************************************
//Description:
//		This function finds the average and standard deviation of a list of numbers.
//Method:
//
*******************************************************************************/
{
    double Avg = 0;

    for (unsigned long i = 0; i < nElements; i++)
    {
        Avg += Numbers[i];
    }

    Avg /= nElements;

    *std = 0;

    for (unsigned long i = 0; i < nElements; i++)
    {
        *std += SQR(Numbers[i] - Avg);
    }

    if (nElements > 1)
    {
        *std /= nElements - 1;
        *std = sqrt(*std);
    }
    else
    {
        *std = 0;
    }

    return (float)Avg;
}

float Average(float* Numbers, unsigned long nElements)
/*******************************************************************************
//Created: 02/12/97 S.R.
//Last Revision: 02/12/97 S.R.
//Parameters:
//	Numbers:	array of numbers to find average of
//	nElements:	number of elements in Numbers
//Return Value:
//	the average of Numbers.
********************************************************************************
//Description:
//		This function finds the average of a list of numbers.
//Method:
//
*******************************************************************************/
{
    double Avg = 0;

    for (unsigned long i = 0; i < nElements; i++)
    {
        Avg += Numbers[i];
    }

    Avg /= nElements;
    return (float)Avg;
}

double Min(double* x, unsigned int n)
/*******************************************************************************
//Created: 02/25/97 S.R.
//Last Revision: 02/25/97 S.R.
//Parameters:
//	x:	array of numbers
//	n:	number of elements in array
//Return Value:
//	minimum value in the list x
********************************************************************************
//Description:
//	finds the minimum value in a list of numbers.
//Method:
//
*******************************************************************************/
{
    if (n == 0)
    {
        return 1.7E308;
    }

    double min = x[0];

    for (unsigned int i = 1; i < n; i++)
    {
        if (x[i] < min)
        {
            min = x[i];
        }
    }

    return min;
}

double Max(double* x, unsigned int n)
/*******************************************************************************
//Created: 02/25/97 S.R.
//Last Revision: 02/25/97 S.R.
//Parameters:
//	x:	array of numbers
//	n:	number of elements in array
//Return Value:
//	maximum value in the list x
********************************************************************************
//Description:
//	finds the maximum value in a list of numbers.
//Method:
//
*******************************************************************************/
{
    if (n == 0)
    {
        return -1.7E308;
    }

    double max = x[0];

    for (unsigned int i = 1; i < n; i++)
    {
        if (x[i] > max)
        {
            max = x[i];
        }
    }

    return max;
}

double Average(int* Numbers, unsigned long nElements, double* std)
/*******************************************************************************
//Created: 02/28/97 S.R.
//Last Revision: 02/28/97 S.R.
//Parameters:
//	Numbers:	array of numbers to find average of
//	nElements:	number of elements in Numbers
//	std:			Standard deviation of list
//Return Value:
//	the average of Numbers.
********************************************************************************
//Description:
//		This function finds the average and standard deviation of a list of numbers.
//Method:
//
*******************************************************************************/
{
    double Avg = 0;

    for (unsigned long i = 0; i < nElements; i++)
    {
        Avg += Numbers[i];
    }

    Avg /= nElements;

    *std = 0;

    for (unsigned long i = 0; i < nElements; i++)
    {
        *std += SQR(Numbers[i] - Avg);
    }

    if (nElements > 1)
    {
        *std /= nElements - 1;
        *std = sqrt(*std);
    }
    else
    {
        *std = 0;
    }

    return Avg;
}

void FindCircle(robPOINT P1, robPOINT P2, robPOINT P3, robPOINT* Center, double* radius)
/*******************************************************************************
//Created: 10/21/97 S.R.
//Last Revision: 10/21/97 S.R.
//Parameters:
//	P1, P2, P3:	points on the circle
//	Center:		point at center of the circle
//	radius:		radius of the circle
//Return Value:
//	none.
********************************************************************************
//Description:
//		This function finds the circle that goes through the three points P1, P2, P3.
//	It returns the center and radius of the circle.  If any two points are the
//	same then the function returns a circle of radius 0 centered at the origin.
//Method:
//
*******************************************************************************/
{
    if (((P1.x == P2.x) && (P1.y == P2.y)) ||
            ((P1.x == P3.x) && (P1.y == P3.y)) ||
            ((P3.x == P2.x) && (P3.y == P2.y)))
    {
        Center->x = 0;
        Center->y = 0;
        *radius = 0;
        return;
    }

    Center->x = P1.y * (SQR(P2.x) - SQR(P3.x) + SQR(P2.y) - SQR(P3.y));
    Center->x += P2.y * (-SQR(P1.x) + SQR(P3.x) - SQR(P1.y) + SQR(P3.y));
    Center->x += P3.y * (-SQR(P2.x) + SQR(P1.x) + SQR(P1.y) - SQR(P2.y));
    Center->x /= 2 * (P1.y * (P2.x - P3.x) + P2.y * (-P1.x + P3.x) + P3.y * (P1.x - P2.x));
    Center->y = SQR(P2.x) - SQR(P3.x) + SQR(P2.y) - SQR(P3.y) - 2 * Center->x * (P2.x - P3.x);
    Center->y /= 2 * (P2.y - P3.y);
    *radius = sqrt(SQR(P1.x - Center->x) + SQR(P1.y - Center->y));
    return;
}

void robRECT::Normalize(void)
{
    double temp;

    if (left > right)
    {
        temp = left;
        left = right;
        right = temp;
    }

    if (top < bottom)
    {
        temp = top;
        top = bottom;
        bottom = temp;
    }
}

void robRECT::InflateRect(double x, double y)
/*Note!!!!! rectangle must be normalized before performing this operation*/
{
    left -= x;
    right += x;
    top += y;
    bottom -= y;
}

void robRECT::Include(robPOINT point)
/*Note!!!!! rectangle must be normalized before performing this operation*/
{
    if (point.x < left)
    {
        left = point.x;
    }
    else if (point.x > right)
    {
        right = point.x;
    }

    if (point.y > top)
    {
        top = point.y;
    }
    else if (point.y < bottom)
    {
        bottom = point.y;
    }
}

double Max(double x1, double x2)
{
    if (x1 >= x2)
    {
        return x1;
    }

    return x2;
}

bool robRECT::Inside(robPOINT point)
{
    return ((point.x >= left) && (point.x <= right) && (point.y <= top) && (point.y >= bottom));
}

robPOINT robRECT::TopLeft()
{
    robPOINT TopLeft;
    TopLeft.x = left;
    TopLeft.y = top;
    return TopLeft;
}

robPOINT robRECT::BottomRight()
{
    robPOINT BottomRight;
    BottomRight.x = right;
    BottomRight.y = bottom;
    return BottomRight;
}

int RobotHeadingToBitmapFrame(double Heading, int TotalFrames)
{
    int Frame;
    double FrameHeading = Heading - 2 * pi / TotalFrames / 2;
    FrameHeading = BringAngleInRange(FrameHeading);

    if (FrameHeading <= 3 * pi / 2)
    {
        Frame = int(-20 / pi * FrameHeading + 30);
    }
    else
    {
        const float X1 = float(3 * pi / 2 + 2 * pi / TotalFrames);
        const float X2 = float(2 * pi - 2 * pi / TotalFrames);
        const float m = 8 / (X1 - X2);
        Frame = int(m * FrameHeading - m * X1 + TotalFrames - 1);
    }

    if (Frame > TotalFrames - 1)
    {
        Frame = 0;
    }

    return Frame;
}

bool robPOINT::operator==(robPOINT P2)
{
    return ((x == P2.x) && (y == P2.y));
}

void robRECT::Union(robRECT Rect)
{
    Include(Rect.TopLeft());
    Include(Rect.BottomRight());
}

bool robPOINT::operator !=(robPOINT P2)
{
    return ((x != P2.x) || (y != P2.y));
}

double robRECT::Height()
{
    return top - bottom;
}

double robRECT::Width()
{
    return right - left;
}
