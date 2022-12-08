#include <iostream>
#include <math.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include "xwindow.h"
#include "pcm.h"


using namespace std;

XWindow::XWindow() :
  window_width(1000), window_height(1000), header_height(100),
  vertical_grid_line_space(50),
  track_height(95),
  space_between_tracks(5),
  track_head_width(175),
  steel_fill(cairo_image_surface_create_from_png("data/steel.png")),
  samples_per_wwidth(44100)
{
  // Do all the display / window initing
  if ((display = XOpenDisplay(NULL)) == NULL) {
    cout << "XOpenDisplay() failed!\n";
    exit(1);
  }
  screen = DefaultScreen(display);
  window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                               0, 0, window_width, window_height, 
                               0, 0, 0);
  XSelectInput(display, window, ButtonPressMask | KeyPressMask | ExposureMask);
  XMapWindow(display, window);
  
  // The cairo surface potentially needs to be as big as 
  // the largest screen in the display.
  int surface_width = DisplayWidth(display, screen);
  int surface_height = DisplayHeight(display, screen);
  c_surf = cairo_xlib_surface_create(display,
             window, DefaultVisual(display, screen),
             surface_width, surface_height);
  cairo_xlib_surface_set_size(c_surf, surface_width, surface_height);
  cr = cairo_create(c_surf);


  // Create tracks
  for (int i=0; i<3; i++)
    tracks.push_back(new Track);

  tracks[0]->add("./snare.wav");

  event_loop();
}


void XWindow::draw_header()
{
  cairo_set_source_rgb(cr, 0, 0, 0.01);
  cairo_rectangle(cr, 0, 0, window_width, header_height);
  cairo_fill(cr);
}


void XWindow::draw_vertical_grid_lines()
{
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_set_line_width(cr, 1);
  for (double i = track_head_width + 0.5; 
       i<window_width; i += vertical_grid_line_space) {
    cairo_move_to(cr, i, header_height);
    cairo_line_to(cr, i, window_height - header_height);
  }
  cairo_stroke(cr);
}


void XWindow::draw_tracks()
{
  int height = track_height;
  double space = space_between_tracks;
  int head_width = track_head_width;
  int total = height + space;

  // Horizontal lines
  cairo_set_line_width(cr, 1);
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_move_to(cr, head_width, header_height+0.5);
  cairo_line_to(cr, window_width, header_height+0.5);
  for (vector<Track*>::size_type i=0; i < tracks.size(); i++) {
    double y = i*total + height + header_height + space/2;
    cairo_move_to(cr, head_width, y);
    cairo_line_to(cr, window_width, y);
  }
  cairo_stroke(cr);

  // Heads
  for (vector<Track*>::size_type i=0; i < tracks.size(); i++) {
    int y = i*total + header_height;
    cairo_move_to(cr, head_width + 10, y);
    cairo_line_to(cr, 3, y);
    cairo_line_to(cr, 3, y + height);
    cairo_line_to(cr, head_width + 10, y + height);
    cairo_curve_to(cr, head_width + 10, y + height,
                       head_width - 50, y + height/2,
                       head_width + 10, y);
    cairo_close_path(cr);
    
    cairo_set_source_surface(cr, steel_fill, 0, i*total);
    cairo_fill_preserve(cr);

    cairo_set_source_rgb(cr, 1, 1, 0);
    cairo_stroke(cr);
  }
}


void XWindow::draw_pcms()
{
  double offset = 2;
  for (vector<Track*>::size_type i=0; i<tracks.size(); i++) {
    for (vector<PCM*>::size_type j=0; j<tracks[i]->pcms.size(); j++) {
      PCM *pcm = tracks[i]->pcms[j];
      vector<int16_t>::size_type nsamples = pcm->data.size();
      double pcm_pixel_width = 
        window_width * (nsamples / samples_per_wwidth);
      
      // Draw surrounding rounded rect
      cairo_set_source_rgb(cr, 0, 0, 1);
      rounded_rect(cr, track_head_width, 
                       header_height + offset,
                       pcm_pixel_width, 
                       track_height - offset);
      cairo_fill(cr);

      // Draw waveform
      cairo_set_source_rgb(cr, 1, 1, 1);
      cairo_move_to(cr, track_head_width,
          header_height + track_height / 2);
      for (vector<int16_t>::size_type i=1; i<nsamples; i++) {
        
        double x = track_head_width + (i / pcm_pixel_width);
        double y = header_height + 
          (pcm->data[i] / 65536.0 + 0.5) * track_height;
        
        cairo_line_to(cr, i/pcm_pixel_width + track_head_width,
            header_height + (pcm->data[i] / 65536.0 + 0.5) * track_height);
        //cout << "x: " << x << " y: " << y << endl; 
      }
      cairo_stroke(cr);
    }
  }
}


void XWindow::event_loop()
{
  char keybuf[8];
  KeySym key;
  XEvent e;
  for (;;) {
    XNextEvent(display, &e);
    switch (e.type)
    {
      case ButtonPress:
        printf("Click! @ (%d, %d) by button %u\n", 
            e.xbutton.x, e.xbutton.y, e.xbutton.button);
        break;
      case KeyPress:
        XLookupString(&e.xkey, keybuf, sizeof(keybuf), &key, NULL);
        printf("KeyPress string: %s\n", keybuf);
        break;
      case Expose:
        window_width = e.xexpose.width;
        window_height = e.xexpose.height;
        
        draw_header();
        //draw_vertical_grid_lines();
        draw_pcms();
        draw_tracks();
        
        break;
      default:
        cout << "Unhandled XEvent.type: " << e.type << endl;
    }
  }
}


XWindow::~XWindow()
{
  XCloseDisplay(display);
  for (auto track : tracks)
    delete track;
  cairo_surface_destroy(steel_fill);
}


void rounded_rect(cairo_t *cr, double x, double y,
                               double w, double h)
{
  double radius = 20; 
  cairo_move_to(cr, x+radius, y);
  cairo_line_to(cr, x+w-radius, y);
  cairo_arc(cr, x+w-radius, y+radius,
            radius, M_PI/-2.0, 0);
  cairo_line_to(cr, x+w, y+h-radius);
  cairo_arc(cr, x+w-radius, y+h-radius,
            radius, 0, M_PI/2.0);
  cairo_line_to(cr, x+radius, y+h);
  cairo_arc(cr, x+radius, y+h-radius,
            radius, M_PI/2.0, M_PI);
  cairo_line_to(cr, x, y+radius);
  cairo_arc(cr, x+radius, y+radius,
            radius, M_PI, M_PI/-2.0);
  cairo_close_path(cr);
}
