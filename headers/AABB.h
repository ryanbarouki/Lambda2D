#pragma once
#include "Vector2.h"

struct AABB
{
    Vector2 Min;
    Vector2 Max;
};

bool AABBIntersect(AABB const& A, AABB const& B)
{
    float d1x = B.Min.x - A.Max.x;
    float d1y = B.Min.y - A.Max.y;
    float d2x = A.Min.x - B.Max.x;
    float d2y = A.Min.y - B.Max.y;

    if (d1x > 0.0f || d1y > 0.0f)
        return false;

    if (d2x > 0.0f || d2y > 0.0f)
        return false;
    
    return true;
}