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

TEST_F(Vector2DTest, TestGetMagnitude)
{
    Vector2D vector { 3.0f, 4.0f };
    EXPECT_NEAR(vector.getMagnitude(), 5.0f, 0.001f);
}

TEST_F(Vector2DTest, TestGetMagnitudeZero)
{
    Vector2D vector { 0.0f, 0.0f };
    EXPECT_NEAR(vector.getMagnitude(), 0.0f, 0.001f);
}

TEST_F(Vector2DTest, TestSetMagnitude)
{
    Vector2D vector { 3.0f, 4.0f };
    vector.setMagnitude(10.0f);
    EXPECT_NEAR(vector.getMagnitude(), 10.0f, 0.001f);
    EXPECT_NEAR(vector.getX(), 6.0f, 0.001f);
    EXPECT_NEAR(vector.getY(), 8.0f, 0.001f);
}

TEST_F(Vector2DTest, TestSetMagnitudeZeroVector)
{
    Vector2D vector { 0.0f, 0.0f };
    vector.setMagnitude(5.0f);
    EXPECT_NEAR(vector.getX(), 0.0f, 0.001f);
    EXPECT_NEAR(vector.getY(), -5.0f, 0.001f);
}

TEST_F(Vector2DTest, TestSetMagnitudeNegativeAssert)
{
    Vector2D vector { 3.0f, 4.0f };
    EXPECT_DEATH(vector.setMagnitude(-1.0f), "");
}

TEST_F(Vector2DTest, TestRotateTo0Degrees)
{
    Vector2D vector { 5.0f, 0.0f };
    vector.rotateTo(0.0f);
    EXPECT_NEAR(vector.getX(), 0.0f, 0.001f);
    EXPECT_NEAR(vector.getY(), -5.0f, 0.001f);
    EXPECT_NEAR(vector.getAngleDegrees(), 0.0f, 0.001f);
}

TEST_F(Vector2DTest, TestRotateTo90Degrees)
{
    Vector2D vector { 0.0f, -3.0f };
    vector.rotateTo(90.0f);
    EXPECT_NEAR(vector.getX(), 3.0f, 0.001f);
    EXPECT_NEAR(vector.getY(), 0.0f, 0.001f);
    EXPECT_NEAR(vector.getAngleDegrees(), 90.0f, 0.001f);
}

TEST_F(Vector2DTest, TestRotateTo180Degrees)
{
    Vector2D vector { 0.0f, -4.0f };
    vector.rotateTo(180.0f);
    EXPECT_NEAR(vector.getX(), 0.0f, 0.001f);
    EXPECT_NEAR(vector.getY(), 4.0f, 0.001f);
    EXPECT_NEAR(vector.getAngleDegrees(), 180.0f, 0.001f);
}

TEST_F(Vector2DTest, TestRotateTo270Degrees)
{
    Vector2D vector { 2.0f, 0.0f };
    vector.rotateTo(270.0f);
    EXPECT_NEAR(vector.getX(), -2.0f, 0.001f);
    EXPECT_NEAR(vector.getY(), 0.0f, 0.001f);
    EXPECT_NEAR(vector.getAngleDegrees(), 270.0f, 0.001f);
}

TEST_F(Vector2DTest, TestRotateToPreservesMagnitude)
{
    Vector2D vector { 3.0f, 4.0f };
    float originalMagnitude = vector.getMagnitude();
    vector.rotateTo(45.0f);
    EXPECT_NEAR(vector.getMagnitude(), originalMagnitude, 0.001f);
    EXPECT_NEAR(vector.getAngleDegrees(), 45.0f, 0.001f);
}

TEST_F(Vector2DTest, TestRotateTo60Degrees)
{
    Vector2D vector { 0.0f, -2.0f };
    vector.rotateTo(60.0f);
    EXPECT_NEAR(vector.getX(), 1.732f, 0.001f);
    EXPECT_NEAR(vector.getY(), -1.0f, 0.001f);
    EXPECT_NEAR(vector.getAngleDegrees(), 60.0f, 0.001f);
}

TEST_F(Vector2DTest, TestGetAngleDegrees0)
{
    Vector2D vector { 0.0f, -1.0f };
    EXPECT_NEAR(vector.getAngleDegrees(), 0.0f, 0.001f);
}

TEST_F(Vector2DTest, TestGetAngleDegrees90)
{
    Vector2D vector { 1.0f, 0.0f };
    EXPECT_NEAR(vector.getAngleDegrees(), 90.0f, 0.001f);
}

TEST_F(Vector2DTest, TestGetAngleDegrees180)
{
    Vector2D vector { 0.0f, 1.0f };
    EXPECT_NEAR(vector.getAngleDegrees(), 180.0f, 0.001f);
}

TEST_F(Vector2DTest, TestGetAngleDegrees270)
{
    Vector2D vector { -1.0f, 0.0f };
    EXPECT_NEAR(vector.getAngleDegrees(), 270.0f, 0.001f);
}

TEST_F(Vector2DTest, TestGetAngleDegreesRoundTrip)
{
    Vector2D vector { 3.0f, 4.0f };
    float originalAngle = vector.getAngleDegrees();
    vector.rotateTo(originalAngle);
    EXPECT_NEAR(vector.getAngleDegrees(), originalAngle, 0.001f);
}

TEST_F(Vector2DTest, TestSetMagnitudePreservesAngle)
{
    Vector2D vector { 3.0f, 4.0f };
    float originalAngle = vector.getAngleDegrees();
    vector.setMagnitude(10.0f);
    EXPECT_NEAR(vector.getAngleDegrees(), originalAngle, 0.001f);
}