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
#include <mesh_smooth.h>
#include <polygon_triangulate.h>

int test_smooth( int argc, char *argv[] )
{
    /* Polygon *p = polygon_create_box( 0.0, 0.0, 1.0, 1.0 ); */
    Polygon *p = polygon_create_island();

    /* LAPLACIAN SMOOTHING */
    Mesh *mesh = mesh_triangulate_polygon( p );
    mesh_make_cdt_by_edge_flipping( mesh );

    gdouble area = 0.0005;
    guint reps = 10;
    
    mesh_refine( mesh, RUPPERT_REFINEMENT, area, RADIANS(30) );
    mesh_save_to_eps( "test_smooth_lapl_1.eps", mesh );
    mesh_relax( mesh );
    mesh_save_to_eps( "test_smooth_lapl_2.eps", mesh );
    mesh_smooth( mesh, LAPLACIAN_SMOOTHING, reps, area );
    mesh_save_to_eps( "test_smooth_lapl_3.eps", mesh );
    mesh_relax( mesh );
    mesh_save_to_eps( "test_smooth_lapl_4.eps", mesh );
    mesh_smooth( mesh, LAPLACIAN_SMOOTHING, reps, area );
    mesh_save_to_eps( "test_smooth_lapl_5.eps", mesh );

    mesh_free( mesh );
    
    /* ELEMENT QUALITY SMOOTHING */
    mesh = mesh_triangulate_polygon( p );
    mesh_make_cdt_by_edge_flipping( mesh );
    
    mesh_refine( mesh, RUPPERT_REFINEMENT, area, RADIANS(30) );
    mesh_save_to_eps( "test_smooth_elq_1.eps", mesh );
    mesh_relax( mesh );
    mesh_save_to_eps( "test_smooth_elq_2.eps", mesh );
    mesh_smooth( mesh, ELEMENT_QUALITY_SMOOTHING, reps, area );
    mesh_save_to_eps( "test_smooth_elq_3.eps", mesh );
    mesh_relax( mesh );
    mesh_save_to_eps( "test_smooth_elq_4.eps", mesh );
    mesh_smooth( mesh, ELEMENT_QUALITY_SMOOTHING, reps, area );
    mesh_save_to_eps( "test_smooth_elq_5.eps", mesh );

    mesh_free( mesh );

    /* EDGE LENGTH SMOOTHING */
    mesh = mesh_triangulate_polygon( p );
    mesh_make_cdt_by_edge_flipping( mesh );
    
    mesh_refine( mesh, RUPPERT_REFINEMENT, area, RADIANS(30) );
    mesh_save_to_eps( "test_smooth_el_1.eps", mesh );
    mesh_relax( mesh );
    mesh_save_to_eps( "test_smooth_el_2.eps", mesh );
    mesh_smooth( mesh, EDGE_LENGTH_SMOOTHING, reps, area );
    mesh_save_to_eps( "test_smooth_el_3.eps", mesh );
    mesh_relax( mesh );
    mesh_save_to_eps( "test_smooth_el_4.eps", mesh );
    mesh_smooth( mesh, EDGE_LENGTH_SMOOTHING, reps, area );
    mesh_save_to_eps( "test_smooth_el_5.eps", mesh );

    mesh_free( mesh );

    /* BOSSEN-HECKBERT SMOOTHING */
    mesh = mesh_triangulate_polygon( p );
    mesh_make_cdt_by_edge_flipping( mesh );
    
    mesh_refine( mesh, RUPPERT_REFINEMENT, area, RADIANS(30) );
    mesh_save_to_eps( "test_smooth_bh_1.eps", mesh );
    mesh_relax( mesh );
    mesh_save_to_eps( "test_smooth_bh_2.eps", mesh );
    mesh_smooth( mesh, BOSSEN_HECKBERT_SMOOTHING, 100*reps, area );
    mesh_save_to_eps( "test_smooth_bh_3.eps", mesh );
    mesh_relax( mesh );
    mesh_save_to_eps( "test_smooth_bh_4.eps", mesh );
    mesh_smooth( mesh, BOSSEN_HECKBERT_SMOOTHING, 100*reps, area );
    mesh_save_to_eps( "test_smooth_bh_5.eps", mesh );

    mesh_free( mesh );

    polygon_free( p );

    return EXIT_SUCCESS;
}

