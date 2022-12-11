#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include "xwindow.h"
#include "track.h"
#include "pcm.h"

using namespace std;


Track::Track(XWindow *window) :
  window(window) {}


void Track::add(PCM *pcm, double start_time)
{
  double c0 = (double)rand() / RAND_MAX;
  double c1 = (double)rand() / RAND_MAX;
  double c2 = 2.25 - c0 - c1;
  pcm_regions.push_back({pcm, start_time, c0, c1, c2});
}


void Track::add(const char* filename, double start_time)
{
  double c0 = (double)rand() / RAND_MAX;
  double c1 = (double)rand() / RAND_MAX;
  double c2 = 2.25 - c0 - c1;
  pcm_regions.push_back({new PCM(filename), 
      start_time, c0, c1, c2});
}


void Track::draw(cairo_t *cr, double x, double y, double w,
    double h)
{
  double start_time = window->start_time;
  double end_time = window->end_time;

  // Draw PCMs
  
  // Draw track head
  double side_space = window->space_between_tracks/2;
  double head_width = window->track_head_width;
  cairo_move_to(cr, head_width + 10, y);
  cairo_line_to(cr, side_space, y);
  cairo_line_to(cr, side_space, y+h);
  cairo_line_to(cr, head_width + 10, y+h);
  cairo_curve_to(cr, head_width + 10, y+h, 
      head_width - 50, y+h/2,
      head_width + 10, y);
  cairo_close_path(cr);
    
  cairo_set_source_surface(cr, window->track_head_fill, 0, y);
  cairo_fill(cr);
}


Track::~Track()
{
  for (auto pcmregion : pcm_regions)
    delete pcmregion.pcm;
}

