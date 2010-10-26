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

#ifndef __MESH_TYPES_H_INCLUDED__
#define __MESH_TYPES_H_INCLUDED__ 

/** \file
 * \brief Definition of mesh-related data types
 *
 * \author Vladimir Chalupecky
 */

#include <glib.h>

/** Small positive number used for some epsilon comparisons */
#define SMALL_NUMBER (10*DBL_EPSILON)

/** Square-root of 3 */
#define SQRT3 1.7320508075688772

typedef struct Point2_tag Point2;
typedef struct Node_tag Node;
typedef struct HalfEdge_tag HalfEdge;
typedef struct Edge_tag Edge;
typedef struct Element_tag Element;
typedef struct Mesh_tag Mesh;

/** 2D point data structure */
struct Point2_tag
{
    /** x-coordinate */
    gdouble x;
    /** y-coordinate */
    gdouble y;
};

/** Convenience macro to cast a void pointer to a pointer to Point2 */
#define POINT2(p) ((Point2*)(p))

/** Node data structure */
struct Node_tag
{
    /** Position of the node */
    Point2 position;

    /** One of the half-edges that emanate from the node */
    HalfEdge *out_halfedge;
};

/** Convenience macro to cast a void pointer to a pointer to Node */
#define NODE(p) ((Node*)(p))

/** Convenience macro to get the pointer to Point2 position of a node given the
 * pointer to the node. Since position is the first member in the Node struct,
 * we can just cast the pointer to Point2* */
#define NODE_POSITION(n) (POINT2(n))

/** Half-edge data structure */
struct HalfEdge_tag
{
    /** Oppositely-oriented adjacent, pair half-edge */
    HalfEdge *pair;

    /** Parent edge to which this half-edge belongs. Needed when we have an
     * half-edge (that has an orientation) and need to call a function that
     * accepts edges (that have no orientation). */
    Edge *edge;

    /** Node at the beginning of the half-edge */
    Node *origin;

    /** Element at whose boundary the half-edge lies. The half-edges are
     * oriented so that the elements lie to the left when looking in the
     * direction of the half-edge. */
    Element *element;

    /** Next half-edge around the element */
    HalfEdge *next;

    /** Previous half-edge around the element */
    HalfEdge *previous;
};

/** Convenience macro to cast a void pointer to a pointer to HalfEdge */
#define HALFEDGE(p) ((HalfEdge*)(p))

/** Edge data structure */
struct Edge_tag
{
    /** The two half-edges that form the edge */
    HalfEdge he[2];
};

/** Convenience macro to cast a void pointer to a pointer to Edge */
#define EDGE(p) ((Edge*)(p))

/** Element data structure. */
struct Element_tag
{
    /** For the definition of an element it is enough to specify just one
     * half-edge that lies at its boundary */
    HalfEdge *adjacent_halfedge;
};

/** Convenience macro to cast a void pointer to a pointer to Element */
#define ELEMENT(p) ((Element*)(p))

/** Mesh data structure */
struct Mesh_tag
{
    /** Number of nodes in the mesh */
    guint Np;
    /** Number of edges in the mesh */
    guint Ne;
    /** Number of triangles in the mesh */
    guint Nt;

    /** List of all nodes. If empty, it is equal to NULL. */
    GList *nodes;

    /** List of all edges. If empty, it is equal to NULL. */
    GList *edges;

    /** List of all elements. If empty, it is equal to NULL. */
    GList *elements;

    /** \privatesection */
    GHashTable *hash;
};

#endif /* __MESH_TYPES_H_INCLUDED__ */
