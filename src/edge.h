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

#ifndef __EDGE_H_INCLUDED__
#define __EDGE_H_INCLUDED__ 

/** \file
 * \brief Edge and half-edge related functions
 *
 * \author Vladimir Chalupecky
 */

#include "mesh_types.h"

/** Allocates a new empty Edge data structure.
 *
 * \return newly allocated edge data structure, or NULL if the memory allocation
 * failed
 */
Edge * edge_new( void );

/** Sets the end nodes of an edge.
 *
 * \param edge edge
 * \param n1 first node
 * \param n2 second node
 */
void edge_set_nodes( Edge *edge, Node *n1, Node *n2 );

/** Frees an edge from memory
 *
 * \param edge edge */
void edge_free( Edge *edge );

/** Prints formatted information about an edge and its half-edges
 *
 * \param edge edge
 */
void edge_print( const Edge *edge );

/** Prints formatted information about a half-edge
 *
 * \param he half-edge
 */
void halfedge_print( const HalfEdge *he );

/** Returns the length of an edge by computing the Euclidean distance between
 * its two end nodes.
 *
 * \param edge edge
 *
 * \return length of the edge
 */
gdouble edge_length( const Edge *edge );

/** Indicates if the given half-edge is at a boundary.
 *
 * \param he half-edge
 *
 * \return TRUE if the half-edge is at a boundary, otherwise FALSE
 */
gboolean halfedge_is_at_boundary( const HalfEdge *he );

/** Indicates if the given edge is at the boundary (i.e., one of its half-edges
 * is at the boundary).
 *
 * \param edge edge
 *
 * \return TRUE if the edge is at a boundary, otherwise FALSE
 */
gboolean edge_is_at_boundary( const Edge *edge );

/** Tests if:
 *   - the edge is not at the boundary (i.e., both of its half-edges have an
 *     adjacent triangle)
 *   - the quadrilateral formed by the two adjacent triangles is convex
 *
 * If these two conditions are fulfilled, the edge can be swapped.
 * 
 * \param edge edge
 *
 * \return TRUE if the edge can be swapped, otherwise FALSE
 */
gboolean edge_is_swappable( const Edge *edge );

/** Tests if the edge is Delaunay, i.e., if it satisfies the local Delaunay
 * condition.
 *
 * \attention No tests are performed to check, for example, if the edge is not
 * at the boundary.
 *
 * \param edge edge to be tested
 *
 * \return TRUE if the edge is Delaunay, otherwise FALSE
 */
gboolean edge_is_delaunay( const Edge *edge );

/** Checks half-edge for encroachment by a point. Main use of this routine is in
 * the Ruppert's algorithm.
 *
 * \param he half-edge to be checked
 * \param p point
 * 
 * \return TRUE if p encroaches upon he, otherwise FALSE
 */
gboolean halfedge_is_encroached_upon_by_point( const HalfEdge *he, const Point2 *p );

/** Checks if a point lies in the left half-plane of a half-edge (direction
 * taken in the sense from the origin to the destination node).
 *
 * \param he half-edge
 * \param p point
 *
 * \return TRUE if point lies in left half-plane, otherwise FALSE
 */
gboolean halfedge_point_is_in_left_half_plane( const HalfEdge *he, const Point2 *p );

#endif /* __EDGE_H_INCLUDED__ */
