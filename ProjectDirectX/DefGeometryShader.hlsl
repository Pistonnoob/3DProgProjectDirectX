#include "ShaderLibrary.hlsli"

cbuffer MatrixBuffer : register(b0) {
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


[maxvertexcount(3)]
void main(
	triangle GS_IN_DEF input[3] : SV_POSITION,
	inout TriangleStream< PS_IN_DEF > outputStream
	)
{
	bool facingCamera = false;
	for (int i = 0; i < 3; i++)
	{
		float angle = dot(normalize(-mul(input[i].WorldPos, viewMatrix)), normalize(mul(input[i].Normal, viewMatrix)));
		if (angle >= 0.0f)
			facingCamera = true;
	}
	if (facingCamera)
	{
		float3 edge1 = normalize(input[1].WorldPos - input[0].WorldPos);
		float3 edge2 = normalize(input[2].WorldPos - input[0].WorldPos);

		float2 texEdge1 = normalize(input[1].UV - input[0].UV);
		float2 texEdge2 = normalize(input[2].UV - input[0].UV);

		float det = (texEdge1.x * texEdge2.y) - (texEdge1.y * texEdge2.x);

		float3 tangent = float3(0.0f, 0.0f, 0.0f);
		tangent = normalize((texEdge2.y * edge1 - texEdge1.y * edge2) * det);

		for (uint i = 0; i < 3; i++)
		{
			PS_IN_DEF element = (PS_IN_DEF)0;
			element.Pos = input[i].Pos;
			element.WorldPos = input[i].WorldPos;
			element.Normal = input[i].Normal;
			element.Tangent = tangent;
			element.UV = input[i].UV;
			outputStream.Append(element);
		}
	}
	outputStream.RestartStrip();
}

//[maxvertexcount(3)]
//void main(
//	triangle GS_IN_DEF input[3] : SV_POSITION,
//	inout TriangleStream< PS_IN_DEF > outputStream
//	)
//{
//	bool facingCamera = false;
//	for (uint i = 0; i < 3; i++)
//	{
//		PS_IN_DEF element = (PS_IN_DEF)0;
//		element.Pos = input[i].Pos;
//		element.WorldPos = input[i].WorldPos;
//		element.Normal = input[i].Normal;
//		element.Tangent = input[i].Tangent;
//		element.Binormal = input[i].Binormal;
//		element.UV = input[i].UV;
//		outputStream.Append(element);
//	}
//	outputStream.RestartStrip();
//}
//[maxvertexcount(3)]
//void main(
//	triangle GS_IN_DEF input[3] : SV_POSITION,
//	inout TriangleStream< PS_IN_DEF > outputStream
//	)
//{
//	bool facingCamera = false;
//	PS_IN_DEF elements[3];
//	for (uint i = 0; i < 3; i++)
//	{
//		PS_IN_DEF element = (PS_IN_DEF)0;
//		element.Pos = input[i].Pos;
//		element.UV = input[i].UV;
//		element.Normal = normalize(input[i].Normal);
//		//element.Normal = normalize(float4(cross(input[1].Pos - input[0].Pos, input[2].Pos - input[0].Pos), 0));
//		element.Normal = mul(element.Normal, worldMatrix);
//		element.Normal = normalize(element.Normal);
//		element.Pos = mul(element.Pos, worldMatrix);
//		element.WorldPos = element.Pos;
//		element.Pos = mul(element.Pos, viewMatrix);
//		float4 toCamera = normalize(-element.Pos);
//		element.Pos = mul(element.Pos, projectionMatrix);
//		element.Tangent = float3(0.0f, 0.0f, 0.0f);
//
//		float4 viewNormal = normalize(mul(element.Normal, viewMatrix));
//		if (dot(viewNormal, toCamera) > -0.0f)
//		{
//			facingCamera = true;
//		}
//		elements[i] = element;
//		//outputStream.Append(element);
//	}
//	if (facingCamera)
//	{
//		//Do normal mapping here
//		///*edge1.w = edge2.w = 0.0f;*/
//		//float uEdge1 = elements[0].UV.x - elements[1].UV.x, vEdge1 = elements[0].UV.y - elements[1].UV.y;
//		//float uEdge2 = elements[0].UV.x - elements[2].UV.x, vEdge2 = elements[0].UV.y - elements[2].UV.y;
//		/*float3 edge1 = input[0].Pos - input[1].Pos, edge2 = input[0].Pos - input[2].Pos;*/
//		float3 edge1 = input[1].Pos.xyz - input[0].Pos.xyz, edge2 = input[2].Pos.xyz - input[0].Pos.xyz;
//		float uEdge1 = elements[1].UV.x - elements[0].UV.x, vEdge1 = elements[1].UV.y - elements[0].UV.x;
//		float uEdge2 = elements[2].UV.x - elements[0].UV.x, vEdge2 = elements[2].UV.y - elements[0].UV.x;
//		float2 texEdge0 = elements[1].UV - elements[0].UV, texEdge1 = elements[2].UV - elements[0].UV;
//		float3 tempTangent = (vEdge1 * edge1 - vEdge2 * edge2) * (1.0f / (uEdge1 * vEdge2 - uEdge2 * vEdge1));
//		float3 tTangent;
//		tTangent.x = (texEdge0.y * edge1.x - texEdge1.y * edge2.x) * (1.0f / (texEdge0.x * texEdge1.y - texEdge1.x * texEdge0.y));
//		tTangent.y = (texEdge0.y * edge1.y - texEdge1.y * edge2.y) * (1.0f / (texEdge0.x * texEdge1.y - texEdge1.x * texEdge0.y));
//		tTangent.z = (texEdge0.y * edge1.z - texEdge1.y * edge2.z) * (1.0f / (texEdge0.x * texEdge1.y - texEdge1.x * texEdge0.y));
//		tTangent = normalize(tTangent);
//		//tempTangent = mul(tempTangent, worldMatrix);
//		//tempTangent = tempTangent / 3;
//		tempTangent = normalize(tempTangent);
//		for (uint j = 0; j < 3; j++)
//		{
//			elements[j].Tangent.x = tTangent.x;
//			elements[j].Tangent.y = tTangent.y;
//			elements[j].Tangent.z = tTangent.z;
//			outputStream.Append(elements[j]);
//		}
//	}
//	outputStream.RestartStrip();
//}