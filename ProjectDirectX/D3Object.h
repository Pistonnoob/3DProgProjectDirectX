#ifndef _D3OBJECT_H_
#define _D3OBJECT_H_

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
	int GetIndexCount()const;
	ID3D11ShaderResourceView* GetTexture();
#pragma endregion getters

	bool CreateFromData(vector<VertexModel> vertexData);	//Allows for external definition of vertices into a D3Object
	bool InitializeBuffers(ID3D11Device* device);
	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext *deviceContext, char* textureFileName);
private:

	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	void ReleaseTexture();

	bool LoadModelObjLH(char* fileName);
	bool LoadModelObjRH(char* fileName);
	bool LoadModelObj(char* fileName, int invert = 1);
	bool LoadModelTXT(char* filename);
	void ReleaseModel();
};

#endif