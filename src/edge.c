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

#include "edge.h"
#include "point2.h"
#include "predicates.h"

Edge * edge_new( void )
{
    Edge *edge = g_slice_new0( Edge );
    if ( edge != NULL )
    {
        edge->he[0].pair = &edge->he[1];
        edge->he[1].pair = &edge->he[0];
        edge->he[0].edge = edge->he[1].edge = edge;
        edge->he[0].next = &edge->he[1];
        edge->he[1].next = &edge->he[0];
        edge->he[0].previous = &edge->he[1];
        edge->he[1].previous = &edge->he[0];
    }
    return edge;
}


void edge_set_nodes( Edge *edge, Node *n1, Node *n2 )
{
    g_return_if_fail( edge != NULL );

    edge->he[0].origin = n1;
    edge->he[1].origin = n2;
}


void edge_free( Edge *edge )
{
    g_slice_free( Edge, edge );
}


void edge_print( const Edge *edge )
{
    g_return_if_fail( edge != NULL );

    g_print( "edge %p:\n", edge );
    g_print( "    " ); halfedge_print( &edge->he[0] );
    g_print( "    " ); halfedge_print( &edge->he[1] );
}


void halfedge_print( const HalfEdge *he )
{
    g_return_if_fail( he != NULL );

    g_print( "he %p: pair = %p, edge = %p, origin = %p (%f,%f), element = %p, next = %p, previous = %p\n",
            he, he->pair, he->edge, he->origin, NODE_POSITION(he->origin)->x,
            NODE_POSITION(he->origin)->y, he->element, he->next, he->previous );
}


gdouble edge_length( const Edge *edge )
{
    Point2 *p1 = NODE_POSITION( edge->he[0].origin );
    Point2 *p2 = NODE_POSITION( edge->he[1].origin );
    return point2_distance( p1, p2 );
}


gboolean halfedge_is_at_boundary( const HalfEdge *he )
{
    return he->element == NULL;
}


gboolean edge_is_at_boundary( const Edge *edge )
{
    return halfedge_is_at_boundary( &(edge->he[0]) ) || halfedge_is_at_boundary( &(edge->he[1]) );
}


/* 
 *       n1
 *     / | \
 *   /   |   \
 * n2    |    n4
 *   \   |   /
 *     \ | /
 *       n3
 */
gboolean edge_is_swappable( Edge *edge )
{
    g_return_val_if_fail( edge != NULL, FALSE );

    if ( edge_is_at_boundary( edge ) )
        return FALSE;

    Node *n1 = edge->he[0].origin;
    Node *n2 = edge->he[1].previous->origin;
    Node *n3 = edge->he[1].origin;
    Node *n4 = edge->he[0].previous->origin;

    double p[4][2];
    p[0][0] = NODE_POSITION(n1)->x;
    p[0][1] = NODE_POSITION(n1)->y;
    p[1][0] = NODE_POSITION(n2)->x;
    p[1][1] = NODE_POSITION(n2)->y;
    p[2][0] = NODE_POSITION(n3)->x;
    p[2][1] = NODE_POSITION(n3)->y;
    p[3][0] = NODE_POSITION(n4)->x;
    p[3][1] = NODE_POSITION(n4)->y;

    if ( orient2d_test( p[1], p[2], p[3] ) <= 0.0 ||
         orient2d_test( p[3], p[0], p[1] ) <= 0.0 )
        return FALSE;
 
    return TRUE;
}


/* 
 *       n1
 *     / | \
 *   /   |   \
 * n2    |    n4
 *   \   |   /
 *     \ | /
 *       n3
 */
gboolean edge_is_delaunay( Edge *edge )
{
    g_return_val_if_fail( edge != NULL, FALSE );

    Node *n1 = edge->he[0].origin;
    Node *n2 = edge->he[1].previous->origin;
    Node *n3 = edge->he[1].origin;
    Node *n4 = edge->he[0].previous->origin;

    double p[4][2];
    p[0][0] = NODE_POSITION(n1)->x;
    p[0][1] = NODE_POSITION(n1)->y;
    p[1][0] = NODE_POSITION(n2)->x;
    p[1][1] = NODE_POSITION(n2)->y;
    p[2][0] = NODE_POSITION(n3)->x;
    p[2][1] = NODE_POSITION(n3)->y;
    p[3][0] = NODE_POSITION(n4)->x;
    p[3][1] = NODE_POSITION(n4)->y;

    if ( incircle_test( p[0], p[1], p[2], p[3] ) > 0.0 )
        return FALSE;

    return TRUE;
}


