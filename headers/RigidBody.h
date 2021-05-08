#pragma once
#include "Vector2.h"
#include "IShape2.h"
#include <memory>

class RigidBody
{
private:
    Vector2 Position;
    Vector2 LinearVelocity;
    float Angle;
    float AngularVelocity;
    Vector2 Force;
    float Torque;
    std::unique_ptr<IShape2> Shape;

public:
    RigidBody() = default;
    // add more constructors and methods 
};