#include <iostream>
#include <math.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include "xwindow.h"

using namespace std;


XWindow::XWindow() :
  wwidth(1000), wheight(1000),
  total_time(10),
  start_time(0),
  end_time(10),
  tempo(120),
  track_manager(new TrackManager(3)), 
  header_height(100),
  header(new Header(track_manager->head_width)),
  pa_sink(new PAHandler(4096))
{
  PCM *snare = new PCM("data/snare.wav");
  track_manager->add_pcm(snare, 1);
  
  create_window(); 
  event_loop();
}


XWindow::~XWindow()
{
  XCloseDisplay(display);
  delete header;
  delete track_manager;
  delete pa_sink;
}


void XWindow::create_window()
{
  if ((display = XOpenDisplay(NULL)) == NULL) {
    cout << "XOpenDisplay() failed!\n";
    exit(1);
  }
  screen = DefaultScreen(display);
  window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                               0, 0, wwidth, wheight, 
                               0, 0, 0);
  XSelectInput(display, window, ButtonPressMask | KeyPressMask | ExposureMask);
  XMapWindow(display, window);
  
  // The cairo surface potentially needs to be as big as 
  // the screen in the display. Defensive would be to make it
  // as big as the Largest screen in the display.
  int surface_width = DisplayWidth(display, screen);
  int surface_height = DisplayHeight(display, screen);
  cairo_surf = cairo_xlib_surface_create(display,
             window, DefaultVisual(display, screen),
             surface_width, surface_height);
  cairo_xlib_surface_set_size(cairo_surf, surface_width, surface_height);
  cr = cairo_create(cairo_surf);
}


void XWindow::draw_vertical_grid_lines()
{
  vertical_grid_line_space = wwidth / 
      ((tempo/60)*(end_time-start_time));
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_set_line_width(cr, 1);
  for (double i = track_manager->head_width + 0.5; 
       i < wwidth * (total_time/(end_time-start_time));
       i += vertical_grid_line_space) {
    cairo_move_to(cr, i, header_height);
    cairo_line_to(cr, i, wheight);
  }
  cairo_stroke(cr);
}


/*
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
                       track_height - offset, 20);
      cairo_fill(cr);

      // Draw waveform
      cairo_set_source_rgb(cr, 1, 1, 1);
      cairo_move_to(cr, track_head_width,
          header_height + track_height / 2);
      double samples_per_pixel = nsamples / pcm_pixel_width;
      for (int i=0; i<pcm_pixel_width; i+=2) {
        double datum = pcm->data[floor(i*samples_per_pixel)];
        cairo_line_to(cr, track_head_width + i, 
            (datum / 65536.0 + 0.5) * track_height + header_height);
      }

      cairo_stroke(cr);
    }
  }
}*/


void XWindow::event_loop()
{
  char keybuf[8];
  KeySym key;
  XEvent e;
  bool scrolled;
  for (;;) {
    XNextEvent(display, &e);
    switch (e.type)
    {
      case ButtonPress:
        printf("Click! @ (%d, %d) by button %u\n", 
            e.xbutton.x, e.xbutton.y, e.xbutton.button);
        
        scrolled = false;
        // Scroll up
        if (e.xbutton.button == 4) {
          end_time += 0.5; 
          scrolled = true;
        // Scroll down
        } else if (e.xbutton.button == 5 && end_time > 0.01) {
          end_time -= 0.5;
          scrolled = true;
        }
        if (scrolled) {
          cairo_set_source_rgb(cr, 0, 0, 0);
          cairo_rectangle(cr, track_manager->head_width,
              header_height, wwidth - track_manager->head_width,
              wheight - header_height);
          cairo_fill(cr);
          draw_vertical_grid_lines();
          track_manager->draw(cr, 0, header_height,
            wwidth, wheight - header_height);
        }
        break;

      case KeyPress:
        XLookupString(&e.xkey, keybuf, sizeof(keybuf), &key, NULL);
        printf("KeyPress string: %s\n", keybuf);
        break;

      case Expose:
        wwidth = e.xexpose.width;
        wheight = e.xexpose.height;
        
        header->draw(cr, 0, 0, wwidth, header_height);
        draw_vertical_grid_lines();
        track_manager->draw(cr, 0, header_height,
            wwidth, wheight - header_height);        
        break;

      default:
        if (e.type != 65)
          cout << "Unhandled XEvent.type: " << e.type << endl;
    }
  }
}


void rounded_rect(cairo_t *cr, double x, double y,
                               double w, double h, double radius)
{
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
