#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include "xwindow.h"


using namespace std;

XWindow::XWindow() :
  width(1000), height(1000)
{
  if ((display = XOpenDisplay(NULL)) == NULL) {
    cout << "XOpenDisplay() failed!\n";
    exit(1);
  }

  screen = DefaultScreen(display);

  window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                               0, 0, width, height, 0, 0, 0);

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
  event_loop();
}


void XWindow::initial_draw()
{
  int track_height = 95;
  int space_between_tracks = 5;
  int tot = track_height + space_between_tracks;
  int track_head_width = 125;

  // Draw vertical grid lines
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_set_line_width(cr, 1);
  int line_space = 50;
  for (int i=track_head_width; i<width; i+=line_space) {
    cairo_move_to(cr, i, 0);
    cairo_line_to(cr, i, height);
  }
  cairo_stroke(cr);
  
  // Horizontal lines  
  for (int i=0; i*tot < height; i++) {
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_move_to(cr, track_head_width, (i+1)*tot - space_between_tracks / 2);
    cairo_line_to(cr, width, i*tot + track_height);
    cairo_stroke(cr);
  }
  
  // Track heads steel fill
  cairo_surface_t *steel = cairo_image_surface_create_from_png("data/steel.png");
  for (int i=0; i*tot < height; i++) {
    cairo_move_to(cr, track_head_width + 10, i*tot);
    cairo_line_to(cr, 3, i*tot);
    cairo_line_to(cr, 3, i*tot + track_height);
    cairo_line_to(cr, track_head_width + 10, i*tot + track_height);
    cairo_curve_to(cr, track_head_width + 10, i*tot + track_height,
                       track_head_width - 30, i*tot + track_height / 2,
                       track_head_width + 10, i*tot);
    cairo_close_path(cr);
    
    cairo_set_source_surface(cr, steel, 0, i*tot);
    cairo_fill_preserve(cr);

    cairo_set_source_rgb(cr, 1, 1, 0);
    cairo_stroke(cr);
  } 
  cairo_surface_destroy(steel);
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
        /*printf("Expose! from (%d, %d) to (%d,%d)\n",
                e.xexpose.x, e.xexpose.y, e.xexpose.width,
                e.xexpose.height);*/
        width = e.xexpose.width;
        height = e.xexpose.height;
        initial_draw();
        break;
      default:
        cout << "Unhandled XEvent.type: " << e.type << endl;
    }
  }
}

XWindow::~XWindow()
{
  XCloseDisplay(display);
}
