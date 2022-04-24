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
static std::shared_ptr<RawTexture2D> EmuTexture;
static std::shared_ptr<RawTexture2D> EavgTexture;

static constexpr size_t LUT_TEXTURE_SIZE = 256;


std::shared_ptr<RawTexture2D> LoadImg(const std::string& path)
{
    int width, height, channels;
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 3);
    if (!data)
    {
        throw std::logic_error("Cannot find given image");
    }
    auto ptr = std::make_shared<RawTexture2D>(width, height, data, true);
    stbi_image_free(data);
    return ptr;
}

int main(int argc, char** argv)
{
    EmuTexture = LoadImg("E.png");
    EavgTexture = LoadImg("Eavg.png");
    auto samples = GenerateSamples(32);
    std::shared_ptr<RawTexture2D> texture = std::make_shared<RawTexture2D>(LUT_TEXTURE_SIZE, LUT_TEXTURE_SIZE);
    for (int i = 0; i <= LUT_TEXTURE_SIZE - 1; i++)
    {
        // N 和 V 的夹角，因为 H 只在积分中出现
        float cosTheta = (float)(i + 0.5f) / LUT_TEXTURE_SIZE;
        for (int j = 0; j <= LUT_TEXTURE_SIZE - 1; j++)
        {
            float roughness = (float)(j + 0.5f) / LUT_TEXTURE_SIZE;
            auto value = SampleBRDF(samples, roughness, cosTheta);
            //if (value.r + value.g + value.b > 1.f)
            //{
            //    printf("%f %f %f\n", value.r,value.g,value.b);
            //}
            texture->SetPixel(Vector2i(i, j), value);
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

Vector3f GGXImportanceSample(const Vector2f& sample, float alpha)
{
    float a2 = alpha * alpha;
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


float D(const Vector3f& N, const Vector3f& H, float alpha)
{
    float a2 = square(alpha);
    float NdotH = std::max(0.0f, glm::dot(N, H));
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

Vector3f SampleBRDF( const std::vector<Vector2f>& samples,
    float roughness, float NdotV)
{
    float alpha = roughness * roughness;
    Vector3f sampledValue(0.f);
    Vector3f V(std::sqrt(1.f - NdotV * NdotV), NdotV, 0);
    Vector3f N = Vector3f(0, 1, 0);

    float count = 0.f;
    for (auto& sample : samples)
    {
        Vector3f H = GGXImportanceSample(sample, alpha);
        Vector3f L = glm::reflect(-V, H);
        
        if (L.y > 0)
        {
            float VoH = std::max(0.f, glm::dot(V, H));
            float NoH = std::max(0.f, H.y);
            float NoL = std::max(0.f, glm::dot(L, H));
            float NoV = std::max(0.f, glm::dot(V, H));
            // Incident light = SampleColor * NoL
            // Microfacet specular = D*G*F / (4*NoL*NoV)
            // pdf = D * NoH / (4 * VoH)
            // Total = fr * NoL / pdf

            // Fr = DVF + x
            // pdf = D * NoH / (4 * VoH)
            // Total = (DVF + x) * HoV / (D * NoH / (4 * VoH))
            //       = (DVF*NoL + x*NoL) / (D * NoH / (4 * VoH))
            //       = 4VF * NoL * VoH / NoH  +  4 * VoH * x * NoL/ (D * NoH)

            float Vis = V_SmithGGXCorrelated(N, V, L, alpha);

            Vector3f EmuL = 1.f - EmuTexture->Sample(Vector2f(NoL, roughness));

            float GWf = (Vis * VoH * L.y) / NoH;
            float fms = 1.0 - EmuL.x;
            float ext = VoH * L.y * fms / (D(N, H, alpha) * NoH);
            //GWf += ext;
            float t = std::pow(1.0f - VoH, 5);

            sampledValue.x += (1.0f - t) * GWf;
            sampledValue.y += t * GWf;
            // sampledValue.z += ext;
        }
        count++;
    }

    float invN = 4.f / count;
    return sampledValue * invN;
}