#include "QuadTree.h"



QuadTree::QuadTree()
{
	this->min = Vector2(0.0f, 0.0f);
	this->max = Vector2(0.0f, 0.0f);
	this->topLeft = NULL;
	this->topRight = NULL;
	this->bottomLeft = NULL;
	this->bottomRight = NULL;
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

void QuadTree::Initialize(Vector2 min, Vector2 end, int depth)
{
	this->min = min;
	this->max = max;

	if (depth > 0)
	{
		depth--;

		int width = this->max.x - this->min.x;
		int height = this->max.y - this->min.y;

		this->topLeft = new QuadTree();
		this->topLeft->Initialize(this->min, this->min + Vector2(width / 2, height / 2), depth);

		this->topRight = new QuadTree();
		this->topRight->Initialize(this->min + Vector2(width / 2, 0), this->min + Vector2(width, height / 2), depth);

		this->bottomLeft = new QuadTree();
		this->bottomLeft->Initialize(this->min + Vector2(0, height / 2), this->min + Vector2(width / 2, height), depth);

		this->bottomRight = new QuadTree();
		this->bottomRight->Initialize(this->min + Vector2(width / 2, height / 2), this->max, depth);
	}
}

bool QuadTree::AddModel(vector<D3Object*> models)
{
	return false;
}

vector<D3Object*> QuadTree::GetObjectsInFrustrum(Frustrum * frustrum, Vector2 cameraPos)
{
	return vector<D3Object*>();
}

void QuadTree::StoreObjects(vector<D3Object*> storeIn, Frustrum * frustrum, Vector2 cameraPos)
{
}
