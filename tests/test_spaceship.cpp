#include "engine/Texture.h"
#include "lunar_lander/Spaceship.h"
#include <gtest/gtest.h>

class SpaceshipTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create mock texture with dimensions but nullptr SDL_Texture
        mockTexture.setTexture(nullptr, 32, 32);
        testSpaceship = Spaceship(0, 0, &mockTexture, 0.0f, 2.0f, 100.0f); // Zero gravity, high thrust for tests
    }

    Texture mockTexture;
    Spaceship testSpaceship { 0, 0, nullptr, 0.0f, 2.0f, 100.0f };
};

TEST_F(SpaceshipTest, TestGetDrawBounds)
{
    SDL_Rect bounds = testSpaceship.getDrawBounds();
    EXPECT_EQ(bounds.x, 0);
    EXPECT_EQ(bounds.y, 0);
    EXPECT_EQ(bounds.w, 32);
    EXPECT_EQ(bounds.h, 32);
}

TEST_F(SpaceshipTest, TestGetCollisionBounds)
{
    SDL_Rect bounds = testSpaceship.getCollisionBounds();
    EXPECT_EQ(bounds.x, 4); // drawBounds.x + COLLISION_BOX_MARGIN
    EXPECT_EQ(bounds.y, 4);
    EXPECT_EQ(bounds.w, 24); // 32 minus margin of 4 on both sides
    EXPECT_EQ(bounds.h, 24);
}

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

TEST_F(SpaceshipTest, TestTerrainCollision_TerrainInside)
{
    std::vector<std::vector<int>> terrain(50, std::vector<int>(50, 0));
    terrain[10][10] = 1; // Place terrain within spaceship bounds

    testSpaceship.rotate(135.0f);
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();

    bool collision = testSpaceship.handleTerrainCollision(terrain);

    // Verify collision was detected
    EXPECT_TRUE(collision);

    // Verify velocity was zeroed
    EXPECT_EQ(testSpaceship.getVelX(), 0.0f);
    EXPECT_EQ(testSpaceship.getVelY(), 0.0f);

    // // Verify position was restored to starting position (0,0)
    EXPECT_FLOAT_EQ(testSpaceship.getPosX(), 0.0f);
    EXPECT_FLOAT_EQ(testSpaceship.getPosY(), 0.0f);
}

TEST_F(SpaceshipTest, TestTerrainCollision_NoCollision)
{
    std::vector<std::vector<int>> terrain(50, std::vector<int>(50, 0));

    testSpaceship.rotate(135.0f);
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();

    bool collision = testSpaceship.handleTerrainCollision(terrain);

    // Verify no collision was detected
    EXPECT_FALSE(collision);

    EXPECT_GT(testSpaceship.getVelX(), 0.0f);
    EXPECT_GT(testSpaceship.getVelY(), 0.0f);

    EXPECT_GT(testSpaceship.getPosX(), 0.0f);
    EXPECT_GT(testSpaceship.getPosY(), 0.0f);
}

TEST_F(SpaceshipTest, TestTerrainCollision_RightWallNoCollision)
{
    std::vector<std::vector<int>> terrain(50, std::vector<int>(50, 0));
    terrain[10][32] = 1;
    EXPECT_FALSE(testSpaceship.handleTerrainCollision(terrain));

    // move vertically down
    testSpaceship.rotate(180.0f);
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();

    bool collision = testSpaceship.handleTerrainCollision(terrain);

    // Verify no collision was detected
    EXPECT_FALSE(collision);

    EXPECT_NEAR(testSpaceship.getVelX(), 0.0f, 1e-6);
    EXPECT_GT(testSpaceship.getVelY(), 0.0f);
    EXPECT_NEAR(testSpaceship.getPosX(), 0.0f, 1e-6);
    EXPECT_GT(testSpaceship.getPosY(), 0.0f);
}

TEST_F(SpaceshipTest, TestTerrainCollision_RightWallCollision)
{
    std::vector<std::vector<int>> terrain(50, std::vector<int>(50, 0));
    terrain[10][28] = 1;
    EXPECT_FALSE(testSpaceship.handleTerrainCollision(terrain));

    // move right - hit wall
    testSpaceship.rotate(90.0f);
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();

    bool collision = testSpaceship.handleTerrainCollision(terrain);

    // Verify collision was detected
    EXPECT_TRUE(collision);

    EXPECT_NEAR(testSpaceship.getVelX(), 0.0f, 1e-6);
    EXPECT_NEAR(testSpaceship.getVelY(), 0.0f, 1e-6);
    EXPECT_NEAR(testSpaceship.getPosX(), 0.0f, 1e-6);
    EXPECT_NEAR(testSpaceship.getPosY(), 0.0f, 1e-6);
}

TEST_F(SpaceshipTest, TestTerrainCollision_BottomWallNoCollision)
{
    std::vector<std::vector<int>> terrain(50, std::vector<int>(50, 0));
    terrain[28][10] = 1;
    EXPECT_FALSE(testSpaceship.handleTerrainCollision(terrain));

    testSpaceship.rotate(90.0f);
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();

    bool collision = testSpaceship.handleTerrainCollision(terrain);

    // Verify no collision was detected
    EXPECT_FALSE(collision);

    EXPECT_GT(testSpaceship.getVelX(), 0.0f);
    EXPECT_NEAR(testSpaceship.getVelY(), 0.0f, 1e-6);
    EXPECT_GT(testSpaceship.getPosX(), 0.0f);
    EXPECT_NEAR(testSpaceship.getPosY(), 0.0f, 1e-6);
}

TEST_F(SpaceshipTest, TestTerrainCollision_BottomWallCollision)
{
    std::vector<std::vector<int>> terrain(50, std::vector<int>(50, 0));
    terrain[28][10] = 1;
    EXPECT_FALSE(testSpaceship.handleTerrainCollision(terrain));

    testSpaceship.rotate(180.0f);
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();

    bool collision = testSpaceship.handleTerrainCollision(terrain);

    // Verify collision was detected
    EXPECT_TRUE(collision);

    EXPECT_NEAR(testSpaceship.getVelX(), 0.0f, 1e-6);
    EXPECT_NEAR(testSpaceship.getVelY(), 0.0f, 1e-6);
    EXPECT_NEAR(testSpaceship.getPosX(), 0.0f, 1e-6);
    EXPECT_NEAR(testSpaceship.getPosY(), 0.0f, 1e-6);
}

TEST_F(SpaceshipTest, TestTerrainCollision_DiagonalWallCollision)
{
    std::vector<std::vector<int>> terrain(50, std::vector<int>(50, 0));
    terrain[28][28] = 1;
    EXPECT_FALSE(testSpaceship.handleTerrainCollision(terrain));

    // move right - hit wall
    testSpaceship.rotate(135.0f);
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();

    bool collision = testSpaceship.handleTerrainCollision(terrain);

    // Verify collision was detected
    EXPECT_TRUE(collision);

    EXPECT_NEAR(testSpaceship.getVelX(), 0.0f, 1e-6);
    EXPECT_NEAR(testSpaceship.getVelY(), 0.0f, 1e-6);
    EXPECT_NEAR(testSpaceship.getPosX(), 0.0f, 1e-6);
    EXPECT_NEAR(testSpaceship.getPosY(), 0.0f, 1e-6);
}

// Parameterized test for boundary collision at different orientations
class SpaceshipBoundaryCollisionTest : public SpaceshipTest, public ::testing::WithParamInterface<float>
{
};

TEST_P(SpaceshipBoundaryCollisionTest, TestBoundaryCollisionAtOrientation)
{
    float angle = GetParam();

    // Start spaceship in center of slightly larger world (34x34 vs 32x32 spaceship)
    testSpaceship = Spaceship(1, 1, &mockTexture, 0.0f, 4.0f, 100.0f);

    // Verify no initial collision in 34x34 world
    EXPECT_FALSE(testSpaceship.handleBoundaryCollision(34, 34));

    // Rotate to the test angle and thrust
    testSpaceship.rotate(angle);
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();

    // Check boundary collision with same world dimensions
    bool collision = testSpaceship.handleBoundaryCollision(34, 34);

    // Verify collision was detected
    EXPECT_TRUE(collision);

    EXPECT_NEAR(testSpaceship.getVelX(), 0.0f, 1e-6);
    EXPECT_NEAR(testSpaceship.getVelX(), 0.0f, 1e-6);
}

INSTANTIATE_TEST_SUITE_P(
    OrientationTests,
    SpaceshipBoundaryCollisionTest,
    ::testing::Values(0.0f, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f, 315.0f));

TEST_F(SpaceshipTest, TestBoundaryCollision_VerticalWall_OnlyXVelocityZeroed)
{
    testSpaceship = Spaceship(1, 40, &mockTexture, 0.0f, 4.0f, 100.0f);
    EXPECT_FALSE(testSpaceship.handleBoundaryCollision(100, 100));

    // Create velocity in both X and Y directions
    testSpaceship.rotate(0.0f); // Face up
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();
    EXPECT_FALSE(testSpaceship.handleBoundaryCollision(100, 100));

    testSpaceship.rotate(-90.0f); // Face left
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();

    bool collision = testSpaceship.handleBoundaryCollision(100, 100);

    EXPECT_TRUE(collision);
    EXPECT_NEAR(testSpaceship.getVelX(), 0.0f, 1e-6); // X velocity zeroed (hit horizontal wall)
    EXPECT_LT(testSpaceship.getVelY(), 0.0f); // Y preserved
}

TEST_F(SpaceshipTest, TestBoundaryCollision_HorizontalWall_OnlyYVelocityZeroed)
{
    testSpaceship = Spaceship(40, 1, &mockTexture, 0.0f, 4.0f, 100.0f);
    EXPECT_FALSE(testSpaceship.handleBoundaryCollision(100, 100));

    // Create velocity in both X and Y directions
    testSpaceship.rotate(90.0f); // Face right
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();
    EXPECT_FALSE(testSpaceship.handleBoundaryCollision(100, 100));

    testSpaceship.rotate(-90.0f); // Face up
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();

    bool collision = testSpaceship.handleBoundaryCollision(100, 100);

    EXPECT_TRUE(collision);
    EXPECT_GT(testSpaceship.getVelX(), 0.0f); // X velocity preserved
    EXPECT_NEAR(testSpaceship.getVelY(), 0.0f, 1e-6); // Y velocity zeroed (hit horizontal wall)
}

TEST_F(SpaceshipTest, TestBoundaryCollision_Corner_BothVelocitiesZeroed)
{
    testSpaceship = Spaceship(1, 1, &mockTexture, 0.0f, 4.0f, 100.0f);
    EXPECT_FALSE(testSpaceship.handleBoundaryCollision(40, 40));

    // Create velocity in both X and Y directions
    testSpaceship.rotate(135.0f);
    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();
    EXPECT_FALSE(testSpaceship.handleBoundaryCollision(40, 40));

    testSpaceship.thrustIncrease();
    testSpaceship.updatePhysics();

    bool collision = testSpaceship.handleBoundaryCollision(40, 40);

    EXPECT_TRUE(collision);
    EXPECT_NEAR(testSpaceship.getVelX(), 0.0f, 1e-6); // X velocity zeroed
    EXPECT_NEAR(testSpaceship.getVelY(), 0.0f, 1e-6); // Y velocity zeroed
}