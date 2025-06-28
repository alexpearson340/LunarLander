#include "lunar_lander/Constants.h"
#include "lunar_lander/Spaceship.h"

Spaceship::Spaceship() :
    mNoseAngle { 0 },
    mPosition { 0, 0 },
    mVelocity { 0, 0 },
    mAcceleration { 0, 0 },
    mJerk { 0, 0 },
    mGravity { 0, GRAVITY },
    mTexture { nullptr }
{}

Spaceship::Spaceship(int x, int y, Texture * texture) :
    mNoseAngle { 0 },
    mPosition { static_cast<float>(x), static_cast<float>(y) },
    mVelocity { 0, 0 },
    mAcceleration { 0, 0 },
    mJerk { 0, 0 },
    mGravity { 0, GRAVITY},
    mTexture { texture }
{}

void Spaceship::rotate(const int angle)
{
    mNoseAngle += angle;
}

void Spaceship::updatePhysics()
{
    mAcceleration = mGravity;
    mVelocity += mAcceleration;
    mPosition += mVelocity;
}

void Spaceship::render() const
{
    mTexture->render(static_cast<int>(mPosition.getX()), static_cast<int>(mPosition.getY()), NULL, mNoseAngle);
}