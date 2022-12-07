#include <iostream>
#include "track.h"
#include "pcm.h"

using namespace std;


Track::Track(){}


void Track::add(const char* filename)
{
  pcms.push_back( new PCM(filename) );
}


Track::~Track()
{
  for (auto pcm : pcms)
    delete pcm;
}
