#pragma once
#include "MathsUtils.h"
#include "IShape2.h"
#include <memory>

class RigidBody
{
public:
    RigidBody() = default;
    // add more constructors and methods 

    Vec2 Position;
    Vec2 LinearVelocity;
    float Mass;
    float InvMass;
    float Angle;
    float AngularVelocity;
    float I;
    float InvI;
    Vec2 Force;
    float Torque;
    float Friction;
    std::unique_ptr<IShape2> Shape;
};