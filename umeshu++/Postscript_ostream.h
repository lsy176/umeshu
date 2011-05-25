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

#include "Bounding_box.h"
#include "Point2.h"

#include <fstream>
#include <string>

namespace umeshu {

class Postscript_ostream {    
public:
    Postscript_ostream (std::string const& filename, Bounding_box const& bb);
    
    void newpath() {
        of_ << "np\n";
    }
    void closepath() {
        of_ << "cp\n";
    }
    void moveto(Point2 const& p) {
        Point2 tp = transform(p);
        of_ << tp.x() << " " << tp.y() << " m\n";
    }
    void lineto(Point2 const& p) {
        Point2 tp = transform(p);
        of_ << tp.x() << " " << tp.y() << " l\n";
    }
    void stroke() {
        of_ << "s\n";
    }
    void fill() {
        of_ << "f\n";
    }
    void dot(Point2 const& p) {
        Point2 tp = transform(p);
        of_ << tp.x() << " " << tp.y() << " c\n";
    }
    void setrgbcolor(double r, double g, double b) {
        of_ << r << " " << g << " " << b << " sc\n";
    }
    void setgray(double g) {
        of_ << g << " sg\n";
    }

private:
    Point2 transform(Point2 const& p) {
        Point2 tp;
        tp.x() = x_scale*p.x() - x_trans;
        tp.y() = y_scale*p.y() - y_trans;
        return tp;
    }

    std::ofstream of_;
    Bounding_box bb_;
    double fig_width_, fig_height_;
    double x_scale, y_scale;
    double x_trans, y_trans;

    static float const default_fig_size;
    static float const default_fig_margin;
    static float const default_line_width;
};

} // namespace umeshu

#endif // __POSTSCRIPT_STREAM_H_INCLUDED__
