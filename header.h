#ifndef HEADER_H
#define HEADER_H

#include <cairo.h>

class XWindow;

class Header
{
  public:
    Header(XWindow *);
    ~Header();
    void draw(cairo_t *, double, double, double, double);
  private:
    XWindow *parent_window;
    double loop_area_height; 
};

#endif //HEADER_H
