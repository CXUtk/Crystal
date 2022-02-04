#pragma once
#include <Core/Application.h>
#include "Engine.h"
#include "Camera.h"

namespace crystal
{
	class OrbitControllerTest : public Application
	{
	public:
		OrbitControllerTest();
		virtual ~OrbitControllerTest() override;

		virtual void Initialize() override;

		virtual void Update(const crystal::GameTimer& gameTimer) override;

		virtual void Draw(const crystal::GameTimer& gameTimer) override;

		virtual void Exit() override;

		virtual bool Paused() override;

	private:
		bool m_renderPause = false;
		std::shared_ptr<crystal::ShaderProgram> m_pShader = nullptr;
		std::shared_ptr<Camera> m_pCamera = nullptr;
		std::shared_ptr<crystal::PipelineStateObject> m_PSO = nullptr;
	};
}