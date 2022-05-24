Texture2D LastFrameTexture : register(t0);

// Float 1
Texture2D SceneData : register(t1);
// Float 4
Texture2D GeometryData : register(t2);

SamplerState MeshTextureSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};


cbuffer ConstantBuffer : register(b0)
{
	float4x4 MVP;
	
	float uIteration;
	float AspectRatio;
	float FOV;
	float3 Up;
	float3 Right;
	float3 Front;
	float3 EyePos;
	
	float uFrameWidth;
	float uFrameHeight;
}

static const float RT_GEOMETRY_COLLECTION = 1.0;

static const float RT_GEOMETRY_SPHERE = 1.0;
static const float RT_GEOMETRY_TRIANGLE = 2.0;

static const float MAX_FLOAT = 9999999.0;
static const float PI = 3.14159265;
static const float EPS = 1e-4;

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
	vout.texCoord = float2(vIn.pos.x, vIn.pos.y);
	return vout;
}

struct Ray
{
	float3 Start;
	float3 Dir;
};

struct Sphere
{
	float3 Center;
	float R;
};


struct Triangle
{
	float3 P[3];
};

struct SurfaceInteraction
{
	float	GeometryId;
	float	t;
	float3  Normal;
};


bool SphereIntersect(int index, Ray ray, out SurfaceInteraction isec);
bool TriangleIntersect(int index, Ray ray, out SurfaceInteraction isec);
float RandXY(float x, float y);
float GetValue(float index, Texture2D tex);
float4 GetData(float index, Texture2D tex);
float3x3 BuildTNB(float3 N);



float RandXY(float2 coord)
{
	return frac(cos(coord.x * (12.9898) + coord.y * (4.1414)) * 43758.5453);
}

float RandXY(float x, float y)
{
	return frac(cos(x * (12.9898) + y * (4.1414)) * 43758.5453);
}

float3x3 BuildTNB(float3 N)
{
	float3 n = normalize(N);
	float3 v = abs(dot(n, float3(0, 0, 1))) < 1e-3 ? float3(1, 0, 0) : float3(0, 0, 1);
	float3 T = normalize(cross(v, n));
	float3 B = normalize(cross(n, T));
	return float3x3(T, n, B);
}

float GetValue(float index, Texture2D tex)
{
	float width, height;
	tex.GetDimensions(width, height);
	
	float row = index / width;
	float y = int(row) / height;
	float x = (index - (int) (row) * width) / width;

	return tex.SampleLevel(MeshTextureSampler, float2(x, y), 0).r;
}

float4 GetData(float index, Texture2D tex)
{
	float width, height;
	tex.GetDimensions(width, height);
	
	float row = index / width;
	float y = int(row) / height;
	float x = (index - (int) (row) * width) / width;

	return tex.SampleLevel(MeshTextureSampler, float2(x, y), 0);
}

Ray GenerateRay(float2 coord)
{
	float upLength = tan(FOV / 2.f);
	float rightLength = upLength * AspectRatio;
	float2 uv = coord * 2.f - 1.f;
	float3 dir = float3(1.f, uv.y * upLength, uv.x * rightLength);
	dir = dir.x * Front + dir.y * Up + dir.z * Right;
	
	Ray ray;
	ray.Start = EyePos;
	ray.Dir = normalize(dir);
	return ray;
}

bool SphereIntersect(int index, Ray ray, out SurfaceInteraction isec)
{
	float4 data = GetData(index, GeometryData);
	Sphere sphere;
	sphere.Center = data.xyz;
	sphere.R = data.w;
	
	float3 P = ray.Start - sphere.Center;
	float a = dot(ray.Dir, ray.Dir);
	float b = 2 * dot(ray.Dir, P);
	float c = dot(P, P) - sphere.R * sphere.R;
	float discrim = b * b - 4 * a * c;
	if (discrim < 0)
		return false;
	discrim = sqrt(discrim);
	float t1 = (-b + discrim) / (2 * a);
	float t2 = (-b - discrim) / (2 * a);
	
	// swap
	if (t2 < t1)
	{
		float t = t2;
		t2 = t1;
		t1 = t;
	}
	
	isec.t = t1 < 0 ? t2 : t1;
	isec.GeometryId = index;
	isec.Normal = normalize(ray.Start + ray.Dir * isec.t - sphere.Center);
	return isec.t >= 0;
}

bool TriangleIntersect(int index, Ray ray, out SurfaceInteraction isec)
{
	float4 point1 = GetData(index, GeometryData);
	float4 point2 = GetData(index + 1, GeometryData);
	float4 point3 = GetData(index + 2, GeometryData);
	
	Triangle face;
	
	face.P[0] = point1.xyz;
	face.P[1] = point2.xyz;
	face.P[2] = point3.xyz;
	
	
	float3 E1 = face.P[1] - face.P[0];
	float3 E2 = face.P[2] - face.P[0];
	float3 D = ray.Dir;
	float3 T = ray.Start - face.P[0];
	
	float3 A = cross(D, E2);
	float3 B = cross(T, E1);

	if (abs(dot(A, E1)) < EPS)
	{
		return false;
	}
		
	float det = 1 / (dot(A, E1));
	
	float t = dot(B, E2) * det;
	float u = dot(A, T) * det;
	float v = dot(B, D) * det;
	
	if (u < 0 || u > 1 || v < 0 || v > 1 || u + v > 1 + EPS)
	{
		return false;
	}
	
	isec.GeometryId = index;
	isec.Normal = normalize(cross(E1, E2));
	isec.t = t;
	return t >= 0;
}


bool Scene_Intersect(Ray ray, out SurfaceInteraction isec)
{
	float type = GetValue(0, SceneData);
	
	isec.Normal = float3(0, 0, 0);
	isec.GeometryId = -1;
	isec.t = MAX_FLOAT;
	
	bool hitSomething = false;
	
	if (type == RT_GEOMETRY_COLLECTION)
	{
		float count = GetValue(1, SceneData);

		for (int i = 0; i < count; i++)
		{
			float geometryType = GetValue(i * 2 + 2, SceneData);
			float geometryId = GetValue(i * 2 + 3, SceneData);
			
			SurfaceInteraction tmp;
			
			if (geometryType == RT_GEOMETRY_SPHERE)
			{
				if (SphereIntersect(geometryId, ray, tmp))
				{
					if (tmp.t < isec.t)
					{
						isec = tmp;
					}
					hitSomething = true;
				}
			}
			else if (geometryType == RT_GEOMETRY_TRIANGLE)
			{
				if (TriangleIntersect(geometryId, ray, tmp))
				{
					if (tmp.t < isec.t)
					{
						isec = tmp;
					}
					hitSomething = true;
				}
			}
		}
	}
	return hitSomething;
}


float4 PS(VertexOut pIn) : SV_Target
{
	SurfaceInteraction isec;
	
	float sampleA = RandXY(RandXY(RandXY(RandXY(pIn.texCoord * 10.0), 44), 114), uIteration);
	float sampleB = RandXY(RandXY(RandXY(RandXY(pIn.texCoord * 10.0), 33), 514), uIteration);
	
	float2 invFrameSize = float2(1.0 / uFrameWidth, 1.0 / uFrameHeight);
	Ray ray = GenerateRay(pIn.texCoord + invFrameSize * float2(sampleA, sampleB));
	
	float3 Li = float3(0, 0, 0);
	float3 beta = float3(1, 1, 1);
	
	for (int i = 0; i < 10; i++)
	{
		if (Scene_Intersect(ray, isec))
		{
			float sampleX = RandXY(RandXY(RandXY(RandXY(pIn.texCoord * 10.0), i), 114), uIteration);
			float sampleY = RandXY(RandXY(RandXY(RandXY(pIn.texCoord * 10.0), i), 514), uIteration);
			
			float r = sqrt(sampleX);
			float phi = PI * 2 * sampleY;

			float x = r * cos(phi);
			float z = r * sin(phi);
			float y = sqrt(1.0f - r * r);
			
			float3x3 TNB = BuildTNB(isec.Normal);
			
			Ray newRay;
			newRay.Start = ray.Start + ray.Dir * isec.t + isec.Normal * 1e-3;
			newRay.Dir = mul(float3(x, y, z), TNB);
			
			ray = newRay;
			beta *= float3(0.9, 0.5, 0.6);

		}
		else
		{
			Li += beta * float3(0.5, 0.5, 0.5);
			break;
		}
	}
	
	float2 uv = float2(pIn.texCoord.x, 1 - pIn.texCoord.y);
	return (LastFrameTexture.Sample(MeshTextureSampler, uv) * uIteration + float4(Li, 1.0)) / (uIteration + 1);
}