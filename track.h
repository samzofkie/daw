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
    void add(const char*);
    std::vector<PCM*> pcms;
};

#endif //TRACK_H
