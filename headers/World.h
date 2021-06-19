#pragma once
#include "RigidBody.h"
#include "Arbiter.h"
#include <memory>
#include <vector>
#include <map>

class World
{
public:
    World(Vec2 const& gravity, float iterations) : Gravity(gravity), Iterations(iterations)
    {}

    void Step(float dt);
    void Add(std::shared_ptr<RigidBody> const& body);

private:
    Vec2 Gravity;
    float Iterations;
    std::vector<std::shared_ptr<RigidBody>> Bodies;
    std::map<ArbiterKey, Arbiter> Arbiters;
};