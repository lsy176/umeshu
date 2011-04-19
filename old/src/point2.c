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

#include "point2.h"
#include "predicates.h"

void point2_set( Point2 *point, gdouble x, gdouble y )
{
    g_return_if_fail( point != NULL );
    point->x = x;
    point->y = y;
}


gdouble point2_distance( const Point2 *p1, const Point2 *p2 )
{
    gdouble x = p1->x - p2->x;
    gdouble y = p1->y - p2->y;
    return sqrt( x*x + y*y );
}


gboolean point2_is_in_triangle( const Point2 *p, const Point2 *p1, const Point2 *p2, const Point2 *p3 )
{
    if ( orient2d_test( (double*)p1, (double*)p2, (double*)p ) > 0.0 &&
         orient2d_test( (double*)p2, (double*)p3, (double*)p ) > 0.0 &&
         orient2d_test( (double*)p3, (double*)p1, (double*)p ) > 0.0 )
        return TRUE;
    else
        return FALSE;
}

Point2 point2_interpolate( const Point2 *p1, const Point2 *p2, gdouble t )
{
    Point2 p;
    g_return_val_if_fail( 0.0 <= t && t <= 1.0, p );

    p.x = (1.0-t)*p1->x + t*p2->x;
    p.y = (1.0-t)*p1->y + t*p2->y;

    return p;
}


