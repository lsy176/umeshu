#ifndef __POLYGON_H_INCLUDED__
#define __POLYGON_H_INCLUDED__ 

#include "BoundingBox.h"
#include "Point2.h"

#include <vector>

class Polygon
{
public:
    typedef std::vector<Point2>::iterator vertex_iterator;
    typedef std::vector<Point2>::const_iterator const_vertex_iterator;

    void append_vertex(Point2 vertex) { vertices_.push_back(vertex); }
    size_t number_of_vertices() const { return vertices_.size(); }

    BoundingBox bounding_box() const;

    vertex_iterator vertices_begin() { return vertices_.begin(); }
    const_vertex_iterator vertices_begin() const { return vertices_.begin(); }
    vertex_iterator vertices_end() { return vertices_.end(); }
    const_vertex_iterator vertices_end() const { return vertices_.end(); }

    static Polygon island();
    static Polygon letter_a();
    static Polygon letter_u();
    static Polygon square(double size);
    static Polygon kidney();
    static Polygon crack();
    static Polygon coastline();

private:
    std::vector<Point2> vertices_;
};

#endif /* __POLYGON_H_INCLUDED__ */
