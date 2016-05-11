#include "ShaderLibrary.hlsli"

cbuffer MatrixBuffer : register(b0) {
	matrix worldMatrix;
	matrix inverseWorldMatrix;
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