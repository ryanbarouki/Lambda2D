#pragma once
#include "IShape2.h"
#include "MathsUtils.h"

class Circle : public IShape2 
{
    Vec2 Centre;
    float Radius;

public:
    Circle() = default;
    Circle(Vec2 const& centre, float radius);

    Vec2 GetCentre() const { return Centre; }
    float GetRadius() const { return Radius; }
    AABB GetAABB() const override;
};
