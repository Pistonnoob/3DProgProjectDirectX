#include "ShaderLibrary.hlsli"

cbuffer MatrixBuffer : register(b0) {
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


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
//		element.Pos = mul(element.Pos, worldMatrix);
//		element.WorldPos = element.Pos;
//		element.Pos = mul(element.Pos, viewMatrix);
//		element.Pos = mul(element.Pos, projectionMatrix);
//		element.Normal = input[i].Normal;
//		element.Normal = mul(element.Normal, worldMatrix);
//		element.Normal = normalize(element.Normal);
//		element.UV = input[i].UV;
//		outputStream.Append(element);
//	}
//	outputStream.RestartStrip();
//}
[maxvertexcount(3)]
void main(
	triangle GS_IN_DEF input[3] : SV_POSITION,
	inout TriangleStream< PS_IN_DEF > outputStream
	)
{
	bool facingCamera = false;
	PS_IN_DEF elements[3];
	for (uint i = 0; i < 3; i++)
	{
		PS_IN_DEF element = (PS_IN_DEF)0;
		element.Pos = input[i].Pos;
		element.UV = input[i].UV;
		element.Pos = mul(element.Pos, worldMatrix);
		element.WorldPos = element.Pos;
		element.Pos = mul(element.Pos, viewMatrix);
		float4 toCamera = normalize(-element.Pos);
		element.Pos = mul(element.Pos, projectionMatrix);
		element.Tangent = float4(0.0f, 0.0f, 0.0f, 0.0f);
		element.Normal = normalize(input[i].Normal);
		//element.Normal = float4(cross(input[1].Pos - input[0].Pos, input[2].Pos - input[0].Pos), 0);
		element.Normal = mul(element.Normal, worldMatrix);
		element.Normal = normalize(element.Normal);
		float4 viewNormal = normalize(mul(element.Normal, viewMatrix));
		if (dot(viewNormal, toCamera) > -0.0f)
		{
			facingCamera = true;
		}
		elements[i] = element;
		//outputStream.Append(element);
	}
	if (facingCamera)
	{
		//Do normal mapping here
		float3 edge1 = elements[0].WorldPos - elements[1].WorldPos, edge2 = elements[0].WorldPos - elements[2].WorldPos;
		/*edge1.w = edge2.w = 0.0f;*/
		float uEdge1 = elements[0].UV.x - elements[1].UV.x, vEdge1 = elements[0].UV.y - elements[1].UV.y;
		float uEdge2 = elements[0].UV.x - elements[2].UV.x, vEdge2 = elements[0].UV.y - elements[2].UV.y;
		float3 tempTangent = (vEdge1 * edge1 - vEdge2 * edge2) * (1.0f / (uEdge1 * vEdge2 - uEdge2 * vEdge1));
		//tempTangent = tempTangent / 3;
		for (uint j = 0; j < 3; j++)
		{
			elements[j].Tangent.x = tempTangent.x;
			elements[j].Tangent.y = tempTangent.y;
			elements[j].Tangent.z = tempTangent.z;
			outputStream.Append(elements[j]);
		}
	}
	outputStream.RestartStrip();
}