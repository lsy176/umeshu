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

#ifndef __MESH_DELAUNAY_H_INCLUDED__
#define __MESH_DELAUNAY_H_INCLUDED__ 

#include "mesh_types.h"

/** Restores the Delaunay condition by a recursive algorithm. After a call to
 * \ref mesh_split_element the three newly created edges are Delaunay, but the
 * three edges of the original element have to be checked. For details, see
 * Øyvind Hjelle, Morten Dæhlen: Triangulations and Applications (Mathematics
 * and Visualization). With some caution, this function can be called also in
 * other situations to restore the Delaunay condition efficiently.
 *
 * \param mesh mesh
 * \param he outgoing half-edge from the newly inserted node that is suspicios
 * of not being Delaunay after splitting an element. 
 *
 * \sa mesh_refine_chew
 */
void recursive_swap_delaunay( Mesh *mesh, HalfEdge *he );

/** Modifies the mesh by edge flipping so that it forms the constrained Delaunay
 * triangulation. The function maintains a list of candidate edges for flipping.
 * If the edge is Delaunay, the edge is removed from the list. Otherwise, it is
 * flipped and the four surrounding edges, whose Delaunay condition might have
 * been affected, are added to the list (if they are not already there). This is
 * repeated until the list is empty.
 *
 * \param mesh mesh
 *
 * \sa edge_is_swappable
 * \sa edge_is_delaunay
 */
void mesh_make_cdt_by_edge_flipping( Mesh *mesh );

/** Splits boundary edges of a mesh that are encroached upon. This routine is
 * mainly used in the initialization step of Ruppert's algorithm.
 *
 * \attention The mesh has to be CDT (obtained, e.g., from a call to
 * mesh_make_cdt_by_edge_flipping).
 *
 * \param mesh mesh
 */
void mesh_split_encroached_boundary_edges( Mesh *mesh );

#endif /* __MESH_DELAUNAY_H_INCLUDED__ */

