Texture2D diffuseTexture : register(t0);
Texture2D specularTexture : register(t1);
TextureCube environmentTexture : register(t2);

SamplerState mySampler0 : register(s0);
SamplerState mySampler1 : register(s1);

cbuffer ConstantBuffer : register(b0)
{
	float4x4 M;
	float3x3 MN;
	float4x4 VP;
	float3 uCameraPos;
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
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut vout;
	float4 pos = mul(float4(vIn.pos, 1.0), M);
	vout.position = pos;
	vout.posH = mul(pos, VP);
	vout.texCoord = vIn.texCoord;
	vout.normal = mul(vIn.normal, MN);
	return vout;
}

float4 PS(VertexOut pIn) : SV_Target
{
	float3 normal = normalize(pIn.normal);
	float3 I = normalize(pIn.position - uCameraPos);
	return environmentTexture.Sample(mySampler0, reflect(I, normal));
}