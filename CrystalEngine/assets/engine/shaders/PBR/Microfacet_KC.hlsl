Texture2D baseColor : register(t0);
SamplerState baseColorSampler : register(s0);

Texture2D EmuTexture : register(t1);
SamplerState EmuSampler : register(s1);

Texture2D EavgTexture : register(t2);
SamplerState EavgSampler : register(s2);

cbuffer ConstantBuffer : register(b0)
{
	float4x4 M;
	float3x3 MN;
	float4x4 VP;
	float3 uCameraPos;
	
	float3 uLightPos;
	float3 uLightIntensity;
	
	float3 uTint;
	
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

float D(float3 N, float3 H, float alpha)
{
	float a2 = square(alpha);
	float NdotH = max(0.0, dot(N, H));
	return a2 / (PI * square(square(NdotH) * (a2 - 1.0) + 1.0));
}

float G1(float3 N, float3 V)
{
	float k = square(uRoughness + 1.0) * 0.125;
	float NdotV = max(0.0, dot(N, V));
	return NdotV / (NdotV * (1.0 - k) + k);
}

//float Vis(float3 N, float3 V, float3 L)
//{
//	float NdotL = max(0.0, dot(N, L));
//	float NdotV = max(0.0, dot(N, V));
	
//	float GGXV = NdotL * (NdotV * (1 - alpha) + a);
//	float GGXL = NdotV * (NdotL * (1 - a) + a);
//	return 2 / (GGXL + GGXV);
//}

float V_SmithGGXCorrelated(float3 N, float3 V, float3 L, float alpha)
{
	float NdotL = max(0.0, dot(N, L));
	float NdotV = max(0.0, dot(N, V));
	
	float a2 = square(alpha);
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

float3 BRDFDirect(float3 N, float3 V, float3 L, float alpha, float3 albedo, float3 F0)
{
	float3 H = normalize(V + L);
	
	float d = D(N, H, alpha);
	float v = V_SmithGGXCorrelated(N, V, L, alpha);
	float3 f = F(F0, H, V);

	float3 cook_torrance = d * v * f;
	float3 lambertian = albedo / PI * (1.0 - f) * (1.0 - uMetallic);
	
	return (cook_torrance) * uLightIntensity;
}

float4 PS(VertexOut pIn) : SV_Target
{
	float3 N = normalize(pIn.normal);
	float3 V = normalize(uCameraPos - pIn.position);
	float3 L = normalize(uLightPos - pIn.position);
	
	float3 albedo = baseColor.Sample(baseColorSampler, pIn.texCoord).xyz * uTint;
	float3 F0 = float3(0.04, 0.04, 0.04);
	F0 = lerp(F0, albedo, uMetallic);

	float roughness = lerp(0.01, 1.0, uRoughness);
	float alpha = roughness * roughness;
	
	float NdotL = max(0.0, dot(N, L));
	float NdotV = max(0.0, dot(V, N));
	
	
	float Eavg = EavgTexture.Sample(EavgSampler, float2(alpha, 0)).x;
	float EmuL = EmuTexture.Sample(EmuSampler, float2(NdotL, 1.0 - alpha)).x;
	float EmuV = EmuTexture.Sample(EmuSampler, float2(NdotV, 1.0 - alpha)).x;
	
	float OneMinusEavg = 1.0 - Eavg;
	float3 Favg = (1.0 + F0 * 20.0) / 21.0;
	
	float3 Fss = BRDFDirect(N, V, L, alpha, albedo, F0);
	float3 Fms = (1.0 - EmuL) * (1.0 - EmuV) * Eavg * (Favg * Favg)
	/ (PI * OneMinusEavg * (1.0 - Favg * OneMinusEavg));
	//float3 Fms = (1.0 - EmuL) * (1.0 - EmuV) 
	/// (PI * OneMinusEavg);
	
	float3 color = (Fss + Fms) * NdotL;

	color = color / (color + 1.0);
	color = pow(color, 1.0 / 2.2);
	return float4(color, 1.0);
}