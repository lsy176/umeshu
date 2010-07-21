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


void node_print( Node *node )
{
    g_return_if_fail( node != NULL );

    g_print( "node %p: position = [%f,%f], out_halfedge = %p\n",
            node, NODE_POSITION(node)->x, NODE_POSITION(node)->y, node->out_halfedge );
}


gboolean node_is_isolated( const Node *node )
{
    return node->out_halfedge == NULL;
}


HalfEdge * node_is_at_boundary( Node *node )
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


guint node_degree( Node *node )
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

