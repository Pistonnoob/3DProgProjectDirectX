#ifndef _DEFERREDBUFFER_H_
#define _DEFERREDBUFFER_H_

#include "StructLibrary.h"
#include <d3d11.h>
#include <DirectXMath.h>

static const int BUFFER_COUNT = 3;

class DeferredBuffer
{
private:
	int m_textureWidth;
	int m_textureHeight;

	//Rendertargets
	ID3D11Texture2D* m_renderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView* m_renderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView* m_shaderResourceViewArray[BUFFER_COUNT];
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
public:
	DeferredBuffer();
	DeferredBuffer(const DeferredBuffer& original);
	virtual ~DeferredBuffer();

	bool Initialize(ID3D11Device * device, int width, int height, float screenNear, float screenFar);
	void Shutdown();

	void SetRenderTargets(ID3D11DeviceContext* context);
	void ClearRenderTargets(ID3D11DeviceContext * context, DirectX::SimpleMath::Color color);
	void ClearRenderTargets(ID3D11DeviceContext * context, float red, float green, float blue, float alpha);

	ID3D11ShaderResourceView* GetShaderResourceView(int view);
private:
};

#endif