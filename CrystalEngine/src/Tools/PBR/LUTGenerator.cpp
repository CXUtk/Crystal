#include <Core/Sampling/Sampling.h>
#include <Core/Sampling/ImageSampling.h>
#include <Core/Utils/Logger.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stbi/stb_image.h>
#include <stbi/stb_image_write.h>

using namespace crystal;

/**
 * @brief Generate N x N samples
 * @param N
 * @return
*/
static std::vector<Vector2f> GenerateSamples(size_t N);
static Vector3f SampleBRDF(const std::vector<Vector2f>& samples,
    float roughness,
    float cosTheta);
static Vector3f SampleSkyCubemap(const Vector3f& v);
static Vector3f GGXImportanceSample(const Vector2f& sample, float roughness);

static std::shared_ptr<RawTexture2D> SkyCubemap[6];
static std::shared_ptr<RawTexture2D> CubemapMipmap[6];

static constexpr size_t LUT_TEXTURE_SIZE = 256;

int main(int argc, char** argv)
{
    auto samples = GenerateSamples(32);
    std::shared_ptr<RawTexture2D> texture = std::make_shared<RawTexture2D>(LUT_TEXTURE_SIZE, LUT_TEXTURE_SIZE);
    for (int i = 0; i <= LUT_TEXTURE_SIZE - 1; i++)
    {
        // N 和 V 的夹角，因为 H 只在积分中出现
        float cosTheta = (float)(i + 0.5f) / LUT_TEXTURE_SIZE;
        for (int j = 0; j <= LUT_TEXTURE_SIZE - 1; j++)
        {
            float roughness = (float)(j + 0.5f) / LUT_TEXTURE_SIZE;
            texture->SetPixel(Vector2i(i, j), SampleBRDF(samples, roughness, cosTheta));
        }
    }
    auto data = texture->GetByteData();
    stbi_write_jpg("LUT.jpg", texture->Width, texture->Height, 3, data.data(), 100);
    return 0;
}

Vector3f SampleSkyCubemap(const Vector3f& v)
{
    auto cube = XYZToCubeUV(v);
    return SkyCubemap[cube.Id]->Sample(cube.UV);
}

Vector3f GGXImportanceSample(const Vector2f& sample, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float cosTheta = std::sqrt((1 - sample.x) / (sample.x * (a2 - 1) + 1));
    float phi = glm::two_pi<float>() * sample.y;
    return GetUnitVectorUsingCos(cosTheta, phi);
}

std::vector<Vector2f> GenerateSamples(size_t N)
{
    static RNG rng;

    std::vector<Vector2f> samples;
    float unit = 1.f / N;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            float x = UniformRandomFloat(rng);
            float y = UniformRandomFloat(rng);
            samples.push_back(Vector2f(i + x, j + y) * unit);
        }
    }
    return samples;
}


float D(const Vector3f& N, const Vector3f& H, float roughness)
{
    float a = square(roughness);
    float a2 = square(a);
    float NdotH = std::max(0.f, glm::dot(N, H));
    return a2 / (glm::pi<float>() * square(square(NdotH) * (a2 - 1.0) + 1.0));
}


float G1(const Vector3f& N, const Vector3f& V, float roughness)
{
    float k = square(roughness) / 2;
    float NdotV = std::max(0.f, dot(N, V));
    return NdotV / (NdotV * (1.0 - k) + k);
}

float G(const Vector3f& N, const Vector3f& V, const Vector3f& L, float roughness)
{
    float g1 = G1(N, V, roughness);
    float g2 = G1(N, L, roughness);
    return  g1 * g2;
}


Vector3f SampleBRDF( const std::vector<Vector2f>& samples,
    float roughness, float NdotV)
{
    Vector3f sampledValue(0.f);
    Vector3f V(std::sqrt(1.f - NdotV * NdotV), NdotV, 0);
    for (auto& sample : samples)
    {
        Vector3f H = GGXImportanceSample(sample, roughness);
        Vector3f L = glm::reflect(-V, H);

        if (L.y > 0)
        {
            float VoH = std::max(0.f, glm::dot(V, H));
            float NoH = std::max(0.f, H.y);
            // Incident light = SampleColor * NoL
            // Microfacet specular = D*G*F / (4*NoL*NoV)
            // pdf = D * NoH / (4 * VoH)
            // Total = fr * NoL / pdf

            float g = G(Vector3f(0, 1, 0), V, L, roughness);

            float GWf = (g * VoH) / (NdotV * NoH);
            float t = std::pow(1.0f - VoH, 5);

            sampledValue.x += (1.0f - t) * GWf;
            sampledValue.y += t * GWf;
        }
    }

    float invN = 1.f / samples.size();
    return sampledValue * invN;
}