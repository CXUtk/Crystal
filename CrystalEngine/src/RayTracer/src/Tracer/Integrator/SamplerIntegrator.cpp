#include "SamplerIntegrator.h"
#include <thread>
#include <mutex>

namespace tracer
{
    SamplerIntegrator::SamplerIntegrator(const std::shared_ptr<Sampler>& sampler, int threads)
        : Integrator(), _sampler(sampler), _numThreads(threads)
    {
        assert(threads <= 16);
        _threadPool = std::make_unique<FixedThreadPool>(threads);
    }

    void tracer::SamplerIntegrator::Render(const RayScene* rayScene,
        const CameraComponent* camera, FrameBuffer* frameBuffer)
    {
        Preprocess(rayScene);
        int w = frameBuffer->GetWidth(), h = frameBuffer->GetHeight();
        int SPP = _sampler->GetSamplesPerPixel();

        constexpr int TILE_SIZE = 32;
        Point2i nTiles((w + TILE_SIZE - 1) / TILE_SIZE, (h + TILE_SIZE - 1) / TILE_SIZE);

        std::atomic<int> finishedCount = 0;

        for (int x = 0; x < nTiles.x; x++)
        {
            for (int y = 0; y < nTiles.y; y++)
            {
                auto task = [&, x, y]() {
                    int x0 = TILE_SIZE * x;
                    int x1 = std::min(x0 + TILE_SIZE, w);
                    int y0 = TILE_SIZE * y;
                    int y1 = std::min(y0 + TILE_SIZE, h);

                    auto sampler_thread = _sampler->Clone(x * nTiles.y + y);

                    for (int xx = x0; xx < x1; xx++)
                    {
                        for (int yy = y0; yy < y1; yy++)
                        {
                            sampler_thread->StartPixel(Point2i(xx, yy));
                            do
                            {
                                auto s = sampler_thread->Get2D();
                                glm::vec2 pos = glm::vec2(xx, yy) + s;
                                pos.x = pos.x / w;
                                pos.y = pos.y / h;

                                auto ray = camera->GenerateRay(pos);
                                auto color = Evaluate(ray, rayScene, ptr(sampler_thread));

                                frameBuffer->AddSample(xx, yy, color);
                                //printf("%lf, %lf\n", s.x, s.y);
                            } while (sampler_thread->StartNextSample());

                            //printf("================\n");
                        }
                    }

                    finishedCount++;
                };
                _threadPool->AddTask(crystal::FixedThreadPool::Task{ task });
            }
        }
        // Wait until finish a frame
        while (finishedCount < nTiles.x * nTiles.y) {}
    }
}