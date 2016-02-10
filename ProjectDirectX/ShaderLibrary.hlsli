struct VS_IN
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
};

struct VS_IN_UV
{
	float4 Pos : POSITION;
	float2 UV : TEXCOORD;
};

struct VS_IN_3D
{
	float4 Pos : POSITION;
	float2 UV : TEXCOORD;
	float3 Normal : NORMAL;
};

struct GS_IN
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

struct GS_IN_UV
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};

struct GS_IN_3D
{
	float4 Pos : SV_POSITION;
	float4 Normal : NORMAL;
	float2 UV : TEXCOORD;
};

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

struct PS_IN_TEST
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};

struct PS_IN_UV
{
	float4 Pos : SV_POSITION;
	float4 Normal : NORMAL;
	float2 UV : TEXCOORD;
};

SamplerState p_sampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
