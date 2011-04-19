//
//  BoundarySegment.h
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/24/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#ifndef __BOUNDARY_SEGMENT_H_INCLUDED__
#define __BOUNDARY_SEGMENT_H_INCLUDED__

#include "Point2.h"

class BoundarySegment
{
public:
    virtual ~BoundarySegment() {}
    virtual Point2 midpoint(Point2 const& p1, Point2 const& p2 ) = 0;
};


class LineBoundarySegment : public BoundarySegment
{
public:
    LineBoundarySegment(Point2 const& p1, Point2 const& p2 ) : p1_(p1), p2_(p2) {}
    Point2 midpoint(Point2 const& p1, Point2 const& p2);
    
private:
    Point2 p1_, p2_;
};


class CircleBoundarySegment : public BoundarySegment
{
public:
    CircleBoundarySegment(Point2 const& p1, Point2 const& p2, Point2 const& p3);
    Point2 midpoint(Point2 const& p1, Point2 const& p2);
    
private:
    Point2 p1_, p2_, p3_;
    Point2 center_;
    double radius_;
};

#endif // __BOUNDARY_SEGMENT_H_INCLUDED__