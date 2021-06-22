#pragma once
#include <algorithm>
#include <math.h>

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

struct Mat22
{
	Mat22() {}
	Mat22(float angle)
	{
		float c = cosf(angle), s = sinf(angle);
		col1.x = c; col2.x = -s;
		col1.y = s; col2.y = c;
	}

	Mat22(const Vec2& col1, const Vec2& col2) : col1(col1), col2(col2) {}

	Mat22 Transpose() const
	{
		return Mat22({col1.x, col2.x}, {col1.y, col2.y});
	}

	Mat22 Invert() const
	{
		float a = col1.x, b = col2.x, c = col1.y, d = col2.y;
		Mat22 B;
		float det = a * d - b * c;

        if (det != 0)
        {
            det = 1.0f / det;
        }

		B.col1.x =  det * d;	B.col2.x = -det * b;
		B.col1.y = -det * c;	B.col2.y =  det * a;
		return B;
	}

	Vec2 col1;
    Vec2 col2;
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

inline Vec2 operator*(Mat22 const& R, Vec2 const& U)
{
    return {R.col1.x * U.x + R.col2.x * U.y, R.col1.y * U.x + R.col2.y * U.y};
}