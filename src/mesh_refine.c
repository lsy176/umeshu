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
#include "element.h"
#include "mesh.h"
#include "mesh_delaunay.h"
#include "mesh_macros.h"
#include "mesh_refine.h"

static void mesh_refine_ruppert( Mesh * mesh, gdouble max_element_area, gdouble min_angle );
static void mesh_refine_chew( Mesh * mesh, gdouble max_element_area, gdouble min_angle );
static Element * mesh_find_large_element( const Mesh *mesh, gdouble max_element_area );
static GSList * find_half_edges_encroached_upon_by_point( const Mesh *mesh, const Point2 *p );
static void split_half_edges_encroached_upon_by_point( Mesh *mesh, GSList * encroached_half_edges, const Point2 *p );
static Element * mesh_find_skinny_element( const Mesh *mesh, gdouble min_angle );

void mesh_refine( Mesh * mesh, RefinementAlgorithm alg, gdouble max_element_area, gdouble min_angle )
{
    switch ( alg )
    {
        case RUPPERT_REFINEMENT:
            mesh_refine_ruppert( mesh, max_element_area, min_angle );
            break;
        case CHEW_REFINEMENT:
            mesh_refine_chew( mesh, max_element_area, min_angle );
            break;
        default:
            g_return_if_reached();
    }
}


/* we assume that mesh is already CDT */
static void mesh_refine_ruppert( Mesh * mesh, gdouble max_element_area, gdouble min_angle )
{
    g_debug( "Entered mesh_refine_ruppert" );
    while ( TRUE )
    {
        mesh_split_encroached_boundary_edges( mesh );

        Element *bad_element = mesh_find_large_element( mesh, max_element_area );
        g_debug( "Found large element: %p", bad_element );
        if ( bad_element == NULL )
        {
            bad_element = mesh_find_skinny_element( mesh, min_angle );
            g_debug( "Found skinny element: %p", bad_element );
            if ( bad_element == NULL )
                break;
        }

        Point2 circumcenter;
        element_circumcenter_coordinates( bad_element, &circumcenter );

        /* TODO: the following code makes trouble ... why? Is there a bug a
         * halfedge_ideal_triangle_point? */
        /* gdouble angle1, angle2, angle3;
        triangle_angles( NODE_POSITION(he->origin), NODE_POSITION(he->pair->origin), &circumcenter,
                &angle1, &angle2, &angle3 );
        if ( MIN( angle1, MIN( angle2, angle3 )) < min_angle )
        {
            g_debug( "Using off-center instead of circumcenter to eliminate the bad triangle" );
            gdouble length;
            HalfEdge *he = element_min_edge( bad_element, &length );
            halfedge_ideal_triangle_point( he, length, &circumcenter );
        } */

        GSList *encroached_half_edges = find_half_edges_encroached_upon_by_point( mesh, &circumcenter );

        if ( encroached_half_edges != NULL )
        {
            g_debug( "The point would encroach some edges, so split them recursively in half" );
            split_half_edges_encroached_upon_by_point( mesh, encroached_half_edges, &circumcenter );
        }
        else
        {
            g_debug( "We can safely insert the point into the mesh" );
            Element *element_to_kill = mesh_locate_element( &circumcenter, bad_element );
            HalfEdge *he1 = mesh_split_element( mesh, element_to_kill, &circumcenter )->out_halfedge->next;
            HalfEdge *he2 = he1->next->pair->next;
            HalfEdge *he3 = he2->next->pair->next;

            recursive_swap_delaunay( mesh, he1 );
            recursive_swap_delaunay( mesh, he2 );
            recursive_swap_delaunay( mesh, he3 );
        }
    }
}


static Element * mesh_find_large_element( const Mesh *mesh, gdouble max_element_area )
{
    GList *elements_iter;
    
    for ( elements_iter = mesh->elements; elements_iter != NULL; elements_iter = g_list_next( elements_iter ) )
    {
        if ( element_area( ELEMENT( elements_iter->data ) ) > max_element_area )
            return ELEMENT( elements_iter->data );
    }

    return NULL;
}


static Element * mesh_find_skinny_element( const Mesh *mesh, gdouble min_angle )
{
    g_debug("minimum angle: %f", min_angle );
    GList *elements_iter;
    for ( elements_iter = mesh->elements; elements_iter != NULL; elements_iter = g_list_next( elements_iter ) )
    {
        gdouble el_min_angle = element_minimum_angle( ELEMENT( elements_iter->data ) ); 
        g_debug( "element's minimum angle: %f", el_min_angle );
        if ( el_min_angle < min_angle )
            return ELEMENT( elements_iter->data );
    }

    return NULL;

    /* using the following code we return the most skinny element, but we have
     * to sort the elements, which is something that I do not want to do at
     * this point. This whole thing of deciding which elements to refine has to
     * be implemented in a better way anyway */
    /* mesh->elements = g_list_sort( mesh->elements, compare_element_quality );
    if ( element_minimum_angle( ELEMENT( mesh->elements->data ) ) < min_angle )
        return ELEMENT( mesh->elements->data );
    else
        return NULL; */
}


static GSList * find_half_edges_encroached_upon_by_point( const Mesh *mesh, const Point2 *p )
{
    GSList *encroached_half_edges = NULL;
    HalfEdge *start_he = mesh_get_boundary_halfedge( mesh );
    HalfEdge *boundary_iter = start_he;
    do
    {
        if ( halfedge_is_encroached_upon_by_point( boundary_iter->pair, p ) )
            encroached_half_edges = g_slist_prepend( encroached_half_edges, boundary_iter->pair );
        boundary_iter = boundary_iter->next;
    }
    while ( boundary_iter != start_he );

    return encroached_half_edges;
}


static void split_half_edges_encroached_upon_by_point( Mesh *mesh, GSList * encroached_half_edges, const Point2 *p )
{
    while ( encroached_half_edges != NULL )
    {
        HalfEdge *he = HALFEDGE( encroached_half_edges->data );
        encroached_half_edges = g_slist_delete_link( encroached_half_edges, encroached_half_edges );

        HalfEdge *he1 = he->next;
        HalfEdge *he2 = he->previous;
        mesh_split_edge( mesh, he->edge, NULL, NULL );
        recursive_swap_delaunay( mesh, he1 );
        recursive_swap_delaunay( mesh, he2 );

        if ( halfedge_is_encroached_upon_by_point( he1->previous, p ) )
            encroached_half_edges = g_slist_prepend( encroached_half_edges, he1 );
        if ( halfedge_is_encroached_upon_by_point( he2->next, p ) )
            encroached_half_edges = g_slist_prepend( encroached_half_edges, he2 );
    }
}




/* static void find_half_edges_encroached_upon( Mesh *mesh ) */
/* { */
    /* HalfEdge *start_he = mesh_get_boundary_halfedge( mesh ); */
    /* HalfEdge *boundary_iter = start_he; */
    /* do */
    /* { */
        /* if ( halfedge_is_encroached_upon( boundary_iter->pair ) ) */
            /* encroached_half_edges = g_slist_prepend( encroached_half_edges, boundary_iter->pair ); */
        /* boundary_iter = boundary_iter->next; */
    /* } */
    /* while ( boundary_iter != start_he ); */
/* } */


/* static gint compare_element_quality( gconstpointer a, gconstpointer b ) */
/* { */
    /* gdouble el1_min_angle = element_minimum_angle( ELEMENT(a) ); */
    /* gdouble el2_min_angle = element_minimum_angle( ELEMENT(b) ); */

    /* if ( el1_min_angle < el2_min_angle ) */
        /* return -1; */
    /* if ( el1_min_angle > el2_min_angle ) */
        /* return 1; */
    /* else */
        /* return 0; */
/* } */


static void mesh_refine_chew( Mesh * mesh, gdouble max_element_area, gdouble min_angle )
{
    g_return_if_reached();
}

