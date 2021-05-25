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
    float min = Dot(normedAxis, Vertices[0]);
    float max = min;

    for (int i = 0; i < Vertices.size(); ++i)
    {
        float p = Dot(normedAxis, Vertices[i]);
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
