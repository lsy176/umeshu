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

#include <box2.h>

int test_box2( int argc, char *argv[] )
{
    Box2 *box = box2_new( 1.0, 2.0, 3.0, 4.0 );

    g_return_val_if_fail( box->min[0] == 1.0, 1 );
    g_return_val_if_fail( box->max[0] == 2.0, 1 );
    g_return_val_if_fail( box->min[1] == 3.0, 1 );
    g_return_val_if_fail( box->max[1] == 4.0, 1 );

    box2_set( box, 1.0, 2.0, 3.0, 4.0 );
    g_return_val_if_fail( box->min[0] == 1.0, 1 );
    g_return_val_if_fail( box->max[0] == 2.0, 1 );
    g_return_val_if_fail( box->min[1] == 3.0, 1 );
    g_return_val_if_fail( box->max[1] == 4.0, 1 );

    g_return_val_if_fail( box2_width( box ) == 2.0-1.0, 1 );
    g_return_val_if_fail( box2_height( box ) == 4.0-3.0, 1 );
    
    box2_include_point( box, 5.0, 5.0 );
    g_return_val_if_fail( box->min[0] == 1.0, 1 );
    g_return_val_if_fail( box->max[0] == 5.0, 1 );
    g_return_val_if_fail( box->min[1] == 3.0, 1 );
    g_return_val_if_fail( box->max[1] == 5.0, 1 );

    box2_include_point( box, -1.0, -1.0 );
    g_return_val_if_fail( box->min[0] == -1.0, 1 );
    g_return_val_if_fail( box->max[0] == 5.0, 1 );
    g_return_val_if_fail( box->min[1] == -1.0, 1 );
    g_return_val_if_fail( box->max[1] == 5.0, 1 );

    Box2 *box2 = box2_new( -1.0, 6.0, -1.0, 6.0 );
    box2_include_box( box, box2 );
    g_return_val_if_fail( box->min[0] == -1.0, 1 );
    g_return_val_if_fail( box->max[0] == 6.0, 1 );
    g_return_val_if_fail( box->min[1] == -1.0, 1 );
    g_return_val_if_fail( box->max[1] == 6.0, 1 );

    box2_set( box, 1.0, 2.0, 3.0, 4.0 );
    box2_set( box2, 1.0, 2.0, 2.0, 5.0 );
    box2_include_box( box, box2 );
    g_return_val_if_fail( box->min[0] == 1.0, 1 );
    g_return_val_if_fail( box->max[0] == 2.0, 1 );
    g_return_val_if_fail( box->min[1] == 2.0, 1 );
    g_return_val_if_fail( box->max[1] == 5.0, 1 );

    box2_set( box, 1.0, 2.0, 3.0, 4.0 );
    box2_set( box2, 1.5, 1.8, 3.5, 3.8 );
    box2_include_box( box, box2 );
    g_return_val_if_fail( box->min[0] == 1.0, 1 );
    g_return_val_if_fail( box->max[0] == 2.0, 1 );
    g_return_val_if_fail( box->min[1] == 3.0, 1 );
    g_return_val_if_fail( box->max[1] == 4.0, 1 );

    box2_free( box );
    box2_free( box2 );

    return 0;
}

