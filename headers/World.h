#pragma once
#include "RigidBody.h"
#include "Arbiter.h"
#include "DynamicBVHTree.h"
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>

class World
{
public:
    World(Vec2 const& gravity, float iterations) : Gravity(gravity), Iterations(iterations)
    {}

    void Step(float dt);
    void Add(RigidBody const& body);
    std::map<ArbiterKey, Arbiter> const& GetArbiters() const {return Arbiters;}
    std::vector<std::shared_ptr<RigidBody>> const& GetBodies() const {return Bodies;}

    static bool accumulateImpulses;
    static bool warmStarting;
    static bool positionCorrection;
    static bool goodBroadPhase;

private:
    // O(NlogN) Broad Phase
    void BroadPhase();
    // O(N^2) Broad Phase
    void BadBroadPhase();

    Vec2 Gravity;
    int Iterations;
    std::vector<std::shared_ptr<RigidBody>> Bodies;
    std::unordered_map<std::shared_ptr<RigidBody>, int> BodyIndices;
    std::map<ArbiterKey, Arbiter> Arbiters;
    DynamicBVHTree CollisionTree; // I think the Add should add the body to this tree
};