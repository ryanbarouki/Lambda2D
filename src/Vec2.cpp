#include "../headers/Vec2.h"

Vec2 operator+(Vec2 const& U, Vec2 const& V)
{
    return Vec2{U.x + V.x, U.y + V.y};
}

Vec2 operator-(Vec2 const& U, Vec2 const& V)
{
    return Vec2{U.x - V.x, U.y - V.y};
}

Vec2 operator*(float scalar, Vec2 const& U)
{
    return Vec2{scalar * U.x, scalar * U.y};
}