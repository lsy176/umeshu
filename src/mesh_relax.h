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

#ifndef __MESH_RELAX_H_INCLUDED__
#define __MESH_RELAX_H_INCLUDED__ 

#include <mesh_types.h>

/** Relaxes the mesh by swapping and/or collapsing edges in order to improve
 * degrees of nodes and thus to improve subsequent mesh smoothing. The function
 * implements the algorithm published in William H. Frey, David A. Field, Mesh
 * relaxation: A new technique for improving triangulations, International
 * Journal for Numerical Methods in Engineering 31(6) (1991), pp. 1121-1133,
 * DOI: 10.1002/nme.1620310607.
 *
 * It seems to be a good idea to generate a mesh, relax it using this function
 * and then smooth it.
 *
 * \param mesh mesh to be relaxed
 *
 * \sa mesh_smooth
 */
void mesh_relax( Mesh *mesh );

#endif /* __MESH_RELAX_H_INCLUDED__ */

