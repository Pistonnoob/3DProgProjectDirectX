#include "ShaderLibrary.hlsli"

cbuffer MatrixBuffer : register(b0) {
	matrix worldMatrix;
	matrix inverseWorldMatrix;
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
	float3 newNormal = cross(input[1].WorldPos - input[0].WorldPos, input[2].WorldPos - input[0].WorldPos);
	newNormal = normalize(newNormal);
	float angle = dot(normalize(-mul(input[1].WorldPos, viewMatrix)), normalize(mul(newNormal, viewMatrix)));

	facingCamera = angle >= 0.0f;
	if (facingCamera)
	{
		float3 tangent;

		//Calculate edges of triandle
		float3 edge0 = input[1].WorldPos.xyz - input[0].WorldPos.xyz;
		float3 edge1 = input[2].WorldPos.xyz - input[0].WorldPos.xyz;

		float2 texEdge0 = input[1].UV - input[0].UV;
		float2 texEdge1 = input[2].UV - input[0].UV;

		tangent.x = (texEdge0.y * edge0.x - texEdge1.y * edge1.x) * (1.0f / (texEdge0.x * texEdge1.y - texEdge1.x * texEdge0.y));
		tangent.y = (texEdge0.y * edge0.y - texEdge1.y * edge1.y) * (1.0f / (texEdge0.x * texEdge1.y - texEdge1.x * texEdge0.y));
		tangent.z = (texEdge0.y * edge0.z - texEdge1.y * edge1.z) * (1.0f / (texEdge0.x * texEdge1.y - texEdge1.x * texEdge0.y));

		tangent = normalize(tangent);


		/*float3 edge1 = normalize(input[0].WorldPos - input[1].WorldPos);
		float3 edge2 = normalize(input[2].WorldPos - input[1].WorldPos);
		edge1 = normalize(input[0].WorldPos - input[1].WorldPos);
		edge2 = normalize(input[0].WorldPos - input[2].WorldPos);

		float2 texEdge1 = normalize(input[0].UV - input[1].UV);
		float2 texEdge2 = normalize(input[0].UV - input[2].UV);

		float tcU1 = input[0].UV.x - input[2].UV.x;
		float tcV1 = input[0].UV.y - input[2].UV.y;
		float tcU2 = input[2].UV.x - input[1].UV.x;
		float tcV2 = input[2].UV.y - input[1].UV.y;

		float det = (texEdge1.x * texEdge2.y) - (texEdge1.y * texEdge2.x);
		det = 1 / (tcU1 * tcV2 - tcU2 * tcV1);

		float3 tangent = float3(0.0f, 0.0f, 0.0f);
		tangent = normalize((texEdge2.y * edge1 - texEdge1.y * edge2) * det);*/
		//tangent = normalize((tcV1 * edge1 - tcV2 * edge2) * det);
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