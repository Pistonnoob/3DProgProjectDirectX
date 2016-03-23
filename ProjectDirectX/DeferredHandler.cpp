#include "DeferredHandler.h"



DeferredHandler::DeferredHandler()
{
	this->m_vertexShader = NULL;
	this->m_pixelShader = NULL;
	this->m_layout = NULL;
	this->m_samplerState = NULL;

	this->m_matrixBuffer = NULL;
}

DeferredHandler::DeferredHandler(const DeferredHandler & original)
{
}


DeferredHandler::~DeferredHandler()
{
}

bool DeferredHandler::Initialize(ID3D11Device * device, HWND hwnd)
{
	bool result = false;
	//Initialize the deferred shaders
	return true;
}

void DeferredHandler::Shutdown()
{
}

bool DeferredHandler::Render(ID3D11DeviceContext * deviceContext, int indexCount, WVPBufferStruct * matrices, LightStruct * light, ID3D11ShaderResourceView * resourceView, PixelMaterial * material)
{
	return false;
}

bool DeferredHandler::Render(ID3D11DeviceContext * deviceContext, int indexCount, Matrix * world, Matrix * view, Matrix * projection, LightStruct * light, ID3D11ShaderResourceView * resourceView, PixelMaterial * material)
{
	return false;
}

bool DeferredHandler::InitializeShader(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * psFilename)
{
	return false;
}

void DeferredHandler::FreeMemory()
{
}

void DeferredHandler::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
{
}

bool DeferredHandler::SetShaderParameters(ID3D11DeviceContext * deviceContext, WVPBufferStruct & matrices, LightStruct light, ID3D11ShaderResourceView * resourceView, PixelMaterial & material)
{
	return false;
}

void DeferredHandler::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
}
