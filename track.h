#ifndef TRACK_H
#define TRACK_H

#include <vector>
#include <cairo.h>
#include "pcm.h"

class XWindow;

class Track
{
  public:
    Track(XWindow*);
    ~Track();
    void add(PCM*, double start_time=0);
    void add(const char*, double start_time=0);
    void draw(cairo_t*, double, double, double,
        double);
    
    struct PCMRegion {
      PCM *pcm;
      double start_time;
      double c0, c1, c2;
    }; 
    std::vector<PCMRegion> pcm_regions;
    XWindow *window;
};

double* rand_color();

#endif //TRACK_H
