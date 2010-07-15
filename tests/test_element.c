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

    return 0;
}

