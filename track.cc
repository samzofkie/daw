#include <iostream>
#include <stdlib.h>
#include "xwindow.h"
#include "track.h"
#include "pcm.h"

using namespace std;


Track::Track(){}


void Track::add(PCM *pcm, double position)
{
  pcms.push_back(pcm);
  pcm_positions.push_back(position);
}


void Track::add(const char* filename, double position)
{
  pcms.push_back( new PCM(filename) );
  pcm_positions.push_back(position);
}


void Track::draw(cairo_t *cr, double x, double y, double w,
    double h, double in_view_start, double in_view_end)
{
  for (vector<PCM*>::size_type i=0; i<pcms.size(); i++) {
    PCM *pcm = pcms[i];
    vector<int16_t>::size_type nsamples = pcm->data.size();
    double pcm_start = pcm_positions[i];
    double pcm_end = pcm_start + (double)nsamples / 44100;
    
    // Don't draw if it ain't in view!
    if (pcm_end < in_view_start || pcm_start > in_view_end)
      return;
    
    double c0 = (double)rand() / RAND_MAX;
    double c1 = (double)rand() / RAND_MAX;
    double c2 = 2.25 - c0 - c1;
    
    cairo_set_source_rgb(cr, c0, c1, c2);

    double elapsed_time = in_view_end - in_view_start;
    double seconds_per_pixel = elapsed_time / w;
    rounded_rect(cr, x+pcm_start * seconds_per_pixel,
        y, x + (pcm_end - pcm_start) * seconds_per_pixel, h, 20);
    cairo_fill(cr);

  }
}


Track::~Track()
{
  for (auto pcm : pcms)
    delete pcm;
}
