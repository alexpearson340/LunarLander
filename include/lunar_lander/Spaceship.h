#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "engine/Vector2D.h"
#include "lunar_lander/FlightStats.h"
#include <SDL.h>
#include <engine/Texture.h>
#include <vector>

const int COLLISION_BOX_MARGIN { 4 };

class Spaceship
{
public:
    Spaceship();
    Spaceship(int x, int y, Texture* texture, float gravity, float thrustUnit, float maxThrust);

    float getPosX() const { return mPosition.getX(); };
    float getPosY() const { return mPosition.getY(); };
    float getVelX() const { return mVelocity.getX(); };
    float getVelY() const { return mVelocity.getY(); };
    float getNoseAngle() const { return mNoseAngle; };

    FlightStats getFlightStats() const;
    void rotate(const float);
    void alignVertical(const float);
    void thrustIncrease();
    void thrustDecay();
    void updatePhysics();
    SDL_Rect getDrawBounds() const;
    SDL_Rect getCollisionBounds() const;
    bool checkBoundaryCollision(int, int);
    bool checkTerrainCollision(std::vector<std::vector<int>>&);
    void render(const int, const int) const;

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