#ifndef PCM_H
#define PCM_H

#include <fstream>
#include <vector>
#include <cstdint>
#include <complex>

struct WavHeader {
  uint32_t file_size;
  uint16_t num_channels;
  uint32_t sample_rate;
  uint16_t bits_per_sample;
  uint32_t data_size;
};


class PCM
{
  public:
    WavHeader header;
    std::vector<int16_t> samples;
    std::vector<std::complex<double>> fourier_series;
   
    PCM(const char*, double);
    double start_time;
    double sample_rate;
    double length;
  
    struct Color {
      double c0, c1, c2;
    };

    Color color;

  private:
    void read_wav_header(std::ifstream&);
    void print_wav_header(WavHeader);
    void read_wav_data(std::ifstream&);
    Color RandomColor();
    //void fft();
    //void sine_gen(int, int);
};

#endif
