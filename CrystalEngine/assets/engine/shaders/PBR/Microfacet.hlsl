Texture2D diffuseTexture : register(t0);
SamplerState mySampler0 : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	float4x4 M;
	float3x3 MN;
	float4x4 VP;
	float3 uCameraPos;
	
	float3 uLightPos;
	float3 uLightIntensity;
	float uRoughness;
	
	float3 uF0;
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

float square(float x)
{
	return x * x;
}

static float PI = 3.14159265358;

float D(float3 N, float3 H)
{
	float a2 = square(uRoughness);
	float NdotH = max(0.0, dot(N, H));
	return a2 / (PI * square(square(NdotH) * (a2 - 1.0) + 1.0));
}

float G1(float3 N, float3 V)
{
	float k = square(uRoughness + 1.0) * 0.125;
	float NdotV = max(0.0, dot(N, V));
	return NdotV / (NdotV * (1.0 - k) + k);
}

float G(float3 N, float3 V, float3 L)
{
	return G1(N, V) * G1(N, L);
}

float3 F(float3 H, float3 V)
{
	float HdotV = max(0.0, dot(H, V));
	return uF0 + (1.0 - uF0) * pow(1.0 - HdotV, 5.0);
}

float4 PS(VertexOut pIn) : SV_Target
{
	float3 N = normalize(pIn.normal);
	float3 V = normalize(uCameraPos - pIn.position);
	float3 L = normalize(uLightPos - pIn.position);
	float3 H = normalize(V + L);
	
	float NdotV = max(0.0, dot(N, V));
	float NdotL = max(0.0, dot(N, L));
	
	if (NdotL == 0.0 || NdotV == 0.0)
		return float4(0.0, 0.0, 0.0, 0.0);
	
	return float4((D(N, H) * G(N, V, L) * F(H, V)) / (4.0 * NdotV), 1.0);
}