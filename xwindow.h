#ifndef XWINDOW_H
#define XWINDOW_H

#include <vector>
#include <X11/Xlib.h>
#include "track.h"

class XWindow
{
  private:
    Display *display;
    int screen;
    int window_width, window_height;
    Drawable window;
    cairo_surface_t *c_surf;
    cairo_t *cr;

    int header_height;

    int vertical_grid_line_space;
    
    std::vector<Track*> tracks;
    double track_height, space_between_tracks,
           track_head_width;
    cairo_surface_t *steel_fill;

    void draw_header();
    void draw_vertical_grid_lines();
    void draw_tracks();
    void draw_pcms();
    void event_loop();

    double samples_per_wwidth;
  
  public:
    XWindow();
    ~XWindow(); 
};

void rounded_rect(cairo_t *cr, double, double, double, double);

#endif //XWINDOW_H
