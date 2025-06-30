#include "lunar_lander/Spaceship.h"
#include "lunar_lander/Constants.h"
#include <iostream>
#include <cassert>

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

Spaceship::Spaceship(int x, int y, Texture* texture)
    : mNoseAngle { 0 }
    , mThrustUnit { THRUST_UNIT }
    , mMaxThrust { MAX_THRUST }
    , mPosition { static_cast<float>(x), static_cast<float>(y) }
    , mVelocity { 0, 0 }
    , mAcceleration { 0, 0 }
    , mThrust { 0, 0 }
    , mGravity { 0, GRAVITY }
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

SDL_Rect Spaceship::getBounds() const
{
    return { static_cast<int>(mPosition.getX()),
        static_cast<int>(mPosition.getY()), mTexture->getWidth(),
        mTexture->getHeight() };
}

void Spaceship::checkBoundaryCollision(int screenWidth, int screenHeight)
{
    SDL_Rect bounds = getBounds();
    if (bounds.x < 0)
    {
        mPosition.setX(0);
        mVelocity.setX(0);
    }
    if (bounds.x + bounds.w >= screenWidth)
    {
        mPosition.setX(screenWidth - bounds.w - 1);
        mVelocity.setX(0);
    }
    if (bounds.y < 0)
    {
        mPosition.setY(0);
        mVelocity.setY(0);
    }
    if (bounds.y + bounds.h >= screenHeight)
    {
        mPosition.setY(screenHeight - bounds.h - 1);
        mVelocity.setY(0);
    }
}

void Spaceship::render() const
{
    mTexture->render(static_cast<int>(mPosition.getX()),
        static_cast<int>(mPosition.getY()), NULL, static_cast<double>(mNoseAngle));
}
