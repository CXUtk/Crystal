Texture2D LastFrameTexture : register(t0);

// Float 1
Texture2D SceneData : register(t1);
// Float 4
Texture2D ObjectData : register(t2);

TextureCube skyboxTexture : register(t3);

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
static const float RT_GEOMETRY_BVH_NODE = 2.0;

static const float RT_GEOMETRY_SPHERE = 1.0;
static const float RT_GEOMETRY_TRIANGLE = 2.0;

static const float RT_LIGHT_POINT = 1.0;
static const float RT_LIGHT_AREA = 2.0;

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
	float3 N[3];
	float3 Tex[3];
};

struct SurfaceInteraction
{
	int	    GeometryId;
	float	t;
	float3  Normal;
	float3  Albedo;
};

struct SurfacePatch
{
	float3 Position;
	float3 Normal;
};

SurfaceInteraction newSurfaceInteraction()
{
	SurfaceInteraction isec;
	isec.Normal = float3(0, 0, 0);
	isec.GeometryId = -1;
	isec.t = MAX_FLOAT;
	isec.Albedo = float3(0, 0, 0);
	return isec;
}

SurfacePatch newSurfacePatch()
{
	SurfacePatch patch;
	patch.Normal = float3(0, 0, 0);
	patch.Position = float3(0, 0, 0);
	return patch;
}

struct Bound3f
{
	float3 minPos;
	float3 maxPos;
};

bool SphereIntersect(int index, Ray ray, inout SurfaceInteraction isec);
bool TriangleIntersect(int index, Ray ray, inout SurfaceInteraction isec);
bool RayBoxIntersect(Bound3f bound, Ray ray, bool3 dirIsNeg, float3 invDir,
	float tMin, float tMax);
bool Do_RayObjectIntersect(int index, Ray ray, inout SurfaceInteraction isec);
bool Scene_Intersect(Ray ray, inout SurfaceInteraction isec);

float RandXY(float x, float y);
float GetValue(float index, Texture2D tex);
float4 GetData(float index, Texture2D tex);
float3x3 BuildTNB(float3 N);
void ExtractBoundingBox(float index, out Bound3f bound);

static int counter = 0;


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
	float3 v = abs(dot(n, float3(0, 0, 1))) == 1.0 ? float3(1, 0, 0) : float3(0, 0, 1);
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

bool SphereIntersect(int index, Ray ray, inout SurfaceInteraction isec)
{
	float4 data = GetData(index, ObjectData);
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
	isec.GeometryId = index - 1;
	isec.Normal = normalize(ray.Start + ray.Dir * isec.t - sphere.Center);
	isec.Albedo = GetData(index + 1, ObjectData).xyz;
	return isec.t >= 0;
}

bool TriangleIntersect(int index, Ray ray, inout SurfaceInteraction isec)
{
	float4 point1 = GetData(index, ObjectData);
	float4 point2 = GetData(index + 1, ObjectData);
	float4 point3 = GetData(index + 2, ObjectData);
	
	float4 normal1 = GetData(index + 3, ObjectData);
	float4 normal2 = GetData(index + 4, ObjectData);
	float4 normal3 = GetData(index + 5, ObjectData);
	
	float4 tex1 = GetData(index + 6, ObjectData);
	float4 tex2 = GetData(index + 7, ObjectData);
	float4 tex3 = GetData(index + 8, ObjectData);
	
	Triangle face;
	
	face.P[0] = point1.xyz;
	face.P[1] = point2.xyz;
	face.P[2] = point3.xyz;
	
	face.N[0] = normal1.xyz;
	face.N[1] = normal2.xyz;
	face.N[2] = normal3.xyz;
	
	face.Tex[0] = point1.xyz;
	face.Tex[1] = point2.xyz;
	face.Tex[2] = point3.xyz;
	
	
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
	
	isec.GeometryId = index - 1;
	//isec.Normal = normalize(cross(E1, E2));
	
	isec.Normal = normalize(face.N[0] * (1 - u - v) + face.N[1] * u + face.N[2] * v);
	if (dot(isec.Normal, ray.Dir) > 0)
	{
		isec.Normal = -isec.Normal;
	}
	isec.t = t;
	isec.Albedo = GetData(index + 9, ObjectData).rgb;
	return t >= 0;
}

bool RayBoxIntersect(Bound3f bound, Ray ray, bool3 dirIsNeg, float3 invDir, float tMin, float tMax)
{
	float3 minP = (bound.minPos - ray.Start) * invDir;
	float3 maxP = (bound.maxPos - ray.Start) * invDir;
	
	if (dirIsNeg.x)
	{
		float t = minP.x;
		minP.x = maxP.x;
		maxP.x = t;
	}
	tMin = max(tMin, minP.x);
	tMax = min(tMax, maxP.x);
	if (tMax < tMin)
	{
		return false;
	}
	
	if (dirIsNeg.y)
	{
		float t = minP.y;
		minP.y = maxP.y;
		maxP.y = t;
	}
	tMin = max(tMin, minP.y);
	tMax = min(tMax, maxP.y);
	if (tMax < tMin)
	{
		return false;
	}
	
	if (dirIsNeg.z)
	{
		float t = minP.z;
		minP.z = maxP.z;
		maxP.z = t;
	}
	tMin = max(tMin, minP.z);
	tMax = min(tMax, maxP.z);
	if (tMax < tMin)
	{
		return false;
	}
	return true;
}

void ExtractBoundingBox(float index, out Bound3f bound)
{
	bound.minPos = GetData(index, ObjectData).xyz;
	bound.maxPos = GetData(index + 1, ObjectData).xyz;
}

bool Do_RayObjectIntersect(int index, Ray ray, inout SurfaceInteraction isec)
{	
	SurfaceInteraction tmp = newSurfaceInteraction();
	float geometryType = GetData(index, ObjectData).x;
	if (geometryType == RT_GEOMETRY_SPHERE)
	{
		if (SphereIntersect(index + 1, ray, tmp))
		{
			if (tmp.t < isec.t)
			{
				isec = tmp;
			}
			return true;
		}
	}
	else if (geometryType == RT_GEOMETRY_TRIANGLE)
	{
		if (TriangleIntersect(index + 1, ray, tmp))
		{
			if (tmp.t < isec.t)
			{
				isec = tmp;
			}
			return true;
		}
	}
	return false;
}

void SampleArea(int primOffset, float2 sampleL, inout SurfacePatch patch, out float pdf)
{
	float geometryType = GetData(primOffset, ObjectData).x;
	int index = primOffset + 1;
	pdf = 1.f;
	if (geometryType == RT_GEOMETRY_TRIANGLE)
	{
		float u = 1 - sqrt(sampleL.x);
		float v = sampleL.y * sqrt(sampleL.x);
		
		float4 point1 = GetData(index, ObjectData);
		float4 point2 = GetData(index + 1, ObjectData);
		float4 point3 = GetData(index + 2, ObjectData);
	
		float4 normal1 = GetData(index + 3, ObjectData);
		float4 normal2 = GetData(index + 4, ObjectData);
		float4 normal3 = GetData(index + 5, ObjectData);
		
		Triangle face;
		face.P[0] = point1.xyz;
		face.P[1] = point2.xyz;
		face.P[2] = point3.xyz;
	
		face.N[0] = normal1.xyz;
		face.N[1] = normal2.xyz;
		face.N[2] = normal3.xyz;
		
		// temp
		face.Tex[0] = normal1.xyz;
		face.Tex[1] = normal2.xyz;
		face.Tex[2] = normal3.xyz;
		
		patch.Position = face.P[0] * (1 - u - v) + face.P[1] * u + face.P[2] * v;
		patch.Normal = normalize(face.N[0] * (1 - u - v) + face.N[1] * u + face.N[2] * v);
		pdf = 1.0 / (0.5 * length(cross(face.P[1] - face.P[0], face.P[2] - face.P[0])));
	}
}

float3 SampleLights(SurfacePatch refP, float2 sampleS)
{
	float lightsOffset = GetValue(1, SceneData);
	float lightCount = GetValue(lightsOffset, SceneData);
	float currentPos = lightsOffset + 1;
	float3 Li = float3(0, 0, 0);
	for (int i = 0; i < lightCount; i++)
	{
		float lightOffset = GetValue(currentPos + i, SceneData);
		
		float lightType = GetData(lightOffset, ObjectData);
		SurfacePatch patch = newSurfacePatch();
		if (lightType == RT_LIGHT_AREA)
		{
			float4 params1 = GetData(lightOffset + 1, ObjectData);
			
			int primOffset = int(params1.x);
			float3 Le = params1.yzw;
			
			float x = RandXY(sampleS.x, sampleS.y);
			float2 sampleL = float2(RandXY(RandXY(x, i * 0.1), 0.15), RandXY(RandXY(i * 0.1, x), 0.73));
			
			float pdfLight;
			SampleArea(primOffset, sampleL, patch, pdfLight);
			
			
			//patch.Position = float3(0, 15, 0);
			float3 d = patch.Position - refP.Position;
			float3 wi = normalize(d);
			pdfLight *= dot(d, d) / dot(patch.Normal, -wi);
			
			
			Ray lightRay;
			lightRay.Start = refP.Position + refP.Normal * 1e-3;
			lightRay.Dir = patch.Position - lightRay.Start;
		
			float len2 = dot(patch.Position - refP.Position, patch.Position - refP.Position);
			
			SurfaceInteraction isec2 = newSurfaceInteraction();
			if (Scene_Intersect(lightRay, isec2))
			{
				if (isec2.GeometryId == primOffset)
				{
					Li += Le * max(0, dot(refP.Normal, wi)) / pdfLight;
				}
				//Li += float3(0.3, 0.3, 0.3);

				//Li += abs(isec2.GeometryId - primOffset) / 10.0;

			}
		}
	}
	return Li;
}

bool Scene_Intersect(Ray ray, inout SurfaceInteraction isec)
{
	bool hitSomething = false;
	
	float3 invDir = float3(1.f / ray.Dir.x, 1.f / ray.Dir.y, 1.f / ray.Dir.z);
	bool3 dirIsNeg = bool3(ray.Dir.x < 0, ray.Dir.y < 0, ray.Dir.z < 0);
	
	int TraceStack[64];
	int StackPtr = 0;
	
	float primitivesOffset = GetValue(0, SceneData);
	
	TraceStack[0] = primitivesOffset;
	StackPtr = 0;
	
	while (StackPtr >= 0)
	{
		float currentPos = TraceStack[StackPtr];
		StackPtr--;
		counter++;
		
		float type = GetValue(currentPos, SceneData);
		float ptrBBox = GetValue(currentPos + 1, SceneData);
		Bound3f bound;
		ExtractBoundingBox(ptrBBox, bound);
		
		if (type == RT_GEOMETRY_COLLECTION)
		{
			float tMin = 0;
			float tMax = isec.t;
			if (!RayBoxIntersect(bound, ray, dirIsNeg, invDir, tMin, tMax))
			{
				continue;
			}
		
			float count = GetValue(currentPos + 2, SceneData);
			for (int i = 0; i < count; i++)
			{
				int geometryId = int(GetValue(currentPos + 3 + i, SceneData));
				if (Do_RayObjectIntersect(geometryId, ray, isec))
				{
					hitSomething = true;
				}
			}
		}
		else if (type == RT_GEOMETRY_BVH_NODE)
		{
			float tMin = 0;
			float tMax = isec.t;
			if (!RayBoxIntersect(bound, ray, dirIsNeg, invDir, tMin, tMax))
			{
				continue;
			}
			int splitAxis = int(GetValue(currentPos + 2, SceneData));
			float left = GetValue(currentPos + 3, SceneData);
			float right = GetValue(currentPos + 4, SceneData);
			
			if (dirIsNeg[splitAxis])
			{
				TraceStack[++StackPtr] = left;
				TraceStack[++StackPtr] = right;
			}
			else
			{
				TraceStack[++StackPtr] = right;
				TraceStack[++StackPtr] = left;
			}
		}
	}
	return hitSomething;
}

float4 PS(VertexOut pIn) : SV_Target
{
	SurfaceInteraction isec = newSurfaceInteraction();
	
	float sampleA = RandXY(RandXY(RandXY(pIn.texCoord), uIteration * pIn.texCoord.x), pIn.texCoord.x);
	float sampleB = RandXY(pIn.texCoord.y, RandXY(RandXY(pIn.texCoord), uIteration * pIn.texCoord.y));
	
	float2 invFrameSize = float2(1.0 / uFrameWidth, 1.0 / uFrameHeight);
	Ray ray = GenerateRay(pIn.texCoord + invFrameSize * float2(sampleA, sampleB));
	
	float3 Li = float3(0, 0, 0);
	float3 beta = float3(1, 1, 1);
	
	counter = 0;
	
	for (int i = 0; i < 5; i++)
	{
		if (Scene_Intersect(ray, isec))
		{
			float sampleX = RandXY(RandXY(sampleA, i), RandXY(uIteration * 0.1, pIn.texCoord.x));
			float sampleY = RandXY(RandXY(uIteration * 0.1, pIn.texCoord.y), RandXY(sampleB, i));
			
			SurfacePatch isecPatch;
			isecPatch.Position = ray.Start + ray.Dir * isec.t;
			isecPatch.Normal = isec.Normal;
			
			Li += beta * SampleLights(isecPatch, float2(sampleX, sampleY));
			
			beta *= isec.Albedo;
		
			
			//float r = sqrt(sampleX);
			float phi = PI * 2 * sampleY;
			//float cosTheta = pow(sampleX, 1.0 / (200 + 1));
			//float sinTheta = sqrt(max(0, 1 - cosTheta * cosTheta));
			float sinTheta = sqrt(sampleX);
			float cosTheta = sqrt(max(0, 1 - sinTheta * sinTheta));

			float x = sinTheta * cos(phi);
			float z = sinTheta * sin(phi);
			float y = cosTheta;
			
			//float3 R = reflect(ray.Dir, isec.Normal);
			float3x3 TNB = BuildTNB(isec.Normal);
			
			Ray newRay;
			newRay.Start = ray.Start + ray.Dir * isec.t + isec.Normal * 1e-3;
			newRay.Dir = normalize(mul(float3(x, y, z), TNB));
			ray = newRay;

		}
		else
		{
			Li += beta * pow(skyboxTexture.SampleLevel(MeshTextureSampler, ray.Dir, 0).rgb, 2.2);
			break;
		}
	}
	//float c = counter / 100.0;
	//return float4(c, c, c, 1);
	float2 uv = float2(pIn.texCoord.x, 1 - pIn.texCoord.y);
	if (uIteration == 1.0)
	{
		return float4(Li, 1.0);
	}
	return (LastFrameTexture.Sample(MeshTextureSampler, uv) * (uIteration - 1) + float4(Li, 1.0)) / (uIteration);
}