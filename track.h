#ifndef TRACK_H
#define TRACK_H

class Track
{
  public:
    Track();
    ~Track();
    void render();
};

struct TrackSizeInfo
{
  int track_height;
  int space_between_tracks;
  int track_head_width;
};

#endif //TRACK_H
