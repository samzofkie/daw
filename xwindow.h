#ifndef XWINDOW_H
#define XWINDOW_H

#include <X11/Xlib.h>

class XWindow
{
  private:
    Display *display;
    int screen;
    int width, height;
    Drawable window;
    cairo_surface_t *c_surf;
    cairo_t *cr;

    void event_loop();
    void initial_draw();
  public:
    XWindow();
    ~XWindow(); 
};

#endif //XWINDOW_H
