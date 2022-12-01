#ifndef PCM_DRAW_AREA_H
#define PCM_DRAW_AREA_H

#include <iterator>
#include <gtkmm/drawingarea.h>
#include "pcm.h"

class PCMDrawArea : public Gtk::DrawingArea
{
  public:
   PCMDrawArea(std::vector<int16_t>::iterator,
               std::vector<int16_t>::iterator,
               int, int);
  
  private:
    int width, height;
    std::vector<int16_t>::iterator start, finish;
    //std::vector<int16_t> pcm_data;

  protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
};
#endif //PCM_DRAW_AREA_H
