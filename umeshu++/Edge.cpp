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

#include "BoundarySegment.h"
#include "Edge.h"
#include "Node.h"

#include <iostream>

HalfEdge::HalfEdge(NodeHandle n, EdgeHandle e)
: pair_(this), next_(this), prev_(this), edge_(e), origin_(n), face_(NULL)
{}

std::ostream& operator<<(std::ostream& os, HalfEdge const& he)
{
    os << "Half-edge " << &he << ": origin=" << he.origin() << ", face=" << he.face() << ", edge=" << he.edge() << std::endl
       << "                         pair=" << he.pair() << ", next=" << he.next() << ", prev=" << he.prev() << std::endl;
    return os; 
}

Edge::Edge(NodeHandle n1, NodeHandle n2, BoundarySegment *bseginfo)
: he1_(n1, this), he2_(n2, this), bseginfo_(bseginfo)
{
    he1_.next_ = he1_.prev_ = &he2_;
    he2_.next_ = he2_.prev_ = &he1_;
    he1_.pair_ = &he2_;
    he2_.pair_ = &he1_;
}

bool Edge::is_encroached_upon(Point2 const& p) const {
    Point2 p1, p2;
    this->vertices(p1, p2);
    double dot_p = (p1.x() - p.x())*(p2.x() - p.x()) +
                   (p1.y() - p.y())*(p2.y() - p.y());
    return dot_p < 0.0;
}

Point2 Edge::midpoint() const
{
	Point2 p1, p2;
	this->vertices(p1, p2);
	if (bseginfo_ == NULL) {
		return ::midpoint(p1, p2);
	} else {
		return bseginfo_->midpoint(p1, p2);
	}
}

std::ostream& operator<<(std::ostream& os, Edge const& e)
{
    os << "Edge " << &e << ":" << std::endl;
    os << "    " << e.he1_;
    os << "    " << e.he2_;
    return os;
}
