#include "ShaderLibrary.hlsli"

Texture2D c_text : register(t0);
SamplerState samplerType;

cbuffer LightBuffer : register(b0) {
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float4 diffusePos;
};

cbuffer MaterialBuffer : register(b1) {
	Material material;
}

//float4 main(PS_IN_TEST input) : SV_TARGET
//{
//	float4 m_color = (float4)0;
//
//	//Sample the pixel color from the color texture
//	m_color = c_text.Sample(samplerType, input.UV);
//	m_color = float4(1.0f, 0.0f, 0.0f, 1.0f);
//	return m_color;
//}

float4 main(PS_IN_UV input) : SV_TARGET
{
	float4 m_color = (float4)0;
	float4 additionColor = (float4)0;
	//Sample the pixel color from the color texture
	m_color = c_text.Sample(samplerType, input.UV);
	//Ambient
	additionColor = ambientColor / 255;
	additionColor *= float4(material.Ka, 0.0);
	//Ambient End
	//Diffuse
	float3 lightDirection = normalize(diffusePos - input.WorldPos);
	float lightIntensity = saturate(dot(lightDirection, input.Normal));
	float4 diffuseResult = saturate((diffuseColor / 255) * lightIntensity);

	if (lightIntensity > 0.0f)
		additionColor += diffuseResult;
	//Diffuse End
	//Speculare
	//Speculare End
	additionColor = saturate(additionColor);
	m_color = m_color * additionColor;
	//To print normals, for debugging
	//m_color = float4(input.Normal.x, input.Normal.y, input.Normal.z, 1.0f);
	return m_color;
}

//float4 main(PS_IN input) : SV_TARGET
//{
//	float4 m_color = input.Color;
//	return m_color;
//}