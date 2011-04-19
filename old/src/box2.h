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

#ifndef __BOX2_H_INCLUDED__
#define __BOX2_H_INCLUDED__ 

/** \file
 * \brief Two-dimensional bounding box structure and related functions */

#include <glib.h>

/** 2D bounding-box data structure */
typedef struct
{
    gdouble min[2];  /** coordinates of the lower-left corner */
    gdouble max[2];  /** coordinates of the upper-right corner */
} Box2;

/** Allocates a new bounding box.
 *
 * \param min_x x-coordinate of the lower left corner
 * \param max_x x-coordinate of the upper right corner
 * \param min_y y-coordinate of the lower left corner
 * \param max_y y-coordinate of the upper right corner
 *
 * \return Pointer to the newly created bounding box
 */
Box2 * box2_new( gdouble min_x, gdouble max_x, gdouble min_y, gdouble max_y );

/** Frees the bounding box from the memory
 *
 * \param box Pointer to a bounding box data structure
 */
void box2_free( Box2 *box );

/** Sets the extents of a bounding box
 * 
 * \param box Pointer to a bounding box data structure
 * \param min_x x-coordinate of the lower left corner
 * \param max_x x-coordinate of the upper right corner
 * \param min_y y-coordinate of the lower left corner
 * \param max_y y-coordinate of the upper right corner
 */
void box2_set( Box2 *box, gdouble min_x, gdouble max_x, gdouble min_y, gdouble max_y );

/** Return the width of a bounding box
 * \param box Pointer to a bounding box data structure
 */
gdouble box2_width( Box2 *box );

/** Return the height of a bounding box
 * \param box Pointer to a bounding box data structure
 */
gdouble box2_height( Box2 *box );

/** Updates the bounding box to include a given point
 *
 * \param box box to be updated
 * \param x x-coordinate of the point
 * \param y y-coordinate of the point
 */
void box2_include_point( Box2 *box, gdouble x, gdouble y );

/** Updates the bounding box to include another bounding box
 *
 * \param box box to be updated
 * \param box2 box to be included
 */
void box2_include_box( Box2 *box, const Box2 *box2 );

/** Prints a formatted information about the bounding box
 * 
 * \param box Pointer to the bounding box data structure
 */
void box2_print( const Box2 *box );

#endif /* __BOX2_H_INCLUDED__ */
