#include "ShaderLibrary.hlsli"

Texture2D c_text : register(t0);
SamplerState samplerType;

//float4 main(PS_IN_TEST input) : SV_TARGET
//{
//	float4 m_color = (float4)0;
//
//	//Sample the pixel color from the color texture
//	m_color = c_text.Sample(samplerType, input.UV);
//	m_color = float4(1.0f, 0.0f, 0.0f, 1.0f);
//	return m_color;
//}
cbuffer LightBuffer
{
	float4 diffColor;
	float3 lightDir;
	float padding;
};

float4 main(PS_IN_UV input) : SV_TARGET
{
	float4 m_color = (float4)0;
	float4 textureColor = (float4)0;
	float3 lightDirection = (float3)0;
	float lightInt = (float)0;

	//Sample the pixel color from the color texture
	textureColor = c_text.Sample(samplerType, input.UV);
	m_color = float4(1, 1, 0.0f, 1.0f);

	//Invert light direction
	/*lightDirection = -lightDir;*/

	//Calculate the amount of light on the pixel
	/*lightInt = saturate(dot(input.Normal, lightDirection));*/

	//Combine lightIntensity and diffuseColor
	/*m_color = saturate(diffColor * lightInt);*/

	//Multiply texture pixel and the color
	m_color = m_color * textureColor;
	//m_color = input.Normal;
	return m_color;
}

//float4 main(PS_IN input) : SV_TARGET
//{
//	float4 m_color = input.Color;
//	return m_color;
//}