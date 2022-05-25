#include "GPUPresenter.h"
#include <Platform/RHI/FileSystem/File.h>
#include <Function/Framework/Object/GameObject.h>
#include <Function/Framework/Components/Light/Lights/SphereEnvironmentLight.h>

#include <Function/Input/InputController.h>
#include <Function/Framework/Components/Shape/Shapes/Triangle.h>

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
        geometryData[GeometryStartIndex++] = Vector4f(Vector3f(1.f), 0);
        GeometryCount++;
    }

    static void AddTriangle(Float* sceneData, Vector4f* geometryData,
    const Vector3f& A, const Vector3f& B, const Vector3f& C, const Vector3f& abeldo)
    {
        sceneData[SceneStartIndex++] = 2;
        sceneData[SceneStartIndex++] = GeometryStartIndex;
        geometryData[GeometryStartIndex++] = Vector4f(A, 0);
        geometryData[GeometryStartIndex++] = Vector4f(B, 0);
        geometryData[GeometryStartIndex++] = Vector4f(C, 0);
        geometryData[GeometryStartIndex++] = Vector4f(abeldo, 0);
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

        RNG mt;

        auto scene = Scene::LoadScene(File::ReadAllText("assets/engine/scene/Bunny.json"));
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

        auto m_rayScene = std::make_shared<RayScene>(scene, renderprops.EnvironmentLight);
        m_GPUData = m_rayScene->GetGPUPackage();

        size_t DataSize_Side = 128;

        auto& objData = m_GPUData.GetObjectData();
        auto& sceneData = m_GPUData.GetSceneData();
        size_t totalData = std::max(objData.size(), sceneData.size());

        while (DataSize_Side * DataSize_Side < totalData)
        {
            DataSize_Side *= 2;
        }

        auto objectDataArray = std::make_unique<Vector4f[]>(DataSize_Side * DataSize_Side);
        auto sceneDataArray = std::make_unique<Float[]>(DataSize_Side * DataSize_Side);
        memcpy(objectDataArray.get(), objData.data(), sizeof(Vector4f) * objData.size());
        memcpy(sceneDataArray.get(), sceneData.data(), sizeof(Float) * sceneData.size());

        //// Object group
        //sceneData[0] = 1.0f;

        //SceneStartIndex = 2;
        //GeometryStartIndex = 0;

        //AddTriangle(sceneData.get(), geometryData.get(),
        //    Vector3f(-5, -3, -5),
        //    Vector3f(5, -3, -5),
        //    Vector3f(5, -3, 5));
        //AddTriangle(sceneData.get(), geometryData.get(),
        //    Vector3f(5, -3, 5),
        //    Vector3f(-5, -3, 5),
        //    Vector3f(-5, -3, -5));

        //AddTriangle(sceneData.get(), geometryData.get(),
        //    Vector3f(-5, 3, -5),
        //    Vector3f(5, 3, -5),
        //    Vector3f(5, 3, 5));
        //AddTriangle(sceneData.get(), geometryData.get(),
        //    Vector3f(5, 3, 5),
        //    Vector3f(-5, 3, 5),
        //    Vector3f(-5, 3, -5));

        //AddTriangle(sceneData.get(), geometryData.get(),
        //    Vector3f(-5, 3, -5),
        //    Vector3f(-5, -3, -5),
        //    Vector3f(-5, -3, 5), Vector3f(0.1, 1.0, 0.1));
        //AddTriangle(sceneData.get(), geometryData.get(),
        //    Vector3f(-5, -3, 5),
        //    Vector3f(-5, 3, 5),
        //    Vector3f(-5, 3, -5), Vector3f(0.1, 1.0, 0.1));

        //AddTriangle(sceneData.get(), geometryData.get(),
        //    Vector3f(5, 3, -5),
        //    Vector3f(5, -3, -5),
        //    Vector3f(5, -3, 5), Vector3f(1, 0.1, 0.1));
        //AddTriangle(sceneData.get(), geometryData.get(),
        //    Vector3f(5, -3, 5),
        //    Vector3f(5, 3, 5),
        //    Vector3f(5, 3, -5), Vector3f(1, 0.1, 0.1));

        //AddTriangle(sceneData.get(), geometryData.get(),
        //    Vector3f(-5, 3, -5),
        //    Vector3f(-5, -3, -5),
        //    Vector3f(5, -3, -5));
        //AddTriangle(sceneData.get(), geometryData.get(),
        //    Vector3f(5, -3, -5),
        //    Vector3f(5, 3, -5),
        //    Vector3f(-5, 3, -5));

        //for (auto& p : m_rayScene->GetPrimitives())
        //{
        //    if (dynamic_cast<const Triangle*>(p->GetShape()) != nullptr)
        //    {
        //        auto triangle = (const Triangle*)p->GetShape();
        //        auto V = triangle->GetVertices();
        //        AddTriangle(sceneData.get(), geometryData.get(),
        //        V[0]->Position,
        //        V[1]->Position,
        //        V[2]->Position);
        //    }
        //}


        //// Object count
        //sceneData[1] = GeometryCount;


        Texture2DDescription sceneDesc = {};
        sceneDesc.Format = RenderFormat::R32f;
        sceneDesc.MipmapLevels = 1;
        sceneDesc.Size = Vector2i(DataSize_Side, DataSize_Side);
        sceneDesc.Usage = BufferUsage::Default;
        m_sceneBuffer = graphicsDevice->CreateTexture2DFromMemory(sceneDataArray.get(),
            0, sceneDesc);

        Texture2DDescription sceneDataDesc = {};
        sceneDataDesc.Format = RenderFormat::RGBA32f;
        sceneDataDesc.MipmapLevels = 1;
        sceneDataDesc.Size = Vector2i(DataSize_Side, DataSize_Side);
        sceneDataDesc.Usage = BufferUsage::Default;
        m_geometryDataBuffer = graphicsDevice->CreateTexture2DFromMemory(objectDataArray.get(),
            0, sceneDataDesc);

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

        m_cameraTransform = std::make_shared<Transform>();
        m_cameraTransform->SetTranslation(Vector3f(0, 0, 15));
        m_cameraTransform->SetRotation(glm::quatLookAt(Vector3f(0, 0, -1), Vector3f(0, 1, 0)));
        PerspectiveCameraSetting setting = {};
        auto windowSize = m_engine->GetWindow()->GetWindowSize();
        setting.Aspect = (float)windowSize.x / windowSize.y;
        setting.FovY = glm::pi<float>() / 3.f;
        setting.ZNear = 0.f;
        setting.ZFar = 10000.f;
        m_pCamera = std::make_shared<Camera>(m_cameraTransform.get(), setting);
    }

    static Vector2f orbitControl = Vector2f(glm::half_pi<float>(), glm::half_pi<float>());
    static Point2i oldMousePos;
    static float distanceFactor = 0.f;
    static constexpr float BASE_DISTANECE = 15;
    void GPUPresenter::Update(const crystal::GameTimer& gameTimer)
    {

        auto window = m_engine->GetWindow();
        auto windowSize = window->GetWindowSize();

        auto inputController = m_engine->GetInstance()->GetInputController();

        auto curPos = window->GetMousePos();
        if (inputController->IsMouseJustPressed(crystal::MouseButtonCode::LEFT_BUTTON))
        {
            oldMousePos = curPos;
        }
        if (inputController->IsMouseDowned(crystal::MouseButtonCode::LEFT_BUTTON))
        {
            auto delta = curPos - oldMousePos;
            orbitControl += Vector2f(delta) * 0.01f;
            oldMousePos = curPos;
            m_iterations = 0;
        }
        if (orbitControl.y < 0.01f)
        {
            orbitControl.y = 0.01f;
        }
        if (orbitControl.y > glm::pi<float>() - 0.01f)
        {
            orbitControl.y = glm::pi<float>() - 0.01f;
        }
        if (inputController->IsKeyDowned(crystal::KeyCode::CRYSTAL_A_KEY))
        {
            orbitControl.x += 0.01f;
            //m_renderPause = !m_renderPause;
            //printf("%s\n", m_renderPause ? "Paused" : "Resumed");
            m_iterations = 0;
        }
        if (inputController->IsKeyDowned(crystal::KeyCode::CRYSTAL_D_KEY))
        {
            orbitControl.x -= 0.01f;
            //printf("Time: %lf\n", gameTimer.GetLogicTime());
            //printf("Time: %lf\n", gameTimer.GetLogicalDeltaTime());
            m_iterations = 0;
        }

        if (inputController->GetScrollValue() != glm::vec2(0.f))
        {
            m_iterations = 0;
        }

        distanceFactor -= inputController->GetScrollValue().y * 0.04f;
        distanceFactor = glm::clamp(distanceFactor, -1.f, 1.f);


        auto sinTheta = std::sin(orbitControl.y);
        auto cosTheta = std::sqrt(1.f - sinTheta * sinTheta);
        if (orbitControl.y > glm::half_pi<float>())
        {
            cosTheta = -cosTheta;
        }
        Point3f eyePos(sinTheta * std::cos(-orbitControl.x), cosTheta, -sinTheta * std::sin(-orbitControl.x));

        m_cameraTransform->SetTranslation(eyePos * BASE_DISTANECE * std::exp(distanceFactor * 2.5f));
        m_cameraTransform->SetRotation(glm::quatLookAt(glm::normalize(-eyePos), Vector3f(0, 1, 0)));

        m_iterations++;
    }

    void GPUPresenter::Draw(const crystal::GameTimer& gameTimer)
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
            m_pShader->SetUniform1f("AspectRatio", m_pCamera->GetAspectRatio());
            m_pShader->SetUniform1f("FOV", m_pCamera->GetFOV());

            m_pShader->SetUniformVec3f("Up", m_pCamera->GetUpDir());
            m_pShader->SetUniformVec3f("Right", m_pCamera->GetRightDir());
            m_pShader->SetUniformVec3f("Front", m_pCamera->GetForwardDir());
            m_pShader->SetUniformVec3f("EyePos", m_pCamera->GetEyePos());
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
