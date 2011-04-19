//
//  ExactAdaptiveKernel.h
//  umeshu++
//
//  Created by Chalupecky Vladimir on 4/11/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#ifndef __EXACT_ADAPTIVE_KERNEL_H_INCLUDED__
#define __EXACT_ADAPTIVE_KERNEL_H_INCLUDED__

#include "Mesh_fwd.h"

class ExactAdaptiveKernel {
public:
    static Oriented_side oriented_side (Point2 const& pa, Point2 const& pb, Point2 const& test);
    static Oriented_side oriented_side (HalfEdgeHandle he, Point2 const& test);
    static Oriented_side oriented_circle (Point2 const& pa, Point2 const& pb, Point2 const& pc, Point2 const& test);
    static double signed_area (Point2 const& pa, Point2 const& pb, Point2 const& pc);
    static Point2 circumcenter(Point2 const& p1, Point2 const& p2, Point2 const& p3);
    static Point2 offcenter(Point2 const& p1, Point2 const& p2, Point2 const& p3, double offconstant);
};

#endif /* __EXACT_ADAPTIVE_KERNEL_H_INCLUDED__ */
