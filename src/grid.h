#ifndef GRID_H
#define GRID_H

#include <cairo.h>

class XWindow;

/* Grid is created and held by the top level XWindow. When the 
 * parent window says draw, it blacks out the background and draws
 * the grid based on the tempo, start, end and total times held by 
 * the parent, and the x,y,w,h parameters passed in the call to draw.
 * 
 * Some helper functions are public not because they really should be,
 * but because they are complex enough I want to keep them small and
 * well tested (which seems like some real cognitive dissonance for
 * OOP and TDD and clean code, right?).
 */
class Grid
{
  public:
    Grid(XWindow *);
    void draw(cairo_t*, double, double, double, double);
    
    void CalcFirstAndLastVisibleBeatTimes(double, 
        double, double, double*, double*);
  private:
    XWindow *parent_window;
    void black_out_background(cairo_t*, double, double, double, double);
    void draw_vertical_lines(cairo_t*, double, double, double, double);
    void draw_horizontal_lines(cairo_t*, double, double, double, double);
};

#endif //HEADER_H
