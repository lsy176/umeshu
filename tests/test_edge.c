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

#include <edge.h>
#include <node.h>
#include <math.h>

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

    return 0;
}


