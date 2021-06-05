#pragma once

struct Interval
{
    float Min;
    float Max;

    bool Overlap(Interval const& I) const;
    float GetOverlap(Interval const& I) const; 
};

struct Vec2 
{
    float x;
    float y;

    float SquaredLength() const;
    float Length() const;
    Vec2 Perp() const;
    Vec2 Normalised() const;
    float Dot(Vec2 const& U) const;
};

struct EdgePair
{
    // pair of vertices
    Vec2 v1;
    Vec2 v2;
    Vec2 max;

    Vec2 GetEdgeVec();
};

// vector operations
Vec2 operator+(Vec2 const& U, Vec2 const& V);
Vec2 operator-(Vec2 const& U, Vec2 const& V);
Vec2 operator-(Vec2 const& U);
Vec2 operator*(float scalar, Vec2 const& U);