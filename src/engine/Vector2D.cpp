#include "engine/Vector2D.h"

Vector2D::Vector2D(float x, float y) :
    mX { x },
    mY { y }
    {};

Vector2D& Vector2D::operator+=(const Vector2D& other)
{
    mX += other.mX;
    mY += other.mY;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& other)
{
    mX -= other.mX;
    mY -= other.mY;
    return *this;
}