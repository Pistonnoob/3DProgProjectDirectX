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

struct Container
{
	D3Object* object;
	BoundingVolume boundingVolume;
	bool isRendered;
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
	vector<Container*> models;
	//std::pair<D3Object*, std::pair<BoundingVolume, bool>>
public:
	QuadTree();
	QuadTree(const QuadTree& original);
	virtual ~QuadTree();

	void ShutDown();
	void Initialize(Vector2 m_min, Vector2 m_max, int depth);
	bool DefineQuadTree(vector<D3Object*> models);

	vector<D3Object*> GetObjectsInFrustrum(Frustrum* frustrum, Vector2 cameraPos);
	
private:	//Functions
	void DivideToChildren();
	bool contains(BoundingVolume* volume);
	bool OverlappsFrustrum(Frustrum* frustrum);
	void StoreObjects(vector<Container*> &storeIn, Frustrum* frustrum, Vector2 cameraPos);
	BoundingVolume GenerateBoundingVolume(D3Object* model);
};

#endif