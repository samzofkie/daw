#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include "xwindow.h"
#include "track.h"
#include "pcm.h"

using namespace std;


Track::Track(XWindow *window) :
  window(window) {}


void Track::add(PCM *pcm, double position)
{
  double c0 = (double)rand() / RAND_MAX;
  double c1 = (double)rand() / RAND_MAX;
  double c2 = 2.25 - c0 - c1;
  pcmregions.push_back({pcm, position, c0, c1, c2});
}


void Track::add(const char* filename, double position)
{
  double c0 = (double)rand() / RAND_MAX;
  double c1 = (double)rand() / RAND_MAX;
  double c2 = 2.25 - c0 - c1;
  pcmregions.push_back({new PCM(filename), 
      position, c0, c1, c2});
}


void Track::draw(cairo_t *cr, double x, double y, double w,
    double h)
{
  // Draw track head
  //double top_y = (track_height + space_between_tracks) * i + y
     //+ space_between_tracks/2;
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

  
  /*// Draw pcms
  double in_view_start = 0, in_view_end = 10;
  
  for (vector<PCM*>::size_type i=0; i<pcmregions.size(); i++) {
    
    PCM *pcm = pcmregions[i].pcm;
    vector<int16_t>::size_type nsamples = pcm->data.size();
    
    double pcm_start = pcmregions[i].position;
    double pcm_end = pcm_start + (double)nsamples / 44100;
    
    // Don't draw if it ain't in view!
    if (pcm_end < in_view_start || pcm_start > in_view_end)
      continue;
         
    // Convert from start and end times (seconds) of the sample
    // were gonna show to pixels
    double elapsed_time = in_view_end - in_view_start;
    double pixels_per_second = w / elapsed_time;
     
    // Draw region body
    double seconds_per_pixel = elapsed_time / w;

    rounded_rect(cr, x + head_width + pcm_start*seconds_per_pixel,
        y, x + (pcm_end - pcm_start) * seconds_per_pixel, h, 20);
    cairo_set_source_rgb(cr, pcmregions[i].c0,
        pcmregions[i].c1, pcmregions[i].c2);
    cairo_fill(cr);

    // Draw waveform
    //double nsamples_to_draw = floor(elapsed_time * 44100);
    for (double i = max(in_view_start, pcm_start);
         i < min(in_view_end, pcm_end);
         i += w / elapsed_time) {
    }
  }*/
}


Track::~Track()
{
  for (auto pcmregion : pcmregions)
    delete pcmregion.pcm;
}

