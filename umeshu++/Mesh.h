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

#ifndef __MESH_H_INCLUDED__
#define __MESH_H_INCLUDED__

#include "BoundarySegment.h"
#include "BoundingBox.h"
#include "Edge.h"
#include "Face.h"
#include "Node.h"
#include "Postscript_ostream.h"
#include "Predicates.h"

#include <boost/assert.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/unordered_set.hpp>
#include <boost/utility.hpp>
#include <boost/foreach.hpp>

#include <set>

template <class Kernel>
class Mesh : public boost::noncopyable {
public:
    typedef enum {IN_FACE, ON_EDGE, ON_NODE, OUTSIDE_MESH} Point2Location;
    typedef Kernel kernel_type;

    NodeHandle add_node (Point2 const& p);
    void remove_node (NodeHandle n);

    EdgeHandle add_edge (NodeHandle n1, NodeHandle n2, BoundarySegment *bseginfo);
    void remove_edge (EdgeHandle e);
    NodeHandle split_edge (EdgeHandle e);
    NodeHandle split_edge (EdgeHandle e, Point2 const& p);
    EdgeHandle swap_edge (EdgeHandle e);

    FaceHandle add_face (HalfEdgeHandle he1, HalfEdgeHandle he2, HalfEdgeHandle he3);
    void remove_face (FaceHandle f);
    NodeHandle split_face (FaceHandle f, Point2 const& p);

    BoundingBox bounding_box () const;

    HalfEdgeHandle boundary_halfedge() const;

    double smallest_angle() const;

    size_t number_of_nodes () const { return nodes_.size(); }
    size_t number_of_edges () const { return edges_.size(); }
    size_t number_of_faces () const { return faces_.size(); }

private:
    void attach_edge (EdgeHandle e, NodeHandle n);
    void detach_edge (EdgeHandle e, NodeHandle n);
    bool make_adjacent (HalfEdgeHandle in, HalfEdgeHandle out);

    typedef boost::unordered_set<NodeHandle> Nodes;
    typedef boost::unordered_set<EdgeHandle> Edges;
    typedef boost::unordered_set<FaceHandle> Faces;
    
    Nodes nodes_;
    Edges edges_;
    Faces faces_;
    
    boost::object_pool<Node> node_pool_;
    boost::object_pool<Edge> edge_pool_;
    boost::object_pool<Face> face_pool_;
    
    template <typename K> friend Postscript_stream& operator<< (Postscript_stream& ps, Mesh<K> const& m);
    
    template<typename> friend class Mesher;
};

template <typename Kernel>
double Mesh<Kernel>::smallest_angle() const
{
    double sa = std::numeric_limits<double>::max();
    BOOST_FOREACH(FaceHandle f, faces_) {
        double a = f->minimum_angle();
        if (a < sa) {
            sa = a;
        }
    }
    return radians_to_degrees(sa);
}


template <class Kernel>
NodeHandle Mesh<Kernel>::add_node (Point2 const& p)
{
    NodeHandle n(node_pool_.construct(p));
    nodes_.insert(n);
    return n;
}

template <class Kernel>
void Mesh<Kernel>::remove_node (NodeHandle n)
{
    HalfEdgeHandle next = n->out_he();
    while (not n->is_isolated()) {
        HalfEdgeHandle cur = next;
        next = cur->pair()->next();
        this->remove_edge(cur->edge());
    }

    nodes_.erase(n);
    node_pool_.destroy(n);
}

template <class Kernel>
EdgeHandle Mesh<Kernel>::add_edge (NodeHandle n1, NodeHandle n2, BoundarySegment *bseginfo)
{
    BOOST_ASSERT(n1 != n2);
    EdgeHandle e(edge_pool_.construct(n1, n2, bseginfo));
    edges_.insert(e);
    this->attach_edge(e, n1);
    this->attach_edge(e, n2);
    return e;
}

template <class Kernel>
void Mesh<Kernel>::attach_edge (EdgeHandle e, NodeHandle n)
{
    HalfEdgeHandle out_he = e->halfedge_with_origin(n);
    
    if (n->is_isolated()) {
        n->out_he() = out_he;
    } else {
        HalfEdgeHandle free_in_he = n->find_free_incident_halfedge();
        // TODO: better handling of this situation (which should not normally happen)
        BOOST_ASSERT_MSG(free_in_he != NULL, "Did not find free incident half-edge");
        
        HalfEdgeHandle free_out_he = free_in_he->next();        
        BOOST_ASSERT(free_out_he->face() == NULL);
        
        free_in_he->next() = out_he;
        out_he->prev() = free_in_he;
        out_he->pair()->next() = free_out_he; 
        free_out_he->prev() = out_he->pair();
    }
}

template <class Kernel>
void Mesh<Kernel>::remove_edge (EdgeHandle e)
{
    // remove elements on both sides of the edge
    if (e->he1()->face() != NULL) {
        this->remove_face(e->he1()->face());
    }
    if (e->he2()->face() != NULL) {
        this->remove_face(e->he2()->face());
    }
    
    this->detach_edge(e, e->he1()->origin());
    this->detach_edge(e, e->he2()->origin());
    
    edges_.erase(e);
    edge_pool_.destroy(e);
}

template <class Kernel>
NodeHandle Mesh<Kernel>::split_edge (EdgeHandle e, Point2 const& p)
{
    FaceHandle f1 = e->he1()->face();
    FaceHandle f2 = e->he2()->face();
    HalfEdgeHandle he1 = e->he1();
    HalfEdgeHandle he2 = e->he2();
    
    HalfEdgeHandle he1n = NULL, he1p = NULL;
    if (f1 != NULL) {
        he1n = he1->next();
        he1p = he1->prev();
    }
    
    HalfEdgeHandle he2n = NULL, he2p = NULL;
    if (f2 != NULL) {
        he2n = he2->next();
        he2p = he2->prev();
    }
    
    NodeHandle n1 = he1->origin();
    NodeHandle n2 = he2->origin();
    BoundarySegment *bseginfo = e->boundary_info();
    this->remove_edge(e);
    NodeHandle n = this->add_node(p);
    EdgeHandle e1 = this->add_edge(n1, n, bseginfo);
    EdgeHandle e2 = this->add_edge(n2, n, bseginfo);
    if (f1 != NULL) {
        EdgeHandle e3 = this->add_edge(he1p->origin(), n, NULL);
        this->add_face(he1p, e1->halfedge_with_origin(n1), e3->halfedge_with_origin(n));
        this->add_face(he1n, e3->halfedge_with_origin(n)->pair(), e2->halfedge_with_origin(n));
    }
    if (f2 != NULL) {
        EdgeHandle e4 = this->add_edge(he2p->origin(), n, NULL);
        this->add_face(he2p, e2->halfedge_with_origin(n2), e4->halfedge_with_origin(n));
        this->add_face(he2n, e4->halfedge_with_origin(n)->pair(), e1->halfedge_with_origin(n));
    }
    
    return n;
}

template <class Kernel>
NodeHandle Mesh<Kernel>::split_edge (EdgeHandle e)
{
    return this->split_edge(e, e->midpoint());
}

template <class Kernel>
void Mesh<Kernel>::detach_edge (EdgeHandle e, NodeHandle n)
{
    HalfEdgeHandle he = e->halfedge_with_origin(n);
    
    if ( n->out_he() == he )
    {
        if ( he->pair()->next() != he )
            n->out_he() = he->pair()->next();
        else
            n->out_he() = NULL;
    }
    
    he->prev()->next() = he->pair()->next();
    he->pair()->next()->prev() = he->prev();
}

template <class Kernel>
EdgeHandle Mesh<Kernel>::swap_edge (EdgeHandle e)
{   
    BOOST_ASSERT(not e->is_boundary());

    HalfEdgeHandle he1 = e->he1()->next();
    HalfEdgeHandle he2 = e->he1()->prev();
    HalfEdgeHandle he3 = e->he2()->next();
    HalfEdgeHandle he4 = e->he2()->prev();
    NodeHandle n1 = he2->origin();
    NodeHandle n2 = he4->origin();
    
    this->remove_edge(e);
    EdgeHandle new_edge = this->add_edge(n1, n2, NULL);
    this->add_face(he2, he3, new_edge->halfedge_with_origin(n2));
    this->add_face(he4, he1, new_edge->halfedge_with_origin(n1));
    
    return new_edge;
}

template <class Kernel>
bool Mesh<Kernel>::make_adjacent (HalfEdgeHandle in, HalfEdgeHandle out)
{
    // half-edges are already adjacent
    if (in->next() == out)
        return true;
    
    HalfEdgeHandle b = in->next();
    HalfEdgeHandle d = out->prev();
    
    NodeHandle n = out->origin();
    HalfEdgeHandle g = n->find_free_incident_halfedge_in_range(out->pair(), in);
    // half-edges cannot be made adjacent
    if (g == NULL)
        return false;
    
    HalfEdgeHandle h = g->next();
    in->next() = out;
    out->prev() = in;
    g->next() = b;
    b->prev() = g;
    d->next() = h;
    h->prev() = d;
    
    return true;
}

template <class Kernel>
FaceHandle Mesh<Kernel>::add_face (HalfEdgeHandle he1, HalfEdgeHandle he2, HalfEdgeHandle he3)
{
    if (not(he1->face() == NULL &&
            he2->face() == NULL &&
            he3->face() == NULL))
    {
        std::clog << "Mesh<Kernel>::add_face: half-edges are not free, cannot add face\n";
        return NULL;
    }
    
    if (not(he1->pair()->origin() == he2->origin() &&
            he2->pair()->origin() == he3->origin() &&
            he3->pair()->origin() == he1->origin()))
    {
        std::clog << "Mesh<Kernel>::add_face: half-edges do not form a chain, cannot add face\n";
        return NULL;
    }
    
    if (not(make_adjacent(he1, he2) &&
            make_adjacent(he2, he3) &&
            make_adjacent(he3, he1)))
    {
        std::clog << "Mesh<Kernel>::add_face: attempting to create non-manifold mesh, cannot add face\n";
        return NULL;
    }
    
    FaceHandle f(face_pool_.construct(he1));
    he1->face() = he2->face() = he3->face() = f;
    faces_.insert(f);
    
    return f;
}

template <class Kernel>
void Mesh<Kernel>::remove_face (FaceHandle f)
{    
    HalfEdgeHandle begin = f->adjacent_he();
    HalfEdgeHandle he_iterator = begin;
    do {
        he_iterator->face() = NULL;
        he_iterator = he_iterator->next();
    } while (he_iterator != begin);
    
    faces_.erase(f);
    face_pool_.destroy(f);
}

template <class Kernel>
NodeHandle Mesh<Kernel>::split_face (FaceHandle f, Point2 const& p)
{
    HalfEdgeHandle he1 = f->adjacent_he();
    HalfEdgeHandle he2 = he1->next();
    HalfEdgeHandle he3 = he2->next();
    
    this->remove_face(f);
    
    NodeHandle n = this->add_node(p);
    EdgeHandle e1 = this->add_edge(n, he1->origin(), NULL);
    EdgeHandle e2 = this->add_edge(n, he2->origin(), NULL);
    EdgeHandle e3 = this->add_edge(n, he3->origin(), NULL);
    this->add_face(he1, e2->halfedge_with_origin(n)->pair(), e1->halfedge_with_origin(n));
    this->add_face(he2, e3->halfedge_with_origin(n)->pair(), e2->halfedge_with_origin(n));
    this->add_face(he3, e1->halfedge_with_origin(n)->pair(), e3->halfedge_with_origin(n));
    
    return n;
}

template <class Kernel>
HalfEdgeHandle Mesh<Kernel>::boundary_halfedge() const
{
    BOOST_FOREACH(EdgeHandle e, edges_)
    {
        if (e->is_boundary()) {
            return e->he1()->is_boundary() ? e->he1() : e->he2();
        }
    }
    return NULL;
}

template <class Kernel>
BoundingBox Mesh<Kernel>::bounding_box () const
{
    BoundingBox bb;
    BOOST_FOREACH(NodeHandle n, this->nodes_)
    {
        bb.include(n->position());
    }
    return bb;
}

template<class Kernel>
Postscript_stream& operator<< (Postscript_stream& ps, Mesh<Kernel> const& m)
{
    ps.setgray(0.8);
    BOOST_FOREACH(FaceHandle f, m.faces_)
    {
        ps << *f;
        ps.fill();
    }
    
    ps.setgray(0);
    ps.newpath();
    BOOST_FOREACH(EdgeHandle e, m.edges_)
    {
        ps << *e;
    }
    ps.stroke();

    ps.setrgbcolor(1,0,0);
    BOOST_FOREACH(NodeHandle n, m.nodes_)
    {
        ps << n->position();
        ps.fill();
    }
    
    return ps;
}

template <class Kernel>
void locate_point(Point2 const& p, FaceHandle start_face, typename Mesh<Kernel>::Point2Location& loc, void*& entity)
{
    HalfEdgeHandle he_iter = start_face->adjacent_he();
    HalfEdgeHandle he_start = he_iter;
    Point2 q = p;
    
    while (true) {
        Oriented_side os = Kernel::oriented_side(he_iter, q);
        switch (os) {
            case ON_POSITIVE_SIDE:
                he_iter = he_iter->next();
                if ( he_iter == he_start ) {
                    entity = he_iter->face();
                    loc = Mesh<Kernel>::IN_FACE;
                    return;
                }
                break;
            case ON_ORIENTED_BOUNDARY:
            {
                Point2 p1 = he_iter->origin()->position();
                Point2 p2 = he_iter->pair()->origin()->position();
                if ((std::min(p1.x(),p2.x()) < q.x() && q.x() < std::max(p1.x(),p2.x())) ||
                    (std::min(p1.y(),p2.y()) < q.y() && q.y() < std::max(p1.y(),p2.y())) )
                {
                    entity = he_iter->edge();
                    loc = Mesh<Kernel>::ON_EDGE;
                    return;
                } else if (q == p1) {
                    entity = he_iter->origin();
                    loc = Mesh<Kernel>::ON_NODE;
                    return;
                } else if (q == p2) {
                    entity = he_iter->pair()->origin();
                    loc = Mesh<Kernel>::ON_NODE;
                    return;
                }
            }
            case ON_NEGATIVE_SIDE:
                he_iter = he_iter->pair();
                if ( he_iter->is_boundary() )
                {
                    loc = Mesh<Kernel>::OUTSIDE_MESH;
                    entity = he_iter->pair();
                    return;
                }
                he_start = he_iter;
                he_iter = he_iter->next();
        }
    }
}

#endif // __MESH_H_INCLUDED__
