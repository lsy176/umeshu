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

#include "node.h"
#include "edge.h"
#include "mesh.h"
#include "mesh_output.h"

static gint edge_relaxation_index( const Edge *edge );

void mesh_relax( Mesh *mesh )
{
    g_debug( "mesh_relax: begin" );

    gint relax;
    for ( relax = 4; relax >=2; relax-- )
    {
        gboolean swapped = TRUE;
        while ( swapped )
        {
            swapped = FALSE;
            GList *edges_iter = mesh->edges;
            while( edges_iter != NULL )
            {
                Edge *edge = EDGE( edges_iter->data );
                edges_iter = g_list_next( edges_iter );

                g_debug( "mesh_relax: checking edge %p", edge );
                edge_print( edge );
                if ( edge_is_at_boundary( edge ) )
                    continue;

                gint index = edge_relaxation_index( edge );
                g_debug( "mesh_relax: relaxation index = %d", index );
                if ( index > relax )
                {
                    g_debug( "mesh_relax: index > relax" );
                    if ( edge_is_swappable( edge ) )
                    {
                        g_debug( "mesh_relax: edge is swappable, going to swap it" );
                        swapped = TRUE;
                        mesh_swap_edge( mesh, edge );
                    }
                }
            }
        }
    }

    gboolean swapped = TRUE;
    while ( swapped )
    {
        swapped = FALSE;
        GList * edges_iter = mesh->edges;
        while( edges_iter != NULL )
        {
            Edge *edge = EDGE( edges_iter->data );
            edges_iter = g_list_next( edges_iter );

            if ( edge_is_at_boundary( edge ) )
                continue;

            Node *n1 = edge->he[0].origin;
            if ( node_is_at_boundary( n1 ) )
                continue;
            Node *n2 = edge->he[1].origin;
            if ( node_is_at_boundary( n2 ) )
                continue;

            if ( node_degree( n1 ) + node_degree( n2 ) == 10 )
            {
                mesh_collapse_edge( mesh, edge );
                edges_iter = mesh->edges;
            }
        }
    }

    g_debug( "mesh_relax: end" );
    return;
}

static gint edge_relaxation_index( const Edge *edge )
{
    Node *n1, *n2, *n3, *n4;

    n1 = edge->he[0].origin;
    n2 = edge->he[1].origin;
    n3 = edge->he[0].previous->origin;
    n4 = edge->he[1].previous->origin;

    return node_virtual_degree( n1 ) + node_virtual_degree( n2 ) - node_virtual_degree( n3 ) - node_virtual_degree( n4 );
}


