//#include "OrbitControllerTest.h"
//#include <Engine.h>
//
//#include <Core/Utils/Logger.h>
//#include <Core/Math/Geometry.h>
//#include <Core/Utils/Misc.h>
//#include <Core/Utils/GameTimer.h>
//
//#include "Resource/Asset/AssetManager.h"
//#include <Function/Framework/Components/Mesh/Mesh.h>
//#include "Function/Input/InputController.h"
//#include "Platform/RHI/Interfaces.h"
//
//#include <Function/Framework/Components/Transform/TransformComponent.h>
//
//
//namespace crystal
//{
//	OrbitControllerTest::OrbitControllerTest()
//	{
//
//	}
//	
//	
//	OrbitControllerTest::~OrbitControllerTest()
//	{
//		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "OrbitController destruct");
//	}
//
//	static int VNum;
//	void OrbitControllerTest::Initialize()
//	{
//		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "OrbitController initialize");
//
//		auto window = m_engine->GetWindow();
//		auto windowSize = window->GetWindowSize();
//        auto assetManager = m_engine->GetAssetManager();
//		m_pCamera = std::make_shared<Camera>(1.0f, windowSize.x / windowSize.y, 0.5f, 100.f);
//
//        InitSkybox();
//
//        auto model = assetManager->LoadAssetBuiltIn<Mesh>("Sphere");
//
//		std::vector<ElementDescription> elements = {
//			{ SemanticType::POSITION, 0, RenderFormat::RGB32f, 0 },
//			{ SemanticType::TEXCOORD, 0, RenderFormat::RG32f, 12 },
//			{ SemanticType::NORMAL, 0, RenderFormat::RGB32f, 20 },
//		};
//
//
//		VertexLayout vLayout(elements, sizeof(MeshVertexData));
//		auto graphicsDevice = m_engine->GetGraphicsDevice();
//
//		VertexBufferDescription bufferDesc{};
//		bufferDesc.Usage = BufferUsage::Immutable;
//		IndexBufferDescription ibufferDesc{};
//		ibufferDesc.Usage = BufferUsage::Immutable;
//		ibufferDesc.Format = DataFormat::UInt32;
//
//		m_PSO = graphicsDevice->CreatePipelineStateObject();
//		m_PRO = graphicsDevice->CreatePipelineResourceObject();
//
//        auto& meshData = model->GetData();
//		auto vertexBuffer = graphicsDevice->CreateVertexBuffer(bufferDesc, 
//            (void*)meshData.data(), sizeof(MeshVertexData) * meshData.size());
//
//        VNum = meshData.size();
//
//        m_pShader = assetManager->LoadAssetBuiltIn<IShaderProgram>("PBR/Microfacet");
//        m_texture2D = assetManager->LoadAsset<ITexture2D>("engine:White");
//
//		vertexBuffer->BindVertexLayout(vLayout);
//		m_PRO->SetVertexBuffer(vertexBuffer);
//		m_PRO->SetShaderProgram(m_pShader);
//		m_PRO->SetShaderResource(m_texture2D, 0);
//        m_PRO->SetShaderResource(m_EmuTexture, 1);
//        m_PRO->SetShaderResource(m_EavgTexture, 2);
//        m_PRO->SetShaderResource(m_skyBoxIrradiance, 3);
//        m_PRO->SetShaderResource(m_skyBoxPrefilter, 4);
//        m_PRO->SetShaderResource(m_skyBoxLUT, 5);
//        
//		m_PRO->SetSamplerState(graphicsDevice->GetCommonSamplerState(SamplerStates::LinearClamp), 0);
//        m_PRO->SetSamplerState(graphicsDevice->GetCommonSamplerState(SamplerStates::LinearClamp), 1);
//        m_PRO->SetSamplerState(graphicsDevice->GetCommonSamplerState(SamplerStates::LinearClamp), 2);
//        m_PRO->SetSamplerState(graphicsDevice->GetCommonSamplerState(SamplerStates::LinearClamp), 3);
//        m_PRO->SetSamplerState(graphicsDevice->GetCommonSamplerState(SamplerStates::LinearClamp), 4);
//        m_PRO->SetSamplerState(graphicsDevice->GetCommonSamplerState(SamplerStates::LinearClamp), 5);
//
//		m_PSO->SetBlendState(graphicsDevice->CreateBlendStateFromTemplate(BlendStates::Opaque));
//		m_PSO->SetDepthStencilState(graphicsDevice->CreateDepthStencilStateFromTemplate(DepthStencilStates::DefaultDepthTest));
//		m_PSO->SetRasterState(graphicsDevice->CreateRasterStateFromTemplate(RasterStates::CullCCW));
//		//indexBuffer->Bind(0);
//	}
//
//
//	static Vector2f orbitControl = Vector2f(-glm::half_pi<float>(), glm::half_pi<float>());
//	static Point2i oldMousePos;
//    static float distanceFactor = 0.f;
//	void OrbitControllerTest::Update(const crystal::GameTimer& gameTimer)
//	{
//		auto window = m_engine->GetWindow();
//		auto windowSize = window->GetWindowSize();
//
//		auto inputController = m_engine->GetInstance()->GetInputController();
//
//		auto curPos = window->GetMousePos();
//		if (inputController->IsMouseJustPressed(crystal::MouseButtonCode::LEFT_BUTTON))
//		{
//			oldMousePos = curPos;
//		}
//		if (inputController->IsMouseDowned(crystal::MouseButtonCode::LEFT_BUTTON))
//		{
//			auto delta = curPos - oldMousePos;
//			orbitControl += Vector2f(delta) * 0.01f;
//			oldMousePos = curPos;
//		}
//		if (orbitControl.y < 0.01f)
//		{
//			orbitControl.y = 0.01f;
//		}
//		if (orbitControl.y > glm::pi<float>() - 0.01f)
//		{
//			orbitControl.y = glm::pi<float>() - 0.01f;
//		}
//		if (inputController->IsKeyDowned(crystal::KeyCode::CRYSTAL_A_KEY))
//		{
//			orbitControl.x += 0.01f;
//			//m_renderPause = !m_renderPause;
//			//printf("%s\n", m_renderPause ? "Paused" : "Resumed");
//		}
//		if (inputController->IsKeyDowned(crystal::KeyCode::CRYSTAL_D_KEY))
//		{
//			orbitControl.x -= 0.01f;
//			//printf("Time: %lf\n", gameTimer.GetLogicTime());
//			//printf("Time: %lf\n", gameTimer.GetLogicalDeltaTime());
//		}
//
//        distanceFactor -= inputController->GetScrollValue().y * 0.04f;
//        distanceFactor = glm::clamp(distanceFactor, -1.f, 1.f);
//        
//
//		auto sinTheta = std::sin(orbitControl.y);
//		auto cosTheta = std::sqrt(1.f - sinTheta * sinTheta);
//		if (orbitControl.y > glm::half_pi<float>())
//		{
//			cosTheta = -cosTheta;
//		}
//		Point3f eyePos(sinTheta * std::cos(orbitControl.x), cosTheta, -sinTheta * std::sin(orbitControl.x));
//		m_pCamera->SetEyePos(eyePos * 5.f * std::exp(distanceFactor * 2.5f));
//		m_pCamera->SetLookAt(Point3f(0.f));
//		m_pCamera->SetAspectRatio(static_cast<crystal::Float>(windowSize.x) / windowSize.y);
//	}
//
//    void OrbitControllerTest::Draw(const crystal::GameTimer& gameTimer)
//    {
//        auto windowSize = m_engine->GetWindow()->GetWindowSize();
//        auto graphicsContext = m_engine->GetGraphicsContext();
//        graphicsContext->Clear(
//            crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
//            | crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
//            | crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
//            crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);
//        //m_pShader->SetUniform1f("M", 0.5f + 0.5f * std::sin(gameTimer.GetLogicTime()));
//        //m_pShader->SetUniform1f("uBase", 1.0f);
//
//        auto P = m_pCamera->GetProjectionMatrix();
//        auto V = m_pCamera->GetViewMatrix();
//
//
//        m_pShader->SetUniformMat3f("MN", glm::identity<Matrix3f>());
//        m_pShader->SetUniformMat4f("VP", P * V);
//        m_pShader->SetUniformVec3f("uCameraPos", m_pCamera->GetEyePos());
//        m_pShader->SetUniformVec3f("uLightPos", Vector3f(-500.f, 500.f, -1000.f));
//        m_pShader->SetUniformVec3f("uLightIntensity", Vector3f(1.f));
//        m_pShader->SetUniformVec3f("uTint", Vector3f(1.f, 1.f, 1.f));
//
//        //Vector3f vx(81 / 255.0f, 35 / 255.0f, 67 / 255.0f);
//        //auto ss = glm::pow(vx, Vector3f(2.2f));
//        //auto res = ss / (1.0f - ss);
//        //printf("%lf %lf %lf\n", res.x, res.y, res.z);
//
//        auto identity = glm::identity<Matrix4f>();
//        graphicsContext->LoadPipelineState(m_PSO);
//
//
//        for (int i = 0; i < 7; i++)
//        {
//            for (int j = 0; j < 7; j++)
//            {
//                m_pShader->SetUniformMat4f("M", glm::translate(identity, Vector3f(i - 3, j - 3, 0) * 3.f));
//                m_pShader->SetUniform1f("uRoughness", i / 6.f);
//                m_pShader->SetUniform1f("uMetallic", j / 6.f);
//
//                graphicsContext->LoadPipelineResources(m_PRO);
//                {
//                    graphicsContext->DrawPrimitives(PrimitiveType::TRIANGLE_LIST, 0, VNum);
//                }
//                graphicsContext->UnloadPipelineResources();
//            }
//        }
//        RenderSkybox();
//    }
//
//	void OrbitControllerTest::Exit()
//	{
//		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "OrbitController exit");
//	}
//
//	bool OrbitControllerTest::Paused()
//	{
//		return m_renderPause;
//	}
//
//    static int CubeMapVertices = 0;
//    void OrbitControllerTest::RenderSkybox()
//    {
//        auto graphicsContext = m_engine->GetGraphicsContext();
//
//        auto P = m_pCamera->GetProjectionMatrix();
//        // Remove translation
//        auto V = Matrix4f(Matrix3f(m_pCamera->GetViewMatrix()));
//        m_pSkyboxShader->SetUniformMat4f("ViewRemoveTranslation", V);
//        m_pSkyboxShader->SetUniformMat4f("Proj", P);
//
//        graphicsContext->LoadPipelineState(m_pSkyboxPSO);
//        graphicsContext->LoadPipelineResources(m_pSkyboxPRO);
//        {
//            graphicsContext->DrawPrimitives(PrimitiveType::TRIANGLE_LIST, 0, CubeMapVertices);
//        }
//        graphicsContext->UnloadPipelineResources();
//    }
//
//    void OrbitControllerTest::InitSkybox()
//    {
//        auto assetManager = m_engine->GetAssetManager();
//        m_pSkyboxShader = assetManager->LoadAssetBuiltIn<IShaderProgram>("Skybox");
//        m_skyBox = assetManager->LoadAssetBuiltIn<ITextureCubemap>("Cubemaps/Sky2/Skybox");
//        m_skyBoxIrradiance = assetManager->LoadAssetBuiltIn<ITextureCubemap>("Cubemaps/Sky2/Skybox_Irradiance");
//        m_skyBoxPrefilter = assetManager->LoadAssetBuiltIn<ITextureCubemap>("Cubemaps/Sky2/Skybox_Prefilter");
//        m_skyBoxLUT = assetManager->LoadAssetBuiltIn<ITexture2D>("LUT");
//        m_EavgTexture = assetManager->LoadAssetBuiltIn<ITexture2D>("Eavg");
//        m_EmuTexture = assetManager->LoadAssetBuiltIn<ITexture2D>("Emu");
//
//        std::vector<ElementDescription> elements = {
//            { SemanticType::POSITION, 0, RenderFormat::RGB32f, 0 },
//        };
//
//        VertexLayout vLayout(elements, sizeof(Vector3f));
//
//        auto graphicsDevice = m_engine->GetGraphicsDevice();
//        auto model = assetManager->LoadAssetBuiltIn<Mesh>("Cube");
//
//        std::vector<Vector3f> vertices;
//        for (const auto& data : model->GetData())
//        {
//            vertices.push_back(data.Position);
//        }
//
//        CubeMapVertices = vertices.size();
//
//        VertexBufferDescription bufferDesc{};
//        bufferDesc.Usage = BufferUsage::Immutable;
//
//        auto vertexBuffer = graphicsDevice->CreateVertexBuffer(bufferDesc,
//            vertices.data(), sizeof(Vector3f) * vertices.size());
//
//        vertexBuffer->BindVertexLayout(vLayout);
//
//        m_pSkyboxPRO = graphicsDevice->CreatePipelineResourceObject();
//        m_pSkyboxPRO->SetVertexBuffer(vertexBuffer);
//        m_pSkyboxPRO->SetShaderProgram(m_pSkyboxShader);
//        m_pSkyboxPRO->SetShaderResource(m_skyBox, 0);
//        m_pSkyboxPRO->SetSamplerState(graphicsDevice->GetCommonSamplerState(SamplerStates::LinearClamp), 0);
//
//        m_pSkyboxPSO = graphicsDevice->CreatePipelineStateObject();
//        m_pSkyboxPSO->SetBlendState(graphicsDevice->CreateBlendStateFromTemplate(BlendStates::Opaque));
//        m_pSkyboxPSO->SetDepthStencilState(graphicsDevice->CreateDepthStencilStateFromTemplate(DepthStencilStates::DefaultDepthTest));
//        m_pSkyboxPSO->SetRasterState(graphicsDevice->CreateRasterStateFromTemplate(RasterStates::CullNone));
//    }
//}