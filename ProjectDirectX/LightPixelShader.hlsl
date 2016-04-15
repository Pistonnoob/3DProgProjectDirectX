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
	//Get the color from the G_Buffer
	float4 color = colorTex.Sample(samplerType, input.UV);
	float4 normal = normalTex.Sample(samplerType, input.UV);
	float4 position = positionTex.Sample(samplerType, input.UV);
	float4 diffuse = diffuseTex.Sample(samplerType, input.UV);
	float4 specular = specularTex.Sample(samplerType, input.UV);
	float Ns = specular.a;
	specular.a = 1.0f;
	position.w = 1.0f;

	float4 additionColor = float4(0.3f, 0.3f, 0.3f, 1.0f);
	float4 diffuseLightColor = (lightColor, 1.0f);

	//Create the normalized vector from position to light source
	float3 outVec = normalize(lightPos - position);

	//Create the normalized reflection vector
	float3 refVec = normalize(reflect(-outVec, normal));

	//Creathe the normalized vector from position to camera
	float3 viewDir = normalize(cameraPos - position);

	float specIntensity = saturate(dot(refVec, viewDir));

	/*float3 specular = specularTex.Sample(samplerType, input.UV);*/
	float3 lightDirection = (float3)normalize(lightPos - (float3)position);
	float lightIntensity = (dot((float3)normal, lightDirection));
	float4 diffuseResult = saturate((diffuseLightColor) * lightIntensity);
	if (lightIntensity > 0.0f)
	{
		diffuseResult *= diffuse;
		//additionColor += diffuseResult;
	}
	additionColor = saturate(additionColor);
	//result = saturate(color * additionColor);
	//SPECULAR
	float4 specularResult = (float4)0;	//The color the specular light will produce
	if (lightIntensity > 0.0f || true)
	{
		float3 viewerDirection = (float3)normalize(cameraPos - position);
		// r = l + 2u = l + 2(n' - l) = 2(dot(n, l))*n - l = r
		float3 lightReflect = normalize(reflect(-lightDirection, normal));//Create the reflection
		float specIntensity = saturate(dot(refVec, viewDir));
		specularResult = float4(specular * lightColor * max(pow(specIntensity, Ns), 0.0f), 1.0f);	//Result with applied material and light
		specularResult = saturate(specularResult);
	}
	result = saturate(result + specularResult);
	//result = (specIntensity, specIntensity, specIntensity, 1.0f);
	//result = specular;
	//result = saturate(diffuse);
	//result = float4(position.y, 0.0f, 0.0f, 1.0f);
	//result = float4(1.0f, 0.0f, 0.0f, 1.0f);
	return result;
}