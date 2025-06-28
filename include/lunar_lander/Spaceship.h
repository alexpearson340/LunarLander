#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "engine/Vector2D.h"
#include <engine/Texture.h>

class Spaceship
{
public:
    Spaceship();
    Spaceship(int x, int y, Texture * texture);

    void rotate(const int);
    void render() const;

private:
    int mNoseAngle;
    // int mMass { 1 };
    
    Vector2D mPosition;
    Vector2D mVelocity;
    Vector2D mAcceleration;
    Vector2D mJerk;

    Texture* mTexture;
};

#endif // SPACESHIP_H