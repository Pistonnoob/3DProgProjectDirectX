#ifndef _QUADMODEL_H_
#define _QUADMODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include "StructLibrary.h"

class QuadModel
{
private:	//Variables
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
public:
	QuadModel();
	QuadModel(const QuadModel& original);
	virtual ~QuadModel();

	bool Initialize(ID3D11Device* device, int width, int height);
	void Shutdown();
	void Render(ID3D11DeviceContext* context);

	int GetIndexCount();

private:	//Functions
	bool InitializeBuffers(ID3D11Device* device, int width, int height);
	void FreeMemory();
	void RenderBuffers(ID3D11DeviceContext* context);
};

#endif