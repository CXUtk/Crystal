Texture2D textures[8] : register(t0);
SamplerState mySampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	float4x4 MVP;
}


struct VertexIn
{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD;
	float4 color : COLOR;
	float index : POSITION1;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float4 color : COLOR;
	float index : POSITION1;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut vout;
	vout.posH = mul(float4(vIn.pos, 1.0), MVP);
	vout.texCoord = float2(vIn.texCoord.x, 1.0 - vIn.texCoord.y);
	vout.color = vIn.color;
	vout.index = vIn.index;
	return vout;
}

float4 PS(VertexOut pIn) : SV_Target
{
	return textures[(int) pIn.index].Sample(mySampler, pIn.texCoord) * pIn.color;
}