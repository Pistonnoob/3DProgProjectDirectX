#include "QuadTree.h"



QuadTree::QuadTree()
{
}

QuadTree::QuadTree(const QuadTree & original)
{
}


QuadTree::~QuadTree()
{
}

void QuadTree::ShutDown()
{
}

void QuadTree::Initialize(int width, int height, int depth)
{
	
}

void QuadTree::GenerateQuadTree(BoundingBoxNode active, int depth)
{
	if (depth > 0)
	{
		depth--;
		Vector2 xMiddle = Vector2((active.min.x + active.max.x) / 2, active.min.y);
		Vector2 yMiddle = Vector2(active.min.x, (active.min.y + active.max.y) / 2);
		Vector2 xyMiddle = Vector2(xMiddle.x, yMiddle.y);
		float xMid = (active.min.x + active.max.x) / 2;
		float yMid = (active.min.y + active.max.y) / 2;
		active.topLeft->min = active.min;
		active.topLeft->max = Vector2(active.min + active.max) / 2;

		active.topRight->min = Vector2((active.min.x + active.max.x) / 2, active.min.y);
		active.topRight->max = Vector2(active.min + active.max);

		GenerateQuadTree(*active.topLeft, depth);
		GenerateQuadTree(*active.topRight, depth);
		GenerateQuadTree(*active.botLeft, depth);
		GenerateQuadTree(*active.botRight, depth);
	}
}
