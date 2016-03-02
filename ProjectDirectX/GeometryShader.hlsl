#include "ShaderLibrary.hlsli"

cbuffer MatrixBuffer : register(b0){
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//[maxvertexcount(3)]
//void main(
//	triangle GS_IN_UV input[3] : SV_POSITION,
//	inout TriangleStream< PS_IN_TEST > outputStream
//	)
//{
//	for (uint i = 0; i < 3; i++)
//	{
//		PS_IN_TEST element = (PS_IN_TEST)0;
//		element.UV = input[i].UV;
//		element.Pos = input[i].Pos;
//		element.Pos = mul(element.Pos, worldMatrix);
//		element.Pos = mul(element.Pos, viewMatrix);
//		element.Pos = mul(element.Pos, projectionMatrix);
//
//		outputStream.Append(element);
//	}
//	outputStream.RestartStrip();
//}

[maxvertexcount(3)]
void main(
	triangle GS_IN_3D input[3] : SV_POSITION,
	inout TriangleStream< PS_IN_UV > outputStream
)
{
	bool facingCamera = false;
	PS_IN_UV elements[3];
	for (uint i = 0; i < 3; i++)
	{
		PS_IN_UV element = (PS_IN_UV)0;
		element.Pos = input[i].Pos;
		element.UV = input[i].UV;
		element.Pos = mul(element.Pos, worldMatrix);
		element.WorldPos = element.Pos;
		element.Pos = mul(element.Pos, viewMatrix);
		element.Pos = mul(element.Pos, projectionMatrix);

		element.Normal = normalize(input[i].Normal);
		//element.Normal = float4(cross(input[1].Pos - input[0].Pos, input[2].Pos - input[0].Pos), 0);
		element.Normal = mul(element.Normal, worldMatrix);
		element.Normal = normalize(element.Normal);
		float4 viewNormal = normalize(mul(element.Normal, viewMatrix));
		float4 toCamera = normalize(-element.Pos);
		if (dot(toCamera, viewNormal) > -0.0f)
		{
			facingCamera = true;
		}
		elements[i] = element;
	}
	if(facingCamera)
		for (uint j = 0; j < 3; j++)
			outputStream.Append(elements[j]);
	outputStream.RestartStrip();
}

//[maxvertexcount(3)]
//void main(
//	triangle GS_IN input[3] : SV_POSITION, 
//	inout TriangleStream< PS_IN > outputStream
//)
//{
//	for (uint i = 0; i < 3; i++)
//	{
//		PS_IN element;
//		element.Pos = mul(input[i].Pos, mul(worldMatrix, mul(viewMatrix, projectionMatrix)));
//		element.Color = input[i].Color;
//		outputStream.Append(element);
//	}
//	outputStream.RestartStrip();
//}