Texture2D myTexture : register(t0);
SamplerState TextureSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

cbuffer ConstantBuffer : register(b0)
{
	float4x4 MVP;
}

struct VertexIn
{
	float3 pos : POSITION;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 texCoord : TEXCOORD;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut vout;
	vout.posH = mul(float4(vIn.pos, 1.0), MVP);
	vout.texCoord = float2(vIn.pos.x, 1.0 - vIn.pos.y);
	return vout;
}

float4 PS(VertexOut pIn) : SV_Target
{
	return myTexture.Sample(TextureSampler, pIn.texCoord);
}