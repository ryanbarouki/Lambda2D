#include "../headers/MathsUtils.h"
#include <cmath>
#include <cassert>

bool Interval::Overlap(Interval const& I) const
{
    float d1 = Min - I.Max;     
    float d2 = I.Min - Max; 

    if (d1 > 0.0f || d2 > 0.0f)
        return false;

    return true;
}

float Interval::GetOverlap(Interval const& I) const
{
    assert(Overlap(I));

    float minMax = std::min(Max, I.Max);
    float maxMin = std::max(Min, I.Min);

    return minMax - maxMin;
}

Vec2 operator+(Vec2 const& U, Vec2 const& V)
{
    return Vec2{U.x + V.x, U.y + V.y};
}

Vec2 operator-(Vec2 const& U, Vec2 const& V)
{
    return Vec2{U.x - V.x, U.y - V.y};
}

Vec2 operator-(Vec2 const& U)
{
    return Vec2{-U.x, -U.y};
}

Vec2 operator*(float scalar, Vec2 const& U)
{
    return Vec2{scalar * U.x, scalar * U.y};
}

void operator+=(Vec2& U, Vec2 const& V)
{
    U = U + V;
}

void operator-=(Vec2& U, Vec2 const& V)
{
    U = U - V;
}

std::ostream& operator<<(std::ostream& stream, Vec2 const& U)
{
    stream << "(" << U.x << ", " << U.y << ")\n";
    return stream;
}

float Vec2::SquaredLength() const
{
    return x*x + y*y;
}

float Vec2::Length() const
{
    return sqrt(x*x + y*y);
}

float Vec2::Dot(Vec2 const& U) const
{
    return x * U.x + y * U.y;
}

Vec2 Vec2::Perp() const
{
    return Vec2{-y, x};
}

Vec2 Vec2::Normalised() const
{
    float length = Length();
    return length > 0 ? Vec2{x / length, y / length} : Vec2{0,0};
}

// points from v1 to v2;
Vec2 EdgePair::GetEdgeVec()
{
    return v2 - v1;
}