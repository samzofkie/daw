#include <gtest/gtest.h>
#include "xwindow.h"
#include "grid.h"

TEST(GridTests, CalcFirstVisibleBeatTimeTest)
{
  XWindow xwindow;
  Grid grid = Grid(&xwindow);
  EXPECT_EQ(grid.CalcFirstVisibleBeatTime(0,10,120), 0);
  EXPECT_EQ(grid.CalcFirstVisibleBeatTime(1.1,10,120), 1.5); 
}
