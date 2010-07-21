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

#include <stdio.h>
#include <math.h>

#include <config.h>

#include "box2.h"
#include "edge.h"
#include "mesh.h"
#include "mesh_output.h"

/* 1cm is this amount of points */
#define CM 28.3464567

/* default figure size is 10cm */
#define FIG_SIZE 10*CM

/* default figure margin is 1mm */
#define FIG_MARGIN 0.1*CM

void mesh_save_to_eps( const gchar *filename, const Mesh *mesh )
{
    FILE *eps_file = fopen( filename, "w" );
    if ( eps_file == NULL )
    {
        g_warning( "EPS file %s could not be opened\n", filename );
        return;
    }

    Box2 box;
    mesh_get_bounding_box( mesh, &box );
    gdouble width = box2_width( &box );
    gdouble height = box2_height( &box );

    guint fig_width = FIG_SIZE;
    guint fig_height = height / width * fig_width;

    GTimeVal cur_time;
    g_get_current_time( &cur_time );

    fprintf( eps_file, "%%!PS-Adobe-3.0 EPSF-3.0\n" );
    fprintf( eps_file, "%%%%BoundingBox: %u %u %u %u\n", 0, 0, (unsigned int) ceil( fig_width + 2*FIG_MARGIN ), (unsigned int) ceil( fig_height + 2*FIG_MARGIN ) );
    fprintf( eps_file, "%%%%HiResBoundingBox: %f %f %f %f\n", 0.0, 0.0, fig_width + 2*FIG_MARGIN, fig_height + 2*FIG_MARGIN );
    fprintf( eps_file, "%%%%Creator: %s\n", PACKAGE_STRING );
    fprintf( eps_file, "%%%%CreationDate: %s\n", g_time_val_to_iso8601( &cur_time ) );
    fprintf( eps_file, "%%%%Title: %s %s\n", "Unstructured mesh", filename );
    fprintf( eps_file, "%%%%EndComments\n" );
    fprintf( eps_file, "newpath\n" );

    GList *edges_iter;
    for ( edges_iter = mesh->edges; edges_iter != NULL; edges_iter = g_list_next( edges_iter ) )
    {
        const Edge *edge = EDGE(edges_iter->data);
        edge_print( edge );
        const Point2 *p1 = NODE_POSITION(edge->he[0].origin);
        const Point2 *p2 = NODE_POSITION(edge->he[1].origin);
        fprintf( eps_file, "%f %f moveto\n", fig_width*(p1->x - box.min[0])/width + FIG_MARGIN, fig_height*(p1->y - box.min[1])/height + FIG_MARGIN );
        fprintf( eps_file, "%f %f lineto\n", fig_width*(p2->x - box.min[0])/width + FIG_MARGIN, fig_height*(p2->y - box.min[1])/height + FIG_MARGIN );
    }
    fprintf( eps_file, "%f setlinewidth\n", 0.05*CM );
    fprintf( eps_file, "stroke\n" );
    fprintf( eps_file, "%%%%EOF\n" );

    fclose( eps_file );
}

