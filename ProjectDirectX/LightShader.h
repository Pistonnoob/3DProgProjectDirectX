#ifndef _LIGHTSHADER_H_
#define _LIGHTSHADER_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>

#include <DirectXMath.h>

#include "StructLibrary.h"
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace std;

class LightShader
{
private:	//variables
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_samplerState;

	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;

public:
	LightShader();
	LightShader(const LightShader& original);
	virtual ~LightShader();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, WVPBufferStruct* matrices, ID3D11ShaderResourceView** resources, LightStructTemp* lights);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, WVPBufferStruct* matrices, ID3D11ShaderResourceView* color, ID3D11ShaderResourceView* normal, ID3D11ShaderResourceView* position, ID3D11ShaderResourceView* diffuse, ID3D11ShaderResourceView* specular, LightStructTemp* light);
private:	//functions
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void FreeMemory();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, WVPBufferStruct * matrices, ID3D11ShaderResourceView** resources, LightStructTemp* light);
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, WVPBufferStruct * matrices, ID3D11ShaderResourceView* color, ID3D11ShaderResourceView* normal, ID3D11ShaderResourceView* position, ID3D11ShaderResourceView* diffuse, ID3D11ShaderResourceView* specular, LightStructTemp* light);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
};

#endif