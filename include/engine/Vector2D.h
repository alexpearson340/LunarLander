#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2D
{
public:

    Vector2D(const int x, const int y);

    int getX() const { return mX; }
    int getY() const { return mY; }
    
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);

private:
    int mX;
    int mY;
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