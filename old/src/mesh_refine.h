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

#ifndef __MESH_REFINE_H_INCLUDED__
#define __MESH_REFINE_H_INCLUDED__ 

#include <mesh_types.h>

typedef enum { RUPPERT_REFINEMENT, CHEW_REFINEMENT } RefinementAlgorithm;

/** Refines a constrained Delaunay triangulation by a given algorithm.
 *
 * \attention The input mesh already has to be a constrained Delaunay
 * triangulation.
 *
 * \todo Ruppert's refinement implementaton is not very robust yet. If we set
 * max_element_are to 0.0004 or less, the function segfaults.
 *
 * \todo Implement Chew's refinement algorithm
 *
 * \param mesh mesh to be refined
 * \param alg refinement algorithm
 * \param max_element_area if an element has the area larger than this value, it
 * is considered large and is split
 * \param min_angle minimum acceptable angle in an element for it not to be
 * considered skinny
 */
void mesh_refine( Mesh * mesh, RefinementAlgorithm alg, gdouble max_element_area, gdouble min_angle );


#endif /* __MESH_REFINE_H_INCLUDED__ */
