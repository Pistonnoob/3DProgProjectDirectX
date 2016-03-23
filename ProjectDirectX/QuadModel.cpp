#include "QuadModel.h"



QuadModel::QuadModel()
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_vertexCount = 0;
	m_indexCount = 0;
}

QuadModel::QuadModel(const QuadModel & original)
{
}


QuadModel::~QuadModel()
{
}

bool QuadModel::Initialize(ID3D11Device * device, int width, int height)
{
	bool result;


	//Initialize the vertex and index buffer that hold the geometry for the ortho window model.
	result = InitializeBuffers(device, width, height);
	if (!result)
	{
		return false;
	}

	return true;
}

void QuadModel::Shutdown()
{
	this->FreeMemory();
}

void QuadModel::Render(ID3D11DeviceContext * context)
{
	//Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->RenderBuffers(context);
}

int QuadModel::GetIndexCount()
{
	return this->m_indexCount;
}

bool QuadModel::InitializeBuffers(ID3D11Device * device, int width, int height)
{
	float left = 0.0f, right = 0.0f, top = 0.0f, bottom = 0.0f;
	VertexUV* vertices = NULL;
	unsigned long* indices = 0;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	//Remember that the middle of the screen is (0, 0) for our 2D rendering.

	// Calculate the screen coordinates of the left side of the window.
	left = (float)((width / 2) * -1);

	// Calculate the screen coordinates of the right side of the window.
	right = left + (float)height;

	// Calculate the screen coordinates of the top of the window.
	top = (float)(width / 2);

	// Calculate the screen coordinates of the bottom of the window.
	bottom = top - (float)height;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexUV[m_vertexCount];
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

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = Vector3(left, top, 0.0f);  // Top left.
	vertices[0].UV = Vector2(0.0f, 0.0f);

	vertices[1].position = Vector3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].UV = Vector2(1.0f, 1.0f);

	vertices[2].position = Vector3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].UV = Vector2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = Vector3(left, top, 0.0f);  // Top left.
	vertices[3].UV = Vector2(0.0f, 0.0f);

	vertices[4].position = Vector3(right, top, 0.0f);  // Top right.
	vertices[4].UV = Vector2(1.0f, 0.0f);

	vertices[5].position = Vector3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].UV = Vector2(1.0f, 1.0f);

	// Load the index array with data.
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	//Create the buffers
	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexUV) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
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

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void QuadModel::FreeMemory()
{
	//Release the buffers
	if (m_vertexBuffer != NULL)
	{
		this->m_vertexBuffer->Release();
		this->m_vertexBuffer = NULL;
		this->m_vertexCount = 0;
	}
	if (m_indexBuffer != NULL)
	{
		this->m_indexBuffer->Release();
		this->m_indexBuffer = NULL;
		this->m_indexCount = 0;
	}
}

void QuadModel::RenderBuffers(ID3D11DeviceContext * context)
{
	unsigned int stride = 0;
	unsigned int offset = 0;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexUV);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
