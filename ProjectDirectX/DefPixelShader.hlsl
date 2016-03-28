#include "ShaderLibrary.hlsli"

Texture2D c_text : register(t0);
SamplerState samplerType : register(s0);

cbuffer LightBuffer : register(b0) {
	float4 diffuseColor;	//Kd
	float4 specularColor;	//Ks
	float Ns;				//Specular Exponent
	float3 padding;
};

PS_OUT_DEF main(PS_IN_DEF input) : SV_TARGET
{
	PS_OUT_DEF output = (PS_OUT_DEF)0;
	
	//Store the sampled color
	output.Color = c_text.Sample(samplerType, input.UV);
	//Store the normal
	output.Normal = input.Normal;
	//Store the position
	output.Position = input.WorldPos;
	//Store the diffuse material
	output.Diffuse = diffuseColor;
	//Store the specular material
	output.Specular = float4(specularColor.r, specularColor.g, specularColor.b, Ns);
	return output;
}