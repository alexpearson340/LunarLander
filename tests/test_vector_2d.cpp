#include "engine/Vector2D.h"
#include <gtest/gtest.h>

// Test fixture for Block tests
class Vector2DTest : public ::testing::Test {
protected:
  Vector2D testVector{1, 1};
  Vector2D otherVector{2, 3};
};

TEST_F(Vector2DTest, TestConstructor) {
  Vector2D vector{0, 0};
  EXPECT_EQ(vector.getX(), 0);
  EXPECT_EQ(vector.getY(), 0);
}

TEST_F(Vector2DTest, TestGetX) { EXPECT_EQ(testVector.getX(), 1); }

TEST_F(Vector2DTest, TestGetY) { EXPECT_EQ(testVector.getY(), 1); }

TEST_F(Vector2DTest, TestInPlaceAdd) {
  testVector += otherVector;
  EXPECT_EQ(testVector.getX(), 3);
  EXPECT_EQ(testVector.getY(), 4);
}

TEST_F(Vector2DTest, TestInPlaceSubtract) {
  testVector -= otherVector;
  EXPECT_EQ(testVector.getX(), -1);
  EXPECT_EQ(testVector.getY(), -2);
}

TEST_F(Vector2DTest, TestInPlaceAddChained) {
  testVector += otherVector += otherVector;
  EXPECT_EQ(testVector.getX(), 5);
  EXPECT_EQ(testVector.getY(), 7);
}

TEST_F(Vector2DTest, TestAdd) {
  Vector2D result{testVector + otherVector};
  EXPECT_EQ(result.getX(), 3);
  EXPECT_EQ(result.getY(), 4);
}

TEST_F(Vector2DTest, TestSubtract) {
    Vector2D result{testVector - otherVector};
    EXPECT_EQ(result.getX(), -1);
    EXPECT_EQ(result.getY(), -2);
  }