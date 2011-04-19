//
//  Postscript_ostream.h
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/30/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#ifndef __POSTSCRIPT_STREAM_H_INCLUDED__
#define __POSTSCRIPT_STREAM_H_INCLUDED__

#include "BoundingBox.h"
#include "Mesh_fwd.h"

#include <fstream>
#include <string>

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

#endif // __POSTSCRIPT_STREAM_H_INCLUDED__