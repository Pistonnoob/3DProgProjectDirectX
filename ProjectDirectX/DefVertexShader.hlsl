#include "ShaderLibrary.hlsli"


GS_IN_DEF main(VS_IN_DEF input)
{
	GS_IN_DEF output = (GS_IN_DEF)0;
	output.Pos.x = input.Pos.x;
	output.Pos.y = input.Pos.y;
	output.Pos.z = input.Pos.z;
	output.Pos.w = 1.0f;

	output.UV = input.UV;

	output.Normal = float4(input.Normal.x, input.Normal.y, input.Normal.z, 0);

	return output;
}