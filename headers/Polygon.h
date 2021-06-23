#pragma once
#include "IShape2.h"
#include "MathsUtils.h"
#include <vector>
#include <optional>
#include <memory>

class RigidBody;
// only convex polygons
class Polygon : public IShape2 
{
private:
    RigidBody const& Body;
    std::vector<Vec2> Vertices;

public:
    Polygon(std::vector<Vec2> const& vertices, RigidBody const& body);
    Polygon(RigidBody const& body);
    Polygon() = default;
    void SetSquare(float width);
    std::vector<Vec2> GetAxes() const;
    std::vector<Vec2> GetVertices() const;
    void SetVertices(std::vector<Vec2> newVertices);
    Interval Project(Vec2 const& axis) const;
    AABB GetAABB() const override;
    EdgePair FindBestEdge(Vec2 const& normal) const;
};