#include "../headers/World.h"

void World::Add(std::shared_ptr<RigidBody> const& body)
{
    Bodies.push_back(body);
}

void World::Step(float dt)
{
    float invDt = dt > 0.0f ? 1.0f / dt : 0.0f;

    // Do the broadphase here

    // Integrate forces - do we need this at first?

    // Do PreStep

    // Do iterations i.e. ApplyImpulse over a few iterations

    // Integrate velocities
}
