#include "ShaderLibrary.hlsli"

Texture2D c_text : register(t0);
SamplerState samplerType;

cbuffer LightBuffer : register(b0) {
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float4 diffusePos;
	float4 specularPos;
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
	//AMBIENT
	additionColor = ambientColor / 255;
	additionColor = float4(material.Ka, 0.0);
	//DIFFUSE
	float3 lightDirection = normalize(diffusePos - input.WorldPos);
	float lightIntensity = saturate(dot(lightDirection, input.Normal));
	float4 diffuseResult = saturate((diffuseColor / 255) * lightIntensity);

	if (lightIntensity > 0.0f)
	{
		additionColor += diffuseResult;
	}
	additionColor = saturate(additionColor);
	//SPECULAR
	float4 specularResult = (float4)0;
	if (true/*lightIntensity > 0.0f*/)
	{
		float3 lightReflect = 0.0f;
		float3 viewerDirection = (float3)normalize(specularPos - input.WorldPos);
		// r = l + 2u = l + 2(n' - l) = 2(dot(n, l))*n - l = r
		float3 l = dot(input.Normal, lightDirection);
		lightReflect = normalize(2 * (lightIntensity * (float3)input.Normal) - lightIntensity);

		specularResult = pow(dot(lightReflect, viewerDirection), material.Ns);
		specularResult = specularResult * float4(material.Ks, 1.0f);
		specularResult = saturate(specularResult);
	}
	//calculating final color
	additionColor = saturate(additionColor);
	m_color = m_color * additionColor;

	m_color = saturate(m_color + specularResult);
	m_color = saturate(m_color);
	//To print normals, for debugging
	//m_color = float4(input.Normal.x, input.Normal.y, input.Normal.z, 1.0f);
	return m_color;
}

//float4 main(PS_IN input) : SV_TARGET
//{
//	float4 m_color = input.Color;
//	return m_color;
//}