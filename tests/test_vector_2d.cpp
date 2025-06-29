#include "engine/Vector2D.h"
#include <gtest/gtest.h>

// Test fixture for Block tests
class Vector2DTest : public ::testing::Test
{
protected:
    Vector2D testVector { 1.0f, 1.0f };
    Vector2D otherVector { 2.0f, 3.0f };
};

TEST_F(Vector2DTest, TestConstructor)
{
    Vector2D vector { 0.0f, 0.0f };
    EXPECT_NEAR(vector.getX(), 0.0f, 0.0f);
    EXPECT_NEAR(vector.getY(), 0.0f, 0.0f);
}

TEST_F(Vector2DTest, TestGetX) { EXPECT_NEAR(testVector.getX(), 1.0f, 0.0f); }

TEST_F(Vector2DTest, TestGetY) { EXPECT_NEAR(testVector.getY(), 1.0f, 0.0f); }

TEST_F(Vector2DTest, TestSetX)
{
    testVector.setX(5.5f);
    EXPECT_NEAR(testVector.getX(), 5.5f, 0.0f);
}

TEST_F(Vector2DTest, TestSetY)
{
    testVector.setY(-3.2f);
    EXPECT_NEAR(testVector.getY(), -3.2f, 0.0f);
}

TEST_F(Vector2DTest, TestInPlaceAdd)
{
    testVector += otherVector;
    EXPECT_NEAR(testVector.getX(), 3.0f, 0.0f);
    EXPECT_NEAR(testVector.getY(), 4.0f, 0.0f);
}

TEST_F(Vector2DTest, TestInPlaceSubtract)
{
    testVector -= otherVector;
    EXPECT_NEAR(testVector.getX(), -1.0f, 0.0f);
    EXPECT_NEAR(testVector.getY(), -2.0f, 0.0f);
}

TEST_F(Vector2DTest, TestInPlaceAddChained)
{
    testVector += otherVector += otherVector;
    EXPECT_NEAR(testVector.getX(), 5.0f, 0.0f);
    EXPECT_NEAR(testVector.getY(), 7.0f, 0.0f);
}

TEST_F(Vector2DTest, TestAdd)
{
    Vector2D result { testVector + otherVector };
    EXPECT_NEAR(result.getX(), 3.0f, 0.0f);
    EXPECT_NEAR(result.getY(), 4.0f, 0.0f);
}

TEST_F(Vector2DTest, TestSubtract)
{
    Vector2D result { testVector - otherVector };
    EXPECT_NEAR(result.getX(), -1.0f, 0.0f);
    EXPECT_NEAR(result.getY(), -2.0f, 0.0f);
}