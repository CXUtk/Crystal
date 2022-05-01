#include "SamplerIntegrator.h"
#include <thread>
#include <mutex>

namespace tracer
{
    static int dx[4] = { 1, -1, 0, 0 };
    static int dy[4] = { 0, 0, 1, -1 };
    static constexpr float EPS = 1e-4;

    SamplerIntegrator::SamplerIntegrator(const std::shared_ptr<Sampler>& sampler, int threads)
        : Integrator(), m_sampler(sampler), m_numThreads(threads)
    {
        assert(m_numThreads <= 16);
        m_threadPool = std::make_unique<FixedThreadPool>(m_numThreads);
    }

    void tracer::SamplerIntegrator::Render(const RayScene* rayScene,
        const CameraComponent* camera, FrameBuffer* frameBuffer)
    {
        Preprocess(rayScene);

        IntegratorBlock block = {};
        block.TargetWidth = frameBuffer->GetWidth();
        block.TargetHeight = frameBuffer->GetHeight();
        block.SamplesPerPixel = m_sampler->GetSamplesPerPixel();

        block.Camera = camera;
        block.FrameBuffer = frameBuffer;
        block.RayScene = rayScene;

        constexpr int TILE_SIZE = 32;
        m_nTiles = Point2i((block.TargetWidth + TILE_SIZE - 1) / TILE_SIZE,
            (block.TargetHeight + TILE_SIZE - 1) / TILE_SIZE);
        m_completedBlocks = 0;

        int centerBlockX = block.TargetWidth / 2 / TILE_SIZE;
        int centerBlockY = block.TargetHeight / 2 / TILE_SIZE;

        std::queue<TileNode> Q;
        Q.push({ centerBlockX, centerBlockY });

        m_visit.resize(m_nTiles.x * m_nTiles.y, false);

        while (!Q.empty())
        {
            TileNode node = Q.front();
            Q.pop();

            if (m_visit[node.y * m_nTiles.x + node.x]) continue;
            m_visit[node.y * m_nTiles.x + node.x] = true;

            block.TileXBegin = TILE_SIZE * node.x;
            block.TileXEnd = std::min(block.TileXBegin + TILE_SIZE, block.TargetWidth);
            block.TileYBegin = TILE_SIZE * node.y;
            block.TileYEnd = std::min(block.TileYBegin + TILE_SIZE, block.TargetHeight);
            block.X = node.x;
            block.Y = node.y;

            PushNewBlock(block);

            for (int i = 0; i < 4; i++)
            {
                int nx = node.x + dx[i];
                int ny = node.y + dy[i];

                if (nx < 0 || ny < 0 || nx >= m_nTiles.x || ny >= m_nTiles.y) continue;
                Q.push({ nx, ny });
            }
        }
        //// Wait until finish a frame
        //while (m_completedBlocks < m_nTiles.x * m_nTiles.y) {}
    }
    void SamplerIntegrator::PushNewBlock(IntegratorBlock block)
    {
        auto task = [this, block]() {
            auto sampler_thread = m_sampler->Clone(block.X * m_nTiles.y + block.Y);

            for (int xx = block.TileXBegin; xx < block.TileXEnd; xx++)
            {
                for (int yy = block.TileYBegin; yy < block.TileYEnd; yy++)
                {
                    sampler_thread->StartPixel(Point2i(xx, yy));
                    do
                    {
                        auto s = sampler_thread->Get2D();
                        glm::vec2 pos = glm::vec2(xx, yy) + s;
                        pos.x = pos.x / block.TargetWidth;
                        pos.y = pos.y / block.TargetHeight;

                        auto ray = block.Camera->GenerateRay(pos);
                        auto color = Evaluate(ray, block.RayScene, ptr(sampler_thread));

                        block.FrameBuffer->AddSample(yy, xx, color);
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