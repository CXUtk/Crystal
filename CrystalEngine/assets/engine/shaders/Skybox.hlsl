TextureCube skyboxTexture : register(t0);
SamplerState samplerState : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	float4x4 ViewRemoveTranslation;
	float4x4 Proj;
}

struct VertexIn
{
	float3 pos : POSITION;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float3 texCoords : TEXCOORD;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut vout;
	vout.texCoords = vIn.pos;
	
	float4 pos = mul(mul(float4(vIn.pos, 1.0), ViewRemoveTranslation), Proj);
	vout.position = pos.xyww;
	return vout;
}

float4 PS(VertexOut pIn) : SV_Target
{
	float3 color = skyboxTexture.Sample(samplerState, pIn.texCoords).rgb;
	//color = color / (color + 1.0);
	//color = pow(color, 1.0 / 2.2);
	return float4(color, 1.0);
}