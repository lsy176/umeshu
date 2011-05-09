//
//  Copyright (c) 2011 Vladimir Chalupecky
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.

#ifndef __FASTKERNEL_H_INCLUDED__
#define __FASTKERNEL_H_INCLUDED__

#include "Mesh_fwd.h"

namespace umeshu {

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

} // namespace umeshu

#endif /* __FASTKERNEL_H_INCLUDED__ */
