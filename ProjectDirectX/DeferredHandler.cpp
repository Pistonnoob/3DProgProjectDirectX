#include "DeferredHandler.h"



DeferredHandler::DeferredHandler()
{
	this->m_vertexShader = NULL;
	this->m_geometryShader = NULL;
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
	WCHAR* vsName = (WCHAR*)VERTEXSHADER_DEFERRED_NAME_WCHAR;
	WCHAR* gsName = (WCHAR*)GEOMETRYSHADER_DEFERRED_NAME_WCHAR;
	WCHAR* psName = (WCHAR*)PIXELSHADER_DEFERRED_NAME_WCHAR;
	//Initialize the deferred shaders
	result = this->InitializeShader(device, hwnd, vsName, gsName, psName);
	if (!result)
	{
		return false;
	}
	return true;
}

void DeferredHandler::Shutdown()
{
	this->FreeMemory();
}

bool DeferredHandler::Render(ID3D11DeviceContext * deviceContext, int indexCount, WVPBufferStruct * matrices, ID3D11ShaderResourceView * resourceView, PixelMaterial * material)
{
	bool result = false;

	//Set the shader parameters that we will use when rendering
	result = SetShaderParameters(deviceContext, matrices, material, resourceView);
	if (!result)
	{
		return false;
	}

	//Now render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool DeferredHandler::InitializeShader(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR* gsFilename, WCHAR * psFilename)
{
	HRESULT hResult;
	ID3DBlob* errorMessage = nullptr;
	ID3DBlob* pVS = nullptr;
	ID3DBlob* pGS = nullptr;
	ID3DBlob* pPS = nullptr;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC materialBufferDesc;

#pragma region

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	// Prefer higher CS shader profile when possible as CS 5.0 provides better performance on 11 - class hardware.
	LPCSTR profile = (device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";
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
		"vs_5_0",		// shader model (target)
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
		"gs_5_0",		// shader model (target)
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
	errorMessage = NULL;
	profile = (device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";
	hResult = D3DCompileFromFile(
		psFilename,		// filename PIXELSHADER_NAME_WCHAR
		defines,		// optional macros
		D3D_COMPILE_STANDARD_FILE_INCLUDE,		// optional include files
		"main",		// entry point
		"ps_5_0",		// shader model (target)
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

	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	// Create the vertex input layout description.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	hResult = device->CreateInputLayout(polygonLayout, numElements, pVS->GetBufferPointer(), pVS->GetBufferSize(),
		&m_layout);
	//unsigned int numElements = ARRAYSIZE(INPUT_DESC_DEFERRED);
	//hResult = device->CreateInputLayout(INPUT_DESC_DEFERRED, numElements, pVS->GetBufferPointer(), pVS->GetBufferSize(), &m_layout);
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

	//Create the material buffer
	memset(&materialBufferDesc, 0, sizeof(materialBufferDesc));
	materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBufferDesc.MiscFlags = 0;
	materialBufferDesc.StructureByteStride = 0;
	materialBufferDesc.ByteWidth = sizeof(PixelMaterial);

	hResult = device->CreateBuffer(&materialBufferDesc, NULL, &m_materialBuffer);
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

void DeferredHandler::FreeMemory()
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

	//Release the material buffer
	if (m_materialBuffer != NULL)
	{
		m_materialBuffer->Release();
		m_materialBuffer = NULL;
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
}

void DeferredHandler::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
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

bool DeferredHandler::SetShaderParameters(ID3D11DeviceContext * deviceContext, WVPBufferStruct* matrices, PixelMaterial* material, ID3D11ShaderResourceView * resourceView)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	WVPBufferStruct* dataPtr = NULL;
	PixelMaterial* materialPtr = NULL;
	unsigned int bufferNumber = 0;
	//Transpose the matrices to prepare them for the shader

	//Lock the m_matrix
	result = deviceContext->Map(this->m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//Get a pointer to the data
	dataPtr = (WVPBufferStruct*)mappedResource.pData;
	WVPBufferStruct tempMatrices;
	tempMatrices.world = matrices->world.Transpose();
	tempMatrices.view = matrices->view.Transpose();
	tempMatrices.projection = matrices->projection.Transpose();
	//Now we copy the matrices into the mapped data
	dataPtr->world = tempMatrices.world;
	dataPtr->view = tempMatrices.view;
	dataPtr->projection = tempMatrices.projection;

	//Unlock/unmap the constant buffer
	deviceContext->Unmap(this->m_matrixBuffer, 0);

	//set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	// Lock/map the m_material
	result = deviceContext->Map(this->m_materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	//Get a pointer to the data
	materialPtr = (PixelMaterial*)mappedResource.pData;
	materialPtr->Ka = material->Ka;
	materialPtr->Kd = material->Kd;
	materialPtr->Ks = material->Ks;
	materialPtr->Ns = material->Ns;
	materialPtr->padding1 = material->padding1;
	//memcpy(&materialPtr, &material, sizeof(material));

	//Unlock/unmap the constant buffer
	deviceContext->Unmap(this->m_materialBuffer, 0);


	//Set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(0, 1, &this->m_matrixBuffer);
	//deviceContext->GSSetConstantBuffers(bufferNumber, 1, &this->m_matrixBuffer);
	deviceContext->PSSetConstantBuffers(1, 1, &this->m_materialBuffer);

	//Set the shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &resourceView);

	return true;
}

void DeferredHandler::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
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
