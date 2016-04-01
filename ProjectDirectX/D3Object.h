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
	int* m_indices;
	TextureObject* m_texture;
	VertexModel* m_model;
	ObjMaterial m_material;
	Matrix m_worldMatrix;
public:
	D3Object();
	D3Object(const D3Object& original);
	~D3Object();
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFileName, char* textureFileName, FactoryObjectFormat format);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);	//Move model geometry to device context / Graphicscard
#pragma region
	int GetIndexCount()const;
	int GetVertexCount()const;
	ObjMaterial GetMaterial()const;
	ID3D11ShaderResourceView* GetTexture();
	void GetWorldMatrix(Matrix& worldMatrix);
	std::vector<VertexModel> getVertexData();
	int GetVertexCount();
	VertexModel* getVertedData();
#pragma endregion getters
#pragma region
	void SetIndexCount(int);
	void SetVertexCount(int);
	void SetTexture(TextureObject*);
	void SetVertexBuffer(ID3D11Buffer*);
	void SetIndexBuffer(ID3D11Buffer*);
	void SetVertexModel(VertexModel*);

	bool SetMaterial(ObjMaterial material);
	void SetWorldMatrix(const Matrix worldMatrix);
#pragma endregion setters
	void ApplyMatrix(const Matrix applyToWorld);
	void SetIndices(vector<int> indices);

	bool CreateFromData(vector<VertexModel> vertexData, vector<int> indiceData);
	bool CreateFromData(vector<VertexModel> vertexData);	//Allows for loading an external definition of vertices into the D3Object
	bool InitializeBuffers(ID3D11Device* device);
	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext *deviceContext, char* textureFileName, TextureFormat fileFormat);
private:
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadModelObjLH(char* fileName);
	bool LoadModelObjRH(char* fileName);
	bool LoadModelObj(char* fileName, int invert = 1);
	bool LoadModelTXT(char* filename);

	void ShutdownBuffers();
	void ReleaseTexture();
	void ReleaseModel();
};

#endif