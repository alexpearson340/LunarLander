#include "lunar_lander/Spaceship.h"
#include "lunar_lander/Constants.h"
#include <cassert>
#include <iostream>

Spaceship::Spaceship()
    : mNoseAngle { 0 }
    , mThrustUnit { THRUST_UNIT }
    , mMaxThrust { MAX_THRUST }
    , mPosition { 0, 0 }
    , mVelocity { 0, 0 }
    , mAcceleration { 0, 0 }
    , mThrust { 0, 0 }
    , mGravity { 0, GRAVITY }
    , mTexture { nullptr }
{
}

Spaceship::Spaceship(int x, int y, Texture* texture, float gravity, float thrustUnit, float maxThrust)
    : mNoseAngle { 0 }
    , mThrustUnit { thrustUnit }
    , mMaxThrust { maxThrust }
    , mPosition { static_cast<float>(x), static_cast<float>(y) }
    , mVelocity { 0, 0 }
    , mAcceleration { 0, 0 }
    , mThrust { 0, 0 }
    , mGravity { 0, gravity }
    , mTexture { texture }
{
}

void Spaceship::rotate(const float angle)
{
    assert(angle >= -360.0f && angle <= 360.0f);
    mNoseAngle += angle;
    if (mNoseAngle >= 360.0f)
    {
        mNoseAngle -= 360.0f;
    }
    if (mNoseAngle < 0.0f)
    {
        mNoseAngle += 360.0f;
    }
    assert(mNoseAngle >= 0.0f && mNoseAngle < 360.0f);
    mThrust.rotateTo(mNoseAngle);
}

void Spaceship::alignVertical(const float angle)
{
    assert(mNoseAngle >= 0.0f && mNoseAngle < 360.0f);
    if (std::fabs(mNoseAngle) <= angle)
    {
        rotate(-mNoseAngle);
    }
    else if (mNoseAngle > 180.0f)
    {
        rotate(std::fabs(angle));
    }
    else
    {
        rotate(-std::fabs(angle));
    }
}

void Spaceship::thrustIncrease()
{
    float currentMagnitude { mThrust.getMagnitude() };

    // handling for small / zero thrust
    if (currentMagnitude < mThrustUnit)
    {
        mThrust.setMagnitude(mThrustUnit);
        mThrust.rotateTo(mNoseAngle);
    }
    else
    {
        mThrust.setMagnitude(currentMagnitude + mThrustUnit);
    }

    // limit thrust to the max thrust
    if (mThrust.getMagnitude() > mMaxThrust)
    {
        mThrust.setMagnitude(mMaxThrust);
    }
}

void Spaceship::thrustDecay()
{
    float currentMagnitude { mThrust.getMagnitude() };
    if (currentMagnitude > mThrustUnit)
    {
        mThrust.setMagnitude(currentMagnitude - mThrustUnit);
    }
    else
    {
        mThrust.setMagnitude(0);
    }

    // the minimum thrust is zero
    if (mThrust.getMagnitude() < 0)
    {
        mThrust.setMagnitude(0);
    }
}

void Spaceship::updatePhysics()
{
    mAcceleration = mGravity + mThrust;
    mVelocity += mAcceleration;
    mPosition += mVelocity;
}

SDL_Rect Spaceship::getDrawBounds() const
{
    return {
        static_cast<int>(mPosition.getX()),
        static_cast<int>(mPosition.getY()),
        mTexture->getWidth(),
        mTexture->getHeight()
    };
}

SDL_Rect Spaceship::getCollisionBounds() const
{
    SDL_Rect drawBounds { getDrawBounds() };
    return {
        drawBounds.x + COLLISION_BOX_MARGIN,
        drawBounds.y + COLLISION_BOX_MARGIN,
        mTexture->getWidth() - (2 * COLLISION_BOX_MARGIN),
        mTexture->getHeight() - (2 * COLLISION_BOX_MARGIN)
    };
}

bool Spaceship::handleBoundaryCollision(int worldWidth, int worldHeight)
{
    SDL_Rect bounds = getDrawBounds();
    bool collision = false;

    if (bounds.x < 0)
    {
        mPosition.setX(0);
        mVelocity.setX(0);
        collision = true;
    }
    if (bounds.x + bounds.w >= worldWidth)
    {
        mPosition.setX(worldWidth - bounds.w - 1);
        mVelocity.setX(0);
        collision = true;
    }
    if (bounds.y < 0)
    {
        mPosition.setY(0);
        mVelocity.setY(0);
        collision = true;
    }
    if (bounds.y + bounds.h >= worldHeight)
    {
        mPosition.setY(worldHeight - bounds.h - 1);
        mVelocity.setY(0);
        collision = true;
    }

    return collision;
}

bool Spaceship::handleTerrainCollision(std::vector<std::vector<int>>& terrain)
{
    SDL_Rect bounds { getCollisionBounds() };
    int startX { std::max(0, bounds.x) };
    int startY { std::max(0, bounds.y) };
    int endX { std::min(static_cast<int>(terrain.at(0).size()), (bounds.x + bounds.w)) };
    int endY { std::min(static_cast<int>(terrain.size()), (bounds.y + bounds.h)) };

    for (int y = startY; y != endY; y++)
    {
        for (int x = startX; x != endX; x++)
        {
            if (terrain[static_cast<size_t>(y)][static_cast<size_t>(x)] == TERRAIN_ROCK)
            {
                Vector2D oppositeVelocity { mVelocity * -1.0f };
                mPosition += oppositeVelocity;
                mVelocity.setX(0);
                mVelocity.setY(0);
                return true;
            }
        }
    }

    return false;
}

FlightStats Spaceship::getFlightStats() const
{
    return FlightStats {
        mNoseAngle,
        mPosition.getX(),
        mPosition.getY(),
        mVelocity.getX(),
        mVelocity.getY(),
        mAcceleration.getX(),
        mAcceleration.getY(),
        mThrust.getMagnitude()
    };
}

void Spaceship::render(const int x, const int y) const
{
    mTexture->render(x, y, NULL, static_cast<double>(mNoseAngle));
}
