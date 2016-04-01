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

//	int vertexCount = 23;
//	vector<VertexModel> vertices;
//	vector<int> indices;
//#pragma region	
//	int z = 50;
//	int x = 50;
//	float y = 0.0;
//	int index = 1;
//	for (int j = 0; j < vertexCount; j++)
//	{
//		VertexModel vertex;
//		//Triangel 1
//		vertex.position.x = x; // 50
//		vertex.position.y = 0;
//		vertex.position.z = z;
//		vertex.normal.x = 0;
//		vertex.normal.y = 1;
//		vertex.normal.z = 0;
//		vertex.UV.x = 0;
//		vertex.UV.y = y;
//		vertices.push_back(vertex);
//		indices.push_back(index);
//		j++;
//		x -= 25;
//		vertex.position.x = x; // 25
//		vertex.position.y = 0;
//		vertex.position.z = z;
//		vertex.normal.x = 0;
//		vertex.normal.y = 1;
//		vertex.normal.z = 0;
//		vertex.UV.x = 0.25;
//		vertex.UV.y = y;
//		vertices.push_back(vertex);
//		indices.push_back(index);
//		j++;
//		x -= 25;
//		vertex.position.x = x; // 0
//		vertex.position.y = 0;
//		vertex.position.z = z;
//		vertex.normal.x = 0;
//		vertex.normal.y = 1;
//		vertex.normal.z = 0;
//		vertex.UV.x = 0.50;
//		vertex.UV.y = y;
//		vertices.push_back(vertex);
//		indices.push_back(index);
//		j++;
//		x -= 25;
//		//Triangle 2
//		vertex.position.x = x; // -25
//		vertex.position.y = 0;
//		vertex.position.z = z;
//		vertex.normal.x = 0;
//		vertex.normal.y = 1;
//		vertex.normal.z = 0;
//		vertex.UV.x = 0.75;
//		vertex.UV.y = y;
//		vertices.push_back(vertex);
//		indices.push_back(index);
//		j++;
//		x -= 25;
//		vertex.position.x = x; // -50
//		vertex.position.y = 0;
//		vertex.position.z = z;
//		vertex.normal.x = 0;
//		vertex.normal.y = 1;
//		vertex.normal.z = 0;
//		vertex.UV.x = 1;
//		vertex.UV.y = y;
//		vertices.push_back(vertex);
//		indices.push_back(index);
//		z -= 25;
//		y += 0.25;
//		index++;
//	}
//	this->CreateFromData(vertices);
//	this->SetIndices();
//#pragma endregion VerticesPositions 

	return true;
}

bool HeightMap::DisplaceVertices()
{
	return false;
}