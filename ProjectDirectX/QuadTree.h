#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "StructLibrary.h"
#include "D3Object.h"
#include <vector>
#include "Frustrum.h"

static const int m_max = 10;

struct BoundingVolume
{
	Vector3 middle;
	Vector3 sideDelta;
};

class QuadTree
{
private:	//Variables
	Vector2 m_min;
	Vector2 m_max;
	QuadTree* topLeft;
	QuadTree* topRight;
	QuadTree* bottomLeft;
	QuadTree* bottomRight;

	vector<std::pair<D3Object*, BoundingVolume>> models;
public:
	QuadTree();
	QuadTree(const QuadTree& original);
	virtual ~QuadTree();

	void ShutDown();
	void Initialize(Vector2 m_min, Vector2 end, int depth);
	bool DefineQuadTree(vector<D3Object*> models);

	vector<D3Object*> GetObjectsInFrustrum(Frustrum* frustrum, Vector2 cameraPos);
	
private:	//Functions
	void StoreObjects(vector<D3Object*> storeIn, Frustrum* frustrum, Vector2 cameraPos);
	BoundingVolume GenerateBoundingVolume(D3Object* model);
};

#endif