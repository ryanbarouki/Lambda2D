#pragma once
#include <vector>
#include <memory>
#include "IShape2.h"

using ColliderPair = std::pair<std::shared_ptr<IShape2>, std::shared_ptr<IShape2>>;
using ColliderPairList = std::vector<ColliderPair>;

class BroadPhase 
{
public:
    ColliderPairList& FindCollidingPairs();
};