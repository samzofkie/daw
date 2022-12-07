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
    TrackSizeInfo track_size_info;
    cairo_surface_t *steel_fill;

    void draw_header();
    void draw_vertical_grid_lines();
    void draw_tracks();
    void event_loop();
  
  public:
    XWindow();
    ~XWindow(); 
};

#endif //XWINDOW_H
