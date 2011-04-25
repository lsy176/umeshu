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

#ifndef __FACE_H_INCLUDED__
#define __FACE_H_INCLUDED__

#include "Identifiable.h"
#include "Mesh_fwd.h"
#include "Point2.h"
#include "ExactAdaptiveKernel.h"

#include <boost/assert.hpp>

class Face : public Identifiable
{
public:
    explicit Face(HalfEdgeHandle adj_edge)
    : adj_edge_(adj_edge), area_(0.0)
    { this->compute_area<ExactAdaptiveKernel>(); }

    inline HalfEdgeHandle adjacent_he() const { return adj_edge_; }
    inline HalfEdgeHandle& adjacent_he() { return adj_edge_; }
    
    void edge_lengths(double& el1, double& el2, double& el3) const;
    void edge_lengths_squared(double& el1, double& el2, double& el3) const;

    HalfEdgeHandle longest_edge(double *l = NULL) const;
    double longest_edge_length() const;
    double longest_edge_length_squared() const;

    HalfEdgeHandle shortest_edge(double *l = NULL) const;
    double shortest_edge_length() const;
    double shortest_edge_length_squared() const;
    
    void angles(double& a1, double& a2, double& a3) const;
    double minimum_angle() const;
    double maximum_angle() const;

    template <typename Kernel>
    double circumradius() const;
    
    Point2 barycenter() const;

    template <typename Kernel>
    Point2 circumcenter() const;

    template <typename Kernel>
    Point2 offcenter(double offconstant) const;

    template <typename Kernel>
    double area() const { return area_; }
    template <typename Kernel>
    double compute_area();

    template <typename Kernel>
    double quality() const;
    
    bool is_restricted() const;

    void nodes(NodeHandle& n1, NodeHandle& n2, NodeHandle& n3) const;
    void vertices(Point2& v1, Point2& v2, Point2& v3) const;

    friend bool operator== (Face const& f1, Face const& f2);
    friend bool operator!= (Face const& f1, Face const& f2);
    friend std::ostream& operator<< (std::ostream& os, Face const& f);
    
private:
    HalfEdgeHandle adj_edge_;
    double area_;
};

inline bool operator== (Face const& f1, Face const& f2)
{
    NodeHandle f1n1, f1n2, f1n3;
    NodeHandle f2n1, f2n2, f2n3;
    f1.nodes(f1n1, f1n2, f1n3);
    f2.nodes(f2n1, f2n2, f2n3);
    if (f1n1 == f2n1 && f1n2 == f2n2 && f1n3 == f2n3 ) return true;
    if (f1n1 == f2n2 && f1n2 == f2n3 && f1n3 == f2n1 ) return true;
    if (f1n1 == f2n3 && f1n2 == f2n1 && f1n3 == f2n2 ) return true;
    return false;
}

inline bool operator!= (Face const& f1, Face const& f2)
{
    return !(f1 == f2);
}

struct compare_faces {
    bool operator() (FaceHandle f1, FaceHandle f2) const;
};

template <typename Kernel>
double Face::compute_area()
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    area_ = Kernel::signed_area(p1, p2, p3);
    BOOST_ASSERT(area_ > 0.0);
    return area_;
}

template <typename Kernel>
double Face::quality() const
{
    // TODO
    return 1.0;
}


template <typename Kernel>
Point2 Face::circumcenter() const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    return Kernel::circumcenter(p1, p2, p3);    
}

template <typename Kernel>
Point2 Face::offcenter(double offconstant) const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    return Kernel::offcenter(p1, p2, p3, offconstant);
}

template <typename Kernel>
double Face::circumradius() const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    return Kernel::circumradius(p1, p2, p3);
}

#endif // __FACE_H_INCLUDED__
