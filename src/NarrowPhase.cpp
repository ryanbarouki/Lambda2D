#include "../headers/NarrowPhase.h"

std::optional<Vec2> NarrowPhase::PolygonsCollide(Polygon const& poly1, Polygon const& poly2)
{
    float minOverlap = std::numeric_limits<float>::max(); // v large number
    Vec2 mtvDirection;
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
           return std::nullopt;
       } 
       else
       {
           float overlap = p1.GetOverlap(p2);

           if (overlap < minOverlap)
           {
               minOverlap = overlap;
               mtvDirection = axis;
           }
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
           return std::nullopt;
       } 
       else
       {
           float overlap = p1.GetOverlap(p2);

           if (overlap < minOverlap)
           {
               minOverlap = overlap;
               mtvDirection = axis;
           }
       }
    }

    Vec2 mtv = minOverlap * mtvDirection.Normalised();
    return mtv;
}

bool NarrowPhase::CirclesCollide(Circle const& A, Circle const& B)
{
    Vec2 vecCentres = A.GetCentre() - B.GetCentre();
    float distanceCentresSq = vecCentres.SquaredLength();
    float sumRadii = A.GetRadius() + B.GetRadius();
    return distanceCentresSq <= sumRadii*sumRadii;
}