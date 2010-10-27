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

#ifndef __MESH_OUTPUT_H_INCLUDED__
#define __MESH_OUTPUT_H_INCLUDED__ 

/** \file
 * \brief Functions related to the output of the mesh (plotting, printing,
 * saving)
 *
 * \author Vladimir Chalupecky
 */

#include "mesh_types.h"

/** Save the mesh as an Encapsulated PostScript image.
 *
 *  \param filename name of the file
 *  \param mesh mesh
 */
void mesh_save_to_eps( const gchar *filename, const Mesh *mesh );

/** Save the mesh in the PLY file format.
 *
 *  \param filename name of the file
 *  \param mesh mesh
 */
void mesh_save_to_ply( const gchar *filename, const Mesh *mesh );

/** Save the mesh in the POLY file format.
 *
 *  \param filename name of the file
 *  \param mesh mesh
 */
void mesh_save_to_poly( const gchar *filename, const Mesh *mesh );

/** Save the mesh in the OBJ file format.
 *
 *  \param filename name of the file
 *  \param mesh mesh
 */
void mesh_save_to_obj( const gchar *filename, const Mesh *mesh );

#endif /* __MESH_OUTPUT_H_INCLUDED__ */
