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

struct ContactPoint
{
    Vec2 point;
    Vec2 normal;
    float depth;
    float massNormal;
    float massTangent;
    float bias = 0.0f;
};

// vector operations
Vec2 operator+(Vec2 const& U, Vec2 const& V);
Vec2 operator-(Vec2 const& U, Vec2 const& V);
Vec2 operator-(Vec2 const& U);
Vec2 operator*(float scalar, Vec2 const& U);
void operator+=(Vec2& U, Vec2 const& V);
void operator-=(Vec2& U, Vec2 const& V);

inline float Cross(Vec2 const& a, Vec2 const& b)
{
	return a.x * b.y - a.y * b.x;
}

inline Vec2 Cross(Vec2 const& a, float s)
{
	return {s * a.y, -s * a.x};
}

inline Vec2 Cross(float s, Vec2 const& a)
{
	return {-s * a.y, s * a.x};
}

inline float Clamp(float a, float low, float high)
{
	return std::max(low, std::min(a, high));
}