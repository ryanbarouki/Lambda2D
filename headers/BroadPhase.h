#pragma once
#include <vector>
#include <memory>
#include "IShape2.h"
#include "DynamicBVHTree.h"

using ColliderPair = std::pair<std::shared_ptr<IShape2>, std::shared_ptr<IShape2>>;
using ColliderPairList = std::vector<ColliderPair>;

class BroadPhase 
{
    ColliderPairList CollidingPairs;
    // std::vector<std::shared_ptr<IShape2>> Objects;
    DynamicBVHTree CollisionTree;
public:
    ColliderPairList& FindCollidingPairs(std::vector<std::shared_ptr<IShape2>> const& objects);
};