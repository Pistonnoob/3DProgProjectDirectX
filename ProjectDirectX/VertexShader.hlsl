#include "ShaderLibrary.hlsli"


cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

GS_IN_3D main(VS_IN_3D input)
{
	GS_IN_3D output = (GS_IN_3D)0;
	output.Pos.x = input.Pos.x;
	output.Pos.y = input.Pos.y;
	output.Pos.z = input.Pos.z;
	output.Pos.w = 1.0f;

	output.Normal = float4(input.Normal.x, input.Normal.y, input.Normal.z, 0);
	output.UV = input.UV;
	return output;
}

//GS_IN_UV main(VS_IN_UV input)
//{
//	GS_IN_UV output = (GS_IN_UV)0;
//	output.Pos.x = input.Pos.x;
//	output.Pos.y = input.Pos.y;
//	output.Pos.z = input.Pos.z;
//	output.Pos.w = 1.0f;
//
//	output.UV = input.UV;
//	return output;
//}

//GS_IN main(VS_IN input)
//{
//	GS_IN output = (PS_IN)0;
//	output.Pos.x = input.Pos.x;
//	output.Pos.y = input.Pos.y;
//	output.Pos.z = input.Pos.z;
//	output.Pos = float4(input.Pos, 1);
//	output.Color = input.Color;
//
//	return output;
//}