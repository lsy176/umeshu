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

#include "Edge.h"
#include "Face.h"
#include "Node.h"

bool compare_faces::operator() (FaceHandle f1, FaceHandle f2) const
{
    double s1 = f1->shortest_edge_length();
    s1 *= s1;
    double s2 = f2->shortest_edge_length();
    s2 *= s2;

    if (s1 == s2)
        return f1 < f2;
    else
        return s1 < s2;
}

void Face::edge_lengths(double& el1, double& el2, double& el3) const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    el1 = ::distance(p1, p2);
    el2 = ::distance(p2, p3);
    el3 = ::distance(p3, p1);
}

void Face::edge_lengths_squared(double& el1, double& el2, double& el3) const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    el1 = ::distance_squared(p1, p2);
    el2 = ::distance_squared(p2, p3);
    el3 = ::distance_squared(p3, p1);
}

HalfEdgeHandle Face::longest_edge(double *l) const
{
    HalfEdgeHandle he1 = adj_edge_;
    HalfEdgeHandle he2 = he1->next();
    HalfEdgeHandle he3 = he2->next();
    BOOST_ASSERT(he3->next() == he1);
    
    double l1 = he1->edge()->length();
    double l2 = he2->edge()->length();
    double l3 = he3->edge()->length();
    if (l1 > l2 && l1 > l3) {
        *l = l1;
        return he1;
    } else if (l2 > l3) {
        *l = l2;
        return he2;
    } else {
        *l = l3;
        return he3;
    }
}

double Face::longest_edge_length() const
{
    double l1, l2, l3;
    this->edge_lengths(l1, l2, l3);
    return std::max(l1,std::max(l2,l3));
}

double Face::longest_edge_length_squared() const
{
    double l1, l2, l3;
    this->edge_lengths_squared(l1, l2, l3);
    return std::max(l1,std::max(l2,l3));
}

HalfEdgeHandle Face::shortest_edge(double *l) const
{
    HalfEdgeHandle he1 = adj_edge_;
    HalfEdgeHandle he2 = he1->next();
    HalfEdgeHandle he3 = he2->next();
    BOOST_ASSERT(he3->next() == he1);

    double l1 = he1->edge()->length();
    double l2 = he2->edge()->length();
    double l3 = he3->edge()->length();
    if (l1 < l2 && l1 < l3) {
        *l = l1;
        return he1;
    } else if (l2 < l3) {
        *l = l2;
        return he2;
    } else {
        *l = l3;
        return he3;
    }
}

double Face::shortest_edge_length() const
{
    double l1, l2, l3;
    this->edge_lengths(l1, l2, l3);
    return std::min(l1, std::min(l2, l3));
}

double Face::shortest_edge_length_squared() const
{
    double l1, l2, l3;
    this->edge_lengths_squared(l1, l2, l3);
    return std::min(l1, std::min(l2, l3));
}

void Face::angles(double& a1, double& a2, double& a3) const
{    
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    ::triangle_angles(p1, p2, p3, a1, a2, a3);
}

double Face::minimum_angle() const
{
    double a1, a2, a3;
    this->angles(a1, a2, a3);
    return std::min(a1,std::min(a2,a3));
}

double Face::maximum_angle() const
{
    double a1, a2, a3;
    this->angles(a1, a2, a3);
    return std::max(a1,std::max(a2,a3));
}

Point2 Face::barycenter() const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    return ::barycenter(p1, p2, p3);
}

bool Face::is_restricted() const
{
    int bedges = 0;
    HalfEdgeHandle he = this->adjacent_he();
    if (he->pair()->face() == NULL) {
        ++bedges;
    }
    he = he->next();
    if (he->pair()->face() == NULL) {
        ++bedges;
    }
    he = he->next();
    if (he->pair()->face() == NULL) {
        ++bedges;
    }
    return bedges >= 2;
}

void Face::nodes(NodeHandle& n1, NodeHandle& n2, NodeHandle& n3) const
{
    n1 = adj_edge_->origin();
    n2 = adj_edge_->next()->origin();
    n3 = adj_edge_->prev()->origin();
}

void Face::vertices(Point2& v1, Point2& v2, Point2& v3) const
{
    NodeHandle n1, n2, n3;
    this->nodes(n1, n2, n3);
    v1 = n1->position();
    v2 = n2->position();
    v3 = n3->position();
}

std::ostream& operator<< (std::ostream& os, Face const& f)
{
    Point2 p1, p2, p3;
    f.vertices(p1, p2, p3);
    os << "Face " << &f << ": " << p1 << p2 << p3;
    return os;
}

