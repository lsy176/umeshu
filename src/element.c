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

#include "edge.h"
#include "element.h"
#include "point2.h"

Element * element_new( void )
{
    Element *element = g_slice_new0( Element );
    return element;
}


void element_free( Element *el )
{
    g_slice_free( Element, el );
}


void element_edge_lengths( const Element *element,
        gdouble *length1, gdouble *length2, gdouble *length3 )
{
    g_return_if_fail( element != NULL );
    g_return_if_fail( length1 != NULL );
    g_return_if_fail( length2 != NULL );
    g_return_if_fail( length3 != NULL );

    HalfEdge *he = element->adjacent_halfedge;
    Point2 *p1 = NODE_POSITION(he->origin);
    Point2 *p2 = NODE_POSITION(he->next->origin);
    Point2 *p3 = NODE_POSITION(he->next->next->origin);
    
    *length1 = point2_distance( p1, p2 );
    *length2 = point2_distance( p2, p3 );
    *length3 = point2_distance( p3, p1 );
}


HalfEdge * element_min_edge( const Element *el, gdouble *length )
{
    g_return_val_if_fail( el != NULL, NULL );

    HalfEdge *he_iter = el->adjacent_halfedge;
    HalfEdge *min_he = he_iter;
    gdouble min_length = edge_length( he_iter->edge );

    for ( he_iter = he_iter->next; he_iter != el->adjacent_halfedge; he_iter = he_iter->next )
    {
        gdouble l = edge_length( he_iter->edge );
        if ( l < min_length )
        {
            min_length = l;
            min_he = he_iter;
        }
    }

    if ( length != NULL )
        *length = min_length;

    return min_he;
}


gdouble element_min_edge_length( const Element *element )
{
    g_return_val_if_fail( element != NULL, G_MAXDOUBLE );

    gdouble e1_length, e2_length, e3_length;
    element_edge_lengths( element, &e1_length, &e2_length, &e3_length );
    return MIN( e1_length, MIN( e2_length, e3_length ) );
}


HalfEdge * element_max_edge( const Element *el, gdouble *length )
{
    g_return_val_if_fail( el != NULL, NULL );

    HalfEdge *he_iter = el->adjacent_halfedge;
    HalfEdge *max_he = he_iter;
    gdouble max_length = edge_length( he_iter->edge );

    for ( he_iter = he_iter->next; he_iter != el->adjacent_halfedge; he_iter = he_iter->next )
    {
        gdouble l = edge_length( he_iter->edge );
        if ( l > max_length )
        {
            max_length = l;
            max_he = he_iter;
        }
    }

    if ( length != NULL )
        *length = max_length;

    return max_he;
}


gdouble element_max_edge_length( const Element *element )
{
    g_return_val_if_fail( element != NULL, 0.0 );

    gdouble e1_length, e2_length, e3_length;
    element_edge_lengths( element, &e1_length, &e2_length, &e3_length );
    return MAX( e1_length, MAX( e2_length, e3_length ) );
}


void triangle_angles( const Point2 *p1, const Point2 *p2, const Point2 *p3,
        gdouble *angle1, gdouble *angle2, gdouble *angle3 )
{
    g_return_if_fail( p1 != NULL );
    g_return_if_fail( p2 != NULL );
    g_return_if_fail( p3 != NULL );
    g_return_if_fail( angle1 != NULL );
    g_return_if_fail( angle2 != NULL );
    g_return_if_fail( angle3 != NULL );

    gdouble a, b, c;
    a = point2_distance( p2, p3 );
    b = point2_distance( p1, p3 );
    c = point2_distance( p1, p2 );
    gdouble aa = a*a;
    gdouble bb = b*b;
    gdouble cc = c*c;

    *angle1 = acos( (bb+cc-aa)/(2.0*b*c) );
    *angle2 = acos( (aa+cc-bb)/(2.0*a*c) );
    *angle3 = acos( (aa+bb-cc)/(2.0*a*b) );
}


void element_angles( const Element *element,
        gdouble *angle1, gdouble *angle2, gdouble *angle3 )
{
    g_return_if_fail( element != NULL );
    g_return_if_fail( angle1 != NULL );
    g_return_if_fail( angle2 != NULL );
    g_return_if_fail( angle3 != NULL );

    gdouble a, b, c;
    element_edge_lengths( element, &c, &a, &b );
    gdouble aa = a*a;
    gdouble bb = b*b;
    gdouble cc = c*c;

    *angle1 = acos( (bb+cc-aa)/(2.0*b*c) );
    *angle2 = acos( (aa+cc-bb)/(2.0*a*c) );
    *angle3 = acos( (aa+bb-cc)/(2.0*a*b) );
}


gdouble element_minimum_angle( const Element *element )
{
    g_return_val_if_fail( element != NULL, G_MAXDOUBLE );

    gdouble angle1, angle2, angle3;
    element_angles( element, &angle1, &angle2, &angle3 );
    return MIN( angle1, MIN( angle2, angle3 ) );
}


gdouble element_maximum_angle( const Element *element )
{
    g_return_val_if_fail( element != NULL, 0.0 );

    gdouble angle1, angle2, angle3;
    element_angles( element, &angle1, &angle2, &angle3 );
    return MAX( angle1, MAX( angle2, angle3 ) );
}


gdouble triangle_area( const Point2 *p1, const Point2 *p2, const Point2 *p3 )
{
    gdouble l1 = point2_distance( p1, p2 );
    gdouble l2 = point2_distance( p2, p3 );
    gdouble l3 = point2_distance( p3, p1 );
    gdouble s = 0.5*( l1 + l2 + l3 );
    return sqrt( s*( s-l1 )*( s-l2 )*( s-l3 ) );
}


gdouble element_area( const Element *element )
{
    g_return_val_if_fail( element != NULL, 0.0 );

    HalfEdge *he = element->adjacent_halfedge;
    Point2 *p1 = NODE_POSITION(he->origin);
    Point2 *p2 = NODE_POSITION(he->next->origin);
    Point2 *p3 = NODE_POSITION(he->next->next->origin);

    return triangle_area( p1, p2, p3 );
}


gdouble triangle_circumcircle_radius_edge_length( gdouble e1_length, gdouble e2_length, gdouble e3_length )
{
    gdouble s = 0.5*(e1_length + e2_length + e3_length);
    return 0.25*e1_length*e2_length*e3_length / sqrt(s*(s-e1_length)*(s-e2_length)*(s-e3_length));
}


gdouble triangle_circumcircle_radius( Point2 *p1, Point2 *p2, Point2 *p3 )
{
    return triangle_circumcircle_radius_edge_length(
            point2_distance(p1,p2),
            point2_distance(p2,p3),
            point2_distance(p3,p1) );
}


gdouble element_circumcircle_radius( const Element *el )
{
    g_return_val_if_fail( el != NULL, 0.0 );

    HalfEdge *he = el->adjacent_halfedge;
    Point2 *p1 = NODE_POSITION( he->origin );
    Point2 *p2 = NODE_POSITION( he->pair->origin );
    Point2 *p3 = NODE_POSITION( he->previous->origin );

    return triangle_circumcircle_radius( p1, p2, p3 );
}


void triangle_circumcenter_coordinates(
        const Point2 *p1, const Point2 *p2, const Point2 *p3,
        Point2 *center )
{
    gdouble Bx = p2->x - p1->x;
    gdouble By = p2->y - p1->y;
    gdouble Cx = p3->x - p1->x;
    gdouble Cy = p3->y - p1->y;
    gdouble Bx2 = Bx*Bx;
    gdouble By2 = By*By;
    gdouble Cx2 = Cx*Cx;
    gdouble Cy2 = Cy*Cy;
    gdouble D = 2.0*( Bx*Cy - By*Cx );
    center->x = ( Cy*(Bx2 + By2) - By*(Cx2+Cy2) )/D + p1->x;
    center->y = ( Bx*(Cx2 + Cy2) - Cx*(Bx2+By2) )/D + p1->y;
}


void element_circumcenter_coordinates( const Element *el, Point2 *center )
{
    g_return_if_fail( el != NULL );
    g_return_if_fail( center != NULL );

    HalfEdge *he = el->adjacent_halfedge;
    Point2 *p1 = NODE_POSITION(he->origin);
    Point2 *p2 = NODE_POSITION(he->next->origin);
    Point2 *p3 = NODE_POSITION(he->next->next->origin);

    triangle_circumcenter_coordinates( p1, p2, p3, center );
}


gdouble element_fem_quality( const Element * el )
{
    g_return_val_if_fail( el != NULL, G_MAXDOUBLE );

    HalfEdge *he = el->adjacent_halfedge;
    Point2 *p1 = NODE_POSITION(he->origin);
    Point2 *p2 = NODE_POSITION(he->next->origin);
    Point2 *p3 = NODE_POSITION(he->previous->origin);

    gdouble a, b, c;
    a = point2_distance( p1, p2 );
    b = point2_distance( p2, p3 );
    c = point2_distance( p3, p1 );

    // ratio of the longest edge length and the radius of inscribed circle
    gdouble h_max = MAX( a, MAX( b, c ) );
    gdouble s = ( a + b + c )/2.0;
    return 2.0 * SQRT3 * sqrt((s-a)*(s-b)*(s-c)/s) / h_max;
}
