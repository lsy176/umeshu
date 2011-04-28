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

#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

#include "Identifiable.h"
#include "Mesh_fwd.h"
#include "Point2.h"

#include <iostream>

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
