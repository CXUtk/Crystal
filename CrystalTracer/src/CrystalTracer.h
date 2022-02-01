#pragma once
#include <CrystalEngine/src/Core/Application.h>
#include <CrystalEngine/src/Engine.h>
#include "Camera.h"

namespace tracer
{
	class CrystalTracer : public crystal::Application
	{
	public:
		CrystalTracer();
		virtual ~CrystalTracer() override;

		virtual void Initialize() override;

		virtual void Update(const crystal::GameTimer& gameTimer) override;

		virtual void Draw(const crystal::GameTimer& gameTimer) override;

		virtual void Exit() override;

		virtual bool Paused() override;

	private:
		bool m_renderPause = false;
		std::shared_ptr<crystal::IShaderProgram> m_pShader = nullptr;
		std::shared_ptr<Camera> m_pCamera = nullptr;
	};
}
