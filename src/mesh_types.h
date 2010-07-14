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

typedef struct Point2_tag Point2;

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

#endif /* __MESH_TYPES_H_INCLUDED__ */
