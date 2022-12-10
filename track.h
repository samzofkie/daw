#ifndef TRACK_H
#define TRACK_H

#include <vector>
#include <cairo.h>
#include "pcm.h"

class Track
{
  public:
    Track();
    ~Track();
    void add(PCM*, double position=0);
    void add(const char*, double position=0);
    void draw(cairo_t*, double, double, double,
        double, double, double);
    
    std::vector<PCM*> pcms;
    std::vector<double> pcm_positions;
};

#endif //TRACK_H
