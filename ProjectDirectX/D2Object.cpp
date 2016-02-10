#include "D2Object.h"





D2Object::D2Object()
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_vertexCount = 0;
	m_indexCount = 0;
	m_texture = NULL;
}

D2Object::D2Object(const D2Object & original)
{
}


D2Object::~D2Object()
{
}


//PUBLIC----------------------------------------------------------------------------------
bool D2Object::Initialize(ID3D11Device * device, ID3D11DeviceContext* deviceContext, char* textureFileName)
{
	bool result = true;

	//Initialize the vertex and index buffer
	result = this->InitializeBuffers(device);
	if (!result)
		return false;

	//Load the texture
	result = LoadTexture(device, deviceContext, textureFileName);
	if (!result)
		return false;

	return true;
}

void D2Object::Shutdown()
{
	//Release the texture.
	this->ReleaseTexture();
	//Release the buffers.
	this->CleanBuffers();
	return;
}

void D2Object::Render(ID3D11DeviceContext * deviceContext)
{
	this->RenderBuffers(deviceContext);
	return;
}

int D2Object::GetIndexCount()
{
	return this->m_indexCount;
}

ID3D11ShaderResourceView * D2Object::GetTexture()
{
	return this->m_texture->GetTextureView();
}


//PRIVATE----------------------------------------------------------------------------------
bool D2Object::InitializeBuffers(ID3D11Device * device)
{
	VertexUV* vertices = NULL;
	unsigned long* indices = NULL;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	
	//Set the number of vertices in the vertex array.
	this->m_vertexCount = 3;
	this->m_indexCount = 3;

	//Create buffers and check for successfull completion.
	//Create the vertex array. Fun part :D
	vertices = new VertexUV[this->m_vertexCount];
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
	//Load the vertex data
	vertices[0].position = Vector3(-1.0f, -1.0f, 0.0f);
	vertices[0].UV = Vector2(0.0f, 1.0f);
	//vertices[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = Vector3(0.0f, 1.0f, 0.0f);
	vertices[1].UV = Vector2(0.5f, 0.0f);
	//vertices[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = Vector3(1.0f, -1.0f, 0.0f);
	vertices[2].UV = Vector2(1.0f, 1.0f);
	//vertices[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	/*vertices[3].position = Vector3(1.0f, 1.0f, 0.0f);
	vertices[3].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);*/


	//Load the index data
	indices[0] = 0;	//Bottom left.
	indices[1] = 1; //Top Left.
	indices[2] = 2;	//Bottom right.
	//indices[3] = 3; //Top Right

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexUV) * this->m_vertexCount;
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

void D2Object::CleanBuffers()
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

void D2Object::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride = 0;
	unsigned int offset = 0;

	stride = sizeof(VertexUV);

	//Set the vertex buffer for the input assembler.
	deviceContext->IASetVertexBuffers(0, 1, &this->m_vertexBuffer, &stride, &offset);

	//Do the same for the index buffer.
	deviceContext->IASetIndexBuffer(this->m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Actually set the topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool D2Object::LoadTexture(ID3D11Device * device, ID3D11DeviceContext * deviceContext, char * fileName)
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

void D2Object::ReleaseTexture()
{
	//Release the texture object.
	if (this->m_texture != NULL)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = NULL;
	}

	return;
}




