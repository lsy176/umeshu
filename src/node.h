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

#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__ 

/** \file
 * \brief Node-related functions and macros
 *
 * \author Vladimir Chalupecky
 */

#include "mesh_types.h"

/** Allocates and initializes a new, isolated node.
 *
 * \param x x-coordinate
 * \param y y-coordinate
 *
 * \return the new node
 */
Node * node_new( gdouble x, gdouble y );

/** Frees the node from memory.
 *
 * \param node node to be freed
 */
void node_free( Node *node );

/** Prints information about the node.
 *
 * \param node node
 */
void node_print( const Node *node );

/** Returns a boolean indicating if the node is isolated (has no outgoing
 * half-edge).
 *
 * \param node node
 *
 * \return TRUE if the node is isolated (no outgoing half-edge), otherwise
 * returns FALSE
 */
gboolean node_is_isolated( const Node *node );

/** Computes if the node lies at a boundary.
 *
 * \param node node
 *
 * \return If the node lies at a boundary, the function returns a pointer to the
 * outgoing boundary half-edge. Otherwise (inside a mesh or isolated) it returns
 * NULL. 
 */
HalfEdge * node_is_at_boundary( const Node *node );

/** Returns the degree (number of incident edges) of the node.
 *
 * \param node node
 *
 * \return number of incident edges
 */
guint node_degree( const Node *node );

/** Computes the "virtual" degree of a node. This is computed based on node's
 * position (inner or boundary node) and configuration of the surrounding
 * boundary edges (in case it is a boundary node). Its principal use is in mesh
 * relaxation algorithm according to William H. Frey, David A. Field, Mesh
 * relaxation: A new technique for improving triangulations, International
 * Journal for Numerical Methods in Engineering 31(6) (1991), pp. 1121-1133,
 * DOI: 10.1002/nme.1620310607.
 *
 * \param node node
 *
 * \return Virtual degree of the node
 */
guint node_virtual_degree( const Node *node );

/** Computes the ideal degree of a node used in the computation of the virtual
 * degree.
 *
 * \param node node
 *
 * \return Virtual degree of the node
 */
guint node_ideal_degree( const Node *node );

#endif /* __NODE_H_INCLUDED__ */

