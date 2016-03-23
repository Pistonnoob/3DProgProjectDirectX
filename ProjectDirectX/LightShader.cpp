#include "LightShader.h"



LightShader::LightShader()
{
	m_vertexShader = NULL;
	m_pixelShader = NULL;
	m_layout = NULL;
	m_samplerState = NULL;
	m_matrixBuffer = NULL;
	m_lightBuffer = NULL;
}

LightShader::LightShader(const LightShader & original)
{
}


LightShader::~LightShader()
{
}

bool LightShader::Initialize(ID3D11Device * device, HWND hwnd)
{
	bool result = false;

	WCHAR* vsName = (WCHAR*)VERTEXSHADER_DEFERRED_NAME_WCHAR;
	WCHAR* psName = (WCHAR*)PIXELSHADER_DEFERRED_NAME_WCHAR;
	//Initialize the deferred shaders
	result = this->InitializeShader(device, hwnd, vsName, psName);

	return true;
}

void LightShader::Shutdown()
{
}

bool LightShader::Render(ID3D11DeviceContext * deviceContext, int indexCount, WVPBufferStruct * matrices, ID3D11ShaderResourceView ** resources, LightStructTemp* lights, int lightCount)
{
	return false;
}

bool LightShader::InitializeShader(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * psFilename)
{
	return false;
}

void LightShader::FreeMemory()
{
}

void LightShader::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
{
}

bool LightShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, WVPBufferStruct * matrices, ID3D11ShaderResourceView ** resourceView, LightStructTemp* lights, int lightCount)
{
	return false;
}

void LightShader::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
}
