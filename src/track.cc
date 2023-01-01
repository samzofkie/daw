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
  double sample_rate = parent_window->sample_rate;
  double grid_total_time = grid_end_time - grid_start_time;
  double head_width = parent_window->track_head_width; 
  double pixels_per_second = w / grid_total_time;
  
  for (PCM* pcm : pcms) {
    
    double pcm_start_time = pcm->start_time;
    double pcm_end_time = pcm_start_time + pcm->length;
    
    if (pcm_end_time < grid_start_time 
        || pcm_start_time > grid_end_time) {
      continue;
    }
    
    double grid_total_pixels = w - head_width;
    double pixels_per_second = grid_total_pixels / grid_total_time;
    
    double pcm_first_visible_at_second = max(grid_start_time, pcm->start_time);
    double pcm_last_visible_at_second = min(grid_end_time, pcm_end_time);

    pcm->draw(cr, 
        x + head_width + (pcm_first_visible_at_second - grid_start_time) * pixels_per_second,
        y, 
        (pcm_last_visible_at_second - grid_start_time) * pixels_per_second,
        h);

    // Draw body
    /*bool pcm_start_visible = 
      pcm_start_time >= grid_start_time;
    bool end_visible = 
      pcm_end_time <= grid_end_time;

    double pcm_start_truncated_by_grid = 
      max(0.0, pcm_start_time - grid_start_time); 
    double pcm_x = head_width + 
      pcm_start_truncated_by_grid * pixels_per_second;
    double pcm_w = pcm->length * pixels_per_second;
   
    cairo_rectangle(cr, pcm_x, y, pcm_w, h);
    
    cairo_set_source_rgb(cr, pcm->color.c0,
        pcm->color.c1, pcm->color.c2);
    cairo_fill(cr);

    // Draw waveform
    double pcm_visible_start_second = max(grid_start_time, pcm_start_time);
    double pcm_visible_start_offset = 
      pcm_visible_start_second - pcm_start_time;
    int pcm_visible_start_sample_index = 
      floor(pcm_visible_start_offset * sample_rate);
    double samples_per_pixel = sample_rate * 
      (grid_total_time / (w - head_width));
      // Increment by pixels
    cout << "total samples: " << pcm->samples.size() << endl;
    for (int i=4; i < static_cast<int>(w - head_width); i += 4) {
      int index = pcm_visible_start_sample_index +
        i * samples_per_pixel;
      if (static_cast<vector<int16_t>::size_type>(index) 
          >= pcm->samples.size())
        break;
      int16_t sample = pcm->samples[index];
      double sample_scaled_to_pixels = (sample + 32768) / h;
      cout << sample_scaled_to_pixels << endl;
      cairo_line_to(cr, x + head_width + i,
          y + sample_scaled_to_pixels);
    } cout << endl << endl;
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_stroke(cr); */
  
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

