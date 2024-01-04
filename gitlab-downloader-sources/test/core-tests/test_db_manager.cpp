#include <gtest/gtest.h>

struct DBLibTests : public testing::Test {
protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

// TEST_F(DBLibTests, test0) { EXPECT_EQ(true, false); }