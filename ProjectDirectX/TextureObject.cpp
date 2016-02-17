#include "TextureObject.h"




TextureObject::TextureObject()
{
	this->m_targaData = 0;
	this->m_texture = NULL;
	this->m_textureView = NULL;
}

TextureObject::TextureObject(const TextureObject & original)
{
}


TextureObject::~TextureObject()
{
}

bool TextureObject::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, char * fileName)
{
	bool result = false;
	int width = 0, height = 0;
	HRESULT hResult;
	unsigned int rowPitch = 0;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	//Load the image!!!!
	result = LoadTarga(fileName, height, width);
	if (!result)
	{
		return false;
	}

	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	//Create the empty picture and update it with the data. We could have initiated it with the data but this works.
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult))
	{
		return false;
	}

	//const size_t cSize = strlen(fileName) + 1;
	//std::wstring wc(cSize, L'#');
	//mbstowcs(&wc[0], fileName, cSize);
	//ID3D11Resource* Res;
	////CreateWICTextureFromFile(device, deviceContext, "", m_texture, &m_textureView, 0);
	////hResult = CreateWICTextureFromFile(device, deviceContext, wc.c_str(), NULL, &m_textureView, 0);
	//hResult = CreateWICTextureFromFile(device, wc.c_str(), &Res, &m_textureView);
	//if (FAILED(hResult))
	//{
	//	return false;
	//}

//#ifdef _DEBUG
//	hResult = SaveWICTextureToFile(deviceContext, Res, GUID_ContainerFormatBmp, L"SCREENSHOT.BMP");
//#endif
	//Set the row pitch of the targa image data.
	rowPitch = (width * 4) * sizeof(unsigned char);
	//Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(this->m_texture, 0, NULL, m_targaData, rowPitch, 0);

	//Setup the shader resource view description
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	//Create the shader resource view for the texture
	hResult = device->CreateShaderResourceView(this->m_texture, &srvDesc, &this->m_textureView);
	if (FAILED(hResult))
	{
		return false;
	}

	//Generate mipmaps for this texture. For higher quality we may want to load our own MipMap levels manually.
	deviceContext->GenerateMips(m_textureView);
	
	//Release the targa image data now that the image data has been loaded into the texture
	delete[] this->m_targaData;
	this->m_targaData = NULL;

	return true;
}

void TextureObject::Shutdown()
{
	//Realease the texture view
	if (this->m_textureView != NULL)
	{
		m_textureView->Release();
		m_textureView = NULL;
	}
	//Release the texture
	if (this->m_texture != NULL)
	{
		m_texture->Release();
		m_texture = NULL;
	}
	//Release the targa-data
	if (this->m_targaData != NULL)
	{
		delete[] m_targaData;
		m_targaData = NULL;
	}

	return;
}

ID3D11ShaderResourceView * TextureObject::GetTextureView()
{
	return this->m_textureView;
}



bool TextureObject::LoadTarga(char * fileName, int & height, int & width)
{
	int error = 0, bpp = 0, imageSize = 0, index = 0, i = 0, j = 0, k = 0;
	FILE* filePtr = NULL;
	unsigned int count = 0;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	//Open the file for binary reading
	error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0)
	{
		return false;
	}

	//Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	//Get the important information from the header
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	//Check that it is 32 bit and not 24 bit
	if (bpp != 32)
	{
		return false;
	}

	//Calculate the size of the image data
	imageSize = width * height * 4;
	
	//Allocate memory or image data.
	targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{
		return false;
	}

	//Read in the image data
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	//close the file
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	//Allocate memory for the targa destination data.
	this->m_targaData = new unsigned char[imageSize];
	if (!m_targaData)
	{
		return false;
	}

	//Initialize the index into the targa destination data array
	index = 0;

	// Initialize the index into the targa image data.
	k = (width * height * 4) - (width * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
	for (j = 0; j<height; j++)
	{
		for (i = 0; i<width; i++)
		{
			m_targaData[index + 0] = targaImage[k + 2];  // Red.
			m_targaData[index + 1] = targaImage[k + 1];  // Green.
			m_targaData[index + 2] = targaImage[k + 0];  // Blue
			m_targaData[index + 3] = targaImage[k + 3];  // Alpha

														 // Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (width * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	delete[] targaImage;
	targaImage = 0;

	return true;

}
