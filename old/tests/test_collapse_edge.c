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

#include <stdlib.h>
#include <stdio.h>

#include <edge.h>
#include <mesh.h>
#include <mesh_output.h>
#include <node.h>

int test_collapse_edge( int argc, char *argv[] )
{
    Mesh * mesh = mesh_new();
    Node *n1 = mesh_add_node( mesh, 0.0, 2.0 );
    Node *n2 = mesh_add_node( mesh, 0.0, 1.0 );
    Node *n3 = mesh_add_node( mesh, 0.0, -1.0 );
    Node *n4 = mesh_add_node( mesh, -1.0, -2.0 );
    Node *n5 = mesh_add_node( mesh, -1.0, 0.0 );
    Node *n6 = mesh_add_node( mesh, 1.0, 0.0 );
    Node *n7 = mesh_add_node( mesh, 11.0, 10.0 );
    Edge *e1 = mesh_add_edge( mesh, n1, n2 );
    Edge *e2 = mesh_add_edge( mesh, n2, n6 );
    Edge *e3 = mesh_add_edge( mesh, n1, n6 );
    Edge *e4 = mesh_add_edge( mesh, n2, n5 );
    Edge *e5 = mesh_add_edge( mesh, n1, n5 );
    Edge *e6 = mesh_add_edge( mesh, n3, n5 );
    Edge *e7 = mesh_add_edge( mesh, n3, n6 );
    Edge *e8 = mesh_add_edge( mesh, n3, n4 );
    Edge *e9 = mesh_add_edge( mesh, n4, n5 );
    Edge *e10 = mesh_add_edge( mesh, n4, n7 );
    Edge *e11 = mesh_add_edge( mesh, n2, n3 );
    Edge *e12 = mesh_add_edge( mesh, n3, n7 );
    Edge *e13 = mesh_add_edge( mesh, n6, n7 );

    mesh_add_element( mesh, &e5->he[0], &e4->he[1], &e1->he[1] );
    mesh_add_element( mesh, &e1->he[0], &e2->he[0], &e3->he[1] );
    mesh_add_element( mesh, &e4->he[0], &e6->he[1], &e11->he[1] );
    mesh_add_element( mesh, &e2->he[1], &e11->he[0], &e7->he[0] );
    mesh_add_element( mesh, &e6->he[0], &e9->he[1], &e8->he[1] );
    mesh_add_element( mesh, &e8->he[0], &e10->he[0], &e12->he[1] );
    mesh_add_element( mesh, &e12->he[0], &e13->he[1], &e7->he[1] );

    mesh_save_to_eps( "test_collapse_edge_1.eps", mesh );

    mesh_collapse_edge( mesh, e11 );

    mesh_save_to_eps( "test_collapse_edge_2.eps", mesh );

    mesh_free( mesh );

    return 0;
}
