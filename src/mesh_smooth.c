/* Copyright (c) 2010 Vladimir Chalupecky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <math.h>

#include "node.h"
#include "edge.h"
#include "element.h"
#include "mesh.h"
#include "mesh_smooth.h"
#include "point2.h"


typedef struct 
{
    gdouble h;
    gdouble alpha;
    gdouble (*f)(gdouble);
} SmoothData;


static void node_smooth( Node *node, SmoothData *data );
static void node_smooth_by_element_quality( Node *node, void *data );
static void node_smooth_by_edge_length( Node *node, gdouble *h );
static inline gdouble laplace_f( gdouble d );
static inline gdouble bossen_heckbert_f( gdouble d );

void mesh_smooth( Mesh *mesh, enum MeshSmoothMethod method, guint niter, gdouble area )
{
    g_return_if_fail( mesh != NULL );

    SmoothData data;
    guint iter;
    data.h = sqrt( 4.0*area / SQRT3 );  /* edge length in an equilateral triangle with the given area */
    switch ( method )
    {
        case LAPLACIAN_SMOOTHING:
            data.f = laplace_f;
            data.alpha = 0.0;
            for ( iter = 0; iter < niter; iter++ )
                g_list_foreach( mesh->nodes, (GFunc) node_smooth, &data );
            break;
        case ELEMENT_QUALITY_SMOOTHING:
            for ( iter = 0; iter < niter; iter++ )
                g_list_foreach( mesh->nodes, (GFunc) node_smooth_by_element_quality, NULL );
            break;
        case EDGE_LENGTH_SMOOTHING:
            g_list_foreach( mesh->nodes, (GFunc) node_smooth_by_edge_length, &data.h );
            break;
        case BOSSEN_HECKBERT_SMOOTHING:
            data.f = bossen_heckbert_f;
            data.alpha = 0.2;
            g_list_foreach( mesh->nodes, (GFunc) node_smooth, &data );
            break;
    }
}


static void node_smooth( Node *node, SmoothData *data )
{
    g_return_if_fail( node != NULL );
    g_return_if_fail( data != NULL );

    /* do not move nodes that are at the boundary */
    if ( node_is_at_boundary( node ) )
        return;

    Point2 *center_point = NODE_POSITION(node);
    HalfEdge *in_he = node->out_halfedge->pair;
    HalfEdge *start_he = in_he;
    guint edge_count = 0;
    gdouble translation[2] = { 0.0, 0.0 };
    do
    {
        // we assume there is no loop edge in the mesh
        Point2 *neighbor_point = NODE_POSITION(in_he->origin);
        gdouble normalized_distance = point2_distance( center_point, neighbor_point )/data->h;
        gdouble f = data->f( normalized_distance );
        gdouble u[2];
        u[0] = ( center_point->x - neighbor_point->x )/normalized_distance;
        u[1] = ( center_point->y - neighbor_point->y )/normalized_distance;

        translation[0] += f*u[0];
        translation[1] += f*u[1];
        edge_count++;
        in_he = in_he->next->pair;
    }
    while ( in_he != start_he );

    if ( data->alpha == 0.0 )
    {
        translation[0] /= edge_count;
        translation[1] /= edge_count;
    }
    else
    {
        translation[0] *= data->alpha;
        translation[1] *= data->alpha;
    }

    gdouble w = 0.3;
    NODE_POSITION(node)->x = (1.0 - w)*NODE_POSITION(node)->x + w*(NODE_POSITION(node)->x + translation[0]);
    NODE_POSITION(node)->y = (1.0 - w)*NODE_POSITION(node)->y + w*(NODE_POSITION(node)->y + translation[1]);
}


static void node_smooth_by_element_quality( Node *node, void *data )
{
    g_return_if_fail( node != NULL );

    if ( node_is_at_boundary( node ) )
        return;

    gdouble aux_position[2];
    aux_position[0] = aux_position[1] = 0.0;
    
    HalfEdge *in_he = node->out_halfedge->pair;
    HalfEdge *start_he = in_he;
    gdouble weight_accum = 0.0;
    do
    {
        // we assume there is no loop edge in the mesh
        gdouble weight = 1.0 / element_fem_quality( in_he->element );
        /* weight *= weight; */
        weight_accum += weight;

        HalfEdge *f = in_he->previous;
        Point2 ideal_p;
        halfedge_ideal_triangle_point( f, edge_length( f->edge ), &ideal_p );

        aux_position[0] += weight * ideal_p.x;
        aux_position[1] += weight * ideal_p.y;

        in_he = in_he->next->pair;
    }
    while ( in_he != start_he );

    aux_position[0] /= weight_accum;
    aux_position[1] /= weight_accum;

    gdouble w = 0.6;
    NODE_POSITION(node)->x = (1.0-w)*NODE_POSITION(node)->x + w*aux_position[0];
    NODE_POSITION(node)->y = (1.0-w)*NODE_POSITION(node)->y + w*aux_position[1];
}


static void node_smooth_by_edge_length( Node *node, gdouble *h )
{
    g_return_if_fail( node != NULL );

    if ( node_is_at_boundary( node ) )
        return;

    Point2 *p = NODE_POSITION(node);

    gdouble aux_position[2];
    aux_position[0] = aux_position[1] = 0.0;
    
    HalfEdge *in_he = node->out_halfedge->pair;
    HalfEdge *start_he = in_he;
    gdouble weight_accum = 0.0;
    do
    {
        Point2 *pj = NODE_POSITION(in_he->origin);
        gdouble v[2];
        v[0] = p->x - pj->x;
        v[1] = p->y - pj->y;
        gdouble normalized_distance = sqrt(v[0]*v[0]+v[1]*v[1]) / (*h);
        gdouble weight = 1.0; //0.2*bossen_heckbert_f( normalized_distance );
        weight_accum += weight;
        gdouble ideal_position[2];
        ideal_position[0] = pj->x + v[0]/normalized_distance;
        ideal_position[1] = pj->y + v[1]/normalized_distance;

        aux_position[0] += weight * ideal_position[0];
        aux_position[1] += weight * ideal_position[1];

        in_he = in_he->next->pair;
    }
    while ( in_he != start_he );

    aux_position[0] /= weight_accum;
    aux_position[1] /= weight_accum;

    gdouble w = 0.8;
    NODE_POSITION(node)->x = (1.0-w)*NODE_POSITION(node)->x + w*aux_position[0];
    NODE_POSITION(node)->y = (1.0-w)*NODE_POSITION(node)->y + w*aux_position[1];
}


static inline gdouble laplace_f( gdouble d )
{
    return -d;
}

static inline gdouble bossen_heckbert_f( gdouble d )
{
    gdouble d4 = d*d*d*d;
    return ( 1.0 - d4)*exp(-d4);
}

