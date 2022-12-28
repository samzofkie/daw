#include <fstream>
#include <iostream>
#include <complex>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

#include "pcm.h"

using namespace std;


PCM::PCM(const char* filename, double srate) :
  start_time(0),
  sample_rate(srate),
  color(RandomColor())
{
  srand(time(NULL));
  std::ifstream ifs (filename, std::ios::in | std::ios::binary); 
  if (!ifs) {
    std::cerr << "Failed to open " << filename << endl;
    return;
  }
  read_wav_header(ifs);
  read_wav_data(ifs);
 
  ifs.close();

  length = static_cast<double>(samples.size()) 
    / sample_rate;
}


PCM::Color PCM::RandomColor()
{
  Color c;
  c.c0 = rand() / RAND_MAX;
  c.c1 = rand() / RAND_MAX;
  c.c2 = 2.25 - c.c0 - c.c1;
  return c;
}


template<typename I>
I byte_build(int start, int end, uint8_t(& arr)[])
{
  I res = 0;
  for (int i=end; i>=start; i--) {
    res <<= 8;
    res |= arr[i];
  }
  return res;
}


void PCM::print_wav_header(WavHeader header)
{
  cout << "file size: " << (uint)header.file_size << endl;
  cout << "num channels: " << (uint)header.num_channels << endl;
  cout << "sample rate: " << (uint)header.sample_rate << endl;
  cout << "bits per sample: " << (uint)header.bits_per_sample << endl;
  cout << "data size: " << (uint)header.data_size << endl;
}


void PCM::read_wav_header(ifstream& ifs)
{
  uint8_t header_bytes[44];
  ifs.read((char*)header_bytes, 44);

  header = {
    byte_build<uint32_t>(4,7,header_bytes),
    byte_build<uint16_t>(22,23,header_bytes),
    byte_build<uint32_t>(24,27,header_bytes),
    byte_build<uint16_t>(34,35,header_bytes),
    byte_build<uint32_t>(40,43,header_bytes)
  };
}


void PCM::read_wav_data(ifstream& ifs)
{
  uint8_t pcm_bytes[2];
  for (uint i=0; i<(uint)header.data_size; i+=4) {
    ifs.read((char*)pcm_bytes, 2);
    uint16_t sample = byte_build<uint16_t>(0,1,pcm_bytes);
    
    int16_t a = (int16_t)sample;
            
    samples.push_back(a);
  }
}


/*void PCM::fft()
{
  complex<double> I(0,1);
  for (ulong i=0; i<samples.size(); i++) {
    complex<double> sum(0,0);
    for (ulong j=0; j<samples.size(); j++) {
      complex<double> term = -I * 2.0 * M_PI * (double)i 
                             * (double)j / (double)samples.size();
      term = (double)samples[j] * exp(term);
      sum += term;
    }
    fourier_series.push_back(sum);
  }
  const auto [min, max] = minmax(begin(fourier_series), end(fourier_series));
}


void PCM::sine_gen(int length, int freq)
{
  for (int i=0; i<length; i++) {
    double x = freq * 2 * M_PI * i;
    x /= 44100;
    x = sin(x) * 20000;
    int r = rand() % 10000 - 5000;
    if (r>0)
      x = min(32767.0, x+r);
    else
      x = max(0.0,x+r);
    samples.push_back((int16_t)r*3);
  }
}*/
