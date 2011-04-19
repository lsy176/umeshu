//
//  Node.h
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/24/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

#include "Identifiable.h"
#include "Mesh_fwd.h"
#include "Point2.h"

#include <iostream>

template 
class Node : public Identifiable
{
public:
    Node(double x, double y) : p_(x,y), out_he_(NULL) {}
    explicit Node(Point2 const& p) : p_(p), out_he_(NULL) {}

    inline double x() const { return p_.x(); }
    inline double& x() { return p_.x(); }
    inline double y() const { return p_.y(); }
    inline double& y() { return p_.y(); }
    inline Point2 const& position() const { return p_; }
//    inline Point2& position() { return p_; }
    
    inline HalfEdgeHandle out_he() const { return out_he_; }
    inline HalfEdgeHandle& out_he() { return out_he_; }
    
    inline bool is_isolated() const { return out_he_ == NULL; }
    HalfEdgeHandle is_boundary() const;
    int degree() const;
    int virtual_degree() const;
    int ideal_degree() const;
    
    HalfEdgeHandle find_free_incident_halfedge () const;
    HalfEdgeHandle find_free_incident_halfedge_in_range (HalfEdgeHandle he1, HalfEdgeHandle he2) const;
    
private:
    Point2 p_;
    HalfEdgeHandle out_he_;

    friend std::ostream& operator<<(std::ostream& os, Node const& n);
};

inline double distance_squared(NodeHandle n1, NodeHandle n2)
{
    return distance_squared(n1->position(), n2->position());
}

inline double distance(NodeHandle n1, NodeHandle n2)
{
    return std::sqrt(distance_squared(n1, n2));
}

#endif // __NODE_H_INCLUDED__