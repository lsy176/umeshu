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

#include <edge.h>
#include <mesh.h>
#include <node.h>
#include <point2.h>

int test_edge( int argc, char *argv[] )
{
    Edge *e = edge_new();
    g_return_val_if_fail( e->he[0].origin == NULL, 1 );
    g_return_val_if_fail( e->he[1].origin == NULL, 1 );
    g_return_val_if_fail( e->he[0].pair == &e->he[1], 1 );
    g_return_val_if_fail( e->he[1].pair == &e->he[0], 1 );
    g_return_val_if_fail( e->he[0].edge == e, 1 );
    g_return_val_if_fail( e->he[1].edge == e, 1 );
    g_return_val_if_fail( e->he[0].next == &e->he[1], 1 );
    g_return_val_if_fail( e->he[1].next == &e->he[0], 1 );
    g_return_val_if_fail( e->he[0].previous == &e->he[1], 1 );
    g_return_val_if_fail( e->he[1].previous == &e->he[0], 1 );

    Node *n1 = node_new( 0.0, 0.0 );
    Node *n2 = node_new( 1.0, 1.0 );

    edge_set_nodes( e, n1, n2 );
    g_return_val_if_fail( e->he[0].origin == n1, 1 );
    g_return_val_if_fail( e->he[1].origin == n2, 1 );
    g_return_val_if_fail( e->he[0].pair == &e->he[1], 1 );
    g_return_val_if_fail( e->he[1].pair == &e->he[0], 1 );
    g_return_val_if_fail( e->he[0].edge == e, 1 );
    g_return_val_if_fail( e->he[1].edge == e, 1 );
    g_return_val_if_fail( e->he[0].next == &e->he[1], 1 );
    g_return_val_if_fail( e->he[1].next == &e->he[0], 1 );
    g_return_val_if_fail( e->he[0].previous == &e->he[1], 1 );
    g_return_val_if_fail( e->he[1].previous == &e->he[0], 1 );

    g_return_val_if_fail( fabs( edge_length( e ) - sqrt( 2.0 ) ) < SMALL_NUMBER, 1 );
    g_return_val_if_fail( edge_is_at_boundary( e ), 1 );
    g_return_val_if_fail( ! edge_is_swappable( e ), 1 );

    edge_free( e );
    node_free( n1 );
    node_free( n2 );

    Mesh *mesh = mesh_new();
    n1 = mesh_add_node( mesh, 0.0, 0.0 );
    n2 = mesh_add_node( mesh, 1.0, 0.0 );
    Node *n3 = mesh_add_node( mesh, 1.0, 1.0 );
    Node *n4 = mesh_add_node( mesh, 0.0, 1.0 );
    Node *n5 = mesh_add_node( mesh, 0.5, 0.5 );

    Edge *e1 = mesh_add_edge( mesh, n1, n2 );
    Edge *e2 = mesh_add_edge( mesh, n2, n3 );
    Edge *e3 = mesh_add_edge( mesh, n3, n4 );
    Edge *e4 = mesh_add_edge( mesh, n4, n1 );

    Edge *e5 = mesh_add_edge( mesh, n5, n1 );
    Edge *e6 = mesh_add_edge( mesh, n5, n2 );
    Edge *e7 = mesh_add_edge( mesh, n5, n3 );
    Edge *e8 = mesh_add_edge( mesh, n5, n4 );

    mesh_add_element( mesh, &e1->he[0], &e6->he[1], &e5->he[0] );
    mesh_add_element( mesh, &e2->he[0], &e7->he[1], &e6->he[0] );
    mesh_add_element( mesh, &e3->he[0], &e8->he[1], &e7->he[0] );
    mesh_add_element( mesh, &e4->he[0], &e5->he[1], &e8->he[0] );

    g_return_val_if_fail( ! halfedge_is_at_boundary( &e1->he[0] ), 1 );
    g_return_val_if_fail( halfedge_is_at_boundary( &e1->he[1] ), 1 );
    g_return_val_if_fail( edge_is_at_boundary( e1 ), 1 );
    g_return_val_if_fail( ! edge_is_at_boundary( e5 ), 1 );

    g_return_val_if_fail( ! edge_is_swappable( e1 ), 1 );
    g_return_val_if_fail( ! edge_is_swappable( e5 ), 1 );
    point2_set( NODE_POSITION( n5 ), NODE_POSITION( n5 )->x + SMALL_NUMBER, NODE_POSITION( n5 )->y + SMALL_NUMBER );
    g_return_val_if_fail( edge_is_swappable( e5 ), 1 );
    g_return_val_if_fail( ! edge_is_swappable( e6 ), 1 );
    g_return_val_if_fail( ! edge_is_swappable( e7 ), 1 );
    g_return_val_if_fail( edge_is_delaunay( e5 ), 1 );

    mesh_free( mesh );

    mesh = mesh_new();
    n1 = mesh_add_node( mesh, 0.0, 0.0 );
    n2 = mesh_add_node( mesh, 1.0, 0.0 );
    e1 = mesh_add_edge( mesh, n1, n2 );
    Point2 p;
    halfedge_ideal_triangle_point( &e1->he[0], edge_length( e1 ), &p );
    g_return_val_if_fail( fabs( p.x - 0.5 ) < SMALL_NUMBER, 1 );
    printf( " p.x = %12.5e\n", p.x );
    printf( " p.y = %12.5e\n", p.y );
    g_return_val_if_fail( fabs( p.y - sin( M_PI/3.0 ) ) < SMALL_NUMBER, 1 );

    return 0;
}


