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

#ifndef __MESH_H_INCLUDED__
#define __MESH_H_INCLUDED__ 

/** \file
 * \brief Routinges for mesh creation and manipulation
 *
 * \author Vladimir Chalupecky
 */

#include "mesh_types.h"

/** Allocates an empty mesh
 *
 * \return pointer to mesh data structure, NULL if allocation failed
 */
Mesh * mesh_new( void );

/** Frees a mesh from memory
 *
 * \param mesh mesh
 */
void mesh_free( Mesh *mesh );

/** Adds a node at given coordinates to the mesh.
 *
 * \param mesh mesh
 * \param x x coordinate of the node
 * \param y y coordinate of the node
 *
 * \return pointer to the newly create node, NULL if allocation failed
 */
Node * mesh_add_node( Mesh *mesh, gdouble x, gdouble y );

/** Removes a node from the mesh and frees it from memory. If the node is not
 * isolated (is connected to at least one edge), all affected edges and
 * elements are removed from the mesh as well. This can leave a hole in the
 * mesh.
 *
 * \param mesh mesh
 * \param node node to be removed from the mesh
 */
void mesh_remove_node( Mesh *mesh, Node *node );

/** Adds an edge into the mesh that connects two given nodes from the mesh. The
 * nodes have to be distinct, creation of loop edges is not allowed. There has
 * to be a free incident half-edge at each of the two nodes for the edge to be
 * added with success.
 *
 * \attention For efficiency reasons the function does not check if the added
 * edge is already present in the mesh
 *
 * \param mesh mesh
 * \param node1 first node
 * \param node2 second node
 *
 * \return pointer to the newly created edge, NULL if there was an error (no
 * memory, loop edge, no free incident half-edge)
 */
Edge * mesh_add_edge( Mesh *mesh, Node *node1, Node *node2 );

/** Removes an edge from the mesh. At the same time, it removes the two
 * triangles on each side of the edge (if they exist).
 *
 * \param mesh mesh
 * \param edge edge to be removed
 */
void mesh_remove_edge( Mesh *mesh, Edge *edge );

/** Adds an element into the mesh. The three half-edges have to be free (no
 * adjacent polygon) and have to form a chain (a target node of one half-edge is an
 * origin node of the next half-edge). The function reorders the next/previous
 * connections of the three half-edges so that they form a circular list around
 * the polygon. If such a reordering is not possible, the resulting mesh would
 * be non-manifold and the element is not added.
 *
 * \param mesh mesh
 * \param e1 first half-edge
 * \param e2 second half-edge
 * \param e3 third half-edge
 *
 * \return pointer to the newly created element, NULL if the element cannot be
 * created
 */
Element * mesh_add_element( Mesh *mesh, HalfEdge *e1, HalfEdge *e2, HalfEdge *e3 );

/** Removes an element from the mesh and frees it from memory.
 *
 * \param mesh mesh
 * \param element element to be removed
 */
void mesh_remove_element( Mesh *mesh, Element *element );

#endif /* __MESH_H_INCLUDED__ */

