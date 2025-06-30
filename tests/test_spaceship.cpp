#include "lunar_lander/Spaceship.h"
#include <gtest/gtest.h>

class SpaceshipTest : public ::testing::Test
{
protected:
    Spaceship testSpaceship { 0, 0, nullptr };
};

TEST_F(SpaceshipTest, TestRotateBasic)
{
    testSpaceship.rotate(45.0f);
    EXPECT_FLOAT_EQ(testSpaceship.getNoseAngle(), 45.0f);
}

TEST_F(SpaceshipTest, TestRotateWrappingAt360)
{
    testSpaceship.rotate(45.0f);
    testSpaceship.rotate(320.0f);
    EXPECT_FLOAT_EQ(testSpaceship.getNoseAngle(), 5.0f);
}

TEST_F(SpaceshipTest, TestRotateNegative)
{
    testSpaceship.rotate(-45.0f);
    EXPECT_FLOAT_EQ(testSpaceship.getNoseAngle(), 315.0f);
}

TEST_F(SpaceshipTest, TestRotateMultipleFullRotations)
{
    testSpaceship.rotate(45.0f);
    testSpaceship.rotate(360.0f);
    testSpaceship.rotate(360.0f);
    EXPECT_FLOAT_EQ(testSpaceship.getNoseAngle(), 45.0f);
}

TEST_F(SpaceshipTest, TestAlignVerticalWhenClose)
{
    testSpaceship.rotate(5.0f);
    testSpaceship.alignVertical(10.0f);
    EXPECT_FLOAT_EQ(testSpaceship.getNoseAngle(), 0.0f);
}

TEST_F(SpaceshipTest, TestAlignVerticalFromSmallPositiveAngle)
{
    testSpaceship.rotate(30.0f);
    testSpaceship.alignVertical(10.0f);
    EXPECT_FLOAT_EQ(testSpaceship.getNoseAngle(), 20.0f);
}

TEST_F(SpaceshipTest, TestAlignVerticalFromSmallNegativeAngle)
{
    testSpaceship.rotate(-40.0f);
    testSpaceship.alignVertical(10.0f);
    EXPECT_FLOAT_EQ(testSpaceship.getNoseAngle(), 330.0f);
}

TEST_F(SpaceshipTest, TestAlignVerticalShortestPathFrom270)
{
    testSpaceship.rotate(270.0f);
    testSpaceship.alignVertical(10.0f);
    EXPECT_FLOAT_EQ(testSpaceship.getNoseAngle(), 280.0f);
}

TEST_F(SpaceshipTest, TestAlignVerticalShortestPathFrom90)
{
    testSpaceship.rotate(90.0f);
    testSpaceship.alignVertical(10.0f);
    EXPECT_FLOAT_EQ(testSpaceship.getNoseAngle(), 80.0f);
}

TEST_F(SpaceshipTest, TestAlignVerticalBoundaryAt180)
{
    testSpaceship.rotate(180.0f);
    testSpaceship.alignVertical(10.0f);
    EXPECT_FLOAT_EQ(testSpaceship.getNoseAngle(), 170.0f);
}

TEST_F(SpaceshipTest, TestAlignVerticalBoundaryJustOver180)
{
    testSpaceship.rotate(181.0f);
    testSpaceship.alignVertical(10.0f);
    EXPECT_FLOAT_EQ(testSpaceship.getNoseAngle(), 191.0f);
}