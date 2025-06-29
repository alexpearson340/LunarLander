#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2D
{
public:

    Vector2D(const float x, const float y);

    float getX() const { return mX; }
    float getY() const { return mY; }
    float getMagnitude() const;
    float getAngleDegrees() const;

    void setX(const float x) { mX = x; };
    void setY(const float y) { mY = y; };
    void setMagnitude(float magnitude);
    void rotateTo(float degrees);
    
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);

private:
    float mX;
    float mY;
};

inline Vector2D operator+(const Vector2D& left, const Vector2D& right)
{
    Vector2D result { left };
    result += right;
    return result;
}

inline Vector2D operator-(const Vector2D& left, const Vector2D& right)
{
    Vector2D result { left };
    result -= right;
    return result;
}

#endif // VECTOR2D_H