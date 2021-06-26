#include "../headers/DynamicBVHTree.h"
#include <cassert>
#include <stack>
#include <iostream>

DynamicBVHTree::DynamicBVHTree() : NodeCapacity(16), FreeIndex(0), AllocatedNodeCount(0), Root(nullNode)
{
    Nodes.resize(NodeCapacity);

    for (int i = 0; i < NodeCapacity - 1; ++i)
	{
		Nodes[i].Next = i + 1;
		Nodes[i].Height = -1;
	}
	Nodes[NodeCapacity - 1].Next = nullNode;
	Nodes[NodeCapacity - 1].Height = -1;
}

int DynamicBVHTree::AllocateNode()
{
	// no free nodes in the pool so we must grow the pool 
	if (FreeIndex == nullNode)
	{
		assert(AllocatedNodeCount == NodeCapacity);

		// double the NodeCapacity
		NodeCapacity *= 2;
		Nodes.resize(NodeCapacity);
		for (int i = AllocatedNodeCount; i < NodeCapacity - 1; ++i)
		{
			Nodes[i].Next = i + 1;
			Nodes[i].Height = -1;
		}
		Nodes[NodeCapacity - 1].Next = nullNode;
		Nodes[NodeCapacity - 1].Height = -1;
		FreeIndex = AllocatedNodeCount;
	}

	// Now get a node from the node pool
	int nodeIdx = FreeIndex;
	TreeNode& node = Nodes[nodeIdx];
	node.Parent = nullNode;
	node.LeftChild = nullNode;
	node.RightChild = nullNode;
	node.Height = 0;
	// TODO: moved bool = false ?? not sure on the use yet
	FreeIndex = node.Next;
	++AllocatedNodeCount;

	return nodeIdx;
}

void DynamicBVHTree::DeallocateNode(int nodeIndex)
{
	assert(nodeIndex >= 0);
	assert(nodeIndex < NodeCapacity);
	assert(AllocatedNodeCount > 0);

	TreeNode& node = Nodes[nodeIndex];
	node.Next = FreeIndex;
	node.Height = -1;
	FreeIndex = nodeIndex;
	--AllocatedNodeCount;
}
	
void DynamicBVHTree::InsertLeaf(int leafNodeIndex)
{
	// if the tree is empty then we make the root the leaf
	if (Root == nullNode)
	{
		Root = leafNodeIndex;
		// Nodes[Root].Parent = nullNode;
		return;
	}

	// find the best place to put the new leaf
	int idx = Root;
	while (!Nodes[idx].IsLeaf())
	{
		TreeNode& currNode = Nodes[idx];
		TreeNode& leftChild = Nodes[currNode.LeftChild];
		TreeNode& rightChild = Nodes[currNode.RightChild];

		float area = currNode.FatAABB.GetPerimeter();

		float combinedArea = MergeAABB(currNode.FatAABB, Nodes[leafNodeIndex].FatAABB).GetPerimeter();

		float cost = 2.0f * combinedArea;

		float minInheritanceTax = 2.0f * (combinedArea - area); // lol

		// Cost of going down the left child tree
		float leftChildCost;
		if (leftChild.IsLeaf())
		{
			leftChildCost = MergeAABB(Nodes[leafNodeIndex].FatAABB, leftChild.FatAABB).GetPerimeter() + minInheritanceTax;
		}
		else
		{
			float newArea = MergeAABB(Nodes[leafNodeIndex].FatAABB, leftChild.FatAABB).GetPerimeter();	
			float oldArea = leftChild.FatAABB.GetPerimeter();
			leftChildCost = (newArea - oldArea) + minInheritanceTax;
		}

		// Cost of going down the right child tree
		float rightChildCost;
		if (rightChild.IsLeaf())
		{
			rightChildCost = MergeAABB(Nodes[leafNodeIndex].FatAABB, rightChild.FatAABB).GetPerimeter() + minInheritanceTax;
		}
		else
		{
			float newArea = MergeAABB(Nodes[leafNodeIndex].FatAABB, rightChild.FatAABB).GetPerimeter();	
			float oldArea = rightChild.FatAABB.GetPerimeter();
			rightChildCost = (newArea - oldArea) + minInheritanceTax;
		}

		// Descend into tree with minimum cost;
		if (cost < leftChildCost && cost < rightChildCost)
		{
			break;
		}

		// We must go deeper
		idx = leftChildCost < rightChildCost ? currNode.LeftChild : currNode.RightChild; 
	}

	int sibling = idx;

	int oldParentIndex = Nodes[sibling].Parent;
	int newParentIndex = AllocateNode();
	Nodes[newParentIndex].Parent = oldParentIndex;
	Nodes[newParentIndex].FatAABB = MergeAABB(Nodes[leafNodeIndex].FatAABB, Nodes[sibling].FatAABB);
	Nodes[newParentIndex].Height = Nodes[sibling].Height + 1;

	if (oldParentIndex != nullNode)
	{
		// sibling is not the root
		if (Nodes[oldParentIndex].LeftChild == sibling)
		{
			Nodes[oldParentIndex].LeftChild = newParentIndex;
		}
		else
		{
			Nodes[oldParentIndex].RightChild = newParentIndex;
		}

		Nodes[newParentIndex].LeftChild = sibling;
		Nodes[newParentIndex].RightChild = leafNodeIndex;
		Nodes[sibling].Parent = newParentIndex;
		Nodes[leafNodeIndex].Parent = newParentIndex;
	}
	else
	{
		// the sibling was the root
		Nodes[newParentIndex].LeftChild = sibling;
		Nodes[newParentIndex].RightChild = leafNodeIndex;
		Nodes[sibling].Parent = newParentIndex;
		Nodes[leafNodeIndex].Parent = newParentIndex;
		Root = newParentIndex;
	}

	// goes back up the tree fixing the AABBs of parents
	FixTree(Nodes[leafNodeIndex].Parent);	
}

void DynamicBVHTree::FixTree(int nodeIndex)
{
	while (nodeIndex != nullNode)
	{
		TreeNode& node = Nodes[nodeIndex];

		// Nothing here should be a leaf
		assert(node.LeftChild != nullNode);
		assert(node.RightChild != nullNode);

		TreeNode& leftNode = Nodes[node.LeftChild];
		TreeNode& rightNode = Nodes[node.RightChild];
		node.FatAABB = MergeAABB(leftNode.FatAABB, rightNode.FatAABB);

		nodeIndex = node.Parent;
	}
}

void DynamicBVHTree::RemoveLeaf(int leafNodeIndex)
{
	if (leafNodeIndex == Root)
	{
		Root = nullNode;
		return;
	}

	int parentNodeIndex = Nodes[leafNodeIndex].Parent;
	int grandParentIndex = Nodes[parentNodeIndex].Parent;
	int siblingIndex = Nodes[parentNodeIndex].LeftChild == leafNodeIndex ? Nodes[parentNodeIndex].RightChild : Nodes[parentNodeIndex].LeftChild;

	if (grandParentIndex != nullNode)
	{
		// the parent is not the root
		TreeNode& grandParent = Nodes[grandParentIndex];
		if (grandParent.LeftChild == parentNodeIndex)
		{
			grandParent.LeftChild = siblingIndex;
		}
		else
		{
			grandParent.RightChild = siblingIndex;
		}
		Nodes[siblingIndex].Parent = grandParentIndex;
		DeallocateNode(parentNodeIndex);

		FixTree(grandParentIndex);
	}
	else
	{
		Root = siblingIndex;
		Nodes[siblingIndex].Parent = nullNode;
		DeallocateNode(parentNodeIndex);
	}

	Nodes[leafNodeIndex].Parent = nullNode;	
}

void DynamicBVHTree::UpdateLeaf(int leafNodeIndex, AABB const& newAABB, Vec2 const& displacement)
{
	// if the current AABB contains the new AABB then do nothing
	if (Nodes[leafNodeIndex].FatAABB.Contains(newAABB))
		return;

	AABB fatAABB = newAABB; 
	fatAABB.Fatten(Vec2{aabbFatFactor, aabbFatFactor});
	
	// use the displacement of the object to update the AABB 
	Vec2 d = aabbMultiplier * displacement;

	if (d.x < 0.0f)
	{
		fatAABB.Min.x += d.x;
	}
	else
	{
		fatAABB.Max.x += d.x;
	}

	if (d.y < 0.0f)
	{
		fatAABB.Min.y += d.y;
	}
	else
	{
		fatAABB.Max.y += d.y;
	}

	RemoveLeaf(leafNodeIndex);
	Nodes[leafNodeIndex].FatAABB = fatAABB;
	InsertLeaf(leafNodeIndex);
}

int DynamicBVHTree::Insert(std::shared_ptr<RigidBody> const& object)
{
	int nodeIndex = AllocateNode();
	AABB aabb = object->GetAABB();

	// fatten the object's AABB before adding to tree
	Vec2 fat{aabbFatFactor, aabbFatFactor};
	Nodes[nodeIndex].FatAABB = aabb;
	Nodes[nodeIndex].FatAABB.Fatten(fat);

	Nodes[nodeIndex].Object = object;
	Nodes[nodeIndex].Height = 0;

	InsertLeaf(nodeIndex);
	
	return nodeIndex;
}

std::vector<int> DynamicBVHTree::Query(int queryIndex) const
{
	std::vector<int> overlaps;
	std::stack<int> stack;
	AABB objectAABB = Nodes[queryIndex].Object->GetAABB();

	stack.push(Root);

	while(!stack.empty())
	{
		int currNodeIndex = stack.top();
		stack.pop();

		if (currNodeIndex == nullNode) 
			continue;

		TreeNode const& currNode = Nodes[currNodeIndex];
		if (currNode.FatAABB.Intersects(objectAABB))
		{
			// is a leaf and is not the object we're querying with
			if (currNode.IsLeaf() && currNodeIndex != queryIndex)
			{
				// choose an arbitrary inequality to avoid duplicate pairs
				if (queryIndex < currNodeIndex)
				{
					overlaps.push_back(currNodeIndex);
				}
			}
			else
			{
				stack.push(currNode.LeftChild);
				stack.push(currNode.RightChild);
			}
		}
	}

	return overlaps;
}

const AABB& DynamicBVHTree::GetFatAABB(int queryId) const
{
	return Nodes[queryId].FatAABB;
}


DynamicBVHTree::TreeNode const& DynamicBVHTree::GetNode(int id) const
{
	return Nodes[id];
}