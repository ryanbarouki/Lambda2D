#pragma once

struct Vec2 
{
    float x;
    float y;

    float SquaredLength();
    float Length();
};

Vec2 operator+(Vec2 const& U, Vec2 const& V);
Vec2 operator-(Vec2 const& U, Vec2 const& V);
Vec2 operator*(float scalar, Vec2 const& U);