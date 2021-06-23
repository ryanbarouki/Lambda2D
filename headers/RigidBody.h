#pragma once
#include "MathsUtils.h"
#include "IShape2.h"
#include "Polygon.h"
#include <memory>

class RigidBody
{
public:
    RigidBody() : Shape(std::make_unique<Polygon>(*this))
    {
    }

    RigidBody(std::unique_ptr<IShape2> shape, float mass) : Shape(std::move(shape)), Mass(mass)
    {
        InvMass = mass != 0 ? 1.0f / mass : 0.0f;
    }
    // add more constructors and methods 
    AABB GetAABB();
    void SetSquare(float width);
    void Rotate(float dA);

    Vec2 Position = {0.0f, 0.0f};
    Vec2 LinearVelocity = {0.0f, 0.0f};
    float Mass = std::numeric_limits<float>::max();
    float InvMass = 0.0f;
    float Angle = 0.0f;
    float AngularVelocity = 0.0f;
    float I = std::numeric_limits<float>::max();
    float InvI = 0.0f;
    Vec2 Force = {0.0f, 0.0f};
    float Torque = 0.0f;
    float Friction = 0.2f;
    AABB Aabb;
    std::unique_ptr<IShape2> Shape;
};