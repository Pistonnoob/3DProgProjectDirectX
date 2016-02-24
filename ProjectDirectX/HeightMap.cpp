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
	int width = 100;
	int depth = 100;
	this->SetVertexCount(width * depth);		  // 100 x 100 = 10 000 
	VertexModel* vertices = new VertexModel[this->GetVertexCount()];
	unsigned long* indices = NULL;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
#pragma region
	//Setting position for all vertices
	int z = 0;
	// x = 1 Becuase we want the first vertices to be (0,0,0)
	for (int x = 1; x < this->GetVertexCount(); x++) 
	{
		if (x % 100 == 0)
			z++;
		vertices[x].position.x = x % 100;
		vertices[x].position.z = z;
	}
#pragma endregion VerticesPositions 

	return false;
}

bool HeightMap::DisplaceVertices()
{
	return false;
}