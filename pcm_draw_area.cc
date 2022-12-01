#include <stdio.h>
#include <iostream>
#include <cairomm/context.h>
#include "pcm_draw_area.h"

using namespace std;

PCMDrawArea::PCMDrawArea(vector<int16_t>::iterator start,
                         vector<int16_t>::iterator finish,
                         int w, int h) :
  width(w*2),
  height(h),

  start(start),
  finish(finish)
{
  set_content_width(width);
  set_content_height(height);
  set_draw_func(sigc::mem_fun(*this, &PCMDrawArea::on_draw));
}


void PCMDrawArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
  cr->set_line_width(1);
  
  cr->set_source_rgb(0.0, 0.0, 0.0);
  cr->move_to(0,0);
  cr->line_to(width,0);
  cr->stroke();

  cr->set_source_rgb(1.0, 0.0, 0.0);
  cr->move_to(0,height/2);
  for (int i=0; start+i<finish; i++) {
    float a = (float) *(start+i);
    a /= 32768;
    a += 1;
    a /= 2;
    a *= height;
    cr->line_to(i*2, a);
  }
  cr->stroke();
}
