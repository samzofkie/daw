#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include "xwindow.h"


using namespace std;

XWindow::XWindow() :
  width(500), height(500)
{
  if ((display = XOpenDisplay(NULL)) == NULL) {
    cout << "XOpenDisplay() failed!\n";
    exit(1);
  }

  screen = DefaultScreen(display);

  window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                               0, 0, width, height, 0, 0, 0);

  XSelectInput(display, window, ButtonPressMask | KeyPressMask);
  XMapWindow(display, window);

  c_surf = cairo_xlib_surface_create(display,
                               window, DefaultVisual(display, screen),
                               width, height);
  cairo_xlib_surface_set_size(c_surf, width, height);

  cr = cairo_create(c_surf);
   
  initial_draw();

  event_loop();
}

void XWindow::initial_draw()
{
  cairo_set_source_rgb(cr, 1, 0, 0);
  cairo_rectangle (cr, 100, 100, 200, 200);
  cairo_fill(cr);
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
      default:
        cout << "Unhandled XEvent.type: " << e.type << endl;
    }
  }
}

XWindow::~XWindow()
{
  XCloseDisplay(display);
}
