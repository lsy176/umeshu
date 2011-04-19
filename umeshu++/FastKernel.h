//
//  FastKernel.h
//  umeshu++
//
//  Created by Chalupecky Vladimir on 4/11/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#ifndef __FASTKERNEL_H_INCLUDED__
#define __FASTKERNEL_H_INCLUDED__

#include "Mesh_fwd.h"

class FastKernel {
public:
    static Oriented_side oriented_side (Point2 const& pa, Point2 const& pb, Point2 const& test);
    static Oriented_side oriented_side (HalfEdgeHandle he, Point2 const& test);
    static Oriented_side oriented_circle (Point2 const& pa, Point2 const& pb, Point2 const& pc, Point2 const& test);
    static double signed_area (Point2 const& pa, Point2 const& pb, Point2 const& pc);
    static Point2 circumcenter(Point2 const& p1, Point2 const& p2, Point2 const& p3);
    static Point2 offcenter(Point2 const& p1, Point2 const& p2, Point2 const& p3, double offconstant);
    
private:
    static inline double orient2d(Point2 const& pa, Point2 const& pb, Point2 const& pc)
    {
        return (pa.x() - pc.x()) * (pb.y() - pc.y()) - (pa.y() - pc.y()) * (pb.x() - pc.x());
    }
    
    static inline double incircle(Point2 const& pa, Point2 const& pb, Point2 const& pc, Point2 const& pd)
    {
        double adx, ady, bdx, bdy, cdx, cdy;
        double abdet, bcdet, cadet;
        double alift, blift, clift;
        double pdx = pd.x(), pdy = pd.y();
        
        adx = pa.x() - pdx;
        ady = pa.y() - pdy;
        bdx = pb.x() - pdx;
        bdy = pb.y() - pdy;
        cdx = pc.x() - pdx;
        cdy = pc.x() - pdy;
        
        abdet = adx * bdy - bdx * ady;
        bcdet = bdx * cdy - cdx * bdy;
        cadet = cdx * ady - adx * cdy;
        alift = adx * adx + ady * ady;
        blift = bdx * bdx + bdy * bdy;
        clift = cdx * cdx + cdy * cdy;
        
        return alift * bcdet + blift * cadet + clift * abdet;
    }
};

#endif /* __FASTKERNEL_H_INCLUDED__ */
