#include "../headers/Polygon.h"
#include <limits>

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
        return EdgePair{v0, v};
    }
    else 
    {
        return EdgePair{v, v1};
    }
}
