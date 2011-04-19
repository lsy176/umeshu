//
//  Point2.h
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/24/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#ifndef __POINT_H_INCLUDED__
#define __POINT_H_INCLUDED__

#include <boost/math/constants/constants.hpp>

#include <iosfwd>

class Point2
{
public:
    Point2() { coord_[0] = 0.0; coord_[1] = 0.0; }
    Point2( double x, double y ) { coord_[0] = x; coord_[1] = y; }
    
    double x() const { return coord_[0]; }
    double& x() { return coord_[0]; }
    double y() const { return coord_[1]; }
    double& y() { return coord_[1]; }
    
    const double* coord() const { return &coord_[0]; }

    friend bool operator== (Point2 const& p1, Point2 const& p2);

private:
    double coord_[2];

    friend std::ostream& operator<<(std::ostream& os, Point2 const& p);
};

inline bool operator== (Point2 const& p1, Point2 const& p2 )
{
    return p1.x() == p2.x() && p1.y() == p2.y();
}

inline double distance_squared (Point2 const& p1, Point2 const& p2)
{
    double x = p1.x() - p2.x();
    double y = p1.y() - p2.y();
    return x*x + y*y;
}

inline double distance (Point2 const& p1, Point2 const& p2)
{
    return std::sqrt(distance_squared(p1, p2));
}

inline double degrees_to_radians(double angle)
{
    return angle*boost::math::constants::pi<double>()/180.0;
}

inline double radians_to_degrees(double angle)
{
    return angle/boost::math::constants::pi<double>()*180.0;
}

inline Point2 midpoint (Point2 const& p1, Point2 const& p2)
{
    return Point2(0.5*(p1.x()+p2.x()), 0.5*(p1.y()+p2.y()));
}

double circumradius (Point2 const& p1, Point2 const& p2, Point2 const& p3);
void triangle_angles (Point2 const& p1, Point2 const& p2, Point2 const& p3, double& a1, double& a2, double& a3);
double triangle_area (Point2 const& p1, Point2 const& p2, Point2 const& p3);
Point2 barycenter (Point2 const& p1, Point2 const& p2, Point2 const& p3);
Point2 midpoint (Point2 const& p1, Point2 const& p2);

#endif // __POINT_H_INCLUDED__