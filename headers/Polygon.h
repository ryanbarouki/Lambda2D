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
    // std::vector<Vec2> Normals;
public:
    Polygon() = default;
    std::vector<Vec2> GetAxes() const;
    Interval Project(Vec2 const& axis) const;
    AABB GetAABB() const override;

    // Narrow phase collision detection for polygons - Separating Axis Theorem
    friend bool PolygonsCollide(Polygon const& poly1, Polygon const& poly2);
};