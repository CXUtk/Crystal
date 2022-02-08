Texture2D texture0 : register(t0);
SamplerState mySampler0 : register(s0);

struct VertexIn
{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD;
	float4 color : COLOR;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float4 color : COLOR;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut vout;
	vout.posH = float4(vIn.pos, 1.0);
	vout.texCoord = vIn.texCoord;
	vout.color = vIn.color;
	return vout;
}

float4 PS(VertexOut pIn) : SV_Target
{
	return texture0.Sample(mySampler0, pIn.texCoord);
}