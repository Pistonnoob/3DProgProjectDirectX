#include "ShaderLibrary.hlsli"


Texture2D colorTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D positionTex : register(t2);
//Texture2D specularTex : register(t3);

SamplerState samplerType : register(s0);

cbuffer LightBuffer
{
	float3 lightPos;
	float padding1;
	float3 lightColor;
	float padding2;
};

float4 main(PS_IN_LIGHT input) : SV_TARGET
{
	float4 result = (float4)0;
	float4 additionColor = (float4)0;
	float4 diffuseLightColor = (lightColor, 1.0f);
	//Get the color from the G_Buffer
	float4 color = colorTex.Sample(samplerType, input.UV);
	float4 normal = normalTex.Sample(samplerType, input.UV);
	float4 position = positionTex.Sample(samplerType, input.UV);
	/*float3 specular = specularTex.Sample(samplerType, input.UV);*/
	float3 lightDirection = (float3)normalize(lightPos - (float3)position);
	float lightIntensity = saturate(dot(lightDirection, (float3)normal));
	float4 diffuseResult = saturate((diffuseLightColor / 255) * lightIntensity);
	if (lightIntensity > 0.0f)
	{
		//diffuseResult *= float4(material.Kd, 1.0f);
		additionColor += diffuseResult;
	}

	result = saturate(color * diffuseResult);

	return result;
}