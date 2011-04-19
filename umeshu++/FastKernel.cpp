//
//  FastKernel.cpp
//  umeshu++
//
//  Created by Chalupecky Vladimir on 4/11/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#include "Edge.h"
#include "FastKernel.h"
#include "Node.h"
#include "Point2.h"
#include "Predicates.h"

#include <boost/assert.hpp>

Oriented_side FastKernel::oriented_side (Point2 const& pa, Point2 const& pb, Point2 const& test)
{
    double r = FastKernel::orient2d(pa, pb, test);
    if (r > 0.0)
        return ON_POSITIVE_SIDE;
    else if (r < 0.0)
        return ON_NEGATIVE_SIDE;
    else
        return ON_ORIENTED_BOUNDARY;    
}

Oriented_side FastKernel::oriented_side (HalfEdgeHandle he, Point2 const& test)
{
    return oriented_side(he->origin()->position(), he->pair()->origin()->position(), test);    
}

Oriented_side FastKernel::oriented_circle (Point2 const& pa, Point2 const& pb, Point2 const& pc, Point2 const& test)
{
    double r = FastKernel::incircle(pa, pb, pc, test);
    if (r > 0.0)
        return ON_POSITIVE_SIDE;
    else if (r < 0.0)
        return ON_NEGATIVE_SIDE;
    else
        return ON_ORIENTED_BOUNDARY;    
}

double FastKernel::signed_area(Point2 const& pa, Point2 const& pb, Point2 const& pc)
{
    return  0.5*FastKernel::orient2d(pa, pb, pc);
}

Point2 FastKernel::circumcenter(Point2 const& p1, Point2 const& p2, Point2 const& p3)
{
    Point2 p2p1(p2.x()-p1.x(), p2.y()-p1.y());
    Point2 p3p1(p3.x()-p1.x(), p3.y()-p1.y());
    Point2 p2p3(p2.x()-p3.x(), p2.y()-p3.y());
    double p2p1dist = p2p1.x()*p2p1.x() + p2p1.y()*p2p1.y();
    double p3p1dist = p3p1.x()*p3p1.x() + p3p1.y()*p3p1.y();
    double denominator = 0.5 / (2.0*FastKernel::signed_area(p1, p2, p3));
    BOOST_ASSERT(denominator > 0.0);
    double dx = (p3p1.y()*p2p1dist - p2p1.y()*p3p1dist)*denominator;
    double dy = (p2p1.x()*p3p1dist - p3p1.x()*p2p1dist)*denominator;
    return Point2(p1.x()+dx, p1.y()+dy);
}

Point2 FastKernel::offcenter(Point2 const& p1, Point2 const& p2, Point2 const& p3, double offconstant)
{
    Point2 p2p1(p2.x()-p1.x(), p2.y()-p1.y());
    Point2 p3p1(p3.x()-p1.x(), p3.y()-p1.y());
    Point2 p2p3(p2.x()-p3.x(), p2.y()-p3.y());
    double p2p1dist = p2p1.x()*p2p1.x() + p2p1.y()*p2p1.y();
    double p3p1dist = p3p1.x()*p3p1.x() + p3p1.y()*p3p1.y();
    double p2p3dist = p2p3.x()*p2p3.x() + p2p3.y()*p2p3.y();
    double denominator = 0.5/(2.0*FastKernel::signed_area(p1, p2, p3));
    BOOST_ASSERT(denominator > 0.0);
    double dx = (p3p1.y() * p2p1dist - p2p1.y() * p3p1dist) * denominator;
    double dy = (p2p1.x() * p3p1dist - p3p1.x() * p2p1dist) * denominator;
    double dxoff, dyoff;
    
    if ((p2p1dist < p3p1dist) && (p2p1dist < p2p3dist)) {
        dxoff = 0.5 * p2p1.x() - offconstant * p2p1.y();
        dyoff = 0.5 * p2p1.y() + offconstant * p2p1.x();
        if (dxoff * dxoff + dyoff * dyoff < dx * dx + dy * dy) {
            dx = dxoff;
            dy = dyoff;
        }
    } else if (p3p1dist < p2p3dist) {
        dxoff = 0.5 * p3p1.x() + offconstant * p3p1.y();
        dyoff = 0.5 * p3p1.y() - offconstant * p3p1.x();
        if (dxoff * dxoff + dyoff * dyoff < dx * dx + dy * dy) {
            dx = dxoff;
            dy = dyoff;
        }
    } else {
        dxoff = 0.5 * p2p3.x() - offconstant * p2p3.y();
        dyoff = 0.5 * p2p3.y() + offconstant * p2p3.x();
        if (dxoff * dxoff + dyoff * dyoff < (dx - p2p1.x()) * (dx - p2p1.x()) + (dy - p2p1.y()) * (dy - p2p1.y())) {
            dx = p2p1.x() + dxoff;
            dy = p2p1.y() + dyoff;
        }
    }
    
    return Point2(p1.x()+dx, p1.y()+dy);
}
