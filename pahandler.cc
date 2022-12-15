#include <cstdio>
#include <iostream>

#include <pulse/simple.h>
#include <pulse/error.h>

#include "pahandler.h"
#include "pcm.h"


using namespace std;


PAHandler::PAHandler(int bs) :
  s(NULL),
  buffer_size(bs)
{
  int error;
  if (!(s = pa_simple_new(NULL,
                          "S.CXX!",
                          PA_STREAM_PLAYBACK,
                          NULL,
                          "playback",
                          &ss,
                          NULL,
                          NULL,
                          &error)))
    cerr << __FILE__ << ": pa_simple_new() failed: " << pa_strerror(error) << endl;
}


PAHandler::~PAHandler() 
{
  pa_simple_free(s);
}


void PAHandler::play(PCM pcm)
{
  int error;
  for (uint i=0; i <= pcm.samples.size() / buffer_size; i++) {
    if (pa_simple_write(s, &pcm.samples[i*buffer_size], (size_t)buffer_size, &error) < 0)
      fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
  }

  if (pa_simple_drain(s, &error) < 0) {
    fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
  }
}

