#include "HeightMap.h"



HeightMap::HeightMap()
{
	m_heightMap = NULL;
}
HeightMap::~HeightMap()
{
	ShutDownHM();
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
	filePtr = nullptr;
	
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
	bitmapImage = nullptr;

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
	
	VertexModel* vertices = NULL;
	unsigned long* indices = NULL;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;
	// Calculate the number of vertices in the terrain mesh.
	int Count = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 12;
	int index = 0;
	this->SetVertexCount(Count);
	this->SetIndexCount(Count);

	// Create the vertex array.
	vertices = new VertexModel[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}
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
			vertices[index].normal = Vector3(0.0f, 1.0f, 0.0f);
			vertices[index].UV = Vector2(0.0f,0.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = m_heightMap[index4].position;
			vertices[index].normal = Vector3(0.0f, 1.0f, 0.0f);
			vertices[index].UV = Vector2(0.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = m_heightMap[index4].position;
			vertices[index].normal = Vector3(0.0f, 1.0f, 0.0f);
			vertices[index].UV = Vector2(0.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = m_heightMap[index1].position;
			vertices[index].normal = Vector3(0.0f, 1.0f, 0.0f);
			vertices[index].UV = Vector2(1.0f, 0.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = m_heightMap[index1].position;
			vertices[index].normal = Vector3(0.0f, 1.0f, 0.0f);
			vertices[index].UV = Vector2(1.0f, 0.0f);
			indices[index] = index;
			index++;

			// Upper left.
			vertices[index].position = m_heightMap[index3].position;
			vertices[index].normal = Vector3(0.0f, 1.0f, 0.0f);
			vertices[index].UV = Vector2(0.0f, 0.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = m_heightMap[index1].position;
			vertices[index].normal = Vector3(0.0f, 1.0f, 0.0f);
			vertices[index].UV = Vector2(1.0f, .0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = m_heightMap[index4].position;
			vertices[index].normal = Vector3(0.0f, 1.0f, 0.0f);
			vertices[index].UV = Vector2(0.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = m_heightMap[index4].position;
			vertices[index].normal = Vector3(0.0f, 1.0f, 0.0f);
			vertices[index].UV = Vector2(0.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom right.
			vertices[index].position = m_heightMap[index2].position;
			vertices[index].normal = Vector3(0.0f, 1.0f, 0.0f);
			vertices[index].UV = Vector2(1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom right.
			vertices[index].position = m_heightMap[index2].position;
			vertices[index].normal = Vector3(0.0f, 1.0f, 0.0f);
			vertices[index].UV = Vector2(1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = m_heightMap[index1].position;
			vertices[index].normal = Vector3(0.0f, 1.0f, 0.0f);
			vertices[index].UV = Vector2(1.0f, 0.0f);
			/*vertices[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);*/
			indices[index] = index;

			index++;
		}
	}
	this->m_model = vertices;
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexModel) * m_vertexCount;
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

	return true;
}