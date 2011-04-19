//
//  Node.cpp
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/24/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

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
