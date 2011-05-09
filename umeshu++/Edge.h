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

#ifndef __EDGE_H_INCLUDED__
#define __EDGE_H_INCLUDED__

#include "Identifiable.h"
#include "Mesh_fwd.h"
#include "Node.h"
#include "Point2.h"

#include <iosfwd>

namespace umeshu {

class HalfEdge
{
public:
    inline NodeHandle origin() const { return origin_; }
    inline NodeHandle& origin() { return origin_; }

    inline HalfEdgeHandle next() const { return next_; }
    inline HalfEdgeHandle& next() { return next_; }
    
    inline HalfEdgeHandle prev() const { return prev_; }
    inline HalfEdgeHandle& prev() { return prev_; }
    
    inline HalfEdgeHandle pair() const { return pair_; }
    inline EdgeHandle edge() const { return edge_; }
    
    inline FaceHandle face() const { return face_; }
    inline FaceHandle& face() { return face_; }

    inline bool is_boundary() const { return face_ == NULL; }
    
    void vertices(Point2& porig, Point2& pdest) const;

private:
    HalfEdge(NodeHandle n, EdgeHandle e);

    HalfEdgeHandle pair_;
    HalfEdgeHandle next_;
    HalfEdgeHandle prev_;
    EdgeHandle edge_;
    NodeHandle origin_;
    FaceHandle face_;

    friend class Edge;
    friend std::ostream& operator<<(std::ostream& os, HalfEdge const& he);
};


class BoundarySegment;

class Edge : public Identifiable
{
public:
    Edge(NodeHandle n1, NodeHandle n2, BoundarySegment *bseginfo);

    inline HalfEdgeHandle he1() { return &he1_; }
    inline HalfEdgeHandle he2() { return &he2_; }
    inline HalfEdge const * he1() const { return &he1_; }
    inline HalfEdge const * he2() const { return &he2_; }

    inline HalfEdgeHandle halfedge_with_origin(NodeHandle n)
    {
        return (he1()->origin() == n) ? he1() : he2();
    }

    double length() const;

    inline bool is_boundary() const { return he1_.is_boundary() || he2_.is_boundary(); }

    template <class P>
    bool is_swappable() const;

    template <class P>
    bool is_delaunay() const;

    bool is_encroached_upon(Point2 const& p) const;

	Point2 midpoint() const;
    
    inline BoundarySegment* boundary_info() const { return bseginfo_; }

    void vertices(Point2& p1, Point2& p2) const;
    
private:
    HalfEdge he1_, he2_;
	BoundarySegment *bseginfo_;
    
    friend std::ostream& operator<<(std::ostream& os, Edge const& e);
};

template <class P>
bool Edge::is_swappable() const {
    if (this->is_boundary()) {
        return false;
    }
    Point2 p1 = this->he1()->origin()->position();
    Point2 p2 = this->he2()->prev()->origin()->position();
    Point2 p3 = this->he2()->origin()->position();
    Point2 p4 = this->he1()->prev()->origin()->position();
    
    if (P::oriented_side(p1, p2, p3) != ON_POSITIVE_SIDE ||
        P::oriented_side(p2, p3, p4) != ON_POSITIVE_SIDE ||
        P::oriented_side(p3, p4, p1) != ON_POSITIVE_SIDE ||
        P::oriented_side(p4, p1, p2) != ON_POSITIVE_SIDE)
    {
        return false;
    }
    return true;    
}

template <class P>
bool Edge::is_delaunay() const {
    if (this->is_boundary()) {
        return true;
    }
    
    Point2 p1 = this->he1()->origin()->position();
    Point2 p2 = this->he2()->prev()->origin()->position();
    Point2 p3 = this->he2()->origin()->position();
    Point2 p4 = this->he1()->prev()->origin()->position();
    
    if (P::oriented_circle(p1, p2, p3, p4) == ON_POSITIVE_SIDE) {
        return false;
    }
    return true;
}

inline void HalfEdge::vertices(Point2& porig, Point2& pdest) const
{
    porig = origin()->position();
    pdest = pair()->origin()->position();
}

inline double Edge::length() const
{
    return distance(he1()->origin(), he2()->origin());
}

inline void Edge::vertices(Point2& p1, Point2& p2) const
{
    he1()->vertices(p1, p2);
}

} // namespace umeshu

#endif // __EDGE_H_INCLUDED__
