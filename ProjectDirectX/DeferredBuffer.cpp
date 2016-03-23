#include "DeferredBuffer.h"



DeferredBuffer::DeferredBuffer()
{
	for (int i = 0; i<BUFFER_COUNT; i++)
	{
		m_renderTargetTextureArray[i] = NULL;
		m_renderTargetViewArray[i] = NULL;
		m_shaderResourceViewArray[i] = NULL;
	}

	m_depthStencilBuffer = NULL;
	m_depthStencilView = NULL;
}

DeferredBuffer::DeferredBuffer(const DeferredBuffer & original)
{
}


DeferredBuffer::~DeferredBuffer()
{
}

bool DeferredBuffer::Initialize(ID3D11Device * device, int width, int height, float screenNear, float screenFar)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// Store the width and height of the render texture.
	m_textureWidth = width;
	m_textureHeight = height;

	// Initialize the render target texture description.
	memset(&textureDesc, 0, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target textures.
	for (int i = 0; i<BUFFER_COUNT; i++)
	{
		result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTextureArray[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target views.
	for (int i = 0; i<BUFFER_COUNT; i++)
	{
		result = device->CreateRenderTargetView(m_renderTargetTextureArray[i], &renderTargetViewDesc, &m_renderTargetViewArray[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource views.
	for (int i = 0; i<BUFFER_COUNT; i++)
	{
		result = device->CreateShaderResourceView(m_renderTargetTextureArray[i], &shaderResourceViewDesc, &m_shaderResourceViewArray[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = m_textureWidth;
	depthBufferDesc.Height = m_textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the viewport for rendering.
	m_viewport.Width = (float)m_textureWidth;
	m_viewport.Height = (float)m_textureHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	return true;
}

void DeferredBuffer::Shutdown()
{
	int i;


	if (m_depthStencilView != NULL)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilBuffer != NULL)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	for (i = 0; i<BUFFER_COUNT; i++)
	{
		if (m_shaderResourceViewArray[i] != NULL)
		{
			m_shaderResourceViewArray[i]->Release();
			m_shaderResourceViewArray[i] = 0;
		}

		if (m_renderTargetViewArray[i] != NULL)
		{
			m_renderTargetViewArray[i]->Release();
			m_renderTargetViewArray[i] = 0;
		}

		if (m_renderTargetTextureArray[i] != NULL)
		{
			m_renderTargetTextureArray[i]->Release();
			m_renderTargetTextureArray[i] = 0;
		}
	}

}

void DeferredBuffer::SetRenderTargets(ID3D11DeviceContext * context)
{
	// Bind the render target view array and depth stencil buffer to the output render pipeline.
	context->OMSetRenderTargets(BUFFER_COUNT, m_renderTargetViewArray, m_depthStencilView);

	// Set the viewport.
	context->RSSetViewports(1, &m_viewport);
}

void DeferredBuffer::ClearRenderTargets(ID3D11DeviceContext * context, DirectX::SimpleMath::Color color)
{
	this->ClearRenderTargets(context, color.R, color.G, color.B, color.A);
}

void DeferredBuffer::ClearRenderTargets(ID3D11DeviceContext * context, float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the render target buffers.
	for (int i = 0; i<BUFFER_COUNT; i++)
	{
		context->ClearRenderTargetView(m_renderTargetViewArray[i], color);
	}

	// Clear the depth buffer.
	context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView * DeferredBuffer::GetShaderResourceView(int view)
{
	return this->m_shaderResourceViewArray[view];
}


