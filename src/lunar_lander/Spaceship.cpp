#include "lunar_lander/Spaceship.h"

Spaceship::Spaceship() :
    mNoseAngle { 0 },
    mPosition { 0, 0 },
    mVelocity { 0, 0 },
    mAcceleration { 0, 0 },
    mJerk { 0, 0 },
    mTexture { nullptr }
{}

Spaceship::Spaceship(int x, int y, Texture * texture) :
    mNoseAngle { 0 },
    mPosition { x, y },
    mVelocity { 0, 0 },
    mAcceleration { 0, 0 },
    mJerk { 0, 0 },
    mTexture { texture }
{}

void Spaceship::render() const
{
    mTexture->render(mPosition.getX(), mPosition.getY(), NULL, mNoseAngle);
}