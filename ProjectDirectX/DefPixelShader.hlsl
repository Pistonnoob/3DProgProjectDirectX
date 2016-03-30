#include "ShaderLibrary.hlsli"

Texture2D c_text : register(t0);
SamplerState samplerType : register(s0);

cbuffer LightBuffer : register(b0) {
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float Ns;
	float3 padding;
};

PS_OUT_DEF main(PS_IN_DEF input) : SV_TARGET
{
	PS_OUT_DEF output = (PS_OUT_DEF)0;
	//float4 colorThat = ambientColor;
	//Store the sampled color
	output.Color = c_text.Sample(samplerType, input.UV);
	//Store the normal
	output.Normal = input.Normal;
	//Store the position
	output.Position = input.WorldPos;
	//Store the diffuse material
	output.Diffuse = diffuseColor;
	output.Diffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//Store the specular material
	output.Specular = float4(specularColor.r, specularColor.g, specularColor.b, Ns);
	output.Specular = float4(0.5f, 0.5f, 0.5f, 60);
	return output;
}