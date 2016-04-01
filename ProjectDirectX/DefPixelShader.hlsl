#include "ShaderLibrary.hlsli"

Texture2D c_text : register(t0);
Texture2D c_normal : register(t1);
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
	//Get the normal from the bump map
	float4 bumpMap = c_normal.Sample(samplerType, input.UV);
	bumpMap.w = 0.0f;
	bumpMap = (2.0f * bumpMap) - 1.0f;

	float3 bumpNormal = (bumpMap.x * input.Tangent) + (bumpMap.y * input.Binormal) + (bumpMap.z * input.Normal);
	bumpNormal = normalize(bumpNormal);

	output.Normal = float4(bumpNormal.x, bumpNormal.y, bumpNormal.z, 0.0f);
	//output.Normal = bumpMap;
	//Store the position
	output.Position = input.WorldPos;
	//Store the diffuse material
	output.Diffuse = diffuseColor;
	//output.Diffuse = float4(0.3f, 0.3f, 0.3f, 1.0f);
	//Store the specular material
	output.Specular = float4(specularColor.r, specularColor.g, specularColor.b, Ns);
	//output.Specular = float4(0.5f, 0.5f, 0.5f, 60.0f);
	return output;
}

//PS_OUT_DEF main(PS_IN_DEF input) : SV_TARGET
//{
//	PS_OUT_DEF output = (PS_OUT_DEF)0;
////float4 colorThat = ambientColor;
////Store the sampled color
//output.Color = c_text.Sample(samplerType, input.UV);
////Store the normal
//output.Normal = input.Normal;
////Get the normal from the bump map
//float4 normalMap = c_normal.Sample(samplerType, input.UV);
//normalMap = (2 * normalMap) - 1.0f;
////Make sure tangent is orthogonal to normal
//input.Tangent = normalize(input.Tangent - dot(input.Tangent, input.Normal) * input.Normal);
////Create the bitangent
//float3 biTangent = cross(input.Normal, input.Tangent);
//float3x3 texToObject = float3x3(input.Tangent, biTangent, input.Normal.xyz);
////Convert the normal map values from tangent to object space
//output.Normal = float4(normalize(mul(normalMap, texToObject)), 0.0f);
////Store the position
//output.Position = input.WorldPos;
////Store the diffuse material
//output.Diffuse = diffuseColor;
////output.Diffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
////Store the specular material
//output.Specular = float4(specularColor.r, specularColor.g, specularColor.b, Ns);
//output.Specular = float4(0.5f, 0.5f, 0.5f, 60.0f);
//return output;
//}