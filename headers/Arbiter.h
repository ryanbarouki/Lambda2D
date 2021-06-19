#pragma once
#include "RigidBody.h"
#include <vector>

class Arbiter
{
public:
    Arbiter(std::shared_ptr<RigidBody> const& body1, std::shared_ptr<RigidBody> const& body2);
    void ApplyImpulse(float invDt);

private:
    void PreStep(float invDt);
    std::shared_ptr<RigidBody> Body1;
    std::shared_ptr<RigidBody> Body2;
    std::vector<ContactPoint> ContactManifold;
    float Friction;
};