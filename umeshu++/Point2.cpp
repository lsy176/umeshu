//
//  Point2.cpp
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/24/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#include "Point2.h"

#include <cmath>
#include <limits>
#include <ostream>

std::ostream& operator<<(std::ostream& os, Point2 const& p)
{
    os.precision(std::numeric_limits<double>::digits10);
    os << "[" << std::fixed << p.x() << "," << p.y() << "]";
    return os;
}

double circumradius (Point2 const& p1, Point2 const& p2, Point2 const& p3)
{
    double a = distance(p1, p2);
    double b = distance(p2, p3);
    double c = distance(p3, p1);
    double s = 0.5*(a+b+c);
    return 0.25*a*b*c/(std::sqrt(s*(s-a)*(s-b)*(s-c)));
}

void triangle_angles (Point2 const& p1, Point2 const& p2, Point2 const& p3, double& a1, double& a2, double& a3)
{
    double a, b, c;
    a = distance(p2, p3);
    b = distance(p1, p3);
    c = distance(p1, p2);
    double aa = a*a;
    double bb = b*b;
    double cc = c*c;
    
    a1 = acos((bb+cc-aa)/(2.0*b*c));
    a2 = acos((aa+cc-bb)/(2.0*a*c));
    a3 = acos((aa+bb-cc)/(2.0*a*b));
}

double triangle_area (Point2 const& p1, Point2 const& p2, Point2 const& p3)
{
    double l1 = distance(p1, p2);
    double l2 = distance(p2, p3);
    double l3 = distance(p3, p1);
    double s = 0.5*(l1 + l2 + l3);
    return std::sqrt(s*(s - l1)*(s - l2)*(s - l3));
}

Point2 barycenter (Point2 const& p1, Point2 const& p2, Point2 const& p3)
{
    const double one_third = boost::math::constants::third<double>();
    return Point2(one_third*(p1.x()+p2.x()+p3.x()),one_third*(p1.y()+p2.y()+p3.y()));
}

