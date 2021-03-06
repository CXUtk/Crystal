Texture2D baseColor : register(t0);
SamplerState baseColorSampler : register(s0);

TextureCube irradianceMap : register(t1);
SamplerState irradianceSampler : register(s1);

TextureCube spcularMap : register(t2);
SamplerState spcularSampler : register(s2);

Texture2D LUTTexture : register(t3);
SamplerState LUTSampler : register(s3);

cbuffer ConstantBuffer : register(b0)
{
	float4x4 M;
	float3x3 MN;
	float4x4 VP;
	float3 uCameraPos;
	
	float3 uLightPos;
	float3 uLightIntensity;
	
	float uRoughness;
	float uMetallic;
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
static float EPS = 1e-4;

float D(float3 N, float3 H)
{
	float a = square(uRoughness);
	float a2 = square(a);
	float NdotH = max(0.0, dot(N, H));
	return a2 / (PI * square(square(NdotH) * (a2 - 1.0) + 1.0));
}

float G1(float3 N, float3 V)
{
	float k = square(uRoughness + 1.0) * 0.125;
	float NdotV = max(0.0, dot(N, V));
	return NdotV / (NdotV * (1.0 - k) + k);
}

float Vis(float3 N, float3 V, float3 L)
{
	float NdotL = max(0.0, dot(N, L));
	float NdotV = max(0.0, dot(N, V));
	float a = uRoughness * uRoughness;
	
	float GGXV = NdotL * (NdotV * (1 - a) + a);
	float GGXL = NdotV * (NdotL * (1 - a) + a);
	return 2 / (GGXL + GGXV);
}

float V_SmithGGXCorrelated(float3 N, float3 V, float3 L)
{
	float NdotL = max(0.0, dot(N, L));
	float NdotV = max(0.0, dot(N, V));
	
	float a2 = square(uRoughness * uRoughness);
	float GGXL = NdotV * sqrt((-NdotL * a2 + NdotL) * NdotL + a2);
	float GGXV = NdotL * sqrt((-NdotV * a2 + NdotV) * NdotV + a2);
	return 0.5 / (GGXV + GGXL);
}

float G(float3 N, float3 V, float3 L)
{
	return G1(N, V) * G1(N, L);
}

float3 F(float3 F0, float3 H, float3 V)
{
	float HdotV = max(0.0, dot(H, V));
	return F0 + (1.0 - F0) * pow(1.0 - HdotV, 5.0);
}

float3 F2(float3 F0, float3 H, float3 V)
{
	float HdotV = max(0.0, dot(H, V));
	float3 roughness = uRoughness;
	return F0 + (max(1.0 - roughness, F0) - F0) * pow(1.0 - HdotV, 5.0);
}

float3 BRDFDirect(float3 N, float3 V, float3 L, float3 albedo, float3 F0)
{
	float3 H = normalize(V + L);
	
	float d = D(N, H);
	float v = V_SmithGGXCorrelated(N, V, L);
	float3 f = F(F0, H, V);

	float NdotV = max(0.0, dot(N, V));
	float NdotL = max(0.0, dot(N, L));
	
	float3 cook_torrance = d * v * f;
	float3 lambertian = albedo / PI * (1.0 - f) * (1.0 - uMetallic);
	
	return (lambertian + cook_torrance) * uLightIntensity * NdotL;
}

float4 PS(VertexOut pIn) : SV_Target
{
	float3 albedo = baseColor.Sample(baseColorSampler, pIn.texCoord).xyz;
	float3 F0 = float3(0.04, 0.04, 0.04);
	F0 = lerp(F0, albedo, uMetallic);

	float roughness = lerp(0.002, 1.0, uRoughness);
	
	
	float3 N = normalize(pIn.normal);
	float3 V = normalize(uCameraPos - pIn.position);
	float3 L = normalize(uLightPos - pIn.position);
	float NdotL = max(0.0, dot(N, L));
	float VdotN = max(0.0, dot(V, N));
	
	float3 color = BRDFDirect(N, V, L, albedo, F0);
	
	float4 fSplit = LUTTexture.Sample(LUTSampler, float2(VdotN, 1.0 - uRoughness));
	float3 f = (F0 * fSplit.x + fSplit.yyy);
	float3 f2 = F2(F0, N, V);

	color = color / (color + 1.0);
	color = pow(color, 1.0 / 2.2);
	return float4(color, 1.0);
}