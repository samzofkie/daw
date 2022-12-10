#include <iostream>
#include <cairo.h>
#include "xwindow.h"
#include "header.h"
using namespace std;

Header::Header(double w) :
  track_head_width(w),
  loop_area_height(25)
{}


Header::~Header(){}

void Header::draw(cairo_t *cr, double x, double y,
    double w, double h)
{
  cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
  cairo_rectangle(cr, x, y, w, h);
  cairo_fill(cr);

  double border_width = 3;
  cairo_move_to(cr, x, y);
  cairo_line_to(cr, x+w, y);
  cairo_line_to(cr, x+w-border_width, y+border_width);
  cairo_line_to(cr, x+border_width, y+border_width);
  cairo_line_to(cr, x+border_width, y+h-border_width);
  cairo_line_to(cr, x, y+h);
  cairo_line_to(cr, x, y);
  cairo_close_path(cr);

  cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);
  cairo_fill(cr);

  cairo_move_to(cr, x+w, y+h);
  cairo_line_to(cr, x, y+h);
  cairo_line_to(cr, x+border_width, y+h-border_width);
  cairo_line_to(cr, x+w-border_width, y+h-border_width);
  cairo_line_to(cr, x+w-border_width, y+border_width);
  cairo_line_to(cr, x+w, y);
  cairo_line_to(cr, x+w, y+h);
  cairo_close_path(cr);

  cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
  cairo_fill(cr);


  // Draw loop area
  rounded_rect(cr, track_head_width, 
      (h - loop_area_height) - border_width,
      x+w - track_head_width,
      loop_area_height, 10);
  cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
  cairo_stroke(cr);
}
