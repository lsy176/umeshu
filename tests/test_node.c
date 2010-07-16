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

#include <node.h>
#include <mesh.h>

int test_node( int argc, char *argv[] )
{
    Node *n = node_new( 1.1, 2.5 );
    g_return_val_if_fail( NODE_POSITION(n)->x == 1.1, 1 );
    g_return_val_if_fail( NODE_POSITION(n)->y == 2.5, 1 );
    g_return_val_if_fail( node_is_isolated( n ), 1 );
    g_return_val_if_fail( node_is_at_boundary( n ) == NULL, 1 );
    g_return_val_if_fail( node_degree( n ) == 0, 1 );
    node_free( n );

    Mesh *mesh = mesh_new();
    Node *n1 = mesh_add_node( mesh, 0.0, 1.0 );
    Node *n2 = mesh_add_node( mesh, -0.5, -0.5 );
    Node *n3 = mesh_add_node( mesh, 0.5, -0.5 );
    Node *n4 = mesh_add_node( mesh, 0.0, 0.0 );
    Edge *e1 = mesh_add_edge( mesh, n1, n2 );
    Edge *e2 = mesh_add_edge( mesh, n2, n3 );
    Edge *e3 = mesh_add_edge( mesh, n3, n1 );
    Edge *e4 = mesh_add_edge( mesh, n4, n1 );
    Edge *e5 = mesh_add_edge( mesh, n4, n2 );
    Edge *e6 = mesh_add_edge( mesh, n4, n3 );
    Element *el1 = mesh_add_element( mesh, &e1->he[0], &e5->he[1], &e4->he[0] );
    Element *el2 = mesh_add_element( mesh, &e2->he[0], &e6->he[1], &e5->he[0] );
    Element *el3 = mesh_add_element( mesh, &e3->he[0], &e4->he[1], &e6->he[0] );

    g_return_val_if_fail( ! node_is_isolated( n1 ), 1 );
    g_return_val_if_fail( node_is_at_boundary( n1 ) != NULL, 1 );
    g_return_val_if_fail( node_degree( n1 ) == 3, 1 );

    g_return_val_if_fail( ! node_is_isolated( n2 ), 1 );
    g_return_val_if_fail( node_is_at_boundary( n2 ) != NULL, 1 );
    g_return_val_if_fail( node_degree( n2 ) == 3, 1 );

    g_return_val_if_fail( ! node_is_isolated( n3 ), 1 );
    g_return_val_if_fail( node_is_at_boundary( n3 ) != NULL, 1 );
    g_return_val_if_fail( node_degree( n3 ) == 3, 1 );
    
    g_return_val_if_fail( ! node_is_isolated( n4 ), 1 );
    g_return_val_if_fail( node_is_at_boundary( n4 ) == NULL, 1 );
    g_return_val_if_fail( node_degree( n4 ) == 3, 1 );

    mesh_free( mesh );

    return 0;
}


