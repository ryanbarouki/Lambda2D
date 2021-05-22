#include "../headers/Circle.h"

bool CirclesCollide(Circle const& A, Circle const& B)
{
    Vec2 vecCentres = A.Centre - B.Centre;
    float distanceCentresSq = vecCentres.SquaredLength();
    float sumRadii = A.Radius + B.Radius;
    return distanceCentresSq <= sumRadii*sumRadii;
}