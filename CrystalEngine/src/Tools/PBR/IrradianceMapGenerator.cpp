#include "IrradianceMapGenerator.h"
#include <Core/Utils/Logger.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stbi/stb_image.h>
#include <stbi/stb_image_write.h>

using namespace crystal;

Vector3f SampleSkyCubemap(const Vector3f& v);

/**
 * @brief Generate N x N samples
 * @param N 
 * @return 
*/
static std::vector<Vector2f> GenerateSamples(size_t N);
static Vector3f SampleOneNormalMIS(const Vector3f& N, const std::vector<Vector2f>& samples,
    const std::vector<Vector2f>& samples2,
    const std::vector<Vector2f>& samples3);
static CubeUV WeightedSampleOnePixel(const Vector3f& sample);

static RNG rng;
static std::shared_ptr<RawTexture2D> SkyCubemap[6];
static std::shared_ptr<RawTexture2D> IrradianceCubemap[6];
static float CubemapSamplerSequence[7] = {};

static Vector3f cubemap_normals[6] = {
    Vector3f(-1, 0, 0),
    Vector3f(1, 0, 0),
    Vector3f(0, -1, 0),
    Vector3f(0, 1, 0),
    Vector3f(0, 0, -1),
    Vector3f(0, 0, 1)
};

static const char* filenames_hdr[6] = {
    "posx.hdr",
    "negx.hdr",
    "posy.hdr",
    "negy.hdr",
    "posz.hdr",
    "negz.hdr",
};

static const char* filenames_output_hdr[6] = {
    "posx_irradiance.hdr",
    "negx_irradiance.hdr",
    "posy_irradiance.hdr",
    "negy_irradiance.hdr",
    "posz_irradiance.hdr",
    "negz_irradiance.hdr",
};
static const char* filenames[6] = {
    "posx.jpg",
    "negx.jpg",
    "posy.jpg",
    "negy.jpg",
    "posz.jpg",
    "negz.jpg",
};
static const char* filenames_output[6] = {
    "posx_irradiance.jpg",
    "negx_irradiance.jpg",
    "posy_irradiance.jpg",
    "negy_irradiance.jpg",
    "posz_irradiance.jpg",
    "negz_irradiance.jpg",
};

static constexpr size_t CUBEMAP_SIZE = 32;

int main(int argc, char** argv)
{
    // Load textures
    for (int i = 0; i < 6; i++)
    {
        int width, height, channels;
        // stbi_uc* data = stbi_load(filenames[i], &width, &height, &channels, 3);
        float* data = stbi_loadf(filenames_hdr[i], &width, &height, &channels, 3);
        if (!data)
        {
            throw std::logic_error("Cannot find cubemap images");
        }
        SkyCubemap[i] = std::make_shared<RawTexture2D>(width, height, data);
        // SkyCubemap[i]->MipmapCompress(4);
        CubemapSamplerSequence[i + 1] = CubemapSamplerSequence[i] + SkyCubemap[i]->TotalWeight;
        stbi_image_free(data);
    }
    GlobalLogger::Log(SeverityLevel::Info, "Loaded Images");

    auto samples = GenerateSamples(32);
    auto samples2 = GenerateSamples(32);
    auto samples3 = GenerateSamples(32);

    for (size_t f = 0; f < 6; f++)
    {
        IrradianceCubemap[f] = std::make_shared<RawTexture2D>(CUBEMAP_SIZE, CUBEMAP_SIZE, false);
    }

    //for (int x = 0; x < 10240; x++)
    //{
    //    Vector3f sample(UniformRandomFloat(rng),
    //        UniformRandomFloat(rng),
    //        UniformRandomFloat(rng));

    //    auto uv = WeightedSampleOnePixel(sample);
    //    IrradianceCubemap[uv.Id]->SetPixel(uv.UV, IrradianceCubemap[uv.Id]->Sample(uv.UV) + Vector3f(0.05f));
    //}

    for (size_t f = 0; f < 6; f++)
    {
        // Sample on origin-centered unit cube
        float unit = 1.f / CUBEMAP_SIZE;
        for (size_t i = 0; i < CUBEMAP_SIZE; i++)
        {
            for (size_t j = 0; j < CUBEMAP_SIZE; j++)
            {
                // Sample the center of each pixel
                Vector2f pos = Vector2f(j + 0.5f, i + 0.5f) * unit;
                
                auto vector = CubeUVToVector(CubeUV{ (int)f, pos });
                //auto color = SampleSkyCubemap(vector);
                auto N = glm::normalize(vector);
                auto color = SampleOneNormalMIS(N, samples, samples2, samples3);
                IrradianceCubemap[f]->SetPixel(Vector2i(j, i), color);
            }
        }

        GlobalLogger::Log(SeverityLevel::Info, "Finished Face %d", f);
    }

    GlobalLogger::Log(SeverityLevel::Info, "Finished Sampling");

    // Save IrradianceCubemap to files
    for (size_t f = 0; f < 6; f++)
    {
        auto data = IrradianceCubemap[f]->GetFloatData();
        stbi_write_hdr(filenames_output_hdr[f], CUBEMAP_SIZE, CUBEMAP_SIZE, 3, data.data());
        // stbi_write_jpg(filenames_output[f], IrradianceCubemap[f]->Width, IrradianceCubemap[f]->Height, 3, data.data(), 100);
    }

    GlobalLogger::Log(SeverityLevel::Info, "Finished Writting");
    return 0;
}


Vector3f SampleSkyCubemap(const Vector3f& v)
{
    auto cube = XYZToCubeUV(v);
    return SkyCubemap[cube.Id]->Sample(cube.UV);
}

std::vector<Vector2f> GenerateSamples(size_t N)
{
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
    std::shuffle(samples.begin(), samples.end(), rng);
    return samples;
}



Vector3f SampleOneNormalMIS(const Vector3f& N, const std::vector<Vector2f>& samples,
    const std::vector<Vector2f>& samples2,
    const std::vector<Vector2f>& samples3)
{
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

    double totW = CubemapSamplerSequence[6];
    double IntF = totW / (6 * SkyCubemap[0]->Width * SkyCubemap[0]->Height);
    int totalSamples = samples.size();
    for (int i = 0; i < totalSamples; i++)
    {
        float pdfF;
        Vector3f vectorF = NextCosineUnitHemiSphere(samples[i], &pdfF);
        auto FV = vectorF.x * T + vectorF.y * N + vectorF.z * B;
        auto Li_F = SampleSkyCubemap(FV);
        Vector3f sampledF = Li_F * vectorF.y / glm::pi<float>() / pdfF;

        CubeUV uv = WeightedSampleOnePixel(Vector3f(samples3[i].x, samples2[i]));
        auto vectorG = CubeUVToVector(uv);
        auto L = glm::normalize(vectorG);
        auto Li_G = SampleSkyCubemap(vectorG);

        float wG = (Li_G.x * 0.3 + Li_G.y * 0.6 + Li_G.z * 0.1);
        float NdotL = std::max(0.f, glm::dot(N, L));
        float LNdotT = std::max(0.f, glm::dot(cubemap_normals[uv.Id], -L));
        float sinTheta = std::sqrt(1.0 - NdotL * NdotL);
        float pdfG = wG / IntF / (2 * glm::pi<float>() * glm::pi<float>());

        Vector3f sampledG = Li_G * NdotL * LNdotT / glm::pi<float>() / pdfG;

        float weightF = PowerHeuristic(1, pdfF, 1, pdfG);
        float weightG = PowerHeuristic(1, pdfG, 1, pdfF);

        sampledValue += /*sampledF * weightF +*/ sampledG;
    }

    float invN = 1.f / samples.size();
    return sampledValue * invN;
}

CubeUV WeightedSampleOnePixel(const Vector3f& sample)
{
    int id = std::upper_bound(CubemapSamplerSequence, CubemapSamplerSequence + 7,
        CubemapSamplerSequence[6] * sample.x) - CubemapSamplerSequence - 1;
    return CubeUV{id, SkyCubemap[id]->WeightedSampleUV(Vector2f(sample.y, sample.z))};
}
