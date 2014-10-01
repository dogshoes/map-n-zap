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
 
 
 #ifndef PATH_H
 #define PATH_H
 
 #include <stdafx.h>
 #include <afxcoll.h>
 #include <math.h>
 #include "World.h"
 
 // extern "C" {
 // #include "list.h"
 // }
 
 class Path;
 
 // Note: Quality distances are measures in world units not grid units
 enum QualityMeasure {MinFound, AveFound, StartDist, EndDist, FreeDist};
 
 class Segment : public CObject {
 
 //	DECLARE_SERIAL(Segment);
   private:
     int state;		// The segment state (tells if quality is set)
     int start[2];	// The start of this segment as a grid point
     int end[2];		// The end of the segment
     Path *path;		// The path it belongs to
     double length;	// The length of this segment
     double dir[2];	// Direction
     double quality[5];	// The openness of this segment;
     double CalculatePathLength();
   public:
     Segment(Path *path);
     Segment(Path *path, int x1, int y1, int x2, int y2);
     ~Segment() {;};
     void SetStart(int x, int y);
     void SetEnd(int x, int y);
     void GetStart(int &x, int &y) {x = start[0]; y = start[1];};
     void GetEnd(int &x, int &y) {x = end[0]; y = end[1];};
     double GetGridLength() {return length;};
     double GetWorldLength();
     void GetDirection(double &dx, double &dy) {dx = dir[0];  dy = dir[1];};
     double GetQuality(QualityMeasure i, int recalculate=0);
     double *GetQualities(int recalculate=0) {
 	GetQuality(MinFound,recalculate); return quality;};
     Segment *SplitSegment(double prop);
     Segment *CopySegment(Path *path);
     void Reverse();
 //	void Serialize(CArchive &ar);
 };
 
 
 
 class Path {
   private:
     CObList *segments;	// The segments making up the path
     World *world;	// The world the path goes through
     double min_length;	// Minimum path length from start to finish (world)
     double CalculateMinLength();
   public:
     Path(World *world, Segment *segment=NULL);
     Path(World *world, double x1, double y1, double x2, double y2);
     Path(Path &p);
     ~Path();
     Segment *GetSegment(int i);
 //    int GetNumberOfSegments() {return ListSize(segments);};
     int GetNumberOfSegments() {return segments->GetCount();};
     double GetGridLength();			// Total of segments
     double GetWorldLength();			// Total of segments
     void GetQualities(double qualities[5], int recalculate=0);  /* Quality for
 								   all segments
 								   */
     Segment *SplitSegment(int i, double prop);  // Split ith segment in list
     int JoinSegment(int i);			// Joins segment i and i+1
     int MoveJoint(int i, double x, double y, int end=1); //Move joint btwn seg.
     int MoveSegmentJoint(int i, int x, int y, int end=1); //Move joint btwn seg
     Path *CopyPath();
     World *GetWorld() {return world;};
     int IsEqual(Path &p);
 						// path_length in world units
     int FindSegment(double path_length, int &seg_num, double &prop);
     double MinLength() {return min_length;};
     void Reverse();
     Segment *AppendPoint(double x1, double y1);		// Add at end
     Segment *InsertPoint(int pos, double x1, double y1); // Add before pos
     int DeletePoint(int pos);
 //    int GetNumberOfPoints() {return ListSize(segments) + 1;};
     int GetNumberOfPoints() {return segments->GetCount() + 1;};
     int GetPoint(int pos, double &x1, double &y1);
     int SetPoint(int pos, double x1, double y1);
     int MovePoint(int pos, double x1, double y1);
 };
 
 
 inline double Segment::GetWorldLength() \
 	{return length*((path->GetWorld())->GridSize());};
 
 inline double Path::GetWorldLength() \
 	{return GetGridLength()*((GetWorld())->GridSize());};
 
 
 #endif /*PATH_H*/
