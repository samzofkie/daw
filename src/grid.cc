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


double Grid::CalcFirstVisibleBeatTime(double start_time,
    double end_time, double tempo)
{
  // tempo bpm, tempo/60.0 bpsecond, 
  // 60.0/tempo seconds per beat
  return ceil(start_time * tempo/60.0) * 60.0/tempo;
}


void Grid::draw_vertical_lines(double x, double y,
    double w, double h)
{
  double start_time = parent_window->start_time;
  double end_time = parent_window->end_time;
  double tempo = parent_window->tempo;
  //double total_time = parent_window->total_time;

  double first_visible_beat_time = 
    CalcFirstVisibleBeatTime(start_time, end_time, tempo);
}


void Grid::draw_horizontal_lines(double x, double y,
    double w, double h)
{

}
