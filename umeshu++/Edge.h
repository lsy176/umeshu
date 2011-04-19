//
//  Edge.h
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/24/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#ifndef __EDGE_H_INCLUDED__
#define __EDGE_H_INCLUDED__

#include "Identifiable.h"
#include "Mesh_fwd.h"
#include "Node.h"
#include "Point2.h"

#include <iosfwd>

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

#endif // __EDGE_H_INCLUDED__