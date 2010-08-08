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

#include <mesh.h>
#include <mesh_delaunay.h>
#include <mesh_output.h>
#include <point2.h>
#include <polygon_triangulate.h>

int test_delaunay( int argc, char *argv[] )
{
    Polygon *poly = polygon_create_island();
    Mesh *mesh = mesh_triangulate_polygon( poly );
    mesh_save_to_eps( "test_delaunay_1_after_triangulation.eps", mesh );
    mesh_make_cdt_by_edge_flipping( mesh );
    mesh_save_to_eps( "test_delaunay_1_after_make_cdt.eps", mesh );
    mesh_split_encroached_boundary_edges( mesh );
    mesh_save_to_eps( "test_delaunay_1_after_split_encroached.eps", mesh );
    polygon_free( poly );
    mesh_free( mesh );

    mesh = mesh_new();
    Node *n1 = mesh_add_node( mesh, 0.0, 0.0 );
    Node *n2 = mesh_add_node( mesh, 1.0, 0.0 );
    Node *n3 = mesh_add_node( mesh, 1.0, 1.0 );
    Node *n4 = mesh_add_node( mesh, 0.0, 1.0 );
    Edge *e1 = mesh_add_edge( mesh, n1, n2 );
    Edge *e2 = mesh_add_edge( mesh, n2, n3 );
    Edge *e3 = mesh_add_edge( mesh, n3, n4 );
    Edge *e4 = mesh_add_edge( mesh, n4, n1 );
    Edge *e5 = mesh_add_edge( mesh, n1, n3 );
    mesh_add_element( mesh, &e1->he[0], &e2->he[0], &e5->he[1] );
    mesh_add_element( mesh, &e5->he[0], &e3->he[0], &e4->he[0] );
    e5 = mesh_swap_edge( mesh, e5 );
    Point2 p;
    point2_set( &p, 0.25, 0.25 );
    mesh_split_element( mesh, e1->he[0].element, &p );
    mesh_split_edge( mesh, e5, NULL, NULL );
    mesh_split_edge( mesh, e2, NULL, NULL );
    mesh_make_cdt_by_edge_flipping( mesh );
    mesh_save_to_eps( "test_delaunay_2_before_split_encroached.eps", mesh );
    mesh_split_encroached_boundary_edges( mesh );
    mesh_save_to_eps( "test_delaunay_2_after_split_encroached.eps", mesh );

    return EXIT_SUCCESS;
}

