#ifndef __POLYGON_TRIANGULATE_H_INCLUDED__
#define __POLYGON_TRIANGULATE_H_INCLUDED__ 

#include "mesh_types.h"
#include "polygon.h"

/** Generates a mesh by triangulating the input polygon. The polygon has to be
 * simple and the vertices have to be oriented in a counter-clockwise fashion.
 * The triangulation is done by ear clipping, which has an O(n^2) complexity.
 * The resulting mesh (triangulation) serves as an initial triangulation that
 * will be further processed (made CDT) and refined (by adding Steiner points).
 * 
 * \attention This routine is not completely robust and cannot deal with all
 * possible inputs (for example, a degenerate polygon with a vertex lying in an
 * edge, see test_polygon.c for an example)
 *
 * \param poly input polygon
 *
 * \return triangulation of the input polygon
 */
Mesh * mesh_triangulate_polygon( const Polygon *poly );

#endif /* __POLYGON_TRIANGULATE_H_INCLUDED__ */
