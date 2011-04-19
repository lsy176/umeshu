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

#ifndef __POINT2_H_INCLUDED__
#define __POINT2_H_INCLUDED__ 

#include "mesh_types.h"

void point2_set( Point2 *point, gdouble x, gdouble y );
gdouble point2_distance( const Point2 *p1, const Point2 *p2 );
gboolean point2_is_in_triangle( const Point2 *p, const Point2 *p1, const Point2 *p2, const Point2 *p3 );

/** Computes a weighted average of two points. The resulting point will lie on the
 * segment connecting the two points.
 *
 * \param[in] t weight in the interval \f$[0,1]\f$. If equal to 0, position of
 * point p will be equal to the position of p1, if t equals to 1, the position of p
 * will be equal to position of p2.
 * \param[in] p1 first point
 * \param[in] p2 second point
 *
 * \return resulting point
 */
Point2 point2_interpolate( const Point2 *p1, const Point2 *p2, gdouble t );

#endif /* __POINT2_H_INCLUDED__ */
