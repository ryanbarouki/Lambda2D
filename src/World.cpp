#include "../headers/World.h"
#include <iostream>

// feature flags
bool World::accumulateImpulses = true;
bool World::warmStarting = true;
bool World::positionCorrection = true;
bool World::goodBroadPhase = true;

void World::Add(RigidBody const& body)
{
    auto bodyPtr = std::make_shared<RigidBody>(body);
    Bodies.push_back(bodyPtr);
    if (World::goodBroadPhase)
    {
        int bodyIndex = CollisionTree.Insert(bodyPtr);
        BodyIndices.insert({bodyPtr, bodyIndex});
    }
} 
    
void World::Step(float dt)
{
    float invDt = dt > 0.0f ? 1.0f / dt : 0.0f;

    // Do the broadphase here
    if (World::goodBroadPhase)
    {
        BroadPhase();
    }
    else
    {
        BadBroadPhase();
    }

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
        for (auto& [arbKey, arb] : Arbiters)
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
                    if (World::warmStarting)
                    {
                        it->second.UpdateContacts(newArb.GetContacts());
                    }
                    else
                    {
                        Arbiters[arbKey] = newArb;
                    }
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

void World::BadBroadPhase()
{
    for (auto const& bodyI : Bodies)
    {
        for (auto const& bodyJ : Bodies)
        {
            if (bodyI->InvMass == 0.0f && bodyJ->InvMass == 0.0f)
                continue;

            Arbiter newArb(bodyI, bodyJ);
            ArbiterKey arbKey(bodyI, bodyJ);

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
                    if (World::warmStarting)
                    {
                        it->second.UpdateContacts(newArb.GetContacts());
                    }
                    else
                    {
                        Arbiters[arbKey] = newArb;
                    }
                }
            }
            else
            {
                static int count = 1;
                if (count % 10000 == 0)
                {
                    // std::cout << "Break\n";
                }
                count++;

                Arbiters.erase(arbKey);
            }
        }
    }
}