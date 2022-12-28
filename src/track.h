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
    void add_pcm(PCM*, double start_time=0);
    void add_pcm(const char*, double start_time=0);
    void draw(cairo_t*, double, double, double,
        double);
 
    std::vector<PCM*> pcms;
    XWindow *parent_window;
};

#endif //TRACK_H
