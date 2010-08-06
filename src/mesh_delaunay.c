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
#include "mesh.h"
#include "mesh_delaunay.h"


void recursive_swap_delaunay( Mesh *mesh, HalfEdge *he )
{
    if ( edge_is_delaunay( he->edge ) || ! edge_is_swappable( he->edge ) )
        return;
    
    HalfEdge *he1 = he->pair->next;
    HalfEdge *he2 = he->pair->previous;

    mesh_swap_edge( mesh, he->edge );

    recursive_swap_delaunay( mesh, he1 );
    recursive_swap_delaunay( mesh, he2 );
}


void mesh_make_cdt_by_edge_flipping( Mesh *mesh )
{
    /* for easy and fast indication if an edge is already in the flippable list */
    GHashTable *flippable_edges_hash = g_hash_table_new( NULL, NULL);
    GList *edges_iter;
    
    /* make a copy of mesh edges and add them to the hash table */
    GList *flippable_edges = g_list_copy( mesh->edges );
    for ( edges_iter = flippable_edges; edges_iter != NULL; edges_iter = g_list_next( edges_iter ) )
    {
        Edge *edge = EDGE(edges_iter->data);
        g_hash_table_insert( flippable_edges_hash, edge, edge );
    }

    /* while there are edges to check */
    while ( flippable_edges != NULL )
    {
        /* get the first edge */
        Edge *edge = EDGE(flippable_edges->data);

        /* we will deal with it now, so remove it from the list and the hash */
        flippable_edges = g_list_delete_link( flippable_edges, flippable_edges );
        g_hash_table_remove( flippable_edges_hash, edge );
        
        /* if the hash is delaunay or not flippable, go to the next edge */
        if ( edge_is_delaunay( edge ) || ! edge_is_swappable( edge ) )
            continue;

        /* otherwise get the neighbouring half-edges */
        HalfEdge *he = &edge->he[0];
        Edge *e1 = he->next->edge;
        Edge *e2 = he->previous->edge;
        Edge *e3 = he->pair->next->edge;
        Edge *e4 = he->pair->previous->edge;
        /* and add them to the list and hash if they are not already there */
        if ( g_hash_table_lookup( flippable_edges_hash, e1 ) == NULL )
        {
            g_hash_table_insert( flippable_edges_hash, e1, e1 );
            flippable_edges = g_list_prepend( flippable_edges, e1 );
        }
        if ( g_hash_table_lookup( flippable_edges_hash, e2 ) == NULL )
        {
            g_hash_table_insert( flippable_edges_hash, e2, e2 );
            flippable_edges = g_list_prepend( flippable_edges, e2 );
        }
        if ( g_hash_table_lookup( flippable_edges_hash, e3 ) == NULL )
        {
            g_hash_table_insert( flippable_edges_hash, e3, e3 );
            flippable_edges = g_list_prepend( flippable_edges, e3 );
        }
        if ( g_hash_table_lookup( flippable_edges_hash, e4 ) == NULL )
        {
            g_hash_table_insert( flippable_edges_hash, e4, e4 );
            flippable_edges = g_list_prepend( flippable_edges, e4 );
        }
        /* finally, swap the edge to make it Delaunay */
        mesh_swap_edge( mesh, edge );
    }

    /* clean up */
    g_hash_table_destroy( flippable_edges_hash );
}




