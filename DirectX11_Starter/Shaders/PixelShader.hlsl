

cbuffer lights : register(b1)
{
	float4 light1Color;
	float3 light1Direction;
	float padding1;
	float4 light2Color;
	float3 light2Direction;
	float padding2;
}

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);


// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{

	// Just return the input color
	// - Note that this color (like all values that pass through the rasterizer)
	//   is interpolated for each pixel between the corresponding 
	//   vertices of the triangle
	float3 light1Dir = -normalize(light1Direction);
	float amount1 = saturate(dot(input.normal, light1Dir));

	float3 light2Dir = -normalize(light2Direction);
	float amount2 = saturate(dot(input.normal, light2Dir));

	float4 textureColor = diffuseTexture.Sample(basicSampler, input.uv);

	return light1Color * amount1 + light2Color * amount2 + textureColor;
}