#include "../headers/RigidBody.h"
#include "Polygon.h"


AABB RigidBody::GetAABB() 
{
    // only works for polgons: need to figure this out for general shapes
    auto& polygon = dynamic_cast<Polygon&>(*Shape);
    auto vertices = polygon.GetVertices();
    float x_min = std::numeric_limits<float>::max();
    float y_min = std::numeric_limits<float>::max();
    float x_max = std::numeric_limits<float>::min();
    float y_max = std::numeric_limits<float>::min();
    for (auto const& v : vertices)
    {
        if (v.x < x_min)
            x_min = v.x;
        if (v.x > x_max)
            x_max = v.x;
        if (v.y < y_min)
            y_min = v.y;
        if (v.y > y_max)
            y_max = v.y;
    }

    return {Position + Vec2{x_min, y_min}, Position + Vec2{x_max, y_max}};
}

void RigidBody::SetSquare(float width)
{
    auto pPolygon = dynamic_cast<Polygon*>(Shape.get());
    if (pPolygon)
    {
        pPolygon->SetSquare(width);
    }
}

void RigidBody::Rotate(float dA)
{
    Angle += dA;
    Mat22 R(dA);
    if (auto pPolygon = dynamic_cast<Polygon*>(Shape.get()))
    {
        std::vector<Vec2> newVertices;
        for (auto const& v : pPolygon->GetVertices())
        {
            newVertices.push_back(R * v);     
        }

        pPolygon->SetVertices(newVertices);
    }
}