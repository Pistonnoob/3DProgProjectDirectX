#include "HeightMap.h"



HeightMap::HeightMap()
{
	m_heightMap = NULL;
}
HeightMap::~HeightMap()
{

}

bool HeightMap::LoadHeightMap(char * filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;

	// Open the height map file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_terrainWidth * m_terrainHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}
	
	// Create the structure to hold the height map data.
	m_heightMap = new HeightMapPos[m_terrainWidth * m_terrainHeight];
	if (!m_heightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (m_terrainHeight * j) + i;

			m_heightMap[index].position.x = (float)i;
			m_heightMap[index].position.y = (float)height;
			m_heightMap[index].position.z = (float)j;

			k += 3;
		}
	}
	// Release the bitmap image data.
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}

void HeightMap::NormalizeHeightMap()
{
	int i, j;


	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			m_heightMap[(m_terrainHeight * j) + i].position.y /= 15.0f;
		}
	}
}

void HeightMap::ShutDownHM()
{
	if (m_heightMap)
	{
		delete[] m_heightMap;
		m_heightMap = 0;
	}
}

bool HeightMap::InitializeBuffersHeightMap(ID3D11Device *device)
{
	// Calculate the number of vertices in the terrain mesh.
	Vertex* vertices = NULL;
	unsigned long* indices = NULL;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;
	int Count = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 12;
	int index = 0;
	this->SetVertexCount(Count);
	this->SetIndexCount(Count);
	// Initialize the index to the vertex buffer.
	index = 0;

	// Load the vertex and index array with the terrain data.
	for (int j = 0; j<(m_terrainHeight - 1); j++)
	{
		for (int i = 0; i<(m_terrainWidth - 1); i++)
		{
			index1 = (m_terrainHeight * j) + i;          // Bottom left.
			index2 = (m_terrainHeight * j) + (i + 1);      // Bottom right.
			index3 = (m_terrainHeight * (j + 1)) + i;      // Upper left.
			index4 = (m_terrainHeight * (j + 1)) + (i + 1);  // Upper right.

															 // Upper left.
			vertices[index].position = m_heightMap[index3].position;
			vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = m_heightMap[index4].position;
			vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = m_heightMap[index4].position;
			vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = m_heightMap[index1].position;
			vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = m_heightMap[index1].position;
			vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper left.
			vertices[index].position = m_heightMap[index3].position;
			vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = m_heightMap[index1].position;
			vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = m_heightMap[index4].position;
			vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = m_heightMap[index4].position;
			vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom right.
			vertices[index].position = m_heightMap[index2].position;
			vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom right.
			vertices[index].position = m_heightMap[index2].position;
			vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = m_heightMap[index1].position;
			vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;
		}
	}
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexModel) * this->GetVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	//Create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
}


//bool HeightMap::CreateFlatMesh()
//{
//
////	int vertexCount = 23;
////	vector<VertexModel> vertices;
////	vector<int> indices;
////#pragma region	
////	int z = 50;
////	int x = 50;
////	float y = 0.0;
////	int index = 1;
////	for (int j = 0; j < vertexCount; j++)
////	{
////		VertexModel vertex;
////		//Triangel 1
////		vertex.position.x = x; // 50
////		vertex.position.y = 0;
////		vertex.position.z = z;
////		vertex.normal.x = 0;
////		vertex.normal.y = 1;
////		vertex.normal.z = 0;
////		vertex.UV.x = 0;
////		vertex.UV.y = y;
////		vertices.push_back(vertex);
////		indices.push_back(index);
////		j++;
////		x -= 25;
////		vertex.position.x = x; // 25
////		vertex.position.y = 0;
////		vertex.position.z = z;
////		vertex.normal.x = 0;
////		vertex.normal.y = 1;
////		vertex.normal.z = 0;
////		vertex.UV.x = 0.25;
////		vertex.UV.y = y;
////		vertices.push_back(vertex);
////		indices.push_back(index);
////		j++;
////		x -= 25;
////		vertex.position.x = x; // 0
////		vertex.position.y = 0;
////		vertex.position.z = z;
////		vertex.normal.x = 0;
////		vertex.normal.y = 1;
////		vertex.normal.z = 0;
////		vertex.UV.x = 0.50;
////		vertex.UV.y = y;
////		vertices.push_back(vertex);
////		indices.push_back(index);
////		j++;
////		x -= 25;
////		//Triangle 2
////		vertex.position.x = x; // -25
////		vertex.position.y = 0;
////		vertex.position.z = z;
////		vertex.normal.x = 0;
////		vertex.normal.y = 1;
////		vertex.normal.z = 0;
////		vertex.UV.x = 0.75;
////		vertex.UV.y = y;
////		vertices.push_back(vertex);
////		indices.push_back(index);
////		j++;
////		x -= 25;
////		vertex.position.x = x; // -50
////		vertex.position.y = 0;
////		vertex.position.z = z;
////		vertex.normal.x = 0;
////		vertex.normal.y = 1;
////		vertex.normal.z = 0;
////		vertex.UV.x = 1;
////		vertex.UV.y = y;
////		vertices.push_back(vertex);
////		indices.push_back(index);
////		z -= 25;
////		y += 0.25;
////		index++;
////	}
////	this->CreateFromData(vertices);
////	this->SetIndices();
////#pragma endregion VerticesPositions 
//
//	return true;
//}
//
//bool HeightMap::DisplaceVertices()
//{
//	return false;
//}