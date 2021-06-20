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
    for (auto& body : Bodies)
    {
        if (body->InvMass)
            continue;

        body->LinearVelocity += dt * (Gravity + body->InvMass * body->Force);
        body->AngularVelocity += dt * body->InvI * body->Torque;
    }

    // Do PreStep
    for (auto& [arbKey, arb] : Arbiters)
    {
        arb.PreStep(invDt);
    }

    // Do iterations i.e. ApplyImpulse over a few iterations
    for (int i = 0; i < Iterations; ++i)
    {
        for (auto const& [arbKey, arb] : Arbiters)
        {
            arb.ApplyImpulse();
        }
    }

    // Integrate velocities
    for (auto& body : Bodies)
    {
        // Euler integration
        body->Position += dt * body->LinearVelocity;
        body->Angle += dt * body->AngularVelocity;

        body->Force = {0.0f,0.0f};
        body->Torque = 0.0f;
    }
}
