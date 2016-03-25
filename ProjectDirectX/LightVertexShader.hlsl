#include "ShaderLibrary.hlsli"

cbuffer MatrixBuffer : register(b0) {
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

PS_IN_LIGHT main(VS_IN_LIGHT input)
{
	PS_IN_LIGHT output = (PS_IN_LIGHT)0;
	output.Pos.x = input.Pos.x;
	output.Pos.y = input.Pos.y;
	output.Pos.z = input.Pos.z;
	output.Pos.w = 1.0f;
	output.Pos = mul(output.Pos, worldMatrix);
	output.Pos = mul(output.Pos, viewMatrix);
	output.Pos = mul(output.Pos, projectionMatrix);

	output.UV = input.UV;

	return output;
}

//cbuffer MatrixBufferSimple
//{
//	matrix worldMatrix;
//	matrix viewMatrix;
//	matrix projectionMatrix;
//	matrix lightViewMatrix;
//	matrix lightProjectionMatrix;
//};
//
//struct VertexInput
//{
//	float4 position : POSITION;
//	float2 tex : TEXCOORD0;
//};
//
//struct PSInput
//{
//	float4 position : SV_POSITION;
//	float2 tex : TEXCOORD0;
//};
//
//PSInput main(VertexInput input)
//{
//	PSInput output;
//
//	input.position.w = 1.0f;
//
//	output.position = mul(input.position, worldMatrix);
//	output.position = mul(output.position, viewMatrix);
//	output.position = mul(output.position, projectionMatrix);
//
//	//Store the uv for output
//	output.tex = input.tex;
//
//	return output;
//}