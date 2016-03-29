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
	if (topLeft != NULL)
	{
		this->topLeft->ShutDown();
		this->topLeft = NULL;
	}
	if (topRight != NULL)
	{
		this->topRight->ShutDown();
		this->topRight = NULL;
	}
	if (bottomLeft != NULL)
	{
		this->bottomLeft->ShutDown();
		this->bottomLeft = NULL;
	}
	if (bottomRight != NULL)
	{
		this->bottomRight->ShutDown();
		this->bottomRight = NULL;
	}

	for (std::vector<std::pair<D3Object*, BoundingVolume>>::iterator i = models.begin(); i != models.end(); i++)
	{
		delete i->first;
		i->first = NULL;
	}

	models.clear();
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
	//Loop through all inserted objects
	for (std::vector<D3Object*>::iterator object = models.begin(); object != models.end(); object++)
	{
		//generate BoundingVolumes for every model
	}
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
	BoundingVolume result;
	Vector3 min = Vector3(-1.0f, -1.0f, -1.0f);
	Vector3 max = Vector3(-1.0f, -1.0f, -1.0f);
	bool first = true;

	vector<VertexModel> points = model->getVertexData();
	for (std::vector<VertexModel>::const_iterator index = points.begin(); index != points.end(); index++)
	{
		if (first)
		{
			min = max = (*index).position;
		}
		else
		{
			if ((*index).position.x < min.x)
				min.x = (*index).position.x;
			else if ((*index).position.x > max.x)
				max.x = (*index).position.x;
			if ((*index).position.y < min.y)
				min.y = (*index).position.y;
			else if ((*index).position.y > max.y)
				max.y = (*index).position.y;
			if ((*index).position.z < min.z)
				min.z = (*index).position.z;
			else if ((*index).position.z > max.z)
				max.z = (*index).position.z;
		}
		
	}
	//We now have all sides. Proceed to create a bounding volume
	result.sideDelta = max - min;
	result.middle = min + (max - min);
	return result;
}
