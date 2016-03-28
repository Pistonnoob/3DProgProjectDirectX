#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "StructLibrary.h"
#include "D3Object.h"
#include <vector>

class QuadTree
{
private:	//Varaibles
public:
	QuadTree();
	QuadTree(const QuadTree& original);
	virtual ~QuadTree();

	void Initialize(int width, int height, int depth);
	
private:	//Functions
};

#endif