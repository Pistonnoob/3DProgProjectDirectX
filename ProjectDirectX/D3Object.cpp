#include "D3Object.h"



D3Object::D3Object()
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_vertexCount = 0;
	m_indexCount = 0;
	m_indices = NULL;
	m_texture = NULL;
	m_normalMap = NULL;
	m_model = NULL;
	
	m_material = DEFAULT_MATERIAL;

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = DirectX::XMMatrixIdentity();
}

D3Object::D3Object(const D3Object & original)
{
}


D3Object::~D3Object()
{
}

bool D3Object::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFileName, FactoryObjectFormat format)
{
//	bool result = false;
//
//	//Load the model data
//	switch (format)
//	{
//	case OBJ:
//		result = this->LoadModelObj(modelFilename);
//		break;
//	case OBJ_LH:
//		result = this->LoadModelObjLH(modelFilename);
//		break;
//	case OBJ_RH:
//		result = this->LoadModelObjRH(modelFilename);
//		break;
//	case TXT:
//		result = this->LoadModelTXT(modelFilename);
//		break;
//	default:
//		break;
//	}
////	result = this->LoadModelObj(modelFilename);
//	if (!result)
//		return false;
//
//	//Initialize vertex and index buffers.
//	result = InitializeBuffers(device);
//	if (!result)
//		return false;
//
//	//Load the texture for this model
//	result = this->LoadTexture(device, deviceContext, textureFileName);
//	if (!result)
//		return false;

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

int D3Object::GetIndexCount()const
{
	return this->m_indexCount;
}

ObjMaterial D3Object::GetMaterial() const
{
	return this->m_material;
}

ID3D11ShaderResourceView * D3Object::GetTexture()
{
	return this->m_texture->GetTextureView();
}

ID3D11ShaderResourceView * D3Object::GetNormalMap()
{
	return this->m_normalMap->GetTextureView();
}

void D3Object::GetWorldMatrix(Matrix & worldMatrix)
{
	worldMatrix = this->m_worldMatrix;
}

bool D3Object::SetMaterial(ObjMaterial material)
{
	this->m_material = material;
	return true;
}

void D3Object::SetWorldMatrix(const Matrix worldMatrix)
{
	this->m_worldMatrix = worldMatrix;
}

void D3Object::ApplyMatrix(const Matrix applyToWorld)
{
	this->m_worldMatrix = DirectX::XMMatrixMultiply(m_worldMatrix, applyToWorld);
}

void D3Object::SetIndices(vector<int> indices)
{
	this->m_indices = new int[indices.size()];
	for (int i = 0; i < indices.size(); i++)
	{
		this->m_indices[i] = indices[i];
	}
}

std::vector<VertexModel> D3Object::getVertexData()
{
	std::vector<VertexModel> points;
	for (int i = 0; i < m_vertexCount; i++)
	{
		points.push_back(this->m_model[i]);
	}
	return points;
}

int D3Object::GetVertexCount()
{
	return m_vertexCount;
}

VertexModel * D3Object::getVertedData()
{
	return m_model;
}

void D3Object::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	Vector3 tangent, binormal, normal;
	

	// Calculate the number of faces in the model.
	faceCount = m_vertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (i = 0; i<faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x = m_model[index].position.x;
		vertex1.y = m_model[index].position.y;
		vertex1.z = m_model[index].position.z;
		vertex1.tu = m_model[index].UV.x;
		vertex1.tv = m_model[index].UV.y;
		vertex1.nx = m_model[index].normal.x;
		vertex1.ny = m_model[index].normal.y;
		vertex1.nz = m_model[index].normal.z;
		index++;

		vertex2.x = m_model[index].position.x;
		vertex2.y = m_model[index].position.y;
		vertex2.z = m_model[index].position.z;
		vertex2.tu = m_model[index].UV.x;
		vertex2.tv = m_model[index].UV.y;
		vertex2.nx = m_model[index].normal.x;
		vertex2.ny = m_model[index].normal.y;
		vertex2.nz = m_model[index].normal.z;
		index++;

		vertex3.x = m_model[index].position.x;
		vertex3.y = m_model[index].position.y;
		vertex3.z = m_model[index].position.z;
		vertex3.tu = m_model[index].UV.x;
		vertex3.tv = m_model[index].UV.y;
		vertex3.nx = m_model[index].normal.x;
		vertex3.ny = m_model[index].normal.y;
		vertex3.nz = m_model[index].normal.z;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Calculate the new normal using the tangent and binormal.
		CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		m_model[index - 1].normal = normal;
		m_model[index - 1].tangent = tangent;
		m_model[index - 1].binormal = binormal;

		m_model[index - 2].normal = normal;
		m_model[index - 2].tangent = tangent;
		m_model[index - 2].binormal = binormal;

		m_model[index - 3].normal = normal;
		m_model[index - 3].tangent = tangent;
		m_model[index - 3].binormal = binormal;
	}

	return;
}

bool D3Object::CreateFromData(vector<VertexModel> vertexData, vector<int> indiceData)
{
	this->m_vertexCount = (int)vertexData.size();
	this->m_indexCount = (int)indiceData.size();
	this->m_model = new VertexModel[this->m_vertexCount];
	for (int j = 0; j < m_vertexCount; j++)
	{
		this->m_model[j] = vertexData[m_vertexCount - j - 1];
	}
	this->SetIndices(indiceData);
	return true;
}

bool D3Object::CreateFromData(vector<VertexModel> vertexData)
{
	this->m_vertexCount = (int)vertexData.size();
	this->m_indexCount = (int)vertexData.size();//vertices.size();
	this->m_model = new VertexModel[this->m_vertexCount];
	for (int j = 0; j < m_vertexCount; j++)
	{
		this->m_model[j] = vertexData[m_vertexCount - j - 1];
	}
	m_indices = new int[m_indexCount];
	for (int i = 0; i < m_indexCount; i++)
	{
		this->m_indices[i] = i;
	}
	return true;
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
		vertices[i].tangent = m_model[i].tangent;
		vertices[i].binormal = m_model[i].binormal;
	}

	for (int i = 0; i < this->m_indexCount; i++)
	{
		indices[i] = m_indices[i];
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

bool D3Object::LoadTexture(ID3D11Device *device, ID3D11DeviceContext *deviceContext, char * fileName, TextureFormat fileFormat)
{
	bool result = false;

	//Create the texture object
	this->m_texture = new TextureObject();
	if (this->m_texture == NULL)
	{
		return false;
	}
	//Now initialize the texture object
	result = m_texture->Initialize(device, deviceContext, fileName, fileFormat);
	if (!result)
	{
		return false;
	}

	return true;
}

bool D3Object::LoadTexture(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ObjMaterial * material)
{
	bool result = false;

	//Create the texture object
	this->m_texture = new TextureObject();
	if (this->m_texture == NULL)
	{
		return false;
	}
	//Now initialize the texture object
	result = m_texture->Initialize(device, deviceContext, material->texture, material->textureFormat);
	if (!result)
	{
		return false;
	}

	//Create the normal map object
	this->m_normalMap = new TextureObject();
	if (this->m_texture == NULL)
	{
		return false;
	}
	//Now initialize normal map object
	result = this->m_normalMap->Initialize(device, deviceContext, material->normalmap, material->normalmapFormat);
	if (!result)
	{
		return false;
	}

	return true;
}




bool D3Object::LoadModelObjLH(char * fileName)
{
	return this->LoadModelObj(fileName, 1);
}

bool D3Object::LoadModelObjRH(char * fileName)
{
	return this->LoadModelObj(fileName, -1);
}

bool D3Object::LoadModelObj(char * fileName, int invert)
{
	ifstream fileIn;
	string special = "", line = "", line2 = "";
	const unsigned int SPECIALCHARSIZE = 10;
	char specialChar[SPECIALCHARSIZE];
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
		strncpy_s(temp, 512, line2.c_str(), sizeof(temp));
		temp[sizeof(temp) - 1] = 0;

		inputString.str(line2);
		if (line2.substr(0, 2) == "v ")
		{
			// Vertex Position
			sscanf_s(temp, "%s %f %f %f\n", specialChar, SPECIALCHARSIZE, &vtx.x, &vtx.y, &vtx.z);
			vtx.z *= invert;
			//inputString >> special >> vtx.x >> vtx.y >> vtx.z;
			vertices.push_back(vtx);
		}
		else if (line2.substr(0, 2) == "vt")
		{
			// Vertex UV
			sscanf_s(temp, "%s %f %f\n", specialChar, SPECIALCHARSIZE, &vt.x, &vt.y);
			//inputString >> special >> vt.x >> vt.y;
			UV.push_back(vt);
		}
		else if (line2.substr(0, 2) == "vn")
		{
			// Vertex Norma
			sscanf_s(temp, "%s %f %f %f\n", specialChar, SPECIALCHARSIZE, &vn.x, &vn.y, &vn.z);
			//inputString >> special >> vn.x >> vn.y >> vn.z;
			normals.push_back(vn);
		}
		else if (line2.substr(0, 2) == "g ")
		{
			//Group name
		}
		else if (line2.substr(0, 7) == "mtllib")
		{
			//Material name
		}
		else if (line2.substr(0, 2) == "f ")
		{
			//Vertex Normal Indices in format f v1/vt1/vn1
			struct IndexStruct { int v; int vt; int vn; } faceIndices[3];
			sscanf_s(temp, "%s %i/%i/%i %i/%i/%i %i/%i/%i\n", specialChar, SPECIALCHARSIZE,
				&faceIndices[0].v, &faceIndices[0].vt, &faceIndices[0].vn,
				&faceIndices[1].v, &faceIndices[1].vt, &faceIndices[1].vn,
				&faceIndices[2].v, &faceIndices[2].vt, &faceIndices[2].vn);
			//VertexModel tempModelData = {vertices[&faceIndices[0].v - 1], UV[&faceIndices[0].v], normals[&faceIndices[0].vn]};
			for (int i = 0; i < 3; i++)
				vertexData.push_back({ vertices[faceIndices[i].v - 1], UV[faceIndices[i].vt - 1], normals[faceIndices[i].vn - 1] });

		}
	}
	fileIn.close();

	this->m_vertexCount = (int)vertexData.size();
	this->m_indexCount = (int)vertexData.size();//vertices.size();
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

void D3Object::ReleaseTexture()
{
	if (m_texture != NULL)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = NULL;
	}
	if (m_normalMap != NULL)
	{
		m_normalMap->Shutdown();
		delete m_normalMap;
		m_normalMap = NULL;
	}
	return;
}

void D3Object::ReleaseModel()
{
	if (this->m_model != NULL)
	{
		delete[] m_model;
		m_model = NULL;
	}
	if (this->m_indices != NULL)
	{
		delete[] m_indices;
		m_indices = NULL;
	}
}

void D3Object::CalculateTangentBinormal(TempVertexType& vertex1, TempVertexType& vertex2, TempVertexType& vertex3,
	Vector3& tangent, Vector3& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}

void D3Object::CalculateNormal(Vector3& tangent, Vector3& binormal, Vector3& normal)
{
	float length;


	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
}



