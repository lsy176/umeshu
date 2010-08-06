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
#include <math.h>

#include <mesh.h>
#include <mesh_output.h>
#include <polygon_triangulate.h>

int test_polygon( int argc, char *argv[] )
{
    Polygon *p = polygon_new();
    polygon_add_vertex( p, 0.0, 0.0 );
    polygon_add_vertex( p, 0.5, 0.5 );
    polygon_add_vertex( p, 1.0, 0.0 );
    polygon_add_vertex( p, 1.0, 1.0 );
    polygon_add_vertex( p, 0.0, 1.0 );
    g_return_val_if_fail( p->N == 5, EXIT_FAILURE );
    Mesh *mesh = mesh_triangulate_polygon( p );
    mesh_save_to_eps( "test_polygon_1.eps", mesh );
    g_return_val_if_fail( mesh->Nt == 3, EXIT_FAILURE );
    polygon_free( p );
    mesh_free( mesh );

    p = polygon_create_island();
    mesh = mesh_triangulate_polygon( p );
    mesh_save_to_eps( "test_polygon_2.eps", mesh );
    polygon_free( p );
    mesh_free( mesh );

    p = polygon_create_A();
    mesh = mesh_triangulate_polygon( p );
    mesh_save_to_eps( "test_polygon_3.eps", mesh );
    polygon_free( p );
    mesh_free( mesh );

    /* the triangulation routine is not so robust to cope with this situation.
     * When it is, we can uncomment this test */
    /* p = polygon_new();
    polygon_add_vertex( p, 0.0, 0.0 );
    polygon_add_vertex( p, 1.0, 0.5 );
    polygon_add_vertex( p, 1.0, 0.0 );
    polygon_add_vertex( p, 1.0, 1.0 );
    polygon_add_vertex( p, 0.0, 1.0 );
    mesh = mesh_triangulate_polygon( p );
    mesh_save_to_eps( "test_polygon_4.eps", mesh );
    g_return_val_if_fail( mesh->Nt == 2, EXIT_FAILURE );
    polygon_free( p );
    mesh_free( mesh ); */


    return EXIT_SUCCESS;
}

