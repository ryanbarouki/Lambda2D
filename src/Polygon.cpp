#include "../headers/Polygon.h"
#include <limits>

Polygon::Polygon(std::vector<Vec2> const& vertices) : Vertices(vertices)
{
}

void Polygon::SetSquare(Vec2 const& pos, float width)
{
    // is this the correct winding?
    Vec2 h = {width, width};
    Vec2 v1 = pos;
    Vec2 v2 = {pos.x + width, pos.y};
    Vec2 v3 = pos + h;
    Vec2 v4 = {pos.x, pos.y + width};

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
    float min = normedAxis.Dot(Vertices[0]);
    float max = min;

    for (int i = 0; i < Vertices.size(); ++i)
    {
        float p = normedAxis.Dot(Vertices[i]);
        if (p < min)
        {
            min = p;
        }
        else if (p > max)
        {
            max = p;
        }
    }

    return Interval{min, max};
}


EdgePair Polygon::FindBestEdge(Vec2 const& normal) const
{
    float max = std::numeric_limits<float>::min();
    auto furthestV = Vertices.begin();
    // find the farthest vertex in the polygon along the separation normal
    for (auto it = Vertices.begin(); it != Vertices.end(); ++it)
    {
        float projection = normal.Dot(*it);
        if (projection > max)
        {
            max = projection;
            furthestV = it;
        }
    }

    Vec2 v = *furthestV;
    Vec2 v0 = *(--furthestV); // previous vertex
    Vec2 v1 = *(++furthestV); // next vertex

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