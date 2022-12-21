#include <iostream>
#include <cairo.h>
#include "xwindow.h"
#include "grid.h"
using namespace std;


Grid::Grid(XWindow* window) :
  parent_window(window),
  cr(parent_window->cr)
{}


void Grid::draw(double x, double y,
    double w, double h)
{
  black_out_background(x,y,w,h);
  draw_vertical_lines(x,y,w,h);
  draw_horizontal_lines(x,y,w,h);
}


void Grid::black_out_background(double x, double y,
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


void Grid::draw_vertical_lines(double x, double y,
    double w, double h)
{
  double start_time = parent_window->start_time;
  double end_time = parent_window->end_time;
  double tempo = parent_window->tempo;
  //double total_time = parent_window->total_time;

  double first_visible_beat_time, last_visible_beat_time;
  CalcFirstAndLastVisibleBeatTimes(start_time, end_time, tempo,
      &first_visible_beat_time, &last_visible_beat_time);
}


void Grid::draw_horizontal_lines(double x, double y,
    double w, double h)
{

}
