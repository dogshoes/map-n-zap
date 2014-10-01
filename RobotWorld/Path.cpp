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
  
  #include <stdafx.h>
  #include "World.h"
  #include "Path.h"
  
  
  ////////////////////////// Segment //////////////////////
  
  Segment::Segment(Path *p) 
  {
      state = 0;
      start[0] = start[1] = end[0] = end[1] = 0;
      path = p;
      length = 0.000001;
      quality[MinFound] = quality[AveFound] = 0;
      quality[StartDist] = quality[EndDist] = quality[FreeDist] = -1;
  }
  
  
  Segment::Segment(Path *p, int x1, int y1, int x2, int y2)
  {
      state = 0;
      start[0] = x1;
      start[1] = y1;
      end[0] = x2;
      end[1] = y2;
      path = p;
      
      CalculatePathLength();
      quality[MinFound] = quality[AveFound] = 0;
      quality[StartDist] = quality[EndDist] = quality[FreeDist] = -1;
  }
  
  
  double Segment::CalculatePathLength()
  {
      int dx = end[0] - start[0];
      int dy = end[1] - start[1];
      
      length = sqrt((double)(dx*dx + dy*dy));
      if (length) {
  	dir[0] = dx/length;
  	dir[1] = dy/length;
      }
      else {
  	dir[0] = dir[1] = 0.0;
  	length = 0.000001;
      }
  
      return length;
  }
  
  
  
  unsigned char Streak(World &w, int r1, int col1, int r2, int col2, 
  		     Grid &mask, double step_size, double stats[5],
  		     unsigned char open_value, int break_off);
  
  double Segment::GetQuality(QualityMeasure i, int recalculate) 
  {
      if (!state || recalculate) {
  	World *world = this->path->GetWorld();
  
  	world->rover.SetLocation(0, 0, atan2((double)(end[1] - start[1]),
  					     (double)(end[0] - start[0])));
  	world->rover.SetLocationDelta(0, 0, 0);
  
  	Grid *clear = world->rover.PathGrid(world->GridSize());
  	Streak(*world, this->start[0], this->start[1],
  	       this->end[0], this->end[1], *clear,
  	       world->rover.PathStepSize(), quality, world->OpenValue(), 0);
  	delete clear;
  				 
  	state = 1;
      }
  
      return quality[i];
  }
  
  
  void Segment::SetStart(int x, int y)
  {
      start[0] = x;
      start[1] = y;
      CalculatePathLength();
      state = 0;
  }
  
  
  void Segment::SetEnd(int x, int y)
  {
      end[0] = x;
      end[1] = y;
      CalculatePathLength();
      state = 0;
  }
  
  
  Segment *Segment::SplitSegment(double prop)
  {
      // Check for illegal prop
      if ((prop < 0) || (prop > 1.0))
  	return NULL;
  
      int new_x = (int)(start[0] + prop*(end[0] - start[0]) + 0.5);
      int new_y = (int)(start[1] + prop*(end[1] - start[1]) + 0.5);
  
      Segment *seg = new Segment(path, new_x, new_y, end[0], end[1]);
  
      SetEnd(new_x, new_y);
      return seg;
  }
  
  
  
  Segment *Segment::CopySegment(Path *p)
  {
      Segment *new_seg = new Segment(p, start[0], start[1], end[0], end[1]);
      new_seg->state = state;
      new_seg->length = length;
  
      int i;
      for (i = 0; i < 5; i++)
        new_seg->quality[i] = quality[i];
  
      return new_seg;
  }
  
  
  void Segment::Reverse()
  {
      int x = start[0];
      int y = start[1];
  
      start[0] = end[0];
      start[1] = end[1];
      end[0] = x;
      end[1] = y;
      dir[0] = -dir[0];
      dir[1] = -dir[1];
      
      double q = quality[StartDist];
      quality[StartDist] = quality[EndDist];
      quality[EndDist] = q;
  }
  
  
  ////////////////////////// Path //////////////////////
  
  Path::Path(World *w, Segment *seg)
  {
  //    segments = InitList();
  	segments = new CObList(1);
      world = w;
      min_length = 0;
  
      if (seg) {
  	Segment *new_seg = seg->CopySegment(this);
  //	ApdItem(segments, (Item)new_seg);
  	segments->AddTail(new_seg);
  	min_length = seg->GetWorldLength();
      }
  }
  
  
  Path::Path(World *w, double x1, double y1, double x2, double y2)
  {
  //    segments = InitList();
  	segments = new CObList(1);
      world = w;
      min_length = 0;
  
      int r1, c1, r2, c2;
  
      if (world) {
  	world->WorldToGrid(x1, y1, r1, c1);
  	world->WorldToGrid(x2, y2, r2, c2);
      }
      else {
  	r1 = (int)x1;  c1 = (int)y1;
  	r2 = (int)x2;  c2 = (int)y2;
      }
  
      Segment *new_seg = new Segment(this, r1, c1, r2, c2);
  	segments->AddTail(new_seg);
  //    ApdItem(segments, (Item)new_seg);
      min_length = new_seg->GetWorldLength();
  }
  
  
  
  Path::Path(Path &p)
  {
  //    MARK mark;
      Segment *seg;
  	int i;
  
  //    segments = InitList();
  	segments = new CObList(1);
      world = p.world;
      min_length= p.min_length;
  
  //    FOR_LIST_ITEMS(p.segments, mark, seg)
  //	ApdItem(segments, (Item)seg->CopySegment(this));
  	for (i = 0; i < p.segments->GetCount(); i++)
  	{
  		seg = ((Segment *)(p.segments->GetAt(p.segments->FindIndex(i))))->CopySegment(this);
  		segments->AddTail(seg);
  	}
  }
  
  
  
  Path::~Path()
  {
      Segment *seg;
  
  //    while ((seg = (Segment *)DelHeadItem(segments)) != NULL)
  	while (!segments->IsEmpty())
  	{
  		seg = (Segment *)(segments->RemoveHead());
  		delete seg;
  	}
  	delete segments;
  
  //	delete seg;
  //    FreeList(segments);
  
      world = NULL;
      segments = NULL;
      min_length = 0;
  }
  
  
  
  Segment *Path::GetSegment(int i)
  {
      if (i < 0 || i >= segments->GetCount())
  	return NULL;
  
  //    return (Segment *)ItemAtSite(segments, i);
  	return (Segment *)(segments->GetAt(segments->FindIndex(i)));
  }
  
  
  
  double Path::GetGridLength()
  {
      double length = 0;
  //    MARK mark;
      Segment *seg;
  	int i;
  
  //    FOR_LIST_ITEMS(segments, mark, seg)
  	for (i = 0; i < GetNumberOfSegments(); i++)
  	{
  		seg = GetSegment(i);
  		length += seg->GetGridLength();
  	}
  
      return length;
  }
  
  
  
  
  void Path::GetQualities(double qualities[5], int recalculate)
  {
  //    MARK mark;
      Segment *seg;
      double *seg_qualities;
  	int i;
  
      qualities[MinFound] = 255;
      qualities[AveFound] = 0;
      qualities[StartDist] = 0;
      qualities[EndDist] = 0;
      qualities[FreeDist] = 0;
  
  //    FOR_LIST_ITEMS(segments, mark, seg) 
  	for (i = 0; i < GetNumberOfSegments(); i++)
  	{
  		seg = GetSegment(i);
  		seg_qualities = seg->GetQualities(recalculate); 
  		if (seg_qualities[MinFound] < qualities[MinFound])
  			qualities[MinFound] = seg_qualities[MinFound];
  		qualities[AveFound] += seg_qualities[AveFound] * seg->GetGridLength();
  		qualities[StartDist] += seg_qualities[StartDist];
  		qualities[EndDist] += seg_qualities[EndDist];
  		qualities[FreeDist] += seg_qualities[FreeDist];
      }
      qualities[AveFound] /= GetGridLength();
  }
  
  
  
  Segment *Path::SplitSegment(int i, double prop)
  {
      Segment *seg = GetSegment(i);
      if (seg == NULL)
  	return NULL;
  
      Segment *new_seg = seg->SplitSegment(prop);
      if (new_seg == NULL)
  	return NULL;
  
  	segments->InsertAfter(segments->FindIndex(i), new_seg);
  //    ApdAtSite(segments, i, (Item)new_seg);
      return new_seg;
  }
  
  
  
  int Path::JoinSegment(int i)
  {
      Segment *seg = GetSegment(i);
      Segment *seg2 = GetSegment(i+1);
  
      if (!seg || !seg2)
  	return 0;
  
      int x2, y2;
      seg2->GetEnd(x2, y2);
      seg->SetEnd(x2, y2);
  
      segments->RemoveAt(segments->FindIndex(i+1));
  //    DelAtItem(segments, seg2);
  
      delete seg2;
      return 1;
  }
  
  
  
  int Path::MoveJoint(int i, double x, double y, int end)
  {
      int r, c;
      if (world) {
  	world->WorldToGrid(x, y, r, c);
      }
      else {
  	r = (int)x;  c = (int)y;
      }
  
      return MoveSegmentJoint(i, r, c);
  }
  
  
  
  int Path::MoveSegmentJoint(int i, int r, int c, int end)
  {
      Segment *seg1=GetSegment(i);
      Segment *seg2=GetSegment(i + (end ? 1 : -1));
      
      if (!seg1 && !seg2)
  	return 0;
  
      if (end) {
  	if (seg1)
  	    seg1->SetEnd(r, c);
  	if (seg2)
  	    seg2->SetStart(r, c);
      } else {
  	if (seg1)
  	    seg1->SetStart(r, c);
  	if (seg2)
  	    seg2->SetEnd(r, c);
      }
      return 1;
  }
  
  
  
  
  Path *Path::CopyPath()
  {
      Path *new_path = new Path(*this);
      return new_path;
  }
  
  
  
  int Path::IsEqual(Path &p)
  {
      int i, num_segs;
      Segment *seg1, *seg2;
      int x1_1, y1_1, x2_1, y2_1;
      int x1_2, y1_2, x2_2, y2_2;
  
      if ((num_segs = segments->GetCount()) != p.segments->GetCount())
  	return 0;
  
      for(i = 0; i < num_segs; i++) {
  	seg1 = GetSegment(i);
  	seg2 = p.GetSegment(i);
  	seg1->GetStart(x1_1, y1_1);
  	seg1->GetEnd(x2_1, y2_1);
  	seg2->GetStart(x1_2, y1_2);
  	seg2->GetEnd(x2_2, y2_2);
  
  	if ((x1_1 != x1_2) || (y1_1 != y1_2)
  	    || (x2_1 != x2_2) || (y2_1 != y2_2))
  	    return 0;
      }
  
      return 1;
  }
  
  
  
  int Path::FindSegment(double path_length, int &seg_num, double &prop)
  {
      double wld_length = GetWorldLength();
  
      if (path_length < 0 || path_length > wld_length)
  	return 0;
  
      int i, num_segs=GetNumberOfSegments();
      double total_length = 0.0;
      Segment *seg;
  
      // Switch to grid since less mulitplications;
      path_length /= (GetWorld())->GridSize();
      for (i = 0; i < num_segs; i++) {
  	seg = GetSegment(i);
  	if ((total_length + seg->GetGridLength()) >= path_length) {
  	    seg_num = i;
  	    if (seg->GetGridLength())
  		prop = (path_length - total_length)/seg->GetGridLength();
  	    else
  		prop = 0;
  	    return 1;
  	}
  	total_length += seg->GetGridLength();
      }
  	
      return 0;
  }
  
  
  
  void Path::Reverse()
  {
  //    MARK mark;
      Segment *seg;
  	CObList newlist;
  	int i;
  
  //    ReverseList(segments);
  //    FOR_LIST_ITEMS(segments, mark, seg) {
  //	seg->Reverse();
  //    }
  
  	for (i = 0; i < GetNumberOfSegments(); i++)
  	{
  		seg = GetSegment(i);
  		newlist.AddHead(seg);
  	}
  	segments->RemoveAll();
  
  	segments->AddTail(&newlist);
  }
  
  
  
  Segment *Path::AppendPoint(double x, double y)
  {
      int r1, c1, r2, c2;
      if (world) {
  	world->WorldToGrid(x, y, r2, c2);
      }
      else {
  	r2 = (int)x;  c2 = (int)y;
      }
      
      Segment *seg = GetSegment(GetNumberOfSegments() - 1);
      seg->GetEnd(r1, c1);
  
      Segment *new_seg = new Segment(this, r1, c1, r2, c2);
  	segments->AddTail(new_seg);
  //    ApdItem(segments, (Item)new_seg);
      CalculateMinLength();
      return new_seg;
  }
  
  
  
  Segment *Path::InsertPoint(int i, double x, double y)
  {
      Segment *new_seg;
  
      if (i != 0) {
  	new_seg = SplitSegment(i-1, 0.5);
  
  	if (!new_seg)
  	    return (Segment *)NULL;
  
  	MoveJoint(i-1, x, y);
  	return new_seg;
      }
  
      new_seg = SplitSegment(0, 0.0);
      Segment *seg = GetSegment(0);
  
      int r1, c1;
      if (world) {
  	world->WorldToGrid(x, y, r1, c1);
      }
      else {
  	r1 = (int)x;  c1 = (int)y;
      }
      seg->SetStart(r1, c1);
  
      CalculateMinLength();    
      return seg;
  }
  
  
  
  
  int Path::DeletePoint(int i)
  {
      int num_segs = GetNumberOfSegments();
      
      if ((num_segs == 1) || (i < 0) || (i > num_segs))
  	return 0;
  
      if ((i != 0) && (i != num_segs)) {
  	JoinSegment(i-1);
  	return 1;
      }
  	
  
      Segment *seg;
      if (i == 0) {
  //	seg = (Segment *)DelHeadItem(segments);
  		seg = (Segment *)segments->RemoveHead();
      }
      else {
  //	seg = (Segment *)DelTailItem(segments);
  		seg = (Segment *)segments->RemoveTail();
      }
      delete seg;
  
      CalculateMinLength();
      return 1;
  }
  
  
  
  int Path::GetPoint(int pos, double &x1, double &y1)
  {
      int num_segs = GetNumberOfSegments();
  
      if ((pos < 0) || (pos > num_segs))
  	return 0;
  
      Segment *seg;
      seg = ((pos == num_segs) ? GetSegment(num_segs - 1) : GetSegment(pos));
  
      int r1, c1;
      if (pos == num_segs)
  	seg->GetEnd(r1, c1);
      else
  	seg->GetStart(r1, c1);
  
      world->GridToWorld(r1, c1, x1, y1);
      return 1;
  }
  
  
  
  int Path::SetPoint(int pos, double x1, double y1)
  {
      int num_segs = GetNumberOfSegments();
  
      if ((pos < 0) || (pos > num_segs))
  	return 0;
  
      Segment *seg;
      seg = ((pos == num_segs) ? GetSegment(num_segs - 1) : GetSegment(pos));
  
      int r1, c1;
      world->WorldToGrid(x1, y1, r1, c1);
  
      if (pos == num_segs)
  	seg->SetEnd(r1, c1);
      else
  	seg->SetStart(r1, c1);
  
      if ((pos == 0) || (pos == num_segs))
  	CalculateMinLength();
  
      return 1;
  }
  
  
  
  
  int Path::MovePoint(int pos, double x1, double y1)
  {
      int num_segs = GetNumberOfSegments();
  
      if ((pos < 0) || (pos > num_segs))
  	return 0;
  
  
      if ((pos != 0) && (pos != num_segs)) {
  	MoveJoint(pos-1, x1, y1);
  	return 1;
      }
  
      int r1, c1;
      world->WorldToGrid(x1, y1, r1, c1);
      
      Segment *seg;
      seg = ((pos == num_segs) ? GetSegment(num_segs - 1) : GetSegment(0));
  
      if (pos == num_segs)
  	seg->SetEnd(r1, c1);
      else
  	seg->SetStart(r1, c1);
  
  
      CalculateMinLength();
      return 1;
  }
  
  
  
  double Path::CalculateMinLength()
  {
      int r1, c1, r2, c2;
      Segment *seg;
  
      seg = GetSegment(0);
      seg->GetStart(r1, c1);
  
      seg = GetSegment(GetNumberOfSegments() - 1);
      seg->GetEnd(r2, c2);
  
      min_length = world->GridSize() 
  	* sqrt((double)((r2 - r1)*(r2 - r1) + (c2 - c1)*(c2 - c1)));
  
      return min_length;
  }
