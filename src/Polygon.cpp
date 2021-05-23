#include "../headers/Polygon.h"

std::vector<Vec2> Polygon::GetAxes() const
{
    std::vector<Vec2> axes;
    for (int i = 0; i < Vertices.size(); ++i)
    {
        Vec2 v1 = Vertices[i];
        Vec2 v2 = Vertices[i + 1 == Vertices.size() ? 0 : i + 1];
        
        Vec2 edge = v1 - v2; // the edge vector
        Vec2 axis = edge.Perp(); // perpendicular vector

        axes.push_back(axis.Normalise());
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

bool PolygonsCollide(Polygon const& poly1, Polygon const& poly2)
{
    auto axes1 = poly1.GetAxes();
    auto axes2 = poly2.GetAxes();

    // loop over axes1
    for (auto const& axis : axes1)
    {
        auto p1 = poly1.Project(axis);
        auto p2 = poly2.Project(axis);

       if (!p1.Overlap(p2))
       {
           // if even one axis doesn't overlap then the shapes don't overlap
           return false;
       } 
    }

    // loop over axes 2
    for (auto const& axis : axes2)
    {
        auto p1 = poly1.Project(axis);
        auto p2 = poly2.Project(axis);

       if (!p1.Overlap(p2))
       {
           // if even one axis doesn't overlap then the shapes don't overlap
           return false;
       } 
    }
    return true;
}