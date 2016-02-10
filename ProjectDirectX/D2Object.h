#ifndef _D2OBJECT_H_
#define _D2OBJECT_H_
#include <d3d11.h>
#include "SimpleMath.h"
#include "StructLibrary.h"
#include "TextureObject.h"
class D2Object
{
private:
	//Remember that the bufferstructure is defined using the buffer-description at creation.
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureObject* m_texture;


	//Handles the initialization and clean-up of our vertex & index buffer
	bool InitializeBuffers(ID3D11Device* device);
	void CleanBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName);
	void ReleaseTexture();
public:
	D2Object();
	D2Object(const D2Object& original);
	virtual ~D2Object();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFileName);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);	//Move model geometry to device context / Graphicscard
#pragma region
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
#pragma endregion getters


};
#endif
