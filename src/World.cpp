#include "../headers/World.h"
#include <iostream>

void World::Add(RigidBody const& body)
{
    auto bodyPtr = std::make_shared<RigidBody>(body);
    Bodies.push_back(bodyPtr);
    int bodyIndex = CollisionTree.Insert(bodyPtr);
    BodyIndices.insert({bodyPtr, bodyIndex});
}

void World::Step(float dt)
{
    float invDt = dt > 0.0f ? 1.0f / dt : 0.0f;

    // Do the broadphase here
    BroadPhase();

    // Integrate forces - do we need this at first?
    for (auto& body : Bodies)
    {
        if (body->InvMass == 0.0f)
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
        body->Rotate(dt * body->AngularVelocity);
        body->Force = {0.0f,0.0f};
        body->Torque = 0.0f;
    }
}

void World::BroadPhase()
{
    for (auto const& body : Bodies)
    {
        int bodyIndex = BodyIndices[body];
        if (bodyIndex == nullNode)
        {
            continue;
        }

        auto const& fatAABB = CollisionTree.GetFatAABB(bodyIndex);

        // TODO: Make sure this is the best place for it   
        CollisionTree.UpdateLeaf(bodyIndex, body->GetAABB(), {0,0});
        auto overlaps = CollisionTree.Query(bodyIndex); // can this be done more efficiently?

        for (auto const& overlap : overlaps)
        {
            auto overlapBody = CollisionTree.GetNode(overlap).Object;
            Arbiter newArb(body, overlapBody);
            ArbiterKey arbKey(body, overlapBody);
            
            if (newArb.InContact())
            {
                auto it = Arbiters.find(arbKey);
                if (it == Arbiters.end())
                {
                    Arbiters.insert({arbKey, newArb});
                }
                else
                {
                    // update existing Arbiter pair
                    // for now just do the same
                    Arbiters[arbKey] = newArb;
                    // Arbiters.insert(std::make_pair(arbKey, newArb));
                }
            }
            else
            {
                // not in contact so remove from Arbiters
                Arbiters.erase(arbKey);
            }
        }
    }
}