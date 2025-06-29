#include "lunar_lander/Spaceship.h"
#include "lunar_lander/Constants.h"

Spaceship::Spaceship()
    : mNoseAngle { 0 }
    , mPosition { 0, 0 }
    , mVelocity { 0, 0 }
    , mAcceleration { 0, 0 }
    , mJerk { 0, 0 }
    , mGravity { 0, GRAVITY }
    , mTexture { nullptr }
{
}

Spaceship::Spaceship(int x, int y, Texture* texture)
    : mNoseAngle { 0 }
    , mPosition { static_cast<float>(x), static_cast<float>(y) }
    , mVelocity { 0, 0 }
    , mAcceleration { 0, 0 }
    , mJerk { 0, 0 }
    , mGravity { 0, GRAVITY }
    , mTexture { texture }
{
}

void Spaceship::rotate(const int angle) { mNoseAngle += angle; }

void Spaceship::updatePhysics()
{
    mAcceleration = mGravity;
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
    }
    if (bounds.x + bounds.w >= screenWidth)
    {
        mPosition.setX(screenWidth - bounds.w - 1);
    }
    if (bounds.y < 0)
    {
        mPosition.setY(0);
    }
    if (bounds.y + bounds.h >= screenHeight)
    {
        mPosition.setY(screenHeight - bounds.h - 1);
    }
}

void Spaceship::render() const
{
    mTexture->render(static_cast<int>(mPosition.getX()),
        static_cast<int>(mPosition.getY()), NULL, mNoseAngle);
}
