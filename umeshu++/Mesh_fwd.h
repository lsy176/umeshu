//
//  Mesh_fwd.h
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/30/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#ifndef __MESH_FWD_H_INCLUDED__
#define __MESH_FWD_H_INCLUDED__

class Point2;

class Node;
typedef Node* NodeHandle;

class HalfEdge;
typedef HalfEdge* HalfEdgeHandle;
class Edge;
typedef Edge* EdgeHandle;

class Face;
typedef Face* FaceHandle;

typedef enum {ON_NEGATIVE_SIDE, ON_ORIENTED_BOUNDARY, ON_POSITIVE_SIDE} Oriented_side;

#endif // __MESH_FWD_H_INCLUDED__