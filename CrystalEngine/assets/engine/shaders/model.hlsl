Texture2D diffuseTexture : register(t0);
SamplerState mySampler0 : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	float4x4 M;
	float4x4 MN;
	float4x4 VP;
}

struct VertexIn
{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut vout;
	vout.posH = mul(mul(float4(vIn.pos, 1.0), M), VP);
	vout.texCoord = vIn.texCoord;
	vout.normal = mul(float4(vIn.normal, 0.0), MN);
	return vout;
}

float4 PS(VertexOut pIn) : SV_Target
{
	float3 normal = normalize(pIn.normal);
	return float4(0.5 + 0.5 * normal, 1.0) * diffuseTexture.Sample(mySampler0, pIn.texCoord);
}