#pragma once
#include "MathsUtils.h"
#include "IShape2.h"
#include "Polygon.h"
#include <memory>

class RigidBody
{
public:
    RigidBody(Vec2 const& position, float mass);
    RigidBody(Vec2 const& position);
    RigidBody(RigidBody const& body);
    // should really define the copy assignment operator and move bois too

    AABB GetAABB();
    void SetSquare(float width);
    void Rotate(float dA);
    void SetRectangle(float width, float height, bool phys = true);

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