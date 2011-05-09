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

#include "Point2.h"

#include <cmath>
#include <limits>
#include <ostream>

namespace umeshu {

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
    
    a1 = std::acos((bb+cc-aa)/(2.0*b*c));
    a2 = std::acos((aa+cc-bb)/(2.0*a*c));
    a3 = std::acos((aa+bb-cc)/(2.0*a*b));
}

Point2 barycenter (Point2 const& p1, Point2 const& p2, Point2 const& p3)
{
    const double one_third = boost::math::constants::third<double>();
    return Point2(one_third*(p1.x()+p2.x()+p3.x()),one_third*(p1.y()+p2.y()+p3.y()));
}

} // namespace umeshu
