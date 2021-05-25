#pragma once
#include "MathsUtils.h"

struct AABB
{
    Vec2 Min;
    Vec2 Max;

    AABB();
    AABB(Vec2 const& min, Vec2 const& max);
    float GetPerimeter();
    bool Intersects(AABB const& other) const;
    bool Contains(AABB const& other) const;
    void Fatten(Vec2 const& fat);

private:
    float Perimeter;
    float CalculatePerimeter() const;
};

AABB MergeAABB(AABB const& A, AABB const& B);