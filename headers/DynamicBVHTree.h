#pragma once
#include <vector>
#include <variant>
#include <memory>
#include "IShape2.h"
#include "AABB.h"

constexpr int nullNode = -1;
using ColliderPairList = std::vector<std::pair<std::shared_ptr<IShape2>, std::shared_ptr<IShape2>>>;

// This class is an adaptation of Erin Catto's b2DynamicTree from Box2d
class DynamicBVHTree
{
private:
    struct TreeNode
    {
        AABB FatAABB;
        std::shared_ptr<IShape2> Object; // a ptr to the actual object e.g. a Box, Circle etc.
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
    ColliderPairList CollidingPairs;
    int Root;
    int FreeIndex;
    int NodeCapacity;
    int AllocatedNodeCount;

    int AllocateNode();
    void DeallocateNode(int nodeIndex);
    void InsertLeaf(int leafNodeIndex);
    void RemoveLeaf(int leafNodeIndex);
    void UpdateLeaf(int leafNodeIndex, AABB const& newAABB);
    void FixTree(int nodeIndex);

public:
    DynamicBVHTree();
    void Update(std::shared_ptr<IShape2> const& object);
    void Insert(std::shared_ptr<IShape2> const& object);
    void Remove(std::shared_ptr<IShape2> const& object);
    ColliderPairList& FindCollidingPairs();
};