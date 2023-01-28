#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>
#include <X11/Xlib.h>
#include <X11/Xutil.h> // XLookupString
#include <cairo.h>
#include <cairo-xlib.h>

struct Point {
  double x, y;
};

class Area {
 public:
  Area(double x, double y, double w, double h) : 
    x(x), y(y), w(w), h(h) {}
  double x, y, w, h;
  virtual void draw(cairo_t* cr) =0;
  virtual bool is_point_in(Point) =0;
};

class Rectangle : public Area {
 public:
  using Area::Area;
  void draw(cairo_t* cr) {
    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_rectangle(cr, x, y, w, h);
    cairo_fill(cr);
  }

  bool is_point_in(Point p) {
    if (p.x >= x && p.x <= x+w)
      if (p.y >= y && p.y <= y+h)
        return true;
    return false;
  }
};


int main() {
  double ww = 1500, wh = 750;
  
  Display *display = XOpenDisplay(NULL);
  if (display==NULL) {
    fprintf(stderr, "XOpenDisplay() failed!");
    exit(1);
  }
  int screen = DefaultScreen(display);
  Drawable window = XCreateSimpleWindow(display, DefaultRootWindow(display),
      0, 0, ww, wh, 0, 0, 0);
  XSelectInput(display, window, ButtonPressMask | KeyPressMask | ExposureMask);
  XMapWindow(display, window);
  int surf_width = DisplayWidth(display, screen);
  int surf_height = DisplayHeight(display, screen);
  
  cairo_surface_t *csurf = cairo_xlib_surface_create(display,
     window, DefaultVisual(display, screen), surf_width, surf_height);
  cairo_xlib_surface_set_size(csurf, surf_width, surf_height);
  cairo_t *cr = cairo_create(csurf); 
  
  std::vector<Rectangle> rects;
  for (int i=0; i<50; i++)
    rects.push_back(Rectangle(i*10, i*10, 10, 10));

  char keybuf[8];
  KeySym key;
  XEvent e;
  for (;;) {
    XNextEvent(display, &e);
    switch (e.type)
    {
      case ButtonPress:  
        printf("click\n");       
        break;

      case KeyPress:
        // XLookupString(&e.xkey, keybuf, sizeof(keybuf), &key, NULL);
        // printf("KeyPress string: %s\n", keybuf);
        break;

      case Expose:
        ww = e.xexpose.width;
        wh = e.xexpose.height;
        for (auto r : rects) 
          r.draw(cr);
        break;

      default:
        if (e.type != 65)
          printf("Unhandled XEvent.type: %d\n", e.type);
    }
  }
   
  return 0;
}
