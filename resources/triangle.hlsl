
cbuffer ConstantBuffer : register(b0)
{
	float uLuminance;
	float uBase;
}

struct VertexIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut vout;
	vout.posH = float4(vIn.pos, 1.0);
	vout.color = vIn.color;
	return vout;
}

float4 PS(VertexOut pIn) : SV_Target
{
	return uBase + pIn.color;
}