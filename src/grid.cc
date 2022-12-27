#include <iostream>
#include <cassert>
#include <cairo.h>
#include "xwindow.h"
#include "grid.h"
using namespace std;


Grid::Grid(XWindow* window) :
  parent_window(window)
{
}


void Grid::draw(cairo_t *cr, double x, double y,
    double w, double h)
{
  black_out_background(cr,x,y,w,h);
  draw_vertical_lines(cr,x,y,w,h);
  draw_horizontal_lines(cr,x,y,w,h);
}


void Grid::black_out_background(cairo_t* cr, double x, double y,
    double w, double h)
{
  cairo_rectangle(cr, x, y, w, h);  
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_fill(cr);
}


void Grid::CalcFirstAndLastVisibleBeatTimes(double start_time,
    double end_time, double tempo, double *first_visible_beat_time,
    double *last_visible_beat_time)
{
  // tempo bpm, tempo/60.0 bpsecond, 
  // 60.0/tempo seconds per beat
  double beats_per_second = tempo/60.0;
  double seconds_per_beat = 60.0/tempo;
  *first_visible_beat_time = 
    ceil(start_time * beats_per_second) * seconds_per_beat;
  *last_visible_beat_time = 
    floor(end_time * beats_per_second) * seconds_per_beat;
}


void Grid::draw_vertical_lines(cairo_t *cr, double x, double y,
    double w, double h)
{
  double start_time = parent_window->start_time;
  double end_time = parent_window->end_time;
  double tempo = parent_window->tempo;
  //double total_time = parent_window->total_time;

  double first_visible_beat_time, last_visible_beat_time;
  CalcFirstAndLastVisibleBeatTimes(start_time, end_time, tempo,
      &first_visible_beat_time, &last_visible_beat_time);

  double JustPassedTotalTimeVisibleBeatTime = 
    ceil(parent_window->total_time * tempo/6.0) * 60.0/tempo;
  cout << "JustPassedTotal: " << JustPassedTotalTimeVisibleBeatTime
   << endl; 
  double LastBeatTime = min(last_visible_beat_time,
      JustPassedTotalTimeVisibleBeatTime);
  cout << "LastBeatTime: " << LastBeatTime << endl;

  assert (LastBeatTime >= first_visible_beat_time);
  double TotalPixels = w;
  double TotalSeconds = end_time - start_time;
  double PixelsPerSecond = TotalPixels / TotalSeconds;
  for (double i = max(first_visible_beat_time, 0.0);
       i <= LastBeatTime; i += tempo/60.0) {
    cout << i << endl;
    double pixel_x = i * PixelsPerSecond;
    pixel_x = floor(pixel_x) + 0.5;
    cairo_move_to(cr, x+pixel_x, y);
    cairo_line_to(cr, x+pixel_x, y+h);
  }
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_stroke(cr);

}


void Grid::draw_horizontal_lines(cairo_t* cr,
    double x, double y,
    double w, double h)
{

}
