#pragma once
#include <Core/Application.h>
#include "Engine.h"
#include "Camera.h"

namespace crystal
{
	class RenderTargetTest : public Application
	{
	public:
		RenderTargetTest();
		virtual ~RenderTargetTest() override;

		virtual void Initialize() override;

		virtual void Update(const crystal::GameTimer& gameTimer) override;

		virtual void Draw(const crystal::GameTimer& gameTimer) override;

		virtual void Exit() override;

		virtual bool Paused() override;

	private:
		bool m_renderPause = false;
		std::shared_ptr<IShaderProgram> m_pShader = nullptr;
		std::shared_ptr<IShaderProgram> m_pScreenShader = nullptr;
		std::shared_ptr<Camera> m_pCamera = nullptr;
		std::shared_ptr<IPipelineStateObject> m_PSO = nullptr;
		std::shared_ptr<IPipelineStateObject> m_PSOScreen = nullptr;
		std::shared_ptr<IPipelineResourceObject> m_PRO = nullptr;
		std::shared_ptr<IPipelineResourceObject> m_PROScreen = nullptr;
		std::shared_ptr<ITexture2D> m_texture2D = nullptr;
		std::shared_ptr<IRenderTarget2D> m_renderTarget2D = nullptr;
	};
}
