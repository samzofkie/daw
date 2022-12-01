#include <iostream>

#include <pulse/simple.h>
#include <pulse/error.h>

#include "pcm.h"

class PAHandler
{
  private:
    const pa_sample_spec ss = {
      .format = PA_SAMPLE_S16LE,
      .rate = 44100,
      .channels = 2
    };
    pa_simple *s;
    const uint buffer_size;

  public:
    PAHandler(int);
    ~PAHandler();
    void play(PCM);
};
