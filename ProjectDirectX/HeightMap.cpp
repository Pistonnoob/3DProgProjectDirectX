#include "HeightMap.h"



HeightMap::HeightMap()
{
	heightModel = NULL;
}


HeightMap::~HeightMap()
{
}

bool HeightMap::CreateFlatMesh()
{
	Vector2 size = Vector2(100,100);
	this->SetVertexCount(size.x * size.y);
	VertexModel* vertices = new VertexModel[this->GetVertexCount()];
	unsigned long* indices = NULL;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
#pragma region
	//Setting position for all vertices
	

#pragma endregion VerticesCreation 

	return false;
}

bool HeightMap::DisplaceVertices()
{
	return false;
}