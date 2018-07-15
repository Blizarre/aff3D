#include <gtest/gtest.h>
#include "vertex.h"
#include "normal.h"

namespace tests {

TEST(Normal, constructor) {
  Normal up{{0,0,0}, {1, 1, 0}, {0, 1, 0}};
  EXPECT_FLOAT_EQ(up.x, 0);
  EXPECT_FLOAT_EQ(up.y, 0);
  EXPECT_FLOAT_EQ(up.z, 1);

  Normal down{{0,0,0}, {0, 1, 0}, {1, 1, 0}};
  EXPECT_FLOAT_EQ(down.x, 0);
  EXPECT_FLOAT_EQ(down.y, 0);
  EXPECT_FLOAT_EQ(down.z, -1);

  Normal left{{0,0,0}, {0, 0, 1}, {0, 1, 0}};
  EXPECT_FLOAT_EQ(left.x, -1);
  EXPECT_FLOAT_EQ(left.y, 0);
  EXPECT_FLOAT_EQ(left.z, 0);
}

TEST(Normal, norm) {
  Normal unit{ {0, 0, 1}};
  EXPECT_FLOAT_EQ(unit.norm(), 1);

  Normal sqrt2{ {0, 1, 1}};
  EXPECT_FLOAT_EQ(sqrt2.norm() * sqrt2.norm(), 2);

  Normal norm5{ {3, 0, 4}};
  EXPECT_FLOAT_EQ(norm5.norm(), 5);

  Normal norm5_2{ {3, 4, 0}};
  EXPECT_FLOAT_EQ(norm5_2.norm(), 5);

  Normal norm5_3{ {0, 3, 4}};
  EXPECT_FLOAT_EQ(norm5_3.norm(), 5);
}

}
