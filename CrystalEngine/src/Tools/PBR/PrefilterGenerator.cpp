#include <Core/Sampling/Sampling.h>
#include <Core/Sampling/ImageSampling.h>
#include <Core/Utils/Logger.h>
#include <Core/Utils/Misc.h>

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
static Vector3f SampleOneNormal(const Vector3f& N, const std::vector<Vector2f>& samples,
    float roughness);
static Vector3f SampleSkyCubemap(const Vector3f& v);
static Vector3f GGXImportanceSample(const Vector2f& sample, float roughness);

static const char* filenames[6] = {
    "posx.jpg",
    "negx.jpg",
    "posy.jpg",
    "negy.jpg",
    "posz.jpg",
    "negz.jpg",
};
static const char* filenames_output[6] = {
    "posx_specular_mipmap",
    "negx_specular_mipmap",
    "posy_specular_mipmap",
    "negy_specular_mipmap",
    "posz_specular_mipmap",
    "negz_specular_mipmap",
};

static const float roughness_level[6] = {
    0.02,
    0.25,
    0.5,
    0.75,
    1.0,
    0
};

static std::shared_ptr<RawTexture2D> SkyCubemap[6];
static std::shared_ptr<RawTexture2D> CubemapMipmap[6];

static constexpr size_t CUBEMAP_SIZE = 128;
static constexpr size_t MIPMAP_ROUGHNESS_LEVELS = 5;

int main(int argc, char** argv)
{
    // Load textures
    for (int i = 0; i < 6; i++)
    {
        int width, height, channels;
        stbi_uc* data = stbi_load(filenames[i], &width, &height, &channels, 3);
        if (!data)
        {
            throw std::logic_error("Cannot find cubemap images");
        }
        SkyCubemap[i] = std::make_shared<RawTexture2D>(width, height, data, true);
        stbi_image_free(data);

        SkyCubemap[i]->MipmapCompress(3);
    }
    GlobalLogger::Log(SeverityLevel::Info, "Loaded Images");

    auto samples = GenerateSamples(64);

    for (size_t level = 0; level < MIPMAP_ROUGHNESS_LEVELS; level++)
    {
        int width = CUBEMAP_SIZE >> level;
        int height = CUBEMAP_SIZE >> level;
        for (size_t f = 0; f < 6; f++)
        {
            CubemapMipmap[f] = std::make_shared<RawTexture2D>(width, height, true);

            // Sample on origin-centered unit cube
            float unit = 1.f / width;
            for (size_t i = 0; i < height; i++)
            {
                for (size_t j = 0; j < width; j++)
                {
                    // Sample the center of each pixel
                    Vector2f pos = Vector2f(j + 0.5f, i + 0.5f) * unit;

                    auto vector = CubeUVToVector(CubeUV{ (int)f, pos });
                    auto N = glm::normalize(vector);
                    auto color = SampleOneNormal(N, samples, roughness_level[level]);
                    CubemapMipmap[f]->SetPixel(Vector2i(j, i), color);
                }
            }

            GlobalLogger::Log(SeverityLevel::Info, "Finished Face %d", f);
        }
        // Save IrradianceCubemap to files
        for (size_t f = 0; f < 6; f++)
        {
            auto data = CubemapMipmap[f]->GetByteData();
            auto path = string_format("out/%s_%d.jpg", filenames_output[f], level);
            stbi_write_jpg(path.c_str(), width, height, 3, data.data(), 100);
        }

        GlobalLogger::Log(SeverityLevel::Info, "Finished Writting Level");
    }
    return 0;
}

Vector3f SampleSkyCubemap(const Vector3f& v)
{
    auto cube = XYZToCubeUV(v);
    return SkyCubemap[cube.Id]->Sample(cube.UV);
}

Vector3f GGXImportanceSample(const Vector2f& sample, float alpha)
{
    float a2 = square(alpha);
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
    float NdotH = std::max(0.f, glm::dot(N, H));
    return a2 / (glm::pi<float>() * square(square(NdotH) * (a2 - 1.0) + 1.0));
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

Vector3f SampleOneNormal(const Vector3f& N, const std::vector<Vector2f>& samples, float roughness)
{
    float alpha = roughness * roughness;
    // Calculate TBN vector
    Vector3f T;
    for (int i = 0; i < 3; i++)
    {
        Vector3f v(0);
        v[i] = 1;
        auto tmp = glm::cross(v, N);
        if (tmp != Vector3f(0.f))
        {
            T = glm::normalize(tmp);
            break;
        }
    }
    Vector3f B = glm::normalize(glm::cross(N, T));
    Vector3f sampledValue(0.f);
    float count = 0;
    for (auto& sample : samples)
    {
        Vector3f vector = GGXImportanceSample(sample, alpha);
        auto H = vector.x * T + vector.y * N + vector.z * B;

        auto L = glm::reflect(-N, H);

        float NoL = std::max(0.f, glm::dot(N, L));
        float VoH = std::max(0.f, glm::dot(N, H));
        float NoH = std::max(0.f, glm::dot(N, H));
        // Remember to exclude the samples that are invalid
        //if (NoL > 0)
        //{
        //    // Incident light = SampleColor * NoL
        //    // Microfacet specular = D*G*F / (4*NoL*NoV)
        //    // pdf = D * NoH / (4 * VoH)
        //    // Total = fr * NoL / pdf

        //    // Fr = DVF + x
        //    // pdf = D * NoH / (4 * VoH)
        //    // Total = (DVF + x) * HoV / (D * NoH / (4 * VoH))
        //    //       = (DVF*NoL + x*NoL) / (D * NoH / (4 * VoH))
        //    //       = 4VF * NoL * VoH / NoH  +  4 * VoH * x * NoL/ (D * NoH)

        //    // Here NoH = VoH

        //}
        auto v = V_SmithGGXCorrelated(N, N, L, alpha);
        sampledValue += v * SampleSkyCubemap(L) * NoL;
        count += v * NoL;

        //count++;
    }

    return sampledValue * (1.f / count);
}


