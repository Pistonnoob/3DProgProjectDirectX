#include "D3Object.h"



D3Object::D3Object()
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_vertexCount = 0;
	m_indexCount = 0;
	m_model = NULL;
	m_texture = NULL;
}

D3Object::D3Object(const D3Object & original)
{
}


D3Object::~D3Object()
{
}

bool D3Object::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFileName, FactoryObjectFormat format)
{
	bool result = false;

	//Load the model data
	switch (format)
	{
	case OBJ:
	case OBJ_LH:
	case OBJ_RH:
		result = this->LoadModel(modelFilename);
		break;
	case TXT:
		result = this->LoadModelTXT(modelFilename);
		break;
	default:
		break;
	}
//	result = this->LoadModel(modelFilename);
	if (!result)
		return false;

	//Initialize vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
		return false;

	//Load the texture for this model
	result = this->LoadTexture(device, deviceContext, textureFileName);
	if (!result)
		return false;

	return true;
}

void D3Object::Shutdown()
{
	//Release the texture.
	this->ReleaseTexture();
	//Release the buffers.
	this->ShutdownBuffers();

	//Release the model data.
	this->ReleaseModel();

	return;
}

void D3Object::Render(ID3D11DeviceContext * deviceContext)
{
	//Load in the vertex and index buffers into the graphics pipeline
	this->RenderBuffers(deviceContext);

	return;
}

int D3Object::GetIndexCount()
{
	return this->m_indexCount;
}

ID3D11ShaderResourceView * D3Object::GetTexture()
{
	return this->m_texture->GetTextureView();
}

bool D3Object::InitializeBuffers(ID3D11Device *device)
{
	VertexModel* vertices = NULL;
	unsigned long* indices = NULL;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//Set the number of vertices in the vertex array.
	//this->m_vertexCount = 3;
	//this->m_indexCount = 3;

	//Create buffers and check for successfull completion.
	//Create the vertex array. Fun part :D
	vertices = new VertexModel[this->m_vertexCount];
	if (!vertices)
	{
		return false;
	}
	//Create the index array. Not so much fun, but good.
	indices = new unsigned long[this->m_indexCount];
	if (!indices)
	{
		return false;
	}
	//This is where the .obj files and material files will be loaded into the structures and thus subresources
	
	//Load the vertex array and index array with data.
	for (int i = 0; i < this->m_vertexCount; i++)
	{
		vertices[i].position = m_model[i].position;
		vertices[i].UV = m_model[i].UV;
		vertices[i].normal = m_model[i].normal;
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexModel) * this->m_vertexCount;
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

	//Release the vertex array and the index array. the data has been stored in their respective buffer.
	delete[] vertices;
	vertices = NULL;
	delete[] indices;
	indices = NULL;


	return true;
}

void D3Object::ShutdownBuffers()
{
	//Release the vertex buffer
	if (this->m_vertexBuffer != NULL)
	{
		this->m_vertexBuffer->Release();
		this->m_vertexBuffer = NULL;
	}
	//Release the index buffer
	if (this->m_indexBuffer != NULL)
	{
		this->m_indexBuffer->Release();
		this->m_indexBuffer = NULL;
	}

	return;
}

void D3Object::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	unsigned int stride = 0;
	unsigned int offset = 0;

	stride = sizeof(VertexModel);

	//Set the vertex buffer for the input assembler.
	deviceContext->IASetVertexBuffers(0, 1, &this->m_vertexBuffer, &stride, &offset);

	//Do the same for the index buffer.
	deviceContext->IASetIndexBuffer(this->m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Actually set the topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool D3Object::LoadTexture(ID3D11Device *device, ID3D11DeviceContext *deviceContext, char * fileName)
{
	bool result = false;

	//Create the texture object
	this->m_texture = new TextureObject();
	if (this->m_texture == NULL)
	{
		return false;
	}

	//Now initialize the texture object
	result = m_texture->Initialize(device, deviceContext, fileName);
	if (!result)
	{
		return false;
	}

	return true;
}

void D3Object::ReleaseTexture()
{
	if (m_texture != NULL)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = NULL;
	}
	return;
}

bool D3Object::LoadModel(char * fileName)
{
	ifstream fileIn;
	string special = "", line = "", line2 = "";
	char specialChar[10];
	istringstream inputString;
	vector<Vector3> vertices;
	vector<Vector3> normals;
	vector<Vector2> UV;
	vector<VertexModel> vertexData;
	Vector3 vtx = { 0, 0, 0 }, vn = { 0, 0, 0 };
	Vector2 vt = { 0, 0 };

	char temp[512];

	fileIn.open(fileName, ios::in);
	if (!fileIn.is_open())
	{
		return false;
	}

	while (std::getline(fileIn, line2))
	{
		strncpy(temp, line2.c_str(), sizeof(temp));
		temp[sizeof(temp) - 1] = 0;

		inputString.str(line2);
		if (line2.substr(0, 2) == "v ")
		{
			// Vertex Position
			sscanf(temp, "%s %f %f %f\n", specialChar, &vtx.x, &vtx.y, &vtx.z);
			//inputString >> special >> vtx.x >> vtx.y >> vtx.z;
			vertices.push_back(vtx);
		}
		else if (line2.substr(0, 2) == "vt")
		{
			// Vertex UV
			sscanf(temp, "%s %f %f\n", specialChar, &vt.x, &vt.y);
			//inputString >> special >> vt.x >> vt.y;
			UV.push_back(vt);
		}
		else if (line2.substr(0, 2) == "vn")
		{
			// Vertex Norma
			sscanf(temp, "%s %f %f %f\n", specialChar, &vn.x, &vn.y, &vn.z);
			//inputString >> special >> vn.x >> vn.y >> vn.z;
			normals.push_back(vn);
		}
		else if (line2.substr(0, 2) == "g ")
		{
			//Group name
		}
		else if (line2.substr(0, 8) == "mtllib ")
		{
			//Material name
		}
		else if (line2.substr(0, 2) == "f ")
		{
			//Vertex Normal Indices in format f v1/vt1/vn1
			struct IndexStruct { int v; int vt; int vn; } faceIndices[3];
			sscanf(temp, "%s %i/%i/%i %i/%i/%i %i/%i/%i\n", specialChar,
				&faceIndices[0].v, &faceIndices[0].vt, &faceIndices[0].vn,
				&faceIndices[1].v, &faceIndices[1].vt, &faceIndices[1].vn,
				&faceIndices[2].v, &faceIndices[2].vt, &faceIndices[2].vn);
			//VertexModel tempModelData = {vertices[&faceIndices[0].v - 1], UV[&faceIndices[0].v], normals[&faceIndices[0].vn]};
			for (int i = 0; i < 3; i++)
				vertexData.push_back({ vertices[faceIndices[i].v - 1], UV[faceIndices[i].vt - 1], normals[faceIndices[i].vn - 1] });

		}
	}
	fileIn.close();

	this->m_vertexCount = vertexData.size();
	this->m_indexCount = vertexData.size();//vertices.size();
	this->m_model = new VertexModel[this->m_vertexCount];
	for (int j = 0; j < m_vertexCount; j++)
	{
		this->m_model[j] = vertexData[m_vertexCount - j - 1];
	}

	return true;
}

bool D3Object::LoadModelTXT(char * filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new VertexModel[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].position.x >> m_model[i].position.y >> m_model[i].position.z;
		fin >> m_model[i].UV.x >> m_model[i].UV.y;
		fin >> m_model[i].normal.x >> m_model[i].normal.y >> m_model[i].normal.z;
	}

	// Close the model file.
	fin.close();

	return true;
}

void D3Object::ReleaseModel()
{
	
}
