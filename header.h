#ifndef HEADER_H
#define HEADER_H

#include <cairo.h>

class Header
{
  public:
    Header(double);
    ~Header();
    void draw(cairo_t *, double, double, double, double);
  private:
    double track_head_width, loop_area_height; 
};

#endif //TRACK_H
