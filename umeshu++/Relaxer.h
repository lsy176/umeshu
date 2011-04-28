#ifndef __RELAXER_H_INCLUDED__
#define __RELAXER_H_INCLUDED__ 

#include "Edge.h"
#include "Exceptions.h"
#include "Node.h"
#include "Point2.h"

#include <cmath>

template<typename Mesh>
class Relaxer
{
public:
    typedef typename Mesh::kernel_type kernel_type;

    void relax(Mesh &mesh);

    struct relaxer_error : virtual umeshu_error { };

private:
    int ideal_degree(NodeHandle n) const;

    int virtual_degree(NodeHandle n) const {
        return n->degree() + (6 - ideal_degree(n));
    }

    int edge_relaxation_index(EdgeHandle e) const;
};

template<typename Mesh>
void Relaxer<Mesh>::relax(Mesh &mesh)
{
    for (int relax = 4; relax >=2; --relax)
    {
        bool swapped = true;
        while (swapped) {
            swapped = false;
            typename Mesh::edges_iterator edge_iter = mesh.edges_begin();
            while(edge_iter != mesh.edges_end()) {
                EdgeHandle edge = *edge_iter++;
                if (edge->is_boundary()) {
                    continue;
                }

                int index = edge_relaxation_index(edge);
                if (index > relax) {
                    if (edge->is_swappable<kernel_type>()) {
                        swapped = true;
                        mesh.swap_edge(edge);
                    }
                }
            }
        }
    }

    // gboolean swapped = TRUE;
    // while ( swapped )
    // {
        // swapped = FALSE;
        // GList * edges_iter = mesh->edges;
        // while( edges_iter != NULL )
        // {
            // Edge *edge = EDGE( edges_iter->data );
            // edges_iter = g_list_next( edges_iter );

            // if ( edge_is_at_boundary( edge ) )
                // continue;

            // Node *n1 = edge->he[0].origin;
            // if ( node_is_at_boundary( n1 ) )
                // continue;
            // Node *n2 = edge->he[1].origin;
            // if ( node_is_at_boundary( n2 ) )
                // continue;

            // if ( node_degree( n1 ) + node_degree( n2 ) == 10 )
            // {
                // mesh_collapse_edge( mesh, edge );
                // edges_iter = mesh->edges;
            // }
        // }
    // }
}

template<typename Mesh>
int Relaxer<Mesh>::ideal_degree(NodeHandle n) const
{
    // ideal degree for an interior point
    int D = 6;

    HalfEdgeHandle he2 = n->is_boundary();
    // compute ideal degree for a boundary node
    if (he2 != NULL) {
        HalfEdgeHandle he1 = he2->prev();

        Point2 const &p1 = he1->origin()->position();
        Point2 const &p2 = he2->origin()->position();
        Point2 const &p3 = he2->pair()->origin()->position();

        double v1[2], v2[2];
        v1[0] = p1.x() - p2.x();
        v1[1] = p1.y() - p2.y();
        v2[0] = p3.x() - p2.x();
        v2[1] = p3.y() - p2.y();

        double angle = ::atan2( v1[0]*v2[1] - v2[0]*v1[1], v1[0]*v2[0] + v1[1]*v2[1] );
        if (angle < 0.0) {
            angle = 2.0*M_PI + angle;
        }

        if (angle <= degrees_to_radians(84.85)) {
            D = 2;
        } else if (degrees_to_radians(84.85) < angle && angle <= degrees_to_radians(146.97)) {
            D = 3;
        } else if (degrees_to_radians(146.97) < angle && angle <= degrees_to_radians(207.85)) {
            D = 4;
        } else if (degrees_to_radians(207.85) < angle && angle <= degrees_to_radians(268.33)) {
            D = 5;
        } else if (degrees_to_radians(268.33) < angle && angle <= degrees_to_radians(328.63)) {
            D = 6;
        } else if (degrees_to_radians(328.63) < angle && angle < degrees_to_radians(360.0)) {
            D = 7;
        } else {
            D = 7;
            std::cerr << "Relaxer::ideal_degree: strange value of angle\n";
        }
    }

    return D;
}

template<typename Mesh>
int Relaxer<Mesh>::edge_relaxation_index(EdgeHandle edge) const
{
    NodeHandle n1, n2, n3, n4;

    n1 = edge->he1()->origin();
    n2 = edge->he2()->origin();
    n3 = edge->he1()->prev()->origin();
    n4 = edge->he2()->prev()->origin();

    return virtual_degree(n1) + virtual_degree(n2) - virtual_degree(n3) - virtual_degree(n4);
}

#endif /* __RELAXER_H_INCLUDED__ */
