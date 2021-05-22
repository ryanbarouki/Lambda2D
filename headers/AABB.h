#pragma once
#include "Vec2.h"

struct AABB
{
    Vec2 Min;
    Vec2 Max;

    AABB();
    AABB(Vec2 const& min, Vec2 const& max);
    float GetPerimeter();
    bool Intersects(AABB const& other);
    bool Contains(AABB const& other);
    void Fatten(Vec2 const& fat);

private:
    float Perimeter;
    float CalculatePerimeter();
};

AABB MergeAABB(AABB const& A, AABB const& B);