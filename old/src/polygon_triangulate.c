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

/* #include <stdio.h> */

#include "mesh.h"
#include "polygon_triangulate.h"
#include "predicates.h"

static GHashTable *reflex_vertices = NULL;
static GHashTable *convex_vertices = NULL;
static GHashTable *ears = NULL;

static gboolean halfedge_origin_is_convex( HalfEdge *he );
static gboolean halfedge_origin_is_ear( HalfEdge *he );
static void add_polygon_to_mesh( Mesh *mesh, const Polygon *poly );

Mesh * mesh_triangulate_polygon( const Polygon *poly )
{
    g_return_val_if_fail( poly != NULL, NULL );
    g_return_val_if_fail( poly->vertices != NULL, NULL );
    g_return_val_if_fail( g_list_length( poly->vertices ) >= 3, NULL );

    reflex_vertices = g_hash_table_new( NULL, NULL );
    convex_vertices = g_hash_table_new( NULL, NULL );
    ears = g_hash_table_new( NULL, NULL );

    /* create a new mesh */
    Mesh *mesh = mesh_new();
    /* add polygon's nodes and edges to the mesh */
    add_polygon_to_mesh( mesh, poly );

    /* take the first half-edge of the first edge as the starting half-edge */
    HalfEdge *he_start = &EDGE(mesh->edges->data)->he[0];
    HalfEdge *he_iter = he_start;
    /* iterate over half-edges lying on the inner side of the boundary */
    do
    {
        /* classify half-edge's origin according to its convexity */
        if ( halfedge_origin_is_convex( he_iter ) )
            g_hash_table_insert( convex_vertices, he_iter->origin, he_iter );
        else
            g_hash_table_insert( reflex_vertices, he_iter->origin, he_iter );

        he_iter = he_iter->next;
    }
    while ( he_iter != he_start );

    /* iterate over convex vertices */
    GHashTableIter convex_vert_iter;
    g_hash_table_iter_init( &convex_vert_iter, convex_vertices );
    gpointer key, value;
    while ( g_hash_table_iter_next( &convex_vert_iter, &key, &value ) )
    {
        HalfEdge *conv_he = HALFEDGE(value);
        /* check if the convex vertex is an ear */
        if ( halfedge_origin_is_ear( conv_he ) )
            g_hash_table_insert( ears, conv_he->origin, conv_he );
    }

    /* iterate over ears */
    GHashTableIter ears_iter;
    g_hash_table_iter_init( &ears_iter, ears );
    while ( g_hash_table_iter_next( &ears_iter, &key, &value) )
    {
        /* since we will cut it off, remove the ear from ears and convex vertex
         * hash table */
        g_hash_table_remove( ears, key );
        g_hash_table_remove( convex_vertices, key );

        HalfEdge *he2 = HALFEDGE(value);
        HalfEdge *he1 = he2->previous;
        Node *n1 = he1->origin;
        Node *n3 = he2->pair->origin;

        /* cut off the ear: */
        /* add an edge connecting the ear's neighbours */
        Edge *e = mesh_add_edge( mesh, n3, n1 );
        HalfEdge *he3 = &e->he[0];
        /* add ear's triangle to the mesh */
        mesh_add_element( mesh, he1, he2, he3 );

        he1 = &e->he[1];
        he2 = he1->next;

        gboolean was_reflex_1 = FALSE;
        gboolean was_reflex_became_convex_1 = FALSE;
        gboolean was_reflex_2 = FALSE;
        gboolean was_reflex_became_convex_2 = FALSE;

        /* update the status of the ear's neighbours */

        /* first, we need to update the reflex status of both neighbours, as
         * we need up-to-date info about reflex vertices in the "ear-ness" test */
        if ( g_hash_table_lookup( reflex_vertices, n1 ) )
        {
            was_reflex_1 = TRUE;
            /* reflex vertex can become convex */
            if ( halfedge_origin_is_convex( he1 ) )
            {
                was_reflex_became_convex_1 = TRUE;
                /* is now convex, remove from reflex vertices */
                g_hash_table_remove( reflex_vertices, n1 );
            }
        }

        if ( g_hash_table_lookup( reflex_vertices, n3 ) )
        {
            was_reflex_2 = TRUE;
            if ( halfedge_origin_is_convex( he2 ) )
            {
                was_reflex_became_convex_2 = TRUE;
                g_hash_table_remove( reflex_vertices, n3 );
            }
        }

        /* now we can proceed with the other tests */
        if ( was_reflex_1 )
        {
            if ( was_reflex_became_convex_1 )
            {
                /* add to convex vertices */
                g_hash_table_insert( convex_vertices, n1, he1 );
                /* if was reflex and became convex and even an ear, add it to
                 * ears */
                if ( halfedge_origin_is_ear( he1 ) )
                    g_hash_table_insert( ears, n1, he1 );
            }
            else
                /* if it stayed reflex, just update it with a new half-edge */
                g_hash_table_insert( reflex_vertices, n1, he1 );
        }
        else if ( g_hash_table_lookup( ears, n1 ) )
        {
            /* if it was an ear and now is not, remove it from ears (but it
             * stays convex) */
            if ( ! halfedge_origin_is_ear( he1 ) )
                g_hash_table_remove( ears, n1 );
            else
                /* if it stayed an ear, just update it with a new half-edge */
                g_hash_table_insert( ears, n1, he1 );
        }
        else if ( g_hash_table_lookup( convex_vertices, n1 ) )
        {
            /* if it was convex and now became an ear, insert it into ears */
            if ( halfedge_origin_is_ear( he1 ) )
                g_hash_table_insert( ears, n1, he1 );
            else
                /* otherwise just update it with a new half-edge */
                g_hash_table_insert( convex_vertices, n1, he1 );
        }

        /* the same process with the other neighbour */
        if ( was_reflex_2 )
        {
            if ( was_reflex_became_convex_2 )
            {
                g_hash_table_insert( convex_vertices, n3, he2 );
                if ( halfedge_origin_is_ear( he2 ) )
                    g_hash_table_insert( ears, n3, he2 );
            }
        }
        else if ( g_hash_table_lookup( ears, n3 ) )
        {
            if ( ! halfedge_origin_is_ear( he2 ) )
                g_hash_table_remove( ears, n3 );
        }
        else if ( g_hash_table_lookup( convex_vertices, n3 ) )
        {
            if ( halfedge_origin_is_ear( he2 ) )
                g_hash_table_insert( ears, n3, he2 );
        }
        
        /* we changed the ears, so update the iterator */
        g_hash_table_iter_init( &ears_iter, ears );

        he3 = he2->next;
        /* check if there are only three edges left */
        if ( he3->next == he1 )
        {
            /* if there are, so just add this last element and break out from
             * the loop */
            mesh_add_element( mesh, he1, he2, he3 );
            break;
        }
    }

    /* clean up */
    g_hash_table_destroy( reflex_vertices );
    g_hash_table_destroy( convex_vertices );
    g_hash_table_destroy( ears );

    /* return the resulting mesh */
    return mesh;
}


static void add_polygon_to_mesh( Mesh *mesh, const Polygon *poly )
{
    GList *poly_iter = poly->vertices;
    Point2 *p1 = POINT2(poly_iter->data);
    Node *n1 = mesh_add_node( mesh, p1->x, p1->y );
    Node *first_node = n1;
    for ( poly_iter = g_list_next( poly_iter ); poly_iter != NULL; poly_iter = g_list_next( poly_iter ) )
    {
        Point2 *p2 = POINT2(poly_iter->data);
        Node *n2 = mesh_add_node( mesh, p2->x, p2->y );
        mesh_add_edge( mesh, n1, n2 );

        n1 = n2;
    }
    mesh_add_edge( mesh, n1, first_node );
}


static gboolean halfedge_origin_is_convex( HalfEdge *he )
{
    Node *n1 = he->previous->origin;
    Node *n2 = he->origin;
    Node *n3 = he->next->origin;

    if ( orient2d_test( (double*)NODE_POSITION(n1),
                   (double*)NODE_POSITION(n2),
                   (double*)NODE_POSITION(n3) ) > 0.0 )
        return TRUE;
    else
        return FALSE;
}


static gboolean halfedge_origin_is_ear( HalfEdge *he )
{
    Node *n1 = he->previous->origin;
    Node *n2 = he->origin;
    Node *n3 = he->next->origin;

    /* to test if a vertex is an ear, we just need to iterate over reflex
     * vertices */
    GHashTableIter reflex_vert_iter;
    g_hash_table_iter_init( &reflex_vert_iter, reflex_vertices );
    gpointer key, value;
    while ( g_hash_table_iter_next( &reflex_vert_iter, &key, &value ) )
    {
        Node *refl_node = NODE(key);
        if ( refl_node != n1 && refl_node != n3 )
        {
            if ( orient2d_test( (double*)n1, (double*)n2, (double*)refl_node ) >= 0.0 &&
                 orient2d_test( (double*)n2, (double*)n3, (double*)refl_node ) >= 0.0 &&
                 orient2d_test( (double*)n3, (double*)n1, (double*)refl_node ) >= 0.0 )
                return FALSE;
        }
    }

    return TRUE;
}

