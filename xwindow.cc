#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include "xwindow.h"


using namespace std;

XWindow::XWindow() :
  window_width(1000), window_height(1000), header_height(100),
  vertical_grid_line_space(50),
  track_size_info({95, 5, 175}),
  steel_fill(cairo_image_surface_create_from_png("data/steel.png"))
{
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

  for (int i=0; i<3; i++)
    tracks.push_back(new Track);

  event_loop();
}


void XWindow::draw_header()
{
  cairo_set_source_rgb(cr, 0.05, 0.05, 0.2);
  cairo_rectangle(cr, 0, 0, window_width, header_height);
  cairo_fill(cr);
}


void XWindow::draw_vertical_grid_lines()
{
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_set_line_width(cr, 1);
  for (double i = track_size_info.track_head_width + 0.5; 
       i<window_width; i += vertical_grid_line_space) {
    cairo_move_to(cr, i, header_height);
    cairo_line_to(cr, i, window_height - header_height);
  }
  cairo_stroke(cr);
}


void XWindow::draw_tracks()
{
  int height = track_size_info.track_height;
  int space = track_size_info.space_between_tracks;
  int head_width = track_size_info.track_head_width;
  int total = height + space;

  // Horizontal lines
  for (int i=0; i < tracks.size(); i++) {
    int y = i*total + height - space/2 + header_height;
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_move_to(cr, head_width, y);
    cairo_line_to(cr, window_width, y);
    cairo_stroke(cr);
  }

  // Heads
  for (int i=0; i < tracks.size(); i++) {
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
        printf("Click! @ (%d, %d)\n", e.xbutton.x, e.xbutton.y);
        break;
      case KeyPress:
        XLookupString(&e.xkey, keybuf, sizeof(keybuf), &key, NULL);
        printf("KeyPress string: %s\n", keybuf);
        break;
      case Expose:
        window_width = e.xexpose.width;
        window_height = e.xexpose.height;
        draw_header();
        draw_vertical_grid_lines();
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
