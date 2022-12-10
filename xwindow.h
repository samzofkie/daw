#ifndef XWINDOW_H
#define XWINDOW_H

#include <vector>
#include <X11/Xlib.h>

#include "header.h"
#include "track.h"
#include "track_manager.h"
#include "pahandler.h"

class XWindow
{
 public:
    XWindow();
    ~XWindow();

  private:
    Display *display;
    int screen;
    Drawable window;
    cairo_surface_t *cairo_surf;
    cairo_t *cr;
    
    int wwidth, wheight;
    double total_time, start_time, end_time, tempo;
    double samples_per_wwidth; 
    double vertical_grid_line_space;
    
    TrackManager *track_manager;

    double header_height;
    Header *header;

     
    PAHandler *pa_sink;

    void create_window();
    void draw_vertical_grid_lines();
    void event_loop(); 
 };

void rounded_rect(cairo_t *cr, 
    double, double, double, double, double);

#endif //XWINDOW_H
