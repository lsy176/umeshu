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

#include <math.h>
#include <stdio.h>

#include "edge.h"
#include "mesh_macros.h"
#include "node.h"




Node * node_new( gdouble x, gdouble y )
{
    Node *node = g_slice_new( Node );
    NODE_POSITION(node)->x = x;
    NODE_POSITION(node)->y = y;
    node->out_halfedge = NULL;
    return node;
}


void node_free( Node *node )
{
    g_slice_free( Node, node );
}


void node_print( const Node *node )
{
    g_return_if_fail( node != NULL );

    g_print( "node %p: position = [%f,%f], out_halfedge = %p\n",
            node, NODE_POSITION(node)->x, NODE_POSITION(node)->y, node->out_halfedge );
}


gboolean node_is_isolated( const Node *node )
{
    return node->out_halfedge == NULL;
}


HalfEdge * node_is_at_boundary( const Node *node )
{
    g_return_val_if_fail( node != NULL, NULL );

    if ( node_is_isolated( node ) )
        return NULL;

    HalfEdge *current_he = node->out_halfedge;
    do
    {
        if ( halfedge_is_at_boundary( current_he ) )
            return current_he;
        current_he = current_he->pair->next;
    }
    while ( current_he != node->out_halfedge );

    return NULL;
}


guint node_degree( const Node *node )
{
    g_return_val_if_fail( node != NULL, 0 );

    guint degree = 0;
    if ( ! node_is_isolated( node ) )
    {
        HalfEdge *current_he = node->out_halfedge;
        do
        {
            degree++;
            current_he = current_he->pair->next;
        }
        while ( current_he != node->out_halfedge );
    }
    return degree;
}


guint node_virtual_degree( const Node *node )
{
    g_return_val_if_fail( node != NULL, 0 );

    return node_degree( node ) + ( 6 - node_ideal_degree( node ) );
}


guint node_ideal_degree( const Node *node )
{
    g_return_val_if_fail( node != NULL, 0 );

    /* ideal degree for an interior point */
    guint D = 6;

    HalfEdge *he2 = node_is_at_boundary( node );
    /* compute ideal degree for a boundary node */
    if ( he2 != NULL )
    {
        HalfEdge *he1 = he2->previous;

        Point2 *p1 = NODE_POSITION(he1->origin);
        Point2 *p2 = NODE_POSITION(he2->origin);
        Point2 *p3 = NODE_POSITION(he2->pair->origin);

        gdouble v1[2], v2[2];
        v1[0] = p1->x - p2->x;
        v1[1] = p1->y - p2->y;
        v2[0] = p3->x - p2->x;
        v2[1] = p3->y - p2->y;

        gdouble angle = atan2( v1[0]*v2[1] - v2[0]*v1[1], v1[0]*v2[0] + v1[1]*v2[1] );
        if ( angle < 0.0 )
            angle = 2.0*G_PI + angle;

        if ( angle <= RADIANS(84.85) )
            D = 2;
        else if ( RADIANS(84.85) < angle && angle <= RADIANS(146.97) )
            D = 3;
        else if ( RADIANS(146.97) < angle && angle <= RADIANS(207.85) )
            D = 4;
        else if ( RADIANS(207.85) < angle && angle <= RADIANS(268.33) )
            D = 5;
        else if ( RADIANS(268.33) < angle && angle <= RADIANS(328.63) )
            D = 6;
        else if ( RADIANS(328.63) < angle && angle < RADIANS(360.0) )
            D = 7;
        else
        {
            D = 7;
            g_warning( "node_ideal_degree: strange value of angle" );
        }
    }

    return D;
}
