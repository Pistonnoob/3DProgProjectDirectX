#include "ShaderLibrary.hlsli"


Texture2D colorTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D positionTex : register(t2);
Texture2D diffuseTex : register(t3);
Texture2D specularTex : register(t4);

SamplerState samplerType : register(s0);

cbuffer LightBuffer
{
	float3 lightPos;
	float padding1;
	float3 lightColor;
	float padding2;
	float4 cameraPos;
};

float4 main(PS_IN_LIGHT input) : SV_TARGET
{
	float4 result = (float4)0;
	float4 additionColor = float4(0.3f, 0.3f, 0.3f, 1.0f);
	float4 diffuseLightColor = (lightColor, 1.0f);
	//Get the color from the G_Buffer
	float4 color = colorTex.Sample(samplerType, input.UV);
	float4 normal = normalTex.Sample(samplerType, input.UV);
	float4 position = positionTex.Sample(samplerType, input.UV);
	float4 diffuse = diffuseTex.Sample(samplerType, input.UV);
	float4 specular = specularTex.Sample(samplerType, input.UV);
	float Ns = specular.a;
	specular.w = 1.0f;
	position.w = 1.0f;
	/*float3 specular = specularTex.Sample(samplerType, input.UV);*/
	float3 lightDirection = (float3)normalize(lightPos - (float3)position);
	float lightIntensity = saturate(dot(lightDirection, (float3)normal));
	float4 diffuseResult = saturate((diffuseLightColor) * lightIntensity);
	if (lightIntensity > 0.0f)
	{
		diffuseResult *= diffuse;
		additionColor += diffuseResult;
	}
	additionColor = saturate(additionColor);
	result = saturate(color * additionColor);
	//SPECULAR
	float4 specularResult = (float4)0;	//The color the specular light will produce
	if (lightIntensity > 0.0f)
	{
		float3 lightReflect = 0.0f;
		float3 viewerDirection = (float3)normalize(cameraPos - position);
		// r = l + 2u = l + 2(n' - l) = 2(dot(n, l))*n - l = r
		float3 l = dot((float3)normal, lightDirection);
		lightReflect = normalize(reflect(-viewerDirection, normal));//Create the the reflection
		float specIntensity = saturate(dot(lightReflect, viewerDirection));
		specularResult = float4(specular * lightColor * saturate(pow(specIntensity, Ns)), 1.0f);	//Result with applied material and light
		specularResult = saturate(specularResult);
	}
	result = saturate(result + specularResult);
	//result = specular;
	//result = saturate(diffuse);
	//result = float4(position.y, 0.0f, 0.0f, 1.0f);
	//result = float4(1.0f, 0.0f, 0.0f, 1.0f);
	return result;
}