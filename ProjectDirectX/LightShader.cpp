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

	WCHAR* vsName = (WCHAR*)VERTEXSHADER_LIGHT_NAME_WCHAR;
	WCHAR* psName = (WCHAR*)PIXELSHADER_LIGHT_NAME_WCHAR;
	//Initialize the deferred shaders
	result = this->InitializeShader(device, hwnd, vsName, psName);
	if (!result)
	{
		return false;
	}
	return true;
}

void LightShader::Shutdown()
{
	this->FreeMemory();
}

bool LightShader::Render(ID3D11DeviceContext * deviceContext, int indexCount, WVPBufferStruct * matrices, ID3D11ShaderResourceView ** resources, LightStructTemp* light)
{
	bool result = false;

	//Set the shader parameters that we will use when rendering
	result = SetShaderParameters(deviceContext, matrices, resources, light);
	if (!result)
	{
		return false;
	}

	//Now render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool LightShader::Render(ID3D11DeviceContext * deviceContext, int indexCount, WVPBufferStruct * matrices, ID3D11ShaderResourceView * color, ID3D11ShaderResourceView * normal, ID3D11ShaderResourceView * position, ID3D11ShaderResourceView* diffuse, ID3D11ShaderResourceView* specular, LightStructTemp * light)
{
	bool result = false;

	//Set the shader parameters that we will use when rendering
	result = SetShaderParameters(deviceContext, matrices, color, normal, position, diffuse, specular, light);
	if (!result)
	{
		return false;
	}

	//Now render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool LightShader::InitializeShader(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * psFilename)
{
	HRESULT hResult;
	ID3DBlob* errorMessage = nullptr;
	ID3DBlob* pVS = nullptr;
	ID3DBlob* pPS = nullptr;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

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
	hResult = device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &m_pixelShader);
	if (FAILED(hResult))
	{
		return false;
	}
#pragma endregion Creating shader buffers

	unsigned int numElements = ARRAYSIZE(INPUT_DESC_LIGHT);
	hResult = device->CreateInputLayout(INPUT_DESC_LIGHT, numElements, pVS->GetBufferPointer(), pVS->GetBufferSize(), &m_layout);
	if (FAILED(hResult))
	{
		return false;
	}

	pVS->Release();
	pVS = nullptr;
	pPS->Release();
	pPS = nullptr;



	//Setup the constant buffer for the vertices
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(WVPBufferStruct);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightStructTemp);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	hResult = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
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

void LightShader::FreeMemory()
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
	if (this->m_lightBuffer != NULL)
	{
		m_lightBuffer->Release();
		m_lightBuffer = NULL;
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
	if (m_pixelShader != NULL)
	{
		m_pixelShader->Release();
		m_pixelShader = NULL;
	}
}

void LightShader::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = (unsigned long)errorMessage->GetBufferSize();

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

	// Show a message on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool LightShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, WVPBufferStruct * matrices, ID3D11ShaderResourceView ** resources, LightStructTemp* lights)
{

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	WVPBufferStruct* dataPtr = NULL;
	LightStructTemp* dataPtr2 = NULL;
	unsigned int bufferNumber = 0;
	
	//Lock the m_matrix
	result = deviceContext->Map(this->m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//Get a pointer to the data
	dataPtr = (WVPBufferStruct*)mappedResource.pData;

	//Now we copy the matrices into the mapped data transposed
	dataPtr->world = matrices->world.Transpose();
	dataPtr->view = matrices->view.Transpose();
	dataPtr->projection = matrices->projection.Transpose();

	//Unlock/unmap the constant buffer
	deviceContext->Unmap(this->m_matrixBuffer, 0);

	//set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	//Set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	//Set the shader texture resource in the pixel shader.
	for (int i = 0; i < DEFERRED_BUFFER_COUNT; i++)
	{
		deviceContext->PSSetShaderResources(i, 1, &resources[i]);
	}

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightStructTemp*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	dataPtr2->lightPos = lights->lightPos;
	dataPtr2->padding = 0.0f;
	dataPtr2->lightColor = lights->lightColor;
	// Unlock the constant buffer.
	deviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}

bool LightShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, WVPBufferStruct * matrices, ID3D11ShaderResourceView * color, ID3D11ShaderResourceView * normal, ID3D11ShaderResourceView * position, ID3D11ShaderResourceView* diffuse, ID3D11ShaderResourceView* specular, LightStructTemp * light)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	WVPBufferStruct* dataPtr = NULL;
	LightStructTemp* dataPtr2 = NULL;
	unsigned int bufferNumber = 0;

	//Lock the m_matrix
	result = deviceContext->Map(this->m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//Get a pointer to the data
	dataPtr = (WVPBufferStruct*)mappedResource.pData;

	//Now we copy the matrices into the mapped data transposed
	dataPtr->world = matrices->world.Transpose();
	dataPtr->view = matrices->view.Transpose();
	dataPtr->projection = matrices->projection.Transpose();

	//Unlock/unmap the constant buffer
	deviceContext->Unmap(this->m_matrixBuffer, 0);

	//set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	//Set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	//Set the shader texture resource in the pixel shader.

	deviceContext->PSSetShaderResources(0, 1, &color);
	deviceContext->PSSetShaderResources(1, 1, &normal);
	deviceContext->PSSetShaderResources(2, 1, &position);
	deviceContext->PSSetShaderResources(3, 1, &diffuse);
	deviceContext->PSSetShaderResources(4, 1, &specular);
	

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightStructTemp*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	dataPtr2->lightPos = light->lightPos;
	dataPtr2->padding = 0.0f;
	dataPtr2->lightColor = light->lightColor;
	dataPtr2->padding1 = 0.0f;
	dataPtr2->cameraPos = light->cameraPos;
	// Unlock the constant buffer.
	deviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}

void LightShader::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	//Set the vertex input layout
	deviceContext->IASetInputLayout(this->m_layout);

	//Set the shaders used to render the model
	deviceContext->VSSetShader(this->m_vertexShader, NULL, 0);
	deviceContext->GSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);

	//Set the sampler state in the pixel shader
	deviceContext->PSSetSamplers(0, 1, &this->m_samplerState);

	//Render the quad
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
