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

#ifndef __ELEMENT_H_INCLUDED__
#define __ELEMENT_H_INCLUDED__ 

/** \file
 * \brief Element related functions.
 *
 * \author Vladimir Chalupecky
 */

#include "mesh_types.h"

/** Allocates memory for a new element
 *
 * \return pointer to the newly created element
 * \retval NULL indicates that the memory allocation failed
 */
Element * element_new( void );

/** Frees an element from memory
 *
 * \param el element to be freed
 */
void element_free( Element *el );

/** Computes lengths of element's edges
 *
 * \param[in] element element
 * \param[out] length1 length of the first edge
 * \param[out] length2 length of the second edge
 * \param[out] length3 length of the third edge
 */
void element_edge_lengths( const Element *element,
        gdouble *length1, gdouble *length2, gdouble *length3 );

/** Computes the length of the shortest edge of an element and returns a pointer
 * to it.
 *
 * \param[in] el element
 * \param[out] length length of the shortest edge. If NULL, only the pointer to
 * the shortest half-edge is returned
 *
 * \return pointer to the half-edge of the shortest edge
 *
 * \sa element_min_edge_length
 */
HalfEdge * element_min_edge( const Element *el, gdouble *length );

/** Computes the length of the shortest edge in an element. If the pointer to
 * the shortest edge is not needed, this function should be slightly faster than
 * element_min_edge.
 *
 * \param[in] element element
 * 
 * \return length of the shortest edge
 *
 * \sa element_min_edge
 */
gdouble element_min_edge_length( const Element *element );

/** Computes the length of the longest edge of an element and returns a pointer
 * to it.
 *
 * \param[in] el element
 * \param[out] length length of the longest edge. If NULL, only the pointer to
 * the longest half-edge is returned
 *
 * \return pointer to the half-edge of the longest edge
 *
 * \sa element_max_edge_length
 */
HalfEdge * element_max_edge( const Element *el, gdouble *length );

/** Computes the length of the longest edge in an element. If the pointer to
 * the longest edge is not needed, this function should be slightly faster than
 * element_max_edge.
 *
 * \param[in] element element
 * 
 * \return length of the longest edge
 *
 * \sa element_max_edge
 */
gdouble element_max_edge_length( const Element *element );

/** Computes inner angles in a triangle that is given by its vertices.
 *
 * \param[in] p1 first triangle vertex
 * \param[in] p2 second triangle vertex
 * \param[in] p3 third triangle vertex
 * \param[out] angle1 angle at p1
 * \param[out] angle2 angle at p2
 * \param[out] angle3 angle at p3
 *
 * \sa element_angles
 */
void triangle_angles( const Point2 *p1, const Point2 *p2, const Point2 *p3,
        gdouble *angle1, gdouble *angle2, gdouble *angle3 );

/** Computes inner angles in a triangle that is given by the pointer to the
 * corresponding element.
 *
 * \param[in] element element
 * \param[out] angle1 angle at first vertex
 * \param[out] angle2 angle at second vertex
 * \param[out] angle3 angle at third vertex
 *
 * \sa triangle_angles
 */
void element_angles( const Element *element,
        gdouble *angle1, gdouble *angle2, gdouble *angle3 );

/** Computes minimum angle in an element.
 *
 * \param element element
 *
 * \return minimum angle
 */
gdouble element_minimum_angle( const Element *element );

/** Computes maximum angle in an element.
 *
 * \param element element
 *
 * \return maximum angle
 */
gdouble element_maximum_angle( const Element *element );

/** Computes the area of an element
 *
 * \param element element
 *
 * \return element's area
 */
gdouble element_area( const Element *element );

/** Computes the radius of triangle's circumcircle from the length of its
 * edges.
 *
 * \param e1 length of the first edge
 * \param e2 length of the first edge
 * \param e3 length of the first edge
 *
 * \return radius of the circumcircle
 */
gdouble triangle_circumcircle_radius_edge_length( gdouble e1, gdouble e2, gdouble e3 );

/** Computes the radius of the circumcircle of a triangle given by its vertices.
 *
 * \param p1 first vertex
 * \param p2 second vertex
 * \param p3 third vertex
 *
 * \return radius of the circumcircle
 *
 * \sa element_circumcircle_radius
 * \sa triangle_circumcircle_radius_edge_length
 */
gdouble triangle_circumcircle_radius( Point2 *p1, Point2 *p2, Point2 *p3 );

/** Computes the radius of the circumcircle of an element.
 *
 * \param el element
 *
 * \return radius of the element's circumcircle
 *
 * \sa triangle_circumcircle_radius
 * \sa triangle_circumcircle_radius_edge_length
 */
gdouble element_circumcircle_radius( const Element *el );

/** Computes the coordinates of triangle's circumcircle from the position of
 * triangle's vertices.
 *
 * \param[in] p1 first vertex
 * \param[in] p2 second vertex
 * \param[in] p3 third vertex
 * \param[out] center center of the circumcircle.
 *
 * \sa element_circumcenter_coordinates
 */
void triangle_circumcenter_coordinates(
        const Point2 *p1, const Point2 *p2, const Point2 *p3,
        Point2 *center );

/** Computes the coordinates of element's circumcircle.
 *
 * \param[in] el element
 * \param[out] center center of the circumcircle
 *
 * \sa triangle_circumcenter_coordinates
 */
void element_circumcenter_coordinates( const Element *el, Point2 *center );


#endif /* __ELEMENT_H_INCLUDED__ */

