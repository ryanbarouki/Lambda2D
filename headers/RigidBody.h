#pragma once
#include "Vec2.h"
#include "IShape2.h"
#include <memory>

class RigidBody
{
private:
    Vec2 Position;
    Vec2 LinearVelocity;
    float Mass;
    float Angle;
    float AngularVelocity;
    float MomentOfIntertia;
    Vec2 Force;
    float Torque;
    std::unique_ptr<IShape2> Shape;

public:
    RigidBody() = default;
    // add more constructors and methods 
};