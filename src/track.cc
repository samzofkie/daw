#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include "xwindow.h"
#include "track.h"
#include "pcm.h"

using namespace std;


Track::Track(XWindow *window) :
  parent_window(window) {}


void Track::add_pcm(PCM *pcm, double start_time)
{
  pcm->start_time = start_time;
  pcms.push_back(pcm);
}


void Track::add_pcm(const char* filename, double start_time)
{
  PCM* a = new PCM(filename, parent_window->sample_rate);
  a->start_time = start_time;
  pcms.push_back(a);
}


void Track::draw(cairo_t *cr, double x, double y, double w,
    double h)
{
  // Draw PCMs
  double grid_start_time = parent_window->start_time;
  double grid_end_time = parent_window->end_time;
  double grid_total_time = grid_end_time - grid_start_time;
  double head_width = parent_window->track_head_width; 
  double pixels_per_second = w / grid_total_time;
  
  for (vector<PCM*>::size_type i=0; i < pcms.size(); i++) {
    PCM* pcm = pcms[i];
    double pcm_start_time = pcm->start_time;
    double pcm_end_time = pcm_start_time + pcm->length;
    if (pcm_end_time < grid_start_time 
        || pcm_start_time > grid_end_time) {
      continue;
    }

    bool start_visible = 
      pcm_start_time >= grid_start_time;
    bool end_visible = 
      pcm_end_time <= grid_end_time;

    double pcm_x = head_width + 
      (pcm_start_time - grid_start_time) * pixels_per_second;
    double pcm_y = y;
    double pcm_w = pcm->length * pixels_per_second;
    double pcm_h = h;
    cairo_set_source_rgb(cr, pcm->color.c0,
        pcm->color.c1, pcm->color.c2);
    cairo_rectangle(cr, pcm_x, pcm_y, pcm_w, pcm_h);
    cairo_fill(cr);
  } 

   
  // Draw track head
  double side_space = parent_window->space_between_tracks/2;
  cairo_move_to(cr, head_width + 10, y);
  cairo_line_to(cr, side_space, y);
  cairo_line_to(cr, side_space, y+h);
  cairo_line_to(cr, head_width + 10, y+h);
  cairo_curve_to(cr, head_width + 10, y+h, 
      head_width - 50, y+h/2,
      head_width + 10, y);
  cairo_close_path(cr);
    
  cairo_set_source_surface(cr, 
      parent_window->track_head_fill, 0, y);
  cairo_fill(cr);
}


Track::~Track()
{
  for (auto pcm : pcms)
    delete pcm;
}

