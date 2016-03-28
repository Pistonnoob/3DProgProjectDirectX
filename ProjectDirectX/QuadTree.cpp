#include "QuadTree.h"



QuadTree::QuadTree()
{
	this->m_min = Vector2(0.0f, 0.0f);
	this->m_max = Vector2(0.0f, 0.0f);
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

void QuadTree::Initialize(Vector2 m_min, Vector2 end, int depth)
{
	this->m_min = m_min;
	this->m_max = m_max;

	if (depth > 0)
	{
		depth--;

		int width = this->m_max.x - this->m_min.x;
		int height = this->m_max.y - this->m_min.y;

		this->topLeft = new QuadTree();
		this->topLeft->Initialize(this->m_min, this->m_min + Vector2(width / 2, height / 2), depth);

		this->topRight = new QuadTree();
		this->topRight->Initialize(this->m_min + Vector2(width / 2, 0), this->m_min + Vector2(width, height / 2), depth);

		this->bottomLeft = new QuadTree();
		this->bottomLeft->Initialize(this->m_min + Vector2(0, height / 2), this->m_min + Vector2(width / 2, height), depth);

		this->bottomRight = new QuadTree();
		this->bottomRight->Initialize(this->m_min + Vector2(width / 2, height / 2), this->m_max, depth);
	}
}

bool QuadTree::DefineQuadTree(vector<D3Object*> models)
{

}


vector<D3Object*> QuadTree::GetObjectsInFrustrum(Frustrum * frustrum, Vector2 cameraPos)
{
	return vector<D3Object*>();
}

void QuadTree::StoreObjects(vector<D3Object*> storeIn, Frustrum * frustrum, Vector2 cameraPos)
{
}

BoundingVolume QuadTree::GenerateBoundingVolume(D3Object * model)
{
	Vector3 min = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 max = Vector3(0.0f, 0.0f, 0.0f);

}
