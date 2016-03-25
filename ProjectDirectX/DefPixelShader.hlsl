#include "ShaderLibrary.hlsli"

Texture2D c_text : register(t0);
SamplerState samplerType : register(s0);

PS_OUT_DEF main(PS_IN_DEF input) : SV_TARGET
{
	PS_OUT_DEF output = (PS_OUT_DEF)0;
	
	//Store the sampled color
	output.Color = c_text.Sample(samplerType, input.UV);
	//Store the normal
	output.Normal = input.Normal;
	//Store the position
	output.Position = input.WorldPos;
	return output;
}