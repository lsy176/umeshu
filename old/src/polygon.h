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

#ifndef __POLYGON_H_INCLUDED__
#define __POLYGON_H_INCLUDED__ 

/** \file
 * \brief Functions for working with polygons that are used for definition of
 * domain boundary
 *
 * \author Vladimir Chalupecky
 */

#include <glib.h>

/** Polygon data structure. */
typedef struct
{
    /** List of polygon's vertices represented as pointers to Node. For use with
     * the mesh generation functions, the polygon should be oriented in a
     * anticlockwise fashion. */
    GList *vertices;
    unsigned int N;
} Polygon;

/** Allocates an empty polygon.
 *
 * \return Polygon
 */
Polygon * polygon_new( void );

/** Frees a polygon from memory.
 *
 * \param poly polygon to be freed
 * \param free_node indicates if the nodes should be freed as well
 */
void polygon_free( Polygon *poly );

/** Adds a vertex to the polygon. The vertices are added in a clock-wise
 * fashion.
 *
 * \param poly polygon to which the vertex is added
 * \param x x-coordinate of the vertex
 * \param y y-coordinate of the vertex
 */
void polygon_add_vertex( Polygon *poly, gdouble x, gdouble y );

/** Reads a polygon from a file. Each line corresponds to one vertex and
 * consists of the two coordinates separated by a whitespace. The polygon is
 * ended by EOF. 
 *
 * \param poly polygon where the read vertices should be stored
 * \param filename name of the file
 */
void polygon_read( Polygon *poly, const gchar *filename );

/** Creates a predefined polygon with a shape of an island.
 *
 * \return created polygon 
 */
Polygon * polygon_create_island( void );

/** Creates a predefined polygon with a shape of the letter A.
 *
 * \return created polygon 
 */
Polygon * polygon_create_A( void );

/** Creates a predefined polygon with a shape of a rectangle.
 *
 * \return created polygon 
 */
Polygon * polygon_create_box( gdouble xmin, gdouble ymin, gdouble xmax, gdouble ymax );

#endif /* __POLYGON_H_INCLUDED__ */

