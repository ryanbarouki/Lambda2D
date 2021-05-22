#include "../headers/AABB.h"
#include <algorithm>

AABB::AABB() : Perimeter(0) {}

AABB::AABB(Vec2 const& min, Vec2 const& max) : Min(min), Max(max), Perimeter(0) {}

float AABB::CalculatePerimeter()
{
    float width = Max.x - Min.x;
    float height = Max.y - Min.y;
    return 2*width + 2*height;
}

float AABB::GetPerimeter() 
{
    if (Perimeter == 0)
    {
        Perimeter = CalculatePerimeter();
    }
    return Perimeter;
}

bool AABB::Intersects(AABB const& other)
{
    float d1x = other.Min.x - Max.x;
    float d1y = other.Min.y - Max.y;
    float d2x = Min.x - other.Max.x;
    float d2y = Min.y - other.Max.y;

    if (d1x > 0.0f || d1y > 0.0f)
        return false;

    if (d2x > 0.0f || d2y > 0.0f)
        return false;
    
    return true;
}

bool AABB::Contains(AABB const& other)
{
    return Min.x < other.Min.x && Min.y < other.Min.y
        && Max.x > other.Max.x && Max.y > other.Max.y;
}

AABB MergeAABB(AABB const& A, AABB const& B)
{
    float minX = std::min(A.Min.x, B.Min.x);
    float minY = std::min(A.Min.y, B.Min.y);
    float maxX = std::max(A.Max.x, B.Max.x);
    float maxY = std::max(A.Max.y, B.Max.y);

    return AABB(Vec2{minX, minY}, Vec2{maxX, maxY});
}

void AABB::Fatten(Vec2 const& fat)
{
    Min = Min - fat;
    Max = Max + fat;
}