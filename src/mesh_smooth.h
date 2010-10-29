#ifndef __MESH_SMOOTH_H_INCLUDED__
#define __MESH_SMOOTH_H_INCLUDED__ 

#include "mesh_types.h"

/** Various methods for mesh smoothing */
enum MeshSmoothMethod
{
    /** Simple Laplacian smoothing. */
    LAPLACIAN_SMOOTHING,
    /** Method that smoothes the quality of triangles. Described in the book by
     * Frey et al. Seems to perform very well and gives nice meshes*/
    ELEMENT_QUALITY_SMOOTHING,
    /** Method that smoothes the edge lengths. Described in the book by Frey et
     * al. Does not seem to be so good. Or there is a bug in the code. */ 
    EDGE_LENGTH_SMOOTHING,
    /** Smoothing by the function described in Bossen, Heckbert (1996). Not
     * useable for improvement of created meshes. Perhaps might be used for
     * smoothing during refinement stage. */
    BOSSEN_HECKBERT_SMOOTHING 
};


/** Smoothes a mesh by changing the position of nodes by an averaging process.
 * The weights are chosen according to a different criteria that depend on each
 * particular method.
 *
 * \param mesh mesh to be smoothed
 * \param method particular method to be used for smoothing
 * \param niter number of times the smoothing method is applied
 * \param area desired area of one triangle
 */
void mesh_smooth( Mesh *mesh, enum MeshSmoothMethod method, guint niter, gdouble area );

#endif /* __MESH_SMOOTH_H_INCLUDED__ */

