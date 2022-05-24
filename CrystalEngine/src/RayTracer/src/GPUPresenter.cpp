#include "GPUPresenter.h"
#include <Platform/RHI/FileSystem/File.h>
#include <Function/Framework/Object/GameObject.h>
#include <Function/Framework/Components/Light/Lights/SphereEnvironmentLight.h>

namespace tracer
{
    GPUPresenter::GPUPresenter()
    {}

    GPUPresenter::~GPUPresenter()
    {}

    static int SceneStartIndex = 0;
    static int GeometryStartIndex = 0;
    static int GeometryCount = 0;

    static void AddTriangle(Float* sceneData, Vector4f* geometryData,
        const Vector3f& A, const Vector3f& B, const Vector3f& C)
    {
        sceneData[SceneStartIndex++] = 2;
        sceneData[SceneStartIndex++] = GeometryStartIndex;
        geometryData[GeometryStartIndex++] = Vector4f(A, 0);
        geometryData[GeometryStartIndex++] = Vector4f(B, 0);
        geometryData[GeometryStartIndex++] = Vector4f(C, 0);
        GeometryCount++;
    }

    void GPUPresenter::Initialize()
    {
        auto graphicsDevice = m_engine->GetGraphicsDevice();
        auto window = m_engine->GetWindow();
        auto assetManager = m_engine->GetAssetManager();

        RenderTarget2DDescription renderTargetDesc;
        renderTargetDesc.TargetFormat = RenderFormat::RGBA8ub;
        renderTargetDesc.RTFlags = RenderTargetFlags::CRYSTAL_TEXTURE_TARGET | RenderTargetFlags::CRYSTAL_DEPTH_TARGET;
        renderTargetDesc.Size = window->GetWindowSize();
        renderTargetDesc.MipmapLevels = 1;

        m_screenFBuffer[0] = graphicsDevice->CreateRenderTarget2D(renderTargetDesc);
        m_screenFBuffer[1] = graphicsDevice->CreateRenderTarget2D(renderTargetDesc);


        constexpr int DATA_SIZE_SIDE = 256;
        RNG mt;


        auto scene = Scene::LoadScene(File::ReadAllText("assets/engine/scene/Knob.json"));
        scene->Initialize();

        RenderProperties renderprops = {};
        renderprops.FrameBufferSize = window->GetWindowSize();
        renderprops.Gamma = 2.2;
        renderprops.SampleCount = 64;
        renderprops.NumOfThreads = 8;

        Transform transform;
        renderprops.EnvironmentLight = std::make_shared<SphereEnvironmentLight>(transform,
            std::make_shared<CPUTexture2DPure>(Spectrum(std::pow(0.8f, 2.2f))));

        //auto t = assetManager->LoadAsset<CPUTexture2D>("engine:Environment/DiningRoom");
        //renderprops.EnvironmentLight = std::make_shared<SphereEnvironmentLight>(transform,
        //    t);

        auto m_tracer = std::make_shared<RayScene>(scene, renderprops.EnvironmentLight);

        auto geometryData = std::make_unique<Vector4f[]>(DATA_SIZE_SIDE * DATA_SIZE_SIDE);
        auto sceneData = std::make_unique<Float[]>(DATA_SIZE_SIDE * DATA_SIZE_SIDE);

        // Object group
        sceneData[0] = 1.0f;

        SceneStartIndex = 2;
        GeometryStartIndex = 0;

        AddTriangle(sceneData.get(), geometryData.get(),
            Vector3f(-5, -5, -5),
            Vector3f(5, -5, -5),
            Vector3f(5, -5, 5));
        AddTriangle(sceneData.get(), geometryData.get(),
            Vector3f(5, -5, 5),
            Vector3f(-5, -5, 5),
            Vector3f(-5, -5, -5));

        AddTriangle(sceneData.get(), geometryData.get(),
            Vector3f(-5, 5, -5),
            Vector3f(5, 5, -5),
            Vector3f(5, 5, 5));
        AddTriangle(sceneData.get(), geometryData.get(),
            Vector3f(5, 5, 5),
            Vector3f(-5, 5, 5),
            Vector3f(-5, 5, -5));


        // Object count
        sceneData[1] = GeometryCount;


        Texture2DDescription sceneDesc = {};
        sceneDesc.Format = RenderFormat::R32f;
        sceneDesc.MipmapLevels = 1;
        sceneDesc.Size = Vector2i(DATA_SIZE_SIDE, DATA_SIZE_SIDE);
        sceneDesc.Usage = BufferUsage::Default;
        m_sceneBuffer = graphicsDevice->CreateTexture2DFromMemory(sceneData.get(),
            sizeof(Float) * DATA_SIZE_SIDE * DATA_SIZE_SIDE, sceneDesc);

        Texture2DDescription sceneDataDesc = {};
        sceneDataDesc.Format = RenderFormat::RGBA32f;
        sceneDataDesc.MipmapLevels = 1;
        sceneDataDesc.Size = Vector2i(DATA_SIZE_SIDE, DATA_SIZE_SIDE);
        sceneDataDesc.Usage = BufferUsage::Default;
        m_geometryDataBuffer = graphicsDevice->CreateTexture2DFromMemory(geometryData.get(),
            sizeof(Vector4f) * DATA_SIZE_SIDE * DATA_SIZE_SIDE, sceneDataDesc);

        m_PSO = graphicsDevice->CreatePipelineStateObject();

        m_PRO = graphicsDevice->CreatePipelineResourceObject();
        m_PROScreen = graphicsDevice->CreatePipelineResourceObject();

        m_PSO->SetRasterState(graphicsDevice->CreateRasterStateFromTemplate(RasterStates::CullNone));
        m_PSO->SetBlendState(graphicsDevice->CreateBlendStateFromTemplate(BlendStates::Opaque));
        m_PSO->SetDepthStencilState(graphicsDevice->CreateDepthStencilStateFromTemplate(DepthStencilStates::NoDepthTest));


        std::vector<ElementDescription> elements = {
            { SemanticType::POSITION, 0, RenderFormat::RGB32f, 0 }
        };
        VertexLayout vLayout(elements, sizeof(Vector3f));
        VertexBufferDescription bufferDesc{};
        bufferDesc.Usage = BufferUsage::Immutable;

        Vector3f vertices[6] = {
            Vector3f(0, 0, 0),
            Vector3f(1, 0, 0),
            Vector3f(1, 1, 0),
            Vector3f(0, 0, 0),
            Vector3f(1, 1, 0),
            Vector3f(0, 1, 0),
        };
        auto vertexBuffer = graphicsDevice->CreateVertexBuffer(bufferDesc,
            vertices, sizeof(vertices));
        vertexBuffer->BindVertexLayout(vLayout);

        m_pShader = assetManager->LoadAsset<IShaderProgram>("raytracer:PathTracer");
        m_pToneMapping = assetManager->LoadAsset<IShaderProgram>("raytracer:ToneMapping");

        m_PRO->SetVertexBuffer(vertexBuffer);
        m_PRO->SetShaderProgram(m_pShader);
        m_PRO->SetSamplerState(graphicsDevice->GetCommonSamplerState(SamplerStates::PointClamp), 0);
        m_PRO->SetShaderResource(m_sceneBuffer, 1);
        m_PRO->SetShaderResource(m_geometryDataBuffer, 2);

        m_PROScreen->SetVertexBuffer(vertexBuffer);
        m_PROScreen->SetShaderProgram(m_pToneMapping);
        m_PROScreen->SetSamplerState(graphicsDevice->GetCommonSamplerState(SamplerStates::PointClamp), 0);
    }

    void GPUPresenter::Update(const crystal::GameTimer & gameTimer)
    {
        m_iterations++;
    }

    void GPUPresenter::Draw(const crystal::GameTimer & gameTimer)
    {
        int current = m_iterations & 1;
        int prev = !current;
        m_PRO->SetShaderResource(m_screenFBuffer[prev], 0);

        auto windowSize = m_engine->GetWindow()->GetWindowSize();
        auto graphicsContext = m_engine->GetGraphicsContext();
        auto spriteBatch = m_engine->GetSpriteBatch();

        graphicsContext->PushRenderTarget2D(m_screenFBuffer[current]);
        {
            graphicsContext->Clear(
                crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
                | crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
                | crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
                crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);
            //m_pShader->SetUniform1f("M", 0.5f + 0.5f * std::sin(gameTimer.GetLogicTime()));
            //m_pShader->SetUniform1f("uBase", 1.0f);
            m_pShader->SetUniformMat4f("MVP", glm::ortho(0.f, 1.f, 0.f, 1.f));
            m_pShader->SetUniform1f("AspectRatio", (float)windowSize.x / windowSize.y);
            m_pShader->SetUniform1f("FOV", glm::pi<float>() / 3.f);

            m_pShader->SetUniformVec3f("Up", Vector3f(0, 1, 0));
            m_pShader->SetUniformVec3f("Right", Vector3f(1, 0, 0));
            m_pShader->SetUniformVec3f("Front", Vector3f(0, 0, -1));
            m_pShader->SetUniformVec3f("EyePos", Vector3f(0, 0, 15));
            m_pShader->SetUniform1f("uIteration", m_iterations);
            m_pShader->SetUniform1f("uFrameWidth", windowSize.x);
            m_pShader->SetUniform1f("uFrameHeight", windowSize.y);

            //graphicsDevice->SetPipelineStateObject(m_PSO);
            graphicsContext->LoadPipelineState(m_PSO);
            graphicsContext->LoadPipelineResources(m_PRO);
            {
                graphicsContext->DrawPrimitives(PrimitiveType::TRIANGLE_LIST, 0, 6);
            }
            graphicsContext->UnloadPipelineResources();
        }
        graphicsContext->PopRenderTarget2D();


        m_PROScreen->SetShaderResource(m_screenFBuffer[current], 0);
        m_pToneMapping->SetUniformMat4f("MVP", glm::ortho(0.f, 1.f, 0.f, 1.f));
        graphicsContext->Clear(
            crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
            | crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
            | crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
            crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);
        //graphicsDevice->SetPipelineStateObject(m_PSOScreen);
        graphicsContext->LoadPipelineState(m_PSO);
        graphicsContext->LoadPipelineResources(m_PROScreen);
        {
            graphicsContext->DrawPrimitives(PrimitiveType::TRIANGLE_LIST, 0, 6);
        }
        graphicsContext->UnloadPipelineResources();
    }

    void GPUPresenter::Exit()
    {}

    bool GPUPresenter::Paused()
    {
        return false;
    }
}
