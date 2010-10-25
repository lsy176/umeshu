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
#include <math.h>

#include <mesh.h>
#include <mesh_delaunay.h>
#include <mesh_macros.h>
#include <mesh_output.h>
#include <mesh_refine.h>
#include <mesh_relax.h>
#include <polygon_triangulate.h>

int test_relax( int argc, char *argv[] )
{
    Polygon *p = polygon_create_island();
    Mesh *mesh = mesh_triangulate_polygon( p );
    mesh_make_cdt_by_edge_flipping( mesh );
    mesh_refine( mesh, RUPPERT_REFINEMENT, 0.001, RADIANS(25) );
    mesh_save_to_eps( "test_relax_1.eps", mesh );
    printf( "mesh->Np = %u\n", mesh->Np );
    mesh_relax( mesh );
    printf( "mesh->Np = %u\n", mesh->Np );
    mesh_save_to_eps( "test_relax_2.eps", mesh );
    mesh_free( mesh );

    return EXIT_SUCCESS;
}


