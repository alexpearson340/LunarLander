#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "engine/Vector2D.h"
#include <SDL.h>
#include <engine/Texture.h>

class Spaceship
{
public:
    Spaceship();
    Spaceship(int x, int y, Texture* texture);

    float getNoseAngle() const { return mNoseAngle; };
    void rotate(const float);
    void alignVertical(const float);
    void thrustIncrease();
    void thrustDecay();
    void updatePhysics();
    SDL_Rect getBounds() const;
    void checkBoundaryCollision(int screenWidth, int screenHeight);
    void render() const;

private:
    float mNoseAngle;
    float mThrustUnit; // this is Jerk - time derivative of acceleration
    float mMaxThrust;
    // int mMass { 1 };

    Vector2D mPosition;
    Vector2D mVelocity;
    Vector2D mAcceleration;
    Vector2D mThrust;
    Vector2D mGravity;

    Texture* mTexture;
};

#endif // SPACESHIP_H