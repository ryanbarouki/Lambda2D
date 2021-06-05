#pragma once
#include "IShape2.h"
#include "Polygon.h"
#include "Circle.h"
#include <vector>
#include <memory>
#include <optional>

using ColliderPair = std::pair<std::shared_ptr<IShape2>, std::shared_ptr<IShape2>>;
using ColliderPairList = std::vector<ColliderPair>;

class NarrowPhase
{
public:
    ColliderPairList BroadPhasePairs;

    bool CirclesCollide(Circle const& A, Circle const& B);
    // Narrow phase collision detection for polygons - Separating Axis Theorem
    // returns the minimum translation vector
    // containment is not handled yet
    std::optional<Vec2> PolygonsCollide(Polygon const& poly1, Polygon const& poly2);
    
    // more methods to do with clipping after returning the MTV
    // see https://dyn4j.org/2011/11/contact-points-using-clipping/
    std::vector<Vec2> FindContactPoints(Polygon const& poly1, Polygon const& poly2, Vec2 const& normal);

private:
    std::vector<Vec2> Clip(Vec2 const& v1, Vec2 const& v2, Vec2 const& normal, float offset);
};