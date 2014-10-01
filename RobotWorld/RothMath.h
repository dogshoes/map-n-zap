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
 
 #ifndef ROTHMATH02938576949294856
 
 #define ROTHMATH02938576949294856
 
 #include <math.h>
 
 enum TLineErr{NoErr, ERRParallelLines, ERRSameLine, ERRNoSolution};
 
 struct TLine {
 	double m;
 	double b;
 	bool Vertical;
 };
 
 class robPOINT {
 public:
 	robPOINT() {x = 0, y = 0;};
 	robPOINT(double xval, double yval) {x = xval, y = yval;};
 	bool operator !=(robPOINT P2);
 	bool operator==(robPOINT P2);
 	double x;
 	double y;
 };
 
 class robRECT {
 public:
 	double Width(void);
 	double Height(void);
 	void Union(robRECT Rect);
 	robPOINT BottomRight(void);
 	robPOINT TopLeft(void);
 	bool Inside(robPOINT point);
 	double left;
 	double top;
 	double right;
 	double bottom;
 	void Normalize(void);
 	void Include(robPOINT point);
 	void InflateRect(double x, double y);
 };
 
 struct TCoordSystem {
 	robPOINT Origin;
 	double Phi;
 };
 
 #define SQR(x) ((x)*(x))
 #define ABS(x) ((x)>=0?(x):-(x))
 #define MAX(x,y) ((x)>=(y)?(x):(y))
 #define MIN(x,y) ((x)<=(y)?(x):(y))
 #define DegreeToRadian(x) (x*pi/180.0)
 #define RadianToDegree(x) (x*180.0/pi)
 
 //const double pi = 3.14159;
 const double pi = atan2(0,-1);
 
 double sqr(double x);
 double XPROD(robPOINT V1, robPOINT V2);
 double Distance(POINT P1, POINT P2);
 double Distance(robPOINT P1, robPOINT P2);
 TLineErr FindBisectorsOfLines(TLine* L1, TLine* L2);
 TLineErr FindLineFrom2Points(POINT P1, POINT P2, TLine* Line);
 TLineErr FindLineFrom2Points(robPOINT P1, robPOINT P2, TLine* Line);
 TLineErr FindLineFromPointAndSlope(robPOINT P1, double Slope, TLine* Line);
 TLineErr FindPerpendicularLine(TLine Line, robPOINT P1, TLine* Perpendicular);
 TLineErr IntersectionOfLines(TLine L1, TLine L2, robPOINT* Intersection);
 robPOINT FindPerpendicularIntersection(TLine L, robPOINT P);
 double PerpendicularDistanceFromLine(TLine L, POINT P);
 double PerpendicularDistanceFromLine(TLine L, robPOINT P);
 BOOL PointAboveOrOnLine(robPOINT P, TLine L);
 BOOL PointBelowOrOnLine(robPOINT P, TLine L);
 BOOL PointIsBetween(robPOINT PointBetween, robPOINT P2, robPOINT P3);
 BOOL PointsOnSameSide(robPOINT P1,robPOINT P2,robPOINT P3);
 BOOL TwoLinesEquivalent(TLine L1, TLine L2);
 double DotProduct(robPOINT P1, robPOINT P2);
 double AbsAngularSeparation(double theta1, double theta2);
 double BringAngleInRange(double theta);
 double AngularDifference(double theta1, double theta2);
 long Round(double X);
 robPOINT TransformToNonDisplacedCoordinates( TCoordSystem* DisplacedSystem, robPOINT DisplacedPoint);
 robPOINT TransformToDisplacedCoordinates( TCoordSystem *DisplacedSystem, robPOINT NonDisplacedPoint);
 double AngularBisector( double phi1, double phi2 );
 int Average(int* x, int n);
 double Average(int* Numbers, unsigned long nElements, double* std);
 double Average( double* Numbers, unsigned long nElements, double* std);
 float Average( float* Numbers, unsigned long nElements, double* std);
 float Average( float* Numbers, unsigned long nElements);
 double Min(double* x, unsigned int n);
 double Max(double* x, unsigned int n);
 void FindCircle(robPOINT P1, robPOINT P2, robPOINT P3, robPOINT* Center, double* radius);
 double Max(double x1, double x2);
 int RobotHeadingToBitmapFrame(double Heading, int TotalFrames);
 
 #endif