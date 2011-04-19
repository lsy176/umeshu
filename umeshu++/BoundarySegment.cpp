//
//  BoundarySegment.cpp
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/24/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#include "BoundarySegment.h"
#include "FastKernel.h"

Point2 LineBoundarySegment::midpoint( const Point2& p1, const Point2& p2 )
{
    return Point2(0.5*(p1.x()+p2.x()), 0.5*(p1.y()+p2.y()));
}

CircleBoundarySegment::CircleBoundarySegment(Point2 const& p1, Point2 const& p2, Point2 const& p3 )
: p1_(p1), p2_(p2), p3_(p3)
{
	center_ = FastKernel::circumcenter(p1_, p2_, p3_);
    radius_ = circumradius(p1_, p2_, p3_);
}

Point2 CircleBoundarySegment::midpoint(Point2 const& p1, Point2 const& p2)
{
    Point2 p(0.5*(p1.x()+p2.x()), 0.5*(p1.y()+p2.y()));
    double d = distance(p, center_);
    p.x() = (p.x() - center_.x())/d;
    p.y() = (p.y() - center_.y())/d;
    p.x() = center_.x()+radius_*p.x();
    p.y() = center_.y()+radius_*p.y();
    return p;
}

