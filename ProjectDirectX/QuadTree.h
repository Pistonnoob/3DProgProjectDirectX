#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "StructLibrary.h"
#include "D3Object.h"
#include <vector>


struct BoundingBoxNode
{
	Vector2 min;
	Vector2 max;
	BoundingBoxNode* topLeft;
	BoundingBoxNode* topRight;
	BoundingBoxNode* botLeft;
	BoundingBoxNode* botRight;
	std::vector<D3Object*> models;
};

class QuadTree
{
private:	//Variables
	BoundingBoxNode root;
public:
	QuadTree();
	QuadTree(const QuadTree& original);
	virtual ~QuadTree();

	void ShutDown();
	void Initialize(int width, int height, int depth);
	
private:	//Functions
	void GenerateQuadTree(BoundingBoxNode active, int depth);
};

#endif