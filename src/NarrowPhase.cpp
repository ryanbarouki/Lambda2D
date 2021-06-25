#include "../headers/NarrowPhase.h"
#include <iostream>

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

std::vector<Vec2> NarrowPhase::Clip(Vec2 const& v1, Vec2 const& v2, Vec2 const& normal, float offset)
{
    std::vector<Vec2> cp;
    float d1 = normal.Dot(v1) - offset;
    float d2 = normal.Dot(v2) - offset;
    
    if (d1 >= 0.0f)
    {
        cp.push_back(v1);
    }
    if (d2 >= 0.0f)
    {
        cp.push_back(v2);
    }

    if (d1*d2 < 0.0f)
    {
        Vec2 e = v2 - v1;
        float u = d1 / (d1 - d2);

        e = u * e;
        e = e + v1;
        cp.push_back(e);
    }
    return cp;
}

// returns the contact manifold (list of contact points)
std::vector<ContactPoint> NarrowPhase::FindContactPoints(Polygon const& poly1, Polygon const& poly2)
{
    static int count = 0;
    if (count % 1000 == 0)
    {
        std::cout << "Break\n";
    }
    count++;

    auto pMTV = PolygonsCollide(poly1, poly2);
    if (!pMTV)
    {
        return {};
    }

    Vec2 normal = pMTV->Normalised();
    EdgePair e1 = poly1.FindBestEdge(normal);
    EdgePair e2 = poly2.FindBestEdge(-normal);

    // find reference and incident edges
    EdgePair ref, inc;
    bool flip = false;
    if (abs(e1.GetEdgeVec().Dot(normal)) <= abs(e2.GetEdgeVec().Dot(normal)))
    {
        ref = e1;
        inc = e2;
    }
    else
    {
        ref = e2;
        inc = e1;

        flip = true;
    }

    Vec2 refvDir = ref.GetEdgeVec().Normalised();
    float offset1 = refvDir.Dot(ref.v1);
    auto cp = Clip(inc.v1, inc.v2, refvDir, offset1);

    if (cp.size() < 2) return {};

    float offset2 = refvDir.Dot(ref.v2);
    cp = Clip(cp[0], cp[1], -refvDir, -offset2);

    if (cp.size() < 2) return {};

    Vec2 refNorm = ref.GetEdgeVec().Perp().Normalised();
    // if (flip) refNorm = -refNorm;

    float max = refNorm.Dot(ref.max);

    // make sure the final points are not past the max
    ContactPoint cp1 = {cp[0], normal, refNorm.Dot(cp[0]) - max};
    ContactPoint cp2 = {cp[1], normal, refNorm.Dot(cp[1]) - max};
    std::vector<ContactPoint> cps = {cp1, cp2};
    if (cp1.depth < 0.0f)
    {
        cps.erase(cps.begin());
    }
    if (cp2.depth < 0.0f)
    {
        cps.erase(cps.begin() + 1);
    }

    return cps;
}

