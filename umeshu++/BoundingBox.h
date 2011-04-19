//
//  BoundingBox.h
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/30/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#ifndef __BOUNDING_BOX_H_INCLUDED__
#define __BOUNDING_BOX_H_INCLUDED__

#include "Point2.h"

#include <iosfwd>

class BoundingBox {
public:
    BoundingBox();
    BoundingBox(Point2 const& ll, Point2 const& ur);
    
    Point2 const& ll() const { return ll_; }
    Point2 const& ur() const { return ur_; }
    
    double width() const { return ur_.x() - ll_.x(); }
    double height() const { return ur_.y() - ll_.y(); }
    
    void include(Point2 const& p);
    
private:
    Point2 ll_, ur_;
};

std::ostream& operator<< (std::ostream& os, BoundingBox const& bb);

#endif // __BOUNDING_BOX_H_INCLUDED__