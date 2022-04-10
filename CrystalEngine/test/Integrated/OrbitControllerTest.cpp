#include "OrbitControllerTest.h"
#include <Engine.h>
#include "Platform/RHI/Interfaces.h"
#include <Core/Utils/Logger.h>
#include <Core/Utils/Geometry.h>
#include "Function/Input/InputController.h"
#include <Core/Utils/GameTimer.h>
#include "Resource/Asset/AssetManager.h"

#include <Core/Utils/Misc.h>
#include <Core/Utils/ObjLoader.h>

namespace crystal
{
	OrbitControllerTest::OrbitControllerTest()
	{

	}
	
	
	OrbitControllerTest::~OrbitControllerTest()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer destruct");
	}

	static int indices;
	void OrbitControllerTest::Initialize()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer initialize");

		auto window = m_engine->GetWindow();
		auto windowSize = window->GetWindowSize();
        auto assetManager = m_engine->GetAssetManager();
		m_pCamera = std::make_shared<Camera>(1.0f, windowSize.x / windowSize.y, 0.5f, 100.f);
		//Vertex vertices[] =
		//{
		//	{ Vector3f(0.0f, 0.5f, 0.5f), Vector4f(0.0f, 1.0f, 0.0f, 1.0f) },
		//	{ Vector3f(0.5f, -0.5f, 0.5f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f) },
		//	{ Vector3f(-0.5f, -0.5f, 0.5f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f) },
		//};
		//int indices[] = 
		//{
		//	0, 1, 2
		//};
		objloader::ObjLoader loader;
		loader.load("resources/cube.obj");
		std::vector<ElementDescription> elements = {
			{ SemanticType::POSITION, 0, RenderFormat::RGB32f, 0 },
			{ SemanticType::TEXCOORD, 0, RenderFormat::RG32f, 12 },
			{ SemanticType::NORMAL, 0, RenderFormat::RGB32f, 20 },
		};
		VertexLayout vLayout(elements, sizeof(objloader::VertexData));
		auto graphicsDevice = m_engine->GetGraphicsDevice();

		VertexBufferDescription bufferDesc{};
		bufferDesc.Usage = BufferUsage::Immutable;
		IndexBufferDescription ibufferDesc{};
		ibufferDesc.Usage = BufferUsage::Immutable;
		ibufferDesc.Format = DataFormat::UInt32;

		m_PSO = graphicsDevice->CreatePipelineStateObject();
		m_PRO = graphicsDevice->CreatePipelineResourceObject();
		auto vertexBuffer = graphicsDevice->CreateVertexBuffer(bufferDesc, 
			loader.Vertices.data(), sizeof(objloader::VertexData) * loader.Vertices.size());
		//auto indexBuffer = graphicsDevice->CreateIndexBuffer(ibufferDesc, 
		//	loader.Triangles.data(), sizeof(float) * 3 * loader.Triangles.size());
		indices = loader.Triangles.size() * 3;

        m_pShader = assetManager->LoadAsset<IShaderProgram>("package1:Model");

		Texture2DDescription texturedesc;
		texturedesc.Format = RenderFormat::RGBA8ub;
		texturedesc.MipmapLevels = 1;
		texturedesc.Usage = BufferUsage::Default;

		m_texture2D = graphicsDevice->CreateTexture2DFromFile("resources/dots.png", texturedesc);

		vertexBuffer->BindVertexLayout(vLayout);
		m_PRO->SetVertexBuffer(vertexBuffer);
		m_PRO->SetShaderProgram(m_pShader);
		m_PRO->SetShaderResource(m_texture2D, 0);
		m_PRO->SetSamplerState(graphicsDevice->GetCommonSamplerState(SamplerStates::PointClamp), 0);
		m_PSO->SetBlendState(graphicsDevice->CreateBlendStateFromTemplate(BlendStates::Opaque));
		m_PSO->SetDepthStencilState(graphicsDevice->CreateDepthStencilStateFromTemplate(DepthStencilStates::DefaultDepthTest));
		m_PSO->SetRasterState(graphicsDevice->CreateRasterStateFromTemplate(RasterStates::CullNone));
		//indexBuffer->Bind(0);
	}


	static Vector2f orbitControl = Vector2f(-glm::half_pi<float>(), glm::half_pi<float>());
	static Point2i oldMousePos;
	void OrbitControllerTest::Update(const crystal::GameTimer& gameTimer)
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
		}
		if (inputController->IsKeyDowned(crystal::KeyCode::CRYSTAL_D_KEY))
		{
			orbitControl.x -= 0.01f;
			//printf("Time: %lf\n", gameTimer.GetLogicTime());
			//printf("Time: %lf\n", gameTimer.GetLogicalDeltaTime());
		}

		auto sinTheta = std::sin(orbitControl.y);
		auto cosTheta = std::sqrt(1.f - sinTheta * sinTheta);
		if (orbitControl.y > glm::half_pi<float>())
		{
			cosTheta = -cosTheta;
		}
		Point3f eyePos(sinTheta * std::cos(orbitControl.x), cosTheta, -sinTheta * std::sin(orbitControl.x));
		m_pCamera->SetEyePos(eyePos * 5.f);
		m_pCamera->SetLookAt(Point3f(0.f));
		m_pCamera->SetAspectRatio(static_cast<crystal::Float>(windowSize.x) / windowSize.y);
	}

    void OrbitControllerTest::Draw(const crystal::GameTimer& gameTimer)
    {
        auto windowSize = m_engine->GetWindow()->GetWindowSize();
        auto graphicsContext = m_engine->GetGraphicsContext();
        graphicsContext->Clear(
            crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
            | crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
            | crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
            crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);
        //m_pShader->SetUniform1f("M", 0.5f + 0.5f * std::sin(gameTimer.GetLogicTime()));
        //m_pShader->SetUniform1f("uBase", 1.0f);
        m_pShader->SetUniformMat4f("M", glm::identity<Matrix4f>());
        m_pShader->SetUniformMat4f("MN", glm::identity<Matrix4f>());
        auto P = m_pCamera->GetProjectionMatrix();
        auto V = m_pCamera->GetViewMatrix();
        m_pShader->SetUniformMat4f("VP", P * V);

        graphicsContext->LoadPipelineState(m_PSO);
        graphicsContext->LoadPipelineResources(m_PRO);
        {
            graphicsContext->DrawPrimitives(PrimitiveType::TRIANGLE_LIST, 0, indices);
        }
        graphicsContext->UnloadPipelineResources();

    }

	void OrbitControllerTest::Exit()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer exit");
	}

	bool OrbitControllerTest::Paused()
	{
		return m_renderPause;
	}
}