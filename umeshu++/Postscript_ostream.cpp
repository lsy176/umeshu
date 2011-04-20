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

#include "Edge.h"
#include "Face.h"
#include "Node.h"
#include "Postscript_ostream.h"

#include <cmath>

/* default figure size is 10cm */
const float Postscript_stream::default_fig_size = 10*cm;

/* default figure margin is 1mm */
const float Postscript_stream::default_fig_margin = 0.1*cm;

/* default line width is 0.1mm */
const float Postscript_stream::default_line_width = 0.005*cm;

Postscript_stream::Postscript_stream (std::string const& filename, BoundingBox const& bb)
: of_(filename.c_str()), bb_(bb)
{
    double max_size = std::max(bb_.width(), bb_.height());
    fig_width_ = bb_.width()/max_size*default_fig_size;
    fig_height_ = bb_.height()/max_size*default_fig_size;
    
    of_ << "%!PS-Adobe-3.0 EPSF-3.0" << std::endl;
    of_ << "%%BoundingBox: 0 0 "
        << static_cast<int>(ceil(fig_width_ + 2*default_fig_margin)) << " "
        << static_cast<int>(ceil(fig_height_ + 2*default_fig_margin)) << std::endl;
    of_ << "%%HiResBoundingBox: 0.0 0.0 "
        << fig_width_ + 2*default_fig_margin << " "
        << fig_height_ + 2*default_fig_margin << std::endl;
    of_ << "%%Creator: umeshu++" << std::endl;
    of_ << "%%Title: Unstructured mesh " << filename << std::endl;
    of_ << "%%EndComments" << std::endl;

    of_ << default_fig_margin << " " << default_fig_margin << " translate" << std::endl;
    of_ << default_line_width << " setlinewidth" << std::endl;

    of_ << "/circle {\n"
        << "   newpath " << default_line_width << " 0 360 arc closepath\n"
        << "} def" << std::endl;
}

Postscript_stream& Postscript_stream::operator<< (Point2 const& p)
{
    Point2 c;
    c.x() = fig_width_*(p.x() - bb_.ll().x())/bb_.width();
    c.y() = fig_height_*(p.y() - bb_.ll().y())/bb_.height();
    
    of_ << c.x() << " " << c.y() << " circle" << std::endl;

    return *this;
}

Postscript_stream& Postscript_stream::operator<< (Edge const& e)
{
    Point2 p1, p2;
    e.vertices(p1, p2);

    p1.x() = fig_width_*(p1.x() - bb_.ll().x())/bb_.width();
    p1.y() = fig_height_*(p1.y() - bb_.ll().y())/bb_.height();
    p2.x() = fig_width_*(p2.x() - bb_.ll().x())/bb_.width();
    p2.y() = fig_height_*(p2.y() - bb_.ll().y())/bb_.height();

    of_ << p1.x() << " " << p1.y() << " moveto" << std::endl;
    of_ << p2.x() << " " << p2.y() << " lineto" << std::endl;

    return *this;
}

Postscript_stream& Postscript_stream::operator<< (Face const& f)
{
    Point2 p1, p2, p3;
    f.vertices(p1, p2, p3);
    p1.x() = fig_width_*(p1.x() - bb_.ll().x())/bb_.width();
    p1.y() = fig_height_*(p1.y() - bb_.ll().y())/bb_.height();
    p2.x() = fig_width_*(p2.x() - bb_.ll().x())/bb_.width();
    p2.y() = fig_height_*(p2.y() - bb_.ll().y())/bb_.height();
    p3.x() = fig_width_*(p3.x() - bb_.ll().x())/bb_.width();
    p3.y() = fig_height_*(p3.y() - bb_.ll().y())/bb_.height();

    newpath();
    of_ << p1.x() << " " << p1.y() << " moveto" << std::endl;
    of_ << p2.x() << " " << p2.y() << " lineto" << std::endl;
    of_ << p3.x() << " " << p3.y() << " lineto" << std::endl;
    closepath();

    return *this;
}

void Postscript_stream::newpath()
{
    of_ << "newpath" << std::endl;
}

void Postscript_stream::closepath()
{
    of_ << "closepath" << std::endl;
}

void Postscript_stream::stroke()
{
    of_ << "stroke" << std::endl;
}

void Postscript_stream::fill()
{
    of_ << "fill" << std::endl;
}

void Postscript_stream::setrgbcolor(double r, double g, double b)
{
    of_ << r << " " << g << " " << b << " setrgbcolor" << std::endl;    
}

void Postscript_stream::setgray(double g)
{
    of_ << g << " setgray" << std::endl;    
}
