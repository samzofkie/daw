#include <gtest/gtest.h>
#include "xwindow.h"
#include "grid.h"

TEST(GridTests, CalcFirstVisibleBeatTimeTest)
{
  XWindow xwindow;
  Grid grid = Grid(&xwindow);
  double a, b;
  
  grid.CalcFirstAndLastVisibleBeatTimes(0, 10, 120, &a, &b);
  EXPECT_EQ(a, 0.0);
  EXPECT_EQ(b, 10.0);

  grid.CalcFirstAndLastVisibleBeatTimes(1.1, 11.1, 120, &a, &b);
  EXPECT_EQ(a, 1.5);
  EXPECT_EQ(b, 11.0);
}
