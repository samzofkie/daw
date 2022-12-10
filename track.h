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
    void add(PCM*, double position=0);
    void add(const char*, double position=0);
    void draw(cairo_t*, double, double, double,
        double);
    
    struct PCMRegion {
      PCM *pcm;
      double position;
      double c0, c1, c2;
    }; 
    std::vector<PCMRegion> pcmregions;
    XWindow *window;
};

double* rand_color();

#endif //TRACK_H
