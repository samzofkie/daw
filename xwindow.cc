#include <iostream>
#include <thread>
#include <chrono>
#include <X11/Xlib.h>
#include "xwindow.h"

using namespace std;

XWindow::XWindow()
{
  Display * d = XOpenDisplay(NULL);
  if (d==NULL) {
    cout << "XOpenDisplay(NULL) failed!" << endl;
    exit(1);
  }

  int screen = DefaultScreen(d);

  XSetWindowAttributes attrs;

  attrs.event_mask
    = SubstructureRedirectMask // handle child window requests      (MapRequest)
    | SubstructureNotifyMask   // handle child window notifications (DestroyNotify)
    | StructureNotifyMask      // handle container notifications    (ConfigureNotify)
    | ExposureMask             // handle container redraw           (Expose)
    ;
  attrs.do_not_propagate_mask = 0; // do not hide any events from child window
  
  unsigned long attrs_mask = CWEventMask  // enable attrs.event_mask
    | NoEventMask  // enable attrs.do_not_propagate_mask
    | CWBackPixel  // enable attrs.background_pixel
    ;

  int width = 400, height = 200;
  Window w = XCreateWindow(d, RootWindow(d, screen), 0, 0, width, height,
                           1, CopyFromParent, InputOutput, CopyFromParent, attrs_mask, &attrs);

  XMapWindow(d, w);
  XStoreName(d, w, "MyXlibWindow!");
  
  this_thread::sleep_for(chrono::seconds(2));

  XCloseDisplay(d);

}

XWindow::~XWindow(){}
