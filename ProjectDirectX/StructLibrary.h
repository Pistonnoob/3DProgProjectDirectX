#pragma once

#include "SimpleMath.h"
#include <string>
using namespace DirectX::SimpleMath;

static const float DEGREES_TO_RADIANS = 0.0174532925f;

enum FactoryObjectFormat
{
	OBJ,
	OBJ_RH,
	OBJ_LH,
	TXT
};

enum TextureFormat
{
	TARGA,
	PNG,
	JPEG
};

enum Keyboard_Key
{
	KEY_W = 87,
	KEY_A = 65,
	KEY_S = 83,
	KEY_D = 68,
	KEY_RESET = 82
};

struct Vertex
{
	Vector3 position;
	Vector4 color;
};

struct VertexUV
{
	Vector3 position;
	Vector2 UV;
};

struct VertexModel
{
	Vector3 position;
	Vector2 UV;
	Vector3 normal;
};


struct WVPBufferStruct
{
	Matrix world;
	Matrix view;
	Matrix projection;
};

struct LightStruct
{
	Vector4 lightColor;
	Vector4 lightPos;
	Vector4 lightDir;
};

struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

struct PngHeader
{

};

struct JPEGHeader
{

};

struct ObjMaterial
{
	static const int MATERIAL_NAME_LENGTH = 30;
	char name[MATERIAL_NAME_LENGTH];	//The mtl name
	int illum;		//Illumination models
	Vector3 Kd;		//Diffuse Color
	Vector3 Ka;		//Ambient Color
	Vector3 Tf;		
	char texture[MATERIAL_NAME_LENGTH];	//The texture file
	TextureFormat textureFormat;		//The texture file type
};



// input layout (verified using vertex shader)
//static const D3D11_INPUT_ELEMENT_DESC inputDescColor[] = {
//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//};

//Material description using Position and UV
static const D3D11_INPUT_ELEMENT_DESC INPUT_DESC_UV[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};	//D3D11_APPEND_ALIGNED_ELEMENT is set so new elements are pushed right at the end of the last instead of a static variable

//The one using both a UV and a Normal
static const D3D11_INPUT_ELEMENT_DESC INPUT_DESC_3D[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

//Default Material
static const ObjMaterial DEFAULT_MATERIAL = {"missing", 1, Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), "missing_texture", TextureFormat::JPEG};

//Default Texture
static const char DEFAULT_TEXTURE[] = "missing_texture.png";

//Shader names
static const char *VERTEXSHADER_NAME = "VertexShader.hlsl", *GEOMETRYSHADER_NAME = "GeometryShader.hlsl", *PIXELSHADER_NAME = "PixelShader.hlsl";
static const WCHAR *VERTEXSHADER_NAME_WCHAR = L"VertexShader.hlsl", *GEOMETRYSHADER_NAME_WCHAR = L"GeometryShader.hlsl", *PIXELSHADER_NAME_WCHAR = L"PixelShader.hlsl";

static const char* TEST_TEXTURE_NAME = "../Textures/stone01.tga";