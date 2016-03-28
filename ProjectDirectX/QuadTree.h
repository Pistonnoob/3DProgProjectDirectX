#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "StructLibrary.h"
#include "D3Object.h"
#include <vector>
#include "Frustrum.h"

static const int max = 10;

class QuadTree
{
private:	//Variables
	Vector2 min;
	Vector2 max;
	QuadTree* topLeft;
	QuadTree* topRight;
	QuadTree* bottomLeft;
	QuadTree* bottomRight;

	vector<D3Object*> models;
public:
	QuadTree();
	QuadTree(const QuadTree& original);
	virtual ~QuadTree();

	void ShutDown();
	void Initialize(Vector2 min, Vector2 end, int depth);
	bool AddModel(vector<D3Object*> models);

	vector<D3Object*> GetObjectsInFrustrum(Frustrum* frustrum, Vector2 cameraPos);
	
private:	//Functions
	void StoreObjects(vector<D3Object*> storeIn, Frustrum* frustrum, Vector2 cameraPos);
};

#endif