#ifndef TRACKMANAGER_H
#define TRACKMANAGER_H

#include <vector>
#include <cairo.h>
#include "track.h"

class TrackManager
{
  public:
    TrackManager(int n=1);
    ~TrackManager();
    double track_height, space_between_tracks,
           head_width;
    void add_new_track();
    void draw(cairo_t *, double,
        double, double, double);
  private:
    std::vector<Track*> tracks;
    cairo_surface_t *head_fill;
};

#endif //TRACKMANAGER_H
