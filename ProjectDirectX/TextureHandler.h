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
	ID3D11GeometryShader* m_geometryShader; //Not yet implemented
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_samplerState;

	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);
	void FreeMemory();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, MatrixBufferStruct & matrices, ID3D11ShaderResourceView* resourceView);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
public:
	TextureHandler();
	TextureHandler(const TextureHandler& original);
	virtual ~TextureHandler();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, MatrixBufferStruct &matrices, ID3D11ShaderResourceView* resourceView);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, Matrix &world, Matrix &view, Matrix &projection, ID3D11ShaderResourceView* resourceView);

};

#endif