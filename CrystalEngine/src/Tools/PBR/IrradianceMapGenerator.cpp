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
static Vector3f SampleOneNormal(const Vector3f& N, const std::vector<Vector2f>& samples);

static std::shared_ptr<RawTexture2D> SkyCubemap[6];
static std::shared_ptr<RawTexture2D> IrradianceCubemap[6];
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
        stbi_uc* data = stbi_load(filenames[i], &width, &height, &channels, 3);
        if (!data)
        {
            throw std::logic_error("Cannot find cubemap images");
        }
        SkyCubemap[i] = std::make_shared<RawTexture2D>(width, height, data, true);
        stbi_image_free(data);
    }
    GlobalLogger::Log(SeverityLevel::Info, "Loaded Images");

    auto samples = GenerateSamples(32);

    for (size_t f = 0; f < 6; f++)
    {
        IrradianceCubemap[f] = std::make_shared<RawTexture2D>(CUBEMAP_SIZE, CUBEMAP_SIZE, true);

        // Sample on origin-centered unit cube
        float unit = 1.f / CUBEMAP_SIZE;
        for (size_t i = 0; i < CUBEMAP_SIZE; i++)
        {
            for (size_t j = 0; j < CUBEMAP_SIZE; j++)
            {
                // Sample the center of each pixel
                Vector2f pos = Vector2f(j + 0.5f, i + 0.5f) * unit;
                
                auto vector = CubeUVToVector(CubeUV{ (int)f, pos });
                auto color = SampleOneNormal(vector, samples);
                IrradianceCubemap[f]->SetPixel(Vector2i(j, i), color);
            }
        }

        GlobalLogger::Log(SeverityLevel::Info, "Finished Face %d", f);
    }

    GlobalLogger::Log(SeverityLevel::Info, "Finished Sampling");

    // Save IrradianceCubemap to files
    for (size_t f = 0; f < 6; f++)
    {
        auto data = IrradianceCubemap[f]->GetByteData();
        stbi_write_jpg(filenames_output[f], IrradianceCubemap[f]->Width, IrradianceCubemap[f]->Height, 3, data.data(), 100);
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

Vector3f SampleOneNormal(const Vector3f& N, const std::vector<Vector2f>& samples)
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
    for (auto& sample : samples)
    {
        float pdf;
        Vector3f vector = NextCosineUnitHemiSphere(sample, &pdf);
        auto V = vector.x * T + vector.y * N + vector.z * B;

        sampledValue += SampleSkyCubemap(V) * vector.y / glm::pi<float>() / pdf;
    }

    float invN = 1.f / samples.size();
    return sampledValue * invN;
}
