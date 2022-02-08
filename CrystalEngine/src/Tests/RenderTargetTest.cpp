#include "RenderTargetTest.h"
#include <Engine.h>
#include <Core/Platform/Platforms.h>
#include <Core/Utils/Logger.h>
#include <Core/Utils/Geometry.h>
#include <Core/Input/InputController.h>
#include <Core/Utils/GameTimer.h>

#include <Core/Utils/Misc.h>
#include <Core/Utils/ObjLoader.h>

#ifdef CRYSTAL_USE_OPENGL
#include <Platforms/OpenGL/OpenGLPlatform.h>
#elif defined(CRYSTAL_USE_DX11)
#include <Platforms/DX11/DX11Platform.h>
#endif

namespace crystal
{
	RenderTargetTest::RenderTargetTest()
	{

	}


	RenderTargetTest::~RenderTargetTest()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "RenderTargetTest destruct");
	}

	struct Vertex
	{
		Vector3f pos;
		Vector2f texCoord;
		Vector4f color;
	};

	static int indices;
	void RenderTargetTest::Initialize()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "RenderTargetTest initialize");

		auto window = m_engine->GetWindow();
		auto windowSize = window->GetWindowSize();
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
		m_PSOScreen = graphicsDevice->CreatePipelineStateObject();
		auto vertexBuffer = graphicsDevice->CreateVertexBuffer(bufferDesc,
			loader.Vertices.data(), sizeof(objloader::VertexData) * loader.Vertices.size());
		//auto indexBuffer = graphicsDevice->CreateIndexBuffer(ibufferDesc, 
		//	loader.Triangles.data(), sizeof(float) * 3 * loader.Triangles.size());
		indices = loader.Triangles.size() * 3;
		vertexBuffer->BindVertexLayout(vLayout);

		m_pShader = graphicsDevice->CreateShaderProgramFromFile("resources/model.json");
		m_pScreenShader = graphicsDevice->CreateShaderProgramFromFile("resources/grayscale.json");

		Texture2DDescription texturedesc;
		texturedesc.Format = RenderFormat::RGBA8ub;
		texturedesc.MipmapLevels = 1;
		texturedesc.Usage = BufferUsage::Default;

		m_texture2D = graphicsDevice->CreateTexture2D("resources/result.png", texturedesc);

		Vertex vertices[] =
		{
			{ Vector3f(-0.5f, -0.5f, 0.5f), Vector2f(0.0f, 0.0f), Vector4f(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vector3f(0.5f, -0.5f, 0.5f), Vector2f(1.0f, 0.0f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f) },
			{ Vector3f(0.5f, 0.5f, 0.5f), Vector2f(1.0f, 1.0f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vector3f(-0.5f, 0.5f, 0.5f), Vector2f(0.0f, 1.0f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f) },
		};
		int indices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		std::vector<ElementDescription> elements1 = {
			{ SemanticType::POSITION, 0, RenderFormat::RGB32f, 0 },
			{ SemanticType::TEXCOORD, 0, RenderFormat::RGB32f, 12 },
			{ SemanticType::COLOR, 0, RenderFormat::RGBA32f, 24 },
		};
		VertexLayout vLayout1(elements1, sizeof(Vertex));

		VertexBufferDescription bufferDesc1{};
		bufferDesc1.Usage = BufferUsage::Immutable;
		IndexBufferDescription indexDesc1;
		indexDesc1.Format = DataFormat::UInt32;
		indexDesc1.Usage = BufferUsage::Immutable;
		auto indexBuffer1 = graphicsDevice->CreateIndexBuffer(indexDesc1, indices, sizeof(indices));
		auto vertexBuffer1 = graphicsDevice->CreateVertexBuffer(bufferDesc1, vertices, sizeof(vertices));
		vertexBuffer1->BindVertexLayout(vLayout1);


		RenderTarget2DDescription renderTargetDesc;
		renderTargetDesc.TargetFormat = RenderFormat::RGBA8ub;
		renderTargetDesc.RTFlags = RenderTargetFlags::CRYSTAL_TEXTURE_TARGET;
		renderTargetDesc.Size = windowSize;
		renderTargetDesc.MipmapLevels = 1;

		m_renderTarget2D = graphicsDevice->CreateRenderTarget2D(renderTargetDesc);


		m_PSO->BindVertexBuffer(vertexBuffer);
		m_PSO->BindShaderProgram(m_pShader);
		m_PSO->SetCullMode(CullingMode::CullCCW);
		m_PSO->SetFillMode(FillMode::SOLID);
		m_PSO->SetDepthTestState(true);
		m_PSO->BindShaderResource(m_texture2D, 0);
		m_PSO->BindSamplerState(SamplerState::GetSamplerState(SamplerStates::PointClamp), 0);
		//indexBuffer->Bind(0);


		m_PSOScreen->BindVertexBuffer(vertexBuffer1);
		m_PSOScreen->BindIndexBuffer(indexBuffer1);
		m_PSOScreen->BindShaderProgram(m_pScreenShader);
		m_PSOScreen->BindShaderResource(m_renderTarget2D, 0);
		m_PSOScreen->BindSamplerState(SamplerState::GetSamplerState(SamplerStates::PointClamp), 0);
	}


	static Vector2f orbitControl = Vector2f(-glm::half_pi<float>(), glm::half_pi<float>());
	static Point2i oldMousePos;
	void RenderTargetTest::Update(const crystal::GameTimer& gameTimer)
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

	void RenderTargetTest::Draw(const crystal::GameTimer& gameTimer)
	{
		auto windowSize = m_engine->GetWindow()->GetWindowSize();
		auto graphicsDevice = m_engine->GetGraphicsDevice();

		graphicsDevice->PushRenderTarget2D(m_renderTarget2D);
		{
			graphicsDevice->Clear(
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
			m_pShader->Apply();

			//graphicsDevice->SetPipelineStateObject(m_PSO);
			m_PSO->Begin();
			graphicsDevice->DrawPrimitives(PrimitiveType::TRIANGLE_LIST, 0, indices);
			m_PSO->End();
		}
		graphicsDevice->PopRenderTarget2D();

		graphicsDevice->Clear(
			crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
			| crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
			| crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
			crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);
		//graphicsDevice->SetPipelineStateObject(m_PSOScreen);
		m_PSOScreen->Begin();
		graphicsDevice->DrawIndexedPrimitives(PrimitiveType::TRIANGLE_LIST, 6, 0, 0);
		m_PSOScreen->End();
	}

	void RenderTargetTest::Exit()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "RenderTargetTest exit");
	}

	bool RenderTargetTest::Paused()
	{
		return m_renderPause;
	}
}