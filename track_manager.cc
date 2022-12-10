#include <iostream>
#include <vector>
#include <cairo.h>
#include "track_manager.h"
#include "track.h"
#include "pcm.h"
using namespace std;


TrackManager::TrackManager(int n) :
  track_height(95),
  space_between_tracks(5),
  head_width(175),
  head_fill(
      cairo_image_surface_create_from_png("data/steel.png"))
{
  for (int i=0; i<n; i++)
    tracks.push_back(new Track);
}


TrackManager::~TrackManager()
{
  cairo_surface_destroy(head_fill);
  for (auto track : tracks)
    delete track;
}


void TrackManager::add_new_track()
{
  tracks.push_back(new Track);
}


void TrackManager::draw(cairo_t *cr,
    double x, double y, double w, double h)
{
  // This is a nasty hack to make cairo draw single pixel
  // bright lines instead of dull two-pixel wide lines
  y += 0.5;
  // Horizontal lines
  cairo_set_line_width(cr, 1);
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_move_to(cr, x+head_width, y);
  cairo_line_to(cr, x+w, y);
  for (vector<Track*>::size_type i=0; i < tracks.size(); i++) {
    double line_y = y + (i+1)*(track_height+space_between_tracks);
    cairo_move_to(cr, x+head_width, line_y);
    cairo_line_to(cr, x+w, line_y);
  }
  cairo_stroke(cr);

  // Track heads
  for (vector<Track*>::size_type i=0; i < tracks.size(); i++) {
    int top_y = (track_height + space_between_tracks) * i + y
      + space_between_tracks/2;
    cairo_move_to(cr, head_width + 10, top_y);
    cairo_line_to(cr, 3, top_y);
    cairo_line_to(cr, 3, top_y + track_height);
    cairo_line_to(cr, head_width + 10, top_y + track_height);
    cairo_curve_to(cr, head_width + 10, top_y + track_height,
                       head_width - 50, top_y + track_height/2,
                       head_width + 10, top_y);
    cairo_close_path(cr);
    
    cairo_set_source_surface(cr, head_fill, 0, top_y - y);
    cairo_fill(cr);
  }
}
