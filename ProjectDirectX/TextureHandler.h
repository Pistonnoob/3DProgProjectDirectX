#ifndef _TEXTUREHANDLER_H_
#define _TEXTUREHANDLER_H_


#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>

#include "StructLibrary.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace std;
using namespace DirectX::SimpleMath;

class TextureHandler
{
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11GeometryShader* m_geometryShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_materialBuffer;
	ID3D11SamplerState* m_samplerState;
public:
	TextureHandler();
	TextureHandler(const TextureHandler& original);
	virtual ~TextureHandler();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, WVPBufferStruct &matrices, LightStruct & light, ID3D11ShaderResourceView* resourceView, PixelMaterial & material);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, Matrix &world, Matrix &view, Matrix &projection, LightStruct & light, ID3D11ShaderResourceView* resourceView, PixelMaterial & material);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);
	void FreeMemory();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, WVPBufferStruct & matrices, LightStruct light, ID3D11ShaderResourceView* resourceView, PixelMaterial & material);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
};

#endif