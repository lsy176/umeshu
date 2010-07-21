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

#include "mesh.h"
#include "polygon.h"

Polygon * polygon_new( void )
{
    Polygon *poly = g_slice_new0( Polygon );
    return poly;
}

void polygon_free( Polygon *poly )
{
    GList *vertices_iter;

    if ( poly != NULL )
    {
        for ( vertices_iter = poly->vertices; vertices_iter != NULL; vertices_iter = g_list_next( vertices_iter ) )
            g_slice_free( Point2, vertices_iter->data );
        g_list_free( poly->vertices );
        g_slice_free( Polygon, poly );
    }
}


void polygon_add_vertex( Polygon *poly, gdouble x, gdouble y )
{
    Point2 *vertex = g_slice_new( Point2 );
    vertex->x = x;
    vertex->y = y;
    poly->N++;
    poly->vertices = g_list_append( poly->vertices, vertex );
}


Polygon * polygon_create_island( void )
{
    Polygon *poly = polygon_new();

    polygon_add_vertex( poly, 0.35790337, 0.38864577 );
    polygon_add_vertex( poly, 0.31071450, 0.40218492 );
    polygon_add_vertex( poly, 0.26291196, 0.39019645 );
    polygon_add_vertex( poly, 0.21429804, 0.38283820 );
    polygon_add_vertex( poly, 0.16613300, 0.39383360 );
    polygon_add_vertex( poly, 0.11818326, 0.40414870 );
    polygon_add_vertex( poly, 0.08260139, 0.37538306 );
    polygon_add_vertex( poly, 0.05361811, 0.34207291 );
    polygon_add_vertex( poly, 0.02510484, 0.30843907 );
    polygon_add_vertex( poly, 0.00000000, 0.27304858 );
    polygon_add_vertex( poly, 0.01116075, 0.23446584 );
    polygon_add_vertex( poly, 0.05253742, 0.21147779 );
    polygon_add_vertex( poly, 0.08739790, 0.18215957 );
    polygon_add_vertex( poly, 0.13184782, 0.16365548 );
    polygon_add_vertex( poly, 0.17678370, 0.14569794 );
    polygon_add_vertex( poly, 0.20997123, 0.11555512 );
    polygon_add_vertex( poly, 0.23487661, 0.08013357 );
    polygon_add_vertex( poly, 0.27428417, 0.05577003 );
    polygon_add_vertex( poly, 0.30609623, 0.02448313 );
    polygon_add_vertex( poly, 0.34462248, 0.00000000 );
    polygon_add_vertex( poly, 0.38499725, 0.00703740 );
    polygon_add_vertex( poly, 0.41201013, 0.04110566 );
    polygon_add_vertex( poly, 0.44407198, 0.07251598 );
    polygon_add_vertex( poly, 0.47554188, 0.10433305 );
    polygon_add_vertex( poly, 0.48615282, 0.14127188 );
    polygon_add_vertex( poly, 0.48526458, 0.18209454 );
    polygon_add_vertex( poly, 0.48934181, 0.22286040 );
    polygon_add_vertex( poly, 0.51277842, 0.25712118 );
    polygon_add_vertex( poly, 0.55446391, 0.27665125 );
    polygon_add_vertex( poly, 0.59998065, 0.26611232 );
    polygon_add_vertex( poly, 0.63374109, 0.23635467 );
    polygon_add_vertex( poly, 0.68219858, 0.22657993 );
    polygon_add_vertex( poly, 0.72344682, 0.20595516 );
    polygon_add_vertex( poly, 0.76098068, 0.18314759 );
    polygon_add_vertex( poly, 0.81015186, 0.18044212 );
    polygon_add_vertex( poly, 0.85789280, 0.17704054 );
    polygon_add_vertex( poly, 0.90231042, 0.15823834 );
    polygon_add_vertex( poly, 0.94725198, 0.14091650 );
    polygon_add_vertex( poly, 0.99263450, 0.14300645 );
    polygon_add_vertex( poly, 1.00000000, 0.18113114 );
    polygon_add_vertex( poly, 0.97482369, 0.21584709 );
    polygon_add_vertex( poly, 0.93601160, 0.24166059 );
    polygon_add_vertex( poly, 0.89769796, 0.26773059 );
    polygon_add_vertex( poly, 0.88612883, 0.30756282 );
    polygon_add_vertex( poly, 0.88036141, 0.34822807 );
    polygon_add_vertex( poly, 0.86637401, 0.38744400 );
    polygon_add_vertex( poly, 0.87126547, 0.42698375 );
    polygon_add_vertex( poly, 0.87836588, 0.46738117 );
    polygon_add_vertex( poly, 0.86679577, 0.50495740 );
    polygon_add_vertex( poly, 0.83885184, 0.53889814 );
    polygon_add_vertex( poly, 0.80885902, 0.57161229 );
    polygon_add_vertex( poly, 0.77548249, 0.60209880 );
    polygon_add_vertex( poly, 0.73774132, 0.62777801 );
    polygon_add_vertex( poly, 0.69229886, 0.64485535 );
    polygon_add_vertex( poly, 0.64664787, 0.66155981 );
    polygon_add_vertex( poly, 0.59924870, 0.67318904 );
    polygon_add_vertex( poly, 0.54986089, 0.67381738 );
    polygon_add_vertex( poly, 0.50097893, 0.66757989 );
    polygon_add_vertex( poly, 0.46691922, 0.69313520 );
    polygon_add_vertex( poly, 0.44449627, 0.72973059 );
    polygon_add_vertex( poly, 0.40854079, 0.75668093 );
    polygon_add_vertex( poly, 0.36290331, 0.77039793 );
    polygon_add_vertex( poly, 0.36624793, 0.79505758 );
    polygon_add_vertex( poly, 0.37098973, 0.82539964 );
    polygon_add_vertex( poly, 0.32954240, 0.84365456 );
    polygon_add_vertex( poly, 0.32368068, 0.87731391 );
    polygon_add_vertex( poly, 0.36206162, 0.90145428 );
    polygon_add_vertex( poly, 0.41147458, 0.90716326 );
    polygon_add_vertex( poly, 0.45332716, 0.92956622 );
    polygon_add_vertex( poly, 0.50176592, 0.93780257 );
    polygon_add_vertex( poly, 0.54970945, 0.94883030 );
    polygon_add_vertex( poly, 0.57634292, 0.97479673 );
    polygon_add_vertex( poly, 0.53570147, 0.98649919 );
    polygon_add_vertex( poly, 0.48652917, 0.99385804 );
    polygon_add_vertex( poly, 0.43997944, 0.98533700 );
    polygon_add_vertex( poly, 0.40368105, 1.00000000 );
    polygon_add_vertex( poly, 0.36167108, 0.98189826 );
    polygon_add_vertex( poly, 0.32369722, 0.95572597 );
    polygon_add_vertex( poly, 0.28265677, 0.93271435 );
    polygon_add_vertex( poly, 0.24867592, 0.90286457 );
    polygon_add_vertex( poly, 0.22284050, 0.86781008 );
    polygon_add_vertex( poly, 0.22698494, 0.82708497 );
    polygon_add_vertex( poly, 0.23260900, 0.78640307 );
    polygon_add_vertex( poly, 0.25654498, 0.75174722 );
    polygon_add_vertex( poly, 0.29129654, 0.72243251 );
    polygon_add_vertex( poly, 0.31907943, 0.68878353 );
    polygon_add_vertex( poly, 0.34391654, 0.65324854 );
    polygon_add_vertex( poly, 0.37314261, 0.62077328 );
    polygon_add_vertex( poly, 0.41247022, 0.59552654 );
    polygon_add_vertex( poly, 0.45405330, 0.57278959 );
    polygon_add_vertex( poly, 0.46246974, 0.53265774 );
    polygon_add_vertex( poly, 0.47030304, 0.49222005 );
    polygon_add_vertex( poly, 0.47073181, 0.45153024 );
    polygon_add_vertex( poly, 0.44700288, 0.41863218 );
    polygon_add_vertex( poly, 0.40728793, 0.39400451 );

    return poly;
}


Polygon * polygon_create_A( void )
{
    Polygon *poly = polygon_new();

    polygon_add_vertex( poly, 0.00000000, 0.00000000 );
    polygon_add_vertex( poly, 0.28533333, 0.00000000 );
    polygon_add_vertex( poly, 0.28533333, 0.02735978 );
    polygon_add_vertex( poly, 0.22400000, 0.03556772 );
    polygon_add_vertex( poly, 0.19733333, 0.05335157 );
    polygon_add_vertex( poly, 0.18666667, 0.09028728 );
    polygon_add_vertex( poly, 0.19866667, 0.14637483 );
    polygon_add_vertex( poly, 0.26800000, 0.32147743 );
    polygon_add_vertex( poly, 0.64533333, 0.32147743 );
    polygon_add_vertex( poly, 0.70133333, 0.18467852 );
    polygon_add_vertex( poly, 0.73200000, 0.08207934 );
    polygon_add_vertex( poly, 0.72266667, 0.05061560 );
    polygon_add_vertex( poly, 0.70266667, 0.03556772 );
    polygon_add_vertex( poly, 0.63200000, 0.02735978 );
    polygon_add_vertex( poly, 0.63200000, 0.00000000 );
    polygon_add_vertex( poly, 1.00000000, 0.00000000 );
    polygon_add_vertex( poly, 1.00000000, 0.02735978 );
    polygon_add_vertex( poly, 0.95733333, 0.03419973 );
    polygon_add_vertex( poly, 0.92666667, 0.05335157 );
    polygon_add_vertex( poly, 0.90133333, 0.09028728 );
    polygon_add_vertex( poly, 0.50800000, 1.00000000 );
    polygon_add_vertex( poly, 0.48133333, 1.00000000 );
    polygon_add_vertex( poly, 0.12933333, 0.15458276 );
    polygon_add_vertex( poly, 0.07600000, 0.06155951 );
    polygon_add_vertex( poly, 0.03333333, 0.03283174 );
    polygon_add_vertex( poly, 0.00000000, 0.02735978 );

    return poly;
}


