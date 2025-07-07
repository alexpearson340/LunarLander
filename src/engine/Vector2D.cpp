#include "engine/Vector2D.h"
#include <cmath>
#include <cassert>

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

Vector2D& Vector2D::operator*=(const float scalar)
{
    mX *= scalar;
    mY *= scalar;
    return *this;
}

Vector2D& Vector2D::operator/=(const float scalar)
{
    mX /= scalar;
    mY /= scalar;
    return *this;
}

float Vector2D::getMagnitude() const
{
    return std::sqrt(mX * mX + mY * mY);
}

void Vector2D::setMagnitude(float magnitude)
{
    assert(magnitude >= 0.0f);
    float currentMagnitude = getMagnitude();
    if (currentMagnitude > 0.0f)
    {
        float angle = getAngleDegrees();
        float radians = angle * M_PI / 180.0f;
        mX = magnitude * std::sin(radians);
        mY = -magnitude * std::cos(radians);
    }
    // if the vector is currently length zero, we do not know what angle it should be pointing
    // in this case, set the magnitude to point at 0 degrees
    else
    {
        mX = 0.0f;
        mY = -magnitude;
    }
}

void Vector2D::rotateTo(float degrees)
{
    float magnitude = getMagnitude();
    float radians = degrees * M_PI / 180.0f;
    mX = magnitude * std::sin(radians);
    mY = -magnitude * std::cos(radians);
}

float Vector2D::getAngleDegrees() const
{
    float radians = std::atan2(mX, -mY);
    float degrees = radians * 180.0f / M_PI;
    return degrees >= 0.0f ? degrees : degrees + 360.0f;
}