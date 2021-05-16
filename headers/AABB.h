#pragma once
#include "Vector2.h"

struct AABB
{
    Vector2 Min;
    Vector2 Max;

    AABB();
    AABB(Vector2 const& min, Vector2 const& max);
    float GetPerimeter();
    bool Intersects(AABB const& B);

private:
    float Perimeter;
    float CalculatePerimeter();
};

AABB MergeAABB(AABB const& A, AABB const& B);