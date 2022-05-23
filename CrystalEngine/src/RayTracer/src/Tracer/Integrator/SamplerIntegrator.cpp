#include "SamplerIntegrator.h"
#include <thread>
#include <mutex>

#include <Engine.h>
#include <Function/Framework/Components/Light/Lights/AreaLight.h>

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

        block.Camera = camera;
        block.FrameBuffer = frameBuffer;
        block.RayScene = rayScene;

        constexpr int TILE_SIZE = 32;
        constexpr int MAX_SAMPLES_PER_TASK = 99999;


        m_nTiles = Point2i((block.TargetWidth + TILE_SIZE - 1) / TILE_SIZE,
            (block.TargetHeight + TILE_SIZE - 1) / TILE_SIZE);

        m_completedBlocks = 0;

        int centerBlockX = block.TargetWidth / 2 / TILE_SIZE;
        int centerBlockY = block.TargetHeight / 2 / TILE_SIZE;
        int totalSPP = m_sampler->GetSamplesPerPixel();

        m_totalBlocks = (totalSPP + MAX_SAMPLES_PER_TASK - 1) / MAX_SAMPLES_PER_TASK;
        m_totalBlocks = m_totalBlocks * m_nTiles.x * m_nTiles.y;

        m_visit.resize(m_nTiles.x * m_nTiles.y, false);


        int seed = 0;

        for (int sp = 0; sp < totalSPP; sp += MAX_SAMPLES_PER_TASK)
        {
            block.SampleNumBegin = sp;
            block.SampleNumEnd = std::min(totalSPP, sp + MAX_SAMPLES_PER_TASK);

            std::queue<TileNode> Q;
            Q.push({ centerBlockX, centerBlockY });

            m_visit.assign(m_nTiles.x * m_nTiles.y, false);

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
                block.Seed = seed++;

                PushNewBlock(block);

                for (int i = 0; i < 4; i++)
                {
                    int nx = node.x + dx[i];
                    int ny = node.y + dy[i];

                    if (nx < 0 || ny < 0 || nx >= m_nTiles.x || ny >= m_nTiles.y) continue;
                    Q.push({ nx, ny });
                }
            }
        }
        //// Wait until finish a frame
        //while (m_completedBlocks < m_nTiles.x * m_nTiles.y) {}
        m_startRenderingTime = Engine::GetInstance()->GetCurrentTime();
    }

    void SamplerIntegrator::PushNewBlock(IntegratorBlock block)
    {
        auto task = [this, block]() {
            auto sampler_thread = m_sampler->Clone(block.Seed);

            for (int xx = block.TileXBegin; xx < block.TileXEnd; xx++)
            {
                for (int yy = block.TileYBegin; yy < block.TileYEnd; yy++)
                {
                    sampler_thread->StartPixel(Point2i(xx, yy), block.SampleNumBegin, block.SampleNumEnd);
                    do
                    {
                        auto s = sampler_thread->Get2D();
                        glm::vec2 pos = glm::vec2(xx, yy) + s;
                        pos.x = pos.x / block.TargetWidth;
                        pos.y = pos.y / block.TargetHeight;

                        //auto ray = block.Camera->GenerateRay(pos);
                        RayTr ray = {};
                        ray.Ray = block.Camera->GenerateRay(pos);
                        ray.Medium = nullptr;
                        auto color = Evaluate(ray, block.RayScene, ptr(sampler_thread));

                        block.FrameBuffer->AddSample(yy, xx, color);
                        //printf("%lf, %lf\n", s.x, s.y);
                    } while (sampler_thread->StartNextSample());
                }
            }

            double currentTime = Engine::GetInstance()->GetCurrentTime();
            {
                std::lock_guard<std::mutex> lock(m_countLock);

                m_completedBlocks++;
                printf("Progress: %.2lf%%\n", (double)m_completedBlocks / m_totalBlocks * 100);

                if (m_completedBlocks == m_totalBlocks)
                {
                    printf("Finished! Time Elapsed = %lf Secs\n", currentTime - m_startRenderingTime);
                }
            }

        };
        m_threadPool->AddTask(crystal::FixedThreadPool::Task{ task });
    }

    Spectrum SamplerIntegrator::UniformSampleAllLights(const InteractionInfo& it, const RayScene* scene,
        Sampler* sampler, bool handleMedium)
    {
        Spectrum L(0.f);
        // One sample for each light
        Vector2f sampleLight = sampler->Get2D();
        Vector2f sampleBSDF = sampler->Get2D();
        scene->ForEachLights([&](const crystal::Light* light) {
            if (light->Flux() == Spectrum(0.f)) return;
            if (it.IsSurfaceInteraction())
            {
                const SurfaceInteraction& isec = dynamic_cast<const SurfaceInteraction&>(it);
                if (isec.GetHitPrimitive()->GetAreaLight() == light)
                {
                    return;
                }
            }
            L += EsimateDirect(it, scene, sampleLight, sampleBSDF, light, sampler, handleMedium);
        });
        return L;
    }

    Spectrum SamplerIntegrator::UniformSampleOneLight(const InteractionInfo& it,
        const RayScene* scene, Sampler* sampler, bool handleMedium)
    {
        Float u = sampler->Get1D();
        Vector2f sampleLight = sampler->Get2D();
        Vector2f sampleBSDF = sampler->Get2D();

        Float pdf, remapped;
        auto light = scene->SampleOneLight(u, &pdf, &remapped);
        if (it.IsSurfaceInteraction())
        {
            const SurfaceInteraction& isec = dynamic_cast<const SurfaceInteraction&>(it);
            if (isec.GetHitPrimitive()->GetAreaLight() == light)
            {
                return Spectrum(0.f);
            }
        }
        return EsimateDirect(it, scene, sampleLight, sampleBSDF, light, sampler, handleMedium) / pdf;
    }

    Spectrum SamplerIntegrator::EsimateDirect(const InteractionInfo& it, const RayScene* scene,
        const Vector2f& sampleLight, const Vector2f& sampleBSDF,
        const crystal::Light* light, Sampler* sampler, bool handleMedium)
    {
        Spectrum L(0.f);
        BxDFType bsdfSampleType = (BxDFType)(BxDFType::BxDF_ALL & ~BxDFType::BxDF_SPECULAR);
        Vector3f wOut = it.ToLocalCoordinate(it.GetW_Out());
        Point3f P = it.GetPosition();

        // Sample light source with MIS (Specular BSDF will not have value)
        {
            Point3f lightPos;
            float pdf_light;
            auto Li_light = light->Sample_Li(it.GetGeometryInfo(), sampleLight,
                &lightPos, &pdf_light);
            Vector3f wIn = it.ToLocalCoordinate(glm::normalize(lightPos - P));

            if (pdf_light != 0.f && !std::isinf(pdf_light) && Li_light != Spectrum(0.f))
            {
                Spectrum f;
                Float pdf_bsdf;
                if (it.IsSurfaceInteraction())
                {
                    const SurfaceInteraction& isec = dynamic_cast<const SurfaceInteraction&>(it);
                    auto bsdf = isec.GetBSDF();

                    // For surface interaction, we need to account surface radiance
                    f = bsdf->DistributionFunction(wOut, wIn) * std::abs(wIn.y);
                    pdf_bsdf = bsdf->Pdf(wOut, wIn, bsdfSampleType);
                }
                else
                {
                    const MediumInteractionInfo& mi = dynamic_cast<const MediumInteractionInfo&>(it);
                    auto phase = mi.GetPhaseFunction();
                    f = phase->Pdf(wOut, wIn);
                    pdf_bsdf = f.x;
                }

                if (f != Spectrum(0.f))
                {
                    if (handleMedium)
                    {
                        Li_light *= TraceLi(scene, it, lightPos);
                    }
                    else
                    {
                        if (scene->IntersectTest(it.SpawnRayTo(lightPos).Ray, 0, 1.f - EPS))
                        {
                            Li_light = Spectrum(0.f);
                        }
                    }

                    if (Li_light != Spectrum(0.f))
                    {
                        if (light->IsDeltaLight())
                        {
                            L += f * Li_light / pdf_light;
                        }
                        else
                        {
                            Float weight = PowerHeuristic(1, pdf_light, 1, pdf_bsdf);
                            L += f * Li_light * weight / pdf_light;
                        }
                    }
                }

                NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::Light");
                INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::Light");
            }
        }

        // Sample BSDF (Delta light should not have any value in BSDF sample)
        if (!light->IsDeltaLight())
        {
            float pdf_bsdf = 0.f;
            Vector3f wIn;
            Spectrum f;
            BxDFType sampledType{};
            bool sampledSpecular = false;
            if (it.IsSurfaceInteraction())
            {
                const SurfaceInteraction& isec = dynamic_cast<const SurfaceInteraction&>(it);
                auto bsdf = isec.GetBSDF();
                f = bsdf->SampleDirection(sampler->Get1D(), sampleBSDF, wOut, &wIn,
                    &pdf_bsdf, bsdfSampleType, &sampledType);
                f *= std::abs(wIn.y);
                sampledSpecular = sampledType & BxDFType::BxDF_SPECULAR;
            }
            else
            {
                const MediumInteractionInfo& mi = dynamic_cast<const MediumInteractionInfo&>(it);
                auto phase = mi.GetPhaseFunction();
                f = phase->Sample_p(wOut, &wIn, sampleBSDF);
                pdf_bsdf = f.x;
            }
            Vector3f wi = it.ToWorldCoordinate(wIn);
            if (f == Spectrum(0.f) || pdf_bsdf == 0.f)
            {
                return L;
            }

            float weight = 1.0f;
            if (!sampledSpecular)
            {
                Float pdf_light = light->Pdf_Li(it.GetGeometryInfo(), wi);
                if (pdf_light == 0.f || f == Spectrum(0.f))
                {
                    return L;
                }
                weight = PowerHeuristic(1, pdf_bsdf, 1, pdf_light);
            }

            Spectrum Li(0.f);
            RayTr lightTestRay = it.SpawnRay(wi);
            SurfaceInteraction lightIsec;

            if (handleMedium)
            {
                Spectrum Tr = Spectrum(1.0);
                if (light->GetFlags() & LightFlags::Area)
                {
                    if (scene->IntersectTr(lightTestRay, sampler, &lightIsec, &Tr)
                        && lightIsec.GetHitPrimitive()->GetAreaLight() == light)
                    {
                        Li = lightIsec.Le(-wi);
                    }
                    else
                    {
                        Li = light->Le(wi);
                    }
                }
                else if (light->GetFlags() & LightFlags::Infinite)
                {
                    if (!scene->IntersectTr(lightTestRay, sampler, &lightIsec, &Tr))
                    {
                        Li = light->Le(wi);
                    }
                }
                Li *= Tr;
            }
            else
            {
                if (light->GetFlags() & LightFlags::Area)
                {
                    if (scene->Intersect(lightTestRay.Ray, &lightIsec)
                        && lightIsec.GetHitPrimitive()->GetAreaLight() == light)
                    {
                        Li = lightIsec.Le(-wi);
                    }
                    else
                    {
                        Li = light->Le(wi);
                    }
                }
                else if (light->GetFlags() & LightFlags::Infinite)
                {
                    if (!scene->IntersectTest(lightTestRay.Ray, 0, std::numeric_limits<float>::infinity()))
                    {
                        Li = light->Le(wi);
                    }
                }
            }

            if (Li != Spectrum(0.f))
            {
                if (it.IsSurfaceInteraction())
                {
                    const SurfaceInteraction& isec = dynamic_cast<const SurfaceInteraction&>(it);
                    auto bsdf = isec.GetBSDF();
                    L += Li * bsdf->CalculateBSDFNoLDivideByPdf(wOut, wIn, sampledType) * weight;
                }
                else
                {
                    L += Li * f * weight / pdf_bsdf;
                }
            }
        }

        NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
        INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
        return L;
    }

    //Spectrum VolumePTIntegrator::EsimateDirectMedium(const MediumInteractionInfo& mi, const RayScene* scene,
    //    const Vector2f& sampleLight, const Vector2f& sampleBSDF,
    //    const crystal::Light* light, Sampler* sampler, bool handleMedium)
    //{
    //    Spectrum L(0.f);
    //    BxDFType bsdfSampleType = (BxDFType)(BxDFType::BxDF_ALL & ~BxDFType::BxDF_SPECULAR);

    //    Point3f P = mi.GetPosition();

    //    Vector3f wOut = mi.ToLocalCoordinate(mi.GetW_Out());

    //    auto phase = mi.GetPhaseFunction();

    //    // Sample light source with MIS (Specular BSDF will not have value)
    //    {
    //        Point3f lightPos;
    //        float pdf_light;
    //        auto Li_light = light->Sample_Li(mi.GetGeometryInfo(), sampleLight,
    //            &lightPos, &pdf_light);

    //        Vector3f wIn = mi.ToLocalCoordinate(glm::normalize(lightPos - P));
    //        float NdotL = std::max(0.f, wIn.y);
    //        if (pdf_light != 0.f && !std::isinf(pdf_light)
    //            && Li_light != Spectrum(0.f) && NdotL != 0.f)
    //        {
    //            Spectrum f = phase->Pdf(wOut, wIn);
    //            float pdf_bsdf = phase->Pdf(wOut, wIn).x;
    //            if (f != Spectrum(0.f))
    //            {
    //                if (handleMedium)
    //                {
    //                    Li_light *= TraceLiM(scene, mi, lightPos);
    //                }

    //                if (Li_light != Spectrum(0.f))
    //                {
    //                    if (light->IsDeltaLight())
    //                    {
    //                        L += f * Li_light * NdotL / pdf_light;
    //                    }
    //                    else
    //                    {
    //                        L += f * Li_light * NdotL * PowerHeuristic(1, pdf_light, 1, pdf_bsdf) / pdf_light;
    //                    }
    //                }
    //            }

    //            NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::Light");
    //            INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::Light");
    //        }
    //    }

    //    // Sample BSDF (Delta light should not have any value in BSDF sample)
    //    if (!light->IsDeltaLight())
    //    {
    //        float pdf_bsdf = 0.f;
    //        BxDFType sampledType;
    //        Vector3f wIn;
    //        Spectrum f = phase->Sample_p(wOut, &wIn, sampler->Get2D());
    //        float NdotL = std::max(0.f, wIn.y);

    //        Vector3f wi = mi.ToWorldCoordinate(wIn);

    //        if (f == Spectrum(0.f) || pdf_bsdf == 0.f || NdotL == 0.f)
    //        {
    //            return L;
    //        }
    //        bool specularBSDF = sampledType & BxDFType::BxDF_SPECULAR;

    //        float weight = 1.0f;
    //        if (!specularBSDF)
    //        {
    //            Float pdf_light = light->Pdf_Li(mi.GetGeometryInfo(), wi);
    //            if (pdf_light == 0.f || f == Spectrum(0.f))
    //            {
    //                return L;
    //            }
    //            weight = PowerHeuristic(1, pdf_bsdf, 1, pdf_light);
    //        }

    //        Spectrum Li(0.f);
    //        RayTr lightTestRay = mi.SpawnRay(wi);
    //        SurfaceInteraction lightIsec;

    //        if (handleMedium)
    //        {
    //            Spectrum Tr = Spectrum(1.0);
    //            if (light->GetFlags() & LightFlags::Area)
    //            {
    //                if (scene->IntersectTr(lightTestRay, sampler, &lightIsec, &Tr)
    //                    && lightIsec.GetHitPrimitive()->GetAreaLight() == light)
    //                {
    //                    Li = lightIsec.Le(-wi);
    //                }
    //                else
    //                {
    //                    Li = light->Le(wi);
    //                }
    //            }
    //            else if (light->GetFlags() & LightFlags::Infinite)
    //            {
    //                if (!scene->IntersectTr(lightTestRay, sampler, &lightIsec, &Tr))
    //                {
    //                    Li = light->Le(wi);
    //                }
    //            }
    //            Li *= Tr;
    //        }

    //        if (Li != Spectrum(0.f))
    //        {
    //            L += Li * f * weight / pdf_bsdf;
    //        }
    //    }

    //    NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
    //    INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
    //    return L;
    //}

    Spectrum SamplerIntegrator::TraceLi(const RayScene* scene, const InteractionInfo& it, const Point3f& lightPos)
    {
        Spectrum Tr(1.f);
        RayTr currentRay = it.SpawnRayTo(lightPos);
        while (true)
        {
            SurfaceInteraction isect;
            bool hitSurface = scene->Intersect(currentRay.Ray, &isect, 0.f, 1.f - EPS);

            // Hit a surface instead of a medium, then it is blocked
            if (hitSurface && isect.GetMaterial() != nullptr) return Spectrum(0.f);

            if (currentRay.Medium)
            {
                Tr *= currentRay.Medium->Tr(currentRay.Ray, isect.GetDistance(), nullptr);
            }

            if (!hitSurface) return Tr;

            currentRay = isect.SpawnRayTo(lightPos);
        }
        return Tr;
    }
}