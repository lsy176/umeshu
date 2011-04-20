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

#include "Node.h"
#include "Edge.h"

#include <boost/assert.hpp>

std::ostream& operator<<(std::ostream& os, Node const& n)
{
    os << "Node " << &n << ": position=[" << n.x() << "," << n.y() << "], he=" << n.out_he() << std::endl;
    return os;
}

HalfEdgeHandle Node::is_boundary() const
{
    if (is_isolated()) {
        return NULL;
    }

    BOOST_ASSERT(false);
    // TODO
    
    return NULL;
}

int Node::degree() const
{
    BOOST_ASSERT(false);
    // TODO
    return 0;
}

int Node::virtual_degree() const
{
    BOOST_ASSERT(false);
    // TODO
    return 0;
}

int Node::ideal_degree() const
{
    BOOST_ASSERT(false);
    // TODO
    return 0;
}

HalfEdgeHandle Node::find_free_incident_halfedge () const
{
    return find_free_incident_halfedge_in_range(out_he_->pair(), out_he_->pair());
}


HalfEdgeHandle Node::find_free_incident_halfedge_in_range (HalfEdgeHandle he1, HalfEdgeHandle he2) const
{
    BOOST_ASSERT(this == he1->pair()->origin());
    BOOST_ASSERT(this == he2->pair()->origin());
    
    HalfEdgeHandle result = he1;
    while (result->face() != NULL ) {
        result = result->next()->pair();
        if (result == he2)
            break;
    }
    if (result->face() == NULL) {
        return result;
    } else {
        return NULL;
    }
}
