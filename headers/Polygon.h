#pragma once
#include "IShape2.h"
#include "MathsUtils.h"
#include <vector>
#include <optional>

// only convex polygons
class Polygon : public IShape2 
{
private:
    Vec2 Centre;
    std::vector<Vec2> Vertices;
    // std::vector<Vec2> Normals;
public:
    Polygon(std::vector<Vec2> const& vertices);
    Polygon() = default;
    void SetSquare(Vec2 const& pos, float width);
    std::vector<Vec2> GetAxes() const;
    std::vector<Vec2> GetVertices() const;
    void SetVertices(std::vector<Vec2> newVertices);
    Interval Project(Vec2 const& axis) const;
    AABB GetAABB() const override;
    EdgePair FindBestEdge(Vec2 const& normal) const;
};