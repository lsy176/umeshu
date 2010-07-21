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

/* #include <math.h> */

#include "box2.h"

Box2 * box2_new( gdouble min_x, gdouble max_x, gdouble min_y, gdouble max_y )
{
    g_return_val_if_fail( min_x < max_x, NULL );
    g_return_val_if_fail( min_y < max_y, NULL );

    Box2 *box = g_slice_new( Box2 );
    box2_set( box, min_x, max_x, min_y, max_y );

    return box;
}

void box2_free( Box2 *box )
{
    g_slice_free( Box2, box );
}


void box2_set( Box2 *box, gdouble min_x, gdouble max_x, gdouble min_y, gdouble max_y )
{
    g_return_if_fail( box != NULL );
    g_return_if_fail( min_x < max_x );
    g_return_if_fail( min_y < max_y );

    box->min[0] = min_x;
    box->max[0] = max_x;
    box->min[1] = min_y;
    box->max[1] = max_y;    
}


gdouble box2_width( Box2 *box )
{
    g_return_val_if_fail( box != NULL, 0.0 );
    return box->max[0] - box->min[0];
}


gdouble box2_height( Box2 *box )
{
    g_return_val_if_fail( box != NULL, 0.0 );
    return box->max[1] - box->min[1];
}


void box2_include_point( Box2 *box, gdouble x, gdouble y )
{
    if ( x < box->min[0] )
        box->min[0] = x;
    else if ( x > box->max[0] )
        box->max[0] = x;

    if ( y < box->min[1] )
        box->min[1] = y;
    else if ( y > box->max[1] )
        box->max[1] = y;
}


void box2_include_box( Box2 *box, const Box2 *box2 )
{
    box2_include_point( box, box2->min[0], box2->min[1] );
    box2_include_point( box, box2->max[0], box2->max[1] );
}


void box2_print( const Box2 *box )
{
    g_return_if_fail( box != NULL );

    g_print( "Box2 %p: lower left = [%f, %f], upper right = [%f, %f]\n",
            box, box->min[0], box->min[1], box->max[0], box->max[1] );
}




