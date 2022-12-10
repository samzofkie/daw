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
  total_time(10),
  start_time(0),
  end_time(10),
  tempo(120),
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
  
  for (int i=0; i<2; i++)
    tracks.push_back(new Track(this));

  PCM *snare = new PCM("data/snare.wav");
  tracks[1]->add(snare, 1);


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
  double total_track_height = track_height + space_between_tracks;

  // initial line
  cairo_move_to(cr, track_head_width+0.5, header_height);
  cairo_line_to(cr, track_head_width+0.5, 
      header_height+total_track_height*tracks.size());

  double bps = tempo/60;
  double elapsed_beats = (end_time-start_time)*bps;
  double pixels_per_beat = (wwidth-track_head_width) / elapsed_beats;

  int num_visible_beats = floor(elapsed_beats);
  double offset_beat = elapsed_beats - num_visible_beats;
  double offset_pixel = offset_beat * pixels_per_beat;

  int total_beats = floor(total_time*bps);
  int num_beats_to_draw = min(num_visible_beats, total_beats); 
  
  for (int i=0; i<num_beats_to_draw; i++) {
    double x = track_head_width + offset_pixel + i*pixels_per_beat;
    x = floor(x) + 0.5;
    cairo_move_to(cr, x, header_height);
    cairo_line_to(cr, x, header_height + tracks.size()*total_track_height);
  }
    
  // Vertical lines
  /*double beats_visible = (tempo/60)*(end_time-start_time);
  double vertical_line_space = wwidth / beats_visible;
  
  // Is this drawing off screen?
  for (double i = track_head_width + 0.5; 
       i < wwidth * (total_time/(end_time-start_time));
       i += vertical_line_space) {
    cairo_move_to(cr, i, header_height);
    cairo_line_to(cr, i, header_height + total_track_height * tracks.size());
  }
 
  // Horizontal lines
  for (vector<Track*>::size_type i=0; i<tracks.size()+1; i++) {
    cairo_move_to(cr, track_head_width, total_track_height * i + header_height+0.5);
    cairo_line_to(cr, wwidth * total_time/(end_time-start_time), 
        total_track_height * i + header_height+0.5);
  }*/

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_set_line_width(cr, 1);
  cairo_stroke(cr);
}


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
          cairo_rectangle(cr, track_head_width,
              header_height, wwidth - track_head_width,
              wheight - header_height);
          cairo_fill(cr);
          draw_grid();
          
          for (vector<Track*>::size_type i=0; 
              i<tracks.size(); i++)
            tracks[i]->draw(cr, 0, 
                i*(track_height+space_between_tracks)
                + space_between_tracks/2 + header_height,
                wwidth, track_height);
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
        draw_grid();
        for (vector<Track*>::size_type i=0; i<tracks.size(); i++)
            tracks[i]->draw(cr, 0, 
                i*(track_height+space_between_tracks)
                + space_between_tracks/2 + header_height,
                wwidth, track_height);


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
