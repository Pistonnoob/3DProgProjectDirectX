#ifndef _TEXTUREOBJECT_H_
#define _TEXTUREOBJECT_H_

#include "StructLibrary.h"
#include <d3d11.h>
#include <stdio.h>

//#include "WICTextureLoader.h"

#include "ScreenGrab.h"
#include <wincodec.h>
#include "libraries\DirectXTex.h"

using namespace DirectX;

class TextureObject
{
private:
	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11Resource* m_anotherData;
	ID3D11ShaderResourceView* m_textureView;

	//Extend with more loading functions to support other file formats. Also other headers.
	bool LoadTarga(char* name, int& height, int& width);
public:
	TextureObject();
	TextureObject(const TextureObject& original);
	virtual ~TextureObject();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName, TextureFormat fileFormat = TextureFormat::JPEG);
	void Shutdown();

	ID3D11ShaderResourceView* GetTextureView();
};

#endif