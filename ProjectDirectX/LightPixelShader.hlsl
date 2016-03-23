
Texture2D colorTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D positionTex : register(t2);
Texture2D materialTex : register(t3);


SamplerState samplerType : register(s0);

float4 main(PS_IN_LIGHT input) : SV_TARGET
{
	float4 result = (float4)0;
	//Get the color from the G_Buffer
	float3 color = colorTex.Sample(samplerType, input.UV);


	return result;
}