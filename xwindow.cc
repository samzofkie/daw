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
   
  initial_draw();

  event_loop();
}


void XWindow::initial_draw()
{
  // Draw tracks 
  cairo_set_source_rgb(cr, 0.02, 0.05, 0.3);
  int track_height = 95;
  int space_between_tracks = 5;
  int tot = track_height + space_between_tracks;
  
  for (int i=0; i*tot < height; i++) {
    cairo_rectangle(cr, 0, i*tot, 
                    width, track_height);
    cairo_fill(cr);
  }
    
  // Draw vertical grid lines
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_set_line_width(cr, 1);
  int line_space = 50;
  for (int i=0; i<width; i+=line_space) {
    cairo_move_to(cr, i, 0);
    cairo_line_to(cr, i, height);
  }
  cairo_stroke(cr);
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
