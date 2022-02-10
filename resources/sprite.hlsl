Texture2D texture0 : register(t0);
SamplerState mySampler0 : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	float4x4 MVP;
}


struct VertexIn
{
	float2 pos : POSITION;
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
	vout.posH = mul(float4(vIn.pos, 0.0, 1.0), MVP);
	vout.texCoord = vIn.texCoord;
	vout.color = vIn.color;
	vout.index = vIn.index;
	return vout;
}

float4 PS(VertexOut pIn) : SV_Target
{
	return texture0.Sample(mySampler0, pIn.texCoord) * pIn.color;
}