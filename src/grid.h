#ifndef GRID_H
#define GRID_H

class XWindow;

class Grid
{
  public:
    Grid(XWindow *);
    void draw(double, double, double, double);
  private:
    XWindow *parent_window;
    cairo_t *cr;
    void black_out_background(double, double, double, double);
    void draw_vertical_lines(double, double, double, double);
    void draw_horizontal_lines(double, double, double, double);
};

#endif //HEADER_H
