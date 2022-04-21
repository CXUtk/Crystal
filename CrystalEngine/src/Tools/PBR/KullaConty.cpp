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

static constexpr size_t KC_TEXTURE_SIZE = 256;

int main(int argc, char** argv)
{
    auto samples = GenerateSamples(32);
    std::shared_ptr<RawTexture2D> texture = std::make_shared<RawTexture2D>(KC_TEXTURE_SIZE, KC_TEXTURE_SIZE);
    std::shared_ptr<RawTexture2D> EAvgTexture = std::make_shared<RawTexture2D>(KC_TEXTURE_SIZE, 1);
    for (int i = 0; i <= KC_TEXTURE_SIZE - 1; i++)
    {
        // N 和 V 的夹角，因为 H 只在积分中出现
        float cosTheta = (float)(i + 0.5f) / KC_TEXTURE_SIZE;
        for (int j = 0; j <= KC_TEXTURE_SIZE - 1; j++)
        {
            float roughness = (float)(j + 0.5f) / KC_TEXTURE_SIZE;
            texture->SetPixel(Vector2i(i, j), SampleBRDF(samples, roughness, cosTheta));
        }
    }
    auto data = texture->GetByteData();
    stbi_write_png("E.png", texture->Width, texture->Height, 3, data.data(), 3 * texture->Width);


    for (int i = 0; i < KC_TEXTURE_SIZE; i++)
    {
        Vector3f sampledEavg(0.f);
        for (int j = 0; j < KC_TEXTURE_SIZE; j++)
        {
            sampledEavg += texture->Sample(Vector2i(j, i));
        }
        EAvgTexture->SetPixel(Vector2i(i, 0), sampledEavg * (1.f / KC_TEXTURE_SIZE));
    }
    stbi_write_png("Eavg.png", EAvgTexture->Width, EAvgTexture->Height, 3, EAvgTexture->GetByteData().data(),
        3 * texture->Width);
    return 0;
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


float V_SmithGGXCorrelated(const Vector3f& N, const Vector3f& V, const Vector3f& L, float alpha)
{
    float NdotL = std::max(0.0f, glm::dot(N, L));
    float NdotV = std::max(0.0f, glm::dot(N, V));

    float a2 = square(alpha);
    float GGXL = NdotV * std::sqrt((-NdotL * a2 + NdotL) * NdotL + a2);
    float GGXV = NdotL * std::sqrt((-NdotV * a2 + NdotV) * NdotV + a2);
    return 0.5f / (GGXV + GGXL);
}

Vector3f SampleBRDF(const std::vector<Vector2f>& samples,
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

            // Fr = DVF
            // pdf = D * NoH / (4 * VoH)
            // Total = DVF * NoL / (D * NoH / (4 * VoH))
            //       = 4VF * VoH / NoH

            float Vis = V_SmithGGXCorrelated(Vector3f(0, 1, 0), V, L, roughness);

            float GWf = (4 * Vis * VoH * L.y) / NoH;

            sampledValue += Vector3f(GWf);
        }
    }

    float invN = 1.f / samples.size();
    return sampledValue * invN;
}