#ifndef _D3OBJECT_H_
#define _D3DOBJECT_H_

#include <d3d11.h>
#include "SimpleMath.h"
#include "StructLibrary.h"
#include "TextureObject.h"
#include <fstream>

#include <string>
#include <vector>
#include <sstream>
using namespace std;

class D3Object
{
private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureObject* m_texture;
	VertexModel* m_model;
public:
	D3Object();
	D3Object(const D3Object& original);
	~D3Object();
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFileName, char* textureFileName, FactoryObjectFormat format);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);	//Move model geometry to device context / Graphicscard
#pragma region
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
#pragma endregion getters

private:

	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext *deviceContext, char* textureFileName);
	void ReleaseTexture();

	bool LoadModel(char* fileName);
	bool LoadModelTXT(char* filename);
	void ReleaseModel();
};

#endif