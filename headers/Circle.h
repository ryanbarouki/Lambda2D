#pragma once
#include "IShape2.h"
#include "Vec2.h"

class Circle : public IShape2 
{
    Vec2 Centre;
    float Radius;

public:
    Circle() = default;
    Circle(Vec2 const& centre, float radius);
    AABB GetAABB() const override;
    friend bool CirclesCollide(Circle const& A, Circle const& B);
};
