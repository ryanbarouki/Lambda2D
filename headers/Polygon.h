#pragma once
#include "IShape2.h"
#include "Vec2.h"
#include <vector>

// only convex polygons
class Polygon : public IShape2 
{
private:
    Vec2 Centre;
    std::vector<Vec2> Vertices;
    std::vector<Vec2> Normals;
public:
    Polygon() = default;
    AABB GetAABB() const override;
};