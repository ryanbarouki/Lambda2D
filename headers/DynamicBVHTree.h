#pragma once
#include <vector>
#include <variant>
#include <memory>
#include <map>
#include "IShape2.h"
#include "RigidBody.h"
#include "Arbiter.h"
#include "AABB.h"

constexpr int nullNode = -1;
constexpr float aabbFatFactor = 0.2f;
constexpr float aabbMultiplier = 2.0f;

// This class is an adaptation of Erin Catto's b2DynamicTree from Box2d
class DynamicBVHTree
{
private:
    struct TreeNode
    {
        AABB FatAABB;
        std::shared_ptr<RigidBody> Object; // a ptr to the actual object e.g. a Box, Circle etc.
        int Next;
        int Parent;
        int RightChild;
        int LeftChild; 
        int Height; // leaf = 0, free node = -1

        bool IsLeaf() const
        {
            return RightChild == nullNode;
        }
    };

    // We store a large pool of TreeNodes and keep an index to the free list and root of the tree
    std::vector<TreeNode> Nodes;
    int Root;
    int FreeIndex;
    int NodeCapacity;
    int AllocatedNodeCount;

    int AllocateNode();
    void DeallocateNode(int nodeIndex);
    void InsertLeaf(int leafNodeIndex);
    void FixTree(int nodeIndex);

public:
    DynamicBVHTree();
    TreeNode const& GetNode(int id) const;
    const AABB& GetFatAABB(int queryId) const; 
    void RemoveLeaf(int leafNodeIndex);
    void UpdateLeaf(int leafNodeIndex, AABB const& newAABB, Vec2 const& displacement);
    int Insert(std::shared_ptr<RigidBody> const& object);
    std::vector<int> Query(int queryIndex, std::map<ArbiterKey, Arbiter>& arbiters) const;
};