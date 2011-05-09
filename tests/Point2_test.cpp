#define BOOST_TEST_MODULE Point2
#include <boost/test/unit_test.hpp>
#include <cmath>

#include "Point2.h"

using namespace umeshu;

BOOST_AUTO_TEST_CASE(construction_and_access)
{
    Point2 p1, p2(0.0, 0.0), p3(1.0, 2.0), p4(-10.0, -30.0);

    BOOST_CHECK(p1.x() == 0.0);
    BOOST_CHECK(p1.y() == 0.0);
    BOOST_CHECK(p2.x() == 0.0);
    BOOST_CHECK(p2.y() == 0.0);
    BOOST_CHECK(p3.x() == 1.0);
    BOOST_CHECK(p3.y() == 2.0);
    BOOST_CHECK(p4.x() == -10.0);
    BOOST_CHECK(p4.y() == -30.0);
}

BOOST_AUTO_TEST_CASE(operators)
{
    Point2 p1, p2(0.0, 0.0), p3(1.0, 2.0), p4(-10.0, -30.0);

    BOOST_CHECK_EQUAL(p1, p2);
    BOOST_CHECK(p3 != p4);

    BOOST_CHECK_EQUAL(p2 + p2, p2);
    BOOST_CHECK_EQUAL(p2 + p3, p3);
    BOOST_CHECK_EQUAL(p2 + p4, p4);
    BOOST_CHECK_EQUAL(p3 + p4, Point2(-9.0, -28.0));

    BOOST_CHECK_EQUAL(p2 - p2, p2);
    BOOST_CHECK_EQUAL(p2 - p3, Point2(-1.0, -2.0));
    BOOST_CHECK_EQUAL(p2 - p4, Point2(10.0, 30.0));
    BOOST_CHECK_EQUAL(p3 - p4, Point2(11.0, 32.0));

    const double c = std::sqrt(2.0);
    BOOST_CHECK_EQUAL(c*p1, p1);
    BOOST_CHECK_EQUAL(c*p3, Point2(c, 2.0*c));

    BOOST_CHECK_EQUAL(p1/c, p1);
    BOOST_CHECK_EQUAL(p3/c, Point2(1.0/c, 2.0/c));
}

BOOST_AUTO_TEST_CASE(distances)
{
    Point2 p1(0.0, 0.0), p2(1.0, -2.0);

    BOOST_CHECK_EQUAL(distance_squared(p1, p1), 0.0);
    BOOST_CHECK_EQUAL(distance_squared(p2, p2), 0.0);
    BOOST_CHECK_EQUAL(distance_squared(p1, p2), 5.0);

    BOOST_CHECK_EQUAL(distance(p1, p1), 0.0);
    BOOST_CHECK_EQUAL(distance(p2, p2), 0.0);
    BOOST_CHECK_CLOSE(distance(p1, p2), std::sqrt(5.0), 1.0e-13);
}

BOOST_AUTO_TEST_CASE(degrees_and_radians)
{
    BOOST_CHECK_CLOSE(degrees_to_radians(360), 2.0*boost::math::constants::pi<double>(), 1e-13);
    BOOST_CHECK_CLOSE(degrees_to_radians(180), boost::math::constants::pi<double>(), 1e-13);
    BOOST_CHECK_CLOSE(degrees_to_radians(90), boost::math::constants::pi<double>()/2.0, 1e-13);
    BOOST_CHECK_CLOSE(degrees_to_radians(45), boost::math::constants::pi<double>()/4.0, 1e-13);
    BOOST_CHECK_CLOSE(radians_to_degrees(boost::math::constants::pi<double>()/4.0), 45.0, 1e-13);
    BOOST_CHECK_CLOSE(radians_to_degrees(boost::math::constants::pi<double>()/2.0), 90.0, 1e-13);
    BOOST_CHECK_CLOSE(radians_to_degrees(boost::math::constants::pi<double>()), 180.0, 1e-13);
    BOOST_CHECK_CLOSE(radians_to_degrees(2.0*boost::math::constants::pi<double>()), 360.0, 1e-13);
}

BOOST_AUTO_TEST_CASE(utilities)
{
    double a1, a2, a3;

    Point2 p1, p2(1,0), p3(0,1);
    Point2 c(0.5, 0.5);
    BOOST_CHECK_CLOSE(circumradius(p1, p2, p3), distance(p1, c), 1e-13);
    triangle_angles(p1, p2, p3, a1, a2, a3);
    BOOST_CHECK_CLOSE(a1, boost::math::constants::pi<double>()/2.0, 1e-13);
    BOOST_CHECK_CLOSE(a2, boost::math::constants::pi<double>()/4.0, 1e-13);
    BOOST_CHECK_CLOSE(a3, boost::math::constants::pi<double>()/4.0, 1e-13);

    c = Point2(0.5,0.5*std::tan(degrees_to_radians(30)));
    p3 = Point2(std::cos(degrees_to_radians(60)), std::sin(degrees_to_radians(60)));
    BOOST_CHECK_CLOSE(circumradius(p1, p2, p3), distance(p1, c), 1e-13);
    triangle_angles(p1, p2, p3, a1, a2, a3);
    BOOST_CHECK_CLOSE(a1, boost::math::constants::pi<double>()/3.0, 1e-13);
    BOOST_CHECK_CLOSE(a2, boost::math::constants::pi<double>()/3.0, 1e-13);
    BOOST_CHECK_CLOSE(a3, boost::math::constants::pi<double>()/3.0, 1e-13);
}

