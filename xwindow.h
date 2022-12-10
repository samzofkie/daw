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

    // Master info 
    double wwidth, wheight;
    double total_time, start_time, end_time;
    double tempo;

    // Track drawing info
    double track_height, space_between_tracks,
           track_head_width;
    cairo_surface_t *track_head_fill;
    
    double header_height;


  private:
    // Window stuff
    Display *display;
    int screen;
    Drawable window; 
    cairo_surface_t *cairo_surf;
    cairo_t *cr;
    
    std::vector<Track*> tracks;
    Header *header;
    PAHandler *pa_sink;

    // TrackManager *track_manager;

    //Header *header;

    //PAHandler *pa_sink;

    void create_window();
    //void draw_vertical_grid_lines();
    void draw_grid();
    void event_loop(); 
 };

void rounded_rect(cairo_t *cr, 
    double, double, double, double, double);

#endif //XWINDOW_H
