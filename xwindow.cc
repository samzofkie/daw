#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include "xwindow.h"

using namespace std;


XWindow::XWindow() :
  wwidth(1000), wheight(1000),
  total_time(3),
  start_time(0),
  end_time(4),
  tempo(120),
  sample_rate(44100),
  track_height(95),
  space_between_tracks(5),
  track_head_width(175),
  track_head_fill(
      cairo_image_surface_create_from_png("data/steel.png")),
  header_height(100),
  header(new Header(this)),
  pa_sink(new PAHandler(4096))
{
  srand(time(NULL));
    
  create_window(); 
  
  for (int i=0; i<8; i++)
    tracks.push_back(new Track(this));

  PCM *snare = new PCM("data/snare.wav");
  tracks[1]->add(snare);

  event_loop();
}


XWindow::~XWindow()
{
  XCloseDisplay(display);
  cairo_surface_destroy(track_head_fill);
  delete header;
  for (Track *track : tracks)
    delete track;
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


void XWindow::draw_grid()
{
  // Black out underneath
  cairo_rectangle(cr, track_head_width,
              header_height, wwidth - track_head_width,
              wheight - header_height);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_fill(cr);

  double total_track_height = track_height + space_between_tracks;

  // Initial line
  cairo_move_to(cr, track_head_width+0.5, header_height);
  cairo_line_to(cr, track_head_width+0.5, 
      header_height + total_track_height*tracks.size());

  // Vertical lines
  double beats_per_second = tempo/60;
  int first_beat = ceil(start_time * beats_per_second);
  int last_beat = floor(end_time * beats_per_second);
  last_beat = min(last_beat, (int)ceil(total_time * beats_per_second));
  double seconds_per_beat = 60/tempo;
  double offset_time = (first_beat * seconds_per_beat) - start_time;
  double pixels_per_second = (wwidth - track_head_width) 
                            / (end_time - start_time);
  double offset_pixels = offset_time * pixels_per_second;
  double pixels_per_beat = pixels_per_second * seconds_per_beat;

  for (double i = offset_pixels + track_head_width; 
      i<wwidth; i += pixels_per_beat) {
    cairo_move_to(cr, i, header_height);
    cairo_line_to(cr, i, header_height + 
        tracks.size() * total_track_height);
  }
      
  // Horizontal lines
  for (vector<Track*>::size_type i=0; i<tracks.size()+1; i++) {
    cairo_move_to(cr, track_head_width, 
        total_track_height*i + header_height + 0.5);
    double x = (beat_offset + last_possible_beat) * pixels_per_beat;
    x = floor(x) + 0.5 + track_head_width; 
    cairo_line_to(cr, min(x, wwidth),
        total_track_height*i + header_height + 0.5);
  } 

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_set_line_width(cr, 1);
  cairo_stroke(cr);
}


void XWindow::draw_tracks()
{
  for (vector<Track*>::size_type i=0; i<tracks.size(); i++)
    tracks[i]->draw(cr, 0, 
        i*(track_height+space_between_tracks) + space_between_tracks/2 + header_height,
                wwidth, track_height);
}


void XWindow::handle_click(XEvent e)
{
  /*printf("Click! @ (%d, %d) by button %u\n", 
            e.xbutton.x, e.xbutton.y, e.xbutton.button);*/
  double x = max(0.0, e.xbutton.x - track_head_width);  
  double ratio = x / (wwidth - track_head_width);
  bool scrolled = false;
  
  // Scroll logic
  //   x is mouse position
  //   start_time + x / end_time - x = 
  //   new_start_time + x / new_end_time - x
  // to zoom in on whats underneath the cursor
  
  if (e.xbutton.button == 4) { 
    // Scroll up? Zoom out
    double new_start_time = start_time - ratio;
    double new_end_time = (new_start_time + x) * (end_time - x) / (start_time + x) + x;
    new_start_time = max(0.0, new_start_time);
    start_time = new_start_time;
    end_time = new_end_time;
    scrolled = true;

  } else if (e.xbutton.button == 5) { 
    // Scroll down? Zoom in
    double new_start_time = start_time + ratio;
    double new_end_time = (new_start_time + x) * (end_time - x) / (start_time + x) + x;
    new_end_time = max(0.1, new_end_time);
    start_time = new_start_time;
    end_time = new_end_time;
    scrolled = true;
  }
  if (scrolled) {
    //cout << start_time << " " << end_time << endl;
    draw_grid();
    draw_tracks();          
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
        handle_click(e);
        break;

      case KeyPress:
        XLookupString(&e.xkey, keybuf, sizeof(keybuf), &key, NULL);
        printf("KeyPress string: %s\n", keybuf);
        break;

      case Expose:
        wwidth = e.xexpose.width;
        wheight = e.xexpose.height;
        
        header->draw(cr, 0, 0, wwidth, header_height);
        draw_grid();
        draw_tracks();
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
