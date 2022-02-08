#pragma once
#include <Core/Application.h>
#include "Engine.h"
#include <Core/Utils/Math.h>
#include <Core/Platform/Graphics/GraphicsCommon.h>

namespace crystal
{
	class QuadTest : public Application
	{
	public:
		QuadTest();
		virtual ~QuadTest() override;

		virtual void Initialize() override;

		virtual void Update(const crystal::GameTimer& gameTimer) override;

		virtual void Draw(const crystal::GameTimer& gameTimer) override;

		virtual void Exit() override;

		virtual bool Paused() override;

	private:
		bool m_renderPause = false;
		std::shared_ptr<ShaderProgram> m_pShader = nullptr;
		std::shared_ptr<PipelineStateObject> m_PSO = nullptr;
		std::shared_ptr<crystal::Texture2D> m_texture2D = nullptr;
	};
}