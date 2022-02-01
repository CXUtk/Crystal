#include "CrystalTracer.h"
#include <CrystalEngine/src/Engine.h>
#include <CrystalEngine/src/Core/Platform/Platforms.h>
#include <CrystalEngine/src/Core/Utils/Logger.h>
#include <CrystalEngine/src/Core/Utils/Geometry.h>
#include <CrystalEngine/src/Core/Input/InputController.h>
#include <CrystalEngine/src/Core/Utils/GameTimer.h>

#include <CrystalEngine/src/Core/Utils/Misc.h>
#include <CrystalEngine/src/Core/Utils/ObjLoader.h>

using namespace crystal;

namespace tracer
{
	CrystalTracer::CrystalTracer()
	{

	}
	
	
	CrystalTracer::~CrystalTracer()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer destruct");
	}

	static int indices;
	void CrystalTracer::Initialize()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer initialize");

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
			{ SemanticType::POSITION, 0, VertexElementFormat::Vector3, 0 },
			{ SemanticType::TEXCOORD, 0, VertexElementFormat::Vector2, 12 },
			{ SemanticType::NORMAL, 0, VertexElementFormat::Vector3, 20 },
		};
		VertexLayout vLayout(elements, sizeof(objloader::VertexData));
		auto graphicsDevice = m_engine->GetGraphicsDevice();

		VertexBufferDescription bufferDesc{};
		bufferDesc.Usage = BufferUsage::Immutable;
		IndexBufferDescription ibufferDesc{};
		ibufferDesc.Usage = BufferUsage::Immutable;
		ibufferDesc.Format = DataFormat::UInt32;

		auto vertexBuffer = graphicsDevice->CreateVertexBuffer(bufferDesc, 
			loader.Vertices.data(), sizeof(objloader::VertexData) * loader.Vertices.size());
		//auto indexBuffer = graphicsDevice->CreateIndexBuffer(ibufferDesc, 
		//	loader.Triangles.data(), sizeof(float) * 3 * loader.Triangles.size());
		indices = loader.Triangles.size() * 3;

		vertexBuffer->BindVertexLayout(vLayout);
		vertexBuffer->Bind(0);
		//indexBuffer->Bind(0);

		m_pShader = graphicsDevice->CreateShaderProgramFromFile("resources/model.json");
	}


	static Vector2f orbitControl = Vector2f(0.f, glm::half_pi<float>());
	static Point2i oldMousePos;
	void CrystalTracer::Update(const crystal::GameTimer& gameTimer)
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

	void CrystalTracer::Draw(const crystal::GameTimer& gameTimer)
	{
		auto windowSize = m_engine->GetWindow()->GetWindowSize();
		auto graphicsDevice = m_engine->GetGraphicsDevice();
		graphicsDevice->Clear(
			crystal::ClearOptions::Target 
			| crystal::ClearOptions::Stencil
			| crystal::ClearOptions::Depth,
			crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);
		//m_pShader->SetUniform1f("M", 0.5f + 0.5f * std::sin(gameTimer.GetLogicTime()));
		//m_pShader->SetUniform1f("uBase", 1.0f);
		m_pShader->SetUniformMat4f("M", glm::identity<Matrix4f>());
		m_pShader->SetUniformMat4f("MN", glm::identity<Matrix4f>());
		auto P = m_pCamera->GetProjectionMatrix();
		auto V = m_pCamera->GetViewMatrix();
		m_pShader->SetUniformMat4f("VP", P * V);
		m_pShader->Apply();
		graphicsDevice->DrawPrimitives(PrimitiveType::TRIANGLE_LIST, 0, indices);
	}

	void CrystalTracer::Exit()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer exit");
	}

	bool CrystalTracer::Paused()
	{
		return m_renderPause;
	}
}