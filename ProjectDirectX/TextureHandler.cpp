#include "TextureHandler.h"


TextureHandler::TextureHandler()
{
	this->m_vertexShader = NULL;
	this->m_pixelShader = NULL;
	this->m_layout = NULL;
	this->m_matrixBuffer = NULL;

	this->m_samplerState = NULL;
}

TextureHandler::TextureHandler(const TextureHandler & original)
{
}


TextureHandler::~TextureHandler()
{
}



bool TextureHandler::Initialize(ID3D11Device * device, HWND hwnd)
{
	bool result = false;
	WCHAR* vsName = (WCHAR*)VERTEXSHADER_NAME_WCHAR;
	WCHAR* gsName = (WCHAR*)GEOMETRYSHADER_NAME_WCHAR;
	WCHAR* psName = (WCHAR*)PIXELSHADER_NAME_WCHAR;
	//Initialize the vertex and pixel shaders
	result = this->InitializeShader(device, hwnd, vsName, gsName, psName);

	return result;
}

void TextureHandler::Shutdown()
{
	//Free the memory
	this->FreeMemory();

	return;
}

bool TextureHandler::Render(ID3D11DeviceContext * deviceContext, int indexCount, MatrixBufferStruct &matrices, ID3D11ShaderResourceView * resourceView)
{
	bool result = false;

	//Set the shader parameters that we will use when rendering
	result = SetShaderParameters(deviceContext, matrices, resourceView);
	if (!result)
	{
		return false;
	}

	//Now render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);

	return true;
}


bool TextureHandler::Render(ID3D11DeviceContext * deviceContext, int indexCount, Matrix & world, Matrix & view, Matrix & projection, ID3D11ShaderResourceView * resourceView)
{

	return this->Render(deviceContext, indexCount, MatrixBufferStruct{world, view, projection}, resourceView);
}

bool TextureHandler::InitializeShader(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * gsFilename, WCHAR * psFilename)
{
	HRESULT hResult;
	ID3DBlob* errorMessage = nullptr;
	ID3DBlob* pVS = nullptr;
	ID3DBlob* pGS = nullptr;
	ID3DBlob* pPS = nullptr;
	D3D11_BUFFER_DESC  matrixBufferDesc;

	D3D11_SAMPLER_DESC samplerDesc;

#pragma region

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	// Prefer higher CS shader profile when possible as CS 5.0 provides better performance on 11 - class hardware.
	LPCSTR profile = (device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "cs_4_0";
	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};
	hResult = D3DCompileFromFile(
		vsFilename,		// filename VERTEXSHADER_NAME_WCHAR
		defines,		// optional macros
		D3D_COMPILE_STANDARD_FILE_INCLUDE,		// optional include files
		"main",		// entry point
		"vs_4_0",		// shader model (target)
		flags,			// shader compile options
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
		&errorMessage	// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
		);
	if (FAILED(hResult))
	{
		//If it failed to complile we should be able to get the error from the data blob
		if (errorMessage != nullptr)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else	//If it failed but there was nothing in the error blob, then that means the file could not be located
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}
	errorMessage = NULL;
	profile = (device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "gs_5_0" : "gs_4_0";
	hResult = D3DCompileFromFile(
		gsFilename,		// filename GEOMETRY_SHADER_WCHAR
		defines,		// optional macros
		D3D_COMPILE_STANDARD_FILE_INCLUDE,		// optional include files
		"main",		// entry point
		"gs_4_0",		// shader model (target)
		flags,			// shader compile options
		0,				// effect compile options
		&pGS,			// double pointer to ID3DBlob		
		&errorMessage	// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
		);
	if (FAILED(hResult))
	{
		//If it failed to complile we should be able to get the error from the data blob
		if (errorMessage != nullptr)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, gsFilename);
		}
		else	//If it failed but there was nothing in the error blob, then that means the file could not be located
		{
			MessageBox(hwnd, gsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}
	profile = (device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";
	hResult = D3DCompileFromFile(
		psFilename,		// filename PIXELSHADER_NAME_WCHAR
		defines,		// optional macros
		D3D_COMPILE_STANDARD_FILE_INCLUDE,		// optional include files
		"main",		// entry point
		"ps_4_0",		// shader model (target)
		flags,			// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorMessage	// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
		);
	if (FAILED(hResult))
	{
		//If it failed to complile we should be able to get the error from the data blob
		if (errorMessage != nullptr)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else	//If it failed but there was nothing in the error blob, then that means the file could not be located
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}
#pragma endregion Compiling shaders
#pragma region
	hResult = device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &m_vertexShader);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = device->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &m_geometryShader);
	if (FAILED(hResult))
	{
		return false;
	}
	hResult = device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &m_pixelShader);
	if (FAILED(hResult))
	{
		return false;
	}
#pragma endregion Creating shader buffers
	unsigned int numElements = ARRAYSIZE(INPUT_DESC_3D);
	hResult = device->CreateInputLayout(INPUT_DESC_3D, numElements, pVS->GetBufferPointer(), pVS->GetBufferSize(), &m_layout);
	if (FAILED(hResult))
	{
		return false;
	}

	pVS->Release();
	pVS = nullptr;
	pGS->Release();
	pGS = nullptr;
	pPS->Release();
	pPS = nullptr;

	

	//Setup the constant buffer for the vertices
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferStruct);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Breath life into our samplerstate
	hResult = device->CreateSamplerState(&samplerDesc, &this->m_samplerState);
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}

void TextureHandler::FreeMemory()
{
	//Release the sampler state
	if (this->m_samplerState != NULL)
	{
		m_samplerState->Release();
		m_samplerState = NULL;
	}

	//Release the matrix buffer
	if (this->m_matrixBuffer != NULL)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = NULL;
	}

	//Release the layout
	if (this->m_layout != NULL)
	{
		m_layout->Release();
		m_layout = NULL;
	}
	//Release the shaders
	if (m_vertexShader != NULL)
	{
		m_vertexShader->Release();
		m_vertexShader = NULL;
	}
	if (m_geometryShader != NULL)
	{
		m_geometryShader->Release();
		m_geometryShader = NULL;
	}
	if (m_pixelShader != NULL)
	{
		m_pixelShader->Release();
		m_pixelShader = NULL;
	}

	return;
}

void TextureHandler::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
{

	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;

}

bool TextureHandler::SetShaderParameters(ID3D11DeviceContext * deviceContext, MatrixBufferStruct & matrices, ID3D11ShaderResourceView * resourceView)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferStruct* dataPtr = NULL;
	unsigned int bufferNumber = 0;

	//Transpose the matrices to prepare them for the shader (direct11 requires it?)
	matrices.world = matrices.world.Transpose();
	matrices.view = matrices.view.Transpose();
	matrices.projection = matrices.projection.Transpose();

	//Lock the m_matrix
	result = deviceContext->Map(this->m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//Get a pointer to the data
	dataPtr = (MatrixBufferStruct*)mappedResource.pData;

	//Now we copy the matrices into the mapped data
	dataPtr->world = matrices.world;
	dataPtr->view = matrices.view;
	dataPtr->projection = matrices.projection;

	//Unlock/unmap the constant buffer
	deviceContext->Unmap(this->m_matrixBuffer, 0);

	//set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	//Set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &this->m_matrixBuffer);
	deviceContext->GSSetConstantBuffers(bufferNumber, 1, &this->m_matrixBuffer);
	//deviceContext->PSSetConstantBuffers(0, 0, NULL);
	//Set the shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &resourceView);

	return true;
}

void TextureHandler::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	// Set the vertex input layout
	deviceContext->IASetInputLayout(this->m_layout);
	
	//Set the shaders used to render the model
	deviceContext->VSSetShader(this->m_vertexShader, NULL, 0);
	deviceContext->GSSetShader(this->m_geometryShader, NULL, 0);
	deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);

	deviceContext->PSSetSamplers(0, 1, &this->m_samplerState);

	deviceContext->DrawIndexed(indexCount, 0, 0);
	
	return;
}