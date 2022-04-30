#include "SamplerIntegrator.h"
#include <thread>
#include <mutex>

namespace tracer
{
    SamplerIntegrator::SamplerIntegrator(const std::shared_ptr<Sampler>& sampler, int threads)
        : Integrator(), m_sampler(sampler), m_numThreads(threads)
    {
        assert(threads <= 16);
        m_threadPool = std::make_unique<FixedThreadPool>(threads);
    }

    void tracer::SamplerIntegrator::Render(const RayScene* rayScene,
        const CameraComponent* camera, FrameBuffer* frameBuffer)
    {
        Preprocess(rayScene);
        int w = frameBuffer->GetWidth(), h = frameBuffer->GetHeight();
        int SPP = m_sampler->GetSamplesPerPixel();

        constexpr int TILE_SIZE = 32;
        m_nTiles = Point2i((w + TILE_SIZE - 1) / TILE_SIZE, (h + TILE_SIZE - 1) / TILE_SIZE);

        m_completedBlocks = 0;

        for (int x = 0; x < m_nTiles.x; x++)
        {
            for (int y = 0; y < m_nTiles.y; y++)
            {
                auto task = [&, x, y]() {
                    int x0 = TILE_SIZE * x;
                    int x1 = std::min(x0 + TILE_SIZE, w);
                    int y0 = TILE_SIZE * y;
                    int y1 = std::min(y0 + TILE_SIZE, h);

                    auto sampler_thread = m_sampler->Clone(x * m_nTiles.y + y);

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

                                frameBuffer->AddSample(yy, xx, color);
                                //printf("%lf, %lf\n", s.x, s.y);
                            } while (sampler_thread->StartNextSample());

                            //printf("================\n");
                        }
                    }

                    m_completedBlocks++;
                    printf("Progress: %.2lf%%\n", (double)m_completedBlocks / (m_nTiles.x * m_nTiles.y) * 100);

                };
                m_threadPool->AddTask(crystal::FixedThreadPool::Task{ task });
            }
        }
        // Wait until finish a frame
        while (m_completedBlocks < m_nTiles.x * m_nTiles.y) {}
    }
}