//
//  BoundingBox.cpp
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/30/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#include "BoundingBox.h"

#include <ostream>
#include <limits>

BoundingBox::BoundingBox()
: ll_(std::numeric_limits<double>::max(),std::numeric_limits<double>::max()),
  ur_(-std::numeric_limits<double>::max(),-std::numeric_limits<double>::max())
{}

BoundingBox::BoundingBox(Point2 const& p1, Point2 const& p2)
: ll_(std::numeric_limits<double>::max(),std::numeric_limits<double>::max()),
  ur_(-std::numeric_limits<double>::max(),-std::numeric_limits<double>::max())
{
    include(p1);
    include(p2);
}

void BoundingBox::include(Point2 const& p)
{
    if (p.x() < ll_.x())
        ll_.x() = p.x();
    if (p.x() > ur_.x())
        ur_.x() = p.x();
    if (p.y() < ll_.y())
        ll_.y() = p.y();
    if (p.y() > ur_.y())
        ur_.y() = p.y();
}

std::ostream& operator<< (std::ostream& os, BoundingBox const& bb)
{
    os << "Bounding box: lower left:  " << bb.ll() << std::endl
       << "              upper right: " << bb.ur() << std::endl
       << "              width: " << bb.width() << std::endl
       << "              height: " << bb.height() << std::endl;
    return os;
}
