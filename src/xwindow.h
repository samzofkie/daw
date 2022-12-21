#ifndef XWINDOW_H
#define XWINDOW_H

#include <vector>
#include <X11/Xlib.h>

#include "header.h"
#include "track.h"
#include "pahandler.h"

class XWindow
{
 public:
    XWindow();
    ~XWindow();

    cairo_t *cr;

    // Master info 
    double window_width, window_height;
    double total_time, start_time, end_time;
    double tempo;
    unsigned int sample_rate;

    // Track drawing info
    double track_height, space_between_tracks,
           track_head_width;
    cairo_surface_t *track_head_fill;
    
    double header_height;

    void event_loop(); 
  
 private:
    // Window stuff
    Display *display;
    int screen;
    Drawable window; 
    cairo_surface_t *cairo_surf;
    
    std::vector<Track*> tracks;
    Header *header;
    PAHandler *pa_sink;

    void create_window();
    double global_time_to_adjusted_pixels(double);
    void draw_grid();
    void draw_tracks();
    double calc_start_time(double, double);
    void handle_click(XEvent e);
 };

void rounded_rect(cairo_t *cr, 
    double, double, double, double, double);

#endif //XWINDOW_H
