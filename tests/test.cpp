#include <gtest/gtest.h>

TEST(monTestCase, MonTEST) {
  EXPECT_EQ(3, 3) << "Works";
  EXPECT_EQ(2, 3) << "Fail";
}
