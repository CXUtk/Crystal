#include "TriangleTest.h"
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
	TriangleTest::TriangleTest()
	{

	}
	
	
	TriangleTest::~TriangleTest()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer destruct");
	}

	struct Vertex
	{
		Vector3f pos;
		Vector4f color;
	};

	static int indices;
	void TriangleTest::Initialize()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer initialize");

		auto window = m_engine->GetWindow();
		auto windowSize = window->GetWindowSize();
		m_pCamera = std::make_shared<Camera>(1.0f, windowSize.x / windowSize.y, 0.5f, 100.f);
		Vertex vertices[] =
		{
			{ Vector3f(0.0f, 0.5f, 0.5f), Vector4f(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vector3f(0.5f, -0.5f, 0.5f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f) },
			{ Vector3f(-0.5f, -0.5f, 0.5f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f) },
		};
		int indices[] = 
		{
			0, 1, 2
		};

		std::vector<ElementDescription> elements = {
			{ SemanticType::POSITION, 0, VertexElementFormat::Vector3, 0 },
			{ SemanticType::COLOR, 0, VertexElementFormat::Vector4, 12 },
		};
		VertexLayout vLayout(elements, sizeof(Vertex));
		auto graphicsDevice = m_engine->GetGraphicsDevice();

		VertexBufferDescription bufferDesc{};
		bufferDesc.Usage = BufferUsage::Immutable;
		IndexBufferDescription ibufferDesc{};
		ibufferDesc.Usage = BufferUsage::Immutable;
		ibufferDesc.Format = DataFormat::UInt32;

		m_PSO = graphicsDevice->CreatePipelineStateObject();
		auto vertexBuffer = graphicsDevice->CreateVertexBuffer(bufferDesc, 
			vertices, sizeof(vertices));
		auto indexBuffer = graphicsDevice->CreateIndexBuffer(ibufferDesc, 
			indices, sizeof(indices));

		m_pShader = graphicsDevice->CreateShaderProgramFromFile("resources/triangle.json");

		vertexBuffer->BindVertexLayout(vLayout);
		m_PSO->BindIndexBuffer(indexBuffer);
		m_PSO->BindVertexBuffer(vertexBuffer);
		m_PSO->BindShaderProgram(m_pShader);
		//indexBuffer->Bind(0);
	}


	static Vector2f orbitControl = Vector2f(0.f, glm::half_pi<float>());
	static Point2i oldMousePos;
	void TriangleTest::Update(const crystal::GameTimer& gameTimer)
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
		//if (inputController->IsKeyDowned(crystal::KeyCode::CRYSTAL_A_KEY))
		//{
		//	orbitControl.x += 0.01f;
		//	//m_renderPause = !m_renderPause;
		//	//printf("%s\n", m_renderPause ? "Paused" : "Resumed");
		//}
		//if (inputController->IsKeyDowned(crystal::KeyCode::CRYSTAL_D_KEY))
		//{
		//	orbitControl.x -= 0.01f;
		//	//printf("Time: %lf\n", gameTimer.GetLogicTime());
		//	//printf("Time: %lf\n", gameTimer.GetLogicalDeltaTime());
		//}

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

	void TriangleTest::Draw(const crystal::GameTimer& gameTimer)
	{
		auto windowSize = m_engine->GetWindow()->GetWindowSize();
		auto graphicsDevice = m_engine->GetGraphicsDevice();
		graphicsDevice->Clear(
			crystal::ClearOptions::Target 
			| crystal::ClearOptions::Stencil
			| crystal::ClearOptions::Depth,
			crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);
		m_pShader->SetUniform1f("uLuminance", 0.5f + 0.5f * std::sin(gameTimer.GetLogicTime()));
		m_pShader->SetUniform1f("uBase", 0.0f);
		m_pShader->Apply();
		
		graphicsDevice->SetPipelineStateObject(m_PSO);
		//graphicsDevice->DrawPrimitives(PrimitiveType::TRIANGLE_LIST, 0, 3);
		graphicsDevice->DrawIndexedPrimitives(PrimitiveType::TRIANGLE_LIST, 3, 0, 0);
	}

	void TriangleTest::Exit()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer exit");
	}

	bool TriangleTest::Paused()
	{
		return m_renderPause;
	}
}