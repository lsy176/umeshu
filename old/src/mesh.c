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

#include <string.h>

#include "edge.h"
#include "element.h"
#include "mesh.h"
#include "node.h"
#include "point2.h"

static gboolean make_adjacent_half_edges( HalfEdge *in, HalfEdge *out );
static HalfEdge * find_free_incident_half_edge_in_range( HalfEdge *he1, HalfEdge *he2 );
static HalfEdge * node_find_free_incident_half_edge( const Node *n );


Mesh * mesh_new( void )
{
    Mesh *mesh = g_slice_new0( Mesh );
    mesh->hash = g_hash_table_new( NULL, NULL );
    return mesh;
}


void mesh_free( Mesh * mesh )
{
    if ( mesh != NULL )
    {
        g_list_foreach( mesh->nodes, (GFunc) node_free, NULL );
        g_list_foreach( mesh->edges, (GFunc) edge_free, NULL );
        g_list_foreach( mesh->elements, (GFunc) element_free, NULL );

        g_list_free( mesh->nodes );
        g_list_free( mesh->edges );
        g_list_free( mesh->elements );

        g_hash_table_destroy( mesh->hash );

        memset( mesh, 0, sizeof( Mesh ) );
        g_slice_free( Mesh, mesh );
    }
}


Node * mesh_add_node( Mesh * mesh, gdouble x, gdouble y )
{
    g_return_val_if_fail( mesh != NULL, NULL );

    Node *node = node_new( x, y );
    mesh->nodes = g_list_prepend( mesh->nodes, node );
    g_hash_table_insert( mesh->hash, node, mesh->nodes );
    mesh->Np++;

    return node;
}


void mesh_remove_node( Mesh *mesh, Node *node )
{
    g_return_if_fail( mesh != NULL );
    g_return_if_fail( node != NULL );

    if ( ! node_is_isolated( node ) )
    {
        HalfEdge *next = node->out_halfedge;
        HalfEdge *current;
        do
        {
            current = next;
            next = next->pair->next;
            if ( next->edge == current->edge )
                next = next->pair->next;
            mesh_remove_edge( mesh, current->edge );
        }
        while ( current != next );
    }

    GList *link = g_hash_table_lookup( mesh->hash, node );
    g_hash_table_remove( mesh->hash, node );
    mesh->nodes = g_list_delete_link( mesh->nodes, link );
    node_free( node );
    mesh->Np--;
}


Edge * mesh_add_edge( Mesh *mesh, Node *node1, Node *node2 )
{
    g_return_val_if_fail( mesh != NULL, NULL );
    g_return_val_if_fail( node1 != NULL, NULL );
    g_return_val_if_fail( node2 != NULL, NULL );

    // we do not want loop edges
    if ( node1 == node2 )
    {
        g_warning( "mesh_add_edge: attempted to add a loop edge to the mesh" );
        return NULL;
    }

    // Because of speed, we do not check if the edge already exists in the mesh

    Edge *edge = edge_new();
    edge_set_nodes( edge, node1, node2 );

    if ( node_is_isolated( node1 ) )
    {
        node1->out_halfedge = &edge->he[0];
    }
    else
    {
        HalfEdge *in_he = node_find_free_incident_half_edge( node1 );
        if ( in_he == NULL )
        {
            edge_free( edge );
            g_warning( "mesh_add_edge: did not find free incident half-edge" );
            return NULL;
        }

        HalfEdge *out_he = in_he->next;

        in_he->next = &edge->he[0];
        edge->he[0].previous = in_he;
        edge->he[1].next = out_he;
        out_he->previous = &edge->he[1];
    }

    if ( node_is_isolated( node2 ) )
    {
        node2->out_halfedge = &edge->he[1];
    }
    else
    {
        HalfEdge *in_he = node_find_free_incident_half_edge( node2 );
        if ( in_he == NULL )
        {
            edge_free( edge );
            g_warning( "mesh_add_edge: did not find free incident half-edge" );
            return NULL;
        }

        HalfEdge *out_he = in_he->next;

        in_he->next = &edge->he[1];
        edge->he[1].previous = in_he;
        edge->he[0].next = out_he; 
        out_he->previous = &edge->he[0];
    }

    mesh->edges = g_list_prepend( mesh->edges, edge );
    g_hash_table_insert( mesh->hash, edge, mesh->edges );
    mesh->Ne++;

    return edge;
}


void mesh_remove_edge( Mesh *mesh, Edge *edge )
{
    g_return_if_fail( mesh != NULL );
    g_return_if_fail( edge != NULL );

    HalfEdge *he1 = &edge->he[0];
    HalfEdge *he2 = &edge->he[1];

    // remove elements on both sides of the edge
    if ( ! halfedge_is_at_boundary( he1 ) )
        mesh_remove_element( mesh, he1->element );
    if ( ! halfedge_is_at_boundary( he2 ) )
        mesh_remove_element( mesh, he2->element );

    Node *n1 = he1->origin;
    Node *n2 = he2->origin;

    // unlink the edge on n1 end
    if ( n1->out_halfedge == he1 )
    {
        if ( he1->pair->next != he1 )
            n1->out_halfedge = he1->pair->next;
        else
            n1->out_halfedge = NULL;
    }

    he1->previous->next = he1->pair->next;
    he1->pair->next->previous = he1->previous;

    // unlink the edge on n2 end
    if ( n2->out_halfedge == he2 )
    {
        if ( he2->pair->next != he2 )
            n2->out_halfedge = he2->pair->next;
        else
            n2->out_halfedge = NULL;
    }

    he2->previous->next = he2->pair->next;
    he2->pair->next->previous = he2->previous;
    
    GList *link = g_hash_table_lookup( mesh->hash, edge );
    g_hash_table_remove( mesh->hash, edge );
    mesh->edges = g_list_delete_link( mesh->edges, link );
    edge_free( edge );
    mesh->Ne--;
}


Element * mesh_add_element( Mesh *mesh, HalfEdge *he1, HalfEdge *he2, HalfEdge *he3 )
{
    g_return_val_if_fail( mesh != NULL, NULL );
    g_return_val_if_fail( he1 != NULL, NULL );
    g_return_val_if_fail( he2 != NULL, NULL );
    g_return_val_if_fail( he3 != NULL, NULL );

    if ( he1->element != NULL || he2->element != NULL || he3->element != NULL )
    {
        g_warning( "mesh_add_element: half-edges not free, cannot create element\n" );
        return NULL;
    }

    if ( he1->pair->origin != he2->origin
            || he2->pair->origin != he3->origin
            || he3->pair->origin != he1->origin )
    {
        g_warning( "mesh_add_element: half-edges do not form a chain, cannot create element\n" );
        return NULL;
    }

    if ( ! make_adjacent_half_edges( he1, he2 ) ||
         ! make_adjacent_half_edges( he2, he3 ) ||
         ! make_adjacent_half_edges( he3, he1 ) )
    {
        g_warning( "mesh_add_element: attempting to create non-manifold mesh, cannot create element\n" );
        return NULL;
    }

    Element *element = element_new();
    element->adjacent_halfedge = he1;
    he1->element = he2->element = he3->element = element;
    
    mesh->elements = g_list_prepend( mesh->elements, element );
    g_hash_table_insert( mesh->hash, element, mesh->elements );
    mesh->Nt++;

    return element;
}


void mesh_remove_element( Mesh *mesh, Element *element )
{
    g_return_if_fail( mesh != NULL );
    g_return_if_fail( element != NULL );

    HalfEdge *begin = element->adjacent_halfedge;
    HalfEdge *he_iterator = begin;
    do
    {
        he_iterator->element = NULL;
        he_iterator = he_iterator->next;
    }
    while ( he_iterator != begin );

    GList *link = g_hash_table_lookup( mesh->hash, element );
    g_hash_table_remove( mesh->hash, element );
    mesh->elements = g_list_delete_link( mesh->elements, link );
    element_free( element );
    mesh->Nt--;
}


static gboolean make_adjacent_half_edges( HalfEdge *in, HalfEdge *out )
{
    g_return_val_if_fail( in != NULL, FALSE );
    g_return_val_if_fail( out != NULL, FALSE );

    // half-edges are already adjacent
    if ( in->next == out )
        return TRUE;

    HalfEdge *b = in->next;
    HalfEdge *d = out->previous;

    HalfEdge *g = find_free_incident_half_edge_in_range( out->pair, in );
    // half-edges cannot be made adjacent
    if ( g == NULL )
        return FALSE;

    HalfEdge *h = g->next;

    in->next = out;
    out->previous = in;

    g->next = b;
    b->previous = g;

    d->next = h;
    h->previous = d;

    return TRUE;
}


Edge * mesh_swap_edge( Mesh *mesh, Edge *edge )
{
    g_return_val_if_fail( mesh != NULL, NULL );
    g_return_val_if_fail( edge != NULL, NULL );

    g_debug( "mesh_swap_edge: begin" );

    HalfEdge *e1 = edge->he[0].next;
    HalfEdge *e2 = edge->he[0].previous;
    HalfEdge *e3 = edge->he[1].next;
    HalfEdge *e4 = edge->he[1].previous;
    Node *n1 = e2->origin;
    Node *n2 = e4->origin;

    mesh_remove_edge( mesh, edge );
    Edge *new_edge = mesh_add_edge( mesh, n1, n2 );
    mesh_add_element( mesh, e2, e3, &new_edge->he[1] );
    mesh_add_element( mesh, e1, &new_edge->he[0], e4 );

    g_debug( "mesh_swap_edge: end" );

    return new_edge;
}


Node * mesh_split_element( Mesh *mesh, Element *el, const Point2 *p )
{
    g_return_val_if_fail( mesh != NULL, NULL );
    g_return_val_if_fail( el != NULL, NULL );
    g_return_val_if_fail( p != NULL, NULL );

    g_debug( "mesh_split_element: begin" );
    
    HalfEdge *he1 = el->adjacent_halfedge;
    HalfEdge *he2 = he1->next;
    HalfEdge *he3 = he2->next;
    Node *n = mesh_add_node( mesh, p->x, p->y );
    mesh_remove_element( mesh, el );
    Edge *e1 = mesh_add_edge( mesh, n, he1->origin );
    Edge *e2 = mesh_add_edge( mesh, n, he2->origin );
    Edge *e3 = mesh_add_edge( mesh, n, he3->origin );
    mesh_add_element( mesh, he1, &e2->he[1], &e1->he[0] );
    mesh_add_element( mesh, he2, &e3->he[1], &e2->he[0] );
    mesh_add_element( mesh, he3, &e1->he[1], &e3->he[0] );

    g_debug( "mesh_split_element: end" );

    return n;
}


void mesh_split_edge( Mesh *mesh, Edge *edge, Edge **subedge1, Edge **subedge2 )
{
    g_return_if_fail( mesh != NULL );
    g_return_if_fail( edge != NULL );

    g_debug( "mesh_split_edge: begin" );

    HalfEdge *he0 = &edge->he[0];
    HalfEdge *he0n = NULL;
    HalfEdge *he0p = NULL;
    if ( ! halfedge_is_at_boundary( he0 ) )
    {
        he0n = he0->next;
        he0p = he0->previous;
    }

    HalfEdge *he1 = &edge->he[1];
    HalfEdge *he1n = NULL;
    HalfEdge *he1p = NULL;
    if ( ! halfedge_is_at_boundary( he1 ) )
    {
        he1n = he1->next;
        he1p = he1->previous;
    }

    Node *n0 = he0->origin;
    Node *n1 = he1->origin;

    mesh_remove_edge( mesh, edge );

    Point2 p = point2_interpolate( NODE_POSITION(n0), NODE_POSITION(n1), 0.5 );
    Node *n2 = mesh_add_node( mesh, p.x, p.y );
    Edge *e1 = mesh_add_edge( mesh, n0, n2 );
    Edge *e2 = mesh_add_edge( mesh, n1, n2 );
    if ( he0p != NULL )
    {
        Edge *e3 = mesh_add_edge( mesh, n2, he0p->origin );
        mesh_add_element( mesh, he0p, &e1->he[0], &e3->he[0] );
        mesh_add_element( mesh, he0n, &e3->he[1], &e2->he[1] );
    }

    if ( he1p != NULL )
    {
        Edge *e4 = mesh_add_edge( mesh, n2, he1p->origin );
        mesh_add_element( mesh, he1p, &e2->he[0], &e4->he[0] );
        mesh_add_element( mesh, he1n, &e4->he[1], &e1->he[1] );
    }

    if ( subedge1 != NULL )
        *subedge1 = e1;
    if ( subedge2 != NULL )
        *subedge2 = e2;

    g_debug( "mesh_split_edge: end" );
}


HalfEdge * mesh_get_boundary_halfedge( const Mesh *mesh )
{
    g_return_val_if_fail( mesh != NULL, NULL );

    g_debug( "mesh_get_boundary_halfedge: begin" );

    GList *edges_iter;
    for ( edges_iter = mesh->edges; edges_iter != NULL; edges_iter = g_list_next( edges_iter ) )
    {
        Edge *edge = EDGE(edges_iter->data);

        if ( halfedge_is_at_boundary( &edge->he[0] ) )
        {
            g_debug( "mesh_get_boundary_halfedge: end" );
            return &edge->he[0];
        }
        else if ( halfedge_is_at_boundary( &edge->he[1] ) )
        {
            g_debug( "mesh_get_boundary_halfedge: end" );
            return &edge->he[1];
        }
    }

    /* our mesh should always have a boundary, we do not create solids */
    g_return_val_if_reached( NULL );
}


Element * mesh_locate_element( const Point2 *p, const Element *initial_element )
{
    g_debug( "mesh_locate_element: begin" );

    HalfEdge *he_iter = initial_element->adjacent_halfedge;
    HalfEdge *he_start = he_iter;

    while ( TRUE )
    {
        if ( halfedge_point_is_in_left_half_plane( he_iter, p ) )
        {
            he_iter = he_iter->next;
            if ( he_iter == he_start )
                return he_iter->element;
        }
        else
        {
            he_iter = he_iter->pair;
            if ( halfedge_is_at_boundary( he_iter ) )
                return NULL;
            he_start = he_iter;
            he_iter = he_iter->next;
        }
    }
    g_debug( "mesh_locate_element: end" );
}


void mesh_get_bounding_box( const Mesh *mesh, Box2 *box )
{
    g_return_if_fail( mesh != NULL );
    g_return_if_fail( mesh->nodes != NULL );
    g_return_if_fail( box != NULL );

    GList *nodes_iter = mesh->nodes;
    Node *node = NODE(nodes_iter->data);
    gdouble x = NODE_POSITION(node)->x;
    gdouble y = NODE_POSITION(node)->y;
    box->min[0] = box->max[0] = x;
    box->min[1] = box->max[1] = y;

    for ( nodes_iter = g_list_next( nodes_iter ); nodes_iter != NULL; nodes_iter = g_list_next( nodes_iter ) )
    {
        node = NODE(nodes_iter->data);
        x = NODE_POSITION(node)->x;
        y = NODE_POSITION(node)->y;
        if ( x < box->min[0] ) box->min[0] = x;
        if ( y < box->min[1] ) box->min[1] = y;
        if ( x > box->max[0] ) box->max[0] = x;
        if ( y > box->max[1] ) box->max[1] = y;
    }
}


gboolean mesh_collapse_edge( Mesh *mesh, Edge *edge )
{
    g_debug( "mesh_collapse_edge: begin" );

    if ( node_is_at_boundary( edge->he[0].origin ) || node_is_at_boundary( edge->he[1].origin) )
        return FALSE;

    HalfEdge *he1 = edge->he[0].next;
    HalfEdge *he2 = edge->he[0].previous;
    if ( he1->next != he2 || he2->previous != he1 )
        g_warning( "mesh_collapse_edge: wrong connectivity between he1 and he2" );
    HalfEdge *he3 = edge->he[1].next;
    HalfEdge *he4 = edge->he[1].previous;
    if ( he3->next != he4 || he4->previous != he3 )
        g_warning( "mesh_collapse_edge: wrong connectivity between he3 and he4" );
    HalfEdge *he5 = he2->pair->previous;
    HalfEdge *he6 = he3->pair->next;
    Node *n_del = he3->origin;
    Node *n = he1->origin;

    mesh_remove_edge( mesh, edge );

    he1->element = he2->pair->element;
    he2->pair->element = NULL;
    if ( he1->element == NULL )
        g_warning( "mesh_collapse_edge: he1->element == NULL" ); 
    he1->element->adjacent_halfedge = he1;

    he4->element = he3->pair->element;
    he3->pair->element = NULL;
    if ( he4->element == NULL )
        g_warning( "mesh_collapse_edge: he4->element == NULL" ); 
    he4->element->adjacent_halfedge = he4;
    
    if ( he6->previous == he5 )
        g_warning( "mesh_collapse_edge: he6->previous == he5" );

    mesh_remove_edge( mesh, he2->edge );
    mesh_remove_edge( mesh, he3->edge );

    he5->next = he1;
    he1->previous = he5;
    he6->previous = he4;
    he4->next = he6;

    *NODE_POSITION(n) = point2_interpolate( NODE_POSITION(n), NODE_POSITION(n_del), 0.5 );

    if ( node_is_isolated( n_del ) )
        g_warning( "mesh_collapse_edge: node n_del should not be isolated" );
    HalfEdge *iter = n_del->out_halfedge;
    HalfEdge *start = iter;
    do
    {
        iter->origin = n;
        iter = iter->pair->next;
    }
    while ( iter != start );

    n_del->out_halfedge = NULL;
    mesh_remove_node( mesh, n_del );

    g_debug( "mesh_collapse_edge: end" );

    return TRUE;
}


static HalfEdge * find_free_incident_half_edge_in_range( HalfEdge *he1, HalfEdge *he2 )
{
    g_return_val_if_fail( he1->pair->origin == he2->pair->origin, NULL );

    HalfEdge *result = he1;
    while ( result->element != NULL  )
    {
        result = result->next->pair;
        if ( result == he2 )
            break;
    }
    if ( result->element == NULL )
        return result;
    else
    {
        g_warning( "find_free_incident_half_edge_in_range: could not find free incident half edge\n" );
        return NULL;
    }
}


static HalfEdge * node_find_free_incident_half_edge( const Node *n )
{
    HalfEdge *he = n->out_halfedge->pair;
    return find_free_incident_half_edge_in_range( he, he );
}

