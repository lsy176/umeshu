//
//  Copyright (c) 2011 Vladimir Chalupecky
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  IN THE SOFTWARE.

#include "BoundarySegment.h"
#include "BoundingBox.h"
#include "ExactAdaptiveKernel.h"
#include "Node.h"
#include "Edge.h"
#include "Mesh.h"
#include "Mesher.h"
#include "Postscript_ostream.h"

typedef Mesh<ExactAdaptiveKernel> mesh;

int main (int argc, const char * argv[])
{
    mesh m;
    
    double s = sin(degrees_to_radians(10));
    double c = cos(degrees_to_radians(10));
    
    NodeHandle n1 = m.add_node(Point2(0.0,0.0));
//    NodeHandle n2 = m.add_node(Point2(10*c,10*s));
//    NodeHandle n3 = m.add_node(Point2(c-s,c+s));
//    NodeHandle n4 = m.add_node(Point2(-s,c));
    NodeHandle n2 = m.add_node(Point2(5.3,0.0));
    NodeHandle n3 = m.add_node(Point2(1.0,1.0));
    NodeHandle n4 = m.add_node(Point2(0.0,1.0));
    EdgeHandle e1 = m.add_edge(n1, n2, NULL);
    EdgeHandle e2 = m.add_edge(n2, n3, NULL);
    EdgeHandle e3 = m.add_edge(n3, n4, NULL);
    EdgeHandle e4 = m.add_edge(n4, n1, NULL);
    EdgeHandle e5 = m.add_edge(n3, n1, NULL);
    m.add_face(e1->he1(), e2->he1(), e5->he1());
    m.add_face(e3->he1(), e4->he1(), e5->he2());
    
    Postscript_stream ps1("mesh_1.eps", m.bounding_box());
    ps1 << m;

    Mesher<mesh> mesher(m);
    mesher.make_cdt();
    Postscript_stream ps2("mesh_2.eps", m.bounding_box());
    ps2 << m;
    
    std::cout << m.bounding_box();
    std::cout << *e1->he1();

    mesher.refine(0.001, 20);
    Postscript_stream ps3("mesh_3.eps", m.bounding_box());
    ps3 << m;
    std::cout << m.smallest_angle() << std::endl;

    return 0;
}


//std::cout << "Number of faces: " << m.number_of_faces() << std::endl;
//Postscript_stream ps1("mesh_1.eps", m.bounding_box());
//ps1 << m;
//
//std::cout << "Delaunay edge e5: " << e5->is_delaunay<FastPredicates>() << std::endl;
//e5 = m.swap_edge(e5);
//std::cout << "Delaunay edge e5: " << e5->is_delaunay<FastPredicates>() << std::endl;
//
//NodeHandle n5 = m.split_edge(e5);
//std::cout << "Number of faces after split: " << m.number_of_faces() << std::endl;
//Postscript_stream ps2("mesh_2.eps", m.bounding_box());
//ps2 << m;
//std::cout << "Edge n5->out is swappable: " << n5->out_he()->edge()->is_swappable<ExactAdaptivePredicates>() << std::endl;
//
//Mesh::Point2Location loc;
//void *ent;
//locate_point<ExactAdaptivePredicates>(Point2(0.0,0.2), n5->out_he()->face(), loc, ent);
//std::cout << Mesh::IN_FACE << "  "
//<< Mesh::ON_EDGE << "  " 
//<< Mesh::ON_NODE << " loc: "
//<< loc << std::endl;
//
//FaceHandle f = n5->out_he()->face();
//NodeHandle n6 = m.split_face(f, f->barycenter());
//std::cout << "Number of faces after split_face: " << m.number_of_faces() << std::endl;
//Postscript_stream ps3("mesh_3.eps", m.bounding_box());
//ps3 << m;
//
//EdgeHandle e6;
//EdgeHandle e7;
//if (n6->out_he()->next()->pair()->face() != NULL) {
//    e7 = n6->out_he()->next()->edge();
//} else {
//    if (n6->out_he()->prev()->pair()->next()->pair()->face() != NULL) {
//        e7 = n6->out_he()->prev()->pair()->next()->edge();
//    } else {
//        e7 = n6->out_he()->pair()->prev()->edge();
//    }
//}
//std::cout << "e7 is swappable: " << e7->is_swappable<ExactAdaptivePredicates>() << std::endl;
//std::cout << "e7 is delaunay: " << e7->is_delaunay<ExactAdaptivePredicates>() << std::endl;
//e6 = m.swap_edge(e7);
//std::cout << "Number of faces after swap_edge: " << m.number_of_faces() << std::endl;
//std::cout << "e6 is swappable: " << e6->is_swappable<ExactAdaptivePredicates>() << std::endl;
//std::cout << "e6 is delaunay: " << e6->is_delaunay<ExactAdaptivePredicates>() << std::endl;
//Postscript_stream ps4("mesh_4.eps", m.bounding_box());
//ps4 << m;
