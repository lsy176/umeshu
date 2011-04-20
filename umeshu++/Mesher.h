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

#ifndef __MESHER_H_INCLUDED__
#define __MESHER_H_INCLUDED__

#include "Face.h"
#include "Mesh.h"
#include "Predicates.h"
#include "Postscript_ostream.h"

#include <boost/lexical_cast.hpp>
#include <stack>
#include <list>
#include <set>

template <typename Mesh>
class Mesher {
public:
    typedef typename Mesh::kernel_type kernel_type;

    explicit Mesher(Mesh& mesh)
        : mesh_(mesh), max_face_area_(1.0), min_angle_(degrees_to_radians(20.0)) {}

    void refine (double max_face_area, double min_angle);
    void make_cdt ();

private:
    void split_encroached_boundary_edges (bool check_quality);
    void collect_encroached_boundary_edges ();
    void collect_encroached_boundary_edges (NodeHandle n, std::stack<HalfEdgeHandle>& E);
    void treat_new_node (NodeHandle n, bool check_quality);
    bool split_permitted(HalfEdgeHandle he, double d);

    void recursive_swap_delaunay (HalfEdgeHandle he, bool save_to_undo_stack);
    FaceHandle get_bad_face ();
    void undo_swapping ();
    void remove_node (NodeHandle n);
    FaceHandle add_face (HalfEdgeHandle he1, HalfEdgeHandle he2, HalfEdgeHandle he3);
    EdgeHandle swap_edge (EdgeHandle e);
    NodeHandle split_edge (EdgeHandle e);
    NodeHandle split_edge (EdgeHandle e, Point2 const& p);
    NodeHandle split_face (FaceHandle f, Point2 const& p);
    
    void enqueue_bad_face (FaceHandle f);
    void dequeue_bad_face (FaceHandle f);

    Mesh &mesh_;

    double min_angle_;
    double max_face_area_;

    std::set<FaceHandle,compare_faces> bad_faces_;
    boost::unordered_set<HalfEdgeHandle> enc_hedges_;
    std::stack<EdgeHandle> undo_stack_;
};

// TODO: osetrit pouzivani bad_face a entity_to_kill v pripade, ze ukazatel uz neni platny kvuli prehazovani hran
// TODO: cely ten zatraceny kod zkontrolovat
// 

template <typename Mesh>
void Mesher<Mesh>::refine (double max_face_area, double min_angle)
{
    max_face_area_ = max_face_area;
    min_angle_ = degrees_to_radians(min_angle);

    double cos_min_angle = std::cos(min_angle_);
    double offconstant = 0.0;
    if (cos_min_angle != 1.0) {
        offconstant = 0.475 * sqrt((1.0 + cos_min_angle) / (1.0 - cos_min_angle));
    }

    this->collect_encroached_boundary_edges();
    this->split_encroached_boundary_edges(false);

    BOOST_FOREACH(FaceHandle f, mesh_.faces_) {
        this->enqueue_bad_face(f);
    }

    int i = 0;

    FaceHandle bad_face = NULL;
    while ((bad_face = this->get_bad_face()) != NULL) {
        Point2 center = bad_face->circumcenter<kernel_type>();
//        Point2 center = bad_face->offcenter<kernel_type>(offconstant);

        typename Mesh::Point2Location loc;
        void *entity_to_kill;
        locate_point<kernel_type>(center, bad_face, loc, entity_to_kill);
        
        BOOST_ASSERT(loc != Mesh::ON_NODE);
        BOOST_ASSERT(enc_hedges_.empty());
        BOOST_ASSERT(undo_stack_.empty());
        
        if (loc == Mesh::IN_FACE) {
            FaceHandle face_to_kill = static_cast<FaceHandle>(entity_to_kill);
            double d = bad_face->shortest_edge_length();
            double bad_face_area = bad_face->area<kernel_type>();

            NodeHandle new_node = this->split_face(face_to_kill, center);

            HalfEdgeHandle he1 = new_node->out_he()->next();
            HalfEdgeHandle he2 = he1->next()->pair()->next();
            HalfEdgeHandle he3 = he2->next()->pair()->next();

            this->recursive_swap_delaunay(he1, true);
            this->recursive_swap_delaunay(he2, true);
            this->recursive_swap_delaunay(he3, true);
            
            std::stack<HalfEdgeHandle> E;
            this->collect_encroached_boundary_edges(new_node, E);
            if (E.empty()) {
                while (not undo_stack_.empty()) { undo_stack_.pop(); }
                this->treat_new_node(new_node, true);                
            } else {
                this->undo_swapping();

                he1 = new_node->out_he()->next();
                he2 = he1->next()->pair()->next();
                he3 = he2->next()->pair()->next();
                this->remove_node(new_node);
                this->add_face(he1, he2, he3);

                // IMPORTANT TODO AND A BUG AS IT IS NOW!!!
                // Recover somehow the handle to the original bad face
                // The following line usually gives the correct handle, sometimes it does not
                FaceHandle new_bad_face = this->get_bad_face();
//                std::cout << "New bad face: " << *new_bad_face << std::endl;
//                BOOST_ASSERT(bad_face_area == new_bad_face->area<kernel_type>());

                while (not E.empty()) {
                    HalfEdgeHandle he = E.top();
                    E.pop();
                    if (bad_face_area > max_face_area_ || this->split_permitted(he, d)) {
                        enc_hedges_.insert(he);
                    }
                }
                if (not enc_hedges_.empty()) {
//                    no need to enque again, still in bad set
//                    this->enqueue_bad_face(new_bad_face);
                    this->split_encroached_boundary_edges(true);
                } else {
                    this->dequeue_bad_face(new_bad_face);
                }
            }
        } else if (loc == Mesh::ON_EDGE) {
            EdgeHandle edge_to_kill = static_cast<EdgeHandle>(entity_to_kill);

            double d = bad_face->shortest_edge_length();
            double bad_face_area = bad_face->area<kernel_type>();

            bool build_123 = edge_to_kill->he1()->face() != NULL;
            bool build_142 = edge_to_kill->he2()->face() != NULL;
            NodeHandle n1 = edge_to_kill->he1()->origin();
            NodeHandle n2 = edge_to_kill->he2()->origin();
            NodeHandle n3 = NULL, n4 = NULL;
            if (build_123) {
                n3 = edge_to_kill->he1()->prev()->origin();                
            }
            if (build_142) {
                n4 = edge_to_kill->he2()->prev()->origin();
            }

            BoundarySegment *bseginfo = edge_to_kill->boundary_info();
            NodeHandle new_node = this->split_edge(edge_to_kill, center);

            HalfEdgeHandle he_start = new_node->out_he();
            HalfEdgeHandle he_iter = he_start;
            do {
                if (he_iter->face() != NULL) {
                    this->recursive_swap_delaunay(he_iter->next(), true);
                }
                he_iter = he_iter->pair()->next();
            } while (he_iter != he_start);
            
            std::stack<HalfEdgeHandle> E;
            this->collect_encroached_boundary_edges(new_node, E);
            if (E.empty()) {
                while (not undo_stack_.empty()) { undo_stack_.pop(); }
                this->treat_new_node(new_node, true);                
            } else {
                this->undo_swapping();

                HalfEdgeHandle he1 = NULL, he2 = NULL;
                he_iter = he_start;
                do {
                    if (he_iter->pair()->origin() == n1) {
                        he1 = he_iter;
                    }
                    if (he_iter->pair()->origin() == n2) {
                        he2 = he_iter;
                    }
                    he_iter = he_iter->pair()->next();
                } while (he_iter != he_start);
                HalfEdgeHandle he23 = NULL, he31 = NULL, he14 = NULL, he42 = NULL;
                if (build_123) {
                    he23 = he2->next();
                    he31 = he1->pair()->prev();
                }
                if (build_142) {
                    he14 = he1->next();
                    he42 = he2->pair()->prev();
                }
                this->remove_node(new_node);
                EdgeHandle new_edge = mesh_.add_edge(n1, n2, bseginfo);
                if (build_123) {
                    HalfEdgeHandle he12 = new_edge->halfedge_with_origin(n1);
                    this->add_face(he12, he23, he31);
                }
                if (build_142) {
                    HalfEdgeHandle he21 = new_edge->halfedge_with_origin(n2);
                    this->add_face(he21, he14, he42);
                }

                // IMPORTANT TODO AND A BUG AS IT IS NOW!!!
                // Recover somehow the handle to the original bad face
                // The following line usually gives the correct handle, sometimes it does not
                FaceHandle new_bad_face = this->get_bad_face();
                
                while (not E.empty()) {
                    HalfEdgeHandle he = E.top();
                    E.pop();
                    if (bad_face_area > max_face_area_ || this->split_permitted(he, d)) {
                        enc_hedges_.insert(he);
                    }
                }
                if (not enc_hedges_.empty()) {
                    this->split_encroached_boundary_edges(true);
                } else {
                    this->dequeue_bad_face(new_bad_face);
                }
            }
        } else { // loc == OUTSIDE_MESH
            std::clog << "Point outside mesh, splitting the boundary edge\n";
            HalfEdgeHandle he = static_cast<HalfEdgeHandle>(entity_to_kill);
            enc_hedges_.insert(he);
            this->split_encroached_boundary_edges(false);
        }
    }
}

template <typename Mesh>
void Mesher<Mesh>::split_encroached_boundary_edges (bool check_quality)
{
    while (not enc_hedges_.empty()) {        
        HalfEdgeHandle he = *enc_hedges_.begin();
        enc_hedges_.erase(enc_hedges_.begin());

        HalfEdgeHandle hen = he->next();
        HalfEdgeHandle hep = he->prev();
        
        double split;
        bool acutedest = hen->edge()->is_boundary() && not hep->edge()->is_boundary();
        bool acuteorig = not hen->edge()->is_boundary() && hep->edge()->is_boundary();
        if ( acutedest || acuteorig) {
            double l = he->edge()->length();
            double nearestpoweroftwo = 1.0;
            while (l > 3.0*nearestpoweroftwo) {
                nearestpoweroftwo *= 2.0;
            }
            while (l < 1.5 * nearestpoweroftwo) {
                nearestpoweroftwo *= 0.5;
            }
            split = nearestpoweroftwo / l;
            if (acutedest) {
                split = 1.0 - split;
            }
        } else {
            split = 0.5;
        }

        Point2 porig, pdest;
        he->vertices(porig, pdest);
        Point2 split_point(porig.x()+split*(pdest.x()-porig.x()), porig.y()+split*(pdest.y()-porig.y()));

        NodeHandle new_node = this->split_edge(he->edge(), split_point);

        HalfEdgeHandle he1 = hen->prev();
        HalfEdgeHandle he2 = hep->next();

        this->recursive_swap_delaunay(hen, false);
        this->recursive_swap_delaunay(hep, false);

        this->treat_new_node(new_node, check_quality);

        if (he1->edge()->is_encroached_upon(he1->prev()->origin()->position())) {
            enc_hedges_.insert(he1);
        }
        if (he2->edge()->is_encroached_upon(he2->prev()->origin()->position())) {
            enc_hedges_.insert(he2);
        }
    }
}

template <typename Mesh>
void Mesher<Mesh>::collect_encroached_boundary_edges ()
{
    HalfEdgeHandle bhe_start = mesh_.boundary_halfedge();
    HalfEdgeHandle bhe_iter = bhe_start;
    do {
        HalfEdgeHandle he = bhe_iter->pair();
        BOOST_ASSERT(he->face() != NULL);
        if (he->edge()->is_encroached_upon(he->prev()->origin()->position())) {
            enc_hedges_.insert(he);
        }
        bhe_iter = bhe_iter->next();
    } while (bhe_iter != bhe_start);
}

template <typename Mesh>
void Mesher<Mesh>::collect_encroached_boundary_edges(NodeHandle n, std::stack<HalfEdgeHandle>& E)
{
    HalfEdgeHandle he_start = n->out_he();
    HalfEdgeHandle he_iter = he_start;
    do {
        if (he_iter->face() != NULL) {
            EdgeHandle e = he_iter->next()->edge();
            if (e->is_boundary() && e->is_encroached_upon(n->position())) {
                E.push(he_iter->next());
            }
        }
        he_iter = he_iter->pair()->next();
    } while (he_iter != he_start);
}

template <typename Mesh>
void Mesher<Mesh>::treat_new_node (NodeHandle n, bool check_quality)
{
    HalfEdgeHandle he_start = n->out_he();
    HalfEdgeHandle he_iter = he_start;
    do {
        FaceHandle f = he_iter->face();
        if (f != NULL) {
            EdgeHandle e = he_iter->next()->edge();
            if (e->is_boundary() && e->is_encroached_upon(n->position())) {
                enc_hedges_.insert(he_iter->next());
            } else if (check_quality) {
                enqueue_bad_face(f);
            }
        }
        he_iter = he_iter->pair()->next();
    } while (he_iter != he_start);    
}

template <typename Mesh>
bool Mesher<Mesh>::split_permitted (HalfEdgeHandle he, double d)
{
    if (not he->prev()->edge()->is_boundary() && not he->next()->edge()->is_boundary()) {
        return true;
    }
    if (he->prev()->edge()->is_boundary() && he->next()->edge()->is_boundary()) {
        return true;
    }
    NodeHandle n = NULL;
    HalfEdgeHandle other_he = NULL;
    Point2 p1, p2, p3;
    if (he->prev()->edge()->is_boundary()) {
        other_he = he->prev();
        n = he->origin();
        p1 = other_he->origin()->position();
        p2 = he->origin()->position();
        p3 = he->pair()->origin()->position();
    } else {
        other_he = he->next();
        n = he->pair()->origin();
        p1 = he->origin()->position();
        p2 = other_he->origin()->position();
        p3 = other_he->pair()->origin()->position();
    }
    
    double l = he->edge()->length();
    double other_l =other_he->edge()->length();
    if (std::abs(l-other_l) > 0.001) {
        return true;
    }

    double ll = ::distance(p1, p3);
    double phi_min = std::acos((l*l + other_l*other_l - ll*ll)/(2.0*l*other_l));
    double r_min = l*std::sin(phi_min/2.0);
    if (r_min > d) {
        return true;
    }
    return false;
}


template <typename Mesh>
void Mesher<Mesh>::make_cdt ()
{
    boost::unordered_set<EdgeHandle> flippable_edges;
    BOOST_FOREACH(EdgeHandle e, mesh_.edges_)
    {
        if (e->is_swappable<kernel_type>() && not e->is_delaunay<kernel_type>()) {
            flippable_edges.insert(e);
        }
    }
    
    while (not flippable_edges.empty()) {
        EdgeHandle e = *flippable_edges.begin();
        flippable_edges.erase(flippable_edges.begin());
        if (not e->is_swappable<kernel_type>() || e->is_delaunay<kernel_type>())
            continue;
        HalfEdgeHandle he = e->he1();
        flippable_edges.insert(he->next()->edge());
        flippable_edges.insert(he->prev()->edge());
        flippable_edges.insert(he->pair()->next()->edge());
        flippable_edges.insert(he->pair()->prev()->edge());
        mesh_.swap_edge(e);
    }
}

template <typename Mesh>
void Mesher<Mesh>::recursive_swap_delaunay (HalfEdgeHandle he, bool save_to_undo_stack)
{
    if (not he->edge()->is_swappable<kernel_type>() || he->edge()->is_delaunay<kernel_type>()) {
        return;
    }
    
    HalfEdgeHandle he1 = he->pair()->next();
    HalfEdgeHandle he2 = he->pair()->prev();
    
    EdgeHandle new_edge = this->swap_edge(he->edge());
    if (save_to_undo_stack) {
        undo_stack_.push(new_edge);
    }
    
    this->recursive_swap_delaunay(he1, save_to_undo_stack);
    this->recursive_swap_delaunay(he2, save_to_undo_stack);
}

template <typename Mesh>
void Mesher<Mesh>::enqueue_bad_face(FaceHandle f)
{
    if (f != NULL) {
        double min_angle = f->minimum_angle();
        bool restricted = f->is_restricted();
        if (f->area<kernel_type>() > max_face_area_ || (min_angle < min_angle_ && not restricted)) {
            bad_faces_.insert(f);
        }
    }
}

template <typename Mesh>
void Mesher<Mesh>::dequeue_bad_face(FaceHandle f)
{
    if (f != NULL) {
//        double min_angle = f->minimum_angle();
//        bool restricted = f->is_restricted();
//        if (f->compute_area<kernel_type>() > max_face_area_ || (min_angle < min_angle_ && not restricted)) {
            bad_faces_.erase(f);
//        }
    }
}

template <typename Mesh>
FaceHandle Mesher<Mesh>::add_face(HalfEdgeHandle he1, HalfEdgeHandle he2, HalfEdgeHandle he3)
{
    FaceHandle f = mesh_.add_face(he1, he2, he3);
    this->enqueue_bad_face(f);
    return f;
}

template <typename Mesh>
EdgeHandle Mesher<Mesh>::swap_edge(EdgeHandle e)
{
    this->dequeue_bad_face(e->he1()->face());
    this->dequeue_bad_face(e->he2()->face());
    e = mesh_.swap_edge(e);
    this->enqueue_bad_face(e->he1()->face());
    this->enqueue_bad_face(e->he2()->face());  
    return e;
}

template <typename Mesh>
NodeHandle Mesher<Mesh>::split_edge(EdgeHandle e)
{
    this->dequeue_bad_face(e->he1()->face());
    this->dequeue_bad_face(e->he2()->face());
    NodeHandle new_node = mesh_.split_edge(e);
    HalfEdgeHandle he_start = new_node->out_he();
    HalfEdgeHandle he_iter = he_start;
    do {
        this->enqueue_bad_face(he_iter->face());
        he_iter = he_iter->pair()->next();
    } while (he_iter != he_start);
    return new_node;
}

template <typename Mesh>
NodeHandle Mesher<Mesh>::split_edge(EdgeHandle e, Point2 const& p)
{
    this->dequeue_bad_face(e->he1()->face());
    this->dequeue_bad_face(e->he2()->face());
    NodeHandle new_node = mesh_.split_edge(e, p);
    HalfEdgeHandle he_start = new_node->out_he();
    HalfEdgeHandle he_iter = he_start;
    do {
        this->enqueue_bad_face(he_iter->face());
        he_iter = he_iter->pair()->next();
    } while (he_iter != he_start);
    return new_node;
}

template <typename Mesh>
NodeHandle Mesher<Mesh>::split_face(FaceHandle f, Point2 const& p)
{
    this->dequeue_bad_face(f);
    NodeHandle new_node = mesh_.split_face(f, p);
    HalfEdgeHandle he_start = new_node->out_he();
    HalfEdgeHandle he_iter = he_start;
    do {
        this->enqueue_bad_face(he_iter->face());
        he_iter = he_iter->pair()->next();
    } while (he_iter != he_start);
    return new_node;
}

template <typename Mesh>
void Mesher<Mesh>::remove_node(NodeHandle n)
{
    HalfEdgeHandle he_start = n->out_he();
    HalfEdgeHandle he_iter = he_start;
    do {
        this->dequeue_bad_face(he_iter->face());
        he_iter = he_iter->pair()->next();
    } while (he_iter != he_start);
    mesh_.remove_node(n);
}

template <typename Mesh>
void Mesher<Mesh>::undo_swapping()
{
    while (not undo_stack_.empty()) {
        this->swap_edge(undo_stack_.top());
        undo_stack_.pop();
    }
}

template <typename Mesh>
FaceHandle Mesher<Mesh>::get_bad_face ()
{
    if (bad_faces_.empty()) {
        return NULL;
    } else {
        return *bad_faces_.begin();
    }
}


#endif // __MESHER_H_INCLUDED__
