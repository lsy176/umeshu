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
#include <point2.h>

int test_point2( int argc, char *argv[] )
{
    Point2 p1, p2, p3, p;
    
    point2_set( &p1, 1.23456789, 4.678692 );
    g_return_val_if_fail( p1.x == 1.23456789, 1 );
    g_return_val_if_fail( p1.y == 4.678692, 1 );

    point2_set( &p1, -1.0, 0.0 );
    point2_set( &p2, 0.0, 1.0 );
    g_return_val_if_fail( fabs( point2_distance( &p1, &p2 ) - sqrt(2.0) ) < SMALL_NUMBER, 1 );

    point2_set( &p1, 0.0, 1.0 );
    point2_set( &p2, -1.0, 0.0 );
    point2_set( &p3, 1.0, 0.0 );
    
    point2_set( &p,  0.0, 0.5 );
    g_return_val_if_fail( point2_is_in_triangle( &p, &p1, &p2, &p3 ), 1 );
    
    point2_set( &p,  0.0, 0.0 );
    g_return_val_if_fail( ! point2_is_in_triangle( &p, &p1, &p2, &p3 ), 1 );
    
    point2_set( &p,  0.5, 0.5 );
    g_return_val_if_fail( ! point2_is_in_triangle( &p, &p1, &p2, &p3 ), 1 );
    
    point2_set( &p,  0.5, 0.5 );
    g_return_val_if_fail( ! point2_is_in_triangle( &p, &p1, &p2, &p3 ), 1 );
    
    point2_set( &p,  1.0, 0.0 );
    g_return_val_if_fail( ! point2_is_in_triangle( &p, &p1, &p2, &p3 ), 1 );

    return 0;
}

