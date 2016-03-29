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

	for (std::vector<Container*>::iterator i = models.begin(); i != models.end(); i++)
	{
		if ((*i)->object != NULL)
		{
			delete (*i)->object;
			(*i)->object = NULL;
		}
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
		Container *completeObject;
		completeObject->object = *object;
		completeObject->boundingVolume = this->GenerateBoundingVolume(*object);
		completeObject->isRendered = false;
		this->models.push_back(completeObject);
	}
	this->DivideToChildren();
	return false;
}


vector<D3Object*> QuadTree::GetObjectsInFrustrum(Frustrum * frustrum, Vector2 cameraPos)
{
	vector<D3Object*> result;
	vector<Container*> shouldRender;
	if (this->OverlappsFrustrum(frustrum))
	{
		this->StoreObjects(shouldRender, frustrum, cameraPos);
	}
	for (std::vector<Container*>::iterator i = shouldRender.begin(); i <= shouldRender.end(); i++)
	{
		result.push_back((*i)->object);
		(*i)->isRendered = false;
	}
	return result;
}

void QuadTree::DivideToChildren()//Divides its own models to its children
{
	//Loop through all internal models
	bool contained = false;
	for (std::vector<Container*>::iterator i = this->models.begin(); i != this->models.end(); i++)
	{
		BoundingVolume volume = (*i)->boundingVolume;
		Vector3 minPos = volume.middle - volume.sideDelta, maxPos = volume.middle + volume.sideDelta;
		if (this->topLeft != NULL && this->topLeft->contains(&volume))
		{
			this->topLeft->models.push_back((*i));
			contained = true;
		}
		if (this->topRight != NULL && this->topRight->contains(&volume))
		{
			this->topRight->models.push_back((*i));
			contained = true;
		}
		if (this->bottomLeft != NULL && this->bottomLeft->contains(&volume))
		{
			this->bottomLeft->models.push_back((*i));
			contained = true;
		}
		if (this->bottomRight != NULL && this->bottomRight->contains(&volume))
		{
			this->bottomRight->models.push_back((*i));
			contained = true;
		}
		if (!contained)
		{
			//ERROR SOMETHING IS VERY WRONG. Somehow our four composite quads don't contain what we do
		}
	}
	if (contained)
	{
		//Clear our models, our children contain them now.
		this->models.clear();
	}
	//And proceed to tell the children to divide their own models
	if (topLeft != NULL)
		topLeft->DivideToChildren();
	if (topRight != NULL)
		topRight->DivideToChildren();
	if (bottomLeft != NULL)
		bottomLeft->DivideToChildren();
	if (bottomRight != NULL)
		bottomRight->DivideToChildren();
}

bool QuadTree::contains(BoundingVolume * volume)
{
	bool result = false;
	Vector3 min = volume->middle - volume->sideDelta, max = volume->middle + volume->sideDelta;
	//Do overlapp check
	if (min.x >= m_max.x || max.x <= m_min.x || min.z >= m_max.y || max.z <= m_min.y)
	{
		//No overlapp
		result = false;
	}
	else
	{
		//overlapp
		result = true;
	}

	return result;
}

bool QuadTree::OverlappsFrustrum(Frustrum * frustrum)
{
	bool result = false;
	Vector2 d2SideDelta = (m_max - m_min) / 2, d2Middle = m_min + d2SideDelta;
	Vector3 sideDelta = Vector3(0.0f, 0.0f, 0.0f), middle = Vector3(0.0f, 0.0f, 0.0f);
	middle.x = d2Middle.x;
	middle.z = d2Middle.y;
	middle.y = 0.0f;
	sideDelta.x = d2SideDelta.x;
	sideDelta.z = d2SideDelta.y;
	sideDelta.y = 99999999999.0f;
	//Test against frustrum
	if (frustrum->TestAgainstRectangle(&middle, &sideDelta))
	{
		result = true;
	}
	return result;
}

void QuadTree::StoreObjects(vector<Container*> &storeIn, Frustrum * frustrum, Vector2 cameraPos)
{
	bool overlapsChild = false;
	if (topLeft != NULL && this->topLeft->OverlappsFrustrum(frustrum))
	{
		topLeft->StoreObjects(storeIn, frustrum, cameraPos);
		overlapsChild = true;
	}
	if (topRight != NULL && this->topRight->OverlappsFrustrum(frustrum))
	{
		topRight->StoreObjects(storeIn, frustrum, cameraPos);
		overlapsChild = true;
	}
	if (bottomLeft != NULL && this->bottomLeft->OverlappsFrustrum(frustrum))
	{
		bottomLeft->StoreObjects(storeIn, frustrum, cameraPos);
		overlapsChild = true;
	}
	if (bottomRight != NULL && this->bottomRight->OverlappsFrustrum(frustrum))
	{
		bottomRight->StoreObjects(storeIn, frustrum, cameraPos);
		overlapsChild = true;
	}
	if (!overlapsChild)
	{
		for (std::vector<Container*>::iterator i = this->models.begin(); i != this->models.end(); i++)
		{
			if (!(*i)->isRendered)
			{
				if (frustrum->TestAgainstRectangle(&(*i)->boundingVolume.middle, &(*i)->boundingVolume.sideDelta))
				{
					storeIn.push_back((*i));
				}
			}
		}
	}
	
	
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
	result.sideDelta = (max - min) / 2;
	result.middle = min + result.sideDelta;
	return result;
}
