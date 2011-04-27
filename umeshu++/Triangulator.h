#ifndef __TRIANGULATOR_H_INCLUDED__
#define __TRIANGULATOR_H_INCLUDED__ 

#include "Exceptions.h"
#include "Polygon.h"

#include <boost/assert.hpp>
#include <boost/unordered_set.hpp>
#include <boost/utility.hpp>
#include <boost/foreach.hpp>

template <typename Mesh>
class Triangulator
{
public:
    typedef typename Mesh::kernel_type kernel_type;

    void triangulate(Polygon const &poly, Mesh &mesh);

    struct triangulator_error : virtual umeshu_error { };

private:
    HalfEdgeHandle add_polygon_to_mesh(Polygon const &poly, Mesh &mesh);
    bool halfedge_origin_is_convex(HalfEdgeHandle he) const;
    bool halfedge_origin_is_ear(HalfEdgeHandle he) const;

    void classify_vertices(HalfEdgeHandle bhe);

    boost::unordered_set<HalfEdgeHandle> reflex_vertices;
    boost::unordered_set<HalfEdgeHandle> ears;
};

template <typename Mesh>
HalfEdgeHandle Triangulator<Mesh>::add_polygon_to_mesh(Polygon const &poly, Mesh &mesh)
{
    Polygon::const_vertex_iterator iter = poly.vertices_begin();
    NodeHandle first_node = mesh.add_node(*iter);
    ++iter;
    NodeHandle prev_node = first_node, cur_node;
    for (; iter != poly.vertices_end(); ++iter) {
        cur_node = mesh.add_node(*iter);
        mesh.add_edge(prev_node, cur_node, NULL);
        prev_node = cur_node;
    }
    EdgeHandle last_edge = mesh.add_edge(prev_node, first_node, NULL);
    return last_edge->halfedge_with_origin(prev_node);
}

template <typename Mesh>
void Triangulator<Mesh>::classify_vertices(HalfEdgeHandle bhe)
{
    boost::unordered_set<HalfEdgeHandle> convex_vertices;

    HalfEdgeHandle he_iter = bhe;
    do {
        if (halfedge_origin_is_convex(he_iter)) {
            convex_vertices.insert(he_iter);
        } else {
            reflex_vertices.insert(he_iter);
        }
        he_iter = he_iter->next();
    } while (he_iter != bhe);

    BOOST_FOREACH(HalfEdgeHandle conv_he, convex_vertices) {
        if (this->halfedge_origin_is_ear(conv_he)) {
            ears.insert(conv_he);
        }
    }
}

template <typename Mesh>
void Triangulator<Mesh>::triangulate(Polygon const &poly, Mesh &mesh)
{
    if (poly.number_of_vertices() < 3) {
        throw triangulator_error();
    }

    this->classify_vertices(this->add_polygon_to_mesh(poly, mesh));

    while (not ears.empty()) {
        HalfEdgeHandle he2 = *ears.begin();
        HalfEdgeHandle he1 = he2->prev();
        HalfEdgeHandle he5 = he2->next();
        NodeHandle n1 = he1->origin();
        NodeHandle n3 = he5->origin();

        // since we will cut it off, remove the ear from ears. Also, we have to
        // erase he1 and he5 from all the sets, since after cutting the ear we
        // will have to update their info anyway
        ears.erase(he2);
        ears.erase(he1);
        reflex_vertices.erase(he1);
        ears.erase(he5);
        reflex_vertices.erase(he5);
       
        // if this is not the last ear, i.e., only one triangle left to
        // triangulate
        if (he5 != he1->prev()) {
            EdgeHandle e = mesh.add_edge(n3, n1, NULL);
            HalfEdgeHandle he3 = e->halfedge_with_origin(n3);
            HalfEdgeHandle he4 = he3->pair();
            mesh.add_face(he1, he2, he3);

            bool he4_is_convex = false, he5_is_convex = false;
            if (halfedge_origin_is_convex(he4)) {
                he4_is_convex = true;
            } else {
                reflex_vertices.insert(he4);
            }
            if (halfedge_origin_is_convex(he5)) {
                he5_is_convex = true;
            } else {
                reflex_vertices.insert(he5);
            }
            if (he4_is_convex && halfedge_origin_is_ear(he4)) {
                ears.insert(he4);
            }
            if (he5_is_convex && halfedge_origin_is_ear(he5)) {
                ears.insert(he5);
            }
        } else {
            mesh.add_face(he1, he2, he5);
        }
    }
}

template <typename Mesh>
bool Triangulator<Mesh>::halfedge_origin_is_convex(HalfEdgeHandle he) const
{
    Point2 p = he->pair()->origin()->position();
    HalfEdgeHandle hep = he->prev();

    if (kernel_type::oriented_side(hep, p) == ON_POSITIVE_SIDE) {
        return true;
    } else {
        return false;
    }
}

template <typename Mesh>
bool Triangulator<Mesh>::halfedge_origin_is_ear(HalfEdgeHandle he) const
{
    BOOST_ASSERT(this->halfedge_origin_is_convex(he));

    NodeHandle n1 = he->prev()->origin();
    NodeHandle n2 = he->origin();
    NodeHandle n3 = he->pair()->origin();
    Point2 p1 = n1->position();
    Point2 p2 = n2->position();
    Point2 p3 = n3->position();

    /* to test if a vertex is an ear, we just need to iterate over reflex
     * vertices */
    BOOST_FOREACH(HalfEdgeHandle refl_he, reflex_vertices) {
        NodeHandle refl_node = refl_he->origin();
        if (refl_node != n1 && refl_node != n3) {
            Oriented_side os1, os2, os3;
            Point2 p = refl_node->position();
            os1 = kernel_type::oriented_side(p1, p2, p);
            os2 = kernel_type::oriented_side(p2, p3, p);
            os3 = kernel_type::oriented_side(p3, p1, p);
            if (os1 != ON_NEGATIVE_SIDE && os2 != ON_NEGATIVE_SIDE && os3 != ON_NEGATIVE_SIDE ) {
                return false;
            }
        }
    }
    return true;
}

#endif /* __TRIANGULATOR_H_INCLUDED__ */
