//#pragma once
//#include <Application.h>
//#include "Engine.h"
//#include "Camera.h"
//#include <Function/Framework/Framework.h>
//
//namespace crystal
//{
//	class OrbitControllerTest : public Application
//	{
//	public:
//		OrbitControllerTest();
//		virtual ~OrbitControllerTest() override;
//
//		virtual void Initialize() override;
//
//		virtual void Update(const crystal::GameTimer& gameTimer) override;
//
//		virtual void Draw(const crystal::GameTimer& gameTimer) override;
//
//		virtual void Exit() override;
//
//		virtual bool Paused() override;
//
//	private:
//		bool m_renderPause = false;
//
//        std::shared_ptr<GameObject>                 m_gameObject = nullptr;
//
//		std::shared_ptr<IShaderProgram>				m_pShader = nullptr;
//		std::shared_ptr<Camera>						m_pCamera = nullptr;
//		std::shared_ptr<IPipelineStateObject>		m_PSO = nullptr;
//		std::shared_ptr<ITexture2D>					m_texture2D = nullptr;
//		std::shared_ptr<IPipelineResourceObject>	m_PRO = nullptr;
//
//        std::shared_ptr<ITextureCubemap>            m_skyBox = nullptr;
//        std::shared_ptr<ITextureCubemap>            m_skyBoxIrradiance = nullptr;
//        std::shared_ptr<ITextureCubemap>            m_skyBoxPrefilter = nullptr;
//        std::shared_ptr<ITexture2D>                 m_skyBoxLUT = nullptr;
//        std::shared_ptr<ITexture2D>                 m_EavgTexture = nullptr;
//        std::shared_ptr<ITexture2D>                 m_EmuTexture = nullptr;
//
//        std::shared_ptr<IShaderProgram>				m_pSkyboxShader = nullptr;
//        std::shared_ptr<IPipelineResourceObject>    m_pSkyboxPRO = nullptr;
//        std::shared_ptr<IPipelineStateObject>       m_pSkyboxPSO = nullptr;
//
//        void RenderSkybox();
//        void InitSkybox();
//	};
//}
