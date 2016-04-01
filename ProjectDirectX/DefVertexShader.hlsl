#include "ShaderLibrary.hlsli"

cbuffer MatrixBuffer : register(b0) {
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

GS_IN_DEF main(VS_IN_DEF input)
{
	GS_IN_DEF output = (GS_IN_DEF)0;
	output.Pos.x = input.Pos.x;
	output.Pos.y = input.Pos.y;
	output.Pos.z = input.Pos.z;
	output.Pos.w = 1.0f;
	
	output.UV = input.UV;
	output.Pos = mul(output.Pos, worldMatrix);
	output.WorldPos = output.Pos;
	output.Pos = mul(output.Pos, viewMatrix);
	output.Pos = mul(output.Pos, projectionMatrix);
	
	output.Normal = float4(input.Normal.x, input.Normal.y, input.Normal.z, 0);
	output.Normal = mul(output.Normal, worldMatrix);
	output.Normal = normalize(output.Normal);

	return output;
}

//PS_IN_DEF main(VS_IN_DEF input)
//{
//	PS_IN_DEF output = (PS_IN_DEF)0;
//	output.Pos.x = input.Pos.x;
//	output.Pos.y = input.Pos.y;
//	output.Pos.z = input.Pos.z;
//	output.Pos.w = 1.0f;
//
//	output.UV = input.UV;
//
//	output.Pos = mul(output.Pos, worldMatrix);
//	output.WorldPos = output.Pos;
//	output.Pos = mul(output.Pos, viewMatrix);
//	output.Pos = mul(output.Pos, projectionMatrix);
//
//	output.Normal = float4(input.Normal.x, input.Normal.y, input.Normal.z, 0);
//	output.Normal = mul(output.Normal, worldMatrix);
//	output.Normal = normalize(output.Normal);
//	return output;
//}

//GS_IN_DEF main(VS_IN_DEF input)
//{
//	GS_IN_DEF output = (GS_IN_DEF)0;
//	output.Pos.x = input.Pos.x;
//	output.Pos.y = input.Pos.y;
//	output.Pos.z = input.Pos.z;
//	output.Pos.w = 1.0f;
//
//	output.UV = input.UV;
//
//	output.Normal = float4(input.Normal.x, input.Normal.y, input.Normal.z, 0);
//
//	return output;
//}