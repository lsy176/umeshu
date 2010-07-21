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

#include <element.h>
#include <mesh.h>
#include <point2.h>

int test_element( int argc, char *argv[] )
{
    Element *e = element_new();
    g_return_val_if_fail( e->adjacent_halfedge == NULL , 1 );
    element_free( e );

    Point2 p1, p2, p3;
    point2_set( &p1, 0.0, 1.0 );
    point2_set( &p2, 0.0, 0.0 );
    point2_set( &p3, sqrt( 3.0 ), 0.0 );

    gdouble a1, a2, a3;
    triangle_angles( &p1, &p2, &p3, &a1, &a2, &a3 );
    g_return_val_if_fail( fabs( a1 - G_PI/3.0) < SMALL_NUMBER, 1 );
    g_return_val_if_fail( fabs( a2 - G_PI/2.0) < SMALL_NUMBER, 1 );
    g_return_val_if_fail( fabs( a3 - G_PI/6.0) < SMALL_NUMBER, 1 );

    a1 = triangle_circumcircle_radius( &p1, &p2, &p3 );
    g_return_val_if_fail( fabs( a1 - 1.0 ) < SMALL_NUMBER, 1 );

    a1 = triangle_circumcircle_radius_edge_length( 1.0, sqrt( 3.0 ), 2.0 );
    g_return_val_if_fail( fabs( a1 - 1.0 ) < SMALL_NUMBER, 1 );

    Point2 p;
    triangle_circumcenter_coordinates( &p1, &p2, &p3, &p );
    g_return_val_if_fail( fabs( p.x - sqrt( 3.0 )/2.0 ) < SMALL_NUMBER, 1 );
    g_return_val_if_fail( fabs( p.y - 1.0/2.0 ) < SMALL_NUMBER, 1 );

    a1 = triangle_area( &p1, &p2, &p3 );
    g_return_val_if_fail( fabs( a1 - sqrt( 3.0 )/2.0 ) < SMALL_NUMBER, 1 );


    Mesh *mesh = mesh_new();
    Node *n1 = mesh_add_node( mesh, 0.0, 1.0 );
    Node *n2 = mesh_add_node( mesh, 0.0, 0.0 );
    Node *n3 = mesh_add_node( mesh, sqrt(3.0), 0.0 );
    Edge *e1 = mesh_add_edge( mesh, n1, n2 );
    Edge *e2 = mesh_add_edge( mesh, n2, n3 );
    Edge *e3 = mesh_add_edge( mesh, n3, n1 );
    Element *el = mesh_add_element( mesh, &e1->he[0], &e2->he[0], &e3->he[0] );

    gdouble l1, l2, l3;
    element_edge_lengths( el, &l1, &l2, &l3 );
    g_return_val_if_fail( fabs( l1 - 1.0) < SMALL_NUMBER, 1);
    g_return_val_if_fail( fabs( l2 - sqrt(3.0)) < SMALL_NUMBER, 1);
    g_return_val_if_fail( fabs( l3 - 2.0) < SMALL_NUMBER, 1);

    HalfEdge *he = element_min_edge( el, &l1 );
    g_return_val_if_fail( he->edge == e1, 1 );
    g_return_val_if_fail( fabs( l1 - 1.0) < SMALL_NUMBER, 1);

    l1 = element_min_edge_length( el );
    g_return_val_if_fail( fabs( l1 - 1.0) < SMALL_NUMBER, 1);

    he = element_max_edge( el, &l1 );
    g_return_val_if_fail( he->edge == e3, 1 );
    g_return_val_if_fail( fabs( l1 - 2.0) < SMALL_NUMBER, 1);

    l1 = element_max_edge_length( el );
    g_return_val_if_fail( fabs( l1 - 2.0) < SMALL_NUMBER, 1);

    element_angles( el, &a1, &a2, &a3 );
    g_return_val_if_fail( fabs( a1 - G_PI/3.0) < SMALL_NUMBER, 1);
    g_return_val_if_fail( fabs( a2 - G_PI/2.0) < SMALL_NUMBER, 1);
    g_return_val_if_fail( fabs( a3 - G_PI/6.0) < SMALL_NUMBER, 1);

    a1 = element_maximum_angle( el );
    g_return_val_if_fail( fabs( a1 - G_PI/2.0) < SMALL_NUMBER, 1);

    a1 = element_minimum_angle( el );
    g_return_val_if_fail( fabs( a1 - G_PI/6.0) < SMALL_NUMBER, 1);

    point2_set( NODE_POSITION( n3 ), 3.0, 0.0 );
    a1 = element_area( el );
    g_return_val_if_fail( fabs( a1 - 3.0/2.0) < SMALL_NUMBER, 1);

    element_circumcenter_coordinates( el, &p );
    g_return_val_if_fail( fabs( p.x - sqrt( 3.0 )/2.0 ) < SMALL_NUMBER, 1);
    g_return_val_if_fail( fabs( p.y - 1.0/2.0 ) < SMALL_NUMBER, 1);

    return 0;
}

