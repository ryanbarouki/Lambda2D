#include "../headers/Polygon.h"
#include <limits>
#include <iostream>
#include "RigidBody.h"

Polygon::Polygon(std::vector<Vec2> const& vertices, RigidBody const& body) : Vertices(vertices), Body(body)
{
}

Polygon::Polygon(RigidBody const& body) : Body(body)
{
}

void Polygon::SetSquare(float width)
{
    // is this the correct winding?
    Vec2 h = {width / 2, width / 2};
    Vec2 v1 = {-h.x, -h.y};
    Vec2 v2 = {h.x, -h.y};
    Vec2 v3 = {h.x, h.y};
    Vec2 v4 = {-h.x, h.y};

    Vertices = {v1, v2, v3, v4};
}

std::vector<Vec2> Polygon::GetAxes() const
{
    std::vector<Vec2> axes;
    for (int i = 0; i < Vertices.size(); ++i)
    {
        Vec2 v1 = Vertices[i];
        Vec2 v2 = Vertices[i + 1 == Vertices.size() ? 0 : i + 1];
        
        Vec2 edge = v1 - v2; // the edge vector
        Vec2 axis = edge.Perp(); // perpendicular vector

        axes.push_back(axis.Normalised());
    }

    return axes;
}

std::vector<Vec2> Polygon::GetVertices() const
{
    return Vertices;
}

void Polygon::SetVertices(std::vector<Vec2> newVertices)
{
    Vertices = newVertices;
}

Interval Polygon::Project(Vec2 const& normedAxis) const
{
    float min = normedAxis.Dot(Vertices[0] + Body.Position);
    float max = min;

    for (int i = 1; i < Vertices.size(); ++i)
    {
        float p = normedAxis.Dot(Vertices[i] + Body.Position);
        if (p < min)
        {
            min = p;
        }
        else if (p > max)
        {
            max = p;
        }
    }

    return {min, max};
}


EdgePair Polygon::FindBestEdge(Vec2 const& normal) const
{
    float max = std::numeric_limits<float>::min();
    auto furthestV = Vertices.begin();
    // find the farthest vertex in the polygon along the separation normal
    for (auto it = Vertices.begin(); it != Vertices.end(); ++it)
    {
        float projection = normal.Dot(*it + Body.Position);
        if (projection > max)
        {
            max = projection;
            furthestV = it;
        }
    }
    // std::cout << "Body with mass, " << Body.Mass << " and position, (" << Body.Position.x << ", " << Body.Position.y << ")\n";
    Vec2 v = *furthestV + Body.Position;
    Vec2 v0 = furthestV == Vertices.begin() ? *(Vertices.end() - 1) : *(furthestV - 1); // previous vertex
    v0 += Body.Position;
    Vec2 v1 = furthestV == Vertices.end() - 1? *(Vertices.begin()) : *(furthestV + 1); // previous vertex
    v1 += Body.Position;

    Vec2 l = (v - v1).Normalised();
    Vec2 r = (v - v0).Normalised();

    // retain the winding direction of the vertices v0 -> v -> v1
    if (r.Dot(normal) <= l.Dot(normal))
    {
        return EdgePair{v0, v, v};
    }
    else 
    {
        return EdgePair{v, v1, v};
    }
}

AABB Polygon::GetAABB() const
{
    float x_min = std::numeric_limits<float>::max();
    float y_min = std::numeric_limits<float>::max();
    float x_max = std::numeric_limits<float>::min();
    float y_max = std::numeric_limits<float>::min();
    for (auto const& v : Vertices)
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

    return {{x_min, y_min}, {x_max, y_max}};
}