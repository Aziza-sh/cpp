#include <gtest/gtest.h>

unsigned long long factorial(int n) {
  if (n < 0)
    throw std::invalid_argument("n must be non-negative");
  unsigned long long result = 1;
  for (int i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

TEST(FactorialTest, Zero) { EXPECT_EQ(factorial(0), 1); }

TEST(FactorialTest, Positive) {
  EXPECT_EQ(factorial(1), 1);
  EXPECT_EQ(factorial(5), 120);
  EXPECT_EQ(factorial(10), 3628800);
}

TEST(FactorialTest, Negative) {
  EXPECT_THROW(factorial(-1), std::invalid_argument);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}