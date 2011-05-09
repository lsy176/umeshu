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

#ifndef __POSTSCRIPT_STREAM_H_INCLUDED__
#define __POSTSCRIPT_STREAM_H_INCLUDED__

#include "BoundingBox.h"
#include "Mesh_fwd.h"

#include <fstream>
#include <string>

namespace umeshu {

/* 1cm is this amount of Points */
const float cm = 28.3464567;

class Postscript_stream {    
public:
    Postscript_stream (std::string const& filename, BoundingBox const& bb);
    
    void newpath();
    void closepath();
    void stroke();
    void fill();
    void setrgbcolor(double r, double g, double b);
    void setgray(double g);

    Postscript_stream& operator<< (Point2 const& p);
    Postscript_stream& operator<< (Edge const& e);
    Postscript_stream& operator<< (Face const& f);

private:
    std::ofstream of_;
    BoundingBox bb_;
    double fig_width_, fig_height_;

    static const float default_fig_size;
    static const float default_fig_margin;
    static const float default_line_width;
};

} // namespace umeshu

#endif // __POSTSCRIPT_STREAM_H_INCLUDED__
