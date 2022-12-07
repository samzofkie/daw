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

struct TrackSizeInfo
{
  int track_height;
  int space_between_tracks;
  int track_head_width;
};

#endif //TRACK_H
